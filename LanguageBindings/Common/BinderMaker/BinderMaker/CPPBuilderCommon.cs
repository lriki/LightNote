using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    static class CppBuilderCommon
    {
        public class CTypeInfo
        {
            public string InType;
            public string OutType;
            public string CppInType;
            public string CppOutType;
            public string CppInOutType;
            public CTypeInfo(string inType, string outType, string cppType, string cppOutType, string cppInOutType)
            {
                InType = inType;
                OutType = outType;
                CppInType = cppType;
                CppOutType = cppOutType;
                CppInOutType = cppInOutType;
            }
        }

        // CLType → C 型名の変換テーブル
        public static Dictionary<CLType, CTypeInfo> _typeNameTable = new Dictionary<CLType, CTypeInfo>()
        {
            { CLPrimitiveType.Void,         new CTypeInfo("void", "void", "void", "void", null) },
            { CLPrimitiveType.ByteArray,    new CTypeInfo("const void*", "void**", "const void*", "void**", "void*") },
            { CLPrimitiveType.String,       new CTypeInfo("const lnChar*", "lnChar**", "const lnChar*", "lnChar**", null) },

            { CLPrimitiveType.Bool,         new CTypeInfo("lnBool", "lnBool*", "bool", "bool*", null) },
            { CLPrimitiveType.Byte,         new CTypeInfo("lnU8", "lnU8*", "lnU8", "lnU8*", null) },
            { CLPrimitiveType.Int,          new CTypeInfo("int", "int*", "int", "int*", null) },
            { CLPrimitiveType.UInt32,       new CTypeInfo("lnU32", "lnU32*", "lnU32", "lnU32*", null) },
            { CLPrimitiveType.Float,        new CTypeInfo("float", "float*", "float", "float*", null) },
            { CLPrimitiveType.Double,       new CTypeInfo("double", "double*", "double", "double*", null) },

            { CLPrimitiveType.ResultCode,   new CTypeInfo("lnResult", "lnResult*", "lnResult", "lnResult*", null) },
            { CLPrimitiveType.ExceptionCallback, new CTypeInfo("lnExceptionCallback", "lnExceptionCallback*", "lnExceptionCallback", "lnExceptionCallback*", null) },
            { CLPrimitiveType.IntPtr,       new CTypeInfo("lnIntPtr", "lnIntPtr*", "lnIntPtr", "lnIntPtr*", null) },
            { CLPrimitiveType.HWND,         new CTypeInfo("HWND", "HWND*", "void*", "void**", null) },

            //{ CLPrimitiveType.Handle,       new CTypeInfo("void", "void") },
            //{ CLPrimitiveType.Generic, "double" },

            { CLPrimitiveType.IntNativeArray,   new CTypeInfo("const int*", null, "const int*", null, null) },
        };

        /// <summary>
        /// 各機能を使う前にセットしておく
        /// </summary>
        public static CLAnalyzer Analyzer { get; set; }

        /// <summary>
        /// C言語の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertTypeToCName(CLType type)
        {
            // テーブル検索
            CTypeInfo info;
            if (_typeNameTable.TryGetValue(type, out info))
            {
                return info.InType;
            }

            // enum 型であるか
            if (type is CLEnumType)
                return ((CLEnumType)type).CName;

            // 参照オブジェクトまたは struct 型であるか
            var classType = type as CLClassType;
            if (classType != null)
            {
                string name;
                if (classType.IsStruct)
                    name = "ln" + classType.Name;
                else
                    name = "lnHandle";
                return name;
            }

            // Generic
            if (type == CLPrimitiveType.Generic)
                return "lnHandle";

            throw new InvalidOperationException();
        }

#if false
        /// <summary>
        /// C言語の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertParamTypeToCName(CLParam param)
        {
            // テーブル検索
            CTypeInfo info;
            if (_typeNameTable.TryGetValue(param.Type, out info))
            {
                if ((param.Attribute & ParamAttribute.Out) != 0)
                    return info.OutType;
                else
                    return info.InType;
            }

            // enum 型であるか
            if (param.Type is CLEnumType)
            {
                if ((param.Attribute & ParamAttribute.Out) != 0)
                    return ((CLEnumType)param.Type).CName + "*";
                else
                    return ((CLEnumType)param.Type).CName;
            }

            // 参照オブジェクトまたは struct 型であるか
            var classType = param.Type as CLClassType;
            if (classType != null)
            {
                string name;
                if (classType.IsStruct)
                    name = classType.CName;
                else
                    name = "lnHandle";

                if ((param.Attribute & ParamAttribute.Out) != 0)
                    name += "*";
                return name;
            }

            throw new InvalidOperationException();
        }
#endif
    }
}
