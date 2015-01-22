using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    static class CSBuilderCommon
    {
        #region Templates
        public const string XMLCommentTemplate_Summary = "/// <summary>\n/// SUMMARY\n/// </summary>\n";
        public const string XMLCommentTemplate_Param = "/// <param name=\"PARAM_NAME\">DETAIL</param>\n";
        public const string XMLCommentTemplate_Return = "/// <returns>DETAIL</returns>\n";
        #endregion

        /// <summary>
        /// summary コメント作成 (\n 付き)
        /// </summary>
        /// <param name="cppSummary"></param>
        /// <returns></returns>
        public static void MakeSummaryXMLComment(OutputBuffer buffer, string cppSummary)
        {
            if (!string.IsNullOrEmpty(cppSummary))
                buffer.AppendWithIndent(XMLCommentTemplate_Summary.Replace("SUMMARY", cppSummary));
        }

        /// <summary>
        /// param コメント作成 (ひとつ分)
        /// </summary>
        /// <param name="buffer"></param>
        /// <param name="paramList"></param>
        public static void MakeParamXMLComment(OutputBuffer buffer, string paramName, string paramSummary)
        {
            buffer.AppendWithIndent(XMLCommentTemplate_Param.Replace("PARAM_NAME", paramName).Replace("DETAIL", paramSummary));
        }
    }
}
