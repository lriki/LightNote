using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.RegularExpressions;

namespace BinderMaker
{
    class HSPHelpBuilder
    {
        #region Templates
        const string HeaderTemplate = @"
;============================================================
; LNote ヘルプファイル
;============================================================

%dll
LNote

%ver
_LIBRARY_VERSION_

%date
_BUILD_TIME_

%author
lriki

%note
lnote.as をインクルードしてください

%type
拡張命令

%url
http://nnmy.sakura.ne.jp/

";
        
        const string FuncTemplate = @"
;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
_NAME_
_BRIEF_
%group
_GROUP_
%prm
_PRM_LIST_
_PRM_DETAIL_
%inst
_INST_
%href
_HREF_
";

        #endregion

        private class TestCode
        {
            public string SrcCode;
            public string ExtractedCode;
            public bool IsSkipTest;         // ◆ が含まれている → テストやサンプル生成を行わないか、未確定
        }

        private class GlobalReplaceInfo
        {
            public string From;
            public string To;
        }

        private static List<GlobalReplaceInfo> _globalReplaceInfoTable = new List<GlobalReplaceInfo>()
        {
            new GlobalReplaceInfo() { From = "関数", To = "命令" },
        };

        private C_APIAnalyzer _analyzer;
        private Dictionary<string, TestCode> _sampleCodeMap = new Dictionary<string, TestCode>();

        /// <summary>
        /// API呼び出しテストファイルを解析し、サンプル用ソースを取り出す
        /// </summary>
        /// <param name="filePath"></param>
        public void AnalyzeUnitTestFile(string filePath)
        {
            string line;
            bool inTestCode = false;
            string testFuncName = "";
            string testCode = "";

            // テストコードの抽出
            using (var file = new System.IO.StreamReader(filePath, System.Text.Encoding.GetEncoding("shift_jis")))
            {
                while ((line = file.ReadLine()) != null)
                {
                    // 終了行
                    if (line.Contains("/*test_end*/"))
                    {
                        var c = new TestCode();
                        c.SrcCode = testCode;
                        c.ExtractedCode = ConvertSapmleCodeMacros(testCode);
                        c.IsSkipTest = testCode.Contains("◆");
                        _sampleCodeMap.Add(testFuncName, c);
                        inTestCode = false;
                    }
                    // 開始行
                    else if (line.Contains("*test_"))
                    {
                        if (inTestCode)
                            throw new InvalidOperationException();  // 対になる終了行が見つからなかった

                        testFuncName = line.Substring(6).Trim();
                        testCode = "";
                        inTestCode = true;
                    }
                    // テストコード内部であれば抽出し続ける
                    else if (inTestCode)
                    {
                        testCode += line.Trim() + "\n";
                    }
                }
            }
        }

        private class MacroInfo
        {
            public Regex Puttern;
            public string Format;
        }
        private static List<MacroInfo> _macroInfoTable = new List<MacroInfo>()
        {
            new MacroInfo() { Puttern = new Regex(@"PRINT_INT.*"), Format = "mes {0}" },
            new MacroInfo() { Puttern = new Regex(@"PRINT_DOUBLE.*"), Format = "mes {0}" },
            new MacroInfo() { Puttern = new Regex(@"PRINT_VEC2.*"), Format = "LVector2_Get {0} , x, y\nmes \"Vector2:\" + x + \", \" + y" },
        };

        /// <summary>
        /// 単体テストコード内のマクロをサンプルコード用に置換
        /// </summary>
        /// <param name="code"></param>
        /// <returns></returns>
        private string ConvertSapmleCodeMacros(string code)
        {
            foreach (var info in _macroInfoTable)
            {
                var m = info.Puttern.Match(code);
                if (m.Success)
                {
                    string[] values = m.Value.Split(' ');
                    code = info.Puttern.Replace(code, string.Format(info.Format, values[1]));
                }
            }
            return code;
        }

