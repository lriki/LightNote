//==============================================================================
// VMDLoader 
//==============================================================================

#include "stdafx.h"
#include "../../../FileIO/StreamObject.h"
#include "../../../FileIO/BinaryReader.h"
#include "VMDLoader.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
  
//==============================================================================
// VMDLoader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	VMDLoader2::~VMDLoader2()
	{
		ln_foreach( BoneAnimation& anim, mBoneAnimationArray )
		{
			anim.Animation->release();
		}
		ln_foreach( FaceAnimation& anim, mFaceAnimationArray )
		{
			anim.Animation->release();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool VMDLoader2::load( FileIO::Stream* stream )
	{
		mLastFramePosition = 0.0;
		FileIO::BinaryReader reader( stream );

		//-----------------------------------------------------
        // ヘッダのチェック

		VMD_Header vmdHeader;
		reader.read( &vmdHeader, sizeof(VMD_Header) );
		if ( strncmp( vmdHeader.szHeader, "Vocaloid Motion Data 0002", 30 ) != 0 )
		    return false;

		//-----------------------------------------------------
        // ボーンアニメーション

		lnU32 allBoneKeyCount = reader.readUInt32();
		for ( lnU32 i = 0 ; i < allBoneKeyCount ; ++i )
	    {
			VMD_Motion vmdMotion;
			reader.read( &vmdMotion, sizeof(VMD_Motion) );

			// 最大フレーム更新
		    if ( mLastFramePosition < (float)vmdMotion.ulFrameNo ) {	
				mLastFramePosition = (float)vmdMotion.ulFrameNo;	
			}

            // 既存アニメーション検索
            BoneAnimationIndexMap::iterator itr = 
				mBoneAnimationIndexMap.find( vmdMotion.szBoneName );

            // 新しいボーン
            if ( itr == mBoneAnimationIndexMap.end() )
            {
				BoneAnimation anim;
				anim.TargetBoneName = lnRefString( vmdMotion.szBoneName );

                // 名前・インデックスの対応
                mBoneAnimationIndexMap.insert( 
					BoneAnimationIndexPair( anim.TargetBoneName, mBoneAnimationArray.size() ) );

                // アニメーション作成、キー追加
				anim.Animation = LN_NEW Animation::BezierSQTTransformAnimation();
                anim.Animation->addKeyFrame(
                    static_cast< double >( vmdMotion.ulFrameNo ),
                    vmdMotion.vec3Position,
                    vmdMotion.vec4Rotation,
                    vmdMotion.cInterpolationX,
                    vmdMotion.cInterpolationY,
                    vmdMotion.cInterpolationZ,
                    vmdMotion.cInterpolationRot );
                mBoneAnimationArray.push_back( anim );
            }
            // すでにあるボーン
            else
            {
				mBoneAnimationArray[ (itr->second) ].Animation->addKeyFrame(
                    static_cast< double >( vmdMotion.ulFrameNo ),
                    vmdMotion.vec3Position,
                    vmdMotion.vec4Rotation,
                    vmdMotion.cInterpolationX,
                    vmdMotion.cInterpolationY,
                    vmdMotion.cInterpolationZ,
                    vmdMotion.cInterpolationRot );
            }
		}

		// キーフレーム順にソート
		ln_foreach( BoneAnimation& anim, mBoneAnimationArray )
		{
			anim.Animation->sortKeyFrame();
		}

		//-----------------------------------------------------
	    // 表情アニメーション

        lnU32 allFaceKeyCount = reader.readUInt32();
        for ( lnU32 i = 0 ; i < allFaceKeyCount ; ++i )
	    {
			VMD_Face vmdFace;
			reader.read( &vmdFace, sizeof(VMD_Face) );

            // 最大フレーム更新
		    if ( mLastFramePosition < (float)vmdFace.ulFrameNo )	
				mLastFramePosition = (float)vmdFace.ulFrameNo;	

            // 既存アニメーション検索
            FaceAnimationIndexMap::iterator itr = 
				mFaceAnimationIndexMap.find( vmdFace.szFaceName );

            // 新しい表情
            if ( itr == mFaceAnimationIndexMap.end() )
            {
				FaceAnimation anim;
				anim.TargetFaceName = lnRefString( vmdFace.szFaceName );

                // 名前・インデックスの対応
                mFaceAnimationIndexMap.insert( 
					FaceAnimationIndexPair( anim.TargetFaceName, mFaceAnimationArray.size() ) );

                // アニメーション作成、キー追加
				anim.Animation = LN_NEW Animation::ScalarAnimation();
				anim.Animation->addKeyFrame( vmdFace.ulFrameNo, vmdFace.fFactor );
                mFaceAnimationArray.push_back( anim );
            }
            // すでにある表情
            else
            {
				mFaceAnimationArray[ (itr->second) ].Animation->addKeyFrame( 
					vmdFace.ulFrameNo, vmdFace.fFactor );
            }
	    }

#if 0	// ScalarAnimation::addKeyFrame() は追加時にソートされるため必要ない
        // キーフレーム順にソート
		ln_foreach( FaceAnimation& anim, mFaceAnimationArray )
		{
			anim.Animation->sortKeyFrame();
		}
#endif
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
