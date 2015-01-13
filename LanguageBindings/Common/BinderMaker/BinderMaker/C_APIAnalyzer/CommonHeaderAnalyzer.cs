using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CommonHeaderAnalyzer
    {
        /// <summary>
        /// バージョン文字列
        /// </summary>
        public string Version { get; private set; }

        /// <summary>
        /// 解析実行
        /// </summary>
        /// <param name="filePath"></param>
        public void Analyze(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath);

            // バージョン情報は正規表現で検索
            var r = new System.Text.RegularExpressions.Regex(@"LNOTE_VERSION_STR.+\n");
            System.Text.RegularExpressions.Match m = r.Match(text);

            r = new System.Text.RegularExpressions.Regex(@""".+""");
            m = r.Match(m.Value);

            Version = m.Value.Trim('"');
        }
    }
}
