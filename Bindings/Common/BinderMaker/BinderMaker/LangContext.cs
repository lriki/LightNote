using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
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
        public const string BriefTag = "brief";
        public const string ParamTag = "param";
        public const string DetailsTag = "details";
        #endregion

        private LangFlags _langFlags;

        public LangContext(LangFlags langFlags)
        {
            _langFlags = langFlags;
        }

        /// <summary>
        /// クラスの brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLClass classType)
        {
            return MakeText(classType.Document.OriginalBriefText, classType.Document, BriefTag);
        }

        /// <summary>
        /// メソッドの brief 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetBriefText(CLMethod method)
        {
            return MakeText(method.Document.OriginalBriefText, method.Document, BriefTag);
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

        /// <summary>
        /// クラスの details 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetDetailsText(CLClass classType)
        {
            return MakeText(classType.Document.OriginalDetailsText, classType.Document, DetailsTag);
        }

        /// <summary>
        /// メソッドの details 説明文
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public string GetDetailsText(CLMethod method)
        {
            return MakeText(method.Document.OriginalDetailsText, method.Document, DetailsTag);
        }

        /// <summary>
        /// return Param コメント作成
        /// (return として選択された Param のコメント作成で使用する)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="cppReturn"></param>
        public string GetReturnParamText(CLParam param)
        {
            if (param == null) return "";

            string text = GetParamText(param);
            // "演算結果を格納する Vector3 変数" 等の後半を切り取る
            var r = new Regex("を格納する.*");
            return r.Replace(text, "");
        }

        /// <summary>
        /// メソッドのオーバーライドコード
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public bool TryGetOverrideCode(CLMethod method, out string code)
        {
            var d = method.Option.OverrideOptions.Find((opt) => opt.LangFlags == _langFlags);
            if (d != null)
            {
                code = d.Code;
                return true;
            }
            code = "";
            return false;
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

    }
}
