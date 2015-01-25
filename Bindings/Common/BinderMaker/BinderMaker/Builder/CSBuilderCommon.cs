﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    static class CSBuilderCommon
    {
        #region Templates
        public const string XMLCommentTemplate_Summary = "/// <summary>\n/// SUMMARY\n/// </summary>\n";
        public const string XMLCommentTemplate_Param = "/// <param name=\"PARAM_NAME\">DETAIL</param>\n";
        public const string XMLCommentTemplate_Return = "/// <returns>DETAIL</returns>\n";
        #endregion

        #region Fields

        // CLPrimitiveType → C# 型名の変換テーブル
        public static Dictionary<CLType, string> PrimitiveTypeNameTable = new Dictionary<CLType, string>()
        {
            { CLPrimitiveType.Void, "void" },
            //{ CLPrimitiveType.ByteArray , "byte[]" },
            { CLPrimitiveType.String, "string" },

            { CLPrimitiveType.Bool, "bool" },
            { CLPrimitiveType.Byte, "byte" },
            { CLPrimitiveType.Int, "int" },
            { CLPrimitiveType.UInt32, "uint" },
            { CLPrimitiveType.Float, "float" },
            { CLPrimitiveType.Double, "double" },

            //{ CLPrimitiveType.ExceptionCallback, "ExceptionCallback" },
            { CLPrimitiveType.IntPtr, "IntPtr" },
            { CLPrimitiveType.HWND, "IntPtr" },
            
            { CLClass.ByteArray, "byte[]" },
            { CLClass.IntArray, "int[]" },

            //{ CLPrimitiveType.Handle, "IntPtr" },
            //{ CLPrimitiveType.Generic, "double" },

            //{ CLPrimitiveType.IntNativeArray, "int[]" },
        };
        #endregion

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
        /// param コメント作成 (ひとつ分)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        public static void MakeParamXMLComment(OutputBuffer buffer, string paramName, string paramSummary)
        {
            buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", paramName).Replace("DETAIL", paramSummary));
        }

        /// <summary>
        /// return コメント作成
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="param"></param>
        public static void MakeReturnXMLComment(OutputBuffer buffer, string text)
        {
            if (!string.IsNullOrEmpty(text))
                CSBuilderCommon.XMLCommentTemplate_Return.Replace("DETAIL", text);
        }

        /// <summary>
        /// remarks コメント作成 (\n 付き。)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeRemarksXMLComment(OutputBuffer buffer, string cppRemakes)
        {
            // <br> いらない
            cppRemakes = cppRemakes.Replace("<br>", "");
            if (!string.IsNullOrEmpty(cppRemakes))
                buffer.AppendWithIndent(MakeRemarksXMLComment(cppRemakes));
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

        /// <summary>
        /// 受け取った param の属性から ref/out を返す
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string GetParamIOModifier(CLParam param)
        {
            string m = "";
            if ((param.IOModifier & IOModifier.Out) != 0)
            {
                if ((param.IOModifier & IOModifier.In) != 0)
                    m = "ref";
                else
                    m = "out";
            }
            return m;
        }

        /// <summary>
        /// 受け取った param の属性から ref/out を返す (API 定義・呼び出し用)
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string GetAPIParamIOModifier(CLParam param)
        {
            // struct の場合は必ず 
            if ((param.Type is CLClass) && ((CLClass)param.Type).IsStruct)
            {
                if ((param.IOModifier & IOModifier.Out) != 0 && (param.IOModifier & IOModifier.In) == 0)
                    return "out";
                else
                    return "ref";
            }

            // それ以外は GetParamModifier と同じ
            return GetParamIOModifier(param);
        }

        /// <summary>
        /// C# の型名を求める
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        public static string MakeTypeName(CLType type)
        {
            // テーブルにあればそれを使う
            string name;
            if (PrimitiveTypeNameTable.TryGetValue(type, out name))
                return name;

            // enum 型
            if (type is CLEnum) return ((CLEnum)type).Name;
            // class 型
            if (type is CLClass) return ((CLClass)type).Name;
            // delegate 型
            if (type is CLDelegate) return ((CLDelegate)type).Name;

            throw new InvalidOperationException();
        }

        //public static string ConvertTypeToName(CLType type, CLType genericTypeArg)
        //{
        //    // テーブルにあればそれを使う
        //    if (_typeNameTable.ContainsKey(type))
        //        return _typeNameTable[type];

        //    // enum 型であるか
        //    if (type is CLEnumType)
        //        return ((CLEnumType)type).Name;

        //    // 参照オブジェクトまたは struct 型であるか
        //    var classType = type as CLClassType;
        //    if (classType != null)
        //    {
        //        if (classType.IsGeneric)
        //        {
        //            if (string.IsNullOrEmpty(genericTypeArg.Name)) throw new InvalidOperationException();
        //            return classType.Name + "<" + genericTypeArg.Name + ">";
        //        }
        //        else
        //        {
        //            return classType.Name;
        //        }
        //    }

        //    throw new InvalidOperationException("Not found : " + type.Name);
        //}

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
            foreach (var enumType in CLManager.Instance.AllEnums)
            {
                var member = enumType.Members.Find((m) => m.OriginalName == value);
                if (member != null)
                    return enumType.Name + "." + member.CapitalizedName;
            }
            throw new InvalidOperationException("Not Found : " + value);
        }
    }
}
