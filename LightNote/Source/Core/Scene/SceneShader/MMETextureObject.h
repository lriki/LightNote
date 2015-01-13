//==============================================================================
// MMETextureObject 
//------------------------------------------------------------------------------
///**
//  @file       MMETextureObject.h
//  @brief      MMETextureObject
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../Graphics/Interface.h"
#include "../../Graphics/DeviceObjects.h"
#include "MMETypes.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� MMETextureObject
//------------------------------------------------------------------------------
///**
//  @brief      
//  @note
//              Manager �ŊǗ������B
//              �x�[�X�̃r���[�T�C�Y���ύX�����΁AManager �͂��ׂĂ�
//              �C���X�^���X�ɑ΂��� refresh ���ĂԁB
//              shared �C���q�t���ŃV�F�[�_�ϐ�����`����A����ɃA�m�e�[�V�������t���Ă���
//              (�e�N�X�`���쐬��񂪂���=�e�N�X�`�����쐬����)�ꍇ��
//              ���ׂẴC���X�^���X�� equalTextureVariable() �œ����E���Z�}���e�B�N�X��
//              ���ɍ쐬����Ă��Ȃ������ׂāA�쐬����Ă�����G���[�Ƃ���B
//
//				�Q�ƃJ�E���g�� SceneShader �����삷��B
//*/
//==============================================================================
class MMETextureObject
    : public Base::ReferenceObject
{
public:
	MMETextureObject( SceneShaderManager* manager );
	virtual ~MMETextureObject();

public:
    
	/// ������ (���̌�� refresh() ���ĂԂ���)
    void initialize(
		SceneShader*		ownerShader,
		MMEVariableRequest	request,
        const lnRefString&  resourceName,
        const LVector3&     dimensions,
        const LVector2&     viewportRatio,
        int                 miplevels,
        Graphics::SurfaceFormat     format,
        bool                antiAlias );
    
	/// �e�N�X�`�����\�[�X�̍č쐬
    void refresh( const LVector2& viewSize );
    
	/// �e�N�X�`���𐶐������V�F�[�_�ϐ������l�̒�`���𒲂ׂ� (shared �p)
    bool equalTextureVariable( const MMETextureObject* textureObject );

	/// �e�N�X�`���̎擾
	Graphics::Texture* getTexture() { return mTexture; }

private:
	void _dispose();
     
private:
    
	SceneShaderManager*	mManager;
	SceneShader*		mOwnerShader;
    Graphics::Texture*	mTexture;
    lnRefString			mVariableName;          ///< �e�N�X�`���^�̕ϐ���
    MMESemantics		mVariableSemantics;     ///< �e�N�X�`���^�ϐ��̃Z�}���e�B�N�X

	MMEVariableRequest	mRequest;
    lnRefString         mResourceName;
    LVector3            mDimensions;
	LVector2			mViewportRatio;
    int                 mMiplevels;
    Graphics::SurfaceFormat     mFormat;
    bool                mAntiAlias;

};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================