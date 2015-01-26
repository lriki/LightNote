using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * ■ RefObject を返すメソッドは、フィールドにインスタンスを持たせておくべき？  
 *      前は持たせていた。しかし、今回からグローバルな管理配列を使うので必要なくなる。
 */

namespace BinderMaker.Builder
{
    class CSClassLibBuilder : Builder
    {
        #region Fields
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _classText;           // 現在のクラスのテキスト
        private OutputBuffer _fieldsText;          // 現在のクラスのフィールド一覧
        private OutputBuffer _propertiesText;       // 現在のクラスのプロパティ一覧
        private OutputBuffer _methodsText;         // 現在のクラスのメソッド一覧
        private OutputBuffer _classesText = new OutputBuffer(1);
        private OutputBuffer _typeInfoRegistersText = new OutputBuffer(3);
        private OutputBuffer _typeInfoPInvolesText = new OutputBuffer(2);
        #endregion

        /// <summary>
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnInitialize()
        {
            // bool コメント置換
            _context.AddReplacePair("LN_TRUE", "true");
            _context.AddReplacePair("LN_FALSE", "false");

            // enum メンバのコメント置換
            foreach (var t in Manager.AllEnums)
                foreach (var m in t.Members)
                    _context.AddReplacePair(m.OriginalName, t.Name + "." + m.CapitalizedName);
            
        }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected override bool OnClassLookedStart(CLClass type)
        {
            if (type.IsExtension) return false;  // 拡張クラスはなにもしない

            _classText = new OutputBuffer();
            _fieldsText = new OutputBuffer();
            _propertiesText = new OutputBuffer();
            _methodsText = new OutputBuffer();

            // XMLコメント
            CSBuilderCommon.MakeSummaryXMLComment(_classText, _context.GetBriefText(type));
            CSBuilderCommon.MakeRemarksXMLComment(_classText, _context.GetDetailsText(type));

            // 構造体
            if (type.IsStruct)
            {
                // StructLayout 属性
                _classText.AppendWithIndent("[StructLayout(LayoutKind.Sequential)]").NewLine();
                // 構造体ヘッダ
                _classText.AppendWithIndent("public struct {0}", type.Name);
                // 構造体のメンバの数だけ public フィールドを作る
                foreach (var member in type.StructData.Members)
                {
                    CSBuilderCommon.MakeSummaryXMLComment(_fieldsText, member.Comment);
                    _fieldsText.AppendWithIndent("public {0} {1};", CSBuilderCommon.MakeTypeName(member.Type), member.Name).NewLine(2);
                }

#if false       // 構造体のデフォルトコンストラクタは、コンストラクタをオーバーロードする場合、明示的に定義できない
                // デフォルトコンストラクタ (SlimDX は単位行列で初期化とかしない)
                #region 出力例
                /*
                    /// <summary>
                    /// コンストラクタ
                    /// </summary>
                    public Vector3()
                    {
                        X = default(float);
                        Y = default(float);
                        Z = default(float);
                    }
                */
                #endregion
                CSBuilderCommon.MakeSummaryXMLComment(_methodsText, "すべての要素をデフォルト値 (0) で初期化します。");
                _methodsText.AppendWithIndent("public " + type.Name + "()").NewLine(); // メソッドヘッダ
                _methodsText.AppendWithIndent("{").NewLine();
                _methodsText.IncreaseIndent();
                foreach (var member in type.StructData.Members) // 代入式
                    _methodsText.AppendWithIndent("{0} = default({1});", member.Name, CSBuilderCommon.MakeTypeName(member.Type)).NewLine();
                _methodsText.DecreaseIndent();
                _methodsText.AppendWithIndent("}").NewLine(2);
#endif

                // 要素指定のコンストラクタ
                #region 出力例
                /*
                    /// <summary>
                    /// 各要素を指定して初期化します。
                    /// </summary>
                    public Rect(int x, int y, int width, int height)
                    {
                        X = x;
                        Y = y;
                        Width = width;
                        Height = height;
                    }
                */
                #endregion
                CSBuilderCommon.MakeSummaryXMLComment(_methodsText, "各要素を指定して初期化します。");
                _methodsText.AppendWithIndent("public " + type.Name + "("); // メソッドヘッダ
                var paramsText = new OutputBuffer();
                foreach (var member in type.StructData.Members)  // 仮引数リスト → (float x, float y) 等
                    paramsText.AppendCommad(CSBuilderCommon.MakeTypeName(member.Type) + " " + member.Name.ToLower());
                _methodsText.Append(paramsText).Append(")").NewLine();
                _methodsText.AppendWithIndent("{").NewLine().IncreaseIndent();
                foreach (var member in type.StructData.Members)  // フィールド格納 代入式 → X = x; Y = y; 等
                    _methodsText.AppendWithIndent(member.Name + " = " + member.Name.ToLower() + ";").NewLine();
                _methodsText.DecreaseIndent().AppendWithIndent("}").NewLine(2);
            }
            // クラス
            else
            {
                // クラスヘッダ
                _classText.AppendWithIndent("public partial class {0}", type.Name);

                // ReferenceObject なら型情報を作る
                if (type.IsReferenceObject)
                {
                    // ファクトリ
                    _typeInfoRegistersText.AppendWithIndent(@"
var _{0} = new TypeInfo(){{ Factory = (handle) =>
    {{
        var obj = new {0}(_LNInternal.InternalBlock);
        obj.SetHandle(handle);
        return obj;
    }}
}};", type.Name).NewLine();
                    _typeInfoRegistersText.AppendWithIndent("_typeInfos.Add(_{0});", type.Name).NewLine();
                    _typeInfoRegistersText.AppendWithIndent("{0}_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));", type.OriginalName).NewLine();
                    // P/Invoke
                    _typeInfoPInvolesText.AppendWithIndent("[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]").NewLine();
                    _typeInfoPInvolesText.AppendWithIndent("private static extern void {0}_SetTypeUserData(IntPtr data);", type.OriginalName).NewLine(2);
                    // internal コンストラクタ
                    _methodsText.AppendWithIndent("internal {0}(_LNInternal i) : base(i) {{}}", type.Name).NewLine(2);
                }
            }

            // ベースクラス
            if (type.BaseClass != null)
                _classText.Append(" : " + type.BaseClass.Name);

            return true;
        }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedEnd(CLClass classType)
        {
            // class body
            _classText.NewLine();
            _classText.AppendWithIndent("{").NewLine();
            _classText.IncreaseIndent();
            _classText.AppendWithIndent(_fieldsText.ToString()).NewLine();      // フィールド
            _classText.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            _classText.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            _classText.DecreaseIndent();
            _classText.AppendWithIndent("};").NewLine(2);

            // 全クラステキストへ
            _classesText.AppendWithIndent(_classText.ToString());
        }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnPropertyLooked(CLProperty prop)
        {
            // XMLコメント
            var xmlCommentText = new OutputBuffer();
            CSBuilderCommon.MakeSummaryXMLComment(xmlCommentText, _context.GetBriefText(prop));
            CSBuilderCommon.MakeRemarksXMLComment(xmlCommentText, _context.GetDetailsText(prop));

            // getter
            var getterText = new OutputBuffer();
            if (prop.Getter != null && _context.CheckEnabled(prop.Getter))
            {
                // アクセサ作成
                getterText.AppendWithIndent("get").NewLine();
                MakeMethodBodyText(prop.Getter, false, getterText, _fieldsText);    // {} はこれに含まれる
            }
            // setter
            var setterText = new OutputBuffer();
            if (prop.Setter != null && _context.CheckEnabled(prop.Setter))
            {
                // アクセサ作成
                setterText.AppendWithIndent("set").NewLine();
                MakeMethodBodyText(prop.Setter, true, setterText, _fieldsText);    // {} はこれに含まれる
            }

            // 修飾子
            var modifier = "public ";
            if (prop.IsStatic) modifier += "static ";

            // 修飾子 - override/virtual
            //if (prop.IsVirtual)
            //{
            //    if (prop.IsBaseVirtual)
            //        modifier += "virtual ";
            //    else
            //        modifier += "override ";
            //}
            // 型名
            var typeName = CSBuilderCommon.MakeTypeName(prop.Type);
            // 結合
            _propertiesText.AppendWithIndent(xmlCommentText.ToString());
            _propertiesText.AppendWithIndent("{0}{1} {2}", modifier, typeName, prop.Name).NewLine();
            _propertiesText.AppendWithIndent("{").NewLine();
            _propertiesText.IncreaseIndent();
            _propertiesText.AppendWithIndent(getterText.ToString());
            _propertiesText.AppendWithIndent(setterText.ToString());
            _propertiesText.DecreaseIndent();
            _propertiesText.AppendWithIndent("}").NewLine(2);
        }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            // XML コメントと メソッドヘッダを作る
            MakeMethodHeaderText(method);

            // メソッド本体を作る
            MakeMethodBodyText(method, false, _methodsText, _fieldsText);
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            var template = GetTemplate("CSClassLib.txt");
            return template
                .Replace("CLASSES", _classesText.ToString())
                .Replace("TYPE_INFO_REGISTERS", _typeInfoRegistersText.ToString())
                .Replace("TYPE_INFO_PINVOKES", _typeInfoPInvolesText.ToString());
        }

