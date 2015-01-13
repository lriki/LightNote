using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CSBuilder : CLBuilderEngine
    {
        string classesText;
        OutputBuffer _fieldsText = new OutputBuffer();
        OutputBuffer _propertiesText = new OutputBuffer();
        OutputBuffer _methodsText = new OutputBuffer();
        OutputBuffer _typeInfoRegistersText = new OutputBuffer(3);
        OutputBuffer _typeInfoPInvolesText = new OutputBuffer(2);

        public CSBuilder()
        {
            CommentReplaceItemList.Add(new CommentReplaceItem() { From = "NULL", To = "null" });
        }

        /// <summary>
        /// ドキュメントの置換要素を作る
        /// </summary>
        /// <param name="entity"></param>
        public override string MakeCommentConvertItem(CLEntity entity)
        {
            // enum
            var enumMember = entity as CLEnumMember;
            if (enumMember != null)
            {
                if (enumMember.Name == "LN_TRUE")
                    return "true";
                else if (enumMember.Name == "LN_FALSE")
                    return "false";
                else
                    return enumMember.Parent.Name + "." + enumMember.CapitalizedName;
            }

            // method
            var method = entity as CLMethod;
            if (method != null)
            {
                return method.OwnerClass.Name + "." + method.Name;
            }

            return null;
        }

        /// <summary>
        /// クラス・構造体出力 (前処理)
        /// </summary>
        public override bool PreClassOrStructDecl(CLClassType type)
        {
            // これらは自動定義しない
            if (type.Name == "Object") return false;
            if (type.Name == "ObjectList") return false;
            if (type.Name == "Exception") return false;

            // リセット
            _fieldsText = new OutputBuffer();
            _fieldsText.IncreaseIndent();
            _propertiesText = new OutputBuffer();
            _propertiesText.IncreaseIndent();
            _methodsText = new OutputBuffer();
            _methodsText.IncreaseIndent();

            return true;
        }

        /// <summary>
        /// クラス・構造体出力
        /// </summary>
        public override void MakeClassOrStructDecl(CLClassType type, string classBody)
        {
            // これらは独自定義する
            if (type.Name == "ReferenceObject" ||
                type.Name == "ObjectList")
                return;

            var output = new OutputBuffer();
            output.IncreaseIndent();

            // XMLコメント
            CSBuilderCommon.MakeSummaryXMLComment(output, AbjustComment(type.Documment.Summary));
            CSBuilderCommon.MakeRemarksXMLComment(output, AbjustComment(type.Documment.Detail));

            // 構造体
            if (type.IsStruct)
            {
                output.AppendWithIndent("[StructLayout(LayoutKind.Sequential)]").NewLine();
                output.AppendWithIndent("public struct {0}", type.Name);

                // 構造体用の定義を作る
                {
                    foreach (var memberDecl in type.StructMembers)
                    {
                        CSBuilderCommon.MakeSummaryXMLComment(_fieldsText, memberDecl.Comment);
                        _fieldsText.AppendWithIndent("public {0} {1};", CSBuilderCommon.ConvertTypeBasic(memberDecl.Type), memberDecl.Name).NewLine(2);
                    }

                    // コンストラクタ
                    CSBuilderCommon.MakeSummaryXMLComment(_fieldsText, "コンストラクタ");
                    _fieldsText.AppendWithIndent("public " + type.Name + "(");
                    foreach (var memberDecl in type.StructMembers)  // 仮引数 → (float x, float y) 等
                    {
                        if (memberDecl != type.StructMembers.First())
                            _fieldsText.Append(", ");
                        _fieldsText.Append(CSBuilderCommon.ConvertTypeBasic(memberDecl.Type) + " " + memberDecl.Name.ToLower());
                    }
                    _fieldsText.Append(")").NewLine();
                    _fieldsText.AppendWithIndent("{").NewLine();
                    _fieldsText.IncreaseIndent();
                    foreach (var memberDecl in type.StructMembers)  // フィールド格納 → X = x; Y = y; 等
                    {
                        _fieldsText.AppendWithIndent(memberDecl.Name + " = " + memberDecl.Name.ToLower() + ";").NewLine();
                    }
                    _fieldsText.DecreaseIndent();
                    _fieldsText.AppendWithIndent("}").NewLine();
                }
            }
            // クラス
            else
            {
                output.AppendWithIndent("public partial class {0}", type.Name);

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
                    _typeInfoRegistersText.AppendWithIndent("{0}_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));", C_APIAnalyzer.FuncPrefix + type.Name).NewLine();
                    // P/Imvoke
                    _typeInfoPInvolesText.AppendWithIndent("[DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]").NewLine();
                    _typeInfoPInvolesText.AppendWithIndent("private static extern void {0}_SetTypeUserData(IntPtr data);", C_APIAnalyzer.FuncPrefix + type.Name).NewLine(2);
                    // internal コンストラクタ
                    _methodsText.AppendWithIndent("internal {0}(_LNInternal i) : base(i) {{}}", type.Name).NewLine(2);
                }
            }

            // ベースクラス
            if (type.BaseType != null)
                output.Append(" : " + type.BaseType.Name);

            // class body
            output.NewLine();
            output.AppendWithIndent("{").NewLine();
            output.IncreaseIndent();
            output.AppendWithIndent(_fieldsText.ToString()).NewLine();      // フィールド
            output.AppendWithIndent(_propertiesText.ToString()).NewLine();  // プロパティ
            output.AppendWithIndent(_methodsText.ToString()).NewLine();     // メソッド
            output.DecreaseIndent();
            output.AppendWithIndent("};").NewLine(2);

            classesText += output.ToString();

        }

        /// <summary>
        /// プロパティ出力
        /// </summary>
        public override void MakeProperty(CLProperty prop)
        {
            // XMLコメント
            var xmlCommentText = new OutputBuffer();
            CSBuilderCommon.MakeSummaryXMLComment(xmlCommentText, AbjustComment(prop.SummaryDoc));
            CSBuilderCommon.MakeRemarksXMLComment(xmlCommentText, AbjustComment(prop.DetailDoc));

            // getter
            var getterText = new OutputBuffer();
            if (prop.Getter != null && prop.Getter.Option.CS_Enabled)
            {
                string getterBodyText;
                string getterFieldText;
                MakeMethodBodyText(prop.Getter, prop.Type, false, out getterBodyText, out getterFieldText);
                // アクセサ作成
                getterText.AppendWithIndent("get").NewLine();
                getterText.AppendWithIndent("{").NewLine();
                getterText.IncreaseIndent();
                getterText.AppendWithIndent(getterBodyText).NewLine();
                getterText.DecreaseIndent();
                getterText.AppendWithIndent("}").NewLine();
                // フィールド書き込み
                if (!string.IsNullOrEmpty(getterFieldText))
                    _fieldsText.AppendWithIndent(getterFieldText).NewLine();
            }
            // setter
            var setterText = new OutputBuffer();
            if (prop.Setter != null && prop.Setter.Option.CS_Enabled)
            {
                string setterBodyText;
                string setterFieldText;
                MakeMethodBodyText(prop.Setter, null, true, out setterBodyText, out setterFieldText);
                // アクセサ作成
                setterText.AppendWithIndent("set").NewLine();
                setterText.AppendWithIndent("{").NewLine();
                setterText.IncreaseIndent();
                setterText.AppendWithIndent(setterBodyText).NewLine();
                setterText.DecreaseIndent();
                setterText.AppendWithIndent("}").NewLine();
                // フィールド書き込み
                if (!string.IsNullOrEmpty(setterFieldText))
                    _fieldsText.AppendWithIndent(setterFieldText).NewLine();
            }

            if (getterText.IsEmpty && setterText.IsEmpty)   // 両方からだった場合はなにも出力しない
                return;

            // 修飾子
            var modifier = "public ";
            if (prop.IsStatic)
                modifier += "static ";
            // 修飾子 - override/virtual
            if (prop.IsVirtual)
            {
                if (prop.IsBaseVirtual)
                    modifier += "virtual ";
                else
                    modifier += "override ";
            }
            // 型名
            var typeName = CSBuilderCommon.ConvertTypeToName(prop.Type.Type, prop.Type.GenericTypeArg);
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
        /// メソッド出力
        /// </summary>
        public override void MakeMethod(CLMethod method, CLParam returnParam)
        {
            if (!method.Option.CS_Enabled ||                            // オプションで無効指定されている
                method.PropertyType != PropertyNameType.NotProperty)    // プロパティとしてマークされている
                return;

            string methodHeaderText;
            string xmlCommentText;
            MakeMethodHeaderText(method, returnParam, out methodHeaderText, out xmlCommentText);

            string bodyText;
            string fieldText;
            MakeMethodBodyText(method, returnParam, false, out bodyText, out fieldText);

            if (!string.IsNullOrEmpty(fieldText))
                _fieldsText.AppendWithIndent(fieldText).NewLine();

            var declText = new OutputBuffer();
            declText.Append(xmlCommentText);
            declText.Append(methodHeaderText).NewLine();
            declText.Append("{").NewLine();
            declText.IncreaseIndent();
            declText.AppendWithIndent(bodyText).NewLine();
            declText.DecreaseIndent();
            declText.Append("}").NewLine(2);

            _methodsText.AppendWithIndent(declText.ToString());
        }

        /// <summary>
        /// メソッドヘッダ定義の作成
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam"></param>
        /// <returns></returns>
        private void MakeMethodHeaderText(CLMethod method, CLParam returnParam, out string methodHeader, out string xmlComment)
        {
            // XMLコメント - summary
            var xmlCommentText = new OutputBuffer();
            CSBuilderCommon.MakeSummaryXMLComment(xmlCommentText, method.Summary);

            // 戻り値型の決定
            string returnTypeText = (method.Attribute == MethodAttribute.Constructor) ? "" : "void";
            if (returnParam != null)
                returnTypeText = CSBuilderCommon.ConvertTypeToName(returnParam.Type, returnParam.GenericTypeArg);   // out 等はつかないので名前変換のみでよい

            // 仮引数リストを作る
            var paramsText = new OutputBuffer();
            foreach (var param in method.Params)
            {
                // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                if (param == method.Params.First() &&
                    method.IsInstanceMethod)
                {
                    // 定義しない
                    continue;
                }
                else if (param != returnParam)   // return として選択されているものは無視
                {
                    // {ref/out} {型名} {引数名}
                    paramsText.AppendCommad("{0}{1} {2}", CSBuilderCommon.GetParamModifier(param), CSBuilderCommon.ConvertTypeToName(param.Type, param.GenericTypeArg), param.Name);
                    if (!string.IsNullOrEmpty(param.DefaultValueSource))   // デフォルト引数
                        paramsText.Append(" = " + CSBuilderCommon.ConvertLiteral(param.DefaultValueSource, false));

                    // XMLコメント - params
                    CSBuilderCommon.MakeParamXMLComment(xmlCommentText, param.Name, AbjustComment(param.Summary));
                }
            }

            // XMLコメント - return
            if (returnParam != null)
                MakeReturnXMLComment(xmlCommentText, returnParam.Summary);

            // XMLコメント - Remarks
            CSBuilderCommon.MakeRemarksXMLComment(xmlCommentText, AbjustComment(method.Detail));

            // 修飾子を決める
            var modifier = "public ";
            if (!method.IsInstanceMethod)
                modifier += "static ";

            // 修飾子 - override/virtual
            if (method.IsVirtual)
            {
                if (method.IsBaseVirtual)
                    modifier += "virtual ";
                else
                    modifier += "override ";
            }

            // メソッド名
            string methodName;
            if (method.Attribute == MethodAttribute.Constructor)  // コンストラクタである
                methodName = method.OwnerClass.Name;    // メソッド名にはクラス名を使用
            else
                methodName = method.Name;

            // {修飾子} {戻り値型名} {メソッド名} ({仮引数リスト})
            methodHeader = string.Format("{0}{1} {2}({3})", modifier, returnTypeText, methodName, paramsText.ToString());

            // コンストラクタの場合は base 追加
            if (method.Attribute == MethodAttribute.Constructor)
                methodHeader += " : base(_LNInternal.InternalBlock)";

            // XML コメント出力
            xmlComment = xmlCommentText.ToString();
        }

        /// <summary>
        /// メソッド本体の作成
        /// </summary>
        private void MakeMethodBodyText(CLMethod method, CLParam returnParam, bool isPropSetter, out string bodyText, out string fieldText)
        {
            fieldText = "";
            if (method.Option.CS_MethodOverride)
            {
                MakeOverrideMethodBody(method, out bodyText, out fieldText);
                return;
            }

            var initStmtText = new OutputBuffer();
            var argsText = new OutputBuffer();
            var postStmtText = new OutputBuffer();
            var returnStmtText = "";

            // Initializer の場合はファクトリ登録処理を呼び出す
            if (method.IsLibraryInitializer)
                initStmtText.AppendWithIndent("TypeInfo.Register();").NewLine();

            foreach (var param in method.Params)
            {
                // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                if (param == method.Params.First() &&
                    method.IsInstanceMethod)
                {
                    if (param.IsInReferenceObjectType)
                        argsText.Append("_handle");
                    else if (param.IsOutReferenceObjectType)
                        argsText.Append("out _handle");
                    else if (method.OwnerClass.IsStruct)
                        argsText.Append(string.Format("{0} this", CSBuilderCommon.GetAPIParamModifier(param)));  // struct instance

                }
                // return として選択された引数
                else if (param == returnParam)
                {
                    // string 型の return
                    if (param.IsOutString)
                    {
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
                    // 参照オブジェクト型の return
                    else if (param.IsOutReferenceObjectType)
                    {
                        #region 参照オブジェクト型の return
                        string typeName = CSBuilderCommon.ConvertTypeToName(param.Type, param.GenericTypeArg);
                        string fieldName = "_" + method.Name; // 現在プロパティ名を受け取っていないのでとりあえずこれを使う

                        // 初期化
                        initStmtText.AppendWithIndent("IntPtr {0};", param.Name).NewLine();
                        // 実引数
                        argsText.AppendCommad("out " + param.Name);
                        // 後処理
                        string format = @"
if ({0} == null) {{
    {1} = null;
}}
else if ({1} == null || {1}.Handle != {0}) {{
    {1} = new {2}(_LNInternal.InternalBlock);
    {1}.SetHandle({0});
}}
return {1};".Trim();
                        returnStmtText = string.Format(format, param.Name, fieldName, typeName);

                        string modifier = "";
                        if (!method.IsInstanceMethod)
                            modifier += "static";
                        fieldText += string.Format("{0} {1} {2};", modifier, typeName, fieldName) + OutputBuffer.NewLineCode;

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
                        initStmtText.AppendWithIndent("var {0} = new {1}();", param.Name, CSBuilderCommon.ConvertTypeToName(param.Type, param.GenericTypeArg)).NewLine();
                        // 実引数
                        argsText.AppendCommad("{0}{1}", CSBuilderCommon.GetAPIParamModifier(param), param.Name);
                        // return 文
                        returnStmtText = "return " + param.Name + ";";
                        #endregion
                    }
                }
                // 普通の引数
                else //if ((param.Attribute & ParamAttribute.In) != 0)
                {
                    string name = (isPropSetter) ? "value" : param.Name;    // setter の場合は名前を value にする
                    string exp = name;
                    if (param.IsInReferenceObjectType)
                        exp = "(" + name + " != null) ? " + name + ".Handle : default(IntPtr)";

                    argsText.AppendCommad("{0}{1}", CSBuilderCommon.GetAPIParamModifier(param), exp);
                }
            }

            // エラーコードと throw
            string preErrorStmt = "";
            string postErrorStmt = "";
            if (method.ReturnType.Type == CLPrimitiveType.ResultCode)
            {
                preErrorStmt = "var errorCode = ";
                postErrorStmt = "if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);" + OutputBuffer.NewLineCode;
            }

            // 定義文を結合して返す
            bodyText =
                initStmtText.ToString() +
                string.Format("{0}API.{1}({2});", preErrorStmt, method.CName, argsText.ToString()) + OutputBuffer.NewLineCode +
                postErrorStmt + 
                postStmtText.ToString() +
                returnStmtText;
        }

        public override string MakeOutoutFileText()
        {
            var template = GetTemplate("CSClassLib.txt");

            return template
                .Replace("CLASSES", classesText)
                .Replace("TYPE_INFO_REGISTERS", _typeInfoRegistersText.ToString())
                .Replace("TYPE_INFO_PINVOKES", _typeInfoPInvolesText.ToString());
        }

        /// <summary>
        /// return コメント作成
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="cppReturn"></param>
        public static void MakeReturnXMLComment(OutputBuffer buffer, string cppReturn)
        {
            if (!string.IsNullOrEmpty(cppReturn))
            {
                // "演算結果を格納する Vector3 変数" 等の後半を切り取る
                var r = new Regex("を格納する.*");
                buffer.AppendWithIndent(CSBuilderCommon.XMLCommentTemplate_Return.Replace("DETAIL", r.Replace(cppReturn, "")));
            }
        }

        /// <summary>
        /// メソッドの直接定義
        /// </summary>
        public void MakeOverrideMethodBody(CLMethod method, out string methodBodyText, out string fieldText)
        {
            //if (method.CName == "LManager_Initialize" ||
            //    method.CName == "LManager_InitializeFileIO" ||
            //    method.CName == "LManager_InitializeAudio" ||
            //    method.CName == "LManager_InitializeInput")
            //{
            //    stmtCall.AppendWithIndent("API.{0}();", func.CPPName).NewLine();
            //    stmtCall.AppendWithIndent("ReferenceObject.SystemInitialized = true;");
            //    return;
            //}
            //if (func.CPPName == "LManager_Terminate")
            //{
            //    stmtCall.AppendWithIndent("API.{0}();", func.CPPName).NewLine();
            //    stmtCall.AppendWithIndent("ReferenceObject.SystemInitialized = false;");
            //    return;
            //}
            if (method.CName == "LNVirtualPad_GetVirtualPad")
            {
                fieldText = "private static VirtualPad[] _pads = new VirtualPad[0];";
                methodBodyText = @"
                    if (_pads.Length < index + 1)
                    {
                        IntPtr handle;
                        API.LNVirtualPad_GetVirtualPad(index, out handle);
                        Array.Resize(ref _pads, index + 1);
                        _pads[index] = new VirtualPad(_LNInternal.InternalBlock);
                        _pads[index].SetHandle(handle);
                    }
                    return _pads[index];".Trim();
                return;
            }
            //            if (func.CPPName == "LViewPane_GetLayers")
            //            {
            //                fieldsText.AppendWithIndent("ObjectList<Layer> _layers;");
            //                stmtCall.AppendWithIndent(@"
            //                    IntPtr objectList;
            //                    API.LViewPane_GetLayers(_handle, out objectList);
            //                    if (_layers == null || _layers.Handle != objectList)
            //                    {
            //                        _layers = new ObjectList<Layer>();
            //                        _layers.SetHandle(objectList);
            //                    }
            //                    return _layers;".Trim());
            //            }

            // オーバーライド記述忘れ
            throw new InvalidOperationException("オーバーライド記述忘れ");
        }




        
    }



    
}
