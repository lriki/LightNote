using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// 関数定義とドキュメントのまとまり
    /// </summary>
    class FuncSection
    {
        public FuncDocNode FuncDoc;
        public FuncDeclNode FuncDecl;
        public FuncOptionNode FuncOption = new FuncOptionNode();
    }

    /// <summary>
    /// プロパティの getter setter のペア
    /// </summary>
    class Property
    {
        public string Name;
        public string ModuleName;
        public string KeyName;
        public FuncDeclNode Getter;
        public FuncDeclNode Setter;

        #region GetSetType property
        public FuncParamDeclNode GetSetType
        {
            get
            {
                // 現在 getter はラッパー型が正しく取れないので setter 優先
                if (Setter != null)
                    return Setter.InstanceMethodParams.First();

                if (Getter != null)
                {
                    if (Getter.ReturnType != "void")
                    {
                        // TODO: 現状、getter 戻り値の lnHandle は実体のクラス名がわからない。
                        if (Getter.ReturnType == "lnHandle")
                            throw new InvalidOperationException();

                        // TODO: すごくやむなし。呼び出し側は  IsHandleWrapperType を使いたいので
                        // FuncParamDeclNode 型で返す必要があるので、後で型情報は
                        // string じゃなくてちゃんとしたクラスに統一しておく
                        return new FuncParamDeclNode() { Type = Getter.ReturnType };
                    }
                    else
                        return Getter.InstanceMethodParams.First();    // 第2引数が out なもの
                }

                throw new InvalidOperationException();
#if false
                if (Getter != null)
                {
                    if (Getter.ReturnType != "void")
                        return Getter.ReturnType;                           // 戻り値でとれるもの
                    else
                        return Getter.InstanceMethodParams.First().Type;    // 第2引数が out なもの
                }
                if (Setter != null)
                    return Setter.InstanceMethodParams.First().Type;
                throw new InvalidOperationException();
#endif
            }
        }
        #endregion

        #region IsGetterReturnType property
        // getter (API) が return で値を戻しているか (false の場合は第2引数が out である)
        public bool IsGetterReturnType
        {
            get
            {
                if (Getter == null)
                    throw new InvalidOperationException();
                return (Getter.ReturnType != "void");
            }
        }
        #endregion
    }

    enum ModuleType
    {
        Instance,   // lnHandle 関係の関数を持っている
        Static,     // static 関数のみ持っている
        Abstract,   // コンストラクタを持っていない
    }

    /// <summary>
    /// モジュール (関数の集合)
    /// </summary>
    class Module
    {
        public string KeyName;      // L 有りモジュール名
        public string CommonName;   // L 無しモジュール名
        public bool IsHandleWrapper;
        public List<Property> Properties = new List<Property>();
        public List<FuncSection> Functions = new List<FuncSection>();
        public List<FuncSection> FunctionsWithoutProperty = new List<FuncSection>();
        public StructDecl StructClass;  // 構造体クラス
        public ModuleType ModuleType;
        public ModuleDocNode ModuleDoc;
    }

    class C_APIAnalyzer
    {
        public const string StructOrEnumPrefix = "ln";
        public const string FuncPrefix = "LN";

        // tmp
        public CLAnalyzer CLAnalyzer { get; private set; }

        /// <summary>
        /// オーバーロードを表現するサフィックス
        /// </summary>
        private static string[] OverloadSuffix = new string[] 
        {
            "XY", "XYZ", "XYZW", "WH", "XYWH",
            "Vec2", "Vec3", "Vec4",
        };

        /// <summary>
        /// バージョン文字列
        /// </summary>
        public string Version { get; private set; }

        /// <summary>
        /// 全関数エンティティ
        /// </summary>
        public List<FuncEntity> AllFuncEntites { get; private set; }

        /// <summary>
        /// 全プロパティエンティティ
        /// </summary>
        public List<PropertyEntity> AllPropertyEntites { get; private set; }

        /// <summary>
        /// 全モジュールエンティティ
        /// </summary>
        public List<ModuleEntity> AllModuleEntites { get; private set; }




        private IEnumerable<Node> _typedefHeaderNodes;

        /// <summary>
        /// enum 定義ノード一覧
        /// </summary>
        public List<EnumDecl> AllEnumNodes { get; private set; }

        /// <summary>
        /// struct 定義ノード一覧
        /// </summary>
        public List<StructDecl> AllStructNodes { get; private set; }

#if true
        /// <summary>
        /// 関数定義一覧
        /// </summary>
        public List<FuncSection> AllFuncSections { get; private set; }

        /// <summary>
        /// プロパティ一覧
        /// </summary>
        public List<Property> AllProperties { get; private set; }

        /// <summary>
        /// モジュール一覧
        /// </summary>
        public List<Module> AllModules { get; private set; }
#endif

        /// <summary>
        /// 解析実行
        /// </summary>
        public void Analyze(string commonHeaderPath, string typedefHeaderPath, string[] apiHeaderPathes)
        {
            var commonHeaderAnalyzer = new CommonHeaderAnalyzer();
            commonHeaderAnalyzer.Analyze(commonHeaderPath);
            Version = commonHeaderAnalyzer.Version;

            var typedefHeaderAnalyzer = new TypedefHeaderAnalyzer();
            _typedefHeaderNodes = typedefHeaderAnalyzer.Analyze(typedefHeaderPath);


            //-------------------------------------------------
            // enum ノード抽出
            AllEnumNodes = new List<EnumDecl>();
            foreach (var node in _typedefHeaderNodes)
            {
                if (node is EnumDecl)
                    AllEnumNodes.Add((EnumDecl)node);
            }
            // lnErrorCode を複製して lnResult として登録する
            //EnumDecl enumDecl;
            //if (TryGetEnumDeclByName("lnErrorCode", out enumDecl))
            //{
            //    var newDecl = enumDecl.Clone();
            //    newDecl.Name = "lnResult";
            //    AllEnumNodes.Add(newDecl);
            //}

            //-------------------------------------------------
            // struct ノード抽出
            AllStructNodes = new List<StructDecl>();
            foreach (var node in _typedefHeaderNodes)
            {
                if (node is StructDecl)
                    AllStructNodes.Add((StructDecl)node);
            }

            //-------------------------------------------------
            // 
            var apiHeaderAnalyzer = new APIHeaderAnalyzer();
            var _funcSecNodes = apiHeaderAnalyzer.Analyze(apiHeaderPathes);



            // tmp
            CLAnalyzer = new BinderMaker.CLAnalyzer();
            CLAnalyzer.Analyze(_typedefHeaderNodes, _funcSecNodes);
            return;
#if true
            //-------------------------------------------------
            // 関数セクション作成
            AllFuncSections = new List<FuncSection>();
            FuncDocNode lastFuncDoc = null;
            FuncSection lastFuncSection = null;
            foreach (var node in _funcSecNodes)
            {
                if (node is FuncDocNode)
                    lastFuncDoc = (FuncDocNode)node;

                if (node is FuncDeclNode)
                {
                    var f = new FuncSection();
                    f.FuncDoc = (FuncDocNode)lastFuncDoc;
                    f.FuncDecl = (FuncDeclNode)node;
                    AllFuncSections.Add(f);
                    lastFuncDoc = null;

                    lastFuncSection = f;
                }

                if (node is FuncOptionNode)
                {
                    lastFuncSection.FuncOption = (FuncOptionNode)node;
                    lastFuncSection = null;
                }
            }

            //-------------------------------------------------
            // 引数の IO を決定
            //      FuncParamDoc のもつ [in,out] を、同盟の FuncParam にセットする
            foreach (var func in AllFuncSections)
            {
                var docParams = new List<FuncParamDocNode>(func.FuncDoc.Params);    // Find したいために List 化。あとで拡張メソッドにしたい…
                foreach (var param in func.FuncDecl.Params)
                {
                    var docParam = docParams.Find((item) => item.Name == param.Name);
                    if (docParam == null)
                        throw new InvalidOperationException();  // 必ず同じ名前のモノがあるはず

                    param.IOType = docParam.IOType;
                }

                // IO が決定してから処理するもの
                func.FuncDecl.PostAnalyze2();

                if (!func.FuncOption.AutoPropertyGenerate)
                    func.FuncDecl.PropertyType = PropertyNameType.NotProperty;
            }

            //-------------------------------------------------
            // プロパティペアのリストアップ
            AllProperties = new List<Property>();
            foreach (var func in AllFuncSections)
            {
                if (func.FuncDecl.PropertyType != PropertyNameType.NotProperty)
                {
                    // 既存検索
                    Property prop = AllProperties.Find((item) => item.ModuleName == func.FuncDecl.ModuleName &&  item.KeyName == func.FuncDecl.PropertyName);
                    if (prop == null)
                    {
                        prop = new Property();
                        prop.ModuleName = func.FuncDecl.ModuleName;
                        prop.KeyName = func.FuncDecl.PropertyName;
                        AllProperties.Add(prop);
                    }

                    // getter setter 割り振り
                    switch (func.FuncDecl.PropertyType)
                    {
                        case PropertyNameType.Set:
                            prop.Setter = func.FuncDecl;
                            break;
                        case PropertyNameType.Is:
                        case PropertyNameType.Get:
                            prop.Getter = func.FuncDecl;
                            break;
                    }
                }
            }
            // プロパティ名の決定
            foreach (var prop in AllProperties)
            {
                if (prop.Getter != null && prop.Getter.PropertyType == PropertyNameType.Is)
                    prop.Name = "Is" + prop.Getter.PropertyName;
                else if (prop.Getter != null)
                    prop.Name = prop.Getter.PropertyName;
                else //if (prop.Setter != null)
                    prop.Name = prop.Setter.PropertyName;
            }

            //-------------------------------------------------
            // モジュールリスト
            AllModules = new List<Module>();
            foreach (var func in AllFuncSections)   // 関数一覧
            {
                Module modules = AllModules.Find((item) => item.KeyName == func.FuncDecl.ModuleName);
                if (modules == null)
                {
                    modules = new Module();
                    modules.KeyName = func.FuncDecl.ModuleName;
                    modules.CommonName = modules.KeyName.Substring(FuncPrefix.Length);  // プリフィックス L を除く
                    AllModules.Add(modules);
                }
                modules.Functions.Add(func);

                // プロパティではない関数一覧にも追加
                if (FindPropertyContainsFunc(func.FuncDecl) == null)
                    modules.FunctionsWithoutProperty.Add(func);

                // インスタンスメソッドを含んでいれば lnHandle ラッパー
                if (func.FuncDecl.FuncType == FuncType.Instance ||
                    func.FuncDecl.FuncType == FuncType.Constructor)
                    modules.IsHandleWrapper = true;
            }
            foreach (var prop in AllProperties)     // プロパティ一覧
            {
                Module modules = AllModules.Find((item) => item.KeyName == prop.ModuleName);
                if (modules == null)
                {
                    modules = new Module();
                    modules.KeyName = prop.ModuleName;
                    modules.CommonName = modules.KeyName.Substring(FuncPrefix.Length);  // プリフィックス L を除く
                    AllModules.Add(modules);
                }
                modules.Properties.Add(prop);

                // プロパティがあれば問答無用で lnHandle ラッパー
                modules.IsHandleWrapper = true;
            }

            //-------------------------------------------------
            // モジュールと構造体の関連付け
            // (struct に対応する API が一つもない場合でもモジュールとして登録する)
            foreach (var structDecl in AllStructNodes)
            {
                Module modules = AllModules.Find((item) => item.CommonName == structDecl.CommonName);
                if (modules == null)
                {
                    modules = new Module();
                    modules.KeyName = "L" + structDecl.CommonName;
                    modules.CommonName = structDecl.CommonName;
                    AllModules.Add(modules);
                }
                modules.StructClass = structDecl;
            }

            //-------------------------------------------------
            // モジュール性質の決定
            foreach (var module in AllModules)
            {
                bool hasConstructor = false;
                bool hasInstanceMethod = false;
                foreach (var func in module.Functions)
                {
                    if (func.FuncDecl.FuncType == FuncType.Constructor)
                    {
                        hasConstructor = true;
                        hasInstanceMethod = true;
                    }
                    if (func.FuncDecl.FuncType == FuncType.Instance)
                        hasInstanceMethod = true;
                }

                if (hasInstanceMethod)
                {
                    if (hasConstructor)
                        module.ModuleType = ModuleType.Instance;
                    else
                        module.ModuleType = ModuleType.Abstract;
                }
                else
                    module.ModuleType = ModuleType.Static;
            }

            //-------------------------------------------------
            // lnHandle と対応するモジュールの関連付け
            foreach (var func in AllFuncSections)
            {
                foreach (var param in func.FuncDecl.Params)
                {
                    if (param.Type == "lnHandle")
                    {
                        param.HandleWrapperModule = GetModuleByNoCaseCommonName(param.Name);
                        if (param.HandleWrapperModule == null)
                        {
                            if (func.FuncDecl.ModuleName == "LObjectList" &&
                                param.Name == "item")
                            {
                                //Console.WriteLine("引数名からクラス名の特定の特殊化 {0}", func.FuncDecl.Name);
                                param.HandleWrapperModule = GetModuleByNoCaseCommonName("Object");
                            }
                            else
                                throw new InvalidOperationException("引数名からクラス名の特定に失敗 : " + func.FuncDecl.Name + " " + param.Name);
                        }
                    }
                }
            }

            //-------------------------------------------------
            // FuncType.StructOutput 確認
            foreach (var func in AllFuncSections)
            {
                if (func.FuncDecl.Params.Count() > 0)
                {
                    // 第1引数が構造体型であるか
                    StructDecl decl;
                    if (TryGetStructDeclByName(func.FuncDecl.Params.First().Type, out decl))
                    {
                        // さらに、関数のモジュール名と同一であれば StructOutputStatic とする
                        if (func.FuncDecl.CommonModuleName == decl.CommonName &&
                            func.FuncDecl.Params.First().IOType == ParamIO.Out)
                            func.FuncDecl.FuncType = FuncType.StructOutputStatic;
                    }
                }
            }

            //-------------------------------------------------
            // Module と ModuleDocNode の関連付け
            foreach (var module in AllModules)
            {
                foreach (var node in _funcSecNodes)
                {
                    var docNode = node as ModuleDocNode;
                    if (docNode != null &&
                        module.KeyName == docNode.GroupInfo.Title)
                    {
                        module.ModuleDoc = docNode;
                        break;
                    }
                }
                if (module.ModuleDoc == null)
                    throw new InvalidOperationException();  // 見つからなかった
                // 構造体で↑に引っかかるときは、モジュールコメントが無い状態。
                // 強引に作っておく。
            }

#endif
            //=================================================================
            // 新方式

            //-------------------------------------------------
            // 全関数エンティティ作成
            AllFuncEntites = new List<FuncEntity>();
            lastFuncDoc = null;
            FuncEntity lastFuncEntity = null;
            foreach (var node in _funcSecNodes)
            {
                if (node is FuncDocNode)
                    lastFuncDoc = (FuncDocNode)node;

                if (node is FuncDeclNode)
                {
                    var f = new FuncEntity();
                    f.SetDocAndDeclNode(this, (FuncDocNode)lastFuncDoc, (FuncDeclNode)node);
                    AllFuncEntites.Add(f);
                    lastFuncDoc = null;
                    lastFuncEntity = f;
                }

                if (node is FuncOptionNode)
                {
                    lastFuncEntity.SetFuncOptionNode((FuncOptionNode)node);
                    lastFuncEntity = null;
                }
            }
            // 関数の種類はここで決定しておかないと、次のプロパティ作成で必要な情報がそろわない
            foreach (var func in AllFuncEntites)
                func.UpdateFuncType(this);     

            //-------------------------------------------------
            // プロパティ作成
            AllPropertyEntites = new List<PropertyEntity>();
            foreach (var func in AllFuncEntites)
            {
                // 関数チェックでプロパティとしてマークされている
                if (func.PropertyType != PropertyNameType.NotProperty)
                {
                    // 既存検索
                    PropertyEntity prop = AllPropertyEntites.Find((item) => item.CppModuleName == func.CppModuleName && item.KeyName == func.PropertyName);
                    if (prop == null)
                    {
                        prop = new PropertyEntity();
                        AllPropertyEntites.Add(prop);
                    }

                    // getter setter 割り振り
                    switch (func.PropertyType)
                    {
                        case PropertyNameType.Set:
                            prop.Setter = func;
                            break;
                        case PropertyNameType.Is:
                        case PropertyNameType.Get:
                            prop.Getter = func;
                            break;
                    }
                }
            }
            //-------------------------------------------------
            // モジュール作成
            AllModuleEntites = new List<ModuleEntity>();
            // モジュールに構造体情報を設定する(必要があればモジュールを新しく作成する)
            foreach (var node in _typedefHeaderNodes)
            {
                var structDecl = node as StructDecl;
                if (structDecl != null)
                {
                    ModuleEntity module = AllModuleEntites.Find((item) => item.CommonName == structDecl.CommonName);
                    if (module == null)
                    {
                        // 新しく作る
                        module = new ModuleEntity("L" + structDecl.CommonName);
                        AllModuleEntites.Add(module);
                    }
                    // 構造体をベースとするモジュールは struct Type である
                    module.StructDecl = structDecl;
                }
            }
            // モジュールに関数を追加する(必要があればモジュールを新しく作成する)
            foreach (var func in AllFuncEntites)
            {
                ModuleEntity module = AllModuleEntites.Find((item) => item.CPPName == func.CppModuleName);
                if (module == null)
                {
                    // 新しく作る
                    module = new ModuleEntity(func.CppModuleName);
                    AllModuleEntites.Add(module);
                }
                module.Functions.Add(func);
                func.ParentModule = module;

                // 必要があればプロパティでは無い関数リストに追加
                if (func.PropertyType == PropertyNameType.NotProperty)
                    module.FunctionsWithoutProperty.Add(func);
            }
            // モジュールにプロパティを追加する(必要があればモジュールを新しく作成する)
            foreach (var prop in AllPropertyEntites)
            {
                ModuleEntity module = AllModuleEntites.Find((item) => item.CPPName == prop.CppModuleName);
                if (module == null)
                {
                    // 新しく作る
                    module = new ModuleEntity(prop.CppModuleName);
                    AllModuleEntites.Add(module);
                }
                module.Properties.Add(prop);
            }
            //-------------------------------------------------
            // ModuleEntity と ModuleDocNode の関連付け
            foreach (var module in AllModuleEntites)
            {
                ModuleDocNode docNode = null;
                foreach (var node in _funcSecNodes)
                {
                    docNode = node as ModuleDocNode;
                    if (docNode != null &&
                        module.CPPName == docNode.GroupInfo.Title)
                    {
                        module.ModuleDoc = docNode;
                        break;
                    }
                }
                if (module.ModuleDoc == null)
                    throw new InvalidOperationException();  // 見つからなかった
                // 構造体で↑に引っかかるときは、モジュールコメントが無い状態。
                // 強引に作っておく。

                if (!string.IsNullOrEmpty(module.ModuleDoc.Extends))
                {
                    var extends = AllModuleEntites.Find((item) => item.CPPName == module.ModuleDoc.Extends);
                    if (extends == null) throw new InvalidOperationException();

                    module.Inheritance = extends;
                }
            }
            
            //-------------------------------------------------
            // エンティティ作成完了後処理
            foreach (var module in AllModuleEntites)
            {
                module.UpdateModuleType();
            }
            foreach (var func in AllFuncEntites)
            {
                func.PostAnalyze(this);
            }


        }





        /// <summary>
        /// C++ enum メンバ名から、定義を検索する
        /// </summary>
        /// <returns></returns>
        public EnumMemberDecl FindEnumMember(string name)
        {
            foreach (var decl in AllEnumNodes)
            {
                foreach (var m in decl.Members)
                {
                    if (m.Name == name)
                        return m;
                }
            }
            throw new InvalidOperationException("Not Found : " + name);
        }

        /// <summary>
        /// ある関数を含んでいるプロパティの取得
        /// </summary>
        /// <param name="func"></param>
        /// <returns>見つからなければ null</returns>
        public Property FindPropertyContainsFunc(FuncDeclNode func)
        {
            foreach (var prop in AllProperties)
            {
                if (prop.Getter == func)
                    return prop;
                if (prop.Setter == func)
                    return prop;
            }

            return null;
        }
       
        /// <summary>
        /// 大文字小文字を区別せず、CommonName からモジュールを取得する
        /// </summary>
        /// <returns></returns>
        public Module GetModuleByNoCaseCommonName(string name)
        {
            var list = AllModules.FindAll((item) => name.EndsWith(item.CommonName, true, System.Globalization.CultureInfo.DefaultThreadCurrentCulture));
            if (list.Count() > 1)
            {
                // item.CommonName と name を逆にして再建策
                list = AllModules.FindAll((item) => item.CommonName.EndsWith(name, true, System.Globalization.CultureInfo.DefaultThreadCurrentCulture));

                if (list.Count() > 1)
                    throw new InvalidOperationException("大文字小文字区別なしの型検索で複数の結果が見つかってしまった。");
            }
            else if (list.Count() == 0)
                return null;
            return list[0];
        }

        /// <summary>
        /// CommonName からモジュールを取得する
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public ModuleEntity GetModuleByCommonName(string name)
        {
            var m = AllModuleEntites.Find((item) => item.CommonName == name);
            if (m == null)
                throw new InvalidOperationException();
            return m;
        }

        /// <summary>
        /// 型名から構造体を取得 (const や * も考慮)
        /// </summary>
        /// <param name="cppType">ln付</param>
        /// <returns></returns>
        public bool TryGetStructDeclByName(string cppType, out StructDecl decl)
        {
            string name = cppType.Replace("const", "");
            name = name.Replace("*", "");
            name.Trim();

            decl = AllStructNodes.Find((item) => item.Name == name);

            return decl != null;
        }

        /// <summary>
        /// 型名からEnumを取得
        /// </summary>
        /// <param name="cppType">ln付</param>
        /// <returns></returns>
        public bool TryGetEnumDeclByName(string cppType, out EnumDecl decl)
        {
            decl = AllEnumNodes.Find((item) => item.Name == cppType);
            return decl != null;
        }

        /// <summary>
        /// 指定した名前の末尾が OverloadSuffix に含まれているかをチェックし、
        /// それを取り除いた名前を返す
        /// </summary>
        /// <returns></returns>
        public string RemoveOverloadSuffix(string str)
        {
            foreach (var s in OverloadSuffix)
            {
                if (str.EndsWith(s, false, System.Globalization.CultureInfo.DefaultThreadCurrentCulture))
                {
                    return str.Substring(0, str.Length - s.Length);
                }
            }
            return str;
        }
    }
}
