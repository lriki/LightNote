using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * yard 0.8.7.4 は、module をネストすると解析できない不具合がある。
 * そのため、全てのクラスに LN:: を付けて module を表現している。
 */

namespace BinderMaker
{
    class RubyDocBuilder : CLBuilderEngine
    {
        class OverloadMethodInfo
        {
            public CLMethod Method;
            public CLParam ReturnParam;
        }

        class CurrentClassInfo
        {
            /// <summary>
            /// オーバーロードメソッドの集計 (コンストラクタは Constructor という名前で登録する)
            /// </summary>
            public Dictionary<string, List<OverloadMethodInfo>> OverloadTable = new Dictionary<string, List<OverloadMethodInfo>>();
        }

        private CurrentClassInfo _currentClassInfo;
        private OutputBuffer _output = new OutputBuffer();

        public RubyDocBuilder()
        {
            //_output.AppendWithIndent("module LN").NewLine();
            //_output.IncreaseIndent();
        }

        /// <summary>
        /// ドキュメントの置換要素を作る
        /// </summary>
        /// <param name="entity"></param>
        public override string MakeCommentConvertItem(CLEntity entity)
        {
            return null;
        }

        public override bool PreClassOrStructDecl(CLClassType type)
        {
            _currentClassInfo = new CurrentClassInfo();
            return true;
        }

        public override void MakeClassOrStructDecl(CLClassType type, string body)
        {
            _output.AppendWithIndent("# " + type.Documment.Summary).NewLine();
            _output.AppendWithIndent("class LN::" + type.Name);
            if (type.BaseType != null)
                _output.Append(" < " + type.BaseType.Name);
            _output.NewLine();
            _output.IncreaseIndent();

            // initialize
            if (type.IsStruct)
            {
                #region 構造体の場合はコンストラクタ・アクセサを定義する

                // def initialize
                var argsText = new OutputBuffer();
                var paramsText = new OutputBuffer();
                foreach (var memberDecl in type.StructMembers)
                {
                    // TODO:型名
                    paramsText.AppendWithIndent("#   @param [{0}] {1} {2}", memberDecl.Type, memberDecl.Name.ToLower(), memberDecl.Comment).NewLine();
                    argsText.AppendCommad(memberDecl.Name.ToLower());
                }
                _output.AppendWithIndent("# 初期化").NewLine();
                _output.AppendWithIndent("# @overload initialize({0})", argsText.ToString()).NewLine();
                _output.AppendWithIndent(paramsText.ToString());
                _output.AppendWithIndent("def initialize").NewLine();
                _output.AppendWithIndent("end").NewLine(2);

                // プロパティアクセサ
                foreach (var memberDecl in type.StructMembers)
                {
                    string name = memberDecl.Name.ToLower();
                    // setter
                    _output.AppendWithIndent("# " + memberDecl.Comment).NewLine();
                    _output.AppendWithIndent("# @overload {0}=(v)", name).NewLine();
                    _output.AppendWithIndent("#   @param [{0}] v ", memberDecl.Type).NewLine();
                    _output.AppendWithIndent("def {0}=(v)", name).NewLine();
                    _output.AppendWithIndent("end").NewLine(2);
                    // getter
                    _output.AppendWithIndent("# " + memberDecl.Comment).NewLine();
                    _output.AppendWithIndent("# @overload {0}", name).NewLine();
                    _output.AppendWithIndent("#   @return [{0}]", memberDecl.Type).NewLine();
                    _output.AppendWithIndent("def {0}", name).NewLine();
                    _output.AppendWithIndent("end").NewLine(2);
                }
                #endregion
            }

            //-------------------------------------------------
            // メソッド出力
            foreach (var overloads in _currentClassInfo.OverloadTable)
            {
                MakeFuncDecl(overloads.Value);
            }

            _output.DecreaseIndent();
            _output.AppendWithIndent("end").NewLine();


        }

        public override void MakeProperty(CLProperty prop)
        {
        }

        public override void MakeMethod(CLMethod method, CLParam returnParam)
        {
            // Ruby として出力できるメソッドであるか
            if (!RubyCommon.CheckInvalidMethod(method))
                return;

            // オーバーロードの集計 (RubyBuilder と同じ)
            {
                List<OverloadMethodInfo> overloads;
                string key = (method.Attribute == MethodAttribute.Constructor) ? "Constructor" : method.Name;
                if (_currentClassInfo.OverloadTable.TryGetValue(key, out overloads))
                {
                    var info = new OverloadMethodInfo();
                    info.Method = method;
                    info.ReturnParam = returnParam;
                    _currentClassInfo.OverloadTable[key].Add(info);
                }
                else
                {
                    // 新しく登録
                    var info = new OverloadMethodInfo();
                    info.Method = method;
                    info.ReturnParam = returnParam;
                    _currentClassInfo.OverloadTable[key] = new List<OverloadMethodInfo>() { info };
                }
            }
        }

