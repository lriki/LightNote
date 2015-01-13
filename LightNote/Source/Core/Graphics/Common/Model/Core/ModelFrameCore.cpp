//==============================================================================
// ModelFrameCore 
//==============================================================================

#include "stdafx.h"
#include "ModelCore.h"
#include "ModelIKLimitter.h"
#include "ModelFrameCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelFrameCore
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelFrameCore::ModelFrameCore()
		: OwnerModelCore	( NULL )
		, FrameIndex		( -1 )
		, ParentFrameIndex	( -1 )
		, IKLimitter		( NULL )
		, TransformLevel	( 0 )

		, IsRotateProvided	( false )
		, IsMoveProvided	( false )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFrameCore::~ModelFrameCore()
	{
		LN_SAFE_RELEASE( IKLimitter );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrameCore::initialize( ModelCore2* owner, int thisIndex, int parentIndex, const lnRefString& name, const LVector3& orgPos )
	{
		OwnerModelCore = owner;
		FrameIndex = thisIndex;
		ParentFrameIndex = parentIndex;
		Name = name;
		OrgPosition = orgPos;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelFrameCore::postInitialize()
	{
			//_p(depth);

        if ( ParentFrameIndex != -1 )
        {
			OffsetFromParent = OrgPosition - OwnerModelCore->FrameArray[ParentFrameIndex]->OrgPosition;
			OwnerModelCore->FrameArray[ParentFrameIndex]->ChildFrameIndexArray.push_back( FrameIndex );
        }

		/*
			初期姿勢は、スキニングしなくても同じ姿勢。
			つまり、頂点スキニングで初期姿勢にしたいときは Matrix::Identity のボーン行列を渡す。
			


            
            ボーンは最初からオフセットが入ってるけど、
            それをスキニングに適用すると姿勢が崩れてしまう。
            そのため、初期オフセットを打ち消す処理が必要。それがこの行列。

            ID3DXSkinInfo::GetBoneOffsetMatrix() で
            取得できる行列 (SkinnedMeshサンプルの D3DXMESHCONTAINER_DERIVED::pBoneOffsetMatrices) が
            これにあたるものっぽい。
            サンプルでも描画の直前に対象ボーン行列にこの行列を乗算している。
        */
        LMatrix::translation(
            &mSkinningOffsetInv,
            -OrgPosition.x,
            -OrgPosition.y,
            -OrgPosition.z );

		//ln_foreach( int index, ChildFrameIndexArray )
		//{
		//	OwnerModelCore->FrameArray[index]->postInitialize( depth + 1 );
		//}

	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
