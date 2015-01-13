using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;
using Sprache;
using System.Globalization;
using System.Threading;
using Microsoft.CSharp;
using System.CodeDom.Compiler;
using System.Reflection;

namespace BinderMaker
{
    class Node
    {
        /// <summary>
        /// 解析後処理
        /// </summary>
        public virtual void PostAnalyze()
        {

        }
    }

    /// <summary>
    /// enum 定義
    /// </summary>
    class EnumDecl : Node
    {
        public string Name;
        public IEnumerable<EnumMemberDecl> Members;

        /// <summary>
        /// ln プレフィックスを除いた名前
        /// </summary>
        public string CommonName
        {
            get { return Name.Substring(C_APIAnalyzer.StructOrEnumPrefix.Length); }
        }

        /// <summary>
        /// C# の [Flags] 用
        /// </summary>
        public bool IsFlags
        {
            get
            {
                foreach (var memnber in Members)
                {
                    if (memnber.Value.Length >= 2 && 
                        memnber.Value[0] == '0' &&
                        memnber.Value[1] == 'x')
                    {
                        return true;
                    }
                }
                return false;
            }
        }

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            // 先頭の _ を取り除く
            Name = Name.TrimStart('_');

            // 定数定義を持たない EnumMemberDecl に値を割り振っていく
            int lastValue = 0;
            foreach (var decl in Members)
            {
                decl.PostAnalyze();

                // 定数定義が無い場合は直前の値
                if (string.IsNullOrEmpty(decl.Value))
                {
                    decl.Value = lastValue.ToString();
                }
                // 定数定義がある場合は lastValue にする
                else
                {
                    if (!int.TryParse(decl.Value, out lastValue))
                    {
                        // 16進数かもしれない
                        lastValue = (int)Convert.ToInt32(decl.Value, 16);   // Convert.ToInt32 は頭に 0x が付いていても変換できる
                    }
                }
                lastValue++;
            }

        }