        /// <summary>
        /// Enum 出力
        /// </summary>
        /// <param name="enumType"></param>
        public override void PostMakeEnumDecl(CLEnumType enumType)
        {
            // 出力しないもの
            if (enumType.Name == "Bool")
                return;

            _output.AppendWithIndent("# " + enumType.Name.ToUpper()).NewLine();
            _output.AppendWithIndent("module LN::" + enumType.Name/*.ToUpper()*/).NewLine();
            _output.IncreaseIndent();
            foreach (var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは出力しない

                _output.AppendWithIndent("# " + member.Comment).NewLine();

                string name = member.CommonSnakeName.ToUpper();

                // BEGIN と END はキーワードなので別名にする
                if (enumType.Name == "SeekOrigin")
                    name = "SEEK_" + name;
                else if (enumType.Name == "KeyCode")
                    name = "KEY_" + name;

                _output.AppendWithIndent("{0} = {1}", name, member.Value).NewLine();

            }
            _output.DecreaseIndent();
            _output.AppendWithIndent("end").NewLine();
        }

        public override string MakeOutoutFileText()
        {
            //_output.DecreaseIndent();
            //_output.AppendWithIndent("end");
            return _output.ToString();
        }

        /// <summary>
        /// オーバーロードを考慮しつつ、関数定義を作成する
        /// </summary>
        private void MakeFuncDecl(List<OverloadMethodInfo> overloadMethods)
        {
            var baseMethod = overloadMethods[0].Method;

            // メソッド名
            string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);

            // コンストラクタの場合は名前を強制的に initialize にする
            if (baseMethod.Attribute == MethodAttribute.Constructor)
                rubyMethodName = "initialize";

            // キャプション。オーバーロード全てをリスト要素で並べる
            _output.AppendWithIndent("# " + baseMethod.Summary).NewLine();
            //foreach (var info in overloadMethods)
            //{
            //    _output.AppendWithIndent("# - " + info.Method.Summary).NewLine();
            //}

            foreach (var info in overloadMethods)
            {
                var paramsText = new OutputBuffer();
                var argsText = new OutputBuffer();
                string returnText = "";
                foreach (var param in info.Method.Params)
                {
                    // 第1引数かつインスタンスメソッドの場合はドキュメント化する必要はない
                    if (param == info.Method.Params.First() &&
                        info.Method.IsInstanceMethod)
                    {
                    }
                    else if (param == info.ReturnParam)
                    {
                        returnText = param.Summary;
                    }
                    else
                    {
                        argsText.AppendCommad(param.Name);
                        if (!string.IsNullOrEmpty(param.DefaultValueSource))
                            argsText.Append("=" + ConvertDefaultArgVaue(param.DefaultValueSource));
                        paramsText.AppendWithIndent("#   @param [{0}] {1} {2}", ConvertTypeCToRuby(param.Type), param.Name, param.Summary).NewLine();
                    }
                }

                //---------------------------------------------
                // 結合

                _output.AppendWithIndent("# @overload {0}({1})", rubyMethodName, argsText.ToString()).NewLine();

                // overrload がある場合は @overload 下にも概要を書いておく
                // (- を付けてリスト要素化することで details と区別する)
                if (overloadMethods.Count > 1)
                    _output.AppendWithIndent("#   " + info.Method.Summary).NewLine();

                // @note
                if (!string.IsNullOrEmpty(info.Method.Detail))
                {
                    if (overloadMethods.Count == 1)
                        _output.AppendWithIndent("#   　").NewLine();    // ダミーキャプションとして全角スペースを入れる
                    //_output.AppendWithIndent("#   @note ").NewLine();
                    //_output.AppendWithIndent("#   ").NewLine();    // @note は必要なし。また、インデントも @override のすぐ下にしておく
                    // 概要とはインデントをひとつ下げると、別のセクションで表示できる。
                    string[] lines = info.Method.Detail.Split('\n');
                    foreach (var line in lines)
                        _output.AppendWithIndent("#     " + line).NewLine();
                }

                // @param
                _output.AppendWithIndent(paramsText.ToString());

                // @return
                if (!string.IsNullOrEmpty(returnText))
                    _output.AppendWithIndent("#   @return [{0}] {1}", ConvertTypeCToRuby(info.ReturnParam.Type), returnText).NewLine();

                
            }

            _output.AppendWithIndent("def " + rubyMethodName);
            _output.NewLine();
            _output.AppendWithIndent("end").NewLine();
        }

        /// <summary>
        /// デフォルト引数値の変換
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private string ConvertDefaultArgVaue(string value)
        {
            // double に変換できれば、数値リテラル
            double tmp;
            if (double.TryParse(value, out tmp))
                return value;

            if (value == "LN_TRUE")
                return "true";
            if (value == "LN_FALSE")
                return "false";
            if (value == "NULL")
                return "nil";

            // enum メンバかもしれない
            foreach (var enumType in Analyzer.AllEnumTypes)
            {
                foreach (var m in enumType.Members)
                {
                    if (m.Name == value)
                        return m.CommonSnakeName;//value.Substring(3);  // 先頭の LN_ を外す
                }
            }
            throw new InvalidOperationException("Not Found : " + value);
        }

        /// <summary>
        /// 型名変換
        /// </summary>
        /// <returns></returns>
        private string ConvertTypeCToRuby(CLType type)
        {
            // プリミティブ型
            RubyBuilder.RubyTypeInfo info;
            if (RubyBuilder._rubyTypeTable.TryGetValue(type, out info))
                return info.RubyType;

            // enum 型                    
            // ※ LN:: を付けて完全修飾名にすることで、ドキュメントにリンクが張られる
            if (type is CLEnumType)
                return "LN::" + type.Name; // 大文字化

            // class 型
            var classType = type as CLClassType;
            if (classType != null)
                return "LN::" + type.Name;

            throw new InvalidOperationException();
        }
    }
}
