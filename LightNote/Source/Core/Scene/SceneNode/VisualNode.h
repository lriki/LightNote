//==============================================================================
// VisualNode 
//------------------------------------------------------------------------------
///**
//  @file       VisualNode.h
//  @brief      VisualNode
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "SceneNode.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// �� VisualNode
//------------------------------------------------------------------------------
///**
//  @brief	
//*/
//==============================================================================
class VisualNode
    : public SceneNode
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
    VisualNode( SceneGraph* scene );
    virtual ~VisualNode();

public:

	/// �쐬
    void create( int subsetCount, LNDrawCoordSpace cs, SceneNodeContext* context );

public:
#if 0

	/// �s�����x�̐ݒ�
	void setOpacity( lnFloat opacity, lnU32 subsetIndex = 0 )			{ mRenderer->setOpacity( opacity, subsetIndex ); }

	/// �s�����x�̎擾
	lnFloat getOpacity( lnU32 subsetIndex = 0 ) const					{ return mRenderer->getOpacity( subsetIndex ); }

	/// ��Z�F�̐ݒ�
    void setColorScale( const LColorF& color, lnU32 subsetIndex = 0 )	{ mRenderer->setColorScale( color, subsetIndex ); }

	/// ��Z�F�̎擾
    const LColorF& getColorScale( lnU32 subsetIndex = 0 ) const			{ return mRenderer->getColorScale( subsetIndex ); }

	/// �u�����h�J���[�̐ݒ�
    void setBlendColor( const LColorF& color, lnU32 subsetIndex = 0 )	{ mRenderer->setBlendColor( color, subsetIndex ); }

	/// �u�����h�J���[�̎擾
    const LColorF& getBlendColor( lnU32 subsetIndex = 0 ) const			{ return mRenderer->getBlendColor( subsetIndex ); }

	/// �F���̐ݒ�
    void setTone( const LTone& tone, lnU32 subsetIndex = 0 )			{ mRenderer->setTone( tone, subsetIndex ); }

	/// �F���̎擾
    const LTone& getTone( lnU32 subsetIndex = 0 ) const					{ return mRenderer->getTone( subsetIndex ); }

	/// UV �ϊ��s��̐ݒ�
	void setUVTransform( const LMatrix& matrix, lnU32 subsetIndex = 0 )	{ mRenderer->setUVTransform( matrix, subsetIndex ); }

	/// UV �ϊ��s��̐ݒ�
    const LMatrix& getUVTransform( lnU32 subsetIndex = 0 )  const		{ return mRenderer->getUVTransform( subsetIndex ); }

	/// �������@�̐ݒ�
	void setBlendMode( LNBlendMode mode )				{ mRenderer->setBlendMode( mode ); }

	/// �������@�̎擾
	LNBlendMode getBlendMode() const					{ return mRenderer->getBlendMode(); }

	/// �[�x�e�X�g�̗L���ݒ�
    void setEnableDepthTest( bool enabled )				{ mRenderer->setEnableDepthTest( enabled ); }

	/// �[�x�e�X�g�̗L������
    bool isEnableDepthTest() const						{ return mNodeRenderParam.RenderState.DepthTest; }

	/// �[�xWrite�̗L���ݒ�
    void setEnableDepthWrite( bool enabled )			{ mRenderer->setEnableDepthWrite( enabled ); }

	/// �[�x�������݂̗L������
    bool isEnableDepthWritet() const					{ return mNodeRenderParam.RenderState.DepthWrite; }

	/// �J�����O���@�̐ݒ�
    void setCullingMode( LNCullingMode mode )			{ mRenderer->setCullingMode( mode ); }

	/// �J�����O���@�̎擾
    LNCullingMode getCullingMode() const				{ return mNodeRenderParam.RenderState.Culling; }

	/// �V�F�[�_�̐ݒ�
    void setShader( SceneShader* shader );

	/// �V�F�[�_�̎擾
	SceneShader* getShader()							{ return mShader; }

