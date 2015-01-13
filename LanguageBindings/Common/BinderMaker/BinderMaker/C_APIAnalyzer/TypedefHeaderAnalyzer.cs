using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker
{

    static class Parser
    {
        public static Parser<T> GenericToken<T>(this Parser<T> parser)
        {
            if (parser == null) throw new ArgumentNullException("parser");

            return from leading in LNParser.GenericSpace
                   from item in parser
                   from trailing in LNParser.GenericSpace
                   select item;
        }
    }

    class TypedefHeaderAnalyzer
    {
        
        // Doxygen の行コメント
        private static readonly Parser<string> DoxyEOLComment =
            from comment1   in Parse.String("///<")
            from comment2   in Parse.AnyChar.Except(Parse.Char('\n')).Many().Text()
            from eol        in Parse.Char('\n')
            select comment2;

        //---------------------------------------------------------------------
        // version

        //private static readonly Parser<Version> VersionDefine =
        //    from pp1    in Parse.String("#define")
        //    from ver1   in Parse.String("LNOTE_VERSION").GenericToken()
        //    from quart1 in Parse.Char('"')
        //    from str1   in Parse.AnyChar.Except(Parse.Char('"')).Many().Text()
        //    from quart2 in Parse.Char('"')
        //    from eol    in Parse.Char('\n')
        //    select new Version() { Version = str1 };


        //---------------------------------------------------------------------
        // enum

        private static readonly Parser<EnumMemberDecl> EnumMemberDecl =
            (
                // 定数定義のないenumメンバ
                from ident1 in LNParser.Identifier.GenericToken()                // GenericToken() を付けることで、前後の空白を許可する
                from comma1 in Parse.Char(',').GenericToken() 
                from comment1 in DoxyEOLComment.XOr(Parse.Return(""))   // opt (行コメントが無ければ空文字)
                select new EnumMemberDecl() { Name = ident1, Comment = comment1 }
            ).Or(   // ここは XOr にしてしまうと全くパースできなくなってしまう
                // 定数定義のあるenumメンバ
                from ident1 in LNParser.Identifier.GenericToken()
                from equal1 in Parse.Char('=').GenericToken()
                from ident2 in LNParser.Identifier.GenericToken()
                from comma1 in Parse.Char(',').GenericToken()           // 必ず , が付くこと
                from comment1 in DoxyEOLComment.XOr(Parse.Return(""))   // opt (行コメントが無ければ空文字)
                select new EnumMemberDecl() { Name = ident1, Value = ident2, Comment = comment1 }
            );

        private static readonly Parser<Node> EnumDecl =
            from k          in Parse.String("enum")
            from space1     in LNParser.GenericSpace
            from dummyName1 in LNParser.Identifier
            from space2     in LNParser.GenericSpace
            from lbrace1    in Parse.Char('{')
            from space3     in LNParser.GenericSpace
            from decls1     in EnumMemberDecl.Many()
            from end        in Parse.AnyChar.Until(Parse.Char('}'))
            select new EnumDecl() { Name = dummyName1, Members  = decls1 };






        private static readonly Parser<string> SpacedIdentifier =
                from space21     in LNParser.GenericSpace
                from ident21     in LNParser.Identifier
                from space22     in LNParser.GenericSpace
                select ident21;

        /// <summary>
        /// , 区切り識別子 (例: A, B, C)
        /// </summary>
        private static readonly Parser<IEnumerable<string>> CommadIdentifiers =
            //from ident1     in Identifier
            //from space1     in GenericSpace
            from comma1 in Parse.Char(',').Then(_ => SpacedIdentifier).Many()
            select comma1;// Cons(ident1, comma1);

        private static readonly Parser<IEnumerable<StructMemberDecl>> StructMemberDecl =
            from ident1     in LNParser.Identifier
            from space1     in LNParser.GenericSpace
            from ident2     in LNParser.Identifier           // 最初のメンバ名
            from idents1    in CommadIdentifiers.XOr(Parse.Return(new string[0]))  // , 以降のメンバ名
            from space2     in LNParser.GenericSpace
            from comma1     in Parse.Char(';')      // 文末
            from space3     in LNParser.GenericSpace
            from comment1   in DoxyEOLComment.XOr(Parse.Return(""))     // opt (行コメントが無ければ空文字)
            select CreateStructMemberDeclList(ident1, ident2, idents1, comment1);

        private static readonly Parser<StructDecl> StructDecl =
            from keyword    in Parse.String("struct")
            from space1     in LNParser.GenericSpace
            from name1      in LNParser.Identifier
            from space2     in LNParser.GenericSpace
            from lbrace1    in Parse.Char('{')
            from space3     in LNParser.GenericSpace
            from decls1     in StructMemberDecl.GenericToken().Many()
            from end        in Parse.AnyChar.Until(Parse.Char('}'))
            select new StructDecl(name1, decls1);




        private static readonly Parser<Node> EnumDeclOrStructDecl =
            from leading    in Parse.AnyChar.Except(Parse.String("enum").Or(Parse.String("struct"))).Many()    // enum の前まで飛ばす
            from e2         in EnumDecl.Or(StructDecl)
            select e2;

        private static readonly Parser<IEnumerable<Node>> CompileUnit =
            from decls1     in EnumDeclOrStructDecl.Many()
            select decls1;
            //from leading in Parse.WhiteSpace.Many()
            //from e in Parse.AnyChar.Until(Parse.String("enum").XOr(Parse.String("struct")))
            //from e1 in Parse.AnyChar.Except(Parse.String("enum")).Many()
            //from e2 in EnumDecl//Parse.AnyChar.Until(EnumDecl)
            //select e2;
            // from e in Parse.AnyChar.Until(Parse.String("enum").XOr(Parse.String("struct"))).Text()
            // select e;
            //(
            //    from e in Parse.AnyChar.Until(Parse.String("enum")).Text()
            //    select "enum"
            //).XOr(
            //    from e in Parse.AnyChar.Until(Parse.String("struct")).Text()
            //    select "struct"
            //);

        static IEnumerable<T> Cons<T>(T head, IEnumerable<T> rest)
        {
            yield return head;
            foreach (var item in rest)
                yield return item;
        }

        static IEnumerable<StructMemberDecl> CreateStructMemberDeclList(string type, string firstName, IEnumerable<string> restNames, string comment)
        {
            var list = new List<StructMemberDecl>();
            list.Add(new StructMemberDecl() { Name = firstName, Type = type, Comment = comment });
            foreach (var r in restNames)
            {
                list.Add(new StructMemberDecl() { Name = r, Type = type, Comment = comment });
            }
            return list;
        }


        /// <summary>
        /// 解析実行
        /// </summary>
        /// <param name="filePath"></param>
        public IEnumerable<Node> Analyze(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);

            // パース
            IEnumerable<Node> nodes = CompileUnit.Parse(text);
            foreach (var node in nodes)
            {
                node.PostAnalyze();
            }
            return nodes;
        }
    }
}
