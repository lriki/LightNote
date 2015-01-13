using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// P/Invole ソース
    /// </summary>
    class CSPInvokeBuilder : CLBuilderEngine
    {
        #region Templates
        const string FuncDeclTempalte = @"
[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
public extern static RETURN_TYPE FUNC_NAME(ARGS);
";
        const string APISourceTemplate = @"
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ErrorCode errorCode);

ENUM_LIST
    public class API
    {
    
#if DEBUG
    	internal const string DLLName = ""LNote.Debug.dll"";
#else
    	internal const string DLLName = ""LNote.dll"";
#endif
        internal const CharSet DLLCharSet = CharSet.Ansi;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
API_LIST

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
";
        #endregion

        private OutputBuffer _enumText = new OutputBuffer();
        private OutputBuffer _funcsText = new OutputBuffer();

        /// <summary>
        /// ドキュメントの置換要素を作る
        /// </summary>
        /// <param name="entity"></param>
        public override string MakeCommentConvertItem(CLEntity entity)
        {
            return null;
        }

        public override bool PreClassOrStructDecl(CLClassType type)
        {
            return true;
        }

        public override void MakeClassOrStructDecl(CLClassType type, string body)
        {
        }

        public override void MakeProperty(CLProperty prop)
        {
        }

        /// <summary>
        /// 関数定義
        public override void MakeMethod(CLMethod method, CLParam returnParam)
        {
            // この関数は定義しない
            if (method.IsCopy)
                return;

            _funcsText.IncreaseIndent();

            // XML コメント
            CSBuilderCommon.MakeSummaryXMLComment(_funcsText, method.Summary);
            foreach (var param in method.Params)
            {
                CSBuilderCommon.MakeParamXMLComment(_funcsText, param.Name, param.Summary);
            }
            // TODO:returnコメント
            //CSBuilderCommon.MakeReturnXMLComment(method);
            CSBuilderCommon.MakeRemarksXMLComment(method.Detail);

            // DLLImport・型名・関数名
            string declText = FuncDeclTempalte.Trim();
            declText = declText.Replace("RETURN_TYPE", CSBuilderCommon.ConvertTypeToName(method.ReturnType.Type, null));
            declText = declText.Replace("FUNC_NAME", method.CName);

            // 仮引数リスト
            string argsText = "";
            foreach (var param in method.Params)
            {
                if (argsText != "") argsText += ", ";

                // 型名と引数名
                argsText += string.Format("{0} {1}", ConvertParamTypeName(param), param.Name);

                // デフォルト引数
                if (!string.IsNullOrEmpty(param.DefaultValueSource))
                    argsText += " = " + CSBuilderCommon.ConvertLiteral(param.DefaultValueSource, true);
            }
            declText = declText.Replace("ARGS", argsText);

            _funcsText.AppendWithIndent(declText).NewLine(2);
            _funcsText.DecreaseIndent();
        }

        /// <summary>
        /// enum 出力 (後処理)
        /// </summary>
        public override void PostMakeEnumDecl(CLEnumType enumType)
        {
            if (enumType.CName == "lnBool")
            {
                Console.WriteLine("lnBool は enum として出力しない");
                return;
            }
            _enumText.IncreaseIndent();

            // [Flags] 属性
            if (enumType.IsFlags)
                _enumText.AppendWithIndent("[Flags]\n");

            // enum ヘッダ
            _enumText.AppendWithIndent("public enum " + enumType.Name + "\n");
            _enumText.AppendWithIndent("{\n");
            _enumText.IncreaseIndent();

            // 各メンバ
            foreach (var member in enumType.Members)
            {
                // LN_xxxx_MAX のようなターミネータは出力しない
                if (member.IsTerminator) continue;

                // XML コメント
                CSBuilderCommon.MakeSummaryXMLComment(_enumText, member.Comment);

                // メンバ定義
                _enumText.AppendWithIndent("{0} = {1},", member.CapitalizedName, member.Value).NewLine(2);
            }

            _enumText.DecreaseIndent();
            _enumText.AppendWithIndent("}\n\n");
            _enumText.DecreaseIndent();
        }

        public override string MakeOutoutFileText()
        {
            string output = APISourceTemplate.Replace("ENUM_LIST", _enumText.ToString());
            output = output.Replace("API_LIST", _funcsText.ToString());
            return output;
        }

        /// <summary>
        /// PInvoke 用の型変換 (仮引数用)
        /// </summary>
        /// <returns></returns>
        private string ConvertParamTypeName(CLParam param)
        {
            if ((param.Type is CLClassType && ((CLClassType)param.Type).IsReferenceObject) ||
                (param.Type == CLPrimitiveType.Generic) ||
                (param.Type == CLPrimitiveType.String && param.Attribute == ParamAttribute.Out))
                return string.Format("{0}IntPtr", CSBuilderCommon.GetAPIParamModifier(param));
            else
                return string.Format("{0}{1}", CSBuilderCommon.GetAPIParamModifier(param), CSBuilderCommon.ConvertTypeToName(param.Type, null));
        }
    }


#if false
    static class CSCommon
    {
        /// <summary>
        /// C# 向け型追加情報
        /// </summary>
        public struct TypeInfo
        {
            public string CSType;
            public string APIModifier;

            public TypeInfo(string csType, string apiModifier)
            {
                CSType = csType;
                APIModifier = apiModifier;
            }
        }

        /// <summary>
        /// C# 向け型追加情報
        /// </summary>
        public static Dictionary<string, TypeInfo> _typeInfoTable = new Dictionary<string, TypeInfo>()
        {
            // C++ Type             C# Type                     API仮引数型修飾子
            { "lnHandle",           new TypeInfo("IntPtr",      "") },
            { "lnHandle*",          new TypeInfo("IntPtr",      "out") },
            { "void",               new TypeInfo("void",        "") },
            { "void*",              new TypeInfo("byte[]",      "") },
            { "void**",             new TypeInfo("byte[]",      "out") },
            { "const void**",       new TypeInfo("IntPtr",      "out") },
            { "const char*",        new TypeInfo("string",      "") },
            { "const TCHAR*",       new TypeInfo("string",      "") },
            { "const lnChar*",      new TypeInfo("string",      "") },
            { "const lnChar**",     new TypeInfo("IntPtr",      "out") },
            { "int",                new TypeInfo("int",         "") },
            { "float",              new TypeInfo("float",       "") },
            { "double",             new TypeInfo("double",      "") },
            { "int*",               new TypeInfo("int",         "out") },
            { "float*",             new TypeInfo("float",       "out") },
            { "lnBool",             new TypeInfo("bool",        "") },
            { "lnBool*",            new TypeInfo("bool",        "out") },
            { "lnByte*",            new TypeInfo("byte[]",      "") },
            { "const lnByte*",      new TypeInfo("byte[]",      "") },
            { "const void*",        new TypeInfo("byte[]",      "") },
            { "lnU8",               new TypeInfo("byte",        "") },
            { "lnU32",              new TypeInfo("uint",        "") },
            { "lnResult",           new TypeInfo("int",         "") },
            { "lnExceptionCallback",new TypeInfo("ExceptionCallback",         "") },
            { "HWND",               new TypeInfo("IntPtr",         "") },
        };

        #region Templates
        public const string XMLCommentTemplate_Summary = "/// <summary>\n/// SUMMARY\n/// </summary>\n";
        public const string XMLCommentTemplate_Param = "/// <param name=\"PARAM_NAME\">DETAIL</param>\n";
        public const string XMLCommentTemplate_Return = "/// <returns>DETAIL</returns>\n";
        #endregion

        public static C_APIAnalyzer Analyzer;

        /// <summary>
        /// 型名の変換
        /// </summary>
        /// <returns></returns>
        public static string ConvertTypeBasic(string cppFullTypeName)
        {
            // テーブルにあればそれを使う
            if (_typeInfoTable.ContainsKey(cppFullTypeName))
                return _typeInfoTable[cppFullTypeName].CSType;

            // enum 型であるか
            foreach (var node in Analyzer.AllEnumNodes)
            {
                if (cppFullTypeName.IndexOf(node.Name) >= 0)    // out に備えて * を許可
                    return node.CommonName;
            }

            // struct 型であるか
            foreach (var node in Analyzer.AllStructNodes)
            {
                if (cppFullTypeName.IndexOf(node.Name) >= 0)    // const や * はとりあえず考えない
                    return node.CommonName;
            }

            throw new InvalidOperationException("Not found : " + cppFullTypeName);
        }

        /// <summary>
        /// 型名の変換 (仮引数用)
        /// </summary>
        /// <param name="structRef">API 定義の時は false で "ref" を付ける。クラスメンバの時は付けない</param>
        /// <returns></returns>
        public static string ConvertTypeForParam(FuncParamEntity param, bool structRef = true)
        {
            // テーブルにあればそれを使う
            TypeInfo t;
            if (_typeInfoTable.TryGetValue(param.CPPFullTypeName, out t))
                return t.APIModifier + " " + t.CSType;

            // enum 型であるか
            foreach (var node in Analyzer.AllEnumNodes)
            {
                //if (node.Name == param.CPPFullTypeName)
                if (param.CPPFullTypeName.IndexOf(node.Name) >= 0)  // lnXXX* 等の ポインタ型を検索できるように
                {
                    if (param.IOType == ParamIO.Out)
                        return "out " + node.CommonName;
                    else
                        return node.CommonName;
                }
            }

            // struct 型であるか
            foreach (var node in Analyzer.AllStructNodes)
            {
                if (param.CPPFullTypeName.IndexOf(node.Name) >= 0)
                {
                    // 引数のio種類に応じて ref / out を付ける。
                    // クラスライブラリは XNA とかにならって入力用構造体は ref 付けない値渡しだが、
                    // out は付けておく必要がある。公開するのに意味的に正しくない ref は格好悪いし・・・
                    if ((param.IOType & ParamIO.In) != 0)
                        return ((structRef) ? "ref " : "") + node.CommonName;
                    else if (param.IOType == ParamIO.Out)
                        return "out " + node.CommonName;
                }
            }

            throw new InvalidOperationException("Not found : " + param.CPPFullTypeName);
        }

        /// <summary>
        /// C++ のリテラル、lnBool、enum 値から C# 用の値を取得する
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        public static string ConvertLiteral(string value, bool isAPIParam)
        {
            // double に変換できれば、数値リテラル
            double tmp;
            if (double.TryParse(value, out tmp))
                return value;

            if (value == "LN_TRUE")
                return "true";
            if (value == "LN_FALSE")
                return "false";
            if (value == "NULL")
            {
                if (isAPIParam)
                    return "default(IntPtr)";
                else
                    return "null";
            }

            // enum メンバかもしれない
            foreach (var decl in Analyzer.AllEnumNodes)
            {
                foreach (var m in decl.Members)
                {
                    if (m.Name == value)
                        return decl.CommonName + "." + m.CapitalizedName;
                }
            }
            throw new InvalidOperationException("Not Found : " + value);
        }

        /// <summary>
        /// summary コメント作成 (\n 付き)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeSummaryXMLComment(OutputBuffer buffer, string cppSummary)
        {
            if (!string.IsNullOrEmpty(cppSummary))
                buffer.AppendWithIndent(CSCommon.XMLCommentTemplate_Summary.Replace("SUMMARY", cppSummary));
        }

        /// <summary>
        /// remarks コメント作成 (\n 付き。)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeRemarksXMLComment(OutputBuffer buffer, string cppRemakes)
        {
            if (!string.IsNullOrEmpty(cppRemakes))
                buffer.AppendWithIndent(CSCommon.MakeRemarksXMLComment(cppRemakes));
        }

        /// <summary>
        /// param コメント作成 (ひとつ分)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        public static void MakeParamXMLComment(OutputBuffer buffer, FuncParamEntity param)
        {
            buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", param.ParamName).Replace("DETAIL", param.Detail));
        }

        /// <summary>
        /// params コメント作成
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        public static void MakeParamsXMLComment(OutputBuffer buffer, IEnumerable<FuncParamEntity> paramList)
        {
            foreach (var param in paramList)
            {
                buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", param.ParamName).Replace("DETAIL", param.Detail));
            }
        }
        
        /// <summary>
        /// return コメント作成
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="cppReturn"></param>
        public static void MakeParamsXMLComment(OutputBuffer buffer, string cppReturn)
        {
            if (!string.IsNullOrEmpty(cppReturn))
                buffer.AppendWithIndent(CSCommon.XMLCommentTemplate_Return.Replace("DETAIL", cppReturn));
        }


        /// <summary>
        /// remakes コメント作成
        /// </summary>
        /// <returns></returns>
        public static string MakeRemarksXMLComment(string contents)
        {
            string doc = "/// <remarks>\n";

            string[] lines = contents.Split('\n');
            foreach (var line in lines)
                doc += "/// " + line + "\n";

            doc += "/// </remarks>\n";
            return doc;
        }
    }

    /// <summary>
    /// P/Invole ソース
    /// </summary>
    class CSPInvokeBuilder
    {
        #region Templates

        

        const string FuncDeclTempalte = @"
[DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
public extern static RETURN_TYPE FUNC_NAME(ARGS);
";
        const string APISourceTemplate = @"
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LNote
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ErrorCode errorCode);

ENUM_LIST
    public class API
    {
    
#if DEBUG
    	const string DLLName = ""LNote.Debug.dll"";
#else
    	const string DLLName = ""LNote.dll"";
#endif
        const CharSet DLLCharSet = CharSet.Ansi;
        const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
API_LIST

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
";

        #endregion

        private C_APIAnalyzer _analyzer;

        /// <summary>
        /// API ライブラリ作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void BuildAPILib(C_APIAnalyzer analyzer, string outputFilePath)
        {
            _analyzer = analyzer;
            CSCommon.Analyzer = _analyzer;

            //---------------------------------------------------------
            // enum 定数
            var enumText = new OutputBuffer();
            enumText.IncreaseIndent();
            {
                foreach (var decl in analyzer.AllEnumNodes)
                {
                    if (decl.Name == "lnBool")
                    {
                        Console.WriteLine("lnBool は enum として出力しない");
                        continue;
                    }

                    // [Flags] 属性
                    if (decl.IsFlags)
                        enumText.AppendWithIndent("[Flags]\n");

                    // enum ヘッダ
                    enumText.AppendWithIndent("public enum " + decl.CommonName + "\n");
                    enumText.AppendWithIndent("{\n");
                    enumText.IncreaseIndent();

                    // 各メンバ
                    foreach (var member in decl.Members)
                    {
                        // LN_xxxx_MAX のようなターミネータは出力しない
                        if (member.IsTerminator) continue;

                        // XML コメント
                        if (!string.IsNullOrEmpty(member.Comment))
                        {
                            enumText.AppendWithIndent(CSCommon.XMLCommentTemplate_Summary.Replace("SUMMARY", member.Comment));
                        }

                        // メンバ定義
                        enumText.AppendWithIndent(member.CapitalizedName + " = " + member.Value + ",\n\n");
                    }

                    enumText.DecreaseIndent();
                    enumText.AppendWithIndent("}\n\n");
                }
            }

            //---------------------------------------------------------
            // P/Invoke 関数
            var funcsText = new OutputBuffer();
            funcsText.IncreaseIndent();
            {
                foreach (var func in analyzer.AllFuncEntites)
                {
                    // XML コメント
                    if (!string.IsNullOrEmpty(func.Summary))
                    {
                        // XML コメント(summary)
                        funcsText.AppendWithIndent(CSCommon.XMLCommentTemplate_Summary.Replace("SUMMARY", func.Summary));
                        // XMLコメント(param)
                        foreach (var param in func.AllParams)
                        {
                            funcsText.AppendWithIndent(CSCommon.XMLCommentTemplate_Param.Replace("PARAM_NAME", param.ParamName).Replace("DETAIL", param.Detail));
                        }
                        // XMLコメント(return)
                        if (!string.IsNullOrEmpty(func.ReturnType.Detail))
                            funcsText.AppendWithIndent(CSCommon.XMLCommentTemplate_Return.Replace("DETAIL", func.ReturnType.Detail));
                        // XMLコメント(remarks)
                        if (!string.IsNullOrEmpty(func.Detail))
                            funcsText.AppendWithIndent(CSCommon.MakeRemarksXMLComment(func.Detail));
                    }

                    // DLLImport・型名・関数名
                    string declText = FuncDeclTempalte.Trim();
                    declText = declText.Replace("RETURN_TYPE", CSCommon.ConvertTypeBasic(func.ReturnType.CPPFullTypeName));
                    declText = declText.Replace("FUNC_NAME", func.CPPName);

                    // 仮引数リスト
                    string argsText = "";
                    foreach (var param in func.AllParams)
                    {
                        if (argsText != "") argsText += ", ";

                        // 型名と引数名
                        argsText += CSCommon.ConvertTypeForParam(param) + " " + param.ParamName;

                        // デフォルト引数
                        if (!string.IsNullOrEmpty(param.DefaultValue))
                            argsText += " = " + CSCommon.ConvertLiteral(param.DefaultValue, true);
                    }
                    declText = declText.Replace("ARGS", argsText);

                    funcsText.AppendWithIndent(declText + "\n\n");
                }
            }

            //---------------------------------------------------------
            // 出力
            string output = APISourceTemplate.Replace("ENUM_LIST", enumText.ToString());
            output = output.Replace("API_LIST", funcsText.ToString());

            System.IO.File.WriteAllText(outputFilePath, output, Encoding.UTF8);
        }
    }
#endif
}
