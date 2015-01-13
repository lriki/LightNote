//==============================================================================
// DrawCommand 
//------------------------------------------------------------------------------
///**
//  @file       DrawCommand.h
//  @brief      DrawCommand
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <stack>
#include <vector>
#include "Common.h"
#include "SceneShader/MMETypes.h"
#include "../Graphics/Interface.h"
#include "../Graphics/DeviceObjects.h"


namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� DrawingCommandList
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class DrawingCommandList
{
public:
	DrawingCommandList();
	~DrawingCommandList();

public:

	/// ������
	void initialize( SceneShader* ownerShader );

	/// Draw=Geometry �ɂĕ`�悳��� SceneNode ��ݒ肷��
	/// (Pass= �ɂ���Đݒ肳��Ă���R�}���h���X�g�ɑ΂��Ă��ݒ肳���)
	void setOwnerVisualNodeContext( VisualNodeContext* context );

	/// �`�悷��T�u�Z�b�g�ԍ��̔z���ݒ肷��
	/// (Pass= �ɂ���Đݒ肳��Ă���R�}���h���X�g�ɑ΂��Ă��ݒ肳���)
	void setDrawingSubsetNumbers( const std::vector<int>& numbers );

	/// ���̃R�}���h���X�g�����s����
	void execute( ContextDrawParam& param );

	/// ScriptExternal=Color �܂Ŏ��s����
	void executePreScriptExternal_Color( ContextDrawParam& param );

	/// ScriptExternal=Color ������s����
	void executePostScriptExternal_Color( ContextDrawParam& param );

	/// �X�N���v�g�������������m�F����
	bool checkValid( MMEScriptClass scriptClass, MMEScriptOrder scriptOrder, bool onTech );

public:

	/// �����_�����O�^�[�Q�b�g��ݒ肷��
	/// (texture �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	//void add_RenderColorTarget( int index, Graphics::ITexture* texture );

	/// �����_�����O�^�[�Q�b�g��ݒ肷��
	/// (textureVariable �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	void add_RenderColorTarget( int index, Graphics::IShaderVariable* textureVariable );

	/// �[�x�o�b�t�@��ݒ肷��
	/// (texture �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	//void add_RenderDepthStencilTarget( Graphics::ITexture* texture );

	/// �����_�����O�^�[�Q�b�g��ݒ肷��
	/// (textureVariable �� NULL �ɂ���ƁA�f�t�H���g(�R�}���h�J�n��)�Ƀ��Z�b�g)
	void add_RenderDepthStencilTarget( Graphics::IShaderVariable* textureVariable );

	/// �����_�����O�^�[�Q�b�g���N���A����F��ݒ肷��
	void add_ClearSetColor( const LVector4& color );		// TODO: ���̂�����AGraphics::IShaderVariable ���� getVector ����悤�ɕύX

	/// �[�x�o�b�t�@���N���A����Z�l��ݒ肷��
	void add_ClearSetDepth( lnFloat depth );

	/// �����_�����O�^�[�Q�b�g���N���A����
	void add_ClearColor();

	/// �[�x�X�e���V���T�[�t�F�C�X���N���A����
	void add_ClearDepth();

	/// ScriptExternal=Color
	void add_ScriptExternal_Color();

	/// �w�肵���p�X�����s���� 
	/// (���̕`��R�}���h�����s����BcommandList �� NULL �ɂ���ƁADraw=Geometry �݂̂����s����)
	void add_Pass( Graphics::IShaderPass* pass, DrawingCommandList* commandList );

	/// �w�肵���񐔂����A�X�N���v�g�̈ꕔ�����[�v����
	void add_LoopByCount( int count );

	/// ���[�v�I�[
	void add_LoopEnd();

	/// ���[�v���̃��[�v�J�E���^�̒l���A�w�肵���p�����[�^�ɐݒ肷��
	void add_LoopGetIndex( Graphics::IShaderVariable* variable );

	/// �I�u�W�F�N�g��`�悷��
	void add_DrawGeometry( Graphics::IShaderPass* pass );

	/// �����_�����O�^�[�Q�b�g�̃X�N���[���ƈ�v����T�C�Y�́A�����`�̃|���S����`�悷��
	void add_DrawBuffer( Graphics::IShaderPass* pass );

private:

	/// ���̃R�}���h���X�g�����s���� (�߂�l�͍Ō�Ɏ��s�����R�}���h���w���v���O�����J�E���^)
	int _executeInternal( ContextDrawParam& param, int programCounter, int currentLoopCount );

	/// Draw=Geometry
	void _drawGeometry( ContextDrawParam& param, Graphics::IShaderPass* pass );

	/// ���݂̃����_�����O�^�[�Q�b�g���̏�Ԃ��L��
	void _pushCurrentState( ContextDrawParam& param );

	/// �L��������Ԃ𕜋A
	void _popCurrentState( ContextDrawParam& param );

private:

	/// �`��R�}���h�̎��
	enum CommandType
	{
		COMMAND_Unknown = 0,
		COMMAND_RenderColorTarget,
		COMMAND_RenderDepthStencilTarget,
		COMMAND_ClearSetColor,
		COMMAND_ClearSetDepth,
		COMMAND_ClearColor,
		COMMAND_ClearDepth,
		COMMAND_ScriptExternal_Color,
		COMMAND_Pass,
		COMMAND_LoopByCount,
		COMMAND_LoopEnd,
		COMMAND_LoopGetIndex,
		COMMAND_DrawGeometry,
		COMMAND_DrawBuffer,
	};

	/// �`��R�}���h
	struct Command
	{
		CommandType		Type;
		union
		{
			struct
			{
				int							Index;
				Graphics::IShaderVariable*	TextureVariable;
				//Graphics::ITexture*		Texture;
			} RenderColorTarget;

			struct
			{
				Graphics::IShaderVariable*	TextureVariable;
				//Graphics::ITexture*		Texture;
			} RenderDepthStencilTarget;

			struct
			{
				lnFloat						Color[4];
			} ClearSetColor;

			struct
			{
				lnFloat						Depth;
			} ClearSetDepth;

			struct
			{
				Graphics::IShaderPass*		Pass;
				DrawingCommandList*			CommandList;
			} Pass;

			struct
			{
				int							Count;
			} LoopByCount;

			struct
			{
				Graphics::IShaderVariable*	Variable;
			} LoopGetIndex;

			struct
			{
				Graphics::IShaderPass*		Pass;
			} DrawGeometry;

			struct
			{
				Graphics::IShaderPass*		Pass;
			} DrawBuffer;
		};
	};

	typedef std::vector<Command>				CommandArray;
	typedef std::vector<DrawingCommandList*>	ChildCommandListArray;

private:

	SceneShader*				mOwnerShader;
	CommandArray				mCommandArray;
	ChildCommandListArray		mChildCommandListArray;
	VisualNodeContext*			mOwnerVisualNodeContext;
	std::vector<int>			mDrawingSubsetNumbers;
	DrawingCommandList*			mParentList;
	int							mScriptExternalColorPosition;

	LColorF						mClearColor;
	lnFloat						mClearDepth;

	//Graphics::IRenderer*        mRenderer;
 //   Graphics::GeometryRenderer* mGeometryRenderer;
	Graphics::Texture*			mOldRenderTarget[Graphics::MaxMultiRenderTargets];
	Graphics::Texture*			mOldDepthBuffer;
	LColorF						mOldClearColor;
	lnFloat						mOldClearDepth;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================