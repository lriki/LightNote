using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class CSClassLibBuilder : Builder
    {
        #region Fields
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _classesText = new OutputBuffer(1);
        private OutputBuffer _typeInfoRegistersText = new OutputBuffer(3);
        private OutputBuffer _typeInfoPInvolesText = new OutputBuffer(2);
        #endregion

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            var template = GetTemplate("CSClassLib.txt");
            return template
                .Replace("CLASSES", _classesText.ToString())
                .Replace("TYPE_INFO_REGISTERS", _typeInfoRegistersText.ToString())
                .Replace("TYPE_INFO_PINVOKES", _typeInfoPInvolesText.ToString());
        }
    }
}
