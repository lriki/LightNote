using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CLAnalyzer
    {
        /// <summary>
        /// 全モジュール
        /// </summary>
        //public List<CLModule> Modules { get; private set; }

        /// <summary>
        /// ReferenceObject クラス (暗黙的な継承で使う)
        /// </summary>
        //public CLModule ReferenceObjectClass { get; private set; }

        /// <summary>
        /// 全参照オブジェクトクラス
        /// </summary>
        public List<CLClassType> AllClassTypes { get; private set; }

        /// <summary>
        /// 全構造体型
        /// </summary>
        //public List<CLStructType> AllStructTypes { get; private set; }

        /// <summary>
        /// 全プロパティ
        /// </summary>
        public List<CLProperty> AllProperties { get; private set; }

        /// <summary>
        /// 全列挙型
        /// </summary>
        public List<CLEnumType> AllEnumTypes { get; private set; }

        /// <summary>
        /// 全メソッド
        /// </summary>
        public List<CLMethod> Methods { get; private set; }

        /// <summary>
        /// ReferenceObject クラス (暗黙的な継承で使う)
        /// </summary>
        public CLClassType ReferenceObjectClass { get; private set; }

        /// <summary>
        /// ReferenceObjectList クラス
        /// </summary>
        public CLClassType ReferenceObjectListClass { get; private set; }

        /// <summary>
        /// オーバーロード関数のサフィックス
        /// </summary>
        private static string[] OverloadSuffix = new string[] 
        {
            "XYWH","XYZW", "WH", "XYZ", "XY",   // 名前の長い方から見る
            "Vec2", "Vec3", "Vec4",
        };

        /// <summary>
        /// C型名からの変換情報
        /// </summary>
        private struct TypeInfo
        {
            public CLType Type;
            public ParamAttribute ParamAttribute;

            public TypeInfo(CLType type, ParamAttribute attr)
            {
                Type = type;
                ParamAttribute = attr;
            }
        }
        private static Dictionary<string, TypeInfo> _typeInfoTable = new Dictionary<string, TypeInfo>()
        {
            { "lnHandle",           new TypeInfo(CLPrimitiveType.Handle,        ParamAttribute.In) },
            { "lnHandle*",          new TypeInfo(CLPrimitiveType.Handle,        ParamAttribute.Out) },
            { "void",               new TypeInfo(CLPrimitiveType.Void,          ParamAttribute.In) },
            { "void*",              new TypeInfo(CLPrimitiveType.ByteArray,     ParamAttribute.In) },
            { "const void*",        new TypeInfo(CLPrimitiveType.ByteArray,     ParamAttribute.In) },
            { "void**",             new TypeInfo(CLPrimitiveType.ByteArray,     ParamAttribute.Out) },  //TODO: この2つは
            { "const void**",       new TypeInfo(CLPrimitiveType.ByteArray,     ParamAttribute.Out)},   //TODO: バッファクラスを用意する必要がありそう

            { "const lnChar*",      new TypeInfo(CLPrimitiveType.String,        ParamAttribute.In) },
            { "const lnChar**",     new TypeInfo(CLPrimitiveType.String,        ParamAttribute.Out) },

            { "int",                new TypeInfo(CLPrimitiveType.Int,           ParamAttribute.In) },
            { "int*",               new TypeInfo(CLPrimitiveType.Int,           ParamAttribute.Out) },
            { "float",              new TypeInfo(CLPrimitiveType.Float,         ParamAttribute.In) },
            { "float*",             new TypeInfo(CLPrimitiveType.Float,         ParamAttribute.Out) },
            { "double",             new TypeInfo(CLPrimitiveType.Double,        ParamAttribute.In) },
            { "lnBool",             new TypeInfo(CLPrimitiveType.Bool,          ParamAttribute.In) },
            { "lnBool*",　          new TypeInfo(CLPrimitiveType.Bool,          ParamAttribute.Out) },
            { "lnU8",               new TypeInfo(CLPrimitiveType.Byte,          ParamAttribute.In) },
            { "lnU32",              new TypeInfo(CLPrimitiveType.UInt32,        ParamAttribute.In) },

            { "lnResult",           new TypeInfo(CLPrimitiveType.ResultCode,    ParamAttribute.In) },
            { "lnErrorCode",        new TypeInfo(CLPrimitiveType.ResultCode,    ParamAttribute.In) },
            { "lnExceptionCallback",new TypeInfo(CLPrimitiveType.ExceptionCallback,    ParamAttribute.In) },
            { "lnIntPtr",           new TypeInfo(CLPrimitiveType.IntPtr,        ParamAttribute.In) },
            
            { "const int*",         new TypeInfo(CLPrimitiveType.IntNativeArray,ParamAttribute.In) },
            
        }; 

        /// <summary>
        /// パース結果ツリーを解析
        /// </summary>
        /// <param name="typedefHeaderNodes"></param>
        /// <param name="funcSecNodes"></param>
        public void Analyze(IEnumerable<Node> typedefHeaderNodes, IEnumerable<Node> funcSecNodes)
        {
            //-------------------------------------------------
            // 参照クラス・構造体作成
            //Modules = new List<CLModule>();
            AllClassTypes = new List<CLClassType>();
            //AllStructTypes = new List<CLStructType>();
            foreach (var node in funcSecNodes)
            {
                var moduleNode = node as ModuleDocNode;
                if (moduleNode != null)
                {
                    if (moduleNode.GroupInfo.Title.IndexOf(C_APIAnalyzer.FuncPrefix) != 0)
                        throw new InvalidOperationException("モジュール名の先頭が 'LN' ではない");
                    var moduleCommonName = moduleNode.GroupInfo.Title.Substring(C_APIAnalyzer.FuncPrefix.Length);  // プリフィックス削除

                    // 同一名の構造体を探す
                    StructDecl stNode = null;
                    foreach (var typedefNode in typedefHeaderNodes)
                    {
                        var structDecl = typedefNode as StructDecl;
                        if (structDecl != null && structDecl.CommonName == moduleCommonName)
                        {
                            stNode = structDecl;
                        }
                    }

                    // 構造体型を作成
                    if (stNode != null)
                    {
                        var classType = new CLClassType(this, moduleNode, stNode);
                        AllClassTypes.Add(classType);
                    }
                    // 参照オブジェクト型を作成
                    else
                    {
                        var classType = new CLClassType(this, moduleNode);
                        AllClassTypes.Add(classType);

                        // ReferenceObject は覚えておく
                        if (classType.Name == "Object")
                            ReferenceObjectClass = classType;
                        else if (classType.Name == "ObjectList")
                            ReferenceObjectListClass = classType;
                    }
                    //var module = new CLModule(this, moduleNode);
                    //Modules.Add(module);
                    //if (module.Name == "Object")
                    //    ReferenceObjectClass = module;
                }
            }
            //-------------------------------------------------
            // enum ノード抽出
            AllEnumTypes = new List<CLEnumType>();
            foreach (var node in typedefHeaderNodes)
            {
                if (node is EnumDecl)
                {
                    var enumType = new CLEnumType(this, (EnumDecl)node);
                    AllEnumTypes.Add(enumType);
                }
            }
            //-------------------------------------------------
            // 全メソッドエンティティ作成
            Methods = new List<CLMethod>();
            FuncDocNode lastFuncDoc = null;
            CLMethod lastMethodEntity = null;
            foreach (var node in funcSecNodes)
            {
                // 関数コメントのノード。覚えておく
                if (node is FuncDocNode)
                    lastFuncDoc = (FuncDocNode)node;

                // 関数宣言のノード。覚えて置いたコメントと関連付けてエンティティ化
                if (node is FuncDeclNode)
                {
                    // メソッドを持つクラス
                    var ownerType = GetClassTypeByName(((FuncDeclNode)node).CommonModuleName);

                    var m = new CLMethod(this, ownerType, (FuncDocNode)lastFuncDoc, (FuncDeclNode)node);
                    Methods.Add(m);
                    lastFuncDoc = null;
                    lastMethodEntity = m;

                    // クラスに追加
                    ownerType.Methods.Add(m);
                }

                // オプションのノード。最後に作った関数エンティティにくっつける
                if (node is FuncOptionNode)
                {
                    lastMethodEntity.Option = (FuncOptionNode)node;
                    lastMethodEntity = null;
                }
            }
            //-------------------------------------------------
            // 
            // メソッドの属性を決定する
            foreach (var method in Methods)
            {
                method.UpdateMethodAttribute();
                //method.UpdateParamsType();

                if (method.Name == "Identity")
                {
                    method.PropertyName = "Identity";
                    method.PropertyType = PropertyNameType.Get;
                }

            }
            // クラスの属性を決定する
            foreach (var classType in AllClassTypes)
            {
                classType.UpdateAttribute();
            }
            // メソッドの属性を決定する (クラス属性決定後)
            foreach (var method in Methods)
            {
                method.PostUpdateMethodAttribute();
            }
            // メソッドが virtual かチェック
            foreach (var method in Methods)
            {
                method.UpdateVirtualMark();
            }

            // Vector3 の Normalize() 等は static と instance 両方定義する
            // (slimDX と同じような実装)
            for (int i = 0; i < Methods.Count; i++)
            {
                var method = Methods[i];
                if (method.OwnerClass.IsStruct && 
                    method.Attribute == MethodAttribute.StructOutputStatic &&
                    method.Params.Count == 2 &&
                    method.Params[0].Type == method.OwnerClass &&
                    method.Params[1].IsOutStructType)
                {
                    var newMethod = method.Copy();
                    newMethod.Attribute = MethodAttribute.StructInstance;
                    Methods.Insert(i + 1, newMethod);
                    i++;

                    int index = method.OwnerClass.Methods.IndexOf(method);
                    method.OwnerClass.Methods.Insert(index + 1, newMethod);
                }
            }
            
            //-------------------------------------------------
            // プロパティ作成
            AllProperties = new List<CLProperty>();
            foreach (var method in Methods)
            {
                // 関数チェックでプロパティとしてマークされている
                if (method.PropertyType != PropertyNameType.NotProperty)
                {
                    // 既存検索
                    var prop = AllProperties.Find((item) => item.OwnerClass == method.OwnerClass && item.KeyName == method.PropertyName);
                    if (prop == null)
                    {
                        prop = new CLProperty(this, method.OwnerClass);
                        AllProperties.Add(prop);
                        method.OwnerClass.Properties.Add(prop);
                    }

                    // getter setter 割り振り
                    switch (method.PropertyType)
                    {
                        case PropertyNameType.Set:
                            prop.Setter = method;
                            break;
                        case PropertyNameType.Is:
                        case PropertyNameType.Get:
                            prop.Getter = method;
                            break;
                    }

                    // プロパティとメソッドを関連付ける
                    method.OwnerProperty = prop;
                }
            }

            Console.WriteLine();
#if false
            //-------------------------------------------------
            // 構造体定義をモジュールと関連付ける
            //foreach (var node in typedefHeaderNodes)
            //{
            //    var structDecl = node as StructDecl;
            //    if (structDecl != null)
            //    {
            //        GetModuleByName(structDecl.CommonName).AppendStructDecl(structDecl);
            //    }
            //}
            //-------------------------------------------------
            // 全関数エンティティ作成
            Methods = new List<CLMethod>();
            FuncDocNode lastFuncDoc = null;
            CLMethod lastMethodEntity = null;
            foreach (var node in funcSecNodes)
            {
                // 関数コメントのノード。覚えておく
                if (node is FuncDocNode)
                    lastFuncDoc = (FuncDocNode)node;

                // 関数宣言のノード。覚えて置いたコメントと関連付けてエンティティ化
                if (node is FuncDeclNode)
                {
                    // モジュールへの追加もこの中で行われる
                    var m = new CLMethod(this, (FuncDocNode)lastFuncDoc, (FuncDeclNode)node);
                    Methods.Add(m);
                    lastFuncDoc = null;
                    lastMethodEntity = m;
                }

                // オプションのノード。最後に作った関数エンティティにくっつける
                if (node is FuncOptionNode)
                {
                    lastMethodEntity.Option = (FuncOptionNode)node;
                    lastMethodEntity = null;
                }
            }

            // 後処理フェーズ1
            foreach (var m in Methods)
            {
                m.PostAnalyze1();
            }
            // 後処理フェーズ2
            foreach (var module in Modules)
            {
                module.PostAnalyze2();
            }
#endif
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

        /// <summary>
        /// 名前からクラス型または構造体型を取得する
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public CLClassType GetClassTypeByName(string name)
        {
            if (name == "Object")
                return ReferenceObjectClass;

            var classType = AllClassTypes.Find((item) => item.Name == name);
            if (classType != null)
                return classType;
            //var structType = AllStructTypes.Find((item) => item.Name == name);
            //if (structType != null)
            //    return structType;
            throw new InvalidOperationException();
        }

        /// <summary>
        /// C言語名からクラス型または構造体型を取得する
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public CLClassType GetClassTypeByCName(string cName)
        {
            var classType = AllClassTypes.Find((item) => item.CName == cName);
            if (classType != null)
                return classType;
            //var structType = AllStructTypes.Find((item) => item.CName == cName);
            //if (structType != null)
            //    return structType;
            throw new InvalidOperationException();
        }

        /// <summary>
        /// C言語の定義名から CLType を取得する
        /// </summary>
        /// <returns></returns>
        public bool TryGetTypeByCTypeFullName(string cFullTypeName, out CLType type, out ParamAttribute paramAttr)
        {
            type = null;
            paramAttr = ParamAttribute.None;

            // プリミティブ型テーブルを検索
            TypeInfo info;
            if (_typeInfoTable.TryGetValue(cFullTypeName, out info))
            {
                type = info.Type;
                paramAttr = info.ParamAttribute;
                return true;
            }

            // enum 型検索
            foreach (var enumType in AllEnumTypes)
            {
                if (cFullTypeName.IndexOf(enumType.CName) >= 0)
                {
                    type = enumType;
                    if (cFullTypeName.Contains("*"))    // * の有無で out/in 決定
                        paramAttr = ParamAttribute.Out;
                    else
                        paramAttr = ParamAttribute.In;
                    return true;
                }
            }

            // struct 型であるか
            var name = cFullTypeName.Replace("*", "").Replace("const", "").Trim();
            foreach (var classType in AllClassTypes)
            {
                if (classType.IsStruct &&
                    //cFullTypeName.IndexOf("ln" + classType.Name) >= 0)
                    name == "ln" + classType.Name)
                {
                    type = classType;
                    if (cFullTypeName.Contains("*"))    // * の有無で out/in 決定
                        paramAttr = ParamAttribute.Out;
                    else
                        paramAttr = ParamAttribute.In;
                    return true;
                }
            }

            return false;
        }

#if false

        /// <summary>
        /// 型名からモジュールを取得 (const や * も考慮。主に構造体型の取得で使用する)
        /// </summary>
        /// <param name="cppType">ln付</param>
        /// <returns></returns>
        public bool TryGetStructDeclByCFullTypeName(string cFullTypeName, out CLModule module)
        {
            string name = cFullTypeName.Replace("const", "");
            name = name.Replace("*", "");
            name = name.Replace(" ", "");
            name.Trim();

            // ln を取り除く
            if (string.Compare(name, 0, "ln", 0, 2) == 0)
                name = name.Substring(2);

            module = Modules.Find((item) => item.Name == name);

            return module != null;
        }
#endif
    }
}