        /// <summary>
        /// メソッドヘッダ定義の作成
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam"></param>
        /// <returns></returns>
        private void MakeMethodHeaderText(CLMethod method)
        {
            // XMLコメント - summary
            var xmlCommentText = new OutputBuffer();
            CSBuilderCommon.MakeSummaryXMLComment(xmlCommentText, _context.GetBriefText(method));

            // 戻り値型
            string returnTypeText = "";
            if (!method.IsRefObjectConstructor)  // コンストラクタ以外は戻り値型名を作る
                returnTypeText = CSBuilderCommon.MakeTypeName(method.ReturnType);
            
            // 仮引数リストを作る
            var paramsText = new OutputBuffer();
            foreach (var param in method.Params)
            {
                // {ref/out} {型名} {引数名}
                paramsText.AppendCommad("{0} {1} {2}", CSBuilderCommon.GetParamIOModifier(param), CSBuilderCommon.MakeTypeName(param.Type), param.Name);
                if (!string.IsNullOrEmpty(param.OriginalDefaultValue))   // デフォルト引数
                    paramsText.Append(" = " + CSBuilderCommon.ConvertLiteral(param.OriginalDefaultValue, false));

                // XMLコメント - params
                CSBuilderCommon.MakeParamXMLComment(xmlCommentText, param.Name, _context.GetParamText(param));
            }

            // XMLコメント - return
            CSBuilderCommon.MakeReturnXMLComment(xmlCommentText, _context.GetReturnParamText(method.ReturnParam));
            // XMLコメント - Remarks
            CSBuilderCommon.MakeRemarksXMLComment(xmlCommentText, _context.GetDetailsText(method));

            // 修飾子を決める
            var modifier = "public ";
            if (method.IsStatic)
                modifier += "static ";

            // {修飾子} {戻り値型名} {メソッド名} ({仮引数リスト})
            string methodHeader = string.Format("{0}{1} {2}({3})", modifier, returnTypeText, method.Name, paramsText.ToString());

            // コンストラクタの場合は base 追加
            if (method.IsRefObjectConstructor)
                methodHeader += " : base(_LNInternal.InternalBlock)";

            // 出力
            _methodsText.AppendWithIndent(xmlCommentText.ToString());
            _methodsText.AppendWithIndent(methodHeader);
            _methodsText.NewLine();
        }

