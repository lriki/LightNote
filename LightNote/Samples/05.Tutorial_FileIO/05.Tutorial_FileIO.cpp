//==============================================================================
// ファイル入出力のチュートリアル
//------------------------------------------------------------------------------
/*  
	単純なファイルの読み込み・書き込みであれば、
	fopen() 等の標準機能と比べてできることに大差はありません。

	LightNote のファイル操作系機能を使用すると、LightNote 独自の
	暗号化アーカイブからファイルを読み込むことができます。

	・暗号化アーカイブとは？
		リリースしたゲーム作品に含まれる画像や音声データ等が外から読めないように、
		ひとつのリソースファイルとしてまとめ、暗号化する機能です。

		作成には付属のツールを使用し、パスワードで鍵を掛けます。
		使用するときは LNConfig_RegisterArchive にファイル名とパスワードをセットします。
		
		暗号化アーカイブの作成手順…
		① Tools/ArchiveMaker.exe に、アーカイブ化したいフォルダをD&Dします。
		② パスワードの入力を求められるので、入力してEnterを押します。
		③ D&Dしたフォルダと同じ場所にアーカイブファイルが生成されます (拡張子 .lna)
		実際の使用方法は本プログラムをご覧ください。
*/
//==============================================================================

#include <lnote.h>

int main()
{
	// 暗号化アーカイブを登録します。
	// この場所に SampleArchive というフォルダがあるかのように、
	// アーカイブ内のファイルにアクセスできるようになります。
	LNConfig_RegisterArchive( "../Media/SampleArchive.lna", "test" );

	// LNote 初期化
	LNCore_Initialize();

	// アーカイブ内のファイルを開きます。
	LNHandle file1;
	LNFile_Create(&file1, "../Media/SampleArchive/TextFile.txt", LN_FILEMODE_OPEN, LN_FILEACCESS_READ);

	// ファイルサイズを取得します。
	int size;
	LNFile_GetSize(file1, &size);

	// ファイルの内容を全て data に読み込みます。
	char data[1000] = { 0 };
	int validSize;
	LNFile_Read(file1, (lnByte*)data, size, &validSize);

	// ファイルを閉じます。
	LNFile_Close( file1 );

	// 読み込んだ内容をテクスチャに描画します。
	LNHandle texture1;
	LNRect rect1 = { 0, 0, 640, 480 };
	LNTexture_Create(&texture1, 640, 480);
	LNTexture_DrawText(texture1, data, &rect1);



	// アーカイブ内の画像ファイルを読み込み、テクスチャを作成します。
	// テクスチャに限らず音声等も、ファイルアクセスを伴う関数はすべて
	// 登録されたアーカイブの中を探しに行きます。
	LNHandle texture2;
	LNTexture_CreateFromFile(&texture2, "../Media/SampleArchive/20140309.png");

	// テクスチャを表示するためのスプライトの用意
	LNHandle sprite1;
	LNHandle sprite2;
	LNSprite_Create(&sprite2, texture2);
	LNSprite_Create(&sprite1, texture1);
	LNVisualNode_SetOpacity( sprite2, 0.15 );

	// メインループ
	LNBool flag;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&flag);
	} while (flag == LN_FALSE);

	LNCore_Terminate();
	return 0;
}