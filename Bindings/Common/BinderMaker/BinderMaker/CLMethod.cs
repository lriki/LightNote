﻿using System;
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
        StructConstructor,
        LibraryInitializer,
        LibraryTerminator,
    }

    /// <summary>
    /// プロパティ名種別
    /// </summary>
    enum PropertyNameType
    {
        NotProperty,
        Get,
        Set,
        Is,
    }

    /// <summary>
    /// メソッド
    /// </summary>
    class CLMethod : CLEntity
    {
        #region Fields
        #endregion

        #region Properties
        /// <summary>
        /// ドキュメント
        /// </summary>
        public CLClass OwnerClass { get; set; }

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
        /// メソッド名 (コンストラクタであれば親クラス名、そうでなければ関数名)
        /// </summary>
        public string Name { get { return (IsRefObjectConstructor) ? OwnerClass.Name : FuncDecl.Name; } }

        /// <summary>
        /// このメソッドをオーバーロードするメソッドリスト
        /// </summary>
        public List<CLMethod> Overloads { get; private set; }

        /// <summary>
        /// 仮引数リスト (return 振り分け済み。オリジナルを使いたいときは FuncDecl.Param を使う)
        /// </summary>
        public List<CLParam> Params { get; private set; }

        /// <summary>
        /// インスタンスメソッドのとき、自分自身を表す第1引数として選択された Param
        /// </summary>
        public CLParam ThisParam { get; private set; }

        /// <summary>
        /// return として選択された Param
        /// </summary>
        public CLParam ReturnParam { get; private set; }

        /// <summary>
        /// 戻り値型 (オリジナルの仮引数リストから選択されていなければ常に Void)
        /// </summary>
        public CLType ReturnType { get { return (ReturnParam != null) ? ReturnParam.Type : CLPrimitiveType.Void; } }

        /// <summary>
        /// 修飾子
        /// </summary>
        public MethodModifier Modifier { get { return FuncDecl.Modifier; } }

        /// <summary>
        /// インスタンスメソッドであるか
        /// </summary>
        public bool IsInstanceMethod 
        { 
            get 
            {
                return
                    FuncDecl.Modifier == MethodModifier.Instance ||
                    IsRefObjectConstructor;
            } 
        }

        /// <summary>
        /// コンストラクタであるか ("Create" または LN_STRUCT_CONSTRUCTOR)
        /// </summary>
        public bool IsRefObjectConstructor { get { return FuncDecl.IsRefObjectConstructor; } }

        /// <summary>
        /// ライブラリの初期化関数であるか
        /// </summary>
        public bool IsLibraryInitializer { get { return FuncDecl.Attribute == MethodAttribute.LibraryInitializer; } }

        /// <summary>
        /// ライブラリの終了処理関数であるか
        /// </summary>
        public bool IsLibraryTerminator { get { return FuncDecl.Attribute == MethodAttribute.LibraryTerminator; } }

        /// <summary>
        /// static メソッドであるか
        /// </summary>
        public bool IsStatic { get { return !IsInstanceMethod; } }

        /// <summary>
        /// プロパティ種別
        /// </summary>
        public PropertyNameType PropertyNameType { get; set; }
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

            // internal の場合はドキュメント無し。ダミーを作る
            if (FuncDecl.Modifier == MethodModifier.Internal)
            {
                foreach (var param in FuncDecl.Params)
                {
                    var doc = new CLParamDocument("in", param.Name, "");
                    Document.OriginalParams.Add(doc);
                    param.Document = doc;
                }
            }
            // 仮引数とコメントを関連付ける
            else
            {
                foreach (var param in FuncDecl.Params)
                {
                    var doc = Document.OriginalParams.Find((d) => d.Name == param.Name);
                    if (doc == null) throw new InvalidOperationException("invalid param name.");
                    param.Document = doc;
                }
            }

            // return とする引数をチェック & 振り分け
            // out 属性で一番後ろの引数を選択する。
            // (out x, out y) のように 2 つ以上 out がある場合は return としない。
            Params = new List<CLParam>();
            //if (FuncDecl.Params.Count > 0 && !IsConstructor) // out ひとつだけのコンストラクタがこれにマッチするのでここではじく
            {
                foreach (var param in FuncDecl.Params)
                {
                    if (param == FuncDecl.Params.First() && IsInstanceMethod)
                        ThisParam = param;      // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                    else if (param == FuncDecl.Params.Last() &&
                        ((param.IOModifier & IOModifier.In) == 0 && (param.IOModifier & IOModifier.Out) != 0))
                        ReturnParam = param;   // 終端かつ out の場合は return 扱い。
                    else
                        Params.Add(param);
                }
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
            if (FuncDecl.IsRefObjectConstructor)
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
        /// 戻り値型
        /// </summary>
        public CLType ReturnType { get; private set; }

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
        public bool IsRefObjectConstructor { get; private set; }

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
            string attr = new string(apiAtribute.ToArray());
            Attribute = MethodAttribute.None;
            if (attr.Contains(CLManager.APIAttribute_Property))
                Attribute = MethodAttribute.Property;
            if (attr.Contains(CLManager.APIAttribute_StructConstructor))
                Attribute = MethodAttribute.StructConstructor;
            if (attr.Contains(CLManager.APIAttribute_LibraryInitializer))
                Attribute = MethodAttribute.LibraryInitializer;
            if (attr.Contains(CLManager.APIAttribute_LibraryTerminator))
                Attribute = MethodAttribute.LibraryTerminator;
            

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
            IsRefObjectConstructor = (string.Compare(Name, 0, "Create", 0, 6) == 0);
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、階層的に Manager の管理リストに登録する
        /// </summary>
        public override void Register()
        {
            base.Register();
            Params.ForEach((p) => p.Register());
        }

        /// <summary>
        /// 必要に応じてサブクラスでオーバーライドされ、オリジナルの型名から CLType を検索して参照する
        /// </summary>
        public override void LinkTypes()
        {
            ReturnType = Manager.FindType(_originalReturnTypeName);
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
        /// デフォルト値
        /// </summary>
        public string OriginalDefaultValue { get; private set; }

        /// <summary>
        /// 親関数
        /// </summary>
        public CLFuncDecl OwnerFunc { get; set; }

        /// <summary>
        /// ドキュメントコメント
        /// </summary>
        public CLParamDocument Document { get; set; }

        /// <summary>
        /// 入出力
        /// </summary>
        public IOModifier IOModifier { get { return Document.IOModifier; } }

        /// <summary>
        /// out string 型であるか
        /// </summary>
        public bool IsOutStringType { get { return (Type == CLPrimitiveType.String && IOModifier == BinderMaker.IOModifier.Out); } }

        /// <summary>
        /// out RefObject 型であるか
        /// </summary>
        public bool IsOutRefObjectType { get { return (CLType.CheckRefObjectType(Type) && IOModifier == BinderMaker.IOModifier.Out); } }
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
            _originalTypeName = typeName.Trim();
            Name = varName.Trim();
            OriginalDefaultValue = defaultValue.Trim();

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

    /// <summary>
    /// プロパティ。get set の2メソッドのペア。
    /// </summary>
    class CLProperty
    {
        #region Properties
        /// <summary>
        /// プロパティ名
        /// </summary>
        public string Name { get; private set; }

        /// <summary>
        /// getter
        /// </summary>
        public CLMethod Getter { get; set; }

        /// <summary>
        /// setter
        /// </summary>
        public CLMethod Setter { get; set; }

        /// <summary>
        /// 型
        /// </summary>
        public CLType Type
        {
            get
            {
                // ※getter 優先にしておく。(Getter がある場合、 Type は return であることが前提)
                // getter の戻り値型
                if (Getter != null) return Getter.ReturnType;
                // setter
                if (Setter != null) return Setter.Params[0].Type;
                throw new InvalidOperationException();
            }
        }

        /// <summary>
        /// static プロパティであるか
        /// </summary>
        public bool IsStatic 
        {
            get
            {
                if (Getter != null && Getter.IsStatic) return true;
                if (Setter != null && Setter.IsStatic) return true;
                return false;
            }
        }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="method">getter または setter メソッド</param>
        public CLProperty(CLMethod method)
        {
            Attach(method);
            Name = GetPropertyName(method);
        }

        /// <summary>
        /// メソッドを割り当てる (get/set はこの中で判別)
        /// </summary>
        public void Attach(CLMethod method)
        {
            if (string.Compare(method.Name, 0, "Get", 0, 3) == 0)
            {
                if (Getter != null) throw new InvalidOperationException("get プロパティ割り当て済み");
                Getter = method;
                Getter.PropertyNameType = PropertyNameType.Get;
            }
            else if (string.Compare(method.Name, 0, "Is", 0, 2) == 0)
            {
                if (Getter != null) throw new InvalidOperationException("get プロパティ割り当て済み");
                Getter = method;
                Getter.PropertyNameType = PropertyNameType.Is;
            }
            else if (string.Compare(method.Name, 0, "Set", 0, 3) == 0)
            {
                if (Setter != null) throw new InvalidOperationException("set プロパティ割り当て済み");
                Setter = method;
                Setter.PropertyNameType = PropertyNameType.Set;
            }
        }

        /// <summary>
        /// プロパティであるメソッドか
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public static bool CheckProperty(CLMethod method)
        {
            if (method.FuncDecl.Attribute == MethodAttribute.Property)
            {
                if (string.Compare(method.Name, 0, "Get", 0, 3) == 0 ||
                    string.Compare(method.Name, 0, "Set", 0, 3) == 0 ||
                    string.Compare(method.Name, 0, "Is", 0, 2) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// プロパティ名を取得する
        /// </summary>
        /// <param name="method"></param>
        /// <returns></returns>
        public static string GetPropertyName(CLMethod method)
        {
            if (string.Compare(method.Name, 0, "Get", 0, 3) == 0)
            {
                return method.Name.Substring(3);
            }
            else if (string.Compare(method.Name, 0, "Set", 0, 3) == 0)
            {
                return method.Name.Substring(3);
            }
            else if (string.Compare(method.Name, 0, "Is", 0, 2) == 0)
            {
                return method.Name.Substring(2);
            }
            throw new InvalidOperationException();
        }
        #endregion
    }
}
