using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    static class CSBuilderCommon
    {
        #region Templates
        public const string XMLCommentTemplate_Summary = "/// <summary>\n/// SUMMARY\n/// </summary>\n";
        public const string XMLCommentTemplate_Param = "/// <param name=\"PARAM_NAME\">DETAIL</param>\n";
        public const string XMLCommentTemplate_Return = "/// <returns>DETAIL</returns>\n";
        #endregion

        /// <summary>
        /// 各機能を使う前にセットしておく
        /// </summary>
        public static CLAnalyzer Analyzer { get; set; }

        /// <summary>
        /// summary コメント作成 (\n 付き)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeSummaryXMLComment(OutputBuffer buffer, string cppSummary)
        {
            if (!string.IsNullOrEmpty(cppSummary))
                buffer.AppendWithIndent(XMLCommentTemplate_Summary.Replace("SUMMARY", cppSummary));
        }

        /// <summary>
        /// remarks コメント作成 (\n 付き。)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeRemarksXMLComment(OutputBuffer buffer, string cppRemakes)
        {
            if (!string.IsNullOrEmpty(cppRemakes))
                buffer.AppendWithIndent(MakeRemarksXMLComment(cppRemakes));
        }

        /// <summary>
        /// param コメント作成 (ひとつ分)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        public static void MakeParamXMLComment(OutputBuffer buffer, string paramName, string paramSummary)
        {
            buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", paramName).Replace("DETAIL", paramSummary));
        }

        /// <summary>
        /// params コメント作成
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        //public static void MakeParamsXMLComment(OutputBuffer buffer, IEnumerable<FuncParamEntity> paramList)
        //{
        //    foreach (var param in paramList)
        //    {
        //        buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", param.ParamName).Replace("DETAIL", param.Detail));
        //    }
        //}

        


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
            foreach (var enumType in Analyzer.AllEnumTypes)
            {
                foreach (var m in enumType.Members)
                {
                    if (m.Name == value)
                        return enumType.Name + "." + m.CapitalizedName;
                }
            }
            throw new InvalidOperationException("Not Found : " + value);
        }

        // CLType → C# 型名の変換テーブル
        private static Dictionary<CLType, string> _typeNameTable = new Dictionary<CLType, string>()
        {
            { CLPrimitiveType.Void, "void" },
            { CLPrimitiveType.ByteArray , "byte[]" },
            { CLPrimitiveType.String, "string" },

            { CLPrimitiveType.Bool, "bool" },
            { CLPrimitiveType.Byte, "byte" },
            { CLPrimitiveType.Int, "int" },
            { CLPrimitiveType.UInt32, "uint" },
            { CLPrimitiveType.Float, "float" },
            { CLPrimitiveType.Double, "double" },

            { CLPrimitiveType.ResultCode, "ErrorCode" },
            { CLPrimitiveType.ExceptionCallback, "ExceptionCallback" },
            { CLPrimitiveType.IntPtr, "IntPtr" },
            { CLPrimitiveType.HWND, "IntPtr" },

            { CLPrimitiveType.Handle, "IntPtr" },
            //{ CLPrimitiveType.Generic, "double" },

            { CLPrimitiveType.IntNativeArray, "int[]" },
        };

        public static string ConvertTypeToName(CLType type, CLType genericTypeArg)
        {
            //if (type == null)
            //    return "void";  // return 用

            // テーブルにあればそれを使う
            if (_typeNameTable.ContainsKey(type))
                return _typeNameTable[type];

            // enum 型であるか
            if (type is CLEnumType)
                return ((CLEnumType)type).Name;

            // 参照オブジェクトまたは struct 型であるか
            var classType = type as CLClassType;
            if (classType != null)
            {
                if (classType.IsGeneric)
                {
                    if (string.IsNullOrEmpty(genericTypeArg.Name)) throw new InvalidOperationException();
                    return classType.Name + "<" + genericTypeArg.Name + ">";
                }
                else
                {
                    return classType.Name;
                }
            }

            throw new InvalidOperationException("Not found : " + type.Name);
        }

        /// <summary>
        /// 受け取った param の属性から ref/out を返す
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string GetParamModifier(CLParam param)
        {
            string m = "";
            if ((param.Attribute & ParamAttribute.Out) != 0)
            {
                if ((param.Attribute & ParamAttribute.In) != 0)
                    m = "ref ";
                else
                    m = "out ";
            }
            return m;
        }

        /// <summary>
        /// 受け取った param の属性から ref/out を返す (API 定義・呼び出し用)
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string GetAPIParamModifier(CLParam param)
        {
            // struct の場合は必ず 
            if ((param.Type is CLClassType) &&
                ((CLClassType)param.Type).IsStruct)
            {
                if ((param.Attribute & ParamAttribute.Out) != 0 && (param.Attribute & ParamAttribute.In) == 0)
                    return "out ";
                else
                    return "ref ";
            }

            // それ以外は GetParamModifier と同じ
            return GetParamModifier(param);
        }



        //-----------------------------------------------------
        // 以下、構造体メンバの型変換用。
        // 古い処理なので後で削除する予定。

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

        /// <summary>
        /// 型名の変換
        /// </summary>
        /// <returns></returns>
        public static string ConvertTypeBasic(string cppFullTypeName)
        {
            // テーブルにあればそれを使う
            if (_typeInfoTable.ContainsKey(cppFullTypeName))
                return _typeInfoTable[cppFullTypeName].CSType;

            //// enum 型であるか
            //foreach (var node in Analyzer.AllEnumNodes)
            //{
            //    if (cppFullTypeName.IndexOf(node.Name) >= 0)    // out に備えて * を許可
            //        return node.CommonName;
            //}

            //// struct 型であるか
            //foreach (var node in Analyzer.AllStructNodes)
            //{
            //    if (cppFullTypeName.IndexOf(node.Name) >= 0)    // const や * はとりあえず考えない
            //        return node.CommonName;
            //}

            throw new InvalidOperationException("Not found : " + cppFullTypeName);
        }
    }
}
