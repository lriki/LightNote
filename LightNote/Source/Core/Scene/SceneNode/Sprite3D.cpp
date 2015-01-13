//==============================================================================
// Sprite3D 
//==============================================================================

#include "stdafx.h"
#include "../../Graphics/Manager.h"
#include "../../Graphics/Common/GeometryRenderer.h"
#include "../SceneGraph.h"
#include "Util.h"
#include "Sprite3D.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// ■ Sprite3D::DrawingContext
//==============================================================================
class Sprite3D::DrawingContext
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

		Sprite3D* ownerNode = static_cast<Sprite3D*>( mSceneNode );
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
// ■ Sprite3D
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sprite3D::Sprite3D( SceneGraph* scene )
        : VisualNode        ( scene )
        , mAADir            ( LN_AADIR_Z )
        , mFlipFlags        ( LN_FLIP_NONE )
        , mCalledSetSrcRect ( false )
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Sprite3D::~Sprite3D()
    {
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite3D::create( lnFloat widht, lnFloat height, LNAxisAlignedDir frontDir )
    {
		Sprite3D::DrawingContext* context = LN_NEW Sprite3D::DrawingContext();
		context->initialize( this );
        VisualNode::initialize( 1, LN_DRAWCOORD_3D, context );

        mAADir      = frontDir;
        setSize( LVector2( widht, height ) );

        setShader( this->mSceneGraph->getDefaultShader( LN_DEFSHADER_NOLIGHTING ) );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite3D::setSize( const LVector2& size )
    {
        mSize = size;

        lnFloat r = mSize.x * 0.5f;     // +
        lnFloat b = -mSize.y * 0.5f;    // -
        lnFloat l = -r;                 // -
        lnFloat t = -b;                 // +

        switch ( mAADir )
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

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite3D::setTexture( Graphics::Texture* texture )
    {
        this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture );

        if ( texture )
        {
            // 一度も setSrcRect() が呼ばれていなければ、テクスチャ全体を転送するようにする
            if ( !mCalledSetSrcRect )
            {
                mSrcRect.set(
                    0, 0,
                    static_cast< int >( texture->getSize().x ),
                    static_cast< int >( texture->getSize().y ) );
            }
        }
        else
        {
            mCalledSetSrcRect = false;
        }

        _updateTexUV();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    Graphics::Texture* Sprite3D::getTexture()
    {
        return this->mNodeRenderParam.Subset[ 0 ].Material.getTexture();
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite3D::setSrcRect( const Geometry::Rect& rect )
    {
        mSrcRect = rect;
        mCalledSetSrcRect = true;
        _updateTexUV(); 
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    void Sprite3D::_updateTexUV()
    {
        Graphics::Texture* tex = getTexture();

        if ( tex )
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
        else
        {
        }
    }

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================