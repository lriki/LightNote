using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// C# P/Invole ソース
    /// </summary>
    class CSPInvokeBuilder : Builder
    {
        #region Fields
        private LangContext _context = new LangContext(LangFlags.CS);
        private OutputBuffer _enumText = new OutputBuffer(1);
        private OutputBuffer _funcsText = new OutputBuffer(2);
        #endregion


        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected override void OnEnumLooked(CLEnum enumType)
        {
            // LNBool は enum として出力しない
            if (enumType.Name == "Bool")
                return;

            // XML コメント
            CSBuilderCommon.MakeSummaryXMLComment(_enumText, enumType.Comment);

            // [Flags] 属性
            if (enumType.IsFlags)
                _enumText.AppendWithIndent("[Flags]\n");

            // enum ヘッダ
            _enumText.AppendWithIndent("public enum {0}\n", enumType.Name);
            _enumText.AppendWithIndent("{\n");
            _enumText.IncreaseIndent();

            // 各メンバ
            foreach (var member in enumType.Members)
            {
                // LN_xxxx_MAX のようなターミネータは出力しない
                if (member.IsTerminator) continue;

                // XML コメント
                CSBuilderCommon.MakeSummaryXMLComment(_enumText, member.Comment);

                // メンバ定義
                _enumText.AppendWithIndent("{0} = {1},", member.CapitalizedName, member.Value).NewLine(2);
            }

            _enumText.DecreaseIndent();
            _enumText.AppendWithIndent("}\n\n");
        }

        /// <summary>
        /// メソッド 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnMethodLooked(CLMethod method)
        {
            //// この関数は定義しない
            //if (method.IsCopy)
            //    return;

            // XML コメント
            CSBuilderCommon.MakeSummaryXMLComment(_funcsText, _context.GetBriefText(method));
            foreach (var param in method.FuncDecl.Params)
            {
                CSBuilderCommon.MakeParamXMLComment(_funcsText, param.Name, param.Summary);
            }
            // TODO:returnコメント
            //CSBuilderCommon.MakeReturnXMLComment(method);
            CSBuilderCommon.MakeRemarksXMLComment(method.Detail);

            // DLLImport・型名・関数名
            string declText = FuncDeclTempalte.Trim();
            declText = declText.Replace("RETURN_TYPE", CSBuilderCommon.ConvertTypeToName(method.ReturnType.Type, null));
            declText = declText.Replace("FUNC_NAME", method.CName);

            // 仮引数リスト
            string argsText = "";
            foreach (var param in method.Params)
            {
                if (argsText != "") argsText += ", ";

                // 型名と引数名
                argsText += string.Format("{0} {1}", ConvertParamTypeName(param), param.Name);

                // デフォルト引数
                if (!string.IsNullOrEmpty(param.DefaultValueSource))
                    argsText += " = " + CSBuilderCommon.ConvertLiteral(param.DefaultValueSource, true);
            }
            declText = declText.Replace("ARGS", argsText);

            _funcsText.AppendWithIndent(declText).NewLine(2);
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string output = GetTemplate("CSPInvoke.txt");
            output = output.Replace("ENUM_LIST", _enumText.ToString());
            output = output.Replace("API_LIST", _funcsText.ToString());
            return output;
        }
    }
}
