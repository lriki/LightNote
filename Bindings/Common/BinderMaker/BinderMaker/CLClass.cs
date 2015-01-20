using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLClass
    {
        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

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
            Name = name.Trim();
            Methods = new List<CLMethod>(methods);
            Option = option;
        }

        #endregion
    }
}
