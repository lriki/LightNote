using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    /// <summary>
    /// 中間コード管理クラス
    /// </summary>
    class CLManager
    {
        //①型の集計
        //②型の割り当て
        //③オーバーロードメソッドの集計
        //④プロパティの集計




        #region Properties
        /// <summary>
        /// グローバルインスタンス
        /// </summary>
        public static CLManager Instance { get; set; }

        /// <summary>
        /// 全 Entity リスト
        /// </summary>
        public List<CLEntity> AllEntities { get; private set; }

        /// <summary>
        /// 全型リスト
        /// </summary>
        public List<CLType> AllTypes { get; private set; }
        #endregion

        #region Methods
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CLManager()
        {
            AllEntities = new List<CLEntity>();
            AllTypes = new List<CLType>();
        }

        /// <summary>
        /// パース済みの AllEntities を解析し、型情報やプロパティの関係を作成する
        /// </summary>
        public void LinkEntities()
        {
            
        }

        /// <summary>
        /// オリジナルの型名から CLType を検索する。
        /// "LN_HANDLE(LNSound)" とかもOK。 
        /// </summary>
        /// <param name="name"></param>
        public CLType FindType(string name)
        {

        }
        #endregion


    }
}
