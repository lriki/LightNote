//==============================================================================
// SceneGraph 
//------------------------------------------------------------------------------
///**
//  @file       SceneGraph.h
//  @brief      SceneGraph
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include <algorithm>
#include "../Base/ReferenceObjectArray.h"
#include "../Base/STLUtils.h"
#include "../Math/Vector2.h"
#include "../System/Interface.h"
#include "../Game/GameTime.h"
#include "../Graphics/Manager.h"
#include "../Geometry/Geometry.h"
#include "Common.h"
#include "DrawCommand.h"
#include "SceneNode/Viewport.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// SceneGraph
//------------------------------------------------------------------------------
///**
//  @brief      �V�[���O���t
//*/
//==============================================================================
class SceneGraph
    : Base::ReferenceObject
{
public:

    struct InitData
    {
        Base::LogFile*      LogFile;
        FileIO::Manager*      FileManager;
        Graphics::Manager*  GraphicsManager;
        Effect::Manager*    EffectManager;
        Physics::Manager*   PhysicsManager;
        LVector2            VirtualViewSize;
        lnU32               ShaderCacheSize;
    };

public:
    SceneGraph( Manager* manager_ );
    virtual ~SceneGraph();
    LN_REFOBJ_METHODS;

public:

	/// ������
    LNRESULT initialize( const InitData& init_data_ );

	/// �I������
    void finalize();

	/// SceneNode ���������ɐݒ肳��郌���_�����O�X�e�[�g�̐ݒ�
	void setNodeDefaultRenderState(const Graphics::LRenderState& state_) { mNodeDefaultRenderState = state_; }

	/// SceneNode ���������ɐݒ肳��郌���_�����O�X�e�[�g�̎擾
	const Graphics::LRenderState& getNodeDefaultRenderState() { return mNodeDefaultRenderState; }

	/// �V�F�[�_�Ǘ��N���X�̎擾
    SceneShaderManager* getSceneShaderManager() const { return mSceneShaderManager; }

	/// �f�t�H���g�ō쐬�����m�[�h�̎擾
    Viewport* getDefaultNode( LNDefaultSceneNode node_idx_ ) { return mDefaultSceneNodes[ node_idx_ ]; }

	/// �f�t�H���g�ō쐬�����J�����̎擾 (0:3D 1:2D)
    Camera* getDefaultCamera( int type_ = 0 );

	/// �f�t�H���g�ō쐬����郉�C�g�̎擾
    Light* getDefaultLight() { return mDefaultLight; }

    
	/// �f�t�H���g�ō쐬�����V�F�[�_�v���O�����̎擾
    SceneShader* getDefaultShader( LNDefaultSceneShader shader_idx_ ) { return mDefaultShaderArray[ shader_idx_ ]; }

	/// ���C���J�����̎擾 (�V�F�[�_�̃e�N�X�`���쐬�����ASceneGraph �ȉ��̃r���[�T�C�Y�̊�ƂȂ�)
	Camera* getMainCamera() { return mMainCamera; }

	/// �������O�X�V
	void updateBeforePhysics();

	//----------------------------------------------------------------------
	///**
	//  @brief      �t���[���X�V (makeDrawCommand() �̑O�Ɉ�x�Ă�) 
	//
	//  @note       
	//              [�����t�F�[�Y]
	//              �V�[�����ŁA�`��m�[�h�A�J�����Ɉˑ����Ȃ�
	//              �O���[�o���ȃp�����[�^���ŐV�̏�ԂɍX�V����B<br>
	//              ��̓I�ɂ́A<br>
	//                  - �f�t�H���g���C�g�̏�Ԃ�S�V�F�[�_�ɃZ�b�g
	//                  - ���ԁA�}�E�X����S�V�F�[�_�ɃZ�b�g
	//                  - ���ׂẴm�[�h�̃��[���h�s��̊K�w�X�V
	//              <br>
	//              �����J�������g���Ƃ� (�����v���C���[�p�̃r���[�Ȃ�)�A
	//              1�t���[�����ɏd�����Ĕ������鏈�����܂Ƃ߂������B
	//              ���̊֐��� 1 �t���[���� 1 �x�����Ăяo���� OK�B
	//*/
	//----------------------------------------------------------------------
	void updateFrame(float elapsedTime/* const Game::GameTime& gameTime */);

	/// ��������X�V
	void updateAfterPhysics();

	/// [�����t�F�[�Y][�`�悪�K�v�Ȏ�]
    void updateAllContext();

	/// �t���[���X�V�̌㏈���B
	/// ���C���X���b�h����A�`��X�L�b�v�̗L���Ɋ֌W�Ȃ��K�����t���[����΂��B
	void postFrameUpdate();


	SceneGraphContext* getSceneGraphContext() { return mSceneGraphContext; }

public:
	void setBaseViewSize( const LVector2& size );
    void addCamera( Camera* camera );
    void addLight( Light* light );
    bool onEvent( const System::EventArgs& e );

	Manager* getManager() { return mManager; }
    FileIO::Manager* getFileManager() { return mFileManager; }
    Graphics::Manager* getGraphicsManager() { return mGraphicsManager; }
	//Graphics::GeometryRenderer* getGeometryRenderer() { return mGeometryRenderer; }
    Effect::Manager* getEffectManager() { return mEffectManager; }
    Physics::Manager* getPhysicsManager() { return mPhysicsManager; }

    void addNode( SceneNode* node_ );
    void onDeleteNode( SceneNode* node_ );//removeNode( SceneNode* node_ );

	/// (Camera �̃R���X�g���N�^����Ă΂��)
	//void addCamera( Camera* camera_ ) { mCameraList.push_back( camera_ ); }

	/// (Camera �̃f�X�g���N�^����Ă΂��)
	//void removeCamera( Camera* camera_ ) { Base::STLUtil::remove( mCameraList, camera_ ); }

	/// (Light �̃R���X�g���N�^����Ă΂��)
	//void addLight( Light* light_ ) { mLightList.push_back( light_ ); }

	/// (Light �̃f�X�g���N�^����Ă΂��)
	//void removeLight( Light* light_ ) { Base::STLUtil::remove( mLightList, light_ ); }

	/// (Viewport �̃R���X�g���N�^����Ă΂��)
    void addViewport( Viewport* vp_ ) { mViewportList.push_back( vp_ ); }

	/// (Viewport �̃f�X�g���N�^����Ă΂��)
    void removeViewport( Viewport* vp_ ) { mViewportList.remove( vp_ ); }

    

	/// SceneNode �̖��O��ύX���钼�O�ɌĂяo�� (SceneNode �� setName() ����Ă΂��)
    void onSetNodeName( SceneNode* node_, const lnChar* old_name_, const lnChar* new_name_ );

	/// �L�����C�g�̒����烉�C�g��񋓂��� 
	//      ref_pos_ : ��ʒu
	//      makeDrawCommand() ���� SceneNode::updateParamFromCameraAndLight() �o�R�ŌĂ΂��B
	//      ���C�g�� out_->size() �̐������i�[����B���炩���� resize ���Ă������ƁB
    void enumLights( LightArray* out_, const LVector3& ref_pos_ );

	/// SceneShader �̃f�X�g���N�^����Ă΂��
    ViewportList& getViewportList() { return mViewportList; }

	/// SceneNode �ꗗ�̎擾
	SceneNodeArray& getSceneNodeArray() { return mSceneNodeList2.getObjectArray(); }

private:

    struct ActiveLightData
    {
        Light*      ActiveLight;
        lnFloat     DistanceToCamera;   ///< �A�N�e�B�u�ȃJ��������̋�����2��
        lnFloat     Distance;           ///< �\�[�g�p�̊���� (�\�[�g���O�ɐݒ肷��B�A�N�e�B�u�ȃJ��������̋�����2��)
    };

    typedef std::vector< ActiveLightData > ActiveLightDataArray;
    

	/// ���C�g�񋓗p�̔�r�֐�
    static bool _cmpLights( const ActiveLightData& l_, const ActiveLightData& r_ );

private:

	typedef Base::ReferenceObjectArray<SceneNode*>	SceneNodeObjectArray;
	typedef Base::ReferenceObjectArray<Camera*>	CameraObjectArray;
	typedef Base::ReferenceObjectArray<Light*>	LightObjectArray;

    Base::LogFile*          mLogFile;
    Manager*                mManager;
    FileIO::Manager*          mFileManager;
    Graphics::Manager*      mGraphicsManager;
    Graphics::GeometryRenderer* mGeometryRenderer;
    Effect::Manager*        mEffectManager;
    Physics::Manager*       mPhysicsManager;
	//LVector2                mVirtualViewSize;
	Graphics::LRenderState			mNodeDefaultRenderState;
    SceneShaderManager*     mSceneShaderManager;
	//Game::GameTime			mLastUpdateTime;
	double					mTotalTime;		// context
	float					mElapsedTime;	// context

	SceneNodeObjectArray	mSceneNodeList2;
	//SceneNodeList           mNodeRegistOrderList;
	//SceneNodeArray          mSceneNodeList;     ///< �쐬����Ă��� SceneObject �̃|�C���^���X�g
    SceneNodeNameMap        mNodeNameMap;       ///< �쐬����Ă��� SceneObject �̂����A���O�������̂̑Ή��\
	//SceneNodeArray          mSceneNodeAdditionalList;

	//CameraList              mCameraList;
	CameraObjectArray		mCameraObjectArray;
	LightObjectArray		mLightObjectArray;
	//LightList               mLightList;
    ViewportList            mViewportList;

    Viewport*               mDefaultSceneNodes[ LN_MAX_DEFAULT_NODES ];
    Camera*                 mDefaultCamera;
    Camera*                 mDefault2DCamera;
    Light*                  mDefaultLight;
    SceneShader*            mDefaultShaderArray[ LN_MAX_DEFAULT_SHADERS ];
	Camera*					mMainCamera;
	//Pane*                   mDefault3DPane;
	//Pane*                   mDefault2DPane;
	//Pane*					mDefaultPane;
    Camera*                 mActiveCamera;
    Camera*                 mActiveViewFrustum;
    ActiveLightDataArray    mActiveLightDataArray;  ///< �L���ȃ��C�g�̈ꗗ (makeDrawCommand() �ō����)

	//EffectBatchProxy*       mEffect3DBatchProxy;
	//EffectBatchProxy*       mEffect2DBatchProxy;
	//PhysicsDebugDrawProxy*  mPhysicsDebugDrawProxy;

    SceneGraphContext*      mSceneGraphContext;
    
    friend class SceneGraphContext;
};