        /// <summary>
        /// 作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void Build(C_APIAnalyzer analyzer, string outputFilePath)
        {
            _analyzer = analyzer;

            //---------------------------------------------------------
            // ヘッダ
            string headerText = HeaderTemplate.TrimStart();
            headerText = headerText.Replace("_LIBRARY_VERSION_", analyzer.Version);
            headerText = headerText.Replace("_BUILD_TIME_", DateTime.Today.ToString("yyyy/MM/dd"));

            //---------------------------------------------------------
            // 関数
            string allFuncText = "";
            foreach (var func in analyzer.AllFuncEntites)
            {
                if (!func.Option.HSP_Enabled) continue; // この関数は出力しない

                // 基本
                string funcText = FuncTemplate.TrimStart();
                funcText = funcText.Replace("_NAME_", func.CPPName);
                funcText = funcText.Replace("_BRIEF_", func.Summary);
                funcText = funcText.Replace("_INST_", func.Detail);
                funcText = funcText.Replace("_HREF_", "");
                funcText = funcText.Replace("_GROUP_", func.ParentModule.ModuleDoc.Summary);

                // 引数リスト
                string paramsText = "";
                foreach (var param in func.AllParams)
                {
                    if (paramsText.Length != 0) paramsText += ", ";
                    paramsText += param.ParamName;
                }
                if (paramsText.Length == 0) paramsText += "　"; // 空なら全角スペースでごまかす
                funcText = funcText.Replace("_PRM_LIST_", paramsText);

                // インデント量の計算
                // [out] pp   : 説明
                // [in]  name : 説明
                // ^^^^^        … ioColumnWidth
                //       ^^^^   … nameColumnWidth
                int ioColumnWidth = 0;
                int nameColumnWidth = 0;
                foreach (var param in func.AllParams)
                {
                    // 名前部分はデフォルト引数も含んだ長さで考える
                    int nameLen = param.ParamName.Length;
                    if (!string.IsNullOrEmpty(param.DefaultValue))
                        nameLen += param.DefaultValue.Length + 2;   // +2 は () の分

                    ioColumnWidth = Math.Max(ioColumnWidth, param.IOString.Length);
                    nameColumnWidth = Math.Max(nameColumnWidth, nameLen);
                }
                ioColumnWidth += 2; // 前後の [ ] の分

                // 引数の1行説明
                string detailText = "";
                foreach (var param in func.AllParams)
                {
                    string name = param.ParamName;
                    if (!string.IsNullOrEmpty(param.DefaultValue))
                        name += "(" + param.DefaultValue + ")";

                    detailText += string.Format("{0,-" + ioColumnWidth + "}", "[" + param.IOString + "]");
                    detailText += string.Format(" {0,-" + nameColumnWidth + "}", name);
                    detailText += " : ";
                    detailText += param.Detail;
                    detailText += "\n";

                    // enum 型の場合は候補値も追加しておく
                    EnumDecl enumDecl;
                    if (analyzer.TryGetEnumDeclByName(param.CPPFullTypeName, out enumDecl))
                    {
                        string indent = string.Format(" {0,-" + (ioColumnWidth + 3 + nameColumnWidth + 3) + "}", " ");

                        foreach (var member in enumDecl.Members)
                        {
                            if (member.IsTerminator) continue;  // ターミネータは追加しない
                            detailText += indent;
                            detailText += member.Name + "\n";
                            detailText += indent + "    ";
                            detailText += member.Comment + "\n";
                        }
                    }
                }

                // 戻り値 (全て stat 扱い)
                EnumDecl enumDeclReturn;
                if (analyzer.TryGetEnumDeclByName(func.ReturnType.CPPFullTypeName, out enumDeclReturn) ||
                    !string.IsNullOrEmpty(func.ReturnType.Detail))
                {
                    detailText += "\n";
                    detailText += "stat : ";
                    detailText += func.ReturnType.Detail;
                    detailText += "\n";

                    if (enumDeclReturn != null&&
                        enumDeclReturn.Name != "lnResult")  // リザルトコードの場合は出力しない
                    {
                        foreach (var member in enumDeclReturn.Members)
                        {
                            if (member.IsTerminator) continue;  // ターミネータは追加しない
                            detailText += "       ";
                            detailText += member.Name + "\n";
                            detailText += "           ";
                            detailText += member.Comment + "\n";
                        }
                    }
                }
                
                funcText = funcText.Replace("_PRM_DETAIL_", detailText);

                // サンプルコード
                TestCode sampleCode;
                if (_sampleCodeMap.TryGetValue(func.CPPName, out sampleCode) &&
                    !sampleCode.IsSkipTest)
                {
                    funcText += "\n%sample\n" + sampleCode.ExtractedCode + "\n";
                }
                

                allFuncText += funcText;
            }

            //---------------------------------------------------------
            // 自動生成命令 (構造体　Get Set)
            foreach (var decl in analyzer.AllStructNodes)
            {
                //-----------------------------------------
                // Setter 定義
                {
                    var info = HSPHeaderBuilder._structAccessorInfo[decl.Name];
                    var module = _analyzer.GetModuleByCommonName(decl.CommonName);
                    
                    string funcName = info.ModuleName + "_Set";

                    // 基本
                    string funcText = FuncTemplate.TrimStart();
                    funcText = funcText.Replace("_NAME_", funcName);
                    funcText = funcText.Replace("_BRIEF_", module.ModuleDoc.Summary + "(" + decl.CommonName + ") を作成し、値を設定する");
                    funcText = funcText.Replace("_INST_", "");
                    funcText = funcText.Replace("_HREF_", "");
                    funcText = funcText.Replace("_GROUP_", module.ModuleDoc.Summary);

                    // 引数リスト
                    string paramsText = decl.CommonName.ToLower();  // 第1引数
                    foreach (var memnber in decl.Members)
                    {
                        paramsText += ", ";
                        paramsText += memnber.Name;
                    }
                    funcText = funcText.Replace("_PRM_LIST_", paramsText);

                    // 引数詳細
                    paramsText = string.Format("[out] {0, -8}: {1}\n", decl.CommonName.ToLower(), "作成された情報を格納する変数");  // 第1引数
                    foreach (var memnber in decl.Members)
                    {
                        paramsText += string.Format("[in]  {0, -8}: {1}\n", memnber.Name, memnber.Comment);
                    }
                    funcText = funcText.Replace("_PRM_DETAIL_", paramsText);

                    // サンプルコード
                    TestCode sampleCode;
                    if (_sampleCodeMap.TryGetValue(funcName, out sampleCode) &&
                        !sampleCode.IsSkipTest)
                    {
                        funcText += "\n%sample\n" + sampleCode.ExtractedCode + "\n";
                    }

                    allFuncText += funcText;
                }
            }

            //---------------------------------------------------------
            // 出力
            string output = headerText;
            output += allFuncText;
            output = output.Replace("\n", "\r\n");

            // グローバル置換
            foreach (var info in _globalReplaceInfoTable)
                output = output.Replace(info.From, info.To);

            System.IO.File.WriteAllText(outputFilePath, output, System.Text.Encoding.GetEncoding("shift_jis"));

        }

