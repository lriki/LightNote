﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// 中間コード管理クラス
    /// </summary>
    class CLManager
    {
        //①型の集計
        //②型の割り当て
        //③オーバーロードメソッドの集計
        //④プロパティの集計
        //⑤出力言語の指定

        #region Constants
        private const string HandleTypeParamMacro = "LN_HANDLE";
        private const string GenericHandleTypeParamMacro = "LN_HANDLE_GENERIC";
        #endregion

        #region Fields

        /// <summary>
        /// 基本的なC言語の型と CLType との変換テーブル
        /// </summary>
        private static Dictionary<string, CLType> _typeInfoTable = new Dictionary<string, CLType>()
        {
            { "void",               CLPrimitiveType.Void },
            { "void*",              CLClass.ByteArray },
            { "const void*",        CLClass.ByteArray },
            { "void**",             CLClass.ByteArray },  //TODO: この2つは
            { "const void**",       CLClass.ByteArray },   //TODO: バッファクラスを用意する必要がありそう

            { "const LNChar*",      CLPrimitiveType.String },
            { "const LNChar**",     CLPrimitiveType.String },

            { "int",                CLPrimitiveType.Int },
            { "int*",               CLPrimitiveType.Int },
            { "float",              CLPrimitiveType.Float },
            { "float*",             CLPrimitiveType.Float },
            { "double",             CLPrimitiveType.Double },
            { "LNBool",             CLPrimitiveType.Bool },
            { "LNBool*",　          CLPrimitiveType.Bool },
            { "LNU8",               CLPrimitiveType.Byte },
            { "LNU32",              CLPrimitiveType.UInt32 },

            //{ "lnIntPtr",           CLPrimitiveType.IntPtr },
            
            { "const int*",         CLClass.IntArray },
        }; 

        #endregion

        #region Properties
        /// <summary>
        /// グローバルインスタンス
        /// </summary>
        public static CLManager Instance { get; set; }

        /// <summary>
        /// 全 Entity リスト
        /// </summary>
        public List<CLEntity> AllEntities { get; private set; }

        /// <summary>
        /// 全型リスト
        /// </summary>
        public List<CLType> AllTypes { get; private set; }

        /// <summary>
        /// 全クラスリスト
        /// </summary>
        public List<CLClass> AllClasses { get; private set; }

        /// <summary>
        /// 全 struct 型リスト
        /// </summary>
        public List<CLStruct> AllStructs { get; private set; }

        /// <summary>
        /// 全 enum 型リスト
        /// </summary>
        public List<CLEnum> AllEnums { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLManager()
        {
            AllEntities = new List<CLEntity>();
            AllTypes = new List<CLType>();
            AllClasses = new List<CLClass>();
            AllStructs = new List<CLStruct>();
            AllEnums = new List<CLEnum>();
        }

        public void Initialize()
        {
            
        }

        /// <summary>
        /// 
        /// </summary>
        public void RegisterEntities(List<CLEntity> typedefs, List<CLModule> modules)
        {
            foreach (var m in typedefs)
            {
                m.Register();
            }
            foreach (var m in modules)
            {
                m.Register();
            }
        }

        /// <summary>
        /// パース済みの AllEntities を解析し、型情報やプロパティの関係を作成する
        /// </summary>
        public void LinkEntities()
        {
            int i = 0;
            foreach (var e in AllEntities)
            {
                e.LinkTypes();
                ++i;
            }
        }

        /// <summary>
        /// オリジナルの型名から CLType を検索する。
        /// "LN_HANDLE(LNSound)" とかもOK。 
        /// </summary>
        /// <param name="name"></param>
        public CLType FindType(string name)
        {
            // クラス型を検索
            int idx = name.IndexOf(HandleTypeParamMacro);
            if (idx >= 0)
            {
                // LN_HANDLE と () を削除
                name = name
                    .Substring(idx + HandleTypeParamMacro.Length)
                    .Replace("(", "")
                    .Replace(")", "")
                    .Replace("*", "")
                    .Trim();
                foreach (var t in AllTypes)
                {
                    CLClass c = t as CLClass;
                    if (c != null && c.OriginalName == name)
                    {
                        return c;
                    }
                }
                throw new InvalidOperationException("invalid class type.");
            }

            // ジェネリッククラス型を検索
            idx = name.IndexOf(GenericHandleTypeParamMacro);
            if (idx >= 0)
            {
                // LN_HANDLE と () を削除
                name = name
                    .Substring(idx + GenericHandleTypeParamMacro.Length)
                    .Replace("(", "")
                    .Replace(")", "")
                    .Replace("*", "")
                    .Trim();
                foreach (var t in AllTypes)
                {
                    CLClass c = t as CLClass;
                    if (c != null && c.IsGeneric && c.OriginalName == name)
                    {
                        return c;
                    }
                }
                throw new InvalidOperationException("invalid generic class type.");
            }

            // 基本的な型テーブルから検索
            CLType type;
            if (_typeInfoTable.TryGetValue(name, out type))
            {
                return type;
            }

            // 構造体
            foreach (var t in AllStructs)
            {
                if (name.IndexOf(t.Name) >= 0)
                    return t;
            }

            // 列挙型
            foreach (var t in AllEnums)
            {
                if (name.IndexOf(t.Name) >= 0)
                    return t;
            }

            throw new InvalidOperationException("invalid type.");
        }
        #endregion


    }
}
