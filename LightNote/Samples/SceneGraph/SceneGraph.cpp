//==============================================================================
// シーングラフのサンプル
//------------------------------------------------------------------------------
/*  
	シーングラフとは、画面に表示するオブジェクトを階層構造にまとめたものです。
	以下のような要素によって構成されます。

	・ノード
		シーングラフに配置する全てのオブジェクトのベースクラスです。
		位置や回転角度等の姿勢情報を持ち、複数の子ノードを持つことができます。

	・Visual ノード	(base:ノード)
		Sprite 等、実際に画面に表示されるオブジェクトの基本クラスです。
		色等のマテリアル情報や合成方法等、描画に必要な基本的な情報を持ちます。
		ノードの機能を継承しており、ノードの持つすべての機能を使用することができます。

	・Sprite ノード	(base:Visualノード)
		2D画像を表示するためのノードです。
		サンプル「02.Tutorial_Sprite」等で使用しているスプライトはこれにあたります。
		Visualノードの機能を継承しており、Visualノードと、さらにそのベースクラスである
		ノードの機能を使用することができます。

	実際の構成例は以下のようになります。
	
	- ルートノード
		- 3D ルートノード
			- Sprite1
			- Effect1
		- 2D ルートノード
			- Sprite2
				- Sprite3
			- Effect2

	各オブジェクトは生成時に、3D または 2D ルートノードの子ノードとして
	自動的にシーングラフに追加されます。
	例えば、LSprite_Create() で作成された Sprite ノードは 2D ルートノード、
	LSprite_Create3D() で作成された Sprite ノードは 3D ルートノードに追加されます。

	子ノードは親ノードの姿勢情報を引き継ぎます。
	↑の構成例では、「Sprite2」の座標を設定すると、それに追随して「Sprite3」も
	移動するようになります。
*/
//==============================================================================

#include <lnote.h>

int main()
{
	LNCore_Initialize();

	// エフェクトを見易くするため背景を黒にする
	LNColorF color1 = { 0.0, 0.0, 0.0, 1.0 };
	LNHandle viewPane;
	LNViewPane_GetDefaultViewPane(&viewPane);
	LNViewPane_SetBackgroundColor(viewPane, &color1);

	// ノード①
	LNHandle sun;
	LNEffect_Create(&sun, "../Media/Sun1.efk");
	LNEffect_Play(sun, LN_TRUE);

	// ノード①を周回するノード② 
	LNHandle ice;
	LNEffect_Create(&ice, "../Media/Ice1.efk");
	LNEffect_Play(ice, LN_TRUE);
	LNSceneNode_AddChild( sun, ice );	
	LNSceneNode_SetPositionXYZ(ice, 0, 0, 10);

	// ノード②を周回するノード③
	LNHandle starTex1;
	LNHandle star1;
	LNTexture_CreateFromFile(&starTex1, "../Media/Spark1.png");
	LNSprite_Create(&star1, starTex1);
	LNSprite_SetSize( star1, 2, 2 );
	LNVisualNode_SetBlendMode(star1, LN_BLEND_ADD);
	LNVisualNode_SetEnableDepthWrite(star1, LN_FALSE);
	LNSceneNode_SetPositionXYZ(star1, 3, 0, 0);
	LNSceneNode_SetPlacementType(star1, LN_PLACEMENTTYPE_BILLBOARD);
	LNSceneNode_AddChild(ice, star1);

	// カメラの位置を少し離れたところにする
	LNHandle camera;
	LNCamera_GetDefault3DCamera(&camera);
	LNCamera_SetPositionXYZ(camera, 25, 12, -25);

	// メインループ
	LNBool endRequested;
	float angle = 0.0;
	do
	{
		// 周期をずらして、①と②をそれぞれ回転させる
		LNSceneNode_SetAngleXYZ(sun, 0, angle, 0);
		LNSceneNode_SetAngleXYZ(ice, 0, angle * 2, 0);

		angle -= 0.01;

		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}