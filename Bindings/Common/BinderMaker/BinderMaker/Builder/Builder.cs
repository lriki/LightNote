﻿using System;
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

            OnInitialize();

            // enum
            foreach (var e in Manager.AllEnums)
            {
                OnEnumLooked(e);
            }

            // クラス
            foreach (var classType in Manager.AllClasses)
            {
                if (OnClassLookedStart(classType))
                {
                    // プロパティ
                    foreach (var prop in classType.Properties)
                    {
                        OnPropertyLooked(prop);
                    }

                    // メソッド
                    foreach (var method in classType.Methods)
                    {
                        // プロパティ や internal は出力しない
                        if (method.PropertyNameType == PropertyNameType.NotProperty &&
                            method.Modifier != MethodModifier.Internal)
                        {
                            OnMethodLooked(method);
                        }
                    }

                    OnClassLookedEnd(classType);
                }
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
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnInitialize() { }

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnEnumLooked(CLEnum enumType) { }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual bool OnClassLookedStart(CLClass classType) { return true; }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnClassLookedEnd(CLClass classType) { }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnPropertyLooked(CLProperty prop) { }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnMethodLooked(CLMethod method) { }
        
        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected abstract string OnMakeOutoutFileText();
    }
}
