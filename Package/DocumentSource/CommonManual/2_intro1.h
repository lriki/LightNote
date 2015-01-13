//=============================================================================
//【 はじめに 】
//-----------------------------------------------------------------------------
/**
    @page intro1 はじめに
    
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor intro1_anchor1
    <hr>
    @par ライセンス
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LNote のライセンスは <a href="http://sourceforge.jp/projects/opensource/wiki/licenses%2Fzlib_libpng_license">zlib-license</a> です。<br>
    詳細及び LNote が使用しているソフトウェアのライセンスについては @subpage license1 を確認してください。<br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor intro1_anchor2
    <hr>
    @par 動作環境
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <table>
    <tr>
        <td><b>OS</b></td>
        <td>Windows XP/Vista/7/8 32/64Bit (DirectX 9.0c 以上)</td>
    </tr>
    <tr>
        <td><b>CPU</b></td>
        <td>Pentium または互換性のあるCPU (2 Core以上推奨)</td>
    </tr>
    <tr>
        <td><b>メモリ</b></td>
        <td>256 MB以上</td>
    </tr>
    <tr>
        <td><b>ビデオカード</b></td>
        <td>シェーダモデル 3.0 以上</td>
    </tr>
    <tr>
        <td><b>サウンドカード</b></td>
        <td>XAudio2 互換</td>
    </tr>
    </table>
    <br>
    本ライブラリを使用したプログラムを実行するためには、DirectX のエンドユーザーランタイムをインストールする必要があります。<br>
    制作物を配布する場合は、この旨を配布物に含めることをお勧めします。<br>
    <br>
    <br>
    本ライブラリを使用したプログラムを実行するためには、DirectX のエンドユーザーランタイムをインストールする必要があります。<br>
    制作物を配布する場合は、この旨を配布物に含めることをお勧めします。<br>
    <br>
    エンドユーザーランタイムは以下のページからダウンロードできます。<br>
    <a href="http://www.microsoft.com/ja-jp/download/details.aspx?id=35">http://www.microsoft.com/ja-jp/download/details.aspx?id=35</a>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor intro1_anchor3
    <hr>
    @par サポートプログラム言語・開発環境

    @if DOXY_COMMENT --------------------------------------------------- @endif
    
    - C言語<br>
    - C++<br>
    - .NET 対応言語(C＃、VisualBasic 等)<br>
    - Ruby<br>
    - HSP (Hot Soup Processor)<br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor intro1_anchor4
    <hr>
    @par 実装機能・ロードマップ
    <br>
	<table>
	<tr>
	<td style="background-color:#99ff00">Implemented</td>
	<td><span style="white-space:pre">完了</td>
	<td style="background-color:#ff9900">In progress</td>
	<td><span style="white-space:pre">実装中・未解決</span></td>
	<td style="background-color:#93d3ff">Planning</td>
	<td>仕様策定中</td>
	<td>-</td>
	<td>未着手</td>
	</tr>
	</table>
	<br>
<table>
<tr bgcolor="#E0E0E0"><td>カテゴリ</td><td>内容</td><td>進捗状況</td><td>備考</td></tr>
<tr><td rowspan=" 4">基幹</td><td>Windows</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>Linux</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>Mac</td><td bgcolor = "#93d3ff">Planning</td><td></td></tr>
<tr><td>FPSカウンタ・簡易プロファイラ</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td rowspan=" 2">ファイル入出力</td><td>暗号化アーカイブ</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>非同期Read/Write</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td rowspan=" 3">入力</td><td>キーボード/マウス/ジョイパッド</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>仮想コントローラ</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>キーコンフィグ</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td rowspan=" 18">グラフィック</td><td>DirectX9</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>DirectX11</td><td bgcolor = "#93d3ff">Planning</td><td></td></tr>
<tr><td>OpenGL</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>画像ファイル (.bmp)</td><td bgcolor = "#99ff00">Implemented</td><td>DirectX のみ</td></tr>
<tr><td>画像ファイル (.png)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>画像ファイル (.jpg)</td><td bgcolor = "#99ff00">Implemented</td><td>DirectX のみ</td></tr>
<tr><td>画像ファイル (.tiff)</td><td bgcolor = "#99ff00">Implemented</td><td>DirectX のみ</td></tr>
<tr><td>文字列描画</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>任意のフォントファイル(.ttf)の登録</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>2D/3Dスプライト</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>3Dモデル (.x)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>3Dモデル (.pmd)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>3Dモデル (.pmx)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>モーション (.x)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>モーション (.vmx)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>ビジュアルエフェクト(<a href="https://sites.google.com/site/effekseer/">Effekseer</a> 形式)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>シェーダ (HLSL)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>シェーダ (GLSL)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td rowspan=" 10">音声</td><td>音声ファイル (.wav)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>音声ファイル (.ogg)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>音声ファイル (.mp3)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>音声ファイル (.mid)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>音量・ピッチ操作</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>Ogg と Midi はループ位置の埋め込みに対応</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>同一音声のメモリバッファの自動共有</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>3D サウンド</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>ゲームオーディオ</td><td bgcolor = "#99ff00">Implemented</td><td>RGSS の Audio クラスをイメージしたユーティリティ</td></tr>
<tr><td>遅延読み込み</td><td bgcolor = "#93d3ff">Planning</td><td></td></tr>
<tr><td rowspan=" 4">算術演算</td><td>2～4次元ベクトル</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>行列</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>クォータニオン</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>長周期乱数</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td rowspan=" 6">シーングラフ</td><td>ノードツリー</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>レイヤー管理</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>スクリーンエフェクト (ブラー)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>スクリーンエフェクト (色調変更)</td><td bgcolor = "#99ff00">Implemented</td><td></td></tr>
<tr><td>スクリーンエフェクト (トランジション)</td><td bgcolor = "#99ff00">Implemented</td><td>これも RGSS ライク</td></tr>
<tr><td>シェーダフレームワーク (MME)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td rowspan=" 5">物理演算</td><td>剛体 (基本形状(箱、球、カプセル))</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>ジョイント (ばね)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>コリジョンボディ (基本形状(箱、球、カプセル))</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>コリジョンボディ (メッシュ)</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td>ソフトボディ</td><td bgcolor = "#ff9900">In progress</td><td></td></tr>
<tr><td rowspan=" 1">GUI</td><td>ウィンドウシステム</td><td bgcolor = "#93d3ff">Planning</td><td></td></tr>
</table>
    <br>
    <br>

    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor intro1_anchor6
    <hr>
    @par サポート・連絡先
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    感想、不具合、要望、バグ報告などの連絡は、こちらへお願いします。
    - ホームページ<br>
        http://nnmy.sakura.ne.jp/
    - サポート掲示板<br>
        http://nnmy.sakura.ne.jp/public_html/support_bbs/patio.cgi
    <br>
    <br>

*/
//=============================================================================