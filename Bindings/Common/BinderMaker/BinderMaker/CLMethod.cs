using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// メソッド
    /// </summary>
    class CLMethod : CLEntity
    {
        #region Properties
        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLDocument Document { get; private set; }

        /// <summary>
        /// 関数宣言
        /// </summary>
        public CLFuncDecl FuncDecl { get; private set; }

        /// <summary>
        /// オプション
        /// </summary>
        public CLOption Option { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLMethod(
            CLDocument document,
            CLFuncDecl funcDecl,
            CLOption option)
        {
            Document = document;
            FuncDecl = funcDecl;
            Option = option;
        }
        #endregion
    }

    /// <summary>
    /// 仮引数
    /// </summary>
    class CLParam : CLEntity
    {
        #region Fields

        private string _originalTypeName;
        private string _originalDefaultValue;

        #endregion

        #region Properties

        /// <summary>
        /// 仮引数名
        /// </summary>
        public string Name { get; private set; }

        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="typeName"></param>
        /// <param name="varName"></param>
        /// <param name="defaultValue"></param>
        public CLParam(string typeName, string varName, string defaultValue)
        {
            _originalTypeName = typeName;
            Name = varName;
            _originalDefaultValue = defaultValue;
        }

        #endregion
    }

    /// <summary>
    /// 関数定義
    /// </summary>
    class CLFuncDecl : CLEntity
    {
        #region Fields
        private string _originalReturnTypeName;
        #endregion

        #region Properties
        /// <summary>
        /// オリジナルの完全関数名 (LNAudio_PlayBGM 等)
        /// </summary>
        public string OriginalFullName { get; private set; }

        /// <summary>
        /// オリジナルの関数名 ("_" の後ろ。SetPositionXYZ 等)
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// 仮引数リスト
        /// </summary>
        public List<CLParam> Params { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLFuncDecl(
            string returnType,
            string name,
            IEnumerable<CLParam> params1)
        {
            _originalReturnTypeName = returnType;

            OriginalFullName = name;
            var tokens = name.Trim().Split('_');
            OriginalName = tokens[1];

            Params = new List<CLParam>(params1);
        }
        #endregion
    }
}
