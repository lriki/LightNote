//==============================================================================
// テクスチャへの文字列描画サンプル
//------------------------------------------------------------------------------
/*  
*/
//==============================================================================

#include <lnote.h>

int main()
{
	// 任意のフォントをシステムにインストールせずに使用するには、
	// LConfig_RegisterFontFile() でフォントファイルを登録します。
	LNConfig_RegisterFontFile( "../Media/VL-PGothic-Regular.ttf" );

	LNCore_Initialize();

	// 文字列を書き込むテクスチャと表示用スプライトを準備
	lnHandle texture;
	lnHandle sprite;
	LNTexture_Create(&texture, 640, 480);
	LNSprite_Create(&sprite, texture);
	
	lnRect rect = { 16, 16, 640, 480 };

	// フォントを指定せずに DrawText した場合はデフォルトフォントが使われる
	LNTexture_DrawText( texture, "デフォルトフォント", &rect, NULL, -1, LN_TEXTALIGN_LEFT );
	rect.Y += 32;

	// デフォルトフォントの名前を描画してみる
	lnHandle font1;
	const char* name;
	LNFont_Create(&font1);
	LNFont_GetName(font1, &name);
	LNTexture_DrawText(texture, name, &rect);
	rect.Y += 32;

	// システムにインストールされたフォントを使う
	lnHandle font2;
	LNFont_Create(&font2);
	LNFont_SetName(font2, "ＭＳ Ｐ明朝");
	LNTexture_DrawText(texture, "システムにインストールされたフォント", &rect, font2);
	rect.Y += 32;

	// インストールされていない任意のフォント
	lnHandle font3;
	LNFont_Create(&font3);
	LNFont_SetName(font3, "VL PGothic");
	LNTexture_DrawText(texture, "インストールされていない任意のフォント", &rect, font3);
	rect.Y += 32;

	// 非太字
	LNFont_SetBold( font1, LN_FALSE );
	LNTexture_DrawText( texture, "Non Bold (default)", &rect, font1 );
	rect.Y += 32;

	// 太字 & 斜体
	LNFont_SetBold( font1, LN_TRUE );
	LNFont_SetItalic( font1, LN_TRUE );
	LNTexture_DrawText( texture, "Bold and Italic", &rect, font1 );
	rect.Y += 32;

	// フォント色
	lnColor color1 = { 255, 64, 32, 255 };
	LNFont_SetColor( font3, &color1 );
	LNTexture_DrawText( texture, "フォント色", &rect, font3 );

	// 透過色
	lnColor color2 = { 48, 192, 24, 127 };
	LNFont_SetColor( font1, &color2 );
	LNFont_SetSize( font1, 30 );
	rect.X += 64;
	LNTexture_DrawText( texture, "透過色", &rect, font1 );
	rect.X -= 64;
	rect.Y += 32;

	// アンチエイリアス無効
	lnHandle font4;
	LNFont_Create(&font4);
	LNFont_SetName( font4, "VL PGothic" );
	LNFont_SetAntiAlias( font4, LN_FALSE );
	LNTexture_DrawText( texture, "アンチエイリアス無効 (デフォルトは有効)", &rect, font4 );
	rect.Y += 32;

	// 袋文字
	lnColor color3 = { 192, 255, 127, 255 };
	lnColor color4 = { 24, 72, 255, 255 };
	lnHandle font5;
	LNFont_Create(&font5);
	LNFont_SetName( font5, "VL PGothic" );
	LNFont_SetSize( font5, 30 );
	LNFont_SetColor( font5, &color3 );
	LNFont_SetEdgeSize( font5, 1 );
	LNFont_SetEdgeColor( font5, &color4 );
	LNTexture_DrawText( texture, "文字", &rect, font5 );
	rect.Y += 36;

	// 描画するために必要な領域のサイズを取得→ハイライト表示する
	lnSize size;
	LNFont_GetTextSize( font5, "描画サイズでハイライト(｀・ω・´)", -1, &size );
	LNTexture_DrawText( texture, "描画サイズでハイライト(｀・ω・´)", &rect, font5 );

	lnHandle texture2;		// ハイライト用テクスチャ
	lnHandle sprite2;		// ハイライト用スプライト
	lnColor color5 = { 255, 255, 48, 127 };
	LNTexture_Create(&texture2, size.Width, size.Height);
	LNTexture_Clear( texture2, &color5 );
	LNSprite_Create(&sprite2, texture2);
	LNSceneNode_SetPositionXYZ(sprite2, rect.X, rect.Y, 0);

	// メインループ
	lnBool endRequested;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}