using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    abstract class CLBuilderEngine
    {
        protected class CommentReplaceItem
        {
            public string From;
            public string To;
        }

        protected CLAnalyzer Analyzer { get; set; }

        protected List<CommentReplaceItem> CommentReplaceItemList = new List<CommentReplaceItem>();

        /// <summary>
        /// テンプレートファイルを読み込む
        /// </summary>
        /// <returns></returns>
        public string GetTemplate(string fileName)
        {
            return System.IO.File.ReadAllText("../../Templates/" + fileName);
        }

        /// <summary>
        /// 作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void Build(CLAnalyzer analyzer, string outputFilePath)
        {
            Analyzer = analyzer;

            //---------------------------------------------------------
            // ドキュメントコメント置換要素作成
            foreach (var enumType in analyzer.AllEnumTypes)
            {
                MakeCommentConvertItem(enumType);
                foreach (var member in enumType.Members)
                {
                    var item = new CommentReplaceItem();
                    item.From = member.Name;
                    item.To = MakeCommentConvertItem(member);
                    if (!string.IsNullOrEmpty(item.To))
                        CommentReplaceItemList.Add(item);
                }
            }
            foreach (var classType in analyzer.AllClassTypes)
            {
                foreach (var method in classType.Methods)
                {
                    var item = new CommentReplaceItem();
                    item.From = method.CName;
                    item.To = MakeCommentConvertItem(method);
                    if (!string.IsNullOrEmpty(item.To))
                        CommentReplaceItemList.Add(item);
                }
                // クラス名はメソッドよりも後に置換検索したいため、メンバの後に Add する
                var item2 = new CommentReplaceItem();
                item2.From = classType.CName;
                item2.To = MakeCommentConvertItem(classType);
                if (!string.IsNullOrEmpty(item2.To))
                    CommentReplaceItemList.Add(item2);
            }

            //---------------------------------------------------------
            // enum
            foreach (var enumType in analyzer.AllEnumTypes)
            {
                PostMakeEnumDecl(enumType);
            }

            //---------------------------------------------------------
            // クラス・構造体
            foreach (var classType in analyzer.AllClassTypes)
            {
                if (PreClassOrStructDecl(classType))
                {
                    // プロパティ
                    foreach (var prop in classType.Properties)
                    {
                        MakeProperty(prop);
                    }

                    // メソッドひとつ分
                    foreach (var method in classType.Methods)
                    {
                        MakeMethodData(method);
                    }
                    MakeClassOrStructDecl(classType, "");
                }
            }

            System.IO.File.WriteAllText(outputFilePath, MakeOutoutFileText(), Encoding.UTF8);
        }

        /// <summary>
        /// 言語に合わせ、各識別子を置換したコメントを返す
        /// </summary>
        protected string AbjustComment(string str)
        {
            // 識別子置換
            foreach (var item in CommentReplaceItemList)
            {
                str = str.Replace(item.From, item.To);
            }
            // <br> もいらない
            str = str.Replace("<br>", "");
            return str;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="method"></param>
        private void MakeMethodData(CLMethod method)
        {
            // out 属性で一番後ろの引数を選択する。
            // (out x, out y) のように 2 つ以上 out がある場合は return としない。
            CLParam returnParam = null;
            if (method.Params.Count > 0 &&
                method.Attribute != MethodAttribute.Constructor)    // out ひとつだけのコンストラクタがこれにマッチするのでここではじく
            {
                returnParam = method.Params.Find((param) => ((param.Attribute & ParamAttribute.In) == 0 && (param.Attribute & ParamAttribute.Out) != 0));
                if (returnParam != method.Params.Last())
                    returnParam = null;
            }

            // Vector2.Normalize() 等、第1引数の struct を return で返したい
            //if (method.Attribute == MethodAttribute.StructOutputStatic)
            //    returnParam = method.Params[0];

            MakeMethod(method, returnParam);
        }

        /// <summary>
        /// ドキュメントの置換要素を作る。
        /// entity に対応する それぞれの言語での識別子を返す。
        /// </summary>
        /// <param name="entity"></param>
        public abstract string MakeCommentConvertItem(CLEntity entity);

        /// <summary>
        /// クラス・構造体出力 (前処理)
        /// </summary>
        /// <param name="type"></param>
        /// <returns>false を返すと、このクラスを生成しない</returns>
        public abstract bool PreClassOrStructDecl(CLClassType type);

        /// <summary>
        /// クラス・構造体定義出力
        /// </summary>
        public abstract void MakeClassOrStructDecl(CLClassType type, string body);

        /// <summary>
        /// プロパティ出力
        /// </summary>
        public abstract void MakeProperty(CLProperty prop);

        /// <summary>
        /// メソッド出力
        /// </summary>
        /// <param name="method"></param>
        /// <param name="returnParam">method.Param の内、return で返すべきものとして選択された CLParam</param>
        public abstract void MakeMethod(CLMethod method, CLParam returnParam);

        /// <summary>
        /// enum 出力 (後処理)
        /// </summary>
        public virtual void PostMakeEnumDecl(CLEnumType enumType) { }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        public abstract string MakeOutoutFileText();
    }
}
