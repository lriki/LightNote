//=============================================================================
//
//-----------------------------------------------------------------------------
/**
	@page modules1_intro1 はじめに (LightNote の特徴・注意点)

	<br>ここでは、LightNote を使用して開発を行うに当たって、留意しておくべきことについてまとめています。
	
    - @ref modules1_intro1_anchor1   "マルチコア最適化"
    - @ref modules1_intro1_anchor2   "暗号化アーカイブ"
    - @ref modules1_intro1_anchor3   "言語バインディングとドキュメント上の表記について"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_intro1_anchor1
    @par マルチコア最適化
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote はマルチコア CPU に対して最適化されています。<br>
    <br>
    ゲームには描画処理や物理演算等、時間のかかる処理のまとまりがいくつか存在します。<br>
    特に描画処理はシングルスレッドモデルの場合に全体の処理の8割を占めると言われています。<br>
    <br>
    LightNote はこれらの処理をゲーム本体の処理を行うメインスレッドとは切り離し、専用のスレッドを用意して並列処理を行います。<br>
    これによりメインスレッドのCPUリソースは大きく削減され、高いパフォーマンスをゲームロジックに集中することができます。<br>
    <br>
    一方で、各スレッドの同期処理のようにシングルスレッドモデルでは必要のないオーバーヘッドが存在します。<br>
    そのため、シングルコア CPU では逆にパフォーマンスが低下する可能性がありますのでご注意ください。<br>
    (昨今のデスクトップ・ノートPCはほぼ2コア以上持っていますが)<br>
    <br>
    現在の LightNote は、目安として 4 コア以上の CPU で最も高いパフォーマンスを発揮できます。<br>
    <br>
    <br>
    【通常の方法ではパフォーマンスが測れないこと】<br>
    プロファイラを使う。<br>
    システムキーの詳細は @ref modules1_config1_anchor2 を参照してください。<br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_intro1_anchor3
    @par	言語バインディングとドキュメント上の表記について
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote は以下のプログラム言語に対応しています。<br>
    - C言語<br>
    - C++<br>
    - .NET 対応言語(C＃、VisualBasic 等)<br>
    - Ruby<br>
    - HSP (Hot Soup Processor)<br>
    
    @par
    LightNote の機能はオブジェクト指向のスタイルに従って構造化されています。<br>
    そのため、このプログラミングガイドのようなドキュメントでは便宜上、そのような概念の無い言語に対しても<br>
    「クラス」や「継承」といったキーワードを用いることがありますので、ご注意ください。<br>
    <br>
@htmlonly<div class="ln_lang_tabs"><ul class="ln_tab_menu"><li name="tab1"><a>C
</a></li><li name="HSP_tab"><a>HSP
</a></li></ul><div class="ln_tab_content" name="tab1">
<pre class="brush: cpp">lnHandle sprite

// LSprite～ はスプライトクラスの関数です。
LSprite_Create(&sprite);					// スプライトを作成し、スプライトハンドルを得る
LSprite_SetSize(sprite, 10, 10);			// スプライトのサイズを設定する

// Sprite は SceneNode を継承しており、スプライトハンドルに対して LSceneNode～ の関数を使用できます。
LSceneNode_SetPosition(sprite, 10, 20, 0);	// スプライトの位置を設定する
</pre></div><div class="ln_tab_content" name="HSP_tab">
<pre class="brush: cpp">// LSprite～ はスプライトクラスの命令です。
LSprite_Create sprite						// スプライトを作成し、スプライトハンドルを得る
LSprite_SetSize sprite, 10, 10				// スプライトのサイズを設定する

// Sprite は SceneNode を継承しており、スプライトハンドルに対して LSceneNode～ の命令を使用できます。
LSceneNode_SetPosition sprite, 10, 20, 0	// スプライトの位置を設定する
</pre></div></div>@endhtmlonly
    <br>
    <br>


*/
//=============================================================================
