//=============================================================================
// Grid 
//-----------------------------------------------------------------------------
///**
//  @file       Grid.h
//  @brief      Grid
//*/
//=============================================================================

#pragma once

#include "../Interface.h"
#include "VertexTypes.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{
class GeometryRenderer;

//=============================================================================
// Grid
//-----------------------------------------------------------------------------
///**
//  @brief		
//*/
//=============================================================================
class Grid
    : public Base::ReferenceObject
{
public:
    Grid();

protected:
    virtual ~Grid();

public:

	/// 初期化
    LNRESULT initialize( IGraphicsDevice* device_ );

	/// 格子数の設定
    LNRESULT setLatticeNum( lnU32 num_ );

	/// 格子数の取得
    lnU32 getLatticeNum() { return mSquareNum; }

	/// 間隔の設定
    LNRESULT setInterval( lnFloat interval_ );

	/// 間隔の取得
    lnFloat getInterval() { return mInterval; }
    
	/// 各平面の表示範囲の設定
    LNRESULT setPlaceRange( LNGridPlaceRange x_, LNGridPlaceRange y_, LNGridPlaceRange z_ );

	/// 各平面の表示範囲の取得
    void getPlaceRange( LNGridPlaceRange* x_, LNGridPlaceRange* y_, LNGridPlaceRange* z_ );

	/// 各要素の表示フラグの設定 ( LNGridVisible の組み合わせ )
    virtual void setVisibleFlags( lnU32 flags_ ) { mVisibleFlags = flags_; }

	/// 各要素の表示フラグの取得
    virtual lnU32 getVisibleFlags() { return mVisibleFlags; }
    
	/// 描画 (ラインリストで描画するだけ)
    LNRESULT draw();

private:

	// 頂点バッファを作り直す
    LNRESULT _updateGridVertexBuffer();

private:

	IGraphicsDevice*    mGraphicsDevice;
	IRenderer*          mRenderer;
	IVertexBuffer*      mGridVertexBuffer;

	lnU32				mSquareNum;     ///< ひとつの象限内の四角形の数
	lnFloat             mInterval;

	lnU32				mVisibleFlags;	///< LNGridVisible のフラグの組み合わせ

	LNGridPlaceRange    mXPlace;
	LNGridPlaceRange    mYPlace;
	LNGridPlaceRange    mZPlace;

	lnU32				mXZGridIndex;   ///< 頂点バッファ内の XZ 平面グリッドの開始インデックス
	lnU32				mXYGridIndex;   ///< 頂点バッファ内の XY 平面グリッドの開始インデックス
	lnU32				mYZGridIndex;   ///< 頂点バッファ内の ZY 平面グリッドの開始インデックス
	lnU32				mAxisIndex;     ///< 頂点バッファ内の軸の開始インデックス

	bool                mAxisVisible;

};

} // namespace Graphics
} // namespace Core
} // namespace LNote
