using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLClass : CLType
    {
        #region Constants

        /// <summary>
        /// Array 型クラスの定義
        /// </summary>
        public static CLClass Array = new CLClass("Array", null);
        public static CLClass ByteArray = new CLClass("Array", CLPrimitiveType.Byte);
        public static CLClass IntArray = new CLClass("Array", CLPrimitiveType.Int);

        #endregion

        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// オリジナルのクラス名
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// クラス名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// メンバメソッドリスト
        /// </summary>
        public List<CLMethod> Methods { get; private set; }

        /// <summary>
        /// オプション
        /// </summary>
        public CLOption Option { get; private set; }
        
        /// <summary>
        /// RefObj クラス であるか
        /// </summary>
        public bool IsReferenceObject { get { return !IsStatic && !IsStruct; } }

        /// <summary>
        /// static クラス であるか
        /// </summary>
        public bool IsStatic { get; private set; }

        /// <summary>
        /// ジェネリッククラス であるか
        /// </summary>
        public bool IsGeneric { get; private set; }

        /// <summary>
        /// ジェネリッククラスの型引数 (null の場合もある。その場合、バインドされていない(インスタンス化されていない))
        /// </summary>
        public CLType BindingType { get; private set; }

        /// <summary>
        /// struct であるか
        /// </summary>
        public bool IsStruct { get; private set; }

        /// <summary>
        /// 構造体情報
        /// </summary>
        public CLStructDef StructData { get; private set; }

        /// <summary>
        /// 拡張クラス であるか (各言語のコードで独自定義するもの。RefObject や ObjectList)
        /// </summary>
        public bool IsExtension { get; private set; }
        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="startTag"></param>
        /// <param name="doc"></param>
        /// <param name="methods"></param>
        /// <param name="option"></param>
        public CLClass(IEnumerable<char> startTag, CLDocument doc, string name, IEnumerable<CLMethod> methods, CLOption option)
        {
            Document = doc;
            OriginalName = name.Trim();
            Name = OriginalName.Substring(2);
            Methods = new List<CLMethod>(methods);
            Option = option;

            Methods.ForEach((m) => m.OwnerClass = this);

            // クラス種別チェック
            string t = new string(startTag.ToArray());
            if (t.IndexOf("LN_STATIC_CLASS") >= 0)
                IsStatic = true;
            if (t.IndexOf("LN_GENERIC_CLASS") >= 0)
                IsGeneric = true;
            if (t.IndexOf("LN_STRUCT_CLASS") >= 0)
                IsStruct = true;
            if (t.IndexOf("LN_EXTENSION_CLASS") >= 0)
                IsExtension = true;
        }

        /// <summary>
        /// コンストラクタ (型をバインドしたジェネリッククラス(インスタンス化))
        /// ※C# ではインスタンス化しているかにかかわらず Type クラスであらわされる。
        ///   インスタンス化していないものは GenericTypeArgments 配列の要素数が 0 である。
        /// </summary>
        /// <param name="name"></param>
        /// <param name="?"></param>
        public CLClass(string name, CLType bindingType)
        {
            OriginalName = name;
            BindingType = bindingType;
            IsGeneric = true;
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、階層的に Manager の管理リストに登録する
        /// </summary>
        public override void Register()
        {
            base.Register();
            Manager.AllClasses.Add(this);
            Document.Register();
            Methods.ForEach((c) => c.Register());
            Option.Register();
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public override void LinkTypes()
        {
            base.LinkTypes();
            StructData = Manager.AllStructDefs.Find((t) => t.OriginalName == OriginalName);
            if (StructData != null && !IsStruct) throw new InvalidOperationException("invalid struct type.");
        }

        #endregion
    }
}