        /// <summary>
        /// CallTest 用ソースの自動作成
        /// </summary>
        /// <param name="srcFilePath"></param>
        public void BuildCallTestSource(string srcFilePath, string outputFilePath)
        {
            var output = new OutputBuffer();

            foreach (var func in _analyzer.AllFuncEntites)
            {
                if (!func.Option.HSP_Enabled) continue; // 無効
                MakeTestCode(ref output, func.CPPName);
            }


            foreach (var decl in _analyzer.AllStructNodes)
            {
                // Setter 定義
                {
                    var info = HSPHeaderBuilder._structAccessorInfo[decl.Name];
                    string funcName = info.ModuleName + "_Set";
                    MakeTestCode(ref output, funcName);
                }
                // Getter 定義
                {
                    var info = HSPHeaderBuilder._structAccessorInfo[decl.Name];
                    string funcName = info.ModuleName + "_Get";
                    MakeTestCode(ref output, funcName);
                }
            }


            System.IO.File.WriteAllText(outputFilePath, output.ToString(), System.Text.Encoding.GetEncoding("shift_jis"));
        }

        /// <summary>
        /// テストコード作成
        /// </summary>
        /// <param name="output"></param>
        /// <param name="funcName"></param>
        private void MakeTestCode(ref OutputBuffer output, string funcName)
        {
            output.Append("*test_" + funcName).NewLine();
            output.IncreaseIndent();

            TestCode testCode;
            if (_sampleCodeMap.TryGetValue(funcName, out testCode))
            {
                output.AppendWithIndent(testCode.SrcCode);
            }
            else
            {
                // 未解決の部分は◆を入れておく
                output.AppendWithIndent("◆新しく生成されたコード。テストコードを記述する。").NewLine();
                output.AppendWithIndent("コードを記述しない(テストやサンプルを作らない)場合は◆を /**/ で囲むこと。").NewLine();
            }

            output.AppendWithIndent("/*test_end*/").NewLine(2);
            output.DecreaseIndent();
        }
    }
}
