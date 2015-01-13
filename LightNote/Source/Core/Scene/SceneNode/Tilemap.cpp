//==============================================================================
// Tilemap 
//==============================================================================

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../SceneShader/SceneShaderManager.h"
#include "../SceneShader/SceneShader.h"
#include "../SceneGraph.h"
#include "Tilemap.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Tilemap::DrawingContext
//==============================================================================
class Tilemap::DrawingContext
    : public VisualNodeContext
{
public:
	DrawingContext()
		: mMapData(LN_NEW Base::IntTable())
		, mTilesetTexture(NULL)
		, mDirection(LN_TILEMAPDIR_XYR)
		, mTileWidth(0)
		, mTileHeight(0)
		, mTilesetHorizontalTileCount(0)
		, mVertexNum(0)
		, mVertexBuffer(NULL)
		, mIndexBuffer(NULL)
		, mFaceCount(0)
		, mXTileCount(0)
		, mYTileCount(0)
	{
	}

	virtual ~DrawingContext()
	{
		LN_SAFE_RELEASE(mVertexBuffer);
		LN_SAFE_RELEASE(mIndexBuffer);
		LN_SAFE_RELEASE(mTilesetTexture);
		LN_SAFE_RELEASE(mMapData);
	}

public:

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void updateContext(SceneNodeContext* parentContext)
	{
		Tilemap* owner = static_cast<Tilemap*>(mSceneNode);
		mMapData->copy(owner->mMapData);
		LN_REFOBJ_SET(mTilesetTexture, owner->mTilesetTexture);
		mDirection = owner->mDirection;
		mTileWidth = owner->mTileWidth;
		mTileHeight = owner->mTileHeight;
		mTilesetHorizontalTileCount = owner->mTilesetHorizontalTileCount;

		VisualNodeContext::updateContext(parentContext);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	virtual void render(ContextDrawParam& param)
	{
		// TODO:preRender �I�Ȃ��̂ŁA�`��p�X�̍ŏ��Ɉ�x�����Ă΂��悤�Ȃ̂ōX�V������
		refreshVertexBuffer();
		writeVertexUV();

		VisualNodeContext::render(param);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    virtual void drawSubset( ContextDrawParam& param, int index )
    {
        // TODO:
		param.Renderer->setRenderState(Graphics::LRenderState::BUFFER_COPY_STATE);

        param.Renderer->setVertexBuffer( mVertexBuffer );
        param.Renderer->setIndexBuffer( mIndexBuffer );

		param.Renderer->drawPrimitiveIndexed(Graphics::PrimitiveType_TriangleList, 0, mFaceCount);
    }

private:

	/// ���݂̏�Ԃ����ɒ��_�o�b�t�@�����
	void refreshVertexBuffer()
	{
		if (mXTileCount != mMapData->getXSize() || mYTileCount != mMapData->getYSize())
		{
			LN_SAFE_RELEASE(mVertexBuffer);
			LN_SAFE_RELEASE(mIndexBuffer);

			mXTileCount = mMapData->getXSize();
			mYTileCount = mMapData->getYSize();

			// ���_�̑���
			mVertexNum = mXTileCount * mYTileCount * 4;

			// �O�p�`�̐��͎l�p�`�̐��� 2 �{�ŁA�C���f�b�N�X���͂��� 3 �{
			mFaceCount = mXTileCount * mYTileCount * 2;
			int index_num = mFaceCount * 3;

			// ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@�쐬
			mVertexBuffer = this->mSceneNode->getSceneGraph()->getGraphicsManager()->getGraphicsDevice()->createVertexBuffer(
				Graphics::Vertex::PosUV::elements(), mVertexNum, NULL, true);

			mIndexBuffer = this->mSceneNode->getSceneGraph()->getGraphicsManager()->getGraphicsDevice()->createIndexBuffer(
				index_num, NULL, true);

			//-----------------------------------------------------
			// ���_�o�b�t�@�Ɏl�p�`���l�߂Ă���

			Graphics::Vertex::PosUV* vertices = (Graphics::Vertex::PosUV*)mVertexBuffer->lock();
			lnU32 i, x, y;
			lnFloat dx, dy, dz;
			dz = 0.0f;
			switch (mDirection)
			{
			default:    // LN_TILEMAPDIR_XYR	// XY ���ʂɑ΂��� Z+ �����ɍ쐬���� (2D �̏ꍇ�͂�����w�肷��)
			{
				for (y = 0; y < mYTileCount; ++y)
				{
					for (x = 0; x < mXTileCount; ++x)
					{
						i = ((mXTileCount * y) + x) * 4;     // �������݈ʒu
						dx = mTileWidth * x;
						dy = mTileHeight * y;

						// ���ɁA����@�E��@�����@�E��
						vertices[i + 0].Position.set(dx, dy, dz);
						vertices[i + 1].Position.set(dx + mTileWidth, dy, dz);
						vertices[i + 2].Position.set(dx, dy + mTileHeight, dz);
						vertices[i + 3].Position.set(dx + mTileWidth, dy + mTileHeight, dz);
					}
				}
				break;
			}
			case LN_TILEMAPDIR_XY:	// XY ���ʂɑ΂��āA�}�b�v�̍��������_�ɂȂ�悤�ɍ쐬���� (���ی��ɍ쐬����)
			{
				lnFloat oy = mTileHeight * mYTileCount;
				for (y = 0; y < mYTileCount; ++y)
				{
					for (x = 0; x < mXTileCount; ++x)
					{
						i = ((mXTileCount * y) + x) * 4;     // �������݈ʒu
						dx = mTileWidth * x;
						dy = -mTileHeight * y + oy;

						// ���ɁA����@�E��@�����@�E��
						vertices[i + 0].Position.set(dx, dy, dz);
						vertices[i + 1].Position.set(dx + mTileWidth, dy, dz);
						vertices[i + 2].Position.set(dx, dy - mTileHeight, dz);
						vertices[i + 3].Position.set(dx + mTileWidth, dy - mTileHeight, dz);
					}
				}
				break;
			}
			}
			mVertexBuffer->unlock();


			//-----------------------------------------------------
			// �C���f�b�N�X���l�߂Ă���

			lnU16* indices = (lnU16*)mIndexBuffer->lock();
			lnU16 vi;
			for (y = 0; y < mYTileCount; ++y)
			{
				for (x = 0; x < mXTileCount; ++x)
				{
					i = (lnU16)((mXTileCount * y) + x) * 6;
					vi = (lnU16)((mXTileCount * y) + x) * 4;
					indices[i + 0] = vi;
					indices[i + 1] = vi + 1;
					indices[i + 2] = vi + 2;
					indices[i + 3] = vi + 2;
					indices[i + 4] = vi + 1;
					indices[i + 5] = vi + 3;
				}
			}
			mIndexBuffer->unlock();
		}
	}

	/// 
	void writeVertexUV()
	{
		// �^�C���ЂƂ��� UV �T�C�Y
		float uvWidth = static_cast< lnFloat >(mTileWidth) / mTilesetTexture->getRealSize().x;
		float uvHeight = static_cast< lnFloat >(mTileHeight) / mTilesetTexture->getRealSize().y;
	
		// ���_�o�b�t�@�ɏ�������ł���
		Graphics::Vertex::PosUV* vertices = (Graphics::Vertex::PosUV*)mVertexBuffer->lock();
		lnU32 i, x, y, d;
		for (y = 0; y < mYTileCount; ++y)
		{
			for (x = 0; x < mXTileCount; ++x)
			{
				i = (y * mXTileCount + x) * 4;
				d = mMapData->getValue(x, y, 0);

				int idx = ((mXTileCount * y) + x) * 4;
				lnFloat u = uvWidth * (d % mTilesetHorizontalTileCount);
				lnFloat v = uvHeight * (d / mTilesetHorizontalTileCount);
				vertices[i + 0].TexUV.set(u, v);
				vertices[i + 1].TexUV.set(u + uvWidth, v);
				vertices[i + 2].TexUV.set(u, v + uvHeight);
				vertices[i + 3].TexUV.set(u + uvWidth, v + uvHeight);
			}
		}
		mVertexBuffer->unlock();
	}

private:

	Base::IntTable*				mMapData;
	Graphics::Texture*			mTilesetTexture;
	LNTilemapDirection			mDirection;
	lnFloat						mTileWidth;     ///< �^�C���ЂƂ��̕�
	lnFloat						mTileHeight;    ///< �^�C���ЂƂ��̍���
	size_t						mTilesetHorizontalTileCount;	/// �^�C���Z�b�g�e�N�X�`���̉������̃^�C����

	lnU32						mVertexNum;     ///< ���_�̑��� (���_�o�b�t�@�ɓ����Ă邩�炢��Ȃ�����)
	Graphics::IVertexBuffer*	mVertexBuffer;  ///< ���_�o�b�t�@
	Graphics::IIndexBuffer*		mIndexBuffer;   ///< �C���f�b�N�X�o�b�t�@
	lnU32						mFaceCount;
	lnU32						mXTileCount;	///< VertexBuffer �ɍ쐬���� X �����̃^�C���̐�
	lnU32						mYTileCount;	///< VertexBuffer �ɍ쐬���� Y �����̃^�C���̐�
};


//==============================================================================
// Tilemap
//==============================================================================

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Tilemap::Tilemap( SceneGraph* scene_ )
    : VisualNode         ( scene_ )
    , mTilesetTexture   ( NULL )
    //, mXSize            ( 0 )
    //, mYSize            ( 0 )
    //, mFaceNum          ( 0 )
    , mTileWidth        ( 0.0f )
    , mTileHeight       ( 0.0f )
    //, mUVWidth          ( 0.0f )
    //, mUVHeight         ( 0.0f )
    , mMapData          ( NULL )
    //, mSrcXNum          ( 8 )
{
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
Tilemap::~Tilemap()
{
	LN_SAFE_RELEASE(mMapData);
	LN_SAFE_RELEASE(mTilesetTexture);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Tilemap::create(/* lnU32 w_size_, lnU32 h_size_, LNTilemapDirection dir_ */)
{
	Tilemap::DrawingContext* context = LN_NEW Tilemap::DrawingContext();
	context->initialize( this );
    SceneNode::initialize( 1, LN_DRAWCOORD_2D, context );

    // �^�C���̐�
 //   mXSize = w_size_;
	//mYSize = h_size_;

	mDirection = LN_TILEMAPDIR_XYR/*dir_*/;

    // �\������Ƃ��̃^�C���ЂƂ��̑傫��
	mTileWidth = DefaultTilePixelSize;
	mTileHeight = DefaultTilePixelSize;

	mTilesetHorizontalTileCount = 8;

    // ���݂̏�Ԃ����ɒ��_�o�b�t�@�����
    //_makeVertexBuffer( dir_ );

    LRefPtr<SceneShader> shader( 
		mSceneGraph->getSceneShaderManager()->createSceneShader( 
			Resource::ID_Shader_Tilemap,
			lnSharingKey(_T( "LNResource\\Shader\\Tilemap.lnfx" ) ) ) );

    setShader( shader );
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
//void Tilemap::setPosition( int x_, int y_ )
//{
//    LN_PRINT_NOT_IMPL_FUNCTION
//}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Tilemap::setTilesetTexture( Graphics::Texture* texture_ )
{
    LN_SAFE_ADDREF( texture_ );
    LN_SAFE_RELEASE( mTilesetTexture );
    mTilesetTexture = texture_;

  //  if ( mTilesetTexture )
  //  {
  //      mUVWidth  = static_cast< lnFloat >( mTileWidth  ) / mTilesetTexture->getRealSize().x;
		//mUVHeight = static_cast< lnFloat >( mTileHeight ) / mTilesetTexture->getRealSize().y;
  //  }

    this->mNodeRenderParam.Subset[ 0 ].Material.setTexture( texture_ );
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Tilemap::setTileSize(size_t width, size_t height)
{
	mTileWidth = static_cast<lnFloat>(width);
	mTileHeight = static_cast<lnFloat>(height);
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
void Tilemap::update( float elapsedTime ) 
{
}

} // namespace Scene
} // namespace Core
} // namespace LNote
