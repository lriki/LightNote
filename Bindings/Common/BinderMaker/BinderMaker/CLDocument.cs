using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLDocument
    {
        public string Brief { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLDocument(
            string briefText,
            IEnumerable<CLParamDocument> paramDocs,
            string returnText,
            string detailsText,
            IEnumerable<CLOverwriteDocument> overwriteDocs,
            IEnumerable<CLReplaceDocument> replaceDocs,
            IEnumerable<CLExampleDocument> exampleDocs)
        {
            // 渡されるテキストは前後に空白が入っていたりするので Trim() とかしておく
            Brief = briefText.Trim();
        }
    }

    class CLParamDocument
    {
        public CLParamDocument(string io, string name, string text)
        {
            // io は [] を含む？
        }
    }
    class CLOverwriteDocument
    {
        public CLOverwriteDocument(string langs, string target, string text)
        {

        }
    }

    class CLReplaceDocument
    {
        public CLReplaceDocument(string langs, string from, string to)
        {

        }
    }

    class CLExampleDocument
    {
        public CLExampleDocument(string langs, string text)
        {

        }
    }
}
