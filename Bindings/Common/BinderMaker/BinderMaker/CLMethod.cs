using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLMethod
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLMethod(
            CLDocument document,
            CLFuncDecl funcDecl,
            CLOption option)
        {
        }
    }

    class CLParam
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="typeName"></param>
        /// <param name="varName"></param>
        /// <param name="defaultValue"></param>
        public CLParam(string typeName, string varName, string defaultValue)
        {

        }
    }

    class CLFuncDecl
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLFuncDecl(
            string returnType,
            string name,
            IEnumerable<CLParam> params1)
        {

            Console.WriteLine(name);

        }
    }
}
