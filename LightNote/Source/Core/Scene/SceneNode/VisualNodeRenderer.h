//=============================================================================
//�y VisualNodeRenderer �z
//-----------------------------------------------------------------------------
///**
//  @file       VisualNodeRenderer.h
//  @brief      VisualNodeRenderer
//  @author     Riki
//*/
//=============================================================================

#pragma once

#include "NodeRenderParam.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

////=============================================================================
//// �� VisualNodeRenderer
////-----------------------------------------------------------------------------
/////**
////  @brief
////*/
////=============================================================================
//class VisualNodeRenderer
//    : public Base::ReferenceObject
//{
//public:
//    VisualNodeRenderer();
//    virtual ~VisualNodeRenderer();
//
//public:
//
//	/// �쐬
//    void create( VisualNode* ownerNode, int subsetParamCount, VisualNodeRendererContext* rendererContext = NULL );
//	
//	/// ���� Renderer ��ێ����� SceneNode �̎擾
////	SceneNode* getOwnerNode() { return mOwnerNode; }
//
//public:
//
//	///// ����Ԃ̐ݒ�
// //   void setVisible( bool enabled )										{ mIsVisible = enabled; }
//
// //   /// ����Ԃ̔���
// //   bool isVisible() const												{ return mIsVisible; }
//
//	/// �s�����x�̐ݒ�
//    void setOpacity( lnFloat opacity, lnU32 subsetIndex = 0 )			{ mNodeRenderParam.Subset[ subsetIndex ].Opacity = opacity; }
//
//	/// �s�����x�̎擾
//    lnFloat getOpacity( lnU32 subsetIndex = 0 ) const					{ return mNodeRenderParam.Subset[ subsetIndex ].Opacity; }
//
//	/// ��Z�F�̐ݒ�
//    void setColorScale( const LColorF& color, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].ColorScale = color; }          
//
//	/// ��Z�F�̎擾
//    const LColorF& getColorScale( lnU32 subsetIndex = 0 ) const			{ return mNodeRenderParam.Subset[ subsetIndex ].ColorScale; }
//
//	/// �u�����h�J���[�̐ݒ�
//    void setBlendColor( const LColorF& color, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].BlendColor = color; }
//
//	/// �u�����h�J���[�̎擾
//    const LColorF& getBlendColor( lnU32 subsetIndex = 0 ) const			{ return mNodeRenderParam.Subset[ subsetIndex ].BlendColor; }
//
//	/// �F���̐ݒ�
//    void setTone( const LTone& tone, lnU32 subsetIndex = 0 )			{ mNodeRenderParam.Subset[ subsetIndex ].Tone = tone; }
//
//	/// �F���̎擾
//    const LTone& getTone( lnU32 subsetIndex = 0 ) const					{ return mNodeRenderParam.Subset[ subsetIndex ].Tone; }
//
//	/// UV �ϊ��s��̐ݒ�
//	void setUVTransform( const LMatrix& matrix, lnU32 subsetIndex = 0 )	{ mNodeRenderParam.Subset[ subsetIndex ].UVTransform = matrix; }
//
//	/// UV �ϊ��s��̐ݒ�
//    const LMatrix& getUVTransform( lnU32 subsetIndex = 0 )  const		{ return mNodeRenderParam.Subset[ subsetIndex ].UVTransform; }
//
//	/// �������@�̐ݒ�
//    void setBlendMode( LNBlendMode mode )				{ mNodeRenderParam.RenderState.BlendMode = mode; }
//
//	/// �������@�̎擾
//    LNBlendMode getBlendMode() const					{ return mNodeRenderParam.RenderState.BlendMode; }
//
//	/// �[�x�e�X�g�̗L���ݒ�
//    void setEnableDepthTest( bool enabled )				{ mNodeRenderParam.RenderState.DepthTest = enabled; }
//
//	/// �[�x�e�X�g�̗L������
//    bool isEnableDepthTest() const						{ return mNodeRenderParam.RenderState.DepthTest; }
//
//	/// �[�xWrite�̗L���ݒ�
//    void setEnableDepthWrite( bool enabled )			{ mNodeRenderParam.RenderState.DepthWrite = enabled; }
//
//	/// �[�x�������݂̗L������
//    bool isEnableDepthWritet() const					{ return mNodeRenderParam.RenderState.DepthWrite; }
//
//	/// �J�����O���@�̐ݒ�
//    void setCullingMode( LNCullingMode mode )			{ mNodeRenderParam.RenderState.Culling = mode; }
//
//	/// �J�����O���@�̎擾
//    LNCullingMode getCullingMode() const				{ return mNodeRenderParam.RenderState.Culling; }
//
//	///// ��]�����̐ݒ�
//	//void setRotateOrder( Math::RotationOrder order )	{ mRotOrder = order; }
//
//	///// ��]�����̎擾
//	//Math::RotationOrder getRotateOrder() const			{ return mRotOrder; }
//
//	///// �D��x�̐ݒ� (�����������ɕ`�悳���)
//	//void setPriority( lnS32 priority )					{ mPriority = priority; }
//
//	///// �D��x�̎擾
//	//lnS32 getPriority() const							{ return mPriority; }
//  
//	///// �r���{�[�h��Ԃ̐ݒ�
//	//void setBillboardType( LNBillboardType type )		{ mBillboardType = type; }
//
//	///// �r���{�[�h��Ԃ̎擾
//	//LNBillboardType getBillboardType() const			{ return mBillboardType; }
//
//	/// �V�F�[�_�̐ݒ�
//	//void setShader( SceneShader* shader );
//
//	///// �V�F�[�_�̎擾
//	//SceneShader* getShader()							{ return mShader; }
//
//public:
//
//	
//
//protected:
//	VisualNode*					mOwnerNode;
//	VisualNodeRendererContext*	mRendererContext;
//	NodeRenderParam				mNodeRenderParam;
//	//Math::RotationOrder			mRotOrder;
// //   lnS32						mPriority;
// //   LNBillboardType				mBillboardType;
//	//SceneShader*				mShader;
//	//bool						mIsVisible;
//	
//    LMatrix						mCombinedWorldMatrix;
//
//	friend class VisualNodeRendererContext;
//
//	/*	�� mShader �͂����ɒu���ׂ����H
//	 *		�u�`��ɕK�v�ȏ��v���Ă��Ƃł���΂�����OK�B
//	 *		�������ASceneNode ���Ɂu�|�X�g�G�t�F�N�g�̏ꍇ�͐�p�̃��X�g�ɓ������v�Ƃ���
//	 *		���������邽�߁A�����ɂ����Ă� SceneNode �Ɉˑ�����K�v������B
//	 *		�˂ł��A�t�Ɍ����Γ���ȏ��������Ă���͍̂��̂Ƃ��낱�ꂾ���B
//	 *		  �Ƃ肠�����͂��������B
//	 */
//	/*	�� �`��p�̃o�b�N�G���h�f�[�^(Context) �� flip? copy?
//	 *		flip �̏ꍇ�A�N���e�B�J���ȏ����̓|�C���^�̓���ւ������ɂł���B
//	 *		�������A���[�U�[���������ݒ�Ƃ��Đݒ肵���v���p�e�B�͂��̂܂܂���
//	 *		�v���C�}���ɂ������f����Ȃ��Bflip ���ɍX�V���Ȃ����Ƃ��A
//	 *		������ɂ��Ă� +���̏������������邵�A���̂�����̃P�A���������G�ɂȂ�B
//	 *		copy �̏ꍇ�͕K�v�ȃv���p�e�B��S�ăR�s�[����K�v������B
//	 *		���ꂾ���ł��኱�p�t�H�[�}���X�͗����邪�A���ƒv���I�Ȃ̂�
//	 *		�X�L�����b�V���̃{�[����A���_�o�b�t�@�̃R�s�[�B
//	 *		����͍s����ЂƂ̔z��ɂ��� memcpy ���A�ň����ꂾ�� flip �ɂ���Ȃ�
//	 *		��������̑΍􂪕K�v�ɂȂ�B
//	 *		�����A����ȊO�͊�{�I�� copy �̕����P���Ȃ̂ł������x�[�X�ō��B
//	 */
//};
//
////=============================================================================
//// �� VisualNodeRendererContext
////-----------------------------------------------------------------------------
/////**
////  @brief
////*/
////=============================================================================
//class VisualNodeRendererContext
//    : public MMESceneObjectBase
//{
//public:
//    VisualNodeRendererContext();
//    virtual ~VisualNodeRendererContext();
//
//public:
//
//	/// �쐬
//    void create( VisualNodeRenderer* ownerRenderer );
//
//	/// [�����t�F�[�Y] (�Ƃ肠���� updateContext() ����Ă�ł���)
//	virtual void releaseAllResource();
//
//	/// [�����t�F�[�Y]
//    virtual void updateContext( VisualNodeRendererContext* parentNodeContext );
//
//	/// [�`��X���b�h] �`��O����
//    virtual void preRender( ContextDrawParam& param );
//
//	/// [�`��X���b�h] Node �P�ʂ̃p�����[�^���V�F�[�_�ɐݒ肵����Ăяo����� (�ǉ��Őݒ肵�����p�����[�^������Ƃ��ɃI�[�o�[���C�h����)
//    virtual void onSetNodeParameter( SceneShader* shader ) {}
//
//	/// �T�u�Z�b�g���̎擾
//	virtual int getSubsetCount();
//
//	/// �T�u�Z�b�g��`�悷��
//	virtual void drawSubset( int index ) {};
//
//public:
//	/// MMESceneObjectBase
//	virtual const lnChar*			getName() const { return mName.c_str(); }
//    virtual SceneShader*			getShader() { return mShader; }
//	virtual const LMatrix&			getMatrix() const { return mCombinedWorldMatrix; }
//	virtual bool					isVisible() const { return mIsVisible; }
//	virtual lnFloat					getOpacity() const { return mSubsetRenderParamArray[0].Opacity; }
//	//virtual void					setPriorityParameter( LNPriorityParameter* param ) { mPriorityParameter = param; }
//    virtual const MMELightArray&	getAffectLightArray() const { return mAffectLightContextArray; }
//    virtual bool					getObjectMatrix( LMatrix* outMat, MMEVariableRequest req, int lightIndex ) const;
//	virtual const LMatrix&			findFrameMarix( const lnChar* name ) const { return LMatrix::IDENTITY; }
//	virtual lnFloat					findMorphBlend( const lnChar* name ) const { return 0.0f; }
//	virtual void					render( ContextDrawParam& param );
//
//protected:
//	typedef std::vector<LNSubsetRenderParam>				SubsetRenderParamArray;
//	//typedef Base::ReferenceObjectArray<VisualNodeRenderer*>	VisualNodeRendererRefArray;
//
//	VisualNodeRenderer*				mOwnerRenderer;
//	//VisualNodeRendererContextArray	mChildContextArray;
//	MMELightArray					mAffectLightContextArray;	///< �`��Ɏg�����C�g
//	CameraContext*					mAffectCameraContext;		///< ����̕`��Ɏg���J���� (preRender() �ŃZ�b�g)
//
//	SceneShader*			mShader;
//    SubsetRenderParamArray	mSubsetRenderParamArray;
//	lnRefString				mName;
//    LMatrix                 mCombinedWorldMatrix;		///< �s��̊K�w�X�V�ō쐬����錋���ς݃��[���h�s��
//    LRenderState            mRenderState;
//	//LNPriorityParameter*    mPriorityParameter;
//    lnS32					mPriority;
//	bool                    mIsVisible;
//	bool					mEnableDrawing;				///< ������J�����O���ŕ]��������A�`�悷�邩�ǂ���
//
//	lnFloat                 mZDistance;
//	LNBillboardType         mBillboardType;
//	//VisualNodeRendererRefArray	mVisualNodeRendererRefArray;
//};

} // namespace Scene
} // namespace Core
} // namespace LNote

//=============================================================================
//                              end of file
//=============================================================================