//////
#else
	/// ����Ԃ̐ݒ�
    void setVisible( bool flag_ )                   { mIsVisible = flag_; }

	/// ����Ԃ̔���
    bool isVisible() const                          { return mIsVisible; }

	/// �s�����x�̐ݒ�
    void setOpacity( lnFloat opacity_, lnU32 subset_idx_ = 0 )             { mNodeRenderParam.Subset[ subset_idx_ ].Opacity = opacity_; }//mOpacity = opacity_; }

	/// �s�����x�̎擾
    lnFloat getOpacity( lnU32 subset_idx_ = 0 ) const                      { return mNodeRenderParam.Subset[ subset_idx_ ].Opacity; }//return mOpacity; }

	/// ��Z�F�̐ݒ�
    void setColorScale( const LColorF& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].ColorScale = color_; }          

	/// ��Z�F�̎擾
    const LColorF& getColorScale( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].ColorScale; }

	/// �u�����h�J���[�̐ݒ�
    void setBlendColor( const LColorF& color_, lnU32 subset_idx_ = 0 )      { mNodeRenderParam.Subset[ subset_idx_ ].BlendColor = color_; }

	/// �u�����h�J���[�̎擾
    const LColorF& getBlendColor( lnU32 subset_idx_ = 0 ) const             { return mNodeRenderParam.Subset[ subset_idx_ ].BlendColor; }

	/// �F���̐ݒ�
    void setTone( const LTone& tone_, lnU32 subset_idx_ = 0 )              { mNodeRenderParam.Subset[ subset_idx_ ].Tone = tone_; }

	/// �F���̎擾
    const LTone& getTone( lnU32 subset_idx_ = 0 ) const                     { return mNodeRenderParam.Subset[ subset_idx_ ].Tone; }

	/// UV �ϊ��s��̐ݒ�
	void setUVTransform( const LMatrix& matrix_, lnU32 subset_idx_ = 0 )    { mNodeRenderParam.Subset[ subset_idx_ ].UVTransform = matrix_; }

	/// UV �ϊ��s��̐ݒ�
    const LMatrix& getUVTransform( lnU32 subset_idx_ = 0 )  const           { return mNodeRenderParam.Subset[ subset_idx_ ].UVTransform; }

	/// �������@�̐ݒ�
	void setBlendMode(Graphics::BlendMode mode_)          { mNodeRenderParam.RenderState.Blend = mode_; }

	/// �������@�̎擾
	Graphics::BlendMode getBlendMode() const                { return mNodeRenderParam.RenderState.Blend; }

	/// �[�x�e�X�g�̗L���ݒ�
    void setEnableDepthTest( bool flag_ )           { mNodeRenderParam.RenderState.DepthTest = flag_; }

	/// �[�x�e�X�g�̗L������
    bool isEnableDepthTest() const                  { return mNodeRenderParam.RenderState.DepthTest; }

	/// �[�xWrite�̗L���ݒ�
    void setEnableDepthWrite( bool flag_ )          { mNodeRenderParam.RenderState.DepthWrite = flag_; }

	/// �[�x�������݂̗L������
    bool isEnableDepthWritet() const                { return mNodeRenderParam.RenderState.DepthWrite; }

	/// �J�����O���@�̐ݒ�
	void setCullingMode(Graphics::CullingMode mode_)      { mNodeRenderParam.RenderState.Culling = mode_; }

	/// �J�����O���@�̎擾
	Graphics::CullingMode getCullingMode() const            { return mNodeRenderParam.RenderState.Culling; }

	///// ��]�����̐ݒ�
	//void setRotateOrder( Math::RotationOrder order_ )	{ mRotOrder = order_; }

	///// ��]�����̎擾
	//Math::RotationOrder getRotateOrder() const			{ return mRotOrder; }

	///// �D��x�̐ݒ� (�����������ɕ`�悳���)
	//void setPriority( lnS32 priority_ )               { mPriority = priority_; }

	///// �D��x�̎擾
	//lnS32 getPriority() const                         { return mPriority; }
  
	///// �r���{�[�h��Ԃ̐ݒ�
	//void setBillboardType( LNBillboardType type_ )  { mBillboardType = type_; }

	///// �r���{�[�h��Ԃ̎擾
	//LNBillboardType getBillboardType() const        { return mBillboardType; }

	///// �����X�V�̗L���ݒ�
	//void setEnableAutoUpdate( bool flag_ )          { mIsAutoUpdate = flag_; }

	///// �����X�V�̗L������
	//bool isEnableAutoUpdate()                       { return mIsAutoUpdate; }

	/// �V�F�[�_�̐ݒ�
    void setShader( SceneShader* shader_ );

	/// �V�F�[�_�̎擾
    SceneShader* getShader()                        { return mShader; }
#endif



public:
	virtual SceneNodeType	getNodeType() const { return SceneNodeType_VisualNode; }
    virtual void			updateContext();

protected:

	VisualNodeRenderer*	mRenderer;
};


