
#include "stdafx.h"

/*
// 読み込みを別スレッドで行うパターン

GameScene
	main
		_battlerManager.beginGameScene();				// ModelData(Mesh?) とか非同期でもいいかも
		_view = new GameView();							// LNote の非同期読み込みを使う
		_bw = new BackGroundWorker(() = > {いろいろ});	// 自分でスレッドを起こす
		Progress.Show();	// progressEffect = new ProgressEffect();	// セルアニメまたは Effect
		do
		{
		} while (Manager.Update() && _view.Loaded && _battlerManager.Loaded && _bw.Finished)
		Progress.Hide();	// progressEffect.Dispose

		// メインループ
		do
		{
			引き続き非同期ロードされるものもあるが、(PSO2 のプレイヤーモデルとか)
				それはあくまで「視覚的」に必要なもの。
				当たり判定とか、サイズとか、ゲームロジック的に必要なものはすべてロード済み。

		} while (Manager.Update())


Indicator
// 読み込みをメインスレッドで行うパターン

GameScene
	main
		BackGroundProgress.Begin();

		Tex.Load();
		bload();

		BackGroundProgress.End();;

// ↑ダメ。メインスレッドのメッセージループが回っていない。
// Ruby とかなら何とかなるけど、HSP は最初からUIスレッドになっている。
	*/
