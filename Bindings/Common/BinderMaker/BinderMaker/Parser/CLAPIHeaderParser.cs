using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker.Parser
{
    /// <summary>
    /// API の定義ファイル (ヘッダファイル) を解析する
    /// </summary>
    class CLAPIHeaderParser
    {
        /* Note: 
         *  TypeInitializationException が発生する場合、依存する別のパーサが初期化されていない可能性がある。
         *  パーサは static readonly で取っているので、上から順に初期化される。
         *  下の方で定義しているパーサを参照するとNG。
         */

        #region ドキュメントコメント

        // ドキュメントコメントの 1セクション の終了マーク
        public static readonly Parser<IEnumerable<char>> DocumentCommentSectionEnd =
            Parse.String("@").XOr(Parse.String("*/"));    // 次の @ または //*/

        // ドキュメントコメント - 概要
        public static readonly Parser<string> DocumentCommentBrief =
            from mark1      in Parse.String("@brief").GenericToken()                            // "@biref" から
            from summary1   in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select summary1;

        // ドキュメントコメント - 引数
        public static readonly Parser<CLParamDocument> DocumentCommentParam =
            from start      in Parse.String("@param").GenericToken()
            from io         in Parse.Regex(@"\[.+\]").Text()
            from name       in Parse.AnyChar.Until(Parse.Char(':'))                             // : までの全ての文字を取りだす。':' は消費される。
            from text       in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select new CLParamDocument(io, new string(name.ToArray()), text);

        // ドキュメントコメント - 戻り値
        public static readonly Parser<string> DocumentCommentReturn =
            from start      in Parse.String("@return").GenericToken()
            from text       in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント - 詳細
        public static readonly Parser<string> DocumentCommentDetails =
            from start      in Parse.String("@details").GenericToken()
            from text       in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select text;

        // ドキュメントコメント - 言語別上書
        public static readonly Parser<CLOverwriteDocument> DocumentCommentOverwrite =
            from start      in Parse.String("@overwrite").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from target     in Parse.Regex(@"\(.+\)").Text()
            from text       in Parse.AnyChar.Except(DocumentCommentSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select new CLOverwriteDocument(langs, target, text);

        // ドキュメントコメント - 言語別置換
        public static readonly Parser<CLReplaceDocument> DocumentCommentReplace =
            from start      in Parse.String("@replace").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from lp         in Parse.String("(\"\"")
            from fromText   in Parse.AnyChar.Until(Parse.String("\"\"")).Text()                 // "" までの任意の文字。"" は消費される。
            from comma      in Parse.String(",\"\"")
            from toText     in Parse.AnyChar.Until(Parse.String("\"\")")).Text()                // "") までの任意の文字。"") は消費される。
            select new CLReplaceDocument(langs, fromText, toText);

        // ドキュメントコメント - 言語別サンプルコード
        public static readonly Parser<CLExampleDocument> DocumentCommentExample =
            from start      in Parse.String("@example").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from text       in Parse.AnyChar.Until(Parse.String("@example_end")).Text()    // "@example_end" までの任意の文字。最後の "@example_end" は消費される。
            select new CLExampleDocument(langs, text);
        
        // ドキュメントコメント
        private static readonly Parser<CLDocument> DocumentComment =
            from start      in Parse.String("/**")
            from brief      in DocumentCommentBrief
            from params1    in DocumentCommentParam.Many()      // 引数 (0個以上)
            from return1    in DocumentCommentReturn.Or(Parse.Return(""))
            from details    in DocumentCommentDetails.Or(Parse.Return(""))
            from overwrite  in DocumentCommentOverwrite.Many()
            from replace    in DocumentCommentReplace.Many()
            from example    in DocumentCommentExample.Many()
            from text       in Parse.AnyChar.Until(Parse.String("*/")).Text()       // "*/" が見つかるまで任意の文字を繰り返す。見つかった "*/" は破棄される。
            select new CLDocument(brief, params1, return1, details, overwrite, replace, example);

        #endregion

        #region 関数宣言

        
        // 関数宣言 - API 修飾子
        private static readonly Parser<IEnumerable<char>> APIModifier =
            Parse.String("LUMINO_INSTANCE_API").Or(Parse.String("LUMINO_STATIC_API"));
        
        // 関数宣言 - 属性
        private static readonly Parser<IEnumerable<char>> FuncAttribute =
            Parse.String("LUMINO_PROPERTY");

        // 仮引数定義
        public static readonly Parser<CLParam> FuncParamDecl =
            from type       in ParserUtils.TypeName.GenericToken()
            from name       in ParserUtils.Identifier.GenericToken()
            from defaultVal in FuncParamDefault.XOr(Parse.Return(""))   // opt
            select new CLParam(type, name, defaultVal);

        // 仮引数定義並び
        public static readonly Parser<IEnumerable<CLParam>> FuncParamDecls =
            from first1     in FuncParamDecl                                    // 先頭の1つ
            from rest1      in Parse.Char(',').Then(_ => FuncParamDecl).Many()  // 以降の , 区切りの定義が 0 個以上
            select ParserUtils.Cons(first1, rest1);

        // デフォルト引数
        public static readonly Parser<string> FuncParamDefault =
            from mark       in Parse.String("LN_DEFAULT_ARG").GenericToken()
            from lparen     in Parse.Char('(').GenericToken()
            from value      in ParserUtils.IdentifierOrNumeric.GenericToken()
            from rparen     in Parse.Char(')').GenericToken()
            select value;

        // 関数宣言
        private static readonly Parser<CLFuncDecl> FuncDecl =
            from apiMod     in APIModifier.GenericToken()
            from attr       in FuncAttribute.Or(Parse.Return(""))       // opt
            from type1      in ParserUtils.TypeName.GenericToken()                  // 戻り値型
            from name1      in ParserUtils.Identifier.GenericToken()    // 関数名
            from lparen     in Parse.Char('(').GenericToken()
            from params1    in FuncParamDecls.Or(Parse.Return(new CLParam[0]))              // opt
            from rparen     in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new CLFuncDecl(type1, name1, params1);

        #endregion

        #region オプションコメント

        // オプションコメントの 1セクション の終了マーク
        public static readonly Parser<IEnumerable<char>> OptionSectionEnd =
            Parse.String("@").XOr(Parse.String("Option*/"));    // 次の @ または //*/

        // オプションコメント - disable
        private static readonly Parser<CLEntity> OptionDisable =        // 戻り値は規定の CLEntity にして他と合わせておかないと、OptionSection で「推論できませんでした」と怒られる。
            from start      in Parse.String("@disable").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from text       in Parse.AnyChar.Except(OptionSectionEnd).Many().Text()    // DocumentCommentSectionEnd までの任意の文字。最後の DocumentCommentSectionEnd は消費しない。
            select new CLDisableOption(langs);

        // オプションコメント - 関数のオーバーライド
        private static readonly Parser<CLEntity> OptionOverride =
            from start      in Parse.String("@override").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from text       in Parse.AnyChar.Until(Parse.String("@override_end")).Text()    // "@override_end" までの任意の文字。最後の "@override_end" は消費される。
            select new CLOverrideOption(langs, text);

        // オプションコメント - クラスへの追加コード
        private static readonly Parser<CLEntity> OptionClassAddCode =
            from start      in Parse.String("@class_add_code").GenericToken()
            from langs      in Parse.Regex(@"\[.+\]").Text()
            from text       in Parse.AnyChar.Until(Parse.String("@class_add_code_end")).Text()    // "@class_add_code_end" までの任意の文字。最後の "@class_add_code_end" は消費される。
            select new CLClassAddCodeOption(langs, text);

        // オプションコメント セクション
        private static readonly Parser<CLEntity> OptionSection =
                 OptionDisable
            .XOr(OptionOverride)
            .XOr(OptionClassAddCode);

        // オプションコメント
        private static readonly Parser<CLOption> OptionComment =
            from start      in Parse.String("/*Option")
            from options    in (OptionSection.GenericToken()).Many()         // セクションの集合 (前後にスペースを許可して 0 回以上の繰り返し)
            from text       in Parse.AnyChar.Until(Parse.String("Option*/")).Text() // "Option*/" が見つかるまで任意の文字を繰り返す。見つかった "Option*/" は破棄される。
            select new CLOption(options);

        #endregion

        #region フレームワーク

        // メソッド定義
        private static readonly Parser<CLMethod> MethodDecl =
            from doc        in DocumentComment.GenericToken()
            from func       in FuncDecl.GenericToken()
            from option     in (OptionComment.GenericToken()).Or(Parse.Return<CLOption>(null))
            select new CLMethod(doc, func, option);

        // LN_CLASS マクロ引数並び
        public static readonly Parser<IEnumerable<string>> ClassNamesDecl =
            from first1     in ParserUtils.Identifier                                       // 先頭の1つ
            from rest1      in Parse.Char(',').Then(_ => ParserUtils.Identifier  ).Many()  // 以降の , 区切りの定義が 0 個以上
            select ParserUtils.Cons(first1, rest1);

        // クラス定義
        private static readonly Parser<CLClass> ClassDecl =
            from doc        in DocumentComment.GenericToken()
            from start      in Parse.String("LN_CLASS").Or(Parse.String("LN_STATIC_CLASS")).Or(Parse.String("LN_GENERIC_CLASS"))
            from lparen     in Parse.Char('(').GenericToken()
            from names      in ClassNamesDecl
            from rparen     in Parse.Char(')').GenericToken()
            from lead       in Parse.AnyChar.Except(Parse.String("/**")).Many().Text()  // 最初のドキュメントコメントまでを読み飛ばす (/** は消費しない)
            from methods    in MethodDecl.Many()
            from classOpt   in (OptionComment.GenericToken()).Or(Parse.Return<CLOption>(null))
            from end        in Parse.String("LN_CLASS_END")
            select new CLClass(doc, names, methods, classOpt);

        // モジュール
        private static readonly Parser<CLModule> ModuleDecl =
            from doc        in DocumentComment.GenericToken()
            from start      in Parse.String("LN_MODULE")
            from lparen     in Parse.Char('(').GenericToken()
            from name       in ParserUtils.IdentifierOrNumeric.GenericToken()
            from rparen     in Parse.Char(')').GenericToken()
            from classes    in ClassDecl.Many()
            from end        in Parse.String("LN_MODULE_END").GenericToken()
            select new CLModule(doc, classes);

        // ルート要素
        private static readonly Parser<CLModule> CompileUnit =
            from lead       in Parse.AnyChar.Except(Parse.String("/**")).Many()   // ファイル先頭から最初のドキュメントコメントまでを読み飛ばす (/** は消費しない)
            from module     in ModuleDecl
            from any        in Parse.AnyChar.Many()     // 後は適当な文字
            select module;

        #endregion
        
        #region Methods

        private CLModule AnalyzeOneFile(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);
            text = text.Replace("\r\n", "\n");
            return CompileUnit.Parse(text);
        }

        /// <summary>
        /// 解析実行
        /// </summary>
        /// <param name="filePath"></param>
        public List<CLModule> Analyze(string[] filePaths)
        {
            var list = new List<CLModule>();
            foreach (var path in filePaths)
            {
                list.Add(AnalyzeOneFile(path));
            }
            return list;
        }
        #endregion
    }
}
