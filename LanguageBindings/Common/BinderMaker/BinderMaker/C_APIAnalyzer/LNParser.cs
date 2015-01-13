using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Sprache;

namespace BinderMaker
{
    class LNParser
    {
        /// <summary>
        /// 識別子
        /// </summary>
        public static readonly Parser<string> Identifier =
            from ident in Parse.LetterOrDigit.XOr(Parse.Char('-')).XOr(Parse.Char('_')).Many()
            select new string(ident.ToArray());

        /// <summary>
        /// 空白
        /// </summary>
        public static readonly Parser<IEnumerable<char>> GenericSpace =
            Parse.WhiteSpace.Many() // 改行などは含んでいない
            .XOr(
               Parse.Char('\n').Many()
            );    // プリプロとかは考慮しない


        /// <summary>
        /// 型名 (ポインタの * を含む)
        /// </summary>
        public static readonly Parser<string> TypeName =
            from modif1 in Parse.String("const").GenericToken().XOr(Parse.Return("")).Text()   // opt 修飾子 (const とか)
            from ident  in Identifier.GenericToken()
            from ast1   in (Parse.String("*").GenericToken()).XOr(Parse.Return("")).Text()   // opt
            from ast2   in (Parse.String("*").GenericToken()).XOr(Parse.Return("")).Text()   // opt ダブルポインタ許可
            select (modif1 + ((string.IsNullOrEmpty(modif1)) ? "" : " ") + new string(ident.ToArray()) + ast1 + ast2);

        public static IEnumerable<T> Cons<T>(T head, IEnumerable<T> rest)
        {
            yield return head;
            foreach (var item in rest)
                yield return item;
        }
    }
}
