using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{


    enum ModuleEntityType
    {
        StaticClass,            // static 関数のみで構成される
        HandleWrapper,          // lnHandle 型をラップする、ReferenceObject クラス
        AbstractHandleWrapper,  // コンストラクタを持っていない
        Struct,                 // struct クラス
    }

    class Entity
    {
    }

    /// <summary>
    /// モジュールエンティティ
    /// </summary>
    class ModuleEntity : Entity
    {
        /// <summary>
        /// C++ ソースから取得したオリジナルの名前 (LSprite や LVector)
        /// </summary>
        public string CPPName { get; private set; }

        /// <summary>
        /// プレフィックスを除いた名前 (L を取った)
        /// </summary>
        public string CommonName { get; private set; }

        /// <summary>
        /// ModuleType が Struct に使用する struct 定義情報
        /// </summary>
        public StructDecl StructDecl;

        #region Functions property
        /// <summary>
        /// 関数リスト (プロパティも含む)
        /// </summary>
        public List<FuncEntity> Functions { get; private set; }
        #endregion

        #region FunctionsWithoutProperty property
        /// <summary>
        /// 関数リスト (プロパティを含まない)
        /// </summary>
        public List<FuncEntity> FunctionsWithoutProperty { get; private set; }
        #endregion

        #region Properties property
        /// <summary>
        /// プロパティリスト
        /// </summary>
        public List<PropertyEntity> Properties { get; private set; }
        #endregion

        #region ModuleDoc property
        /// <summary>
        /// ドキュメント
        /// </summary>
        public ModuleDocNode ModuleDoc { get; set; }
        #endregion

        #region ModuleType property
        /// <summary>
        /// モジュールの種類
        /// </summary>
        public ModuleEntityType ModuleType { get; private set; }
        #endregion

        #region IsHandleWrapperModule property
        /// <summary>
        /// モジュールの種類
        /// </summary>
        public bool IsHandleWrapperModule
        {
            get
            {
                if (ModuleType == ModuleEntityType.AbstractHandleWrapper ||
                    ModuleType == ModuleEntityType.HandleWrapper)
                    return true;
                if (ModuleType == ModuleEntityType.StaticClass ||
                    ModuleType == ModuleEntityType.Struct)
                    return false;
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region Inheritance property
        /// <summary>
        /// 継承親モジュール
        /// </summary>
        public ModuleEntity Inheritance { get; set; }
        #endregion
        
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="cppFullName">LSprit 等、プレフィックス付きの名前</param>
        public ModuleEntity(string cppFullName)
        {
            Functions = new List<FuncEntity>();
            Properties = new List<PropertyEntity>();
            FunctionsWithoutProperty = new List<FuncEntity>();
            CPPName = cppFullName;

            if (CPPName.IndexOf(C_APIAnalyzer.FuncPrefix) != 0)
                throw new InvalidOperationException();  // モジュール名の先頭が "LN" ではない
            CommonName = CPPName.Substring(C_APIAnalyzer.FuncPrefix.Length);  // プリフィックス削除
        }

        /// <summary>
        /// 現在の情報からモジュールの種類を決定する
        /// ※このモジュールのすべての関数の FuncType が確定した後に呼ぶこと
        /// </summary>
        public void UpdateModuleType()
        {
            // 必要なものがそろっているか
            if (Functions == null) throw new InvalidOperationException("Functions");

            ModuleType = ModuleEntityType.StaticClass;

            if (StructDecl != null)
            {
                // このモジュールは struct から作成された。必ず ModuleEntityType.Struct である
                ModuleType = ModuleEntityType.Struct;
            }
            else 
            {
                bool hasInstanceMethod = false;
                bool hasInstanceConstructor = false;
                foreach (var func in Functions)
                {
                    if (func.FuncType == FuncType.Instance)
                        hasInstanceMethod = true;
                    else if (func.FuncType == FuncType.Constructor)
                        hasInstanceConstructor = true;
                }

                if (hasInstanceMethod || hasInstanceConstructor)
                {
                    // インスタンスメソッド + コンストラクタ → lnHandle のラッパークラス
                    if (hasInstanceConstructor)
                        ModuleType = ModuleEntityType.HandleWrapper;
                    // インスタンスメソッドがあるが、コンストラクタが無い → LVisualNode のような abstruct クラス
                    else
                        ModuleType = ModuleEntityType.AbstractHandleWrapper;
                }
            }
        }
    }

    /// <summary>
    /// 関数エンティティ
    /// </summary>
    class FuncEntity : Entity
    {
        #region FuncDocNode property
        public FuncDocNode FuncDocNode { get; private set; }
        #endregion

        #region FuncDeclNode property
        public FuncDeclNode FuncDeclNode { get; private set; }
        #endregion

        #region ReturnType property
        /// <summary>
        /// 戻り値情報
        /// </summary>
        public ReturnTypeEntity ReturnType { get; private set; }
        #endregion

        #region CPPName property
        /// <summary>
        /// C++ソースから取得したオリジナルの名前 (LSceneNode_SetMatrix 等)
        /// </summary>
        public string CPPName { get { return FuncDeclNode.Name; } }
        #endregion

        #region CommonName property
        /// <summary>
        /// プレフィックスを除いた名前。SetMatrix 等
        /// </summary>
        public string CommonName { get { return _CommonName; } }
        private string _CommonName;
        #endregion

        #region Option property
        /// <summary>
        /// オプション
        /// </summary>
        public FuncOptionNode Option { get; private set; }
        #endregion

        #region Summary property
        /// <summary>
        /// 概要説明文
        /// </summary>
        public string Summary { get { return FuncDocNode.Summary; } }
        #endregion

        #region Detail property
        /// <summary>
        /// 詳細説明文
        /// </summary>
        public string Detail { get { return FuncDocNode.Detail; } }
        #endregion

        #region AllParams property
        /// <summary>
        /// 全仮引数
        /// </summary>
        public List<FuncParamEntity> AllParams { get; private set; }
        #endregion

        #region ActualParams property
        /// <summary>
        /// 関数の種類を考慮した、実際の仮引数
        /// </summary>
        public List<FuncParamEntity> ActualParams { get; private set; }
        #endregion

        #region ParentModule property
        /// <summary>
        /// 親モジュール
        /// </summary>
        public ModuleEntity ParentModule { get; set; }
        #endregion

        #region PropertyType property
        /// <summary>
        /// プロパティとしての種類
        /// </summary>
        public PropertyNameType PropertyType { get; private set; }// = PropertyNameType.NotProperty;
        #endregion

        #region PropertyName property
        /// <summary>
        /// この関数がプロパティとしてマークされているとき、Get/Set/Is を含まない名前
        /// </summary>
        public string PropertyName { get; private set; }
        #endregion

        #region FuncType property
        /// <summary>
        /// 関数の種類
        /// </summary>
        public FuncType FuncType { get; private set; }
        #endregion

        #region IsInstanceMethod property
        /// <summary>
        /// FuncType がインスタンスメソッドであるか
        /// </summary>
        public bool IsInstanceMethod
        {
            get
            {
                return (FuncType == BinderMaker.FuncType.Instance ||
                        FuncType == BinderMaker.FuncType.Constructor ||
                        FuncType == BinderMaker.FuncType.StructInstance);
            }
        }
        #endregion

        #region CppModuleName Property
        /// <summary>
        /// モジュールに関数を登録する際に使用する検索キー (プレフィックス付き)
        /// </summary>
        public string CppModuleName { get { return FuncDeclNode.ModuleName; } }
        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public FuncEntity()
        {
            PropertyType = PropertyNameType.NotProperty;
            FuncType = BinderMaker.FuncType.Static;
            Option = new FuncOptionNode(); // デフォルトで入れておく
        }

        /// <summary>
        /// FuncDocNode が持つ情報の展開
        /// </summary>
        /// <param name="node"></param>
        public void SetDocAndDeclNode(C_APIAnalyzer analyzer, FuncDocNode doc, FuncDeclNode decl)
        {
            FuncDocNode = doc;
            FuncDeclNode = decl;

            ReturnType = new ReturnTypeEntity();
            ReturnType.Detail = doc.Return;
            ReturnType.CPPFullTypeName = decl.ReturnType;

            _CommonName = analyzer.RemoveOverloadSuffix(decl.CommonName);
            

            AllParams = new List<FuncParamEntity>();
            foreach (var param in decl.Params)
            {
                // 同名のParamDoc を検索
                FuncParamDocNode paramDoc = null;
                foreach (var p in doc.Params)
                {
                    if (p.Name == param.Name)
                    {
                        paramDoc = p;
                        break;
                    }
                }
                if (paramDoc == null)   // 見つからなかった
                    throw new InvalidOperationException();

                var fp = new FuncParamEntity();
                fp.SetDocAndDeclNode(paramDoc, param);
                AllParams.Add(fp);
            }
        }

        /// <summary>
        /// FuncOptionNode の情報の設定
        /// </summary>
        /// <param name="node"></param>
        public void SetFuncOptionNode(FuncOptionNode node)
        {
            Option = node;
        }

        /// <summary>
        /// 現在の情報を元に、この関数の種類を決定する
        /// </summary>
        public void UpdateFuncType(C_APIAnalyzer analyzer)
        {
            // 必要なものがそろっているか
            if (string.IsNullOrEmpty(CommonName)) throw new InvalidOperationException("CommonName");
            if (AllParams == null) throw new InvalidOperationException("AllParams");
            if (ReturnType == null) throw new InvalidOperationException("ReturnType");
                

            // 以下のいずれかの条件にも一致しなければ static である
            FuncType = BinderMaker.FuncType.Static;

            if (Option.ForceStaticMethod)
            {
                // static 強制
            }
            // 先頭が Create ならコンストラクタ関数
            else if (string.Compare(CommonName, 0, "Create", 0, 6) == 0)
            {
                FuncType = BinderMaker.FuncType.Constructor;
            }
            // 先頭が GetDefault ならデフォルトで作成される lnHandle の getter
            else if (string.Compare(CommonName, 0, "GetDefault", 0, 10) == 0)
            {
                // 戻り値が lnHandle じゃなかったり、引数を持っている場合は想定していない
                //if (!ReturnType.IsHandleType) throw new InvalidOperationException();
                //if (AllParams.Count != 0) throw new InvalidOperationException();

                // 引数は1つで、第1引数が必ず out lnHandle
                if (AllParams.Count != 1) throw new InvalidOperationException();
                if (!AllParams[0].IsOutHandleType) throw new InvalidOperationException();

                FuncType = BinderMaker.FuncType.DefaultInstanceGetter;
                PropertyType = PropertyNameType.Get;    // get プロパティ扱い
                PropertyName = CommonName.Substring(3); // Get 以降をプロパティ名に

            }
            else if (AllParams.Count() > 0)
            {
                var firstParam = AllParams.First();
                StructDecl structDecl;

                // 第1引数がモジュールと一致する構造体型で、かつ out タイプの場合は static な struct 関数
                if (analyzer.TryGetStructDeclByName(firstParam.CPPFullTypeName, out structDecl) &&
                    FuncDeclNode.CommonModuleName == structDecl.CommonName &&
                    firstParam.IOType == ParamIO.Out)
                {
                    FuncType = BinderMaker.FuncType.StructOutputStatic;
                }
                // 第1引数が Handle 型、または構造体型であればインスタンスメソッドかもしれない
                else if (firstParam.CPPFullTypeName == "lnHandle" ||
                    (firstParam.CommonTypeName == FuncDeclNode.CommonModuleName))
                {
                    // 第1引数が lnHandle ならインスタンスメソッド。それ以外の型なら StructInstance
                    if (firstParam.CPPFullTypeName == "lnHandle")
                        FuncType = BinderMaker.FuncType.Instance;
                    else
                        FuncType = BinderMaker.FuncType.StructInstance;


                    // 引数がインスタンス系のみで、戻り値がある場合は getter かもしれない
                    // 引数がインスタンス系ともうひとつ、これが out 型の場合は getter かもしれない
                    // 引数がインスタンス系ともうひとつ、これが out 以外の場合は setter かもしれない
                    //if ((AllParams.Count() == 1 && ReturnType.CPPFullTypeName != "void") ||
                    //    (AllParams.Count() == 2 && ReturnType.CPPFullTypeName == "void"))7
                    if (AllParams.Count() == 1 || AllParams.Count() == 2)
                    {
                        if (string.Compare(CommonName, 0, "Get", 0, 3) == 0)
                        {
                            PropertyType = PropertyNameType.Get;
                            PropertyName = CommonName.Substring(3);
                        }
                        else if (string.Compare(CommonName, 0, "Set", 0, 3) == 0)
                        {
                            PropertyType = PropertyNameType.Set;
                            PropertyName = CommonName.Substring(3);
                        }
                        else if (string.Compare(CommonName, 0, "Is", 0, 2) == 0)
                        {
                            PropertyType = PropertyNameType.Is;
                            PropertyName = CommonName.Substring(2);
                        }
                    }
                }
            }

            // プロパティ化しないオプションがあれば強制的に戻す
            if (!Option.AutoPropertyGenerate)
                PropertyType = PropertyNameType.NotProperty;
        }

        /// <summary>
        /// 後解析
        /// ModuleEntity.UpdateModuleType() がすべて完了した後に呼ばれる
        /// </summary>
        /// <param name="parentModule"></param>
        public void PostAnalyze(C_APIAnalyzer analyzer)
        {
            if (ParentModule == null) throw new InvalidOperationException();

            // 引数リスト
            foreach (var param in AllParams)
                param.PostAnalyze(analyzer, this, ParentModule);

            // 戻り値
            ReturnType.PostAnalyze(analyzer, ParentModule, this);

            // struct 型で Set は特殊扱い
            if (ParentModule.ModuleType == ModuleEntityType.Struct &&
                CommonName == "Set")
               // string.Compare(CommonName, 0, "Set", 0, 3) == 0)
            {
                FuncType = BinderMaker.FuncType.StructMemberSetter;
            }
            // static 関数またはコンストラクタの場合、すべての引数を使う
            else if (FuncType == BinderMaker.FuncType.Static ||
#if true   // コンストラクタの生成方法変更
#else
                FuncType == BinderMaker.FuncType.Constructor ||
#endif
                FuncType == BinderMaker.FuncType.DefaultInstanceGetter)
            {
                ActualParams = new List<FuncParamEntity>(AllParams);
            }
            // インスタンスメソッドなどは先頭を this とみなし、第2引数以降を使用する
            else if (FuncType == BinderMaker.FuncType.Instance ||
                     FuncType == BinderMaker.FuncType.StructInstance ||
                     FuncType == BinderMaker.FuncType.StructOutputStatic ||
#if true   // コンストラクタの生成方法変更
                    FuncType == BinderMaker.FuncType.Constructor
#endif
            )
            {
                ActualParams = new List<FuncParamEntity>();
                foreach (var param in AllParams)
                {
                    if (param != AllParams.First())
                        ActualParams.Add(param);
                }
            }
            else
                throw new InvalidOperationException();

        }

        #endregion
    }

    /// <summary>
    /// プロパティ (getter と setter のセット)
    /// </summary>
    class PropertyEntity : Entity
    {
        #region Getter property
        /// <summary>
        /// getter
        /// </summary>
        public FuncEntity Getter { get; set; }
        #endregion

        #region Setter property
        /// <summary>
        /// setter
        /// </summary>
        public FuncEntity Setter { get; set; }
        #endregion

        #region Name property
        /// <summary>
        /// プロパティ名 (Get や Set は除き、Isが必要であれば含む)
        /// </summary>
        public string Name
        {
            get
            {
                if (Getter != null && Getter.PropertyType == PropertyNameType.Is)
                    return "Is" + Getter.PropertyName;
                else if (Getter != null)
                    return Getter.PropertyName;
                else if (Setter != null)
                    return Setter.PropertyName;
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region CppModuleName property
        /// <summary>
        /// モジュールに関数を登録する際に使用する検索キー (プレフィックス付き)
        /// </summary>
        public string CppModuleName
        {
            get
            {
                // getter と setter は同じモジュールに属するのでどちらかので良い
                if (Getter != null)
                    return Getter.CppModuleName;
                if (Setter != null)
                    return Setter.CppModuleName;
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region KeyName property
        /// <summary>
        /// 関数の PropertyName を使ってプロパティを検索する際に使用するキー文字列
        /// </summary>
        public string KeyName
        {
            get
            {
                if (Getter != null)
                    return Getter.PropertyName;
                else if (Setter != null)
                    return Setter.PropertyName;
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region Type property
        public FuncParamEntity Type
        {
            get
            {
                // setter の第1引数
                if (Setter != null)
                    return Setter.ActualParams.First();

                // getter の戻り値型
                if (Getter != null)
                {
                    //if (Getter.ReturnType.CPPFullTypeName != "void")
                    //    return Getter.ReturnType;
                    //else
                    return Getter.ActualParams.First();    // 第2引数が out なもの
                }

                throw new InvalidOperationException();
            }
        }
        #endregion

        #region IsGetterReturn property
        /// <summary>
        /// getter (API) が return で値を戻しているか (false の場合は第2引数が out である)
        /// </summary>
        public bool IsGetterReturn
        {
            get
            {
                if (Getter == null)
                    throw new InvalidOperationException();
                return (Getter.ReturnType.CPPFullTypeName != "void");
            }
        }
        #endregion

        #region IsStatic property
        /// <summary>
        /// static プロパティであるか
        /// </summary>
        public bool IsStatic
        {
            get
            {
                // get と set の配置が違う。あり得ないはずだが…
                if (Getter != null && Setter != null &&
                    Getter.FuncType != Setter.FuncType)
                    throw new InvalidOperationException();

                FuncType type;
                if (Getter != null)
                    type = Getter.FuncType;
                else //if (Setter != null)
                    type = Setter.FuncType;

                if (type == FuncType.Static ||
                    type == FuncType.DefaultInstanceGetter ||
                    type == FuncType.StructOutputStatic)
                    return true;
                if (type == FuncType.Constructor ||     // enum 増やしたときの漏れ防止のため、単なるelse にはしない
                    type == FuncType.Instance ||
                    type == FuncType.StructInstance)
                    return false;

                throw new InvalidOperationException();
            }
        }
        #endregion

        #region SummaryDoc property
        /// <summary>
        /// 概要説明文
        /// </summary>
        public string SummaryDoc
        {
            get
            {
                string getter = (Getter != null) ? Getter.Summary : "";
                string setter = (Setter != null) ? Setter.Summary : "";

                getter = getter.Replace("の取得", "");
                setter = setter.Replace("の設定", "");

                // もし両方ある場合はコメント内容が一致しているはず
                if (!string.IsNullOrEmpty(getter) && !string.IsNullOrEmpty(setter) && getter != setter)
                    throw new InvalidOperationException();
                // ここで止まる場合、「を設定する」とかになってないかチェック

                if (!string.IsNullOrEmpty(getter))
                    return getter;
                if (!string.IsNullOrEmpty(setter))
                    return setter;

                throw new InvalidOperationException();
            }
        }
        #endregion

        #region DetailDoc property
        /// <summary>
        /// 詳細説明文
        /// </summary>
        public string DetailDoc
        {
            get
            {
                // 両方あれば間を改行
                if (Getter != null && !string.IsNullOrEmpty(Getter.Detail) &&
                    Setter != null && !string.IsNullOrEmpty(Setter.Detail))
                    return Getter.Detail + "\n\n" + Setter.Detail;

                if (Getter != null)
                    return Getter.Detail;
                if (Setter != null)
                    return Setter.Detail;

                return null;
            }
        }
        #endregion
    }

    /// <summary>
    /// 関数仮引数エンティティ
    /// </summary>
    class FuncParamEntity : TypedEntity
    {
        #region Fields

        private FuncParamDocNode _FuncParamDocNode;
        private FuncParamDeclNode _FuncParamDeclNode;

        #endregion

        #region ParentFuncEntity property
        /// <summary>
        /// 親関数
        /// </summary>
        public FuncEntity ParentFuncEntity { get; private set; }
        #endregion

        #region ParamName property
        /// <summary>
        /// 仮引数名
        /// </summary>
        public string ParamName { get; private set; }
        #endregion

        #region IOType property
        /// <summary>
        /// 入出力の種類
        /// </summary>
        public ParamIO IOType { get; private set; }
        #endregion

        #region IOString property
        /// <summary>
        /// 入出力の種類 を文字列化したもの ("in,out" 等。"[]" は含まない)
        /// </summary>
        public string IOString
        {
            get
            {
                if (IOType == (ParamIO.In | ParamIO.Out))
                    return "in,out";
                else if ((IOType & ParamIO.In) != 0)
                    return "in";
                else if ((IOType & ParamIO.Out) != 0)
                    return "out";
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region Detail property
        /// <summary>
        /// 説明文
        /// </summary>
        public string Detail { get; private set; }
        #endregion

        #region DefaultValue property
        /// <summary>
        /// デフォルト引数の値 (enum ならその enum メンバ名)
        /// </summary>
        public string DefaultValue { get; private set; }
        #endregion

        #region HandleWrapperModule property
        /// <summary>
        /// lnHandle の時の対応するモジュール
        /// </summary>
        public override ModuleEntity HandleWrapperModule { get; protected set; }
        #endregion

        #region IsStringOutput property
        /// <summary>
        /// 文字列の out 型であるか (const lnChar**)
        /// </summary>
        public bool IsStringOutput
        {
            get
            {
                return CPPFullTypeName == "const lnChar**";
            }
        }
        #endregion

        #region Methods

        /// <summary>
        /// 各構造ノードの設定
        /// </summary>
        /// <param name="doc"></param>
        /// <param name="decl"></param>
        public void SetDocAndDeclNode(FuncParamDocNode doc, FuncParamDeclNode decl)
        {
            _FuncParamDocNode = doc;
            _FuncParamDeclNode = decl;

            DefaultValue = decl.DefaultValue;
            ParamName = decl.Name;
            Detail = doc.Summary;

            IOType = 0;
            IOType |= (doc.IO.IndexOf("in") >= 0) ? ParamIO.In : 0;
            IOType |= (doc.IO.IndexOf("out") >= 0) ? ParamIO.Out : 0;
            if (IOType == 0)
                throw new InvalidOperationException();  // IO が空等

            base.SetNodeInfo(decl);
        }

        /// <summary>
        /// 後解析
        /// </summary>
        /// <param name="parentModule"></param>
        public void PostAnalyze(C_APIAnalyzer analyzer, FuncEntity parentFunc, ModuleEntity parentModule)
        {
            ParentFuncEntity = parentFunc;
            if (ParamName == null) throw new InvalidOperationException();

            // lnHandle 型なら引数名からモジュール名を予測する。
            // srcTexture 等は LTexture。
            if (IsInOrOutHandleType)
            {
                HandleWrapperModule = analyzer.AllModuleEntites.Find((item) => ParamName.EndsWith(item.CommonName, true, System.Globalization.CultureInfo.DefaultThreadCurrentCulture));

                if (parentModule.CPPName == "LObjectList" &&
                    (ParamName == "item" || ParamName == "outItem"))
                {
                    Console.WriteLine("引数名からクラス名の特定の特殊化 {0} - {1}", ParentFuncEntity.CPPName, ParamName);
                    HandleWrapperModule = analyzer.AllModuleEntites.Find((item) => item.CPPName == "LObject");
                }
                if (HandleWrapperModule == null) throw new InvalidOperationException();
            }

        }

        #endregion
    }

    /// <summary>
    /// 戻り値型エンティティ
    /// </summary>
    class ReturnTypeEntity : TypedEntity
    {
        /// <summary>
        /// 説明文
        /// </summary>
        public string Detail;

        #region HandleWrapperModule property
        /// <summary>
        /// lnHandle の時の対応するモジュール
        /// </summary>
        public override ModuleEntity HandleWrapperModule { get; protected set; }
        #endregion

        #region Methods

        /// <summary>
        /// 後解析
        /// </summary>
        /// <param name="parentModule"></param>
        public void PostAnalyze(C_APIAnalyzer analyzer, ModuleEntity parentModule, FuncEntity parentFunc)
        {
            // ハンドル型である場合は対応するモジュールを探す
            if (IsInOrOutHandleType)
            {
                if (parentFunc.FuncType == FuncType.Constructor ||
                    parentFunc.FuncType == FuncType.DefaultInstanceGetter)
                {
                    // コンストラクタかデフォルトlnHandle geterの場合は関数が属しているモジュールが使える
                    HandleWrapperModule = parentFunc.ParentModule;
                }
                else
                {
                    // モジュール名を自動特定する材料が無い。オプションを使う。
                    if (string.IsNullOrEmpty(parentFunc.Option.ReturnHandleType))
                        throw new InvalidOperationException();

                    HandleWrapperModule = analyzer.AllModuleEntites.Find((item) => item.CPPName == parentFunc.Option.ReturnHandleType);
                }

                // 見つからなかった。オプションが間違ってるかもしれない
                if (HandleWrapperModule == null)
                    throw new InvalidOperationException();
            }
        }

        #endregion
    }

    /// <summary>
    /// 型情報を持つエンティティ
    /// </summary>
    abstract class TypedEntity : Entity
    {
        #region Fields
        //protected class TypeInfo
        //{
        //    public string CommonTypeName;
        //    public ParamIO IOType;

        //    public TypeInfo(string commonTypeName, ParamIO io)
        //    {
        //        CommonTypeName = commonTypeName;
        //        IOType = io;
        //    }
        //}
        //protected Dictionary<string, TypeInfo> TypeInfoTable = new Dictionary<string, TypeInfo>()
        //{
        //    // C++ Type             Common Type                     API仮引数型修飾子
        //    { "lnHandle",           new TypeInfo("Handle",      ParamIO.In) },
        //    { "lnHandle*",          new TypeInfo("Handle",      ParamIO.Out) },
        //    { "void",               new TypeInfo("Void",        ParamIO.In) },
        //    { "void*",              new TypeInfo("ByteArray",   ParamIO.Out) },
        //    { "const char*",        new TypeInfo("String",      ParamIO.In) },
        //    { "const TCHAR*",       new TypeInfo("String",      ParamIO.In) },
        //    { "const lnChar*",      new TypeInfo("String",      ParamIO.In) },
        //    { "int",                new TypeInfo("Int",         ParamIO.In) },
        //    { "float",              new TypeInfo("Float",       ParamIO.In) },
        //    { "double",             new TypeInfo("Double",      ParamIO.In) },
        //    { "int*",               new TypeInfo("Int",         ParamIO.Out) },
        //    { "float*",             new TypeInfo("Float",       ParamIO.Out) },
        //    { "lnBool",             new TypeInfo("Bool",        ParamIO.In) },
        //    { "lnByte*",            new TypeInfo("ByteArray",   ParamIO.Out) },
        //    { "const lnByte*",      new TypeInfo("ByteArray",   ParamIO.In) },
        //    { "const void*",        new TypeInfo("ByteArray",   ParamIO.In) },
        //    { "lnU8",               new TypeInfo("Byte",        ParamIO.In) },
        //    { "lnU32",              new TypeInfo("UInt",        ParamIO.In) },
        //    { "lnResult",           new TypeInfo("Result",      ParamIO.In) },
        //    { "lnExceptionCallback",new TypeInfo("ExceptionCallback", ParamIO.In) },
        //};
        #endregion

        #region CPPFullTypeName property
        /// <summary>
        /// C++ ソースから取ったそのままの型名 (int や lnHandle、const lnRect* 等)
        /// </summary>
        public string CPPFullTypeName
        {
            get { return _CPPFullTypeName; }
            set
            {
                _CPPFullTypeName = value;
                // const や *、lnプリフィックスを除いた型名
                CommonTypeName = _CPPFullTypeName.Replace("const", "").Replace("*", "").Trim();
                if (CommonTypeName.IndexOf(C_APIAnalyzer.StructOrEnumPrefix) == 0)
                    CommonTypeName = CommonTypeName.Substring(C_APIAnalyzer.StructOrEnumPrefix.Length);
            }
        }
        private string _CPPFullTypeName;
        #endregion

        #region CommonTypeName property
        /// <summary>
        /// const や *、lnプリフィックスを除いた型名
        /// </summary>
        public string CommonTypeName { get; private set; }
        #endregion

        #region IsInHandleType property
        /// <summary>
        /// lnHandle In 型であるか
        /// </summary>
        public bool IsInHandleType 
        { 
            get 
            {
                if (CPPFullTypeName.IndexOf("*") != -1) // * が含まれている場合は out なので false を返す
                    return false;
                return (CPPFullTypeName.IndexOf("lnHandle") != -1); 
            } 
        }
        #endregion

        #region IsOutHandleType property
        /// <summary>
        /// lnHandle Out 型であるか
        /// </summary>
        public bool IsOutHandleType 
        { 
            get
            {
                if (CPPFullTypeName.IndexOf("*") == -1) // * が含まれていない場合は in なので false を返す
                    return false;
                return (CPPFullTypeName.IndexOf("lnHandle") != -1); 
            }
        }
        #endregion

        #region IsInOrOutHandleType property
        /// <summary>
        /// lnHandle 型であるか
        /// </summary>
        public bool IsInOrOutHandleType
        {
            get
            {
                return (CPPFullTypeName.IndexOf("lnHandle") != -1);
            }
        }
        #endregion

        #region HandleWrapperModule property
        /// <summary>
        /// lnHandle の時の対応するモジュール
        /// </summary>
        public abstract ModuleEntity HandleWrapperModule { get; protected set; }
        #endregion

        #region Methods

        /// <summary>
        /// 各構造ノードの設定
        /// </summary>
        /// <param name="decl"></param>
        public void SetNodeInfo(FuncParamDeclNode decl)
        {
            CPPFullTypeName = decl.Type;
        }

        /// <summary>
        /// 後解析
        /// </summary>
        /// <param name="parentModule"></param>
        //public virtual void PostAnalyze(C_APIAnalyzer analyzer, ModuleEntity parentModule)
        //{
        //}



        /// <summary>
        /// struct 型であるか
        /// </summary>
        public bool IsStructType(C_APIAnalyzer analyzer)
        {
            foreach (var node in analyzer.AllStructNodes)
            {
                if (CPPFullTypeName.IndexOf(node.Name) >= 0)
                {
                    return true;
                }
            }
            return false;
        }

        #endregion
    }
}
