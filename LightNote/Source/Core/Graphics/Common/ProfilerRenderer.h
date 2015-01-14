//==============================================================================
// ProfilerRenderer 
//------------------------------------------------------------------------------
///**
//  @file       ProfilerRenderer.h
//  @brief      ProfilerRenderer
//*/
//==============================================================================

#pragma once

#include "Common.h"
#include "../Interface.h"
#include "../../Base/Profiler.h"
#include "../../Math/LMath.h"
#include "TextRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ProfilerRenderer
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ProfilerRenderer
{
public:
	static const int WindowWidth = 320;
	static const int WindowHeight = 192;
	static const int rowHeight = 32;
	static const int ThreadNameColumnWidth = 80;
	static const int SafeAreaWidth = 160;
	static const lnU32 RuleLineColor = 0xAA000000;

public:
	ProfilerRenderer();
	~ProfilerRenderer();

public:
	/// 作成
	void create(Manager* manager, Base::Profiler* prifiler);

	/// 描画情報の更新
	void updateRenderContents();

	/// 描画
	void render(const LVector2& viewSize);

private:

	void renderThreadSummary(SpriteRenderer* r, float x, float y, const Base::Profiler::ThreadEntry& thread);

	void drawThreadList(const LRectF& listRect);

	lnFloat drawSectionGraphBar(const Base::Profiler::ThreadEntry& thread, lnFloat x, lnFloat y, const LRectF& listRect);

	LColor getColorByIndex(int index);


private:
	struct SectionSummary
	{
		lnRefString		Name;
		LColorF			Color;
	};

	struct ThreadEntry
	{
		//lnRefString		
	};

private:
	Manager*			mManager;
	SpriteRenderer*		mSpriteRenderer;

	Base::Profiler*		mProfiler;
	Base::Profiler		mSavedProfiler;	///< 元データからのコピー

	Texture*			mTexture;

	Texture*			mSectionSummaryTexture;
	Font*				mFont_7h;

	//LVector2			mLastViewSize;

	TextRenderer		mTextRenderer;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
