using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{

    class CLOption : CLEntity
    {
        /// <summary>
        /// コンストラクタ (空のオプション)
        /// </summary>
        public CLOption()
        {

        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="options"></param>
        public CLOption(IEnumerable<CLEntity> options)
        {

        }
    }


    class CLDisableOption : CLEntity
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        public CLDisableOption(string langs)
        {

        }
    }

    class CLOverrideOption : CLEntity
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="code"></param>
        public CLOverrideOption(string langs, string code)
        {

        }
    }

    class CLClassAddCodeOption : CLEntity
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="langs"></param>
        /// <param name="code"></param>
        public CLClassAddCodeOption(string langs, string code)
        {

        }
    }
}
