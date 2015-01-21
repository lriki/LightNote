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
        /// メンバメソッドリスト
        /// </summary>
        public List<CLMethod> Methods { get; private set; }

        /// <summary>
        /// オプション
        /// </summary>
        public CLOption Option { get; private set; }

        /// <summary>
        /// ジェネリッククラスの型引数
        /// </summary>
        public bool IsGeneric { get; private set; }

        /// <summary>
        /// ジェネリッククラスの型引数
        /// </summary>
        public CLType BindingType { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="methods"></param>
        /// <param name="option"></param>
        public CLClass(CLDocument doc, string name, IEnumerable<CLMethod> methods, CLOption option)
        {
            Document = doc;
            OriginalName = name.Trim();
            Methods = new List<CLMethod>(methods);
            Option = option;
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

        #endregion
    }
}
