//==============================================================================
// ModelCore 
//==============================================================================

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include "stdafx.h"
#include "../../../../Base/Unicode.h"
#include "../../../../FileIO/StreamObject.h"
#include "../../../../FileIO/BinaryReader.h"
#include "../../../Interface.h"
#include "../../../DeviceObjects.h"
#include "../PMDTypes.h"
#include "../ModelManager.h"
#include "../ModelFrame.h"
#include "../ModelFace.h"
#include "../ModelIK.h"
#include "../ModelRigidBody.h"
#include "../ModelConstraint.h"
#include "ModelCore.h"
#include "../MotionController.h"
#include "../ConvertBlendedMesh.h"
#include "ModelFaceCore.h"

#include "ModelFrameCore.h"
#include "ModelIKCore.h"
#include "ModelRigidBodyCore.h"
#include "ModelConstraintCore.h"


//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelCore2
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelCore2::ModelCore2( ModelManager* manager, ModelFormat format )
        : Manager			( manager )
		, VertexBuffer		( NULL )
        , IndexBuffer		( NULL )
		, mFormat			( format )
    {
		LN_SAFE_ADDREF( Manager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelCore2::~ModelCore2()
    {
		LN_SAFE_RELEASE( VertexBuffer );
		LN_SAFE_RELEASE( IndexBuffer );

		// Frames
		ln_foreach( ModelFrameCore* frame, FrameArray ) {
			frame->release();
		}
		FrameArray.clear();
		
		// IKs
		ln_foreach( ModelIKCore* ik, IKArray ) {
			ik->release();
		}
		IKArray.clear();

		// Morphs
		ln_foreach( ModelMorphCore* m, ModelMorphCores ) {
			m->release();
		}
		ModelMorphCores.clear();

		// RigidBodys
		ln_foreach( ModelRigidBodyCore* rb, RigidBodys ) {
			rb->release();
		}
		RigidBodys.clear();

		// Constraints
		ln_foreach( ModelConstraintCore* c, Constraints ) {
			c->release();
		}
		Constraints.clear();

		LN_SAFE_RELEASE( Manager );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelCore2::postInitialize()
	{
		size_t count = VertexBuffer->getVertexCount();
		mMorphOrgVertices.resize( count );

		// PMD フォーマット
		if ( mFormat == ModelFormat_PMD )
		{
			Vertex::PMDVertex* v = (Vertex::PMDVertex*)VertexBuffer->lock();
			for ( size_t i = 0; i < count; i++, v++ )
			{
				mMorphOrgVertices[i].Position = v->Position;
			}
			VertexBuffer->unlock();
		}
		// PMX フォーマット
		else if ( mFormat == ModelFormat_PMX )
		{
			PMX_Vertex* v = (PMX_Vertex*)VertexBuffer->lock();
			for ( size_t i = 0; i < count; i++, v++ )
			{
				mMorphOrgVertices[i].Position			= v->Position;
				mMorphOrgVertices[i].UV					= v->TexUV;
				mMorphOrgVertices[i].AdditionalUV[0]	= v->AdditionalUV[0];
				mMorphOrgVertices[i].AdditionalUV[1]	= v->AdditionalUV[1];
				mMorphOrgVertices[i].AdditionalUV[2]	= v->AdditionalUV[2];
				mMorphOrgVertices[i].AdditionalUV[3]	= v->AdditionalUV[3];
			}
			VertexBuffer->unlock();
		}

		// 材質
		Material.postInitialize();

		// フレーム
		ln_foreach( ModelFrameCore* frame, FrameArray )
		{
			frame->postInitialize();

			// 親子をつなぐ
			if ( 0 <= frame->ParentFrameIndex && frame->ParentFrameIndex < FrameArray.size() )
				FrameArray[frame->ParentFrameIndex]->addChild( frame );
		}
		
		// ルートフレームからの深さをカウント
		ln_foreach( ModelFrameCore* frame, FrameArray )
		{
			if ( 0 <= frame->ParentFrameIndex && frame->ParentFrameIndex < FrameArray.size() ) {
			}
			else {
				frame->makeDepthLevel( 0 );
			}
		}

#if 0
		struct FrameComparer
		{
			int FrameCount;

			bool operator()( const ModelFrameCore* left, const ModelFrameCore* right )
			{
				int leftScore = 0;
				int rightScore = 0;

				leftScore  += left->getDepthLevel() * FrameCount;
				rightScore += right->getDepthLevel() * FrameCount;

				leftScore  += left->FrameIndex;
				rightScore += right->FrameIndex;

				return (leftScore < rightScore);
			}
		} comparer;
		comparer.FrameCount = FrameArray.size();
		std::sort( FrameArray.begin(), FrameArray.end(), comparer );
#endif
		struct IKLinkComparer
		{
			ModelCore2* Self;

			bool operator()( const ModelIKLink& left, const ModelIKLink& right )
			{
				//return left.[0] - right.LinkBoneIndex[0];

				int leftScore = 0;
				int rightScore = 0;

				leftScore  += Self->FrameArray[left.LinkBoneIndex]->getDepthLevel() * Self->FrameArray.size();
				rightScore += Self->FrameArray[right.LinkBoneIndex]->getDepthLevel() * Self->FrameArray.size();

				leftScore  += left.LinkBoneIndex;
				rightScore += right.LinkBoneIndex;

				return (leftScore < rightScore);
			}
		} ikLinkComparer;
		ikLinkComparer.Self = this;

		// IKリンクをソートする
		ln_foreach( ModelIKCore2* ik, IKs )
		{
			std::sort( ik->IKLinks.begin(), ik->IKLinks.end(), ikLinkComparer );
		}





		//struct IKComparer
		//{
		//	bool operator()( const ModelIKCore2* left, const ModelIKCore2* right )
		//	{
		//		return left->IKLinks[0].LinkBoneIndex - right->IKLinks[0].LinkBoneIndex;
		//	}
		//} ikComparer;
		//std::sort( IKs.begin(), IKs.end(), ikComparer );


#if 0
		ln_foreach( ModelFrameCore* frame, FrameArray )
		{
			if ( frame->IsIK ) {
				std::sort( frame->IKLinks.begin(), frame->IKLinks.end(), ikComparer );
			}
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelCore2::sortIKArray()
	{
		struct cmp {
			bool operator()( const ModelIKCore* left, const ModelIKCore* right ) const {
				return left->SortVal < right->SortVal;
			}
		};
		//qsort( &IKArray[0], IKArray.size(), sizeof(ModelIKCore), cmp::cmpIKDataFunc );

		std::sort( IKArray.begin(), IKArray.end(), cmp() );
	}

//==============================================================================
// ■ ModelCore クラス
//==============================================================================

	/// IKデータソート用比較関数
    static int cmpIKDataFunc( const void* l_, const void* r_ )
    {
	    return (int)(((ModelIK *)l_)->getSortVal() - ((ModelIK *)r_)->getSortVal());
    }

	//----------------------------------------------------------------------
	// ● コンストラクタ
	//----------------------------------------------------------------------
    ModelCore::ModelCore( ModelManager* manager_ )
        : mManager              ( manager_ )
        , mGraphicsDevice       ( manager_->getGraphicsDevice() )
        , mVertexBuffer         ( NULL )
        , mIndexBuffer          ( NULL )
        , mOrgVertexPositions   ( NULL )
        , mOrgVertexNormals     ( NULL )
        , mCollisionMesh        ( NULL )
        , mIKNum                ( 0 )
        , mIKArray              ( NULL )
        , mIKLimitter           ( NULL )
        , mIsSkinMesh           ( false )
    {
    }

	//----------------------------------------------------------------------
	// ● デストラクタ
	//----------------------------------------------------------------------
    ModelCore::~ModelCore()
    {
        _release();
    }

	
	bool ModelCore::checkPMDData( const void* data, lnU32 size )
	{
		PMD_Header	*pPMDHeader = (PMD_Header *)data;
	    if( pPMDHeader->szMagic[0] != 'P' || pPMDHeader->szMagic[1] != 'm' || pPMDHeader->szMagic[2] != 'd' || pPMDHeader->fVersion != 1.0f ) {
			return false;
		}
		return true;
	}

	//----------------------------------------------------------------------
	// ● PMD モデル読み込み
	//----------------------------------------------------------------------
#if 0
    void ModelCore::loadPMD( const void* data_top_, lnU32 size_, lnU32 flags_, Model** model_ )
    {
        _release();

        // スキンメッシュ
        mIsSkinMesh = true;

        *model_ = NULL;

        bool is_dynamic = ( flags_ &ModelCreateFlag_SoftwareSkinning );

        const lnByte* data = static_cast< const lnByte* >( data_top_ );

        
        
        //-----------------------------------------------------
	    // ヘッダのチェック

	    PMD_Header	*pPMDHeader = (PMD_Header *)data;
	    if( pPMDHeader->szMagic[0] != 'P' || pPMDHeader->szMagic[1] != 'm' || pPMDHeader->szMagic[2] != 'd' || pPMDHeader->fVersion != 1.0f ) {
			LN_THROW_FileFormat(0);
		}

        // モデル名のコピー
        char model_name[21];	// モデル名
	    strncpy( model_name, pPMDHeader->szName, 20 );
	    model_name[20] = '\0';
        mModelName = model_name;

	    data += sizeof( PMD_Header );


        // ファイル形式チェック後、Model を作る
        Model* model = LN_NEW Model( mManager );
        model->mModelCore = this;

        //-----------------------------------------------------
	    // 頂点数取得
        
        lnU32 v_num = *((lnU32 *)data);
        data += sizeof( lnU32 );

        // 頂点バッファ作成
        mVertexBuffer = mGraphicsDevice->createVertexBuffer(
			Vertex::PMDVertex::elements(), v_num, NULL, is_dynamic );

        // ソフトウェアスキニング用
        if ( flags_ & ModelCreateFlag_SoftwareSkinning )
        {
            mOrgVertexPositions = LN_NEW LVector3[ v_num ];
            mOrgVertexNormals   = LN_NEW LVector3[ v_num ];
        }

	    // データを流し込む
        PMD_Vertex* pmd_vertex = (PMD_Vertex*)data;
        Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        Vertex::PMDVertex* v;
        for ( lnU32 i = 0 ; i < v_num; ++i, ++pmd_vertex )
	    {
            v = &vertices[ i ];

            v->Position = pmd_vertex->vec3Pos;
            v->Normal   = pmd_vertex->vec3Normal;
            v->TexUV    = pmd_vertex->uvTex;
            v->Color    = 0xffffffff;

            v->BlendIndices[ 0 ] = static_cast<lnFloat>( pmd_vertex->unBoneNo[ 0 ] );
            v->BlendIndices[ 1 ] = static_cast<lnFloat>( pmd_vertex->unBoneNo[ 1 ] );
            v->BlendIndices[ 2 ] = 0;
            v->BlendIndices[ 3 ] = 0;
            v->BlendWeights[ 0 ] = pmd_vertex->cbWeight * 0.01f;
            v->BlendWeights[ 1 ] = 1.0f - v->BlendWeights[ 0 ];
            v->BlendWeights[ 2 ] = 0.0f;
            v->BlendWeights[ 3 ] = 0.0f;

            if ( flags_ & ModelCreateFlag_SoftwareSkinning )
            {
                mOrgVertexPositions[ i ] = v->Position;
                mOrgVertexNormals[ i ]   = v->Normal;
            }
	    }

        mVertexBuffer->unlock();

        lnU32 size = sizeof( PMD_Vertex ) * v_num;
	    data += size;

        
        
        //-----------------------------------------------------
	    // 頂点インデックス

	    lnU32 i_num = *((lnU32 *)data);
	    data += sizeof( lnU32 );
        lnU16* indices = NULL;

        // インデックスバッファ作成 (PMD は 16bit)
        mIndexBuffer = mGraphicsDevice->createIndexBuffer( 
			i_num, NULL, is_dynamic, true );

	    // 頂点インデックス配列をコピー
        indices = (lnU16*)mIndexBuffer->lock();
	    memcpy( indices, data, sizeof( lnU16 ) * i_num );
        mIndexBuffer->unlock();

	    data += ( sizeof( lnU16 ) * i_num );

        //-----------------------------------------------------
	    // マテリアル

        // マテリアル数取得
        lnU32 attr_num = *((lnU32 *)data);
	    data += sizeof( lnU32 );

        // マテリアル、属性配列
        LMaterial*       materials  = LN_NEW LMaterial[ attr_num ];
        LNMeshAttribute* attributes = LN_NEW LNMeshAttribute[ attr_num ];

	    PMD_Material* pmd_material = (PMD_Material*)data;

        lnU32 index_offset = 0;
        lnU32 face_sum = 0;
        LMaterial* m;
        
	    for ( lnU32 i = 0 ; i < attr_num ; ++i )
	    {
            m = &materials[ i ];

            // 色を取得していく
            m->Diffuse = pmd_material->col4Diffuse;

            m->Power = pmd_material->fShininess;

            m->Specular.R = pmd_material->col3Specular.x;
            m->Specular.G = pmd_material->col3Specular.y;
            m->Specular.B = pmd_material->col3Specular.z;
            m->Specular.A = 1.0f;

            m->Ambient.R = pmd_material->col3Ambient.x;
            m->Ambient.G = pmd_material->col3Ambient.y;
            m->Ambient.B = pmd_material->col3Ambient.z;
            m->Ambient.A = 1.0f;

			//_p((int)pmd_material->ToonIndex);
			//_p((int)pmd_material->EdgeFlag);
		   
            // テクスチャ名がある場合はテクスチャ作成
		    if ( pmd_material->szTextureFileName[ 0 ] )
		    {
                char* tex_name = pmd_material->szTextureFileName;
                char* sphere_name = NULL;
                char* c = pmd_material->szTextureFileName;
                while ( *c )
                {
                    if ( *c == '*' )
                    {
                        *c = '\0';
                        sphere_name = ( c + 1 );
                        break;
                    }
                    ++c;
                }
                
                Texture* tex = mManager->createTexture( tex_name, flags_ );
                m->setTexture( tex );
                LN_SAFE_RELEASE( tex );

                // スフィアテクスチャ
                if ( sphere_name )
                {
                    tex = mManager->createTexture( tex_name, flags_ );
                    m->setSphereTexture( tex );
                    LN_SAFE_RELEASE( tex );
                }                
		    }
		    else
		    {
			    m->setTexture( NULL );
                m->setSphereTexture( NULL );
		    }

            // 属性テーブルを埋める
            attributes[ i ].MaterialIndex = i;
            attributes[ i ].StartIndex = index_offset;
            attributes[ i ].PrimitiveNum = pmd_material->ulNumIndices / 3;
            //attributes[ i ].FaceStart = face_sum;
            //attributes[ i ].VertexCount = pmd_material->ulNumIndices / 3;

            index_offset += pmd_material->ulNumIndices;
            face_sum += attributes[ i ].PrimitiveNum;
		    pmd_material++;
	    }

        data += sizeof( PMD_Material ) * attr_num;

        // Model に格納
        model->mAttrNum    = attr_num;
        model->mAttributes = attributes;
        model->mMaterials  = materials;

        //-----------------------------------------------------
	    // ボーン数取得
	    lnU32 frame_num = *((lnU16 *)data);
	    data += sizeof( lnU16 );

	    // ボーン配列を作成
	    ModelFrame* model_frames = LN_NEW ModelFrame[ frame_num ];

	    for ( lnU16 i = 0 ; i < frame_num ; ++i )
	    {
		    model_frames[ i ].initializePMD( this, (const PMD_Bone *)data, i, model_frames );

            mFrameIndexMap.insert( FrameIndexPair( model_frames[ i ].getName(), i ) );

		    data += sizeof( PMD_Bone );
	    }

        // 全ボーンの初期化が終了した後に、親のあるボーンのオフセットを再計算
        for ( lnU16 i = 0 ; i < frame_num ; ++i ) model_frames[i].postInitializePMD();

        // Model に格納 TODO:gccとかだとSJISじゃないのでだめかも
        lnU32 root_frame_idx = getFrameIndexByName( _T( "センター" ) );
        if ( root_frame_idx == LN_INVALID_INDEX ) // PMD なら"センター"があるはずだけど、念のため無かった場合は親のない最初のフレームをルートとする
        {
            printf( "PMD : センター ボーンがありません\n" );
        }
        model->mFrameNum    = frame_num;
        model->mModelFrames = model_frames;
        model->mRootFrame   = &model_frames[ root_frame_idx ];
        for ( lnU32 i = 0; i < frame_num; ++i )
        {
            model_frames[ i ].updateWorldMatrixFromLocalAndParent();

            // ルートフレーム以外の親のないフレームのインデックスリスト
            if ( i != root_frame_idx && model_frames[ i ].getParent() == NULL )
            {
                model->mNoParentFrameIndexArray.push_back( i );
            }
        }

        //-----------------------------------------------------
	    // IK数取得
	    mIKNum = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // IK配列を作成
	    if ( mIKNum )
	    {
		    mIKArray = LN_NEW ModelIK[ mIKNum ];

		    for( unsigned short i = 0 ; i < mIKNum ; ++i )
		    {
			    mIKArray[ i ].initializePMD( this, (const PMD_IK *)data, model_frames );
			    data += sizeof( PMD_IK ) + sizeof(unsigned short) * (((PMD_IK *)data)->cbNumLink - 1);
		    }
		    qsort( mIKArray, mIKNum, sizeof( ModelIK ), cmpIKDataFunc );
	    }

        mIKLimitter = LN_NEW ModelIKLimitter();

        
	    //-----------------------------------------------------
	    // 表情数取得
	    lnU16 faceCount = *((unsigned short *)data);
	    data += sizeof( unsigned short );

	    // 表情配列を作成
	    if ( faceCount )
	    {
			mModelFaceCoreArray.resize( faceCount );
		    for( unsigned short i = 0 ; i < faceCount ; ++i )
		    {
				mModelFaceCoreArray[i] = LN_NEW ModelFaceCore();
			    mModelFaceCoreArray[i]->initializePMD( (const PMD_Face *)data, mModelFaceCoreArray[0] );
			    data += sizeof( PMD_Face ) + sizeof(PMD_FaceVtx) * (((PMD_Face *)data)->ulNumVertices - 1 );
		    }
	    }


		model->mModelFaceArray.resize( faceCount );
		for ( int i = 0; i < faceCount; ++i )
		{
			model->mModelFaceArray[i] = LN_NEW ModelFace( model, mModelFaceCoreArray[i] );
		}
		
        //-----------------------------------------------------
	    // ここから剛体情報まで読み飛ばし

	    // 表情枠用表示リスト
	    unsigned char	cbFaceDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( unsigned short ) * cbFaceDispNum;

	    // ボーン枠用枠名リスト
	    unsigned char	cbBoneDispNum = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    data += sizeof( char ) * 50 * cbBoneDispNum;

	    // ボーン枠用表示リスト
	    unsigned long	ulBoneFrameDispNum = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    data += 3 * ulBoneFrameDispNum;

	    // 後続データの有無をチェック
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return;

	    // 英語名対応
	    unsigned char	cbEnglishNameExist = *((unsigned char *)data);
	    data += sizeof( unsigned char );

	    if( cbEnglishNameExist )
	    {
		    // モデル名とコメント(英語)
            char* name = (char*)data;
		    data += sizeof( char ) * 20;

            char* comment = (char*)data;
            data += sizeof( char ) * 256;

		    // ボーン名(英語)
            for ( lnU32 i = 0; i < frame_num; ++i )
            {
                //char* name = (char*)data;
                //printf( "%d : %s\n", i, name );
		        data += 20;
            }

		    // 表情名(英語)
            for ( lnU32 i = 0; i < faceCount - 1; ++i )	// "base"は英語名リストには含まれない
            {
                //LRefTString name( (char*)data, 20 );
                //printf( "%d : %s\n", i, name.c_str() );
		        data += 20;
            }

		    // ボーン枠用枠名リスト(英語)
		    data += sizeof( char ) * 50 * (cbBoneDispNum);
	    }

	    // 後続データの有無をチェック(ここのチェックは不要かも)
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return;

	    // トゥーンテクスチャリスト
        for ( int i = 0; i < 10; ++i )
        {
            //char* name = (char*)data;// ファイル名 100Byte * 10個
            data += 100;
        }	

	    // ここまで読み飛ばし
	    //-----------------------------------------------------

	    // 後続データの有無をチェック
	    if( (unsigned long)data - (unsigned long)data_top_ >= size_ )	return;

        //-----------------------------------------------------
	    // 剛体
        ModelRigidBody** rigidbodys = NULL;
	    lnU32 rigidbody_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // 剛体配列を作成
	    if ( rigidbody_num )
	    {
            rigidbodys = LN_NEW ModelRigidBody*[ rigidbody_num ];

		    for ( lnU32 i = 0 ; i < rigidbody_num ; ++i )
		    {
				rigidbodys[i] = LN_NEW ModelRigidBody();

			    const PMD_RigidBody* pmd_rigidbody = (const PMD_RigidBody *)data;

			    lnU32 frame_idx;
			    if ( pmd_rigidbody->unBoneIndex == 0xFFFF )	frame_idx = getFrameIndexByName( _T( "センター" ) );
			    else										frame_idx = pmd_rigidbody->unBoneIndex;


                if ( frame_idx != 0xffffffff )
                {
					//_p(pmd_rigidbody->szName);
					//pmd_rigidbody->vec3Position.dump();
					//pmd_rigidbody->vec3Rotation.dump();
					rigidbodys[ i ]->initializePMD( this->mManager, pmd_rigidbody, &model_frames[ frame_idx ] );
                }
			    data += sizeof( PMD_RigidBody );
		    }
	    }

        // Model に格納
        model->mRigidBodyNum    = rigidbody_num;
        model->mModelRigidBodys = rigidbodys;

        //-----------------------------------------------------
	    // コンストレイント
        ModelConstraint** constraints = NULL;
        lnU32 constraint_num = *((unsigned long *)data);
	    data += sizeof( unsigned long );

	    // コンストレイント配列を作成
	    if ( constraint_num )
	    {
		    constraints = LN_NEW ModelConstraint*[ constraint_num ];

		    for( lnU32 i = 0 ; i < constraint_num ; ++i )
		    {
			    const PMD_Constraint* pmd_constraint = (const PMD_Constraint *)data;

			    ModelRigidBody* body_a = rigidbodys[ pmd_constraint->ulRigidA ];
				ModelRigidBody* body_b = rigidbodys[ pmd_constraint->ulRigidB ];

				//if ( strcmp( pmd_constraint->szName, "スカート_0_0" ) == 0)
				//{
				//	//printf("const ModelRigidBody:%p %p\n", body_a, body_a->getRigidBody());
				//	body_a->getRigidBody()->getWorldMatrix().cdump();
				//}

                constraints[ i ] = LN_NEW ModelConstraint();
			    constraints[ i ]->initializePMD( mManager, pmd_constraint, body_a, body_b );
			    data += sizeof( PMD_Constraint );
		    }
	    }

        model->mConstraintNum       = constraint_num;
        model->mConstraints         = constraints;


        

        

        //-----------------------------------------------------
	    // 
        
		model->mMotionController    = NULL;//LN_NEW MotionController( model->mModelFrames, model->mFrameNum, &mModelFaceCoreArray[0], mModelFaceCoreArray.size() );
		model->mAnimationController	= LN_NEW ModelAnimationController( model );
		for ( int i = 0; i < model->mFrameNum; i++ )
		{
			model->mAnimationController->addAnimationApplyTarget( &model->mModelFrames[i] );
		}

		ln_foreach( ModelFace* face, model->mModelFaceArray )
		{
			model->mAnimationController->addAnimationApplyTarget( face );
		}
		

        model->resetRigidBody();

        //model->updateFrameMatrix();

        



        //SAFE_DELETE_ARRAY( model->mConstraints );



        // 変換テスト
        //if ( false )
        /*
        if ( !sw_skinning_ )
        {
            lnU32 new_attr_num;
            LNMeshAttributeRange* new_attrs;
            IVertexBuffer* new_vb;

            ConvertBlendedMesh< Vertex::PMDVertex, lnU16 > cbm;
            LN_CALL_R( cbm.convert(
                mGraphicsDevice,
                mVertexBuffer,
                mIndexBuffer,
                mAttrNum,
                mAttributeRanges,
                mFrameNum,
                2,
                26,
                &new_attr_num,
                &new_attrs,
                &new_vb ));

            LN_SAFE_RELEASE( mVertexBuffer );
            SAFE_DELETE_ARRAY( mAttributeRanges );
            mVertexBuffer = new_vb;
            mAttrNum = new_attr_num;
            mAttributeRanges = new_attrs;


            //_p(mIndexBuffer->getIndexNum());

            //SAFE_DELETE_ARRAY( mAttributeRanges );
            //mAttrNum = new_attr_num;
            //mAttributeRanges = new_attrs;
        }
        */

        if ( flags_ & ModelCreateFlag_CollisionMesh )
        {
            _createCollisionBuffers();
        }

        *model_ = model;
    }
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
#if 0
    bool ModelCore::loadPMX( const void* dataTop, lnU32 size, lnU32 flags, Model** outModel )
	{
		bool isDynamic = ( flags & ModelCreateFlag_SoftwareSkinning );
        const lnByte* data = static_cast< const lnByte* >( dataTop );

		// スキンメッシュ
        //mIsSkinMesh = true;

		FileIO::InUnManagedMemoryStream stream;
		stream.create( data, size );
		FileIO::BinaryReader reader( &stream );


        //-----------------------------------------------------
	    // ヘッダ
		
		PMX_Header pmxHeader;// = (PMX_Header*)stream.getCurrentBuffer();// = (PMX_Header*)data;
		reader.read( &pmxHeader, sizeof(PMX_Header) );
		//data += sizeof( PMX_Header );

		if ( pmxHeader.Magic[0] != 'P' ||
			 pmxHeader.Magic[1] != 'M' ||
			 pmxHeader.Magic[2] != 'X' ||
			 pmxHeader.Magic[3] != ' ')
		{
			return false;
		}

		if (pmxHeader.Version != 2.0f) return false;

		PMX_Encode encode = (PMX_Encode)pmxHeader.Data[0];
		int vertexIndexSize = pmxHeader.Data[2];
		int textureIndexSize = pmxHeader.Data[3];
		int boneIndexSize = pmxHeader.Data[5];

#if 0
		_p( pmxHeader.Version );
		printf( "DataSize              : %d\n", pmxHeader.DataSize );
		printf( "エンコード方式        : %d\n", pmxHeader.Data[0]);
		printf( "追加UV数              : %d\n", pmxHeader.Data[1]);
		printf( "頂点Indexサイズ       : %d\n", pmxHeader.Data[2]);
		printf( "テクスチャIndexサイズ : %d\n", pmxHeader.Data[3]);
		printf( "材質Indexサイズ       : %d\n", pmxHeader.Data[4]);
		printf( "ボーンIndexサイズ     : %d\n", pmxHeader.Data[5]);
		printf( "モーフIndexサイズ     : %d\n", pmxHeader.Data[6]);
		printf( "剛体Indexサイズ       : %d\n", pmxHeader.Data[7]);
#endif

		Model* model = LN_NEW Model( mManager );
        model->mModelCore = this;

		//-----------------------------------------------------
	    // モデル情報

		// モデル名
		lnS32 len = reader.readInt32();
		reader.seek( len );

		// モデル名英
		len = reader.readInt32();
		reader.seek( len );

		// コメント
		len = reader.readInt32();
		reader.seek( len );

		// コメント英
		len = reader.readInt32();
		reader.seek( len );

		//Base::UnicodeString modelName;
		//if ( encode == PMX_Encode_UTF16 )
		//{
		//	modelName.setDecodeBytes( data, len, Base::UnicodeString::getUTF16Codec() );
		//}

		//-----------------------------------------------------
	    // 頂点

		// 頂点数
		int vertexCount = reader.readInt32();

		// 頂点バッファ作成
		mVertexBuffer = mGraphicsDevice->createVertexBuffer( 
			PMX_Vertex::elements(), vertexCount, NULL, isDynamic );

        // ソフトウェアスキニング用
        if ( flags & ModelCreateFlag_SoftwareSkinning )
        {
            mOrgVertexPositions = LN_NEW LVector3[ vertexCount ];
            mOrgVertexNormals   = LN_NEW LVector3[ vertexCount ];
        }

	    // データを流し込む
		struct BaseVertex
		{
			LVector3	Position;
			LVector3	Normal;
			LVector2	TexUV;
		} baseVertex;
        PMX_Vertex* vertices = (PMX_Vertex*)mVertexBuffer->lock();
        PMX_Vertex* v;
        for ( lnU32 i = 0 ; i < vertexCount; ++i )
	    {
			v = &vertices[i];

			// 頂点、法線、テクスチャUV
			reader.read( &baseVertex, sizeof(BaseVertex) );
			v->Position = baseVertex.Position;
			v->Normal = baseVertex.Normal;
			v->TexUV = baseVertex.TexUV;

			// ブレンドウェイト
			int defType = reader.readInt8();
			switch ( defType )
			{
			case 0:	// BDEF1
				v->BlendIndices[0] = reader.readInt( boneIndexSize );
				v->BlendIndices[1] = 0.0f;
				v->BlendIndices[2] = 0.0f;
				v->BlendIndices[3] = 0.0f;
				v->BlendWeights[0] = 1.0f;
				v->BlendWeights[1] = 0.0f;
				v->BlendWeights[2] = 0.0f;
				v->BlendWeights[3] = 0.0f;
				break;
			case 1:	// BDEF2
				v->BlendIndices[0] = reader.readInt( boneIndexSize );
				v->BlendIndices[1] = reader.readInt( boneIndexSize );
				v->BlendIndices[2] = 0.0f;
				v->BlendIndices[3] = 0.0f;
				v->BlendWeights[0] = reader.readFloat();
				v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
				v->BlendWeights[2] = 0.0f;
				v->BlendWeights[3] = 0.0f;
				break;
			case 2:	// BDEF4
				v->BlendIndices[0] = reader.readInt( boneIndexSize );
				v->BlendIndices[1] = reader.readInt( boneIndexSize );
				v->BlendIndices[2] = reader.readInt( boneIndexSize );
				v->BlendIndices[3] = reader.readInt( boneIndexSize );
				v->BlendWeights[0] = reader.readFloat();
				v->BlendWeights[1] = reader.readFloat();
				v->BlendWeights[2] = reader.readFloat();
				v->BlendWeights[3] = reader.readFloat();
				break;
			case 3:	// SDEF
				v->BlendIndices[0] = reader.readInt( boneIndexSize );
				v->BlendIndices[1] = reader.readInt( boneIndexSize );
				v->BlendIndices[2] = 0.0f;
				v->BlendIndices[3] = 0.0f;
				v->BlendWeights[0] = reader.readFloat();
				v->BlendWeights[1] = 1.0f - v->BlendWeights[0];
				v->BlendWeights[2] = 0.0f;
				v->BlendWeights[3] = 0.0f;
				reader.read( &v->SdefC, sizeof(lnFloat) * 3 );
				reader.read( &v->SdefR0, sizeof(lnFloat) * 3 );
				reader.read( &v->SdefR1, sizeof(lnFloat) * 3 );	// TODO:※修正値を要計算
				break;
			}

			// エッジ倍率
			v->EdgeWeight = reader.readFloat();

			// ソフトウェアスキニング用
            if ( flags & ModelCreateFlag_SoftwareSkinning )
            {
                mOrgVertexPositions[ i ] = v->Position;
                mOrgVertexNormals[ i ]   = v->Normal;
            }
	    }

        mVertexBuffer->unlock();

		//-----------------------------------------------------
	    // 頂点インデックス

		// インデックス数
		int indexCount = reader.readInt32();

		// インデックスバッファ作成
		mIndexBuffer = mGraphicsDevice->createIndexBuffer( 
			indexCount, NULL, isDynamic, (vertexIndexSize <= 2) );

		// 1 バイトインデックス
		if ( vertexIndexSize == 1 )
		{
			//TODO:未対応
			LN_ERR2_ASSERT( 0 );
		}
		// 2 or 4 バイトインデックス
		else
		{
			// そのままコピー
			lnByte* indices = (lnByte*)mIndexBuffer->lock();
			reader.read( indices, vertexIndexSize * indexCount );
			mIndexBuffer->unlock();
		}

		//-----------------------------------------------------
	    // テクスチャ

		// テクスチャ数
		int textureCount = reader.readInt32();

		// テクスチャテーブル
		std::vector<Texture*> textureTable;
		textureTable.reserve( textureCount );

		for (int i = 0 ; i < textureCount; ++i )
		{
			// テクスチャ名
			lnS32 len = reader.readInt32();
			Base::UnicodeString str;
			if ( encode == PMX_Encode_UTF16 )
				str.setDecodeBytes( stream.getCurrentBuffer(), len, Base::UnicodeString::getUTF16Codec() );
			else
				str.setDecodeBytes( stream.getCurrentBuffer(), len, Base::UnicodeString::getUTF8Codec() );
			reader.seek( len );
			textureTable.push_back( mManager->createTexture( str.getString(), flags ) );
		}

		//-----------------------------------------------------
	    // マテリアル・属性テーブル

		// マテリアル数
		int materialCount = reader.readInt32();

		// メモリ確保
		LMaterial* materials  = LN_NEW LMaterial[ materialCount ];
        LNMeshAttribute* attributes = LN_NEW LNMeshAttribute[ materialCount ];

		// Model に格納 (例外で抜けたときの解放のため、先に)
        model->mAttrNum    = materialCount;
        model->mAttributes = attributes;
        model->mMaterials  = materials;

		int indexAttrOffset = 0;
		for ( int i = 0 ; i < materialCount; ++i )
		{
			LMaterial* m = &materials[ i ];

			// 材質名
			lnS32 len = reader.readInt32();
			reader.seek( len );

			// 材質英名
			len = reader.readInt32();
			reader.seek( len );

			// Diffuse
			LVector4 color;
			reader.read( &color, sizeof(lnFloat) * 4 );
			m->Diffuse.set( color.x, color.y, color.z, color.w );

			// Specular
			reader.read( &color, sizeof(lnFloat) * 3 );
			m->Specular.set(color.x, color.y, color.z, 1.0);

			// Specular係数
			m->Power = reader.readFloat();

			// Ambient
			reader.read(&color, sizeof(lnFloat) * 3);
			m->Ambient.set(color.x, color.y, color.z, 1.0);

			// 描画フラグ
			lnU8 bitFlag = reader.readInt8();
			m->DrawingFlags = bitFlag;

			// エッジ色
			reader.read( &m->EdgeColor, sizeof(lnFloat) * 4 );

			// エッジサイズ
			m->EdgeSize = reader.readFloat();

			// テクスチャ
			int normalTexture = reader.readInt( textureIndexSize );
			if ( normalTexture >= 0 )
				m->setTexture( textureTable[normalTexture] );

			// スフィアテクスチャ
			int sphereTexture = reader.readInt( textureIndexSize );
			m->SphereMode = (enum Material::SphereMode)reader.readInt8();
			
			// トゥーンテクスチャ
			int shareToon = reader.readInt8();
			if ( shareToon == 0 )
			{
				int index = reader.readInt( textureIndexSize );
				if ( index >= 0 )
					m->setToonTexture( textureTable[index] );
			}
			// 共有Toonテクスチャ (toon01.bmp～toon10.bmp)
			else
			{
				int index = reader.readInt8();
				m->setToonTexture( mManager->getMMDDefaultToonTexture( index ) );
			}

			// 自由欄
			len = reader.readInt32();
			reader.seek( len );

			// マテリアルに対応する頂点数
			int vc = reader.readInt32();

			// 属性テーブルを埋める
            attributes[ i ].MaterialIndex = i;
            attributes[ i ].StartIndex = indexAttrOffset;
			attributes[ i ].PrimitiveNum = vc / 3;
			indexAttrOffset += vc;
		}

		//-----------------------------------------------------
	    // 後始末

		ln_foreach( Texture* tex, textureTable )
		{
			LN_SAFE_RELEASE( tex );
		}

		*outModel = model;
		return true;
	}
#endif

	//----------------------------------------------------------------------
	// ● 名前からフレーム番号を取得する (見つからない場合は 0xffffffff)
	//----------------------------------------------------------------------
    lnU32 ModelCore::getFrameIndexByName( const lnChar* name_ )
    {
        FrameIndexMap::iterator it = mFrameIndexMap.find( name_ );
        if ( it != mFrameIndexMap.end() )
        {
            return it->second;
        }
        return 0xffffffff;
    }


    void skinningVertex(
        Vertex::PMDVertex* v_,
        const LVector3& ipos,
        const LVector3& inor,
        const LMatrix& skinning_mat,
        lnFloat weight_ )
    {
        LVector3 tmp_vec;

		tmp_vec = LVector3::TransformCoord(ipos, skinning_mat);
        v_->Position += tmp_vec * weight_;

		tmp_vec = LVector3::TransformCoord(inor, skinning_mat.GetRotationMatrix());
        v_->Normal   += tmp_vec * weight_;
    }

	//----------------------------------------------------------------------
	// ● ソフトウェアスキニング
	//----------------------------------------------------------------------
    void ModelCore::skinning( ModelFrame* frames_ )
    {
        static const int EFF_BONE_NUM = 2;

        //LMatrix tmp_mat;
        LVector3 tem_vec;
        lnU32 v_num = mVertexBuffer->getVertexCount();
        Vertex::PMDVertex* vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        Vertex::PMDVertex* v;
        LVector3* ipos;
        LVector3* inor;
        for ( lnU32 i = 0; i < v_num; ++i )
        {
            v    = &vertices[ i ];
            ipos = &mOrgVertexPositions[ i ];
            inor = &mOrgVertexNormals[ i ];
            v->Position = LVector3::Zero;
            v->Normal   = LVector3::Zero;


            // 最後の一つ以外の影響ボーン適用
            lnU32 bone_idx = 0;
            lnFloat last_weight = 0.0f;
            for ( ; bone_idx < EFF_BONE_NUM - 1; ++bone_idx )
            {
                skinningVertex(
                    v, *ipos, *inor,
                    frames_[ (int)v->BlendIndices[ bone_idx ] ].getSkinningMatrix(),
                    v->BlendWeights[ bone_idx ] );

                last_weight += v->BlendWeights[ bone_idx ];
            }

            // 最後の影響ボーン適用
            skinningVertex(
                v, *ipos, *inor,
                frames_[ (int)v->BlendIndices[ bone_idx ] ].getSkinningMatrix(),
                1.0f - last_weight );

            //if ( i == 17957 )//i == 5985 )
            //{
            //    v->Position.cdump();
            //    //_p(last_weight);
            //    frames_[ v->BlendIndices[ 0 ] ].getSkinningMatrix().cdump();
            //    //ipos->cdump();
            //    //v->Position.cdump();
            //    //_p( v->BlendWeights[ 0 ] );
            //    //_p( v->BlendWeights[ 1 ] );
            //}
            

            /*
            const LMatrix&  skinning_mat = frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix();
            last_weight = 1.0f - last_weight;

            LVector3::transform( &tem_vec, (*ipos), skinning_mat );
            v->Position += tem_vec * last_weight;

            LVector3::transform( &tem_vec, (*inor), skinning_mat );
            v->Normal   += tem_vec * last_weight;
            */

            /*
            tem_vec = (*ipos) + frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix().getPosition();
            v->Position += tem_vec * last_weight;
            
            LVector3::rotate( &tem_vec, *inor, frames_[ v->BlendIndices[ bone_idx ] ].getSkinningMatrix() );
            v->Normal   += tem_vec * last_weight;
            */


            v->Normal.Normalize();
        }

        mVertexBuffer->unlock();

    }

	//----------------------------------------------------------------------
	// ● モデルデータ解放
	//----------------------------------------------------------------------
    void ModelCore::_release()
    {
        LN_SAFE_RELEASE( mVertexBuffer );
        LN_SAFE_RELEASE( mIndexBuffer );
        SAFE_DELETE_ARRAY( mOrgVertexPositions );
        SAFE_DELETE_ARRAY( mOrgVertexNormals );
        if ( mCollisionMesh )
        {
            SAFE_DELETE_ARRAY( mCollisionMesh->CollisionVertexBuffer );
            SAFE_DELETE_ARRAY( mCollisionMesh->CollisionIndexBuffer );
            SAFE_DELETE( mCollisionMesh );
        }
        
        SAFE_DELETE_ARRAY( mIKArray );
        SAFE_DELETE( mIKLimitter );

		ln_foreach( ModelFaceCore* face, mModelFaceCoreArray )
		{
			face->release();
		}
		mModelFaceCoreArray.clear();
    }

	//----------------------------------------------------------------------
	// ● スフィアマップ用のテクスチャ名かを判定する
	//----------------------------------------------------------------------
    bool ModelCore::_isSphereMapTexName( const char* tex_name_ ) const
    {
	    int		len = strlen( tex_name_ );
	    bool	r = false;

	    if ( ( tex_name_[ len - 3 ] == 's' || tex_name_[ len - 3 ] == 'S') &&
		     ( tex_name_[ len - 2 ] == 'p' || tex_name_[ len - 2 ] == 'P') &&
		     ( tex_name_[ len - 1 ] == 'h' || tex_name_[ len - 1 ] == 'H') )
	    {
		    r = true;
	    }

	    return r;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelCore::_createCollisionBuffers()
    {
        mCollisionMesh = LN_NEW CollisionMesh;

        //-----------------------------------------------------
        // 頂点バッファ

        lnU32 v_num = mVertexBuffer->getVertexCount();
        LVector3* vertices = LN_NEW LVector3[ v_num ];

        Vertex::PMDVertex* org_vertices = (Vertex::PMDVertex*)mVertexBuffer->lock();
        for ( lnU32 i = 0 ; i < v_num; ++i )
	    {
            vertices[i] = org_vertices[i].Position;
	    }
        mVertexBuffer->unlock();

        mCollisionMesh->VertexCount = v_num;
        mCollisionMesh->CollisionVertexBuffer = vertices;

        //-----------------------------------------------------
	    // インデックスバッファ

        lnU32 i_num = mIndexBuffer->getIndexCount();
        lnU32 stride = mIndexBuffer->getIndexStride();
#if 1
        lnByte* indices = LN_NEW lnByte[i_num * 4];
        lnByte* org_indices = (lnByte*)mIndexBuffer->lock();

        if ( stride == 2 )
        {
            for ( lnU32 i = 0; i < i_num; ++i )
            {
                *((lnU32*)&indices[i * 4]) = *((lnU16*)&org_indices[i * stride]);
            }
        }
        else
        {
            for ( lnU32 i = 0; i < i_num; ++i )
            {
                *((lnU32*)&indices[i * 4]) = *((lnU32*)&org_indices[i * stride]);
            }
        }
        mIndexBuffer->unlock();

        mCollisionMesh->IndexCount = i_num;
        mCollisionMesh->IndexStride = 4;
        mCollisionMesh->CollisionIndexBuffer = indices;
#else
        lnByte* indices = LN_NEW lnByte[i_num * stride];
        
        memcpy( indices, mIndexBuffer->lock(), i_num * stride );
        mIndexBuffer->unlock();

        mCollisionMesh->IndexCount = i_num;
        mCollisionMesh->IndexStride = stride;
        mCollisionMesh->CollisionIndexBuffer = indices;
#endif
    }
 
//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================