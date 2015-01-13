//=============================================================================
// プログラミングガイド - 初期化
//-----------------------------------------------------------------------------
/**
    @page modules1_initialize1 LightNote の初期化

    - @ref modules1_initialize1_anchor1   "全ての機能を初期化し、実行する"
    - @ref modules1_initialize1_anchor2   "初期設定を行う"
    - @ref modules1_initialize1_anchor3   "必要な機能だけ個別に初期化する"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_initialize1_anchor1
    @par 全ての機能を初期化し、実行する
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote を正しく動作させる最小限のプログラムは次のとおりです。<br>
    このプログラムを実行すると空のウィンドウが表示され、クローズボタンを押すとプログラムを終了します。<br>
    <br>
@htmlonly<div class="ln_lang_tabs"><ul class="ln_tab_menu"><li name="tab1"><a>C
</a></li><li name="C++_tab"><a>C++
</a></li><li name="C#_tab"><a>C#
</a></li><li name="Ruby_tab"><a>Ruby
</a></li><li name="HSP_tab"><a>HSP
</a></li></ul><div class="ln_tab_content" name="tab1">
<pre class="brush: cpp">#include &lt;lnote.h&gt;

int main()
{
	lnBool isEnd;
	
	// LightNote の初期化
	LCore_Initialize();

	// メインループ
	do
	{
		// ここにゲームのメイン処理を書きます
		
		LCore_Update();
		LCore_IsEndRequested(&isEnd);
	} while(!isEnd);

	// LightNote の終了処理
	LCore_Terminate();
	
	return 0;
}
</pre></div><div class="ln_tab_content" name="C++_tab">
<pre class="brush: cpp">#include &lt;lnote.hpp&gt;
using namespace LN;

int main()
{
	// LightNote の初期化
	Core::Initialize();
	
	// メインループ
	do
	{
		// ここにゲームのメイン処理を書きます
		
		Core::Update();
	} while(!Core::IsEndRequested());
	
	// LightNote の終了処理
	Core::Terminate();
	
	return 0;
}
</pre></div><div class="ln_tab_content" name="C#_tab">
<pre class="brush: csharp">using LN;

class Program
{
	static void Main()
	{
		// LightNote の初期化
		Core.Initialize();

		// メインループ
		do
		{
			// ここにゲームのメイン処理を書きます
			
			Core.Update();
		} while(Core.IsEndRequested);

		// LightNote の終了処理
		Core.Terminate();
	}
}
</pre></div><div class="ln_tab_content" name="Ruby_tab">
<pre class="brush: ruby">require 'lnote'
include LN

# LightNote の初期化
Core.initialize

# メインループ
begin
	# ここにゲームのメイン処理を書きます
	
	Core.update
end while Core.end_requested?

# LightNote の終了処理
Core.terminate
</pre></div><div class="ln_tab_content" name="HSP_tab">
<pre class="brush: cpp">#include "lnote.as"
	// LightNote の初期化
	LCore_Initialize
	
	// メインループ
*main
	// ここにゲームのメイン処理を書きます
		
	LCore_Update
	if (stat == LN_FALSE) : goto *exit
	goto *main
	
*exit
	// LightNote の終了処理
	LCore_Terminate
</pre></div></div>@endhtmlonly
    <br>
	最初の Initialize はウィンドウの生成や描画機能の初期化を行い、LightNote を使用できる状態にします。<br>
	通常、プログラムの先頭で1度だけ呼び出します。<br>
	<br>
	@ref modules1_initialize1_anchor2 "初期設定" を特に行わなければ、640*480のウィンドウを作成し 60fps で画面更新を行うようになります。<br>
	<br>
	<br>
	次のメインループと Update により、1 フレームごとの画面クリア・描画、入力状態等の更新が行われます。<br>
	また、1 ループ 60fps に近づけるように自動的に待ち時間を取ります。<br>
	<br>
	IsEndRequested は、ウィンドウのクローズボタンが押された等、アプリケーションを終了するべき時に true を返します。<br>
	このプログラムではその条件で、メインループを終了させています。<br>
	<br>
	<br>
	最後の Terminate で、LightNote のリソースをすべて解放し、使用を終了します。<br>
	<br>
	<br>
	
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor	modules1_initialize1_anchor2
    @par	初期設定を行う
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>
	
	@if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_initialize1_anchor3
    @par	必要な機能だけ個別に初期化する
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
	<br>
	<br>
	
*/
//=============================================================================