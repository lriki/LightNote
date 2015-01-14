//==============================================================================
// ModelFaceCore 
//==============================================================================

#include "stdafx.h"
#include "ModelCore.h"
#include "ModelFaceCore.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelMorphCore
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelMorphCore::ModelMorphCore()
        : VerticesCount		( 0 )
        , TargetVertices	( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelMorphCore::~ModelMorphCore()
    {
        SAFE_DELETE_ARRAY( TargetVertices );
    }

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
 //   void ModelMorphCore::apply( Vertex::PMDVertex* vertices ) const
	//{
	//	if( !TargetVertices ) {
	//		return;
	//	}

	//	PMD_FaceVtx* v = TargetVertices;
	//	for ( lnU32 i = 0 ; i < VerticesCount ; ++i, ++v )
	//	{
	//		vertices[v->ulIndex].Position = v->vec3Pos;
	//	}
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
 //   void ModelMorphCore::blend( Vertex::PMDVertex* vertices, float rate ) const
 //   {
 //       if ( !TargetVertices || rate < 0.0001f ) {
	//		return;
	//	}

	//	if ( rate >= 1.0f ) {
	//		apply( vertices );
	//		return;
	//	}

	//    PMD_FaceVtx* v = TargetVertices;
	//    lnU32 idx;
	//    for ( lnU32 i = 0 ; i < VerticesCount ; ++i, ++v )
	//    {
	//	    idx = v->ulIndex;
 //           LVector3::lerp( &vertices[idx].Position, vertices[idx].Position, v->vec3Pos, rate );
	//    }
 //   }


//==============================================================================
// ■ ModelFaceCore
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFaceCore::ModelFaceCore()
        : mVerticesCount     ( 0 )
        , mFaceVertices     ( NULL )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    ModelFaceCore::~ModelFaceCore()
    {
        _release();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelFaceCore::initializePMD( const PMD_Face* faceData, const ModelFaceCore* baseFace )
    {
        _release();

        // 表情名のコピー
        char name[ 21 ];
        strncpy( name, faceData->szName, 20 );
	    name[ 20 ] = '\0';
        mName = name;

        // 頂点数
        mVerticesCount = faceData->ulNumVertices;

        // 頂点データのコピー
        mFaceVertices = LN_NEW PMD_FaceVtx[ mVerticesCount ];
        memcpy( mFaceVertices, faceData->pVertices, sizeof(PMD_FaceVtx) * mVerticesCount );

	    // baseとの相対インデックスを絶対インデックスに変換
	    if ( baseFace && baseFace != this )
	    {
		    PMD_FaceVtx	*v = mFaceVertices;
		    for ( lnU32 i = 0 ; i < mVerticesCount ; i++, v++ )
		    {
                v->vec3Pos = baseFace->mFaceVertices[ v->ulIndex ].vec3Pos + v->vec3Pos;
			    v->ulIndex = baseFace->mFaceVertices[ v->ulIndex ].ulIndex;
		    }
	    }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelFaceCore::applyFace( Vertex::PMDVertex* vertices ) const
	{
		if( !mFaceVertices ) {
			return;
		}

		PMD_FaceVtx* v = mFaceVertices;
		for ( lnU32 i = 0 ; i < mVerticesCount ; ++i, ++v )
		{
			vertices[v->ulIndex].Position = v->vec3Pos;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelFaceCore::blendFace( Vertex::PMDVertex* vertices, float rate ) const
    {
        if ( !mFaceVertices || rate < 0.0001f ) {
			return;
		}

		if ( rate == 1.0f ) {
			applyFace( vertices );
			return;
		}

	    PMD_FaceVtx* v = mFaceVertices;
	    lnU32 idx;
	    for ( lnU32 i = 0 ; i < mVerticesCount ; ++i, ++v )
	    {
		    idx = v->ulIndex;
			vertices[idx].Position = LVector3::Lerp(vertices[idx].Position, v->vec3Pos, rate);
	    }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void ModelFaceCore::_release()
    {
        SAFE_DELETE_ARRAY( mFaceVertices );
        mVerticesCount = 0;
    }

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================