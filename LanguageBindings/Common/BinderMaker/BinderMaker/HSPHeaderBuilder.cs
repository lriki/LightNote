using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class HSPHeaderBuilder
    {
        const string ASFileHeader = @"
//=============================================================================
// LNote
//=============================================================================

#include ""user32.as""

#ifndef __lnote__
#define __lnote__

#uselib ""LNote.dll""

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
#define LN_MUSICTIMEBASE 768
#define LN_EXCEPTION_WIN32_MESSAGE 0xA000

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
";

        const string ASFileFooter = @"
#func LException_SetEnableWrappedException ""LException_SetEnableWrappedException"" int
#func LHSPInternal_GetIntPtrStringLength ""LHSPInternal_GetIntPtrStringLength"" int, var
#func LHSPInternal_GetIntPtrString ""LHSPInternal_GetIntPtrString"" int, var

//------------------------------------------------------------------------------
// overrides
//------------------------------------------------------------------------------

//--------------------------------------------------------------------------
// LManager_Initialize
#deffunc _LManager_Initialize
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_Initialize@
	return stat
	
//--------------------------------------------------------------------------
// LManager_Update
#deffunc _LManager_Update
	__LManager_Update@
	await 0
	return stat
	
//--------------------------------------------------------------------------
// LFile_ExistDirectoryOrEXE(stat: 0=なし 1=ディレクトリに存在(優先) 2=EXEに存在)
#deffunc LFile_ExistDirectoryOrEXE str p1
	b = LN_FALSE@
	_LFile_Exists@ p1, b
	if b != LN_FALSE@ : return 1
	exist p1
	if strsize != -1 : return 2
	return 0

//--------------------------------------------------------------------------
// LManager_InitializeAudio
#deffunc _LManager_InitializeAudio
	_LConfig_SetUserWindowHandle@ hwnd
	__LManager_InitializeAudio@
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGM
#deffunc _LAudio_PlayBGM str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGMMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGM@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGM""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayBGS
#deffunc _LAudio_PlayBGS str p1, int p2, int p3, int p4
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayBGSMem@ buf, strsize, p2, p3, p4
	} else : if stat == 1 {
		__LAudio_PlayBGS@ p1, p2, p3, p4
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayBGS""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlayME
#deffunc _LAudio_PlayME str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlayMEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlayME@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlayME""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE
#deffunc _LAudio_PlaySE str p1, int p2, int p3
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySEMem@ buf, strsize, p2, p3
	} else : if stat == 1 {
		__LAudio_PlaySE@ p1, p2, p3
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3D str p1, var p2, double p3, int p4, int p5
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMem@ buf, strsize, p2, p3, p4, p5
	} else : if stat == 1 {
		__LAudio_PlaySE3D@ p1, p2, p3, p4, p5
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LAudio_PlaySE3D
#deffunc _LAudio_PlaySE3DXYZ str p1, double p2, double p3, double p4, double p5, int p6, int p7
	LFile_ExistDirectoryOrEXE p1
	if stat == 2 {
		sdim buf, strsize
		bload p1, buf, strsize
		_LAudio_PlaySE3DMemXYZ@ buf, strsize, p2, p3, p4, p5, p6, p7
	} else : if stat == 1 {
		__LAudio_PlaySE3DXYZ@ p1, p2, p3, p4, p5, p6, p7
	} else {
		_LException_Raise@ LN_ERR_FILE_NOT_FOUND@, p1, __file__, __line__, ""_LAudio_PlaySE3D""
	}
	return

//--------------------------------------------------------------------------
// LMatrix_Identity
#deffunc _LMatrix_Identity var p1
	if (length(p1) < 16) : dim p1, 16
    __LMatrix_Identity@ p1
    return

//--------------------------------------------------------------------------
// LQuaternion_Identity
#deffunc _LQuaternion_Identity var p1
	if (length(p1) < 4) : dim p1, 4
    __LQuaternion_Identity@ p1
    return

//--------------------------------------------------------------------------
// LRandom_SetSeed
#deffunc _LRandom_SetSeed var p1, int p2
	if (length(p1) < 4) : dim p1, 4
    __LRandom_SetSeed@ p1, p2
    return

__INTERNAL_DEFILES__

#global

// 例外発生時にコールバックされるラベルの登録
#define LException_SetHandler(%1) oncmd gosub %1, LN_EXCEPTION_WIN32_MESSAGE

goto *lexception_default_label_dummy

// デフォルトの例外ジャンプ
*lexception_default_label
	LException_ProcDefault
	// HSP外部からの SendMessage() によるコールバック内では、
	// end ひとつだけだと終了できないみたい。
	// (ひとつめ:PostQuit → ふたつめ:Dispatch のように流れている？)
	end
	end
	return

*lexception_default_label_dummy

// デフォルト登録
//LException_SetHandler *lexception_default_label


#endif  // __lnote__
";


        public struct StructAccessorInfo
        {
            public string ModuleName;
        };

        private struct TypeInfo
        {
            public string HSPType;
            public string InternalSetter;
            public string InternalGetter;

            public TypeInfo(string hspType, string internalSetter, string internalGetter)
            {
                HSPType = hspType;
                InternalSetter = internalSetter;
                InternalGetter = internalGetter;
            }
        };

        public static Dictionary<string, StructAccessorInfo> _structAccessorInfo = new Dictionary<string, StructAccessorInfo>()
        {
            { "lnSize",         new StructAccessorInfo() { ModuleName = "LSize" } },
            { "lnRect",         new StructAccessorInfo() { ModuleName = "LRect" } },
            { "lnVector2",      new StructAccessorInfo() { ModuleName = "LVector2" } },
            { "lnVector3",      new StructAccessorInfo() { ModuleName = "LVector3" } },
            { "lnVector4",      new StructAccessorInfo() { ModuleName = "LVector4" } },
            { "lnMatrix",       new StructAccessorInfo() { ModuleName = "LMatrix" } },
            { "lnQuaternion",   new StructAccessorInfo() { ModuleName = "LQuaternion" } },
            { "lnRandom",       new StructAccessorInfo() { ModuleName = "LRandom" } },
            { "lnColor",        new StructAccessorInfo() { ModuleName = "LColor" } },
            { "lnColorF",       new StructAccessorInfo() { ModuleName = "LColorF" } },
            { "lnTone",         new StructAccessorInfo() { ModuleName = "LTone" } },
        };

        private Dictionary<string, TypeInfo> _typeInfoTable = new Dictionary<string, TypeInfo>()
        {
            // C++型             HSP 型
            { "lnU8",           new TypeInfo("int",     "_SetInt8Internal",     "_GetInt8Internal") },
            { "int",            new TypeInfo("int",     "_SetInt32Internal",    "_GetInt32Internal") },
            { "lnU32",          new TypeInfo("int",     "_SetInt32Internal",    "_GetInt32Internal") },
            { "float",          new TypeInfo("double",  "_SetFloatInternal",    "_GetFloatInternal") },
            { "double",         new TypeInfo("double",  "_SetFloatInternal",    "_GetFloatInternal") },
            { "bool",           new TypeInfo("double",  null,                   null) },
            { "void*",          new TypeInfo("var",     null,                   null) },
            { "float*",         new TypeInfo("var",     null,                   null) },
            
            { "lnBool*",        new TypeInfo("var",     null,                   null) },
            { "lnBool",         new TypeInfo("int",     null,                   null) },

            { "const lnChar**", new TypeInfo("var",     null,                   null) },
            { "const lnChar*",  new TypeInfo("str",     null,                   null) },
            { "lnChar*",        new TypeInfo("str",     null,                   null) },
            { "TCHAR*",         new TypeInfo("str",     null,                   null) },
            { "lnHandle",       new TypeInfo("int",     null,                   null) },
            { "lnHandle*",      new TypeInfo("var",     null,                   null) },
            { "LNResult",       new TypeInfo("int",     null,                   null) },

            { "lnVector3*",     new TypeInfo("var",     null,                   null) },
        };

        private C_APIAnalyzer _analyzer;
        private List<FuncEntity> _outputOverrideFuncs = new List<FuncEntity>();

        /// <summary>
        /// 作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void Build(C_APIAnalyzer analyzer)
        {
            _analyzer = analyzer;

            string indent = "\t";
            string output = ASFileHeader.TrimStart();   // 最初の改行を消す

            //---------------------------------------------------------
            // enum 定数
            {
                foreach (var decl in analyzer.AllEnumNodes)
                {
                    foreach (var member in decl.Members)
                        if (!member.IsTerminator)
                            output += "#const " + member.Name + " " + member.Value + "\n";
                    output += "\n";
                }
            }

            //---------------------------------------------------------
            // 関数定義
            {
                foreach (var func in analyzer.AllFuncEntites)
                {
                    if (!func.Option.HSP_Enabled) continue;
                    // struct の Get Set は HSP 用に独自定義するのでここで作る必要はない
                    if (CheckStructGetSetFunc(func)) continue;

                    int i;
                    string outputDecl = "";
                    string prefix = "_"; 
                    if (func.Option.HSP_Override)
                        prefix += "_";

                    // 文字列出力型の仮引数があれば override
#if true
                    //if (func.AllParams.Find((item) => item.IsStringOutput) != null)
                    if (func.AllParams.Find((item) => item.IsStringOutput) != null ||
                        func.AllParams.Find((item) => CheckFloatOutput(item)) != null)
#else
                    if (func.AllParams.Find((item) => item.IsStringOutput) != null ||
                        func.AllParams.Find((item) => CheckFloatOutput(item)) != null ||
                        func.AllParams.Find((item) => CheckFloatInput(item)) != null)
#endif
                    {
                        prefix += "_";
                        _outputOverrideFuncs.Add(func);
                    }
                    

                    // #func
                    if (func.AllParams.Find((item) => CheckFloatOutput(item)) != null ||
                        func.AllParams.Find((item) => CheckFloatInput(item)) != null)
                        outputDecl += "#func " + prefix + func.CPPName + " \"" + func.CPPName + "D\"";
                    else
                        outputDecl += "#func " + prefix + func.CPPName + " \"" + func.CPPName + "\"";


                    // #func の仮引数
                    i = 0;
                    foreach (var param in func.AllParams)
                    {
                        outputDecl += (i == 0) ? " " : ", ";
#if true
                        outputDecl += ConvertToHSPType(param);
#else
                        // float 変換してから渡すため int http://hspwiki.hinekure.net/OpenGL%E3%81%AB%E3%82%88%E3%82%8B%E8%A1%A8%E7%A4%BA.html
                        string type = ConvertToHSPType(param);
                        outputDecl += (param.CPPFullTypeName == "float") ? "int" : type;
#endif

                        i++;
                    }
                    outputDecl += "\n";

                    // #define
                    outputDecl += "#define " + func.CPPName;

                    // #define の仮引数
                    if (func.AllParams.Count() > 0)
                    {
                        outputDecl += "(";
                        i = 0;
                        foreach (var param in func.AllParams)
                        {
                            outputDecl += (i == 0) ? "" : ", ";
                            outputDecl += "%" + (i + 1).ToString();
                            if (!string.IsNullOrEmpty(param.DefaultValue))
                                outputDecl += "=" + param.DefaultValue;
                            i++;
                        }
                        outputDecl += ")";
                    }

                    // #define の置換要素 (呼び出し関数名)
                    if (prefix == "___")    // 多重オーバーライド
                        outputDecl += " __" + func.CPPName;
                    else
                        outputDecl += " _" + func.CPPName;

                    // #define の置換要素 (実引数)
                    if (func.AllParams.Count() > 0)
                    {
                        outputDecl += " ";
                        i = 0;
                        foreach (var param in func.AllParams)
                        {
                            outputDecl += (i == 0) ? "" : ", ";
                            outputDecl += "%" + (i + 1).ToString();
                            i++;
                        }
                    }

                    output += outputDecl + "\n";
                }

            }

            //---------------------------------------------------------
            // HSP 定義関数
            {
                output += "\n\n";
                output += "//------------------------------------------------------------------------------\n";
                output += "// internal\n";
                output += "//------------------------------------------------------------------------------\n";
                output += "#module \"LNote\"\n";
                output += @"
// p1 = U32 配列
// p2 = p1 のインデックス
// p3 = 書き込む値
#deffunc _SetInt8Internal var p1, int p2, int p3
	poke p1, p2, p3
    return

// p1 = U32 配列
// p2 = p1 のインデックス
#defcfunc _GetInt8Internal var p1, int p2
	return peek(p1, p2)

// p1 = U32 配列
// p2 = p1 のインデックス
// p3 = 書き込む値
#deffunc _SetInt32Internal var p1, int p2, int p3
	lpoke p1, 4 * p2, p3
    return

// p1 = U32 配列
// p2 = p1 のインデックス
#defcfunc _GetInt32Internal var p1, int p2
	return lpeek(p1, 4 * p2)

// p1 = U32 配列
// p2 = p1 のインデックス
// p3 = 書き込む値
#deffunc _SetFloatInternal var p1, int p2, double p3
	temp = p3
	lpoke p1, 4 * p2, ( lpeek(temp)>>29&7|(p3<0)<<31|lpeek(temp,4)-(p3!0)*0x38000000<<3 )
    return

// p1 = U32 配列
// p2 = p1 のインデックス
#defcfunc _GetFloatInternal var p1, int p2
	v = lpeek(p1, 4 * p2)
	t = 0.0
	lpoke t, 4, (v & 0x80000000) | (((v & 0x7fffffff) >> 3) + ((v & 0x7fffffff) ! 0) * 0x38000000)
	lpoke t, 0, v << 29
	return t
";
                // TODO: 構造体の各メンバはすべて同じサイズでなければならない。そのチェック

                output += "\n";
                foreach (var decl in analyzer.AllStructNodes)
                {
                    StructAccessorInfo info = _structAccessorInfo[decl.Name];

                    //-----------------------------------------
                    // Setter 定義
                    output += "#deffunc " + info.ModuleName + "_Set ";  // 関数名
                    output += "var p1, ";                               // 第1引数 (変数参照)

                    // struct メンバを引数化
                    int i = 2;
                    foreach (var memnber in decl.Members)
                    {
                        if (i != 2)
                            output += ", ";  // 最初以外なら , 区切り
                        output += _typeInfoTable[memnber.Type].HSPType + " " + "p" + i.ToString();
                        i++;
                    }
                    output += "\n";

                    // dim (領域確保)
                    output += indent + "if (length(p1) < " + decl.Members.Count().ToString() + ") : ";
                    output += "dim p1, " + decl.Members.Count().ToString();
                    output += "\n";

                    // set
                    i = 0;
                    foreach (var memnber in decl.Members)
                    {
                        var typeInfo = _typeInfoTable[memnber.Type];
                        output += indent + typeInfo.InternalSetter + " p1, " + i.ToString() + ", p" + (i + 2).ToString();
                        output += "\n";
                        i++;
                    }

                    output += indent + "return\n\n";

                    //-----------------------------------------
                    // Getter 定義
                    output += "#deffunc " + info.ModuleName + "_Get ";  // 関数名
                    output += "var p1, ";                               // 第1引数 (変数参照)

                    // struct メンバを引数化
                    i = 2;
                    foreach (var memnber in decl.Members)
                    {
                        if (i != 2)
                            output += ", ";  // 最初以外なら , 区切り
                        output += "var p" + i.ToString();
                        i++;
                    }
                    output += "\n";

                    // get
                    i = 0;
                    foreach (var memnber in decl.Members)
                    {
                        var typeInfo = _typeInfoTable[memnber.Type];
                        output += indent + "p" + (i + 2).ToString() + " = ";    // 代入する変数
                        output += typeInfo.InternalGetter + "(p1, " + i.ToString() + ")";
                        output += "\n";
                        i++;
                    }

                    output += indent + "return\n\n";
                }

                //output += "#global\n";
            }

            //---------------------------------------------------------
            // const lnChar** や float 型引数を持つ命令
            string internalDefiens = "";
            {
                foreach (var func in _outputOverrideFuncs)
                {
                    // 命令名
                    string defText = "#deffunc _" + func.CPPName + " ";

                    // 仮引数リスト
                    string paramsText = "";
                    foreach (var param in func.AllParams)
                    {
                        if (!string.IsNullOrEmpty(paramsText)) paramsText += ", ";
                        paramsText += string.Format(
                            "{0} _{1}",
                            ConvertToHSPType(param),
                            param.ParamName);
                    }
                    defText += paramsText + "\n";

                    // 呼び出し
                    string callText = indent + "__" + func.CPPName + "@ ";

                    // 呼び出し - 実引数リスト
                    string argsText = "";
                    string initArgsText = "";
                    string postText = "";
                    for (int i = 0; i < func.AllParams.Count; i++)
                    {
                        if (!string.IsNullOrEmpty(argsText)) argsText += ", ";
                        var param = func.AllParams[i];

                        // const lnChar**
                        if (param.IsStringOutput)
                        {
                            argsText += "_sp" + i.ToString();
                            initArgsText += indent + "_sp" + i.ToString() + " = 0\n";
                        }
#if true
                        // float*
                        else if (CheckFloatOutput(param))
                        {
                            argsText += "_" + param.ParamName + " ";
                            initArgsText += indent + "_" + param.ParamName + " = 0.0\n";
                        }
#else
                        // float*
                        else if (CheckFloatOutput(param))
                        {
                            string valName = "_fp" + i.ToString();
                            argsText += valName;
                            initArgsText += indent + string.Format("if (length({0}) < 1) : dim {0}, 1\n", valName);
                            postText += indent + string.Format("_{0} = _GetFloatInternal({1}, 0)\n", param.ParamName, valName);// param.ParamName + " = " + valName  + "\n";
                        }
                        // float
                        else if (CheckFloatInput(param))
                        {
                            string valName = "_fp" + i.ToString();
                            argsText += valName + ".0";
                            initArgsText += indent + string.Format("if (length({0}) < 1) : dim {0}, 1\n", valName);
                            initArgsText += indent + string.Format("_SetFloatInternal {0}, 0, _{1}\n", valName, param.ParamName);
                            //postText += indent + string.Format("{0} = _GetFloatInternal({1}, 0)\n", param.ParamName, valName);// param.ParamName + " = " + valName  + "\n";
                        }
#endif
                        else
                            argsText += "_" + param.ParamName + " ";
                    }
                    defText += initArgsText;
                    defText += callText;
                    defText += argsText +"\n";
                    defText += postText;

                    // 文字列取得
                    for (int i = 0; i < func.AllParams.Count; i++)
                    {
                        string procText = "";
                        if (func.AllParams[i].IsStringOutput)
                        {
                            string template = @"
    len = 0
    LHSPInternal_GetIntPtrStringLength {0}, len
    sdim {1}, len + 1
    LHSPInternal_GetIntPtrString {0}, {1}";
                            procText += string.Format(indent + template.Trim(), "_sp" + i.ToString(), func.AllParams[i].ParamName);
                        }
                        if (!string.IsNullOrEmpty(procText))    // 改行が増えすぎないように
                            defText += procText + "\n";
                    }


                    // return
                    defText += indent + "return\n\n";

                    internalDefiens += defText;
                }
            }

            output += "\n";
            output += ASFileFooter.TrimStart().Replace("__INTERNAL_DEFILES__", internalDefiens);


            

            System.IO.File.WriteAllText("../../../../../HSP/lnote.as", output, System.Text.Encoding.GetEncoding("shift_jis"));
        }

        /// <summary>
        /// struct 型の Get Set 関数かを調べる
        /// </summary>
        /// <param name="func"></param>
        /// <returns></returns>
        private bool CheckStructGetSetFunc(FuncEntity func)
        {
            if (func.ParentModule.ModuleType == ModuleEntityType.Struct)
            {
                if (func.CommonName == "Set" ||
                    func.CommonName == "Get")
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// float* 仮引数であるか
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        private bool CheckFloatOutput(FuncParamEntity param)
        {
            return param.CPPFullTypeName == "float*";
        }

        /// <summary>
        /// float 仮引数であるか
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        private bool CheckFloatInput(FuncParamEntity param)
        {
            return param.CPPFullTypeName == "float";
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private string ConvertToHSPType(FuncParamEntity param)
        {
            if (_typeInfoTable.ContainsKey(param.CPPFullTypeName))
                return _typeInfoTable[param.CPPFullTypeName].HSPType;

            // struct 型であるか
            foreach (var node in _analyzer.AllStructNodes)
            {
                if (param.CPPFullTypeName.IndexOf(node.Name) >= 0)
                {
                    return "var";
                }
            }

            // 上記以外のポインタ型はすべて out 扱い
            if (param.CPPFullTypeName.Contains("*"))
                return "var";

            return "int";    // 登録された方でなければenumとみなして int にする
        }
        
    }
}
