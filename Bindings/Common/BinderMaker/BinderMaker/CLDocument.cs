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

    [Flags]
    enum ParamAttribute
    {
        None = 0x00,
        In = 0x01,
        Out = 0x02,
    }

    /// <summary>
    /// モジュール、クラス、メソッドのドキュメント
    /// </summary>
    class CLDocument : CLEntity
    {
        #region Fields

        private List<CLReplaceDocument> _replaceDocs;
        private List<CLPostscriptDocument> _postscriptDocs;
        private List<CLOverwriteDocument> _overwriteDocs;
        private List<CLExampleDocument> _exampleDocs;

        #endregion

        #region Properties

        /// <summary>
        /// オリジナルの概要テキスト
        /// </summary>
        public string OriginalBriefText { get; private set; }

        /// <summary>
        /// オリジナルの仮引数リスト
        /// </summary>
        public List<CLParamDocument> OriginalParams { get; private set; }

        /// <summary>
        /// オリジナルの戻り値テキスト
        /// </summary>
        public string OriginalReturnText { get; private set; }

        /// <summary>
        /// オリジナルの詳細テキスト
        /// </summary>
        public string OriginalDetailsText { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLDocument(
            string briefText,
            IEnumerable<CLParamDocument> paramDocs,
            string returnText,
            string detailsText,
            IEnumerable<CLReplaceDocument> replaceDocs,
            IEnumerable<CLPostscriptDocument> postscriptDocs,
            IEnumerable<CLOverwriteDocument> overwriteDocs,
            IEnumerable<CLExampleDocument> exampleDocs)
        {
            OriginalBriefText = briefText.Trim();   // 渡されるテキストは前後に空白が入っていたりするので Trim() とかしておく
            OriginalParams = new List<CLParamDocument>(paramDocs);
            OriginalReturnText = returnText.Trim();
            OriginalDetailsText = detailsText.Trim();

            _replaceDocs = new List<CLReplaceDocument>(replaceDocs);
            _postscriptDocs = new List<CLPostscriptDocument>(postscriptDocs);
            _overwriteDocs = new List<CLOverwriteDocument>(overwriteDocs);
            _exampleDocs = new List<CLExampleDocument>(exampleDocs);
        }

        /// <summary>
        /// 言語名を示す , 区切りのテキストから LangFlags の組み合わせを取得する
        /// </summary>
        /// <param name="langsText">"[]" は含まれていてもよい。空白は削除される。</param>
        /// <returns></returns>
        public static LangFlags MakeLangFlags(string langsText)
        {
            LangFlags flags = 0;
            langsText = langsText.Trim();
            langsText = langsText.Replace("[", "");
            langsText = langsText.Replace("]", "");
            string[] tokens = langsText.Split(',');
            foreach (var token in tokens)
            {
                var t = token.Trim();
                if (t == "c")
                    flags |= LangFlags.C;
                else if (t == "cpp")
                    flags |= LangFlags.Cpp;
                else if (t == "cs")
                    flags |= LangFlags.CS;
                else if (t == "java")
                    flags |= LangFlags.Java;
                else if (t == "rb")
                    flags |= LangFlags.Ruby;
                else if (t == "py")
                    flags |= LangFlags.Python;
                else if (t == "lua")
                    flags |= LangFlags.Lua;
                else if (t == "hsp")
                    flags |= LangFlags.HSP;
                else
                    throw new InvalidOperationException("invalid lang code.");
            }
            return flags;
        }

        #endregion
    }

    /// <summary>
    /// 仮引数のドキュメント
    /// </summary>
    class CLParamDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 入出力
        /// </summary>
        public ParamAttribute Attribute { get; private set; }

        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// テキスト
        /// </summary>
        public string OriginalText { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="io"></param>
        /// <param name="name"></param>
        /// <param name="text"></param>
        public CLParamDocument(string io, string name, string text)
        {
            // io は "[]" が含まれている。IndexOf で探す
            Attribute = 0;
            Attribute |= (io.IndexOf("in") >= 0) ? ParamAttribute.In : 0;
            Attribute |= (io.IndexOf("out") >= 0) ? ParamAttribute.Out : 0;
            if (Attribute == 0)
                throw new InvalidOperationException("io empty");  // IO が空等

            Name = name.Trim();
            OriginalText = text.Trim();
        }

        #endregion
    }

    /// <summary>
    /// @extends ドキュメント
    /// </summary>
    class CLExtendsDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// オリジナルのクラス名
        /// </summary>
        public string OriginalClassName { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="className"></param>
        public CLExtendsDocument(string langs, string className)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            OriginalClassName = className.Trim();
        }

        #endregion
    }

    /// <summary>
    /// @replace ドキュメント
    /// </summary>
    class CLReplaceDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// 置換元文字列
        /// </summary>
        public string From { get; private set; }

        /// <summary>
        /// 置換先文字列
        /// </summary>
        public string To { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="from"></param>
        /// <param name="to"></param>
        public CLReplaceDocument(string langs, string from, string to)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            From = from.Trim('"');  // 前後から " を削除
            To = to.Trim('"');  // 前後から " を削除
        }

        #endregion
    }

    /// <summary>
    /// @postscript ドキュメント
    /// </summary>
    class CLPostscriptDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// 追記先セクション名
        /// </summary>
        public string TargetSection { get; private set; }

        /// <summary>
        /// 追記文字列
        /// </summary>
        public string Text { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="target"></param>
        /// <param name="text"></param>
        public CLPostscriptDocument(string langs, string target, string text)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            TargetSection = target.Trim();
            Text = text.Trim();
        }

        #endregion
    }

    /// <summary>
    /// @overwrite ドキュメント
    /// </summary>
    class CLOverwriteDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// 追記先セクション名
        /// </summary>
        public string TargetSection { get; private set; }

        /// <summary>
        /// 上書き文字列
        /// </summary>
        public string Text { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="target"></param>
        /// <param name="text"></param>
        public CLOverwriteDocument(string langs, string target, string text)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            TargetSection = target.Trim();
            Text = text.Trim();
        }

        #endregion
    }

    /// <summary>
    /// @example ドキュメント
    /// </summary>
    class CLExampleDocument : CLEntity
    {
        #region Properties

        /// <summary>
        /// 言語別有効フラグ
        /// </summary>
        public LangFlags LangFlags { get; private set; }

        /// <summary>
        /// サンプルコード
        /// </summary>
        public string Code { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="text"></param>
        public CLExampleDocument(string langs, string text)
        {
            LangFlags = CLDocument.MakeLangFlags(langs);
            Code = text.Trim();
        }

        #endregion
    }
}
