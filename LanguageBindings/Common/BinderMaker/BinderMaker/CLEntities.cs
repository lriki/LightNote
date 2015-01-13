using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace BinderMaker
{
    enum CLModuleAttr
    {
        StaticClass,            // static 関数のみで構成される
        HandleWrapper,          // lnHandle 型をラップする、ReferenceObject クラス
        AbstractHandleWrapper,  // コンストラクタを持っていない
        Struct,                 // struct クラス
    }

    enum MethodAttribute
    {
        Instance,               // インスタンスメソッド (第1引数が lnHandle)
        Static,                 // static メソッド (デフォルト)
        Constructor,            // コンストラクタ
        DefaultInstanceGetter,  // デフォルトで作成される lnHandle の getter (LCamera_GetDefault3DCamera 等)
        StructOutputStatic,     // 構造体演算用 static 関数
        StructInstance,         // 第1引数が in,out かつ モジュール名と一致する型である関数
        CoreInitializer,        // ライブラリの初期化メソッド
    }

    enum PrimitiveType
    {
        Void,
        Bool,
        Int,
        UInt8,
        UInt32,
        Float,
        Double,
        String,
    
        ByteBuffer,     // void*(in), void**(out)
        Object,

        ResultCode,
        ExceptionCallback,
        HWND,
    }

    [Flags]
    enum ParamAttribute
    {
        None = 0x00,
        In = 0x01,
        Out = 0x02,
    }

    /// <summary>
    /// 型情報
    /// </summary>
    class CLType : CLEntity
    {

        #region Name property
        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; protected set; }
        #endregion

        #region CName property
        /// <summary>
        /// C 言語定義での名前
        /// </summary>
        public string CName { get; protected set; }
        #endregion

        #region BaseType property
        /// <summary>
        /// 基底の型
        /// </summary>
        public CLType BaseType { get; protected set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="analyzer"></param>
        //protected CLType(CLAnalyzer analyzer)
        //{
        //    _analyzer = analyzer;
        //}
        #endregion
    }

    /// <summary>
    /// プリミティブ型
    /// </summary>
    class CLPrimitiveType : CLType
    {
        public static CLPrimitiveType Void = new CLPrimitiveType("Void");
        public static CLPrimitiveType ByteArray = new CLPrimitiveType("ByteArray");
        public static CLPrimitiveType String = new CLPrimitiveType("String");

        public static CLPrimitiveType Bool = new CLPrimitiveType("Bool");
        public static CLPrimitiveType Byte = new CLPrimitiveType("Byte");
        public static CLPrimitiveType Int = new CLPrimitiveType("Int");
        public static CLPrimitiveType UInt32 = new CLPrimitiveType("UInt32");
        public static CLPrimitiveType Float = new CLPrimitiveType("Float");
        public static CLPrimitiveType Double = new CLPrimitiveType("Double");

        public static CLPrimitiveType ResultCode = new CLPrimitiveType("ResultCode");
        public static CLPrimitiveType ExceptionCallback = new CLPrimitiveType("ExceptionCallback");
        public static CLPrimitiveType IntPtr = new CLPrimitiveType("IntPtr");
        public static CLPrimitiveType HWND = new CLPrimitiveType("HWND");

        public static CLPrimitiveType Handle = new CLPrimitiveType("Handle");
        public static CLPrimitiveType Generic = new CLPrimitiveType("Generic");

        public static CLPrimitiveType IntNativeArray = new CLPrimitiveType("IntNativeArray");

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLPrimitiveType(string name)
        {
            Name = name;
        }

        #endregion
    }

    /// <summary>
    /// 参照オブジェクトクラス
    /// </summary>
    class CLClassType : CLType
    {
        #region Fields
        protected CLAnalyzer _analyzer;
        private ModuleDocNode _moduleDocNode;
        private StructDecl _structDecl;
        #endregion

        #region Attribute property
        /// <summary>
        /// 属性
        /// </summary>
        public CLModuleAttr Attribute { get; private set; }
        #endregion

        #region Documment property
        /// <summary>
        /// ドキュメントコメント
        /// </summary>
        public ModuleDocNode Documment { get { return _moduleDocNode; } }
        #endregion

        #region IsReferenceObject property
        /// <summary>
        /// ReferenceObject を継承するか
        /// </summary>
        public bool IsReferenceObject
        {
            get
            {
                if (Attribute == CLModuleAttr.AbstractHandleWrapper ||
                    Attribute == CLModuleAttr.HandleWrapper)
                    return true;
                if (Attribute == CLModuleAttr.StaticClass ||
                    Attribute == CLModuleAttr.Struct)
                    return false;
                throw new InvalidOperationException();
            }
        }
        #endregion

        #region IsStruct property
        /// <summary>
        /// struct であるか
        /// </summary>
        public bool IsStruct
        {
            get
            {
                return _structDecl != null;
                //if (Attribute == CLModuleAttr.AbstractHandleWrapper ||
                //    Attribute == CLModuleAttr.HandleWrapper ||
                //    Attribute == CLModuleAttr.StaticClass)
                //    return false;
                //if (Attribute == CLModuleAttr.Struct)
                //    return true;
                //throw new InvalidOperationException();
            }
        }
        #endregion

        #region IsGeneric property
        /// <summary>
        /// ジェネリックであるか
        /// </summary>
        public bool IsGeneric { get; private set; }
        #endregion

        #region Properties property
        /// <summary>
        /// プロパティリスト
        /// </summary>
        public List<CLProperty> Properties { get; private set; }
        #endregion

        #region Methods property
        /// <summary>
        /// 関数リスト (プロパティも含む)
        /// </summary>
        public List<CLMethod> Methods { get; private set; }
        #endregion

        #region StructMembers property
        /// <summary>
        /// 構造体の場合のメンバ
        /// </summary>
        public IEnumerable<StructMemberDecl> StructMembers { get { return _structDecl.Members; } }
        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ (class 用)
        /// </summary>
        public CLClassType(CLAnalyzer analyzer, ModuleDocNode node)
        {
            _analyzer = analyzer;
            _moduleDocNode = node;
            Properties = new List<CLProperty>();
            Methods = new List<CLMethod>();
            CName = _moduleDocNode.GroupInfo.Title;

            if (CName.IndexOf(C_APIAnalyzer.FuncPrefix) != 0)
                throw new InvalidOperationException();  // モジュール名の先頭が "LN" ではない
            Name = CName.Substring(C_APIAnalyzer.FuncPrefix.Length);  // プリフィックス削除
        }

        /// <summary>
        /// コンストラクタ (struct 用)
        /// </summary>
        public CLClassType(CLAnalyzer analyzer, ModuleDocNode node, StructDecl decl)
        {
            _analyzer = analyzer;
            _moduleDocNode = node;
            _structDecl = decl;
            Properties = new List<CLProperty>();
            Methods = new List<CLMethod>();
            CName = _moduleDocNode.GroupInfo.Title;

            if (CName.IndexOf(C_APIAnalyzer.FuncPrefix) != 0)
                throw new InvalidOperationException();  // モジュール名の先頭が "LN" ではない
            Name = CName.Substring(C_APIAnalyzer.FuncPrefix.Length);  // プリフィックス削除
        }

        /// <summary>
        /// 現在の情報から型の属性を決定する (全メソッドの属性が確定した後で呼ぶ)
        /// </summary>
        public void UpdateAttribute()
        {
            Attribute = CLModuleAttr.StaticClass;

            if (_structDecl != null)
            {
                // このモジュールは struct から作成された。必ず ModuleEntityType.Struct である
                Attribute = CLModuleAttr.Struct;
            }
            else
            {
                bool hasInstanceMethod = false;
                bool hasInstanceConstructor = false;
                foreach (var method in Methods)
                {
                    if (method.Attribute == MethodAttribute.Instance)
                        hasInstanceMethod = true;
                    else if (method.Attribute == MethodAttribute.Constructor)
                        hasInstanceConstructor = true;
                }

                if (hasInstanceMethod || hasInstanceConstructor)
                {
                    // インスタンスメソッド + コンストラクタ → lnHandle のラッパークラス
                    if (hasInstanceConstructor)
                        Attribute = CLModuleAttr.HandleWrapper;
                    // インスタンスメソッドがあるが、コンストラクタが無い → LVisualNode のような abstruct クラス
                    else
                        Attribute = CLModuleAttr.AbstractHandleWrapper;
                }
            }

            if (Attribute == CLModuleAttr.AbstractHandleWrapper ||
                Attribute == CLModuleAttr.HandleWrapper)
            {
                // ベースクラスモジュールを探す
                if (string.IsNullOrEmpty(_moduleDocNode.Extends))
                {
                    if (this != _analyzer.ReferenceObjectClass)
                        BaseType = _analyzer.ReferenceObjectClass;
                }
                else
                    BaseType = _analyzer.GetClassTypeByCName(_moduleDocNode.Extends);
            }

            // 特殊処理
            if (Name == "Object")
                Name = "ReferenceObject";
            if (Name == "ObjectList")
                IsGeneric = true;
        }

        /// <summary>
        /// 仮想関数フラグを再帰的に更新する
        /// </summary>
        public void UpdateVirtualMark(string methodCName)
        {
            // LCamera_SetPostiion と LCamera_SetPostiionXYZ 等を区別して検索する
            var pair = methodCName.Split('_');

            var baseMethod = Methods.Find((item) => item.CName.EndsWith(pair[1]));
            if (baseMethod != null)
            {
                baseMethod.IsVirtual = true;
            }
            if (BaseType != null && BaseType is CLClassType)
            {
                ((CLClassType)BaseType).UpdateVirtualMark(methodCName);
            }
        }

        /// <summary>
        /// このクラスを含め、仮想関数を再帰的に探す
        /// </summary>
        public CLMethod TryGetBaseVirtualMethod(string virtualMethodName)
        {

            // 基底を探す
            if (BaseType != null && BaseType is CLClassType)
            {
                var method = ((CLClassType)BaseType).TryGetBaseVirtualMethod(virtualMethodName);
                if (method != null)
                    return method;
            }

            // このクラスを探す

            // LCamera_SetPostiion と LCamera_SetPostiionXYZ 等を区別して検索する
            var pair = virtualMethodName.Split('_');

            var baseMethod = Methods.Find((item) => item.IsVirtual && item.CName.EndsWith(pair[1]));
            return baseMethod;
        }


        #endregion
    }

    /// <summary>
    /// 列挙型
    /// </summary>
    class CLEnumType : CLType
    {
        #region Fields
        private EnumDecl _enumDecl;
        #endregion

        #region IsFlags property
        /// <summary>
        /// [Flags] 属性であるか
        /// </summary>
        public bool IsFlags { get { return _enumDecl.IsFlags; } }
        #endregion

        #region Members property
        /// <summary>
        /// enum メンバ
        /// </summary>
        public List<CLEnumMember> Members { get; private set; }
        #endregion

        #region Methods

        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLEnumType(CLAnalyzer analyzer, EnumDecl decl)
        {
            _enumDecl = decl;
            Name = _enumDecl.CommonName;
            CName = _enumDecl.Name;
            Members = new List<CLEnumMember>();
            foreach (var member in decl.Members)
            {
                Members.Add(new CLEnumMember(this, member));
            }
        }

        #endregion
    }

    /// <summary>
    /// enum メンバ定義
    /// </summary>
    class CLEnumMember : CLEntity
    {
        #region Parent property
        public CLEnumType Parent { get; private set; }
        #endregion

        #region Name property
        public string Name { get; private set; }
        #endregion

        #region Value property
        public string Value { get; private set; }
        #endregion

        #region Comment property
        public string Comment { get; private set; }
        #endregion

        #region IsTerminator property
        /// <summary>
        /// enum ターミネータであるか
        /// </summary>
        public bool IsTerminator { get; private set; }
        #endregion

        #region CapitalizedName property
        /// <summary>
        /// プレフィックスを除き、各単語の先頭を大文字にして _ を結合した名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → ScalingWithLetterBox
        /// </summary>
        public string CapitalizedName { get; private set; }
        #endregion

        #region CommonSnakeName property
        /// <summary>
        /// プレフィックスを除いた名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → SCALING_WITH_LETTER_BOX
        /// </summary>
        public string CommonSnakeName { get; private set; }
        #endregion

        #region Methods
        public CLEnumMember(CLEnumType parent, EnumMemberDecl decl)
        {
            Parent = parent;
            Name = decl.Name;
            Value = decl.Value;
            Comment = decl.Comment;
            IsTerminator = decl.IsTerminator;
            CapitalizedName = decl.CapitalizedName;
            CommonSnakeName = decl.CommonSnakeName;
        }
        #endregion
    }

    /// <summary>
    /// 各エンティティの基底
    /// </summary>
    class CLEntity
    {
        protected CLAnalyzer _analyzer;

        public CLEntity()
        {
        }

        public CLEntity(CLAnalyzer analyzer)
        {
            _analyzer = analyzer;
        }
    }

    /// <summary>
    /// プロパティ (getter と setter のセット)
    /// </summary>
    class CLProperty : CLEntity
    {
        #region Type property
        /// <summary>
        /// 型 (Generic 型引数も欲しいので CLParam として返す)
        /// </summary>
        public CLParam Type
        {
            get
            {
                // getter 優先にしておく (Getter がある場合、 Type は return であることが前提)

                // getter の戻り値型
                if (Getter != null)
                {
                    //if (Getter.ReturnType.CPPFullTypeName != "void")
                    //    return Getter.ReturnType;
                    //else
                    return Getter.Params.Last();    // 第2引数が out なもの
                }

                // setter の第2引数
                if (Setter != null)
                    return Setter.Params[1];

                throw new InvalidOperationException();
            }
        }
        #endregion

        #region Getter property
        /// <summary>
        /// getter
        /// </summary>
        public CLMethod Getter { get; set; }
        #endregion

        #region Setter property
        /// <summary>
        /// setter
        /// </summary>
        public CLMethod Setter { get; set; }
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

        #region OwnerClass property
        /// <summary>
        /// このプロパティを持つクラス
        /// </summary>
        public CLClassType OwnerClass { get; private set; }
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
                    Getter.Attribute != Setter.Attribute)
                    throw new InvalidOperationException();

                MethodAttribute attr;
                if (Getter != null)
                    attr = Getter.Attribute;
                else //if (Setter != null)
                    attr = Setter.Attribute;

                if (attr == MethodAttribute.Static ||
                    attr == MethodAttribute.DefaultInstanceGetter ||
                    attr == MethodAttribute.StructOutputStatic)
                    return true;
                if (attr == MethodAttribute.Constructor ||     // enum 増やしたときの漏れ防止のため、単なるelse にはしない
                    attr == MethodAttribute.Instance ||
                    attr == MethodAttribute.StructInstance)
                    return false;

                throw new InvalidOperationException();
            }
        }
        #endregion

        #region IsVirtual property
        /// <summary>
        /// 仮想関数であるか (サブクラスのメソッドに対して true を設定すると再帰的に基底クラスを virtual にマークしていく)
        /// </summary>
        public bool IsVirtual
        {
            get
            {
                if (Getter != null && Getter.IsVirtual)
                    return true;
                if (Setter != null && Setter.IsVirtual)
                    return true;
                return false;
            }
        }
        #endregion

        #region IsBaseVirtual property
        /// <summary>
        /// 一番基底の仮想関数であるか
        /// </summary>
        public bool IsBaseVirtual
        {
            get
            {
                if (Getter != null && Getter.IsBaseVirtual)
                    return true;
                if (Setter != null && Setter.IsBaseVirtual)
                    return true;
                return false;
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

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLProperty(CLAnalyzer analyzer, CLClassType ownerClass)
            : base(analyzer)
        {
            OwnerClass = ownerClass;
        }
        #endregion
    }

    /// <summary>
    /// メソッド (関数ひとつ分)
    /// </summary>
    class CLMethod : CLEntity
    {
        #region Fields
        private FuncDocNode _funcDocNode;
        private FuncDeclNode _funcDeclNode;
        #endregion

        #region Attribute property
        /// <summary>
        /// メソッド属性
        /// </summary>
        public MethodAttribute Attribute { get; set; }
        #endregion

        #region Name property
        /// <summary>
        /// プレフィックスを除いた名前。SetMatrix 等
        /// </summary>
        public string Name { get; private set; }
        #endregion

        #region CName property
        /// <summary>
        /// C言語での関数名
        /// </summary>
        public string CName { get { return _funcDeclNode.Name; } }
        #endregion

        #region Option property
        /// <summary>
        /// オプション
        /// </summary>
        public FuncOptionNode Option { get; set; }
        #endregion

        #region ReturnType property
        /// <summary>
        /// 戻り値型
        /// </summary>
        public CLReturnType ReturnType { get; private set; }
        #endregion

        #region Params property
        /// <summary>
        /// 仮引数
        /// </summary>
        public List<CLParam> Params { get; private set; }
        #endregion

        #region OwnerClass property
        /// <summary>
        /// このプロパティを持つクラス
        /// </summary>
        public CLClassType OwnerClass { get; private set; }
        #endregion

        #region IsInstanceMethod property
        /// <summary>
        /// Attribute がインスタンスメソッドであるか
        /// </summary>
        public bool IsInstanceMethod
        {
            get
            {
                return (Attribute == MethodAttribute.Instance ||
                        Attribute == MethodAttribute.Constructor ||
                        Attribute == MethodAttribute.StructInstance );
            }
        }
        #endregion

        #region IsStaticMethod property
        /// <summary>
        /// Attribute が static メソッドであるか
        /// </summary>
        public bool IsStaticMethod
        {
            get
            {
                return (Attribute == MethodAttribute.Static ||
                        Attribute == MethodAttribute.StructOutputStatic ||
                        Attribute == MethodAttribute.DefaultInstanceGetter);
            }
        }
        #endregion

        #region IsLibraryInitializer property
        /// <summary>
        /// LManager_Initialize～
        /// </summary>
        public bool IsLibraryInitializer
        {
            get
            {
                if (OwnerClass.Name == "Core" && Name.Contains("Initialize"))
                    return true;
                return false;
            }
        }
        #endregion

        #region IsVirtual property
        /// <summary>
        /// 仮想関数であるか (サブクラスのメソッドに対して true を設定すると再帰的に基底クラスを virtual にマークしていく)
        /// </summary>
        public bool IsVirtual { get; set; }
        #endregion

        #region IsBaseVirtual property
        /// <summary>
        /// 一番基底の仮想関数であるか
        /// </summary>
        public bool IsBaseVirtual
        {
            get
            {
                if (OwnerClass.BaseType != null)
                {
                    var method = ((CLClassType)OwnerClass.BaseType).TryGetBaseVirtualMethod(CName);
                    return method == null;
                }
                return false;
            }
        }
        #endregion

        #region IsCopy property
        /// <summary>
        /// コピーで作成されたものであるか
        /// 構造体の Normalize() 等のインスタンスと static 2種同時作成で、
        /// P/Invoke 側を多重定義しないために使用する。
        /// </summary>
        public bool IsCopy { get; private set; }
        #endregion

        #region PropertyType property
        /// <summary>
        /// プロパティに属する場合、その種類
        /// </summary>
        public PropertyNameType PropertyType { get; set; }
        #endregion

        #region PropertyName property
        /// <summary>
        /// プロパティに属する場合、その名前
        /// </summary>
        public string PropertyName { get; set; }
        #endregion

        #region OwnerProperty property
        /// <summary>
        /// 属するプロパティ
        /// </summary>
        public CLProperty OwnerProperty { get; set; }
        #endregion

        #region Summary property
        /// <summary>
        /// 概要説明文
        /// </summary>
        public string Summary { get { return _funcDocNode.Summary; } }
        #endregion

        #region Detail property
        /// <summary>
        /// 詳細説明文
        /// </summary>
        public string Detail { get { return _funcDocNode.Detail; } }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLMethod(CLAnalyzer analyzer, CLClassType ownerClass, FuncDocNode doc, FuncDeclNode decl)
            : base(analyzer)
        {
            _funcDocNode = doc;
            _funcDeclNode = decl;
            OwnerClass = ownerClass;
            Name = analyzer.RemoveOverloadSuffix(decl.CommonName);
            ReturnType = new CLReturnType(analyzer, decl.ReturnType, doc.Return);

            Option = new FuncOptionNode();
            PropertyType = PropertyNameType.NotProperty;

            // 仮引数をリスト化
            Params = new List<CLParam>();
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

                var fp = new CLParam(analyzer, paramDoc, param, this);
                Params.Add(fp);
            }
        }

        /// <summary>
        /// 現在の情報を元に、この関数の種類を決定する
        /// </summary>
        public void UpdateMethodAttribute()
        {
            // 必要なものがそろっているか
            if (string.IsNullOrEmpty(Name)) throw new InvalidOperationException("Name");
            if (Params == null) throw new InvalidOperationException("Params");
            //if (ReturnType == null) throw new InvalidOperationException("ReturnType");

            UpdateParamsType();

            // 以下のいずれかの条件にも一致しなければ static である
            Attribute = MethodAttribute.Static;

            if (Option.ForceStaticMethod)
            {
                // static 強制
            }
            // 先頭が Create ならコンストラクタ関数
            else if (string.Compare(Name, 0, "Create", 0, 6) == 0)
            {
                Attribute = MethodAttribute.Constructor;
            }
            // 先頭が GetDefault ならデフォルトで作成される lnHandle の getter
            else if (string.Compare(Name, 0, "GetDefault", 0, 10) == 0)
            {
                Attribute = MethodAttribute.DefaultInstanceGetter;
                PropertyType = PropertyNameType.Get;    // get プロパティ扱い
                PropertyName = Name.Substring(3); // Get 以降をプロパティ名に\
            }
            else if (Params.Count() > 0)
            {
                var firstParam = Params.First();
                //StructDecl structDecl;
                // 第1引数が Handle 型、または構造体型であればインスタンスメソッドかもしれない
                //else if (firstParam.CPPFullTypeName == "lnHandle" ||
                //    (firstParam.CommonTypeName == FuncDeclNode.CommonModuleName))
                if (firstParam.Type is CLClassType &&
                    firstParam.Type.Name == OwnerClass.Name)
                {
                    // 第1引数が lnHandle ならインスタンスメソッド。それ以外の型なら StructInstance
                    if (!((CLClassType)firstParam.Type).IsStruct)
                        Attribute = MethodAttribute.Instance;
                    else
                        Attribute = MethodAttribute.StructInstance;

                    // 引数がインスタンス系のみで、戻り値がある場合は getter かもしれない
                    // 引数がインスタンス系ともうひとつ、これが out 型の場合は getter かもしれない
                    // 引数がインスタンス系ともうひとつ、これが out 以外の場合は setter かもしれない
                    //if ((AllParams.Count() == 1 && ReturnType.CPPFullTypeName != "void") ||
                    //    (AllParams.Count() == 2 && ReturnType.CPPFullTypeName == "void"))7
                    if (/*Params.Count() == 1 || */Params.Count() == 2)
                    {
                        if (string.Compare(Name, 0, "Get", 0, 3) == 0)
                        {
                            PropertyType = PropertyNameType.Get;
                            PropertyName = Name.Substring(3);
                        }
                        else if (string.Compare(Name, 0, "Set", 0, 3) == 0)
                        {
                            PropertyType = PropertyNameType.Set;
                            PropertyName = Name.Substring(3);
                        }
                        else if (string.Compare(Name, 0, "Is", 0, 2) == 0)
                        {
                            PropertyType = PropertyNameType.Is;
                            PropertyName = Name.Substring(2);
                        }
                    }
                }



                // 第1引数がモジュールと一致する構造体型で、かつ out タイプの場合は static な struct 関数
                //if (analyzer.TryGetStructDeclByName(firstParam.CPPFullTypeName, out structDecl) &&
                //    FuncDeclNode.CommonModuleName == structDecl.CommonName &&
                //    firstParam.IOType == ParamIO.Out)
                //if (firstParam.IsOutStructType)
                //{
                //    Attribute = MethodAttribute.StructOutputStatic;
                //}
                if (Params.Last().IsOutStructType &&
                    PropertyType == PropertyNameType.NotProperty)
                {
                    Attribute = MethodAttribute.StructOutputStatic;
                }
            }

            // プロパティ化しないオプションがあれば強制的に戻す
            if (!Option.AutoPropertyGenerate)
                PropertyType = PropertyNameType.NotProperty;
        }

        /// <summary>
        /// 現在の情報を元に、この関数の種類を決定する
        /// (class の属性が決まった後に呼ばれる)
        /// </summary>
        public void PostUpdateMethodAttribute()
        {
            // struct 型で Set は特殊扱い
            if (OwnerClass.Attribute == CLModuleAttr.Struct &&
                Name == "Set")
            {
                Attribute = MethodAttribute.StructInstance;
            }
        }

        /// <summary>
        /// 仮想関数フラグを再帰的に更新する
        /// </summary>
        public void UpdateVirtualMark()
        {
            if (!string.IsNullOrEmpty(_funcDocNode.Override))
            {
                IsVirtual = true;
                OwnerClass.UpdateVirtualMark(CName);
            }
        }

        /// <summary>
        /// 引数の型を決定する
        /// </summary>
        private void UpdateParamsType()
        {
            foreach (var param in Params)
            {
                param.UpdateParamsType();
            }
        }

        /// <summary>
        /// 簡易コピー
        /// </summary>
        /// <returns></returns>
        public CLMethod Copy()
        {
            var m = (CLMethod)MemberwiseClone();
            m.IsCopy = true;
            return m;
        }

        #endregion
    }

    /// <summary>
    /// 仮引数
    /// </summary>
    class CLParam : CLEntity
    {
        #region Fields
        private CLAnalyzer _analyzer;
        private FuncParamDocNode _funcParamDocNode;
        private FuncParamDeclNode _funcParamDeclNode;
        #endregion

        #region Type property
        /// <summary>
        /// 型情報
        /// </summary>
        public CLType Type { get; private set; }
        #endregion

        #region GenericTypeArg property
        /// <summary>
        /// 型情報
        /// </summary>
        public CLType GenericTypeArg { get; private set; }
        #endregion

        #region Name property
        /// <summary>
        /// 名前
        /// </summary>
        public string Name { get; private set; }
        #endregion

        #region Attribute property
        /// <summary>
        /// 属性
        /// </summary>
        public ParamAttribute Attribute { get; private set; }
        #endregion

        #region Summary property
        /// <summary>
        /// 説明文
        /// </summary>
        public string Summary { get; private set; }
        #endregion

        #region DefaultValueSource property
        /// <summary>
        /// デフォルト引数値 (.h の関数宣言の値そのまま)
        /// </summary>
        public string DefaultValueSource { get; private set; }
        #endregion

        #region OwnerMethod property
        /// <summary>
        /// この仮引数を持つメソッド
        /// </summary>
        public CLMethod OwnerMethod { get; private set; }
        #endregion

        #region IsInReferenceObjectType property
        /// <summary>
        /// Type が入力型の参照オブジェクト型であるか
        /// </summary>
        public bool IsInReferenceObjectType
        {
            get
            {
                var t = Type as CLClassType;
                return (
                    t != null &&
                    t.IsReferenceObject &&
                    (Attribute & ParamAttribute.In) != 0 &&
                    (Attribute & ParamAttribute.Out) == 0);
            }
        }
        #endregion

        #region IsOutReferenceObjectType property
        /// <summary>
        /// Type が出力型の参照オブジェクト型であるか
        /// </summary>
        public bool IsOutReferenceObjectType
        {
            get
            {
                var t = Type as CLClassType;
                return (
                    t != null &&
                    t.IsReferenceObject &&
                    (Attribute & ParamAttribute.In) == 0 &&
                    (Attribute & ParamAttribute.Out) != 0);
            }
        }
        #endregion

        #region IsInOutStructType property
        /// <summary>
        /// Type が参照型の構造体型であるか
        /// </summary>
        public bool IsInOutStructType
        {
            get
            {
                var t = Type as CLClassType;
                return (
                    t != null &&
                    t.IsStruct &&
                    (Attribute & ParamAttribute.In) != 0 &&
                    (Attribute & ParamAttribute.Out) != 0);
            }
        }
        #endregion

        #region IsOutStructType property
        /// <summary>
        /// Type が出力型の構造体型であるか
        /// </summary>
        public bool IsOutStructType
        {
            get
            {
                var t = Type as CLClassType;
                return (
                    t != null &&
                    t.IsStruct &&
                    (Attribute & ParamAttribute.In) == 0 &&
                    (Attribute & ParamAttribute.Out) != 0);
            }
        }
        #endregion

        #region IsOutString property
        /// <summary>
        /// Type が出力型の string 型であるか
        /// </summary>
        public bool IsOutString
        {
            get
            {
                var t = Type as CLPrimitiveType;
                return (
                    t != null &&
                    t == CLPrimitiveType.String &&
                    (Attribute & ParamAttribute.In) == 0 &&
                    (Attribute & ParamAttribute.Out) != 0);
            }
        }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLParam(CLAnalyzer analyzer, FuncParamDocNode doc, FuncParamDeclNode decl, CLMethod ownerMethod)
            : base(analyzer)
        {
            _analyzer = analyzer;
            _funcParamDocNode = doc;
            _funcParamDeclNode = decl;
            Name = decl.Name;
            Summary = doc.Summary;
            DefaultValueSource = decl.DefaultValue;
            OwnerMethod = ownerMethod;
        }

        /// <summary>
        /// 引数の型を決定する
        /// </summary>
        public void UpdateParamsType()
        {
            // オプションから型の強制をチェック
            string forcedType;
            if (OwnerMethod.Option.ParamTypes.TryGetValue(Name, out forcedType))
            {
                var r = new Regex(@"<.+>");
                var m = r.Match(forcedType);
                if (!m.Success)
                    throw new InvalidOperationException();

                // <> を削除して型引数とする
                string typeArg = m.Value;
                typeArg = typeArg.Replace("<", "").Replace(">", "");

                // 型引数部分を削除して型検索
                forcedType = forcedType.Replace(m.Value, "");
                Type = _analyzer.GetClassTypeByName(forcedType);

                // 型引数の CLType を検索
                GenericTypeArg = _analyzer.GetClassTypeByName(typeArg);
            }
            // 型検索
            else
            {
                CLType type;
                ParamAttribute attr;
                if (_analyzer.TryGetTypeByCTypeFullName(_funcParamDeclNode.Type, out type, out attr))
                {
                    Type = type;
                    Attribute = attr;

                    // Handle 型の場合は推論する
                    if (Type == CLPrimitiveType.Handle)
                    {
                        Type = _analyzer.AllClassTypes.Find((item) => Name.EndsWith(item.Name, true, System.Globalization.CultureInfo.DefaultThreadCurrentCulture));
                        if (Type == null)
                        {
                            if (Name == "item" || Name == "outItem")
                            {
                                Console.WriteLine("引数名からクラス名の特定の特殊化 {0}", Name);
                                Type = CLPrimitiveType.Generic;
                            }
                        }
                        if (Type == null) throw new InvalidOperationException();
                    }
                }
            }

            // 入出力の決定 (↑でも取得しているが、とりあえずここではコメント優先)
            Attribute = 0;
            Attribute |= (_funcParamDocNode.IO.IndexOf("in") >= 0) ? ParamAttribute.In : 0;
            Attribute |= (_funcParamDocNode.IO.IndexOf("out") >= 0) ? ParamAttribute.Out : 0;
            if (Attribute == 0)
                throw new InvalidOperationException();  // IO が空等
        }

        #endregion
    }

    /// <summary>
    /// 戻り値型
    /// </summary>
    class CLReturnType : CLEntity
    {
        #region Type property
        /// <summary>
        /// 型
        /// </summary>
        public CLType Type { get; private set; }
        #endregion

        #region Summary property
        /// <summary>
        /// 説明文
        /// </summary>
        public string Summary { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLReturnType(CLAnalyzer analyzer, string cTypeName, string comment)
            : base(analyzer)
        {
            Summary = comment;
            CLType type;
            ParamAttribute attr;
            if (analyzer.TryGetTypeByCTypeFullName(cTypeName, out type, out attr))
            {
                Type = type;
            }
            else
                throw new InvalidOperationException();
        }
        #endregion
    }
}
