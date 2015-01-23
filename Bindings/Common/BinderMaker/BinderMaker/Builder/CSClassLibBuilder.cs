using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class CSClassLibBuilder : Builder
    {
        #region Fields
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _classText;           // 現在のクラスのテキスト
        private OutputBuffer _fieldsText;          // 現在のクラスのフィールド一覧
        private OutputBuffer _methodsText;         // 現在のクラスのメソッド一覧
        private OutputBuffer _classesText = new OutputBuffer(1);
        private OutputBuffer _typeInfoRegistersText = new OutputBuffer(3);
        private OutputBuffer _typeInfoPInvolesText = new OutputBuffer(2);
        #endregion

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnClassLookedStart(CLClass type)
        {
            if (type.IsExtension) return;  // 拡張クラスはなにもしない

            _classText = new OutputBuffer();
            _fieldsText = new OutputBuffer();
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

                // フィールド
                foreach (var member in type.StructData.Members)
                {
                    CSBuilderCommon.MakeSummaryXMLComment(_fieldsText, member.Comment);
                    _fieldsText.AppendWithIndent("public {0} {1};", CSBuilderCommon.MakeTypeName(member.Type), member.Name).NewLine(2);
                }

                // デフォルトコンストラクタ (SlimDX は単位行列で初期化とかしない)
                CSBuilderCommon.MakeSummaryXMLComment(_methodsText, "すべての要素をデフォルト値 (0) で初期化します。");
                _methodsText.AppendWithIndent("public " + type.Name + "()").NewLine();
                _methodsText.AppendWithIndent("{").NewLine();
                _methodsText.IncreaseIndent();
                foreach (var member in type.StructData.Members)
                    _methodsText.AppendWithIndent("{0} = default({1});", member.Name, CSBuilderCommon.MakeTypeName(member.Type)).NewLine();
                _methodsText.DecreaseIndent();
                _methodsText.AppendWithIndent("}").NewLine();
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


                // 構造体用の定義を作る
                {
                    

                    //// コンストラクタ
                    //CSBuilderCommon.MakeSummaryXMLComment(_fieldsText, "コンストラクタ");
                    //_fieldsText.AppendWithIndent("public " + type.Name + "(");
                    //foreach (var memberDecl in type.StructMembers)  // 仮引数 → (float x, float y) 等
                    //{
                    //    if (memberDecl != type.StructMembers.First())
                    //        _fieldsText.Append(", ");
                    //    _fieldsText.Append(CSBuilderCommon.ConvertTypeBasic(memberDecl.Type) + " " + memberDecl.Name.ToLower());
                    //}
                    //_fieldsText.Append(")").NewLine();
                    //_fieldsText.AppendWithIndent("{").NewLine();
                    //_fieldsText.IncreaseIndent();
                    //foreach (var memberDecl in type.StructMembers)  // フィールド格納 → X = x; Y = y; 等
                    //{
                    //    _fieldsText.AppendWithIndent(memberDecl.Name + " = " + memberDecl.Name.ToLower() + ";").NewLine();
                    //}
                    //_fieldsText.DecreaseIndent();
                    //_fieldsText.AppendWithIndent("}").NewLine();
                }
            }
            // クラス
            else
            {
                return;
            }

            // ベースクラス
            //if (type.BaseType != null)
            //    output.Append(" : " + type.BaseType.Name);

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
            //_classText.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            _classText.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            _classText.DecreaseIndent();
            _classText.AppendWithIndent("};").NewLine(2);

            // 全クラステキストへ
            _classesText.AppendWithIndent(_classText.ToString());
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
        /// メソッド 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnMethodLooked(CLMethod method)
        {
            // internal は出力しない
            if (method.Modifier == MethodModifier.Internal) return;

            // XML コメントと メソッドヘッダを作る
            MakeMethodHeaderText(method);

            // メソッド本体を作る
            MakeMethodBodyText(method, false);
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
            string returnTypeText = CSBuilderCommon.MakeTypeName(method.ReturnType);

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
            if (method.IsConstructor)
                methodHeader += " : base(_LNInternal.InternalBlock)";

            // 出力
            _methodsText.AppendWithIndent(xmlCommentText.ToString());
            _methodsText.AppendWithIndent(methodHeader);
            _methodsText.NewLine();
        }

        /// <summary>
        /// メソッド本体の作成 (開始と終端の '{''}' を含み、_methodsText、_fieldsText に追記する)
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam"></param>
        /// <param name="isPropSetter"></param>
        private void MakeMethodBodyText(CLMethod method, bool isPropSetter)
        {
            _methodsText.AppendWithIndent("{").NewLine();
            _methodsText.IncreaseIndent();

            // オーバーライドコードがあればそれを使う
            string overrideCode;
            if (_context.TryGetOverrideCode(method, out overrideCode))
            {
                _methodsText.AppendWithIndent(overrideCode);
            }
            else
            {
                var initStmtText = new OutputBuffer();  // API 呼び出し前の処理
                var argsText = new OutputBuffer();      // API 実引数
                var postStmtText = new OutputBuffer();  // API 呼び出し後の処理
                var returnStmtText = "";                // return 文

                // Initializer の場合はファクトリ登録処理を埋め込む
                if (method.IsLibraryInitializer)
                    initStmtText.AppendWithIndent("TypeInfo.Register();").NewLine();

                // this を表す先頭の仮引数がある
                if (method.ThisParam != null)
                {
                    string io = CSBuilderCommon.GetAPIParamIOModifier(method.ThisParam);    
                    CLClass c = method.ThisParam.Type as CLClass;
                    if (c != null && c.IsReferenceObject)
                        argsText.Append(string.Format("{0} _handle", io));  // RefClass instance
                    else
                        argsText.Append(string.Format("{0} this", io));     // struct instance
                }
            }

            _methodsText.DecreaseIndent();
            _methodsText.AppendWithIndent("}").NewLine(2);
        }
    }
}
