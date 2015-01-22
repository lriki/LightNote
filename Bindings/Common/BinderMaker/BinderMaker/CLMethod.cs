using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// メソッド修飾子
    /// </summary>
    enum MethodModifier
    {
        Instance,
        Static,
        Internal,   // (デフォルト)
    }

    /// <summary>
    /// メソッド属性
    /// </summary>
    enum MethodAttribute
    {
        None,
        Property,
    }

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

        /// <summary>
        /// このメソッドをオーバーロードするメソッドリスト
        /// </summary>
        public List<CLMethod> Overloads { get; private set; }

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
            Overloads = new List<CLMethod>();

            FuncDecl.OwnerMethod = this;

            // 仮引数とコメントを関連付ける
            foreach (var param in FuncDecl.Params)
            {
                var doc = Document.OriginalParams.Find((d) => d.Name == param.Name);
                if (doc == null) throw new InvalidOperationException("invalid param name.");

                param.Document = doc;
            }
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、階層的に Manager の管理リストに登録する
        /// </summary>
        public override void Register()
        {
            base.Register();
            Manager.AllMethods.Add(this);
            Document.Register();
            FuncDecl.Register();
            Option.Register();
        }

        /// <summary>
        /// オーバーロードメソッドの関係をリンクする
        /// </summary>
        public void LinkOverloads()
        {
            // コンストラクタであれば Create までが一致するかを確認する
            if (FuncDecl.IsConstructor)
            {
                int idx = FuncDecl.OriginalFullName.IndexOf("_Create");
                string name = FuncDecl.OriginalFullName.Substring(0, idx + 7);

                // 検索 (サフィックスというものが無いので、最初に見つかったものにリンクする)
                var targetMethod = Manager.AllMethods.Find((m) => (m.FuncDecl.OriginalFullName.IndexOf(name) == 0));
                if (targetMethod != this)   // 自分にリンクしちゃダメ
                {
                    // リンク
                    if (targetMethod == null) throw new InvalidOperationException("invalid constructor overload.");
                    targetMethod.Overloads.Add(this);
                }
            }
            // このメソッドが別のメソッドのオーバーロードであれば、
            // 別のメソッドを検索してリンクする
            else if (FuncDecl.IsOverload)
            {
                // 例) LNSoundListener_SetUpDirectionXYZ → LNSoundListener_SetUpDirection
                string newName;
                Manager.RemoveOverloadSuffix(FuncDecl.OriginalFullName, out newName);

                // 検索
                var targetMethod = Manager.AllMethods.Find((m) => m.FuncDecl.OriginalFullName == newName);
                
                // 繋げる
                if (targetMethod == null) throw new InvalidOperationException("invalid overload.");
                targetMethod.Overloads.Add(this);
            }
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
        /// 親メソッド
        /// </summary>
        public CLMethod OwnerMethod { get; set; }

        /// <summary>
        /// オリジナルの完全関数名 (LNAudio_PlayBGM 等)
        /// </summary>
        public string OriginalFullName { get; private set; }

        /// <summary>
        /// オリジナルの関数名 ("_" の後ろ。SetPositionXYZ 等)
        /// </summary>
        public string OriginalName { get; private set; }

        /// <summary>
        /// 関数名 (OverloadSuffix は含まない)
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// 仮引数リスト
        /// </summary>
        public List<CLParam> Params { get; private set; }

        /// <summary>
        /// 修飾子
        /// </summary>
        public MethodModifier Modifier { get; private set; }

        /// <summary>
        /// 属性
        /// </summary>
        public MethodAttribute Attribute { get; private set; }

        /// <summary>
        /// コンストラクタである
        /// </summary>
        public bool IsConstructor { get; private set; }

        /// <summary>
        /// 別のメソッドをオーバーロードする場合は true (オリジナル名がサフィックスを持っていた)
        /// </summary>
        public bool IsOverload { get; private set; }

        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLFuncDecl(
            IEnumerable<char> apiModifier,
            IEnumerable<char> apiAtribute,
            string returnType,
            string name,
            IEnumerable<CLParam> params1)
        {
            // 修飾子の決定
            string modifier = new string(apiModifier.ToArray());
            if (modifier.Contains(CLManager.APIModifier_Instance))
                Modifier = MethodModifier.Instance;
            else if (modifier.Contains(CLManager.APIModifier_Static))
                Modifier = MethodModifier.Static;
            else
                Modifier = MethodModifier.Internal;

            // 属性の決定
            string attr = new string(apiModifier.ToArray());
            if (attr.Contains(CLManager.APIAttribute_Property))
                Attribute = MethodAttribute.Property;
            else
                Attribute = MethodAttribute.None;

            _originalReturnTypeName = returnType;

            OriginalFullName = name;
            var tokens = name.Trim().Split('_');
            OriginalName = tokens[1];

            Params = new List<CLParam>(params1);
            Params.ForEach((param) => param.OwnerFunc = this);  // 所持クラス割り当て

            // オーバーロードチェック (_ や サフィックスの含まない名前を作る)
            string newName;
            IsOverload = Manager.RemoveOverloadSuffix(OriginalName, out newName);
            Name = newName;

            // 先頭が Create ならコンストラクタ関数
            IsConstructor = (string.Compare(Name, 0, "Create", 0, 6) == 0);
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、階層的に Manager の管理リストに登録する
        /// </summary>
        public override void Register()
        {
            base.Register();
            Params.ForEach((p) => p.Register());
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
        /// 型
        /// </summary>
        public CLType Type { get; private set; }

        /// <summary>
        /// 仮引数名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// 親関数
        /// </summary>
        public CLFuncDecl OwnerFunc { get; set; }

        /// <summary>
        /// ドキュメントコメント
        /// </summary>
        public CLParamDocument Document { get; set; }
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

            if (string.IsNullOrEmpty(Name))
            {
                Console.WriteLine();
            }
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public override void LinkTypes()
        {
            Type = Manager.FindType(_originalTypeName);
        }
        #endregion
    }
}
