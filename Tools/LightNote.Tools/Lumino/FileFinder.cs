using System;
using System.Collections.Generic;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// ファイル検索ユーティリティ
    /// </summary>
    public class FileFinder
    {
        /// <summary>
        /// rootPath 以下のファイルの走査(返すファイル名にはrootPathのフォルダ名を含む)
        /// </summary>
        /// <param name="rootPath"></param>
        /// <returns></returns>
        public string[] Lookup(string rootPath)
        {
            //this.listView1.Items.Clear();

            //string dirName = System.IO.Path.GetFileName(rootPath) + "\\";
            string root_directry_name = "";//
            //root_directry_name += "\\";
            string[] filenames = GetFilesMostDeep(rootPath, "*.*", root_directry_name);

            for (int i = 0; i < filenames.Length; ++i)
            {
                filenames[i] = filenames[i];
            }
            return filenames;
        }

        /// <summary>
        /// MakeTree() から呼ばれる
        /// </summary>
        /// <param name="root_path"></param>
        /// <param name="pattern"></param>
        /// <param name="parent_path"></param>
        /// <returns></returns>
        private string[] GetFilesMostDeep(string root_path, string pattern, string parent_path)
        {
            System.Collections.Specialized.StringCollection hStringCollection = (
                new System.Collections.Specialized.StringCollection()
            );

            // このディレクトリ内のすべてのファイルを検索する
            foreach (string file_path in System.IO.Directory.GetFiles(root_path, pattern))
            {
                string ts = parent_path + System.IO.Path.GetFileName(file_path);
                hStringCollection.Add(ts);
            }

            // このディレクトリ内のすべてのサブディレクトリを検索する (再帰)
            foreach (string dir_path in System.IO.Directory.GetDirectories(root_path))
            {
                string ts = parent_path + System.IO.Path.GetFileName(dir_path) + "\\";
                string[] file_pathes = GetFilesMostDeep(dir_path, pattern, ts);

                // 条件に合致したファイルがあった場合は、ArrayList に加える
                if (file_pathes != null)
                {
                    hStringCollection.AddRange(file_pathes);
                }
            }

            // StringCollection を 1 次元の String 配列にして返す
            string[] stReturns = new string[hStringCollection.Count];

            hStringCollection.CopyTo(stReturns, 0);

            return stReturns;
        }
    }
}
