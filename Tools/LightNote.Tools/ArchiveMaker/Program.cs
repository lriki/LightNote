using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace ArchiveMaker
{
    class Program
    {
        static void Main(string[] args)
        {
#if DEBUG
            args = new string[] { @"D:\GameProjects\Release\Data", "test" };
            string password = args[1];
            string outputFilePath = args[0] + ".lna";
#else
            if (args.Length > 2)
            {
                Console.WriteLine("Invalid args.");
                Console.WriteLine("> ArchiveMaker \"Directory\" \"Password\"");
                return;
            }

            if (!Directory.Exists(args[0]))
            {
                Console.WriteLine("Invalid directory.");
                return;
            }

            // 入力パスの拡張子を ".lna" へ
            string outputFilePath = args[0] + ".lna";

            Console.WriteLine("creating {0}", outputFilePath);

            string password;
            if (args.Length == 1)
            {
                // フォルダ名しかなかったら、パスワードを入力してもらう
                Console.Write("Password:");
                password = Console.ReadLine();
            }
            else
            {
                password = args[1];
            }
#endif

            // ファイル名リストを作る
            var finder = new Lumino.FileFinder();
            string[] files = finder.Lookup(args[0]);

            LArchiveMaker_Open(outputFilePath, password);
            foreach (string file in files)
            {
                Console.WriteLine(file);

                // accessName は ターゲットフォルダからの相対パス
                bool r = LArchiveMaker_AddFile(args[0] + "\\" + file, file);
                if (!r)
                {
                    Console.WriteLine("Failed : {0}", file);
                    break;
                }
            }
            LArchiveMaker_Close();

            Console.WriteLine("\nOutput File : {0}", outputFilePath);
        }

        public const string DLLName = "LNote.dll";
        public const CharSet DLLCharSet = CharSet.Ansi;
        public const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;

        /// <summary>
        /// 新規にファイルを開いてアーカイブ作成の準備をする
        /// </summary>
        /// <param name="filePath">: ファイル名</param>
        /// <param name="password">: 暗号化に使う文字列 (127文字まで)</param>
        [DllImport(DLLName, CharSet = CharSet.Ansi, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open(string filePath, string password);

        /// <summary>
        /// ファイルを追加する
        /// </summary>
        /// <param name="filePath">: 追加するファイルの名前</param>
        /// <param name="accessName">: プログラムからアクセスする時の名前</param>
        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile(string filePath, string accessName);

        /// <summary>
        /// アーカイブファイルを閉じる
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();
    }
}
