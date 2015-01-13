//==============================================================================
// Viewport 
//------------------------------------------------------------------------------
///**
//  @file       Viewport.h
//  @brief      Viewport
//  @author     Riki
//*/
//==============================================================================

#pragma once

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "../../Animation/FadeValue.h"
//#include "../../Graphics/Common/Model/Model.h"
#include "VisualNode.h"

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Scene
{
class ViewportContext;

//==============================================================================
// �� Viewport �N���X
//------------------------------------------------------------------------------
///**
//  @brief		
//*/
//==============================================================================
class Viewport
    : public VisualNode
{
public:

	/// �R���X�g���N�^
    Viewport( SceneGraph* scene_ );

	/// �f�X�g���N�^
    virtual ~Viewport();

public:

	/// �쐬
    void create( const Geometry::Size& size_, Graphics::SpriteRenderer* spriteRenderer );

    

	/// �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_��ǉ����� (priority_ �͒l���傫��������ɏ��������BScriptOrder �� preprocess �܂��� postprocess ���w�肳��Ă���V�F�[�_�̂ݒǉ��\)
    void addPostEffectShader( SceneShader* shader_, int priority_ );

	/// �v���v���Z�X�E�|�X�g�v���Z�X�p�̃V�F�[�_���O��
    void removePostEffectShader( SceneShader* shader_ );

	/// �F���̐ݒ�
    void setTone( const LTone& tone_, double duration_ );

	/// �G�t�F�N�g�K�p�����𔻒肷��
    bool isEffect() const { return mTone.isFinished(); }

	/// ���� Viewport �ȉ��� Sprite �`��Ɏg�p���� SpriteRenderer
	Graphics::SpriteRenderer* getSpriteRenderer() { return mSpriteRenderer; }

public:

	/// �`��R�}���h���K�w�I�ɍ쐬����
	//virtual void makeDrawCommand( DrawCommandContainer* container_, MMDPass pass_ );

	/// �m�[�h�̎�ނ̎擾
    virtual LNSceneNodeType getSceneNodeType() const { return LN_SCENENODE_VIEWPORT; }

	/// �t���[���X�V
    virtual void update( float elapsedTime );

private:

    

private:

    struct PPShader
    {
        int          Priority;
        SceneShader* Shader;

        bool operator() ( const PPShader& e_ ) const { return e_.Shader == Shader; }
    };

    typedef std::vector< PPShader >    PPShaderArray;

	/// �\�[�g�p�̔�r�֐� (�~��)
    static bool _cmpPPShader( const PPShader& l_, const PPShader& r_ ) { return l_.Priority >= r_.Priority; }

private:
	friend class ViewportContext;
    PPShaderArray                       mPPShaderArray;
    Animation::FadeValue< LVector4, lnFloat >    mTone;
    LVector2                            mViewSize;
	Graphics::SpriteRenderer*	mSpriteRenderer;

    

};

//==============================================================================
// �� ViewportContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class ViewportContext
	: public VisualNodeContext
{
public:
    ViewportContext();
    virtual ~ViewportContext();
    
public:
	void initialize( Viewport* viewport, Graphics::SpriteRenderer* spriteRenderer );
	virtual void render( ContextDrawParam& param );

private:
	Viewport*					mViewport;
	Graphics::SpriteRenderer*	mSpriteRenderer;
	//Viewport::PPShaderArray	mPPShaderArray;
};

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================