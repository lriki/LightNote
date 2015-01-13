using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    static class RubyCommon
    {

        /// <summary>
        /// メソッドが Ruby に出力できるかをチェックする
        /// </summary>
        /// <param name="methdo"></param>
        /// <returns></returns>
        public static bool CheckInvalidMethod(CLMethod method)
        {
            int count = method.Params.FindAll((param) => param.Attribute == ParamAttribute.Out).Count;
            if (count >= 2)
            {
                Console.WriteLine("{0} Ruby では out タイプの引数を複数処理できないためスキップ。", method.CName);
                return false;
            }
            var byteArrayParam = method.Params.Find((param) => param.Type == CLPrimitiveType.ByteArray);
            if (byteArrayParam != null)
            {
                Console.WriteLine("{0} Ruby では ByteArray 型の引数を処理できないためスキップ。", method.CName);
                return false;
            }
            var intArrayParam = method.Params.Find((param) => param.Type == CLPrimitiveType.IntNativeArray);
            if (intArrayParam != null)
            {
                Console.WriteLine("{0} Ruby では IntArray 型の引数を処理できないためスキップ。", method.CName);
                return false;
            }

            return true;
        }

        /// <summary>
        /// 関数名を Ruby スタイルに変換する
        /// </summary>
        /// <param name="text"></param>
        /// <returns></returns>
        public static string ConvertCommonNameToRubyMethodName(CLMethod method)
        {
            var name = GetSnakeStyleName(method.Name);

            // 先頭が is の場合は 末尾 ? に変換
            if (name.IndexOf("is_") == 0)
            {
                if (char.IsNumber(name, 3))    // 変換した結果数値が識別子の先頭にならないこと
                    name += "?";                // ? はつけてあげる
                else
                    name = name.Substring(3) + "?";
            }


            if (method.PropertyType != PropertyNameType.NotProperty)    // プロパティの場合は = 等に変更
            {
                if (method.PropertyType == PropertyNameType.Get)
                    name = name.Substring(4);   // 先頭の get_ を取り除く
                else if (method.PropertyType == PropertyNameType.Set)
                    name = name.Substring(4) + "=";   // 先頭の set_ を取り除き、後ろに =
            }

            return name;
        }

        /// <summary>
        /// InitAuto → init_audio
        /// </summary>
        /// <returns></returns>
        public static string GetSnakeStyleName(string text)
        {
            string output = "";

            // 後ろから
            bool isLastLow = char.IsLower(text, text.Length - 1);
            int upperCount = 0;
            int lastIndex = text.Length;
            int i;
            for (i = text.Length - 2; i >= 0; i--)
            {
                if (isLastLow)
                {
                    if (char.IsUpper(text, i))
                    {
                        if (lastIndex - i > 0)
                        {
                            if (output.Length != 0) output = "_" + output;
                            output = text.Substring(i, lastIndex - i).ToLower() + output;
                        }
                        lastIndex = i;
                        isLastLow = false;
                    }
                    else
                    {
                        // 小文字が続いている
                    }
                }
                else
                {
                    upperCount++;
                    if (char.IsLower(text, i))
                    {
                        upperCount = 0;
                        if (upperCount == 1)
                            isLastLow = true;
                        else
                        {
                            if (lastIndex - i - 1 > 0)
                            {
                                if (output.Length != 0) output = "_" + output;
                                output = text.Substring(i + 1, lastIndex - i - 1).ToLower() + output;
                            }
                            lastIndex = i + 1;
                            isLastLow = true;
                        }
                    }
                    else
                    {
                        // 大文字が続いている
                    }
                }
            }

            if (lastIndex != 0)
            {
                if (lastIndex - i > 0)
                {
                    if (output.Length != 0) output = "_" + output;
                    output = text.Substring(0, lastIndex).ToLower() + output;
                }
            }

            return output;
        }
    }
}