//==============================================================================
// �� VisualNodeContext
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class VisualNodeContext
	: public SceneNodeContext
{
public:
	VisualNodeContext();
	virtual ~VisualNodeContext() {}

public:
	const Graphics::LRenderState&			getRenderState() const { return mRenderState; }
    int							getSubsetCount() const { return mSubsetRenderParamArray.size(); }
	const LNSubsetRenderParam&  getSubsetRenderParam( int index ) const { return mSubsetRenderParamArray[index]; }
	
public:
	// �T�u�Z�b�g������ꍇ�� render() ����Ă΂�� (�X�e�[�g�͐ݒ�ς݁B���_�̕`�悾���s��)
	virtual void drawSubset( ContextDrawParam& param, int index ) {}

protected:
	// SceneNodeContext
	virtual SceneNodeType	getNodeType() const { return SceneNodeType_VisualNode; }
    virtual void			updateContext( SceneNodeContext* parentContext );
    virtual void			releaseAllResource();
	virtual void			render( ContextDrawParam& param );

protected:
	// MMESceneObjectBase
	virtual SceneShader*	getShader() { return mShader; }
	virtual lnFloat			getOpacity() const { return (mSubsetRenderParamArray.empty()) ? 1.0f : mSubsetRenderParamArray[0].Opacity; }
	
protected:
	typedef std::vector<LNSubsetRenderParam>				SubsetRenderParamArray;

	Graphics::LRenderState			mRenderState;
    SubsetRenderParamArray	mSubsetRenderParamArray;
    SceneShader*            mShader;


#if 0
public:

    void initialize( SceneNode* node );

	/// [�����t�F�[�Y]
    virtual void updateContext( VisualNodeContext* parent_context_ );

	/// [�����t�F�[�Y] (�Ƃ肠���� updateContext() ����Ă�ł���)
    virtual void releaseAllResource();

	/// [�`��X���b�h]
	//void setPriorityParameter( LNPriorityParameter* param_ ) { mPriorityParameter = param_; }

	/// [�`��X���b�h]
    virtual void preRender( ContextDrawParam& param );

	/// [�`��X���b�h] Node �P�ʂ̃p�����[�^���V�F�[�_�ɐݒ肷�鎞�ɌĂяo�����
    virtual void onSetNodeParameter( SceneShader* shader_ ) {}

	/// [�`��X���b�h] (���� Model �݂̂Ŏg�p���B�ق��̂����ɁA����ŕ`�悷��悤�ɂ�����)
	virtual void drawSubset( lnU32 index ) { mSceneNode->drawSubset( index ); }

	/// �`��
	virtual void render( ContextDrawParam& param );

public:

    int getSubsetCount() const { return mSubsetNum; }

    virtual SceneShader* getShader() { return mShader; }

    const LMatrix& getCombinedWorldMatrix() const { return mCombinedWorldMatrix; }

    

    

	// SceneShader::updateSubsetParam() �p
    const LNSubsetRenderParam&  getSubsetRenderParam( int index_ ) const { return mSubsetRenderParams[index_]; }

    const LRenderState&         getRenderState() const { return mRenderState; }

    CameraContext* getCameraContext() { return mAffectCameraContext; }

protected:
	virtual const lnChar*			getName() const { return mName.c_str(); }
	virtual const LMatrix&			getMatrix() const { return mCombinedWorldMatrix; }
	virtual bool					isVisible() const { return mIsVisible; }
	virtual lnFloat					getOpacity() const { return mSubsetRenderParams[0].Opacity; }
	virtual void					setPriorityParameter( LNPriorityParameter* param ) { mPriorityParameter = param; }
    virtual const MMELightArray&	getAffectLightArray() const { return mAffectLightContextArray; }
    virtual bool					getObjectMatrix( LMatrix* out, MMEVariableRequest req, int light_idx ) const;
	virtual const LMatrix&			findFrameMarix( const lnChar* name ) const { return LMatrix::IDENTITY; }
	virtual lnFloat					findMorphBlend( const lnChar* name ) const { return 0.0f; }
protected:

    static bool _cmpZAndPrioritySort( const VisualNodeContext* l_, const VisualNodeContext* r_ );

protected:

    SceneNode*              mSceneNode;
	lnRefString				mName;
    VisualNodeContextArray   mChildSceneNodeContextArray;
    lnU32                   mSubsetNum;
    LNSubsetRenderParam*    mSubsetRenderParams;	///< �v�f���� mSubsetNum
    LRenderState            mRenderState;
    LNPriorityParameter*    mPriorityParameter;
    LMatrix                 mCombinedWorldMatrix;   ///< �s��̊K�w�X�V�ō쐬����錋���ς݃��[���h�s��
    lnS32                   mPriority;
    LNBillboardType         mBillboardType;
    SceneShader*            mShader;
    lnFloat                 mZDistance;
    MMELightArray       mAffectLightContextArray;           ///< �`��Ɏg�����C�g
    CameraContext*          mAffectCameraContext;
    bool                    mIsVisible;
	bool					mEnableDrawing;

public:

	SceneNodeArray			mChildNodeList;
#endif
};
} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================