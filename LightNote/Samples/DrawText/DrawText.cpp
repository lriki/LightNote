//==============================================================================
// �e�N�X�`���ւ̕�����`��T���v��
//------------------------------------------------------------------------------
/*  
*/
//==============================================================================

#include <lnote.h>

int main()
{
	// �C�ӂ̃t�H���g���V�X�e���ɃC���X�g�[�������Ɏg�p����ɂ́A
	// LConfig_RegisterFontFile() �Ńt�H���g�t�@�C����o�^���܂��B
	LNConfig_RegisterFontFile( "../Media/VL-PGothic-Regular.ttf" );

	LNCore_Initialize();

	// ��������������ރe�N�X�`���ƕ\���p�X�v���C�g������
	lnHandle texture;
	lnHandle sprite;
	LNTexture_Create(&texture, 640, 480);
	LNSprite_Create(&sprite, texture);
	
	lnRect rect = { 16, 16, 640, 480 };

	// �t�H���g���w�肹���� DrawText �����ꍇ�̓f�t�H���g�t�H���g���g����
	LNTexture_DrawText( texture, "�f�t�H���g�t�H���g", &rect, NULL, -1, LN_TEXTALIGN_LEFT );
	rect.Y += 32;

	// �f�t�H���g�t�H���g�̖��O��`�悵�Ă݂�
	lnHandle font1;
	const char* name;
	LNFont_Create(&font1);
	LNFont_GetName(font1, &name);
	LNTexture_DrawText(texture, name, &rect);
	rect.Y += 32;

	// �V�X�e���ɃC���X�g�[�����ꂽ�t�H���g���g��
	lnHandle font2;
	LNFont_Create(&font2);
	LNFont_SetName(font2, "�l�r �o����");
	LNTexture_DrawText(texture, "�V�X�e���ɃC���X�g�[�����ꂽ�t�H���g", &rect, font2);
	rect.Y += 32;

	// �C���X�g�[������Ă��Ȃ��C�ӂ̃t�H���g
	lnHandle font3;
	LNFont_Create(&font3);
	LNFont_SetName(font3, "VL PGothic");
	LNTexture_DrawText(texture, "�C���X�g�[������Ă��Ȃ��C�ӂ̃t�H���g", &rect, font3);
	rect.Y += 32;

	// �񑾎�
	LNFont_SetBold( font1, LN_FALSE );
	LNTexture_DrawText( texture, "Non Bold (default)", &rect, font1 );
	rect.Y += 32;

	// ���� & �Α�
	LNFont_SetBold( font1, LN_TRUE );
	LNFont_SetItalic( font1, LN_TRUE );
	LNTexture_DrawText( texture, "Bold and Italic", &rect, font1 );
	rect.Y += 32;

	// �t�H���g�F
	lnColor color1 = { 255, 64, 32, 255 };
	LNFont_SetColor( font3, &color1 );
	LNTexture_DrawText( texture, "�t�H���g�F", &rect, font3 );

	// ���ߐF
	lnColor color2 = { 48, 192, 24, 127 };
	LNFont_SetColor( font1, &color2 );
	LNFont_SetSize( font1, 30 );
	rect.X += 64;
	LNTexture_DrawText( texture, "���ߐF", &rect, font1 );
	rect.X -= 64;
	rect.Y += 32;

	// �A���`�G�C���A�X����
	lnHandle font4;
	LNFont_Create(&font4);
	LNFont_SetName( font4, "VL PGothic" );
	LNFont_SetAntiAlias( font4, LN_FALSE );
	LNTexture_DrawText( texture, "�A���`�G�C���A�X���� (�f�t�H���g�͗L��)", &rect, font4 );
	rect.Y += 32;

	// �ܕ���
	lnColor color3 = { 192, 255, 127, 255 };
	lnColor color4 = { 24, 72, 255, 255 };
	lnHandle font5;
	LNFont_Create(&font5);
	LNFont_SetName( font5, "VL PGothic" );
	LNFont_SetSize( font5, 30 );
	LNFont_SetColor( font5, &color3 );
	LNFont_SetEdgeSize( font5, 1 );
	LNFont_SetEdgeColor( font5, &color4 );
	LNTexture_DrawText( texture, "����", &rect, font5 );
	rect.Y += 36;

	// �`�悷�邽�߂ɕK�v�ȗ̈�̃T�C�Y���擾���n�C���C�g�\������
	lnSize size;
	LNFont_GetTextSize( font5, "�`��T�C�Y�Ńn�C���C�g(�M�E�ցE�L)", -1, &size );
	LNTexture_DrawText( texture, "�`��T�C�Y�Ńn�C���C�g(�M�E�ցE�L)", &rect, font5 );

	lnHandle texture2;		// �n�C���C�g�p�e�N�X�`��
	lnHandle sprite2;		// �n�C���C�g�p�X�v���C�g
	lnColor color5 = { 255, 255, 48, 127 };
	LNTexture_Create(&texture2, size.Width, size.Height);
	LNTexture_Clear( texture2, &color5 );
	LNSprite_Create(&sprite2, texture2);
	LNSceneNode_SetPositionXYZ(sprite2, rect.X, rect.Y, 0);

	// ���C�����[�v
	lnBool endRequested;
	do
	{
		LNCore_Update();
		LNCore_IsEndRequested(&endRequested);
	} while (endRequested == LN_FALSE);

	LNCore_Terminate();
	return 0;
}