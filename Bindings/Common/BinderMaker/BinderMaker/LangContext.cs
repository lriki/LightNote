using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    [Flags]
    enum LangFlags
    {
        C = 0x00,
        Cpp = 0x01,
        CS = 0x02,
        Java = 0x04,
        Ruby = 0x08,
        Python = 0x10,
        Lua = 0x20,
        HSP = 0x40,
    }

    /// <summary>
    /// ドキュメントコメント等を各言語用に変換する
    /// </summary>
    class LangContext
    {
        #region Constants
        public const string BriefTag = "@brief";
        public const string ParamTag = "@param";
        #endregion

        private LangFlags _langFlags;

        public LangContext(LangFlags langFlags)
        {
            _langFlags = langFlags;
        }

        /// <summary>
        /// メソッドの @brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLMethod method)
        {
            return MakeText(method.Document.OriginalBriefText, method.Document, BriefTag);
        }

        private string MakeText(string srcText, CLDocument doc, string targetSection)
        {
            var replaceDocs = doc.ReplaceDocs.FindAll((d) => d.LangFlags == _langFlags);
            var postscriptDocs = doc.PostscriptDocs.FindAll((d) => (d.LangFlags == _langFlags && d.TargetSection == targetSection));
            var overwriteDocs = doc.OverwriteDocs.FindAll((d) => (d.LangFlags == _langFlags && d.TargetSection == targetSection));

            foreach (var replaceDoc in replaceDocs)
            {
                srcText.Replace(replaceDoc.From, replaceDoc.To);
            }
            foreach (var postscriptDoc in postscriptDocs)
            {
                srcText += Builder.OutputBuffer.NewLineCode + Builder.OutputBuffer.NewLineCode;
                srcText += postscriptDoc.Text;
            }
            foreach (var overwriteDoc in overwriteDocs)
            {
                srcText = overwriteDoc.Text;
            }

            return srcText;
        }

        /// <summary>
        /// 仮引数の説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetParamText(CLParam param)
        {
            return MakeText(param.Document.OriginalText, param.OwnerFunc.OwnerMethod.Document, ParamTag);
        }
    }
}
