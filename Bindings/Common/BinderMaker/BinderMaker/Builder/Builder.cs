using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// 各言語ソース生成クラスのベース
    /// </summary>
    abstract class Builder
    {
        /// <summary>
        /// 中間コード管理クラス
        /// </summary>
        protected CLManager Manager { get; private set; }

        /// <summary>
        /// ソースファイル作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void Build(CLManager manager, string outputFilePath)
        {
            Manager = manager;

            // enum
            foreach (var e in Manager.AllEnums)
            {
                OnEnumLooked(e);
            }

            // クラス
            foreach (var classType in Manager.AllClasses)
            {
                OnClassLookedStart(classType);

                // メソッド
                foreach (var method in classType.Methods)
                {
                    OnMethodLooked(method);
                }

                OnClassLookedEnd(classType);
            }

            // ファイルに出力
            System.IO.File.WriteAllText(outputFilePath, OnMakeOutoutFileText(), Encoding.UTF8);
        }

        /// <summary>
        /// テンプレートファイルを読み込む
        /// </summary>
        /// <returns></returns>
        protected string GetTemplate(string fileName)
        {
            return System.IO.File.ReadAllText("../../Builder/Templates/" + fileName);
        }

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnEnumLooked(CLEnum enumType) { }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnClassLookedStart(CLClass classType) { }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnClassLookedEnd(CLClass classType) { }

        /// <summary>
        /// メソッド 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnMethodLooked(CLMethod method) { }
        
        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected abstract string OnMakeOutoutFileText();
    }
}
