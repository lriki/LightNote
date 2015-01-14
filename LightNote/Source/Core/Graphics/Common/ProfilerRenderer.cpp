//==============================================================================
// ProfilerRenderer 
//==============================================================================

#include "stdafx.h"
#include "../../Resource/LNResource.h"
#include "../Manager.h"
#include "SpriteRenderer.h"
#include "GraphicsUtil.h"
#include "BitmapPainter.h"
#include "ProfilerRenderer.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ProfilerRenderer
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ProfilerRenderer::ProfilerRenderer()
		: mSpriteRenderer(NULL)
		, mSectionSummaryTexture(NULL)
		, mFont_7h(NULL)
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ProfilerRenderer::~ProfilerRenderer()
	{
		LN_SAFE_RELEASE(mSpriteRenderer);
		//LN_SAFE_RELEASE(mSectionSummaryTexture);
		LN_SAFE_RELEASE(mFont_7h);
		//LN_SAFE_RELEASE(mTexture);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ProfilerRenderer::create(Manager* manager, Base::Profiler* prifiler)
	{
		mManager = manager;
		mProfiler = prifiler;

		mSpriteRenderer = LN_NEW SpriteRenderer(mManager);
		mSpriteRenderer->initialize(mManager->getGraphicsDevice(), 256, false);

		mFont_7h = Util::createBitmapFont(manager, Resource::ID_Image_BitmapFont1_h7_6x15/*"D:/Proj/t/BitmapFont_h7_6x15.png"*/);
		mFont_7h->setColor(LColor::White);
		mTextRenderer.creaet(mManager, mFont_7h, 256);
#if 0

		mSectionSummaryTexture = mManager->getGraphicsDevice()->createTexture(320, 320, 1);
		mFont_7h = Util::createBitmapFont(manager, "D:/Proj/t/BitmapFont_h7_6x15.png");//Resource::ID_Image_BitmapFont1_h7_6x15);
		mFont_7h->setColor(LColor::White); //LColorF(1, 1, 1, 1));//
		mSectionSummaryTexture->setFont(mFont_7h);



		mTexture = mManager->getGraphicsDevice()->createTexture(WindowWidth, WindowHeight, 1);
		mTexture->setFont(mFont_7h);

		//mFont_7h->setColor(LColor(200, 200, 200, 255));

		//mTextRenderer.creaet(mManager, mFont_7h, 256);


		//mSectionSummaryTexture->clear(LColorF(1, 1, 0, 0.4));

		//mSectionSummaryTexture->clear(LColorF::TRANSPARENCY);


		int totalSectionCount = 0;
		int threadCount = mProfiler->getRegisterdThreadCount();
		for (int iThread = 0; iThread < threadCount; iThread++)
		{
			const Base::Profiler::ThreadEntry& thread = mProfiler->getRegisterdThread(iThread);

			LRect area(8, iThread * 32 + 8, WindowWidth, WindowHeight/2);
			mSectionSummaryTexture->drawText(thread.Name.c_str(), area);

			for (int iSection = 0; iSection < Base::Profiler::MaxSections; iSection++)
			{
				if (!thread.Sections[iSection].Name.empty())
				{
					int secX = (totalSectionCount % 3) * 80;
					int secY = (totalSectionCount / 3) * 24 + 100;

					// セクション名
					LRect area2(secX + 20, secY, 100, 100);
					mSectionSummaryTexture->drawText(thread.Sections[iSection].Name.c_str(), area2);
					totalSectionCount++;

					BitmapPainter painter(mSectionSummaryTexture);
					painter.drawRectangle(LRect(secX, secY, 10, 10), LColor::Red);
				}
			}
		}
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ProfilerRenderer::updateRenderContents()
	{
		if (!mProfiler->isEnabled()) {
			mSavedProfiler.setEnabled(false);
			return;
		}

		mSavedProfiler = *mProfiler;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ProfilerRenderer::render(const LVector2& viewSize)
	{
		if (!mSavedProfiler.isEnabled()) return;

		//SpriteRenderer* r = mManager->getSprite2DRenderer();
		//r->setTransform(LMatrix::IDENTITY);

		LVector3 location(viewSize.X - 8 - WindowWidth, 8, 0);

		LMatrix mat;
		//mat.translation(location.x/*viewSize.x - 8 - WindowWidth*/, location.y, 0);
		mSpriteRenderer->setTransform(mat);
		mSpriteRenderer->setViewSize(viewSize);

		// ウィンドウ背景
		mSpriteRenderer->drawRequest2D(
			LVector3(location.X, location.Y, 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, WindowWidth, WindowHeight),
			0x80333333);

		// キャプションバー
		mSpriteRenderer->drawRequest2D(
			LVector3(location.X, location.Y, 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, WindowWidth, 20),
			0x33FFFFFF);
		mTextRenderer.drawRequest("Statistics", LRect(location.X, location.Y + 3, WindowWidth, 100), -1, TextAlign_Center);


		location.Y += 24;

		//-----------------------------------------------------
		// Main info
		mTextRenderer.drawRequest("Main information:", LRect(location.X + 8, location.Y, 100, 100), -1, TextAlign_Left);
		location.Y += 16;
		location.X += 16;

		lnChar text[256] = { 0 };
		_stprintf_s(text, 256, _T("Average FPS     : %.1f"), mSavedProfiler.MainFPS);
		mTextRenderer.drawRequest(text, LRect(location.X, location.Y, 100, 100), -1, TextAlign_Left);

		_stprintf_s(text, 256, _T(" / Capacity : %.1f"), mSavedProfiler.MainFPSCapacity);
		mTextRenderer.drawRequest(text, LRect(location.X + 150, location.Y, 100, 100), -1, TextAlign_Left);
		location.Y += 16;

		_stprintf_s(text, 256, _T("Window Size     : %d x %d"), mSavedProfiler.WindowSize.w, mSavedProfiler.WindowSize.h);
		mTextRenderer.drawRequest(text, LRect(location.X, location.Y, 100, 100), -1, TextAlign_Left);
		location.Y += 16;

		_stprintf_s(text, 256, _T("Backbuffer Size : %d x %d"), mSavedProfiler.BackbufferSize.w, mSavedProfiler.BackbufferSize.h);
		mTextRenderer.drawRequest(text, LRect(location.X, location.Y, 100, 100), -1, TextAlign_Left);
		location.Y += 24;
		location.X -= 16;

		
#if 0
		

		int threadCount = mProfiler->getRegisterdThreadCount();
		for (int iThread = 0; iThread < threadCount; iThread++)
		{
			const Base::Profiler::ThreadEntry& thread = mSavedProfiler.getRegisterdThread(iThread);
			float ratio = ((float)thread.Sections[0].ElapsedTime) / 16000.0;	//BaseFPS

			mSpriteRenderer->drawRequest2D(
				LVector3(100, iThread * 32 + 8, 0),
				LVector3(0, 0, 0),
				mManager->getDummyTexture(),
				LRectF(0, 0, ratio * 200, 16),
				0xFFFF3311);
		}
		
#endif
		mTextRenderer.drawRequest("Threads performance:", LRect(location.X + 8, location.Y, 100, 100), -1, TextAlign_Left);
		location.Y += 16;

		// スレッドリストの描画
		int threadCount = mProfiler->getRegisterdThreadCount();
		LRectF listRect(location.X + 16, location.Y, WindowWidth - 32, threadCount * rowHeight);
		drawThreadList(listRect);
		
		//mSpriteRenderer->drawRequest2D(
		//	LVector3(0, 0, 0),
		//	LVector3::ZERO,
		//	mSectionSummaryTexture,
		//	LRectF(0, 0, 320, 320),
		//	0xFFFFFFFF);
		mSpriteRenderer->flash();


		//mTextRenderer.drawRequest("TEST", LRect(0, 0, 100, 100), -1, TextAlign_Left);
		//mTextRenderer.prepareRendering();
		//mTextRenderer.render(viewSize);

		mTextRenderer.prepareRendering();
		mTextRenderer.render(viewSize);

#if 0
		SpriteRenderer* r = mManager->getSprite2DRenderer();
		r->setTransform(LMatrix::IDENTITY);


		mTexture->clear(LColorF::TRANSPARENCY);


		//mSectionSummaryTexture->clear(LColorF::TRANSPARENCY);

		LRect textArea(0, 0, 160, 160);

		int threadCount = mSavedProfiler.getRegisterdThreadCount();
		for (int iThread = 0; iThread < threadCount; iThread++)
		{
			const Base::Profiler::ThreadEntry& thread = mSavedProfiler.getRegisterdThread(iThread);
			for (int iSection = 0; iSection < Base::Profiler::MaxSections; iSection++)
			{
				if (!thread.Sections[iSection].Name.empty())
				{
					//mSectionSummaryTexture->drawText(thread.Sections[iSection].Name.c_str(), textArea);
					textArea.y += 20;
				}
			}

			renderThreadSummary(r, 8, iThread * 32 + 16, thread);

			//float ratio = ((float)thread.Sections[0].ElapsedTime) / 16000.0;	//BaseFPS

			//r->drawRequest2D(
			//	LVector3(0, textArea.y, 0),
			//	LVector3(0, 0, 0),
			//	mManager->getDummyTexture(),
			//	LRectF(0, textArea.y, ratio * 200, 16),
			//	0xFFFF3311);
		}

		//
		//

		
		r->drawRequest2D(
			LVector3(viewSize.x - 320 - 8, 8, 0),
			LVector3(0, 0, 0),
			mManager->getDummyTexture(),
			LRectF(0, 0, 320, 320),
			0x70000000);

		r->drawRequest2D(
			LVector3(viewSize.x - 320 - 8, 8, 0),
			LVector3::ZERO,
			mSectionSummaryTexture,
			LRectF(0, 0, 320, 320),
			0xFFFFFFFF);
		r->flash();
#endif
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ProfilerRenderer::renderThreadSummary(SpriteRenderer* r, float x, float y, const Base::Profiler::ThreadEntry& thread)
	{
		//r->drawRequest2D()

		mTexture->drawText(thread.Name.c_str(), LRect(x, y, 320, 320));


	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ProfilerRenderer::drawThreadList(const LRectF& listRect)
	{
		int threadCount = mProfiler->getRegisterdThreadCount();

		// 名前カラム背景色
		mSpriteRenderer->drawRequest2D(
			LVector3(listRect.x, listRect.y, 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, ThreadNameColumnWidth, listRect.h),
			0x33000000);


		// バーのNG領域背景色
		LRectF ngArea(
			listRect.x + (ThreadNameColumnWidth + SafeAreaWidth),
			listRect.y,
			listRect.w - (ThreadNameColumnWidth + SafeAreaWidth),
			threadCount * rowHeight);
		mSpriteRenderer->drawRequest2D(
			LVector3(ngArea.x, ngArea.y, 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, ngArea.w, ngArea.h),
			0x33FF0000);

		// リスト縦罫線
		mSpriteRenderer->drawRequest2D(
			LVector3(listRect.x + ThreadNameColumnWidth - 1, listRect.y, 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, 1, listRect.h),
			RuleLineColor);



		int iThread;
		for (iThread = 0; iThread < threadCount; iThread++)
		{
			// スレッドひとつ分の情報
			const Base::Profiler::ThreadEntry& thread = mSavedProfiler.getRegisterdThread(iThread);

			// row 上部罫線
			mSpriteRenderer->drawRequest2D(
				LVector3(listRect.x, listRect.y + (iThread * rowHeight), 0),
				LVector3::Zero,
				mManager->getDummyTexture(),
				LRectF(0, 0, listRect.w, 1),
				RuleLineColor);

			// スレッド名
			LRect rc(listRect.x + 4, listRect.y + (iThread * rowHeight) + 1, ThreadNameColumnWidth - 10, 100);
			mTextRenderer.drawRequest(thread.Name.c_str(), rc, -1, TextAlign_Left);

			// セクションを積み上げ棒グラフで表示
			lnFloat totalTime = drawSectionGraphBar(thread, listRect.x + ThreadNameColumnWidth, listRect.y + (iThread * rowHeight), listRect);

			// FPS
			lnChar fps[256] = { 0 };
			_stprintf_s(fps, 256, _T("%.1f FPS"), 1.0 / (totalTime / 1000000.0));	// us → s の後、逆数
			rc.y += 16;
			mTextRenderer.drawRequest(fps, rc, -1, TextAlign_Right);
		}

		// list 下部罫線
		mSpriteRenderer->drawRequest2D(
			LVector3(listRect.x, listRect.y + (iThread * rowHeight), 0),
			LVector3::Zero,
			mManager->getDummyTexture(),
			LRectF(0, 0, listRect.w, 1),
			0xFF000000);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	lnFloat ProfilerRenderer::drawSectionGraphBar(const Base::Profiler::ThreadEntry& thread, lnFloat x, lnFloat y, const LRectF& listRect)
	{
		lnFloat totalTime = 0;	// 描画ついでに総時間を集計
		lnFloat nextX = 0;
		for (int iSection = 0; iSection < Base::Profiler::MaxSections; iSection++)
		{
			if (thread.LimitElapsedTime > 0.0)
			{
				const Base::Profiler::Section& section = thread.Sections[iSection];
				float ratio = ((float)section.ElapsedTime) / thread.LimitElapsedTime;	//BaseFPS
				totalTime += (lnFloat)section.ElapsedTime;

				LVector3 pos(x + nextX, y + 5, 0);
				LRectF srcRect(0, 0, ratio * SafeAreaWidth, rowHeight - 10);
				if (listRect.x + listRect.w < pos.X + srcRect.w)
				{
					srcRect.w = listRect.x + listRect.w - pos.X;
				}

				mSpriteRenderer->drawRequest2D(
					pos,
					LVector3::Zero,
					mManager->getDummyTexture(),
					srcRect,
					getColorByIndex(section.ColorIndex).toU32ARGB());

				nextX += ratio * SafeAreaWidth;
				if (listRect.x + listRect.w <= nextX)
					break;
			}
		}

		return totalTime;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LColor ProfilerRenderer::getColorByIndex(int index)
	{
		HSVColor hsv(70 * index, 192, 255, 160);
		return hsv.toRGBAColor();
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
