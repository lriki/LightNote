//==============================================================================
// Sprite 
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "Sprite.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Sprite::DrawingContext
//==============================================================================
class Sprite::DrawingContext
    : public VisualNodeContext
{
public:
	DrawingContext(){}
	virtual ~DrawingContext() {}

protected:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void updateContext( SceneNodeContext* parentContext )
    {
        VisualNodeContext::updateContext( parentContext );

		Sprite* ownerNode = static_cast<Sprite*>( mSceneNode );
		mUpperLeft		= ownerNode->mUpperLeft;
		mLowerRight		= ownerNode->mLowerRight;
		mUVUpperLeft	= ownerNode->mUVUpperLeft;
		mUVLowerRight	= ownerNode->mUVLowerRight;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void drawSubset( ContextDrawParam& param, int index )
    {
		param.GeometryRenderer->drawSquare(
            mUpperLeft.x,   mUpperLeft.y,  0xffffffff, mUVUpperLeft.x,  mUVUpperLeft.y,
            mLowerRight.x,  mUpperLeft.y,  0xffffffff, mUVLowerRight.x, mUVUpperLeft.y,
            mUpperLeft.x,   mLowerRight.y, 0xffffffff, mUVUpperLeft.x,  mUVLowerRight.y,
            mLowerRight.x,  mLowerRight.y, 0xffffffff, mUVLowerRight.x, mUVLowerRight.y );
    }

private:
    LVector3	mUpperLeft;
    LVector3	mLowerRight;
    LVector2	mUVUpperLeft;
    LVector2	mUVLowerRight;
};

//==============================================================================
// ■ Sprite
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Sprite);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sprite::Sprite( SceneGraph* scene )
        : VisualNode        ( scene )
        , mFlipFlags        ( LN_FLIP_NONE )
        , m3DAADir          ( LN_AADIR_Z )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sprite::~Sprite()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::create()
    {
		Sprite::DrawingContext* context = LN_NEW Sprite::DrawingContext();
		context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_2D, context );
		mSrcRect.set( 0, 0, -1, -1 );
		setSize( LVector2( -1, -1 ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::create3D( /*lnFloat widht, lnFloat height, */LNAxisAlignedDir frontDir )
    {
		Sprite::DrawingContext* context = LN_NEW Sprite::DrawingContext();
		context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

        m3DAADir = frontDir;
        //_updateSize( LVector2( widht, height ) );
		mSrcRect.set( 0, 0, -1, -1 );
		setSize( LVector2( -1, -1 ) );


        setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::setTexture( Graphics::Texture* texture )
    {
        this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture );
        _updateTexUV();
		_updateVertexData();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Graphics::Texture* Sprite::getTexture()
    {
        return this->mNodeRenderParam.Subset[ 0 ].Material.getTexture();
    }


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::setSrcRect( const LRectF& rect )
    {
        mSrcRect = rect;
        _updateTexUV();
		_updateVertexData();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::_updateTexUV()
    {
        Graphics::Texture* tex = getTexture();

        if ( tex )
        {
			// 転送元矩形が負値ならテクスチャ全体を転送する
			if ( mSrcRect.w < 0 && mSrcRect.h < 0 )
			{
				LRect srcRect( 0, 0, (int)tex->getSize().x, (int)tex->getSize().y );
				Util::normalizeSrcRect( 
					&mUVUpperLeft.x, 
					&mUVUpperLeft.y, 
					&mUVLowerRight.x, 
					&mUVLowerRight.y, 
					srcRect, 
					tex->getRealSize() );
			}
			else
			{
				lnFloat l, t, r, b;
				Util::normalizeSrcRect( &l, &t, &r, &b, mSrcRect, tex->getRealSize() );

				// 垂直反転
				if ( mFlipFlags & LN_FLIP_V )
				{
					mUVUpperLeft.y = b;
					mUVLowerRight.y = t;
				}
				else
				{
					mUVUpperLeft.y = t;
					mUVLowerRight.y = b;
				}
				// 水平反転
				if ( mFlipFlags & LN_FLIP_H )
				{
					mUVUpperLeft.x = r;
					mUVLowerRight.x = l;
				}
				else
				{
					mUVUpperLeft.x = l;
					mUVLowerRight.x = r;
				}
			}
		}
        else
        {
			mUVUpperLeft.x  = 0.0f;
			mUVUpperLeft.y  = 0.0f;
			mUVLowerRight.x = 1.0f;
			mUVLowerRight.y = 1.0f;
        }
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite::_updateVertexData()
	{
		// サイズが負値     → 転送矩形を使う
		// 転送矩形が負値   → テクスチャサイズを使う
		// テクスチャが無い → サイズ 0,0
		LVector2 realSize = mSize;
		if ( mSize.x < 0.0 && mSize.y < 0.0 ) 
		{
			if ( mSrcRect.w < 0.0 && mSrcRect.h < 0.0 )
			{
				Graphics::Texture* tex = getTexture();
				if ( tex ) 
					realSize.set( tex->getSize().x, tex->getSize().y );
				else 
					realSize = LVector2::ZERO;
			}
			else
			{
				realSize.x = mSrcRect.w;
				realSize.y = mSrcRect.h;
			}
		}


		if ( is3DSprite() )
		{
			lnFloat r =  realSize.x * 0.5f;  // +
			lnFloat b = -realSize.y * 0.5f;  // -
			lnFloat l = -r;                  // -
			lnFloat t = -b;                  // +

			switch ( m3DAADir )
			{
				case LN_AADIR_X:
					mUpperLeft.set(  0, t, r );
					mLowerRight.set( 0, b, l );
					break;
				case LN_AADIR_Y:
					mUpperLeft.set(  l, 0, b );
					mLowerRight.set( r, 0, t );
					break;
				case LN_AADIR_RZ:
					mUpperLeft.set(  l, t, 0 );
					mLowerRight.set( r, b, 0 );
					break;
				case LN_AADIR_RX:
					mUpperLeft.set(  0, t, l );
					mLowerRight.set( 0, b, r );
					break;
				case LN_AADIR_RY:
					mUpperLeft.set(  r, 0, b );
					mLowerRight.set( l, 0, t );
					break;
				case LN_AADIR_Z:
					mUpperLeft.set(  r, t, 0 );
					mLowerRight.set( l, b, 0 );
					break;
			}
		}
		else 
		{
			// 転送元矩形が完全に 0 ならテクスチャ全体を転送する
			//if ( mSrcRect.x == 0 && mSrcRect.y == 0 && mSrcRect.w == 0 && mSrcRect.h == 0 )
			//{
			//	Graphics::ITexture* tex = getTexture();
			//	if ( tex ) 
			//	{
			//		mUpperLeft.set( 0, 0, 0 );
			//		mLowerRight.set( tex->getSize().x, tex->getSize().y, 0 );
			//	}
			//	else 
			//	{
			//		mUpperLeft.set( 0, 0, 0 );
			//		mLowerRight.set( 0, 0, 0 );
			//	}
			//}
			//else
			{
				lnFloat r = (lnFloat)realSize.x;
				lnFloat b = (lnFloat)realSize.y;
				lnFloat l = 0;
				lnFloat t = 0;

				mUpperLeft.set(  l, t, 0 );
				mLowerRight.set( r, b, 0 );
			}
		}
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================