        // コピーを作成するメソッド
        public EnumDecl Clone()
        {
            return (EnumDecl)MemberwiseClone();
        }
    }

    /// <summary>
    /// enum メンバ定義
    /// </summary>
    class EnumMemberDecl : Node
    {
        public string Name;
        public string Value;
        public string Comment;

        /// <summary>
        /// enum ターミネータであるか
        /// </summary>
        public bool IsTerminator
        {
            get { return Name.LastIndexOf("MAX") == Name.Length - 3; }
        }

        /// <summary>
        /// プレフィックスを除き、各単語の先頭を大文字にして _ を結合した名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → ScalingWithLetterBox
        /// </summary>
        public string CapitalizedName
        {
            get
            {
                if (Name == "LN_OK") return "OK";   // これだけ特殊扱い

                // 先頭大文字化の準備
                CultureInfo cultureInfo = Thread.CurrentThread.CurrentCulture;
                TextInfo textInfo = cultureInfo.TextInfo;

                string[] idents = Name.Split('_');
                string name = "";
                for (int i = 2; i < idents.Count(); i++)
                {
                    if (NonCapitalizeWords.Contains(idents[i]))
                        name += idents[i];
                    else
                        name += textInfo.ToTitleCase(textInfo.ToLower(idents[i]));//char.ToUpper(idents[i][0]) + idents[i].Substring(1);    // 文字列の先頭を大文字に
                }
                return name;
            }
        }

        /// <summary>
        /// プレフィックスを除いた名前
        /// 例: LN_BACKBUFFERRESIZEMODE_SCALING_WITH_LETTER_BOX → SCALING_WITH_LETTER_BOX
        /// </summary>
        public string CommonSnakeName
        {
            get
            {
                if (Name == "LN_OK") return "OK";   // これだけ特殊扱い

                // 先頭大文字化の準備
                CultureInfo cultureInfo = Thread.CurrentThread.CurrentCulture;
                TextInfo textInfo = cultureInfo.TextInfo;

                string[] idents = Name.Split('_');
                string name = "";
                for (int i = 2; i < idents.Count(); i++)
                {
                    if (!string.IsNullOrEmpty(name))
                        name += "_";
                    name += idents[i];
                }
                return name;
            }
        }

        // CapitalizedName で小文字化しない enum 定数名
        private static readonly List<string> NonCapitalizeWords = new List<string>()
        {
            "XYZ", "XZY", "YXZ", "YZX", "ZXY", "ZYX",
            "BGM", "BGS", "ME",
            "A8R8G8B8", "X8R8G8B8", "A16B16G16R16F", "A32B32G32R32F", "D24S8",
        };

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            Comment = Comment.Trim();  // 前後の空白を取り除く
        }
    }

    /// <summary>
    /// struct 定義
    /// </summary>
    class StructDecl : Node
    {
        public string Name;
        public IEnumerable<StructMemberDecl> Members;

        /// <summary>
        /// ln プレフィックスを除いた名前
        /// </summary>
        public string CommonName
        {
            get { return Name.Substring(C_APIAnalyzer.StructOrEnumPrefix.Length); }
        }

        public StructDecl(string name, IEnumerable<IEnumerable<StructMemberDecl>> decls)
        {
            Name = name;
            var list = new List<StructMemberDecl>();
            foreach (var d in decls)
            {
                list.AddRange(d);
            }
            Members = list;
        }


        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            // 先頭の _ を取り除く
            Name = Name.TrimStart('_');

            foreach (var member in Members)
            {
                member.Comment = member.Comment.Trim();
            }
        }
    }

    class StructMemberDecl : Node
    {
        public string Type;
        public string Name;
        public string Comment;
    }

    class ModuleDocNode : Node
    {
        public ModuleGroupInfo GroupInfo;
        public string Summary;
        public string Detail;
        public string Extends;

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            // 行頭 (改行の次) にある行コメント開始文字(スペースと //)を取り除き、前後の空白を削除
            Summary = Regex.Replace(Summary, @"[\r|\n]+\/\/", "").Trim();

            Detail = Regex.Replace(Detail, @"^\s*\/\/\s+", "\n");
            Detail = Regex.Replace(Detail, @"[\r|\n]+\/\/\s+", "\n");
            Detail = Detail.Trim();

            Extends = Regex.Replace(Extends, @"[\r|\n]+\/\/", "").Trim();

            GroupInfo.PostAnalyze();
        }
    }

    class ModuleGroupInfo
    {
        public string GroupName;
        public string Title;

        /// <summary>
        /// 解析後処理
        /// </summary>
        public void PostAnalyze()
        {
            // 行頭 (改行の次) にある行コメント開始文字(スペースと //)を取り除き、前後の空白を削除
            Title = Regex.Replace(Title, @"[\r|\n]+\/\/", "").Trim();
        }
    }

    /// <summary>
    /// 関数ヘッダコメント
    /// </summary>
    class FuncDocNode : Node
    {
        public string Summary;  // 概要
        public IEnumerable<FuncParamDocNode> Params;
        public string Return;   // 戻り値型の説明
        public string Detail;   // 詳細
        public string Override;

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            // 行頭 (改行の次) にある行コメント開始文字(スペースと //)を取り除き、前後の空白を削除
            Summary = Regex.Replace(Summary, @"[\r|\n]+\s+\/\/", "");
            Summary = Summary.Trim();
            Return = Regex.Replace(Return, @"[\r|\n]+\s+\/\/", "");
            Return = Return.Trim();
            Detail = Regex.Replace(Detail, @"^\s*\/\/\s+", "\n");
            Detail = Regex.Replace(Detail, @"[\r|\n]+\s+\/\/\s+", "\n");
            Detail = Detail.Trim();

            foreach (var p in Params)
                p.PostAnalyze();
        }

        /// <summary>
        /// FuncParamDocNode 検索
        /// </summary>
        /// <param name="name"></param>
        /// <returns></returns>
        public FuncParamDocNode FindParamDoc(string name)
        {
            foreach (var param in Params)
            {
                if (param.Name == name)
                    return param;
            }
            throw new InvalidOperationException();
        }
    }

    /// <summary>
    /// 関数ヘッダコメント
    /// </summary>
    class FuncParamDocNode : Node
    {
        public string IO;       // 
        public string Name;
        public string Summary;  // 概要

        public ParamIO IOType { get; private set; }

        /// <summary> 
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            Name = Name.Trim();

            // 行頭 (改行の次) にある行コメント開始文字(スペースと //)を取り除き、前後の空白を削除
            Summary = Regex.Replace(Summary, @"[\r|\n]+\s+\/\/", "");
            Summary = Summary.Trim();

            IOType = 0;
            IOType |= (IO.IndexOf("in") >= 0) ? ParamIO.In : 0;
            IOType |= (IO.IndexOf("out") >= 0) ? ParamIO.Out : 0;
            if (IOType == 0)
                throw new InvalidOperationException();  // IO が空等

        }
    }


    enum PropertyNameType
    {
        NotProperty,
        Get,
        Set,
        Is,
    }

    enum FuncType
    {
        Instance,               // インスタンスメソッド (第1引数が lnHandle)
        Static,                 // static メソッド (デフォルト)
        Constructor,            // コンストラクタ
        DefaultInstanceGetter,  // デフォルトで作成される lnHandle の getter (LCamera_GetDefault3DCamera 等)
        StructOutputStatic,     // 第1引数に演算結果を格納する構造体へのポンタを受け取る static 関数。out 属性のみ。第1引数を return で返すように整形する
        StructInstance,         // 第1引数が in,out かつ モジュール名と一致する型である関数
        StructMemberSetter,     // Set
    }

    /// <summary>
    /// 関数定義
    /// </summary>
    class FuncDeclNode : Node
    {
        public string ReturnType;
        public string Name;
        public IEnumerable<FuncParamDeclNode> Params;
        public string ModuleName;       // _ の前。LSceneNode 等
        public string CommonName;       // _ の後。SetMatrix 等
        public string CommonModuleName; // _ の前。プリフィックス無し。SceneNode 等
        public PropertyNameType PropertyType = PropertyNameType.NotProperty;
        public string PropertyName; // (Is～ の場合は Is 無しの部分)
        public FuncType FuncType = FuncType.Static;
        public List<FuncParamDeclNode> InstanceMethodParams;    // インスタンスメソッドの時の引数一覧

        public Module ReturnTypeHandleWrapperModule;  // ReturnType が lnHandle の時の対応するモジュール (基本的に null)

        /// <summary>
        /// FuncType を考慮し、必要な引数リストを返す
        /// </summary>
        /// <returns></returns>
        public IEnumerable<FuncParamDeclNode> GetParams()
        {
            if (FuncType == BinderMaker.FuncType.Instance ||
                FuncType == BinderMaker.FuncType.StructOutputStatic ||
                FuncType == BinderMaker.FuncType.StructInstance)
                return InstanceMethodParams;
            if (FuncType == BinderMaker.FuncType.Static ||
                FuncType == BinderMaker.FuncType.Constructor)
                return Params;
            throw new InvalidOperationException();
        }

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            foreach (var param in Params)
            {
                param.PostAnalyze();
            }

            string[] pair = Name.Split('_');
            ModuleName = pair[0];
            CommonName = pair[1];
            CommonModuleName = ModuleName.Substring(C_APIAnalyzer.FuncPrefix.Length);

            

            // インスタンスメソッドの時の引数一覧
            //if (FuncType == BinderMaker.FuncType.Instance ||
            //    FuncType == BinderMaker.FuncType.StructOutput)
            {
                InstanceMethodParams = new List<FuncParamDeclNode>();
                int i = 0;
                foreach (var param in Params)
                {
                    if (i != 0) // 最初(lnHandle)以外をリストアップ
                        InstanceMethodParams.Add(param);
                     i++;
                }
            }
        }

        public void PostAnalyze2()
        {

            // 先頭が Create ならコンストラクタ関数
            if (string.Compare(CommonName, 0, "Create", 0, 6) == 0)
            {
                FuncType = BinderMaker.FuncType.Constructor;
            }
            // 以下の条件で IsProperty = true
            // ・第1引数が lnHandle
            // ・戻り値が void ではない or 引数が2つ
            else if (Params.Count() > 0)
            {
                var firstParam = Params.First();
                if (firstParam.Type == "lnHandle" ||
                    (firstParam.BasicType == CommonModuleName && firstParam.IOType != 0))
                {
                    // 第1引数が lnHandle ならインスタンスメソッド。それ以外の型なら StructInstance
                    if (firstParam.Type == "lnHandle")
                        FuncType = BinderMaker.FuncType.Instance;
                    else
                        FuncType = BinderMaker.FuncType.StructInstance;


                    if ((Params.Count() == 1 && ReturnType != "void") ||
                        (Params.Count() == 2 && ReturnType == "void"))
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
        }
    }

    [Flags]
    enum ParamIO
    {
        None = 0x00,
        In = 0x01,     // 入力用引数
        Out = 0x02,    // 出力用引数
    }

    /// <summary>
    /// 関数仮引数定義
    /// </summary>
    class FuncParamDeclNode : Node
    {
        public string Type;
        public string Name;
        public string DefaultValue;

        public ParamIO IOType;
        public string BasicType;    // const や *、lnプリフィックスを除いた型名

        public Module HandleWrapperModule;  // Type が lnHandle の時の対応するモジュール (C_APIAnalyzer から設定される)
        public bool IsHandleWrapperType { get { return HandleWrapperModule != null; } }

        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            //IOType = ParamIO.In;

            //if (Type.Contains('*'))     // ポインタ型なら out かもしれない
            //    IOType = ParamIO.Out;

            //if (Type.Contains("const")) // ポインタだろうがなんだろうが const があれば in
            //    IOType = ParamIO.In;

            // const や *、lnプリフィックスを除いた型名
            BasicType = Type.Replace("const", "").Replace("*", "").Trim();
            if (BasicType.IndexOf(C_APIAnalyzer.StructOrEnumPrefix) == 0)
                BasicType = BasicType.Substring(C_APIAnalyzer.StructOrEnumPrefix.Length);
        }
    }

    /// <summary>
    /// 関数定義オプション
    /// </summary>
    class FuncOptionNode : Node
    {
        public string Code;

        public string ReturnHandleType = null;  // 戻り値型が lnHandle の場合に使用されるモジュール名 (L付き)
        public bool AutoPropertyGenerate = true;    // プロパティの対象にしない
        public bool ForceStaticMethod = false;       // static メソッド化の強制
        public Dictionary<string, string> ParamTypes = new Dictionary<string, string>();    // 仮引数型の強制 (ジェネリックの場合は ObjectList<Layer> のように指定)
        public bool Cpp_Enabled = true;
        public bool Cpp_MethodOverride = false;
        public bool CS_Enabled = true; 
        public bool CS_MethodOverride = false;
        public bool Ruby_Enabled = true;
        public bool Ruby_MethodOverride = false;
        public bool HSP_Enabled = true;
        public bool HSP_Override = false;

        // 初期値は↑↓両方で定義しておく (↑はデフォルト値として Eval せずに使用される)
        const string EvalerCode = @"
using System.Collections.Generic;
public class CSCodeEvaler {
    
    public string ReturnHandleType = null;
    public bool AutoPropertyGenerate = true;
    public bool ForceStaticMethod = false;
    public Dictionary<string, string> ParamTypes = new Dictionary<string, string>();
    public bool Cpp_Enabled = true;
    public bool Cpp_MethodOverride = false; 
    public bool CS_Enabled = true;
    public bool CS_MethodOverride = false; 
    public bool Ruby_Enabled = true;
    public bool Ruby_MethodOverride = false;
    public bool HSP_Enabled = true;
    public bool HSP_Override = false;

    public void EvalCode() {
        __CODE__
    }
}
";
        /// <summary>
        /// 解析後処理
        /// </summary>
        public override void PostAnalyze()
        {
            CodeDomProvider provider = CodeDomProvider.CreateProvider("CSharp");
            ICodeCompiler icc = provider.CreateCompiler();
            CompilerParameters cp = new CompilerParameters();

            cp.ReferencedAssemblies.Add("system.dll");
            cp.CompilerOptions = "/t:library";
            cp.GenerateInMemory = true;

            string code = EvalerCode.Replace("__CODE__", Code);

            CompilerResults cr = icc.CompileAssemblyFromSource(cp, code);
            if (cr.Errors.Count > 0)
            {
                throw new InvalidOperationException("ERROR: " + cr.Errors[0].ErrorText);
            }

            // インスタンス作成
            System.Reflection.Assembly a = cr.CompiledAssembly;
            object o = a.CreateInstance("CSCodeEvaler");

            // EvalCode() 呼び出し
            Type t = o.GetType();
            MethodInfo mi = t.GetMethod("EvalCode");
            mi.Invoke(o, null);

            ReturnHandleType = (string)t.GetField("ReturnHandleType").GetValue(o);
            AutoPropertyGenerate = (bool)t.GetField("AutoPropertyGenerate").GetValue(o);
            ForceStaticMethod = (bool)t.GetField("ForceStaticMethod").GetValue(o);
            ParamTypes = (Dictionary<string, string>)t.GetField("ParamTypes").GetValue(o);
            Cpp_Enabled = (bool)t.GetField("Cpp_Enabled").GetValue(o);
            Cpp_MethodOverride = (bool)t.GetField("Cpp_MethodOverride").GetValue(o);
            CS_Enabled = (bool)t.GetField("CS_Enabled").GetValue(o);
            CS_MethodOverride = (bool)t.GetField("CS_MethodOverride").GetValue(o);
            Ruby_Enabled = (bool)t.GetField("Ruby_Enabled").GetValue(o);
            Ruby_MethodOverride = (bool)t.GetField("Ruby_MethodOverride").GetValue(o);
            HSP_Enabled = (bool)t.GetField("HSP_Enabled").GetValue(o);
            HSP_Override = (bool)t.GetField("HSP_Override").GetValue(o);
        }
    }
}
