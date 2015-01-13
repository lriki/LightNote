using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker
{
    class APIHeaderAnalyzer
    {
        /*
         *  Doxygen コメントは @名前 ～ 次の @ または 終端までの範囲を一度に取出し、
         *  各行頭の // は PostAnalyze() で削除している。
         */

        // 関数コメントの開始マーク
        public static readonly Parser<IEnumerable<char>> FuncHeaderCommentStart =
            Parse.String("///**");

        // 関数コメントの終了マーク
        public static readonly Parser<IEnumerable<char>> FuncHeaderCommentEnd =
            Parse.String("//*/");

        // 関数コメント1セクションの終了マーク
        public static readonly Parser<IEnumerable<char>> FuncDocSecEnd =
            Parse.String("@").XOr(Parse.String("//*/"));    // 次の @ または //*/

        // 関数コメント - 概要
        public static readonly Parser<string> FuncDocSummary =
            from mark1      in Parse.String("@brief").GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // 関数コメント - 引数
        public static readonly Parser<FuncParamDocNode> FuncDocParam =
            from mark1      in Parse.String("@param").GenericToken()
            from io1        in Parse.Regex(@"\[.+\]").Text()
            from name1      in Parse.AnyChar.Until(Parse.Char(':'))     // : までの全ての文字を取りだす
            //from name1      in LNParser.Identifier.GenericToken()
            //from delim1     in Parse.Char(':').GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select new FuncParamDocNode() { IO = io1, Name = new string(name1.ToArray()), Summary = summary1 };
        
        // 関数コメント - 引数の集合
        public static readonly Parser<IEnumerable<FuncParamDocNode>> FuncDocParams =
            from params1    in FuncDocParam.Many()
            select params1;


        // 関数コメント - 戻り値
        public static readonly Parser<string> FuncDocReturn =
            from mark1      in Parse.String("@return").GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // 関数コメント - 詳細
        public static readonly Parser<string> FuncDocDetail =
            from mark1      in Parse.String("@details").GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // 関数コメント - override
        public static readonly Parser<string> FuncDocOverride =
            from mark1 in Parse.String("@override").GenericToken()
            from summary1 in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;
        
        // 関数ヘッダコメント
        public static readonly Parser<FuncDocNode> FuncDoc =
            from mark1      in FuncHeaderCommentStart.GenericToken()
            from leading1   in Parse.AnyChar.Except(Parse.String("@")).Many()         // @ まで飛ばす
            from summary1   in FuncDocSummary
            from params1    in FuncDocParams.XOr(Parse.Return(new FuncParamDocNode[0]))   // opt
            from return1    in FuncDocReturn.Or(Parse.Return(""))              // opt   (Or じゃないと次の par が失敗する…)
            from override1  in FuncDocOverride.Or(Parse.Return(""))              // opt
            from detail1    in FuncDocDetail.Or(Parse.Return(""))              // opt
            from mark2      in FuncHeaderCommentEnd.GenericToken()
            select new FuncDocNode() { Summary = summary1, Params = params1, Return = return1, Detail = detail1, Override = override1 };

        // モジュールコメント - @defgroup
        public static readonly Parser<ModuleGroupInfo> ModuleDocGroupDef =
            from mark1      in Parse.String("@defgroup").GenericToken()             // コマンド文字列
            from name1      in LNParser.Identifier.GenericToken()                   // グループ名
            from title1     in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // グループタイトル (FuncDocSecEnd までの任意の文字
            select new ModuleGroupInfo() { GroupName = name1, Title = title1 };

        // モジュールコメント - 概要
        public static readonly Parser<string> ModuleDocSummary =
            from mark1      in Parse.String("@brief").GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // モジュールコメント - 継承
        public static readonly Parser<string> ModuleDocExtends =
            from mark1 in Parse.String("@extends").GenericToken()
            from summary1 in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // その他の Doxygen コマンド
        public static readonly Parser<string> OtherDoxygenCommand =
            from mark1      in Parse.String("@{").GenericToken()
            from summary1   in Parse.AnyChar.Except(FuncDocSecEnd).Many().Text()    // FuncDocSecEnd までの任意の文字
            select summary1;

        // モジュールヘッダコメント
        public static readonly Parser<Node> ModuleDoc =
            from mark1      in FuncHeaderCommentStart.GenericToken()
            from leading1   in Parse.AnyChar.Except(Parse.String("@")).Many()       // @ まで飛ばす
            from groupdef1  in ModuleDocGroupDef                                    // @groupdef
            from summary1   in ModuleDocSummary                                     // @brief
            from detail1    in FuncDocDetail.Or(Parse.Return(""))                   // @details     ※(XOr じゃダメ。XOr は消費を戻さない)
            from extends1   in ModuleDocExtends.Or(Parse.Return(""))                // @extends
            from other1     in OtherDoxygenCommand                                  // @{
            from mark2      in FuncHeaderCommentEnd.GenericToken()
            select new ModuleDocNode() { GroupInfo = groupdef1, Summary = summary1, Detail = detail1, Extends = extends1 };

        // その他のドキュメントコメント (ファイルヘッダなど)
        public static readonly Parser<Node> OtherDoc =
            from mark1      in FuncHeaderCommentStart.GenericToken()
            from leading1   in Parse.AnyChar.Until(FuncHeaderCommentEnd)    // コメント終端まで飛ばす
            select new Node();

        // デフォルト引数
        public static readonly Parser<string> FuncParamDefault =
            from mark1      in Parse.String("LNOTE_DEFAULT_ARG").GenericToken()
            from lparen1    in Parse.Char('(').GenericToken()
            from value1     in LNParser.Identifier.GenericToken()
            from rparen1    in Parse.Char(')').GenericToken()
            select value1;

        // 仮引数定義
        public static readonly Parser<FuncParamDeclNode> FuncParamDecl =
            from type1      in LNParser.TypeName.GenericToken()
            from name1      in LNParser.Identifier.GenericToken()
            from default1   in FuncParamDefault.XOr(Parse.Return(""))   // opt
            select new FuncParamDeclNode() { Type = type1, Name = name1, DefaultValue = default1 };

        // 仮引数定義並び
        public static readonly Parser<IEnumerable<FuncParamDeclNode>> FuncParamDecls =
            from first1     in FuncParamDecl                                    // 先頭の1つ
            from rest1      in Parse.Char(',').Then(_ => FuncParamDecl).Many()  // 以降の , 区切りの定義
            select LNParser.Cons(first1, rest1);

        // 関数定義
        public static readonly Parser<FuncDeclNode> FuncDecl =
            from mark1      in Parse.String("LNOTEAPI").GenericToken()
            from type1      in LNParser.TypeName.GenericToken()
            from name1      in LNParser.Identifier.GenericToken()
            from lparen1    in Parse.Char('(').GenericToken()
            from params1    in FuncParamDecls.XOr(Parse.Return(new FuncParamDeclNode[0]))              // opt
            from rparen1    in Parse.Char(')').GenericToken()
            from end1       in Parse.Char(';').GenericToken()
            select new FuncDeclNode() { ReturnType = type1, Name = name1, Params = params1 };

        // 定義オプション
        public static readonly Parser<FuncOptionNode> FuncDeclOption =
            from mark1      in Parse.String("/*DeclOpt").GenericToken()
            from code1      in Parse.AnyChar.Until(Parse.String("DeclOpt*/")).Text()
            select new FuncOptionNode() { Code = code1 };
            
        
        

        private static readonly Parser<Node> LeadingContents =
            from leading1   in Parse.AnyChar.Except(FuncHeaderCommentStart.Or(FuncDecl.Return("").Or(Parse.String("/*DeclOpt")))).Many()    // いずれかの開始パターンにたどり着くまで飛ばす
            from decls1     in ModuleDoc.Or(FuncDoc).Or(OtherDoc).Or(FuncDecl).Or(FuncDeclOption)//FuncDoc.Or(OtherDoc.Or(FuncDecl).Or(FuncDeclOption))
            select decls1;
        // ↑FuncDecl.Return("") は定義自体を開始パターンにしたもの。全く同じ解析が2回走るので重い　

        private static readonly Parser<IEnumerable<Node>> CompileUnit =
            from nodes1     in LeadingContents.Many()
            select nodes1;

        /// <summary>
        /// 解析実行
        /// </summary>
        /// <param name="filePath"></param>
        public IEnumerable<Node> Analyze(string[] filePaths)
        {
            var nodeList = new List<Node>();
            foreach (var path in filePaths)
            {
                string text = System.IO.File.ReadAllText(path);
                IEnumerable<Node> nodes = CompileUnit.Parse(text);
                foreach (var node in nodes)
                {
                    node.PostAnalyze();
                }
                nodeList.AddRange(nodes);
            }

            return nodeList;
            //string text = System.IO.File.ReadAllText(filePath);
            //IEnumerable<Node> nodes = CompileUnit.Parse(text);
            //foreach (var node in nodes)
            //{
            //    node.PostAnalyze();
            //}
            //return nodes;
        }
    }
}
