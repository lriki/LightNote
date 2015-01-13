//==============================================================================
// 音声の再生チューリアル
//------------------------------------------------------------------------------
/*  
	音声の再生は LNSound または LNAudio モジュールで行います。

	・LNSound
		音声データをサウンドハンドルを通して操作します。
		ハンドルの生成・破棄等の管理を自分で行う必要がありますが、
		ひとつの音声に対して一時停止や再生位置の取得等、細かい操作が可能です。

	・LNAudio
		ゲーム向けのユーティリティです。
		BGM、SE 等の用途に合わせて、音声ファイルを簡単に再生することができます。
		LNSound とは異なり、サウンドハンドルを扱わず、ファイル名を直接指定して再生します。
		(インターフェイスは RGSS(RPGツクール)の Audio モジュールをイメージしています)

	対応ファイルフォーマットは WAVE(.wav)、MP3(.mp3)、Ogg Vorbis(.ogg)、MIDI(.mid) です。
	このうち Ogg Vorbis と MIDI は以下のルールでループ位置を認識します。
	・Ogg Vorbis
		コメントとして LOOPSTART、LOOPLENGTH という値が埋め込まれていると、
		その数値に対応するサンプル位置をループの範囲として認識します。
	・MIDI
		MIDI データ中にコントロールチェンジの 111 番があると、
		曲を最後まで演奏したあとのループ位置の目印として認識します。
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// ファイルから音声データを作成し、その音声を示すサウンドハンドルを取得します。
	LNHandle sound1;
	LNSound_Create(&sound1, "../Media/cursor1.wav");

	// メインループ
	LNBool flag;
	do
	{
		// ← キー : 再生 (LNSound)
		LNInput_IsTrigger(LN_BUTTON_LEFT, &flag);
		if (flag) {
			LNSound_Play(sound1);
		}
		// → キー : 停止 (LNSound)
		LNInput_IsTrigger(LN_BUTTON_RIGHT, &flag);
		if (flag) {
			LNSound_Stop(sound1);
		}

		// Z キー : BGM 再生 (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_0, &flag);
		if (flag) {
			LNAudio_PlayBGM("../Media/ln22.ogg", 100, 100, 0);
		}
		// X キー : BGM 停止 (フェードアウト) (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_1, &flag);
		if (flag) {
			LNAudio_StopBGM(1000);
		}

		// A キー : BGS 再生 (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_3, &flag);
		if (flag) {
			LNAudio_PlayBGS("../Media/water02.wav", 50, 100, 0);
		}
		// S キー : BGS 停止 (フェードアウト) (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_4, &flag);
		if (flag) {
			LNAudio_StopBGS(1000);
		}

		// Q キー : SE 再生 (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_6, &flag);
		if (flag) {
			LNAudio_PlaySE("../Media/cursor1.wav", 50, 100);
		}
		// W キー : SE 停止 (LNAudio)
		LNInput_IsTrigger(LN_BUTTON_7, &flag);
		if (flag) {
			LNAudio_StopSE();
		}

		LNCore_Update();
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}