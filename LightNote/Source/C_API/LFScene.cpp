//==============================================================================
// LFScene 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../Core/Graphics/CompilationException.h"
#include "LFInternal.h"
#include "LFScene.h"



//----------------------------------------------------------------------
///**
//  @brief      描画結果が書き込まれる深度バッファの取得
//	@param[in]	viewPane	: ビューペインハンドル
//	@return		テクスチャハンドル
//*/
//----------------------------------------------------------------------
LNOTEAPI LNHandle LSceneGraph_(LNHandle viewPane);

//==============================================================================
// LNSceneNode
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::SceneNode, LNSceneNode);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetDefault2DRootNode(LNHandle* sceneNode)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);
		*sceneNode = FuncLibManager::Default2DRootNode;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetDefault3DRootNode(LNHandle* sceneNode)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);
		*sceneNode = FuncLibManager::Default3DRootNode;
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetName( LNHandle sceneNode , const LNChar** outName)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		*outName = TO_SCENENODE(sceneNode)->getName();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetName(LNHandle sceneNode, const LNChar* name)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LN_FUNC_TRY_BEGIN;
		TO_SCENENODE(sceneNode)->setName(name);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetPosition(LNHandle sceneNode, const LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setPosition(*((LVector3*)vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetPositionXYZ(LNHandle sceneNode, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setPosition(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetPosition(LNHandle sceneNode, LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_LNVector(
			TO_SCENENODE(sceneNode)->getPosition(),
			vec);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetPositionXYZ(LNHandle sceneNode, float* x, float* y, float* z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ(
			TO_SCENENODE( sceneNode )->getPosition(),
			x, y, z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetAngle(LNHandle sceneNode, const LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setAngle(*((LVector3*)vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetAngleXYZ(LNHandle sceneNode, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setAngle(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetAngleXYZ(LNHandle sceneNode, float* x, float* y, float* z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ(
			TO_SCENENODE( sceneNode )->getAngle(),
			x, y, z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetAngle(LNHandle sceneNode, LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_LNVector(
			TO_SCENENODE( sceneNode )->getAngle(),
			vec);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetScale(LNHandle sceneNode, const LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setScale(*((LVector3*)vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetScaleXYZ(LNHandle sceneNode, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setScale(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetScale(LNHandle sceneNode, LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_LNVector(
			TO_SCENENODE( sceneNode )->getScale(),
			vec);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetScaleXYZ(LNHandle sceneNode, float* x, float* y, float* z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ(
			TO_SCENENODE(sceneNode)->getScale(),
			x, y, z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetCenter(LNHandle sceneNode, const LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setCenter(*((LVector3*)vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetCenterXYZ(LNHandle sceneNode, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setCenter(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetCenter(LNHandle sceneNode, LNVector3* vec)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_LNVector(
			TO_SCENENODE( sceneNode )->getCenter(),
			vec);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetCenterXYZ(LNHandle sceneNode, float* x, float* y, float* z)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_VECTOR3_TO_XYZ(
			TO_SCENENODE(sceneNode)->getCenter(),
			x, y, z);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetMatrix(LNHandle sceneNode, const LNMatrix* mat)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setMatrix(*((LMatrix*)mat));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetMatrix(LNHandle sceneNode, LNMatrix* mat)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LNFUNC_SUBSTITUTE_Matrix_To_LNMatrix(
			TO_SCENENODE( sceneNode )->getMatrix(),
			mat);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetRotateOrder(LNHandle sceneNode, LNRotationOrder rotOrder)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setRotateOrder((Lumino::RotationOrder)rotOrder);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetRotateOrder(LNHandle sceneNode, LNRotationOrder* outOrder)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		*outOrder = (LNRotationOrder)TO_SCENENODE(sceneNode)->getRotateOrder();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetPlacementType(LNHandle sceneNode, LNPlacementType type)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setBillboardType((LNote::LNBillboardType)type);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_GetPlacementType(LNHandle sceneNode, LNPlacementType* outType)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		*outType = (LNPlacementType)TO_SCENENODE(sceneNode)->getBillboardType();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_AddChild(LNHandle sceneNode, LNHandle childSceneNode)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LN_FUNC_TRY_BEGIN;
		TO_SCENENODE(sceneNode)->addChild(TO_SCENENODE(childSceneNode));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_RemoveChild(LNHandle sceneNode, LNHandle childSceneNode)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LN_FUNC_TRY_BEGIN;
		TO_SCENENODE(sceneNode)->removeChild(TO_SCENENODE(childSceneNode));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_Update(LNHandle sceneNode, float elapsedTime)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		LN_FUNC_TRY_BEGIN;
		TO_SCENENODE(sceneNode)->update(elapsedTime);
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_SetEnableAutoUpdate(LNHandle sceneNode, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		TO_SCENENODE(sceneNode)->setEnableAutoUpdate(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSceneNode_IsEnableAutoUpdate(LNHandle sceneNode, LNBool* outEnabled)
	{
		LN_CHECK_ARG_HANDLE(sceneNode);

		*outEnabled = LNOTE_BOOL_TO_LNBOOL(TO_SCENENODE(sceneNode)->isEnableAutoUpdate());
		return ::LN_OK;
	}

//==============================================================================
// LNVisualNode
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::VisualNode, LNVisualNode);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetVisible(LNHandle visualNode, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setVisible(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   LNBool LNVisualNodeNode_IsVisible( LNHandle visualNode )
	//{
	//	return LNOTE_BOOL_TO_LNBOOL( TO_VISUAL_NODE( visualNode )->isVisible() );
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetOpacity(LNHandle visualNode, float opacity)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setOpacity(opacity);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   float LNVisualNodeNode_GetOpacity( LNHandle visualNode )
	//{
	//	return TO_VISUAL_NODE( visualNode )->getOpacity();
	//}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetColorScale(LNHandle visualNode, int materialIndex, const LNColorF* color)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setColorScale(TO_CORE_COLOR(color), materialIndex);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetBlendColor(LNHandle visualNode, int materialIndex, const LNColorF* color)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setBlendColor(TO_CORE_COLOR(color), materialIndex);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetTone(LNHandle visualNode, int materialIndex, const LNTone* tone)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setTone(TO_CORE_TONE(tone), materialIndex);
		return ::LN_OK;
	}
 
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetUVTransform(LNHandle visualNode, int materialIndex, const LNMatrix* matrix)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setUVTransform(TO_CORE_MAT_REF(matrix), materialIndex);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetBlendMode(LNHandle visualNode, LNBlendMode mode)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setBlendMode((LNote::Core::Graphics::BlendMode)mode);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetCullingMode(LNHandle visualNode, LNCullingMode mode)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setCullingMode((LNote::Core::Graphics::CullingMode)mode);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetEnableDepthTest(LNHandle visualNode, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setEnableDepthTest(enabled != LN_FALSE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetEnableDepthWrite(LNHandle visualNode, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setEnableDepthWrite(enabled != LN_FALSE);
		return ::LN_OK;
	}



	

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVisualNode_SetShader(LNHandle visualNode, LNHandle shader)
	{
		LN_CHECK_ARG_HANDLE(visualNode);

		TO_VISUAL_NODE(visualNode)->setShader(TO_SHADER(shader));
		return ::LN_OK;
	}

	
	//----------------------------------------------------------------------
	///**
	//  @brief      シェーダの取得
	//	@param[in]	visualNode	: シーングラフノードハンドル
	//	@return		シェーダハンドル
	//*/
	//----------------------------------------------------------------------
   // LNOTEAPI LNHandle LSVisualNode_GetShader( LNHandle visualNode );
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   LNHandle LSVisualNode_GetShader( LNHandle visualNode )
	//{
	//}

//==============================================================================
// LNSprite
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::Sprite, LNSprite);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_Create(LNHandle* sprite, LNHandle texture)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::Sprite> obj(
			LN_NEW Core::Scene::Sprite( FuncLibManager::SceneManager->getDefaultSceneGraph() ) );
		obj->create();
		obj->setTexture(TO_REFOBJ(Core::Graphics::Texture, texture));
		obj.addRef();
		*sprite = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	///**
	//  @brief      スプライトオブジェクトの作成  (3D空間へ配置する)
	//	@param[out]	sprite		: 作成されたスプライトハンドルを格納する変数
	//	@param[in]	texture		: スプライトが表示するテクスチャのハンドル
	//	@param[in]	frontAxis	:
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNSprite_Create3D(LNHandle* sprite, LNHandle texture, LNFrontAxis frontAxis LNOTE_DEFAULT_ARG(LN_FRONTAXIS_RZ));


	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_Create3D(LNHandle* sprite, LNHandle texture, LNFrontAxis frontAxis)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::Sprite> obj(
			LN_NEW Core::Scene::Sprite( FuncLibManager::SceneManager->getDefaultSceneGraph() ) );
		obj->create3D( (LNote::LNAxisAlignedDir)frontAxis );
		obj->setTexture(TO_REFOBJ(Core::Graphics::Texture, texture));
		obj.addRef();
		*sprite = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetTexture(LNHandle sprite, LNHandle texture)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		TO_SPRITE(sprite)->setTexture(TO_REFOBJ(Core::Graphics::Texture, texture));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetSize(LNHandle sprite, float width, float height)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		TO_SPRITE(sprite)->setSize(LVector2(width, height));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
 //   LNHandle LNSprite_GetTexture( LNHandle sprite )
	//{
	//	return TO_HANDLE( TO_SPRITE( sprite )->getTexture() );
	//}
	
	//----------------------------------------------------------------------
	///**
	//  @brief      設定されているテクスチャの取得
	//	@param[in]	sprite	: スプライトハンドル
	//	@return		テクスチャハンドル
	//*/
	//----------------------------------------------------------------------
	//LNOTEAPI LNHandle LNSprite_GetTexture( LNHandle sprite );
	/*DeclOpt
	$opt.cs.property = ["Texture", "get"]
	$opt.cs.host_field_type = "Texture"
	$opt.cs.host_field = "_texture"
	DeclOpt*/

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetSrcRect(LNHandle sprite, const LNRect* rect)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		TO_SPRITE(sprite)->setSrcRect(TO_CORE_RECTF(rect));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetSrcRectXYWH(LNHandle sprite, int x, int y, int w, int h)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		LRectF rc(static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h));
		TO_SPRITE(sprite)->setSrcRect(rc);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_GetSrcRect(LNHandle sprite, LNRect* rect)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		const LRectF& rc = TO_SPRITE( sprite )->getSrcRect();
		rect->X = static_cast<int>(rc.x);
		rect->Y = static_cast<int>(rc.y);
		rect->Width = static_cast<int>(rc.h);
		rect->Height = static_cast<int>(rc.w);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_GetSrcRectXYWH(LNHandle sprite, int* x, int* y, int* w, int* h)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		const LRectF& rc = TO_SPRITE(sprite)->getSrcRect();
		*x = static_cast<int>(rc.x);
		*y = static_cast<int>(rc.y);
		*w = static_cast<int>(rc.w);
		*h = static_cast<int>(rc.h);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetFlipH(LNHandle sprite, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		uint32_t f = TO_SPRITE( sprite )->getFlipFlag();
		if ( enabled ) {
			f |= LNote::LN_FLIP_H;
		}
		else {
			f &= ~LNote::LN_FLIP_H;
		}
		TO_SPRITE(sprite)->setFlipFlag(f);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_IsFlipH(LNHandle sprite, LNBool* outEnabled)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		*outEnabled = LNOTE_BOOL_TO_LNBOOL((TO_SPRITE(sprite)->getFlipFlag()) & LNote::LN_FLIP_H);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_SetFlipV(LNHandle sprite, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		uint32_t f = TO_SPRITE(sprite)->getFlipFlag();
		if (enabled) {
			f |= LNote::LN_FLIP_V;
		}
		else {
			f &= ~LNote::LN_FLIP_V;
		}
		TO_SPRITE( sprite )->setFlipFlag( f );
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSprite_IsFlipV(LNHandle sprite, LNBool* outEnabled)
	{
		LN_CHECK_ARG_HANDLE(sprite);

		*outEnabled = LNOTE_BOOL_TO_LNBOOL((TO_SPRITE(sprite)->getFlipFlag()) & LNote::LN_FLIP_V);
		return ::LN_OK;
	}

//==============================================================================
// LNSpritePath
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::SpritePath, LNSpritePath);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSpritePath_Create(LNHandle* spritePath, int maxSpriteCount)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::SpritePath> obj(
			LN_NEW Core::Scene::SpritePath( FuncLibManager::SceneManager->getDefaultSceneGraph() ) );
		obj->create(maxSpriteCount, LN_DRAWCOORD_2D);
		obj.addRef();
		*spritePath = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSpritePath_SetPathBlendMode(LNHandle spritePath, LNBlendMode blendMode)
	{
		LN_CHECK_ARG_HANDLE(spritePath);

		TO_SPRITE_PATH( spritePath )->setPathBlendMode( (LNote::Core::Graphics::BlendMode)blendMode );
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSpritePath_SetPathTransform(LNHandle spritePath, const LNMatrix* matrix)
	{
		LN_CHECK_ARG_HANDLE(spritePath);

		TO_SPRITE_PATH(spritePath)->setPathTransform(*((LMatrix*)matrix));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNSpritePath_DrawSprite(LNHandle spritePath, const LNVector3* position, const LNVector3* center, const LNVector2* size, LNHandle texture, const LNRect* srcRect, const LNColorF* vertexColors, LNFrontAxis frontAxis)
	{
		LN_CHECK_ARG_HANDLE(spritePath);

		TO_SPRITE_PATH(spritePath)->drawSprite(
			*((LVector3*)position),
			*((LVector3*)center),
			TO_CORE_VEC2_REF(size),
			TO_REFOBJ(Core::Graphics::Texture, texture),
			TO_CORE_RECTF(srcRect),
			(LColorF*)vertexColors,
			(LNote::LNAxisAlignedDir)frontAxis);
		return ::LN_OK;
	}

//==============================================================================
// LNModel
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::Model, LNModel);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNModel_Create(LNHandle* model, const LNChar* filePath, LNMeshCreateFlags meshCreateFlags)
	{
		return LN_ERR_NOT_IMPLEMENTED;
		//LN_FUNC_TRY_BEGIN;
		//LRefPtr<Core::Scene::Mesh> obj(
		//	LN_NEW Core::Scene::Mesh( FuncLibManager::SceneManager->getDefaultSceneGraph() ) );
		//obj->create( filePath, meshCreateFlags );
		//obj.addRef();
		//*model = TO_HANDLE(FuncLibManager::pushObject(obj));
		//LN_FUNC_TRY_END_RETURN;
	}

//==============================================================================
// LNEffect
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::Effect, LNEffect);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNEffect_Create(LNHandle* effect, const LNChar* filePath)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::Effect> obj(
			LN_NEW Core::Scene::Effect( FuncLibManager::SceneManager->getDefaultSceneGraph() ) );
		obj->create( filePath );
		obj.addRef();
		*effect = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNEffect_Play(LNHandle effect, LNBool overlap)
	{
		LN_CHECK_ARG_HANDLE(effect);

		LN_FUNC_TRY_BEGIN;
		TO_EFFECT(effect)->play((overlap == LN_TRUE));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNEffect_Stop(LNHandle effect)
	{
		LN_CHECK_ARG_HANDLE(effect);

		LN_FUNC_TRY_BEGIN;
		TO_EFFECT(effect)->stop();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNEffect_IsPlaying(LNHandle effect, LNBool* playing)
	{
		LN_CHECK_ARG_HANDLE(effect);

		*playing = LNOTE_BOOL_TO_LNBOOL(TO_EFFECT(effect)->isPlaying());
		return ::LN_OK;
	}


//==============================================================================
// LNTilemap
//==============================================================================

LNOTE_TYPE_INFO_IMPL(Scene::Tilemap, LNTilemap);

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNTilemap_Create(LNHandle* tilemap)
{
	LN_FUNC_TRY_BEGIN;
	LRefPtr<Scene::Tilemap> obj(LN_NEW Scene::Tilemap(FuncLibManager::SceneManager->getDefaultSceneGraph()));
	obj->create();
	obj.addRef();
	*tilemap = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
	LN_FUNC_TRY_END_RETURN;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNTilemap_SetTilesetTexture(LNHandle tilemap, LNHandle texture)
{
	LN_CHECK_ARG_HANDLE(tilemap);
	TO_REFOBJ(Scene::Tilemap, tilemap)->setTilesetTexture(TO_REFOBJ(Graphics::Texture, texture));
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNTilemap_SetTilesetHorizontalTileCount(LNHandle tilemap, int count)
{
	LN_CHECK_ARG_HANDLE(tilemap);
	TO_REFOBJ(Scene::Tilemap, tilemap)->setTilesetHorizontalTileCount((size_t)count);
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNTilemap_SetTilePixelSize(LNHandle tilemap, int size)
{
	LN_CHECK_ARG_HANDLE(tilemap);
	TO_REFOBJ(Scene::Tilemap, tilemap)->setTileSize(size, size);
	return LN_OK;
}

//----------------------------------------------------------------------
//
//----------------------------------------------------------------------
LNOTEAPI LNResult LNTilemap_SetMapData(LNHandle tilemap, LNHandle intTable)
{
	LN_CHECK_ARG_HANDLE(tilemap);
	TO_REFOBJ(Scene::Tilemap, tilemap)->setMapData(TO_REFOBJ(Base::IntTable, intTable));
	return LN_OK;
}

//==============================================================================
// LNShader
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::SceneShader, LNShader);

	static lnString gLNShader_LastError;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNShader_GetLastErrors(const LNChar** errors)
	{
		*errors = gLNShader_LastError.c_str();
		return LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNShader_CreateFromFile(LNHandle* shader, const LNChar* filePath)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::SceneShader> obj(
			FuncLibManager::SceneManager->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(filePath, &gLNShader_LastError));
		obj.addRef();
		*shader = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

//==============================================================================
// LNViewPane
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::Pane, LNViewPane);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNViewPane_GetDefaultViewPane(LNHandle* viewPane)
	{
		*viewPane = FuncLibManager::DefaultViewPane;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNViewPane_SetBackgroundColor(LNHandle viewPane, const LNColorF* color)
	{
		LN_CHECK_ARG_HANDLE(viewPane);

		TO_VIEW_PANE(viewPane)->setBackgroundColor(TO_CORE_COLOR(color));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_GetLayers(LNHandle viewPane, LNHandle* objectList)
	{
		LN_CHECK_ARG_HANDLE(viewPane);

		*objectList = FuncLibManager::getInternalObjectHandle(TO_VIEW_PANE(viewPane)->getLayerList());
		return ::LN_OK;
	}

#if 0
	//----------------------------------------------------------------------
	///**
	//  @brief      ViewPane にレイヤーをアタッチする
	//	@param[in]	viewPane	: ビューペインハンドル
	//	@param[in]	layer		: アタッチするレイヤーハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_AttachLayer(LNHandle viewPane, LNHandle layer);

	//----------------------------------------------------------------------
	///**
	//  @brief      ViewPane からレイヤーをデタッチする
	//	@param[in]	viewPane	: ビューペインハンドル
	//	@param[in]	layer		: デタッチするレイヤーハンドル
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewPane_DetachLayer(LNHandle viewPane, LNHandle layer);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNViewPane_AttachLayer(LNHandle viewPane, LNHandle layer)
	{
		LN_CHECK_ARG_HANDLE(viewPane);

		LN_FUNC_TRY_BEGIN;
		TO_VIEW_PANE(viewPane)->addLayer(TO_REFOBJ(Scene::LayerBase, layer));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNViewPane_DetachLayer(LNHandle viewPane, LNHandle layer)
	{
		LN_CHECK_ARG_HANDLE(viewPane);

		LN_FUNC_TRY_BEGIN;
		TO_VIEW_PANE(viewPane)->removeLayer(TO_REFOBJ(Scene::LayerBase, layer));
		LN_FUNC_TRY_END_RETURN;
	}
#endif
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//void LNViewPane_AttachViewFilter(LNHandle viewPane, LNHandle viewFilter, int priority)
	//{
	//	TO_VIEW_PANE(viewPane)->attachImageFilter(TO_REFOBJ(Scene::ImageFilter, viewFilter), priority);
	//}

	////----------------------------------------------------------------------
	////
	////----------------------------------------------------------------------
	//void LNViewPane_DetachViewFilter(LNHandle viewPane, LNHandle viewFilter)
	//{		
	//	TO_VIEW_PANE(viewPane)->detachImageFilter(TO_REFOBJ(Scene::ImageFilter, viewFilter));
	//}

	//----------------------------------------------------------------------
	///**
	//  @brief      描画結果が書き込まれる深度バッファの取得
	//	@param[in]	viewPane	: ビューペインハンドル
	//	@return		テクスチャハンドル
	//*/
	//----------------------------------------------------------------------
	//LNOTEAPI LNHandle LNViewPane_GetDepthBuffer( LNHandle viewPane );
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//LNHandle LNViewPane_GetDepthBuffer( LNHandle viewPane )
	//{
	//	return NULL;
	//	//return TO_VIEW_PANE( viewPane )->setRenderTarget;
	//}



//==============================================================================
// LNLayer
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::LayerBase, LNLayer);

	//----------------------------------------------------------------------
	///**
	//  @brief      レイヤーの優先度の設定
	//	@param[in]	layer		: レイヤーハンドル
	//	@param[in]	priority	: 優先度 (大きいほど手前(後から)に描画されます)
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_SetPriority(LNHandle layer, int priority);

	//----------------------------------------------------------------------
	///**
	//  @brief      レイヤーの優先度の取得
	//	@param[in]	layer		: レイヤーハンドル
	//	@param[in]	outPriority	: 優先度を格納する変数
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_GetPriority(LNHandle layer, int* outPriority);
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_SetPriority(LNHandle layer, int priority)
	{
		TO_REFOBJ(Scene::LayerBase, layer)->setPriority(priority);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNLayer_GetPriority(LNHandle layer, int* outPriority)
	{
		*outPriority = TO_REFOBJ(Scene::LayerBase, layer)->getPriority();
		return ::LN_OK;
	}


//==============================================================================
// LNViewFilter
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::ImageFilter, LNViewFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_Update(LNHandle viewFilter, float elapsedTime)
	{
		LN_CHECK_ARG_HANDLE(viewFilter);

		TO_REFOBJ(Scene::ImageFilter, viewFilter)->updateFrame(elapsedTime);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_SetEnableAutoUpdate(LNHandle viewFilter, LNBool enabled)
	{
		LN_CHECK_ARG_HANDLE(viewFilter);

		TO_REFOBJ(Scene::ImageFilter, viewFilter)->setEnableAutoUpdate(enabled == LN_TRUE);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNViewFilter_IsEnableAutoUpdate(LNHandle viewFilter, LNBool* outEnabled)
	{
		LN_CHECK_ARG_HANDLE(viewFilter);

		*outEnabled = LNOTE_BOOL_TO_LNBOOL(TO_REFOBJ(Scene::ImageFilter, viewFilter)->isEnableAutoUpdate());
		return ::LN_OK;
	}

//==============================================================================
// LNToneFilter
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::ToneImageFilter, LNToneFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNToneFilter_Create(LNHandle* toneFilter)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::ToneImageFilter> obj( LN_NEW Core::Scene::ToneImageFilter() );
		obj->create( FuncLibManager::SceneManager );
		obj.addRef();
		*toneFilter = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNToneFilter_SetTone(LNHandle toneFilter, const LNTone* tone, float duration)
	{
		LN_CHECK_ARG_HANDLE(toneFilter);

		TO_REFOBJ(Scene::ToneImageFilter, toneFilter)->setTone(TO_CORE_TONE(tone), duration);
		return ::LN_OK;
	}

//==============================================================================
// LNTransitionFilter
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::TransitionImageFilter, LNTransitionFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTransitionFilter_Create(LNHandle* transitionFilter)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::TransitionImageFilter> obj( LN_NEW Core::Scene::TransitionImageFilter() );
		obj->create( FuncLibManager::SceneManager );
		obj.addRef();
		*transitionFilter = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTransitionFilter_Freeze(LNHandle transitionFilter)
	{
		LN_CHECK_ARG_HANDLE(transitionFilter);

		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Scene::TransitionImageFilter, transitionFilter)->freeze();
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNTransitionFilter_Transition(LNHandle transitionFilter, double duration, LNHandle maskTexture, float vague)
	{
		LN_CHECK_ARG_HANDLE(transitionFilter);

		LN_FUNC_TRY_BEGIN;
		TO_REFOBJ(Scene::TransitionImageFilter, transitionFilter)->transition(
			duration, TO_REFOBJ(Core::Graphics::Texture, maskTexture), vague);
		LN_FUNC_TRY_END_RETURN;
	}

//==============================================================================
// LNBlurFilter
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::BlurImageFilter, LNBlurFilter);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNBlurFilter_Create(LNHandle* blurFilter)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Core::Scene::BlurImageFilter> obj( LN_NEW Core::Scene::BlurImageFilter() );
		obj->create( FuncLibManager::SceneManager );
		obj.addRef();
		*blurFilter = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNBlurFilter_Blur(LNHandle blurFilter, double duration, float power, float scale, const LNVector3* center, LNHandle baseCamera)
	{
		LN_CHECK_ARG_HANDLE(blurFilter);

		LN_FUNC_TRY_BEGIN;
		if (baseCamera != NULL)
		{
			TO_REFOBJ(Scene::BlurImageFilter, blurFilter)->blur(
				duration, power, scale, TO_CORE_VEC3_REF(center), TO_CAMERA(baseCamera));
		}
		else 
		{
			TO_REFOBJ(Scene::BlurImageFilter, blurFilter)->blur(
				duration, power, scale, TO_CORE_VEC3_REF(center), NULL);
		}
		LN_FUNC_TRY_END_RETURN;
	}


//==============================================================================
// LNCamera
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(Scene::Camera, LNCamera);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_GetDefault3DCamera(LNHandle* camera)
	{
		*camera = FuncLibManager::Default3DCamera;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_GetDefault2DCamera(LNHandle* camera)
	{
		*camera = FuncLibManager::Default2DCamera;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetCameraType(LNHandle camera, LNCameraType type)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setCameraType((Graphics::CameraType)type);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetPosition(LNHandle camera, const LNVector3* position)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setPosition(TO_CORE_VEC3_REF(position));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetPositionXYZ(LNHandle camera, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setPosition(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetLookAt(LNHandle camera, const LNVector3* at)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setLookAt(TO_CORE_VEC3_REF(at));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetLookAtXYZ(LNHandle camera, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setLookAt(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetUpDirection(LNHandle camera, const LNVector3* up)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setUpDirection(TO_CORE_VEC3_REF(up));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetUpDirectionXYZ(LNHandle camera, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setUpDirection(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetAngle(LNHandle camera, const LNVector3* angle)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setAngle(TO_CORE_VEC3_REF(angle));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetAngleXYZ(LNHandle camera, float x, float y, float z)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setAngle(LVector3(x, y, z));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetMatrix(LNHandle camera, const LNMatrix* matrix)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setMatrix(TO_CORE_MAT_REF(matrix));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetFovY(LNHandle camera, float fov)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA(camera)->setFovY(fov);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_SetNearFarClip(LNHandle camera, float nearClip, float farClip)
	{
		LN_CHECK_ARG_HANDLE(camera);

		TO_CAMERA( camera )->setNearClip( nearClip );
		TO_CAMERA(camera)->setFarClip(farClip);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_GetViewMatrix(LNHandle camera, LNMatrix* outMatrix)
	{
		*(TO_CORE_MAT_PTR(outMatrix)) = TO_CAMERA(camera)->getViewMatrix();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_GetProjectionMatrix(LNHandle camera, LNMatrix* outMatrix)
	{
		LN_CHECK_ARG_HANDLE(camera);

		*(TO_CORE_MAT_PTR(outMatrix)) = TO_CAMERA(camera)->getProjectionMatrix();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNCamera_GetViewProjectionMatrix(LNHandle camera, LNMatrix* outMatrix)
	{
		LN_CHECK_ARG_HANDLE(camera);

		*(TO_CORE_MAT_PTR(outMatrix)) = TO_CAMERA(camera)->getViewProjectionMatrix();
		return ::LN_OK;
	}

//==============================================================================
// LNShader
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	//LNOTEAPI LNHandle LNShader_Create(const LNChar* filePath)
	//{
	//	LN_FUNC_TRY_BEGIN;
	//	LRefPtr<Core::Scene::SceneShader > obj(
	//		FuncLibManager::SceneManager->getDefaultSceneGraph()->getSceneShaderManager()->createSceneShader(filePath));
	//		//LN_NEW Core::Scene::SceneShader(FuncLibManager::SceneManager->getDefaultSceneGraph()->get));
	//	//obj->create(filePath);
	//	obj.addRef();
	//	return TO_HANDLE(FuncLibManager::pushObject(obj));
	//	LN_FUNC_TRY_END;
	//	return NULL;
	//}
