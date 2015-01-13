
■制限
・関数宣言は1行で書くこと。
・enum の終端は "};" であること。また、コメント内に "enum", "};" を書くことはできない。
・enum メンバのコメントは必ず行コメントであること。コメントの開始は ///< であること。
・enum の [Flags] は未サポート
・struct は未サポート

■フォーマット
・ヘッダコメントオプション
	[{hsp disable},{ruby a=1 b=2}]

・関数宣言の抽出
	'('直前のトークンが関数名。それ以前はすべて型名となる。
	xxx yyy zzz ( aaa )
	xxx yyy → 戻り値の型名
	zzz     → 関数名

・C# enum メンバ
	LN_XXXX_YYYY_ZZZZ
	・LN_XXXX_  → 削除
	・YYYY_ZZZZ → YyyyZzzz

