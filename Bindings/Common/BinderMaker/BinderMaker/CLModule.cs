using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLModule
    {
        #region Properties

        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// クラスリスト
        /// </summary>
        public List<CLClass> Classes { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="classes"></param>
        public CLModule(CLDocument doc, IEnumerable<CLClass> classes)
        {
            Document = doc;
            Classes = new List<CLClass>(classes);
        }

        #endregion
    }
}
