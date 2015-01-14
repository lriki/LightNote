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
            mUpperLeft.X,   mUpperLeft.Y,  0xffffffff, mUVUpperLeft.X,  mUVUpperLeft.Y,
            mLowerRight.X,  mUpperLeft.Y,  0xffffffff, mUVLowerRight.X, mUVUpperLeft.Y,
            mUpperLeft.X,   mLowerRight.Y, 0xffffffff, mUVUpperLeft.X,  mUVLowerRight.Y,
            mLowerRight.X,  mLowerRight.Y, 0xffffffff, mUVLowerRight.X, mUVLowerRight.Y );
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
				LRect srcRect( 0, 0, (int)tex->getSize().X, (int)tex->getSize().Y );
				Util::normalizeSrcRect( 
					&mUVUpperLeft.X, 
					&mUVUpperLeft.Y, 
					&mUVLowerRight.X, 
					&mUVLowerRight.Y, 
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
					mUVUpperLeft.Y = b;
					mUVLowerRight.Y = t;
				}
				else
				{
					mUVUpperLeft.Y = t;
					mUVLowerRight.Y = b;
				}
				// 水平反転
				if ( mFlipFlags & LN_FLIP_H )
				{
					mUVUpperLeft.X = r;
					mUVLowerRight.X = l;
				}
				else
				{
					mUVUpperLeft.X = l;
					mUVLowerRight.X = r;
				}
			}
		}
        else
        {
			mUVUpperLeft.X  = 0.0f;
			mUVUpperLeft.Y  = 0.0f;
			mUVLowerRight.X = 1.0f;
			mUVLowerRight.Y = 1.0f;
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
		if ( mSize.X < 0.0 && mSize.Y < 0.0 ) 
		{
			if ( mSrcRect.w < 0.0 && mSrcRect.h < 0.0 )
			{
				Graphics::Texture* tex = getTexture();
				if ( tex ) 
					realSize.Set( tex->getSize().X, tex->getSize().Y );
				else 
					realSize = LVector2::Zero;
			}
			else
			{
				realSize.X = mSrcRect.w;
				realSize.Y = mSrcRect.h;
			}
		}


		if ( is3DSprite() )
		{
			lnFloat r =  realSize.X * 0.5f;  // +
			lnFloat b = -realSize.Y * 0.5f;  // -
			lnFloat l = -r;                  // -
			lnFloat t = -b;                  // +

			switch ( m3DAADir )
			{
				case LN_AADIR_X:
					mUpperLeft.Set(  0, t, r );
					mLowerRight.Set( 0, b, l );
					break;
				case LN_AADIR_Y:
					mUpperLeft.Set(  l, 0, b );
					mLowerRight.Set( r, 0, t );
					break;
				case LN_AADIR_RZ:
					mUpperLeft.Set(  l, t, 0 );
					mLowerRight.Set( r, b, 0 );
					break;
				case LN_AADIR_RX:
					mUpperLeft.Set(  0, t, l );
					mLowerRight.Set( 0, b, r );
					break;
				case LN_AADIR_RY:
					mUpperLeft.Set(  r, 0, b );
					mLowerRight.Set( l, 0, t );
					break;
				case LN_AADIR_Z:
					mUpperLeft.Set(  r, t, 0 );
					mLowerRight.Set( l, b, 0 );
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
				lnFloat r = (lnFloat)realSize.X;
				lnFloat b = (lnFloat)realSize.Y;
				lnFloat l = 0;
				lnFloat t = 0;

				mUpperLeft.Set(  l, t, 0 );
				mLowerRight.Set( r, b, 0 );
			}
		}
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================