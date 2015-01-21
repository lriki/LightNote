using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLEntity
    {
        #region Properties
        /// <summary>
        /// 管理クラス
        /// </summary>
        public CLManager Manager { get { return CLManager.Instance; } }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLEntity()
        {
            Manager.AllEntities.Add(this);
        }
        #endregion
    }

    /// <summary>
    /// 型エンティティ
    /// </summary>
    class CLType : CLEntity
    {
        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLType()
        {
            Manager.AllTypes.Add(this);
        }
        #endregion
    }

    /// <summary>
    /// プリミティブ型
    /// </summary>
    class CLPrimitiveType : CLType
    {
        #region Constants
        public static CLPrimitiveType Void = new CLPrimitiveType("Void");
        public static CLPrimitiveType ByteArray = new CLPrimitiveType("ByteArray");
        public static CLPrimitiveType String = new CLPrimitiveType("String");

        public static CLPrimitiveType Bool = new CLPrimitiveType("Bool");
        public static CLPrimitiveType Byte = new CLPrimitiveType("Byte");
        public static CLPrimitiveType Int = new CLPrimitiveType("Int");
        public static CLPrimitiveType UInt32 = new CLPrimitiveType("UInt32");
        public static CLPrimitiveType Float = new CLPrimitiveType("Float");
        public static CLPrimitiveType Double = new CLPrimitiveType("Double");

        public static CLPrimitiveType ResultCode = new CLPrimitiveType("ResultCode");
        public static CLPrimitiveType IntPtr = new CLPrimitiveType("IntPtr");
        public static CLPrimitiveType HWND = new CLPrimitiveType("HWND");
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLPrimitiveType(string name)
        {
        }
        #endregion
    }
    /// <summary>
    /// リテラル (bool、int、float、enum)
    /// デフォルト引数や enum メンバ値で使用する。
    /// </summary>
    class CLLiteral : CLEntity
    {
        #region Types
        public enum ValueType
        {
            Bool,
            Int,
            Float,
            Enum,
        }
        #endregion

        #region Properties
        /// <summary>
        /// 値の型
        /// </summary>
        public ValueType Type { get; private set; }

        /// <summary>
        /// 値
        /// </summary>
        public object Value { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="value">値の文字列表現</param>
        public CLLiteral(string value)
        {
            if (value == "LN_TRUE")
            {
                Type = ValueType.Bool;
                Value = true;
                return;
            }
            if (value == "LN_FALSE")
            {
                Type = ValueType.Bool;
                Value = false;
                return;
            }
            int vInt;
            if (int.TryParse(value, out vInt))  // 1.5 とか小数点があると失敗する
            {
                Type = ValueType.Int;
                Value = vInt;
                return;
            }
            float vFloat;
            if (float.TryParse(value, out vFloat))
            {
                Type = ValueType.Float;
                Value = vFloat;
                return;
            }

            // 以上に当てはまらなければとりあえず enum
            Type = ValueType.Enum;
            Value = value;
        }
        #endregion
    }

    /// <summary>
    /// struct 定義
    /// </summary>
    class CLStruct : CLType
    {
        #region Properties
        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// メンバリスト
        /// </summary>
        public List<CLStructMember> Members { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="comment"></param>
        /// <param name="name"></param>
        /// <param name="members"></param>
        public CLStruct(string comment, string name, IEnumerable<CLStructMember> members)
        {
            Comment = comment;
            Name = name.Substring(2);   // プレフィックスを取り除く
            Members = new List<CLStructMember>(members);
        }
        #endregion
    }

    /// <summary>
    /// enum メンバ
    /// </summary>
    class CLStructMember : CLEntity
    {
        #region Fields
        private string _originalTypeName;   // オリジナルの型名
        #endregion

        #region Properties
        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="typeName"></param>
        /// <param name="name"></param>
        /// <param name="comment"></param>
        public CLStructMember(string typeName, string name, string comment)
        {
            _originalTypeName = typeName;
            Name = name;
            Comment = comment;
        }
        #endregion
    }

    /// <summary>
    /// enum 定義
    /// </summary>
    class CLEnum : CLType
    {
        #region Properties
        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// メンバリスト
        /// </summary>
        public List<CLEnumMember> Members { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="comment"></param>
        /// <param name="name"></param>
        /// <param name="members"></param>
        public CLEnum(string comment, string name, IEnumerable<CLEnumMember> members)
        {
            Comment = comment;
            Name = name.Substring(2);   // プレフィックスを取り除く
            Members = new List<CLEnumMember>(members);
        }
        #endregion
    }

    /// <summary>
    /// enum メンバ
    /// </summary>
    class CLEnumMember : CLEntity
    {
        #region Fields
        private string _originalValue;   // オリジナルの値
        #endregion

        #region Properties
        /// <summary>
        /// オリジナルの名前
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// コメント
        /// </summary>
        public string Comment { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="name"></param>
        /// <param name="value"></param>
        /// <param name="comment"></param>
        public CLEnumMember(string name, string value, string comment)
        {
            OriginalName = name;
            _originalValue = value;
            Comment = comment;
        }
        #endregion
    }

    /// <summary>
    /// 関数ポインタ
    /// </summary>
    class CLDelegate : CLType
    {
        #region Fields
        private string _originalReturnTypeName;
        private string _originalName;           // オリジナルの名前
        #endregion

        #region Properties
        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// 仮引数リスト
        /// </summary>
        public List<CLParam> Params { get; private set; }
        #endregion

        public CLDelegate(CLDocument doc, string returnType, string name, IEnumerable<CLParam> params1)
        {
            _originalReturnTypeName = returnType;
            _originalName = name;

            Document = doc;
            Name = name.Substring(2);   // プレフィックスを取り除く
            Params = new List<CLParam>(params1);
        }
        
    }
}