//==============================================================================
// �� SceneGraphContext
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class SceneGraphContext
	: public MMESceneBase//
{
public:

    SceneGraphContext();

public:

    void initialize( SceneGraph* scene_ );

    void updateContext();

    void selectLights( MMELightArray* out, const LVector3& ref_pos );

	void render( ContextDrawParam& param );

	//void OnSetActivePaneContext( PaneContext* pane_context_, bool is_3d_ );

protected:
    virtual MMELightBase* getDefaultMMELight();
	virtual MMESceneObjectArray& getMMESceneObjectArray() { return mMMESceneObjectArray; }
	virtual MMESceneObjectBase* findMMESceneObject( const lnChar* name );

private:

    static bool _cmpLights( const LightContext* l_, const LightContext* r_ );

private:

    SceneGraph*             mSceneGraph;
    LVector2                mVirtualViewSize;
	MMESceneObjectArray		mMMESceneObjectArray;
	SceneNodeNameMap		mSceneNodeNameMap;
    CameraContextArray      mCameraContextArray;    // �J�����⃉�C�g�� AdditionalList �� Scene �ɍ��ΕK�v�Ȃ����ǁc
    LightContextArray       mLightContextArray;
    LightContextArray       mActiveLightContextArray;   ///< OnSetActivePaneContext() �ł̎�����J�����O�Ŋi�[
    LViewFrustum            mActiveViewFrustum;         ///< OnSetActivePaneContext() �ł̎�����J�����O�Ŋi�[
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================