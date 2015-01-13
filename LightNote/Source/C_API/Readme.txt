■新しい公開 ReferenceObject の作り方
① Core 側のクラス定義(.h)に LN_TYPE_INFO_ACCESS_DECL; マクロを記述する。
② Core 側のクラス定義(.cpp)に LN_TYPE_INFO_ACCESS_IMPL(クラス名); マクロを記述する。
③ C_API 側のクラス定義(.h)に LNOTE_TYPE_INFO_DECL(クラス名); マクロを記述する。
④ C_API 側のクラス定義(.cpp)に LNOTE_TYPE_INFO_IMPL(Scene::Camera, LNCamera); マクロを記述する。

■構造体クラスの生成ルール
・Set～ (～は "Vec2" 等、オーバーロードを示すもの) は、instance メンバとなる。
・Get～ で、最後の引数が out 属性の場合は getter プロパティとなる。
・Get～ 以外で、最後の引数が out 属性の場合は static なメソッドとなり、out 属性の引数が戻り値となる。
・前述パターンかつ、全引数 2 個しかない場合は instance メソッドと static メソッドの2つが生成される。(slimDXライク)
	例) LNVector3_Normalize(vec, outVec)
		→ Vector3 Normalize()		… this を使用
		→ static Vector3 Normalize(vec) 
 

■戻り値とエラー処理について
	・Create系 → NULL を返したらエラー。詳細は GetLastError を使う。
		out 引数にはしないの？
			→C++やC#でコンストラクタ初期化子に書くことができなくなる。
			  がんばれば直せるが・・・。
	・成否値を lnBool にしないの？
		→ バインダを作るときに、「この戻り値が成否をあらわす」ことがわからなくなる。
		   また @return コメントからとる方法もあるが・・・。
	
	他にも「Create含めて戻り地を」			





このフォルダのファイルが、ライブラリのユーザーにメインで公開するインターフェイスになる。

そのため、ユーザビリティ向上と各種言語バインダ自動生成の簡易化のため、
以下のようなコーディングルールを設けている。

■ 関数名は 「モジュール名_機能名」であること
	例)	LSound_Play




■	lnHandle オブジェクトを作成する関数は 「モジュール名_Create～」であること
	lnHandle を戻り値とするこの関数は、言語バインダでコンストラクタとして展開される。
	例)	LTexture_CreateFromFile

■	lnHandle 型の仮引数は、末尾をモジュール名とすること
	言語バインダは引数名からモジュール(クラス型)を特定する。
	例) font		→ 全体が LFont とマッチ	
	例)	sceneNode	→ 全体が LSceneNode とマッチ
	例)	srcTexture	→ "Texture" の部分が LTexture とマッチ



■	プロパティ getter setter の @biref は「の取得」「の設定」以外が一致していること

■	Doxygen コメントの定義順は (@defgroup) → @brief → @param → @return → @par → (@{)であること。
	これ以外は使えない。

■	@param の後の [in, out] は必ず指定すること。
	C#等の構造体クラスに変換するとき、この種類を元に様々な整形を行う。
	第1引数に[in]が含まれている → 第1引数を ref this とするインスタンスメソッド
	第1引数が[out]のみ			→ 第1引数を戻り値とする static 関数



■	変換トラブルシューティング
	●	[C#] API の仮引数が ref で、クラスライブラリからの呼び出しが out になっていてコンパイルエラー
		→ @param 後の [in, out]の指定が間違っている 

■ @override
	Doxygen の設定で以下のようにしている。
	・ALIASES  = "override=@par Override:\n@endhtmlonly @ref "
	@return 等の同じように、Override: セクションでリンク付きで強調される。
	C# ではこれを元に override キーワードを付加するため、警告回避のために必ず指定すること。