        /// <summary>
        /// メソッド本体の作成
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam"></param>
        /// <param name="isPropSetter"></param>
        private void MakeMethodBodyText(CLMethod method, bool isPropSetter, OutputBuffer methodsOutput, OutputBuffer fieldsOutput)
        {
            methodsOutput.AppendWithIndent("{").NewLine();
            methodsOutput.IncreaseIndent();

            // オーバーライドコードがあればそれを使う
            string overrideCode;
            if (_context.TryGetOverrideCode(method, out overrideCode))
            {
                methodsOutput.AppendWithIndent(overrideCode);
            }
            else
            {
                var initStmtText = new OutputBuffer();  // API 呼び出し前の処理
                var argsText = new OutputBuffer();      // API 実引数
                var postStmtText = new OutputBuffer();  // API 呼び出し後の処理
                var returnStmtText = "";                // return 文

                // Initializer の場合はファクトリ登録処理を埋め込む
                if (method.IsLibraryInitializer)
                    initStmtText.AppendWithIndent("InternalManager.Initialize();").NewLine();

                // this を表す先頭の仮引数がある
                if (method.ThisParam != null)
                {
                    var param = method.ThisParam;
                    string io = CSBuilderCommon.GetAPIParamIOModifier(param);    
                    if (CLType.CheckRefObjectType(param.Type))
                    {
                        if (method.IsRefObjectConstructor)
                        {
                            // コンストラクタの場合は、Handle を一度ローカル変数に受け取り、
                            // 最後に Internalmanager.RegisterWrapperObject で this にセットしつつ管理リストに登録する。
                            initStmtText.AppendWithIndent("IntPtr {0};", param.Name).NewLine();
                            argsText.Append(string.Format("{0} {1}", io, param.Name));  // RefClass instance
                            postStmtText.AppendWithIndent("InternalManager.RegisterWrapperObject(this, {0});", param.Name).NewLine();
                        }
                        else
                            argsText.Append(string.Format("{0} _handle", io));  // RefClass instance
                    }
                    else
                        argsText.Append(string.Format("{0} this", io));     // struct instance
                }

                // 普通の引数
                foreach (var param in method.Params)
                {
                    string name = (isPropSetter) ? "value" : param.Name;    // setter の場合は名前を value にする
                    string exp = name;
                    if (CLType.CheckRefObjectType(param.Type))   // RefObj の場合は .Handle から取得する
                        exp = "(" + name + " != null) ? " + name + ".Handle : default(IntPtr)";

                    argsText.AppendCommad("{0} {1}", CSBuilderCommon.GetAPIParamIOModifier(param), exp);
                }

                // return として選択された引数
                if (method.ReturnParam != null)
                {
                    var param = method.ReturnParam;
                    // string 型の return
                    if (param.IsOutStringType)
                    {
                        // まずは文字列の長さを取得しメモリ確保、その後コピーする。
                        #region string 型の return
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("IntPtr strPtr;").NewLine();
                        // 実引数
                        argsText.AppendCommad("out strPtr");
                        // 後処理
                        returnStmtText += @"
int len;
API.LCSInternal_GetIntPtrStringLength(strPtr, out len);
var sb = new StringBuilder(len + 1);
API.LCSInternal_GetIntPtrString(strPtr, sb);
return sb.ToString();".Trim();
                        #endregion
                    }
                    // 参照オブジェクト型の return (参照オブジェクト型の return は、自動生成ではプロパティのみ可能としている)
                    else if (param.IsOutRefObjectType)
                    {
                        #region 参照オブジェクト型の return
                        // やっていることは、
                        // ・Wrap 側に未登録のオブジェクトなら登録する
                        // ・return するオブジェクトを保持するフィールドの作成
                        string typeName = CSBuilderCommon.MakeTypeName(param.Type);
                        string fieldName = "_" + method.Name; // 現在プロパティ名を受け取っていないのでとりあえずこれを使う

                        // out 値を受け取る変数
                        initStmtText.AppendWithIndent("IntPtr {0};", param.Name).NewLine();
                        // 実引数
                        argsText.AppendCommad("out " + param.Name);
                        // 後処理
                        string format = @"
{0} = IntenalManager.GetWrapperObject<{1}>({2});
return {0};".Trim();
                        returnStmtText = string.Format(format, fieldName, typeName, param.Name);

                        // ラップオブジェクトを保持するメンバを追加
                        string modifier = "";
                        if (!method.IsInstanceMethod)
                            modifier += "static";
                        fieldsOutput.AppendWithIndent(string.Format("{0} {1} {2};", modifier, typeName, fieldName)).NewLine();

                        /* 例)
                        IntPtr viewPane;
                        API.LViewPane_GetDefaultViewPane(out viewPane);
                        if (viewPane == null) {
                            _viewPane = null;
                        }
                        else if (_viewPane == null || _viewPane.Handle != viewPane) {
                            _viewPane = new ViewPane();
                            _viewPane._handle = viewPane;
                        }
                        return _viewPane;
                        */
                        #endregion
                    }
                    // その他の型の return
                    else
                    {
                        #region その他の型の return
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("var {0} = new {1}();", param.Name, CSBuilderCommon.MakeTypeName(param.Type)).NewLine();
                        // 実引数
                        argsText.AppendCommad("{0} {1}", CSBuilderCommon.GetAPIParamIOModifier(param), param.Name);
                        // return 文
                        returnStmtText = "return " + param.Name + ";" + OutputBuffer.NewLineCode;
                        #endregion
                    }
                }

                // エラーコードと throw
                string preErrorStmt = "";
                string postErrorStmt = "";
                if (method.FuncDecl.ReturnType == Manager.ResultEnumType)
                {
                    preErrorStmt = "var result = ";
                    postErrorStmt = "if (result != Result.OK) throw new LNoteException(result);" + OutputBuffer.NewLineCode;
                }

                // Terminator の場合は終了処理を埋め込む
                if (method.IsLibraryTerminator)
                    postStmtText.AppendWithIndent("InternalManager.Terminate();").NewLine();

                // 定義文を結合
                methodsOutput.AppendWithIndent(
                    initStmtText.ToString() +
                    string.Format("{0}API.{1}({2});", preErrorStmt, method.FuncDecl.OriginalFullName, argsText.ToString()) + OutputBuffer.NewLineCode +
                    postErrorStmt +
                    postStmtText.ToString() +
                    returnStmtText);
            }

            methodsOutput.DecreaseIndent();
            methodsOutput.AppendWithIndent("}").NewLine(2);
        }
    }
}
