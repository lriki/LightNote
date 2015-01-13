//==============================================================================
// EffekseerEffectInstance 
//==============================================================================

#include "stdafx.h"
#include "../Manager.h"
#include "EffekseerEffectEngine.h"
#include "EffekseerEffectCore.h"
#include "EffekseerEffectInstance.h"

namespace LNote
{
namespace Core
{
namespace Effect
{
//==============================================================================
// ■ EffekseerEffectInstance
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectInstance::EffekseerEffectInstance( EffekseerEffectCore* core )
		: EffectBase		( core->getEffectEngine()->getManager() )
		, mEffectCore		( core )
		, mCurrentHandle	( NULL )
	{
		LN_SAFE_ADDREF( mEffectCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	EffekseerEffectInstance::~EffekseerEffectInstance() 
	{
		stop();
		LN_SAFE_RELEASE( mEffectCore );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::play( bool overlap )
	{
		if ( mCurrentHandle != 0 ) {
			// 今の状態で更新した後、バックへ
			::Effekseer::Matrix43 efkMat;
			LNToEFKMatrix43( mMatrix, &efkMat );
			mEffectCore->getEffectEngine()->getEffekseerManager()->SetMatrix(mCurrentHandle, efkMat);
			//mEffectCore->getEffekseerEffect()->GetManager()->SetMatrix( mCurrentHandle, efkMat );

			mOldHandleArray.push_back( mCurrentHandle );
		}
		mCurrentHandle = mEffectCore->getEffectEngine()->getEffekseerManager()->Play(mEffectCore->getEffekseerEffect(), 0, 0, 0);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::stop()
	{
		::Effekseer::Manager* efkManager = mEffectCore->getEffectEngine()->getEffekseerManager();
		efkManager->StopEffect( mCurrentHandle );
		ln_foreach( ::Effekseer::Handle handle, mOldHandleArray )
		{
			efkManager->StopEffect( handle );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	bool EffekseerEffectInstance::isPlaying()
	{
		if ( mCurrentHandle == 0 ) {
			return false;
		}
		return mEffectCore->getEffectEngine()->getEffekseerManager()->Exists(mCurrentHandle);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::updateContext()
	{
		EffectBase::updateContext();

		mDrawHandleArray.clear();

		// 逆順の方が良いかもしれない
		ln_foreach( ::Effekseer::Handle handle, mOldHandleArray ) {
			mDrawHandleArray.push_back( handle );
		}
		mDrawHandleArray.push_back( mCurrentHandle );

		//update();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::update()
	{
		

		::Effekseer::Manager* efkManager = mEffectCore->getEffectEngine()->getEffekseerManager();
		
		::Effekseer::Matrix43 efkMat;
		LNToEFKMatrix43( mMatrix, &efkMat );
		efkManager->SetMatrix( mCurrentHandle, efkMat );

		efkManager->BeginUpdate();
		ln_foreach( ::Effekseer::Handle handle, mDrawHandleArray ) {
			efkManager->UpdateHandle( 
				handle, 
				mContext.mDeltaTime * mEffectCore->getEffectEngine()->getManager()->getInternalFrameRate() );
		}
		efkManager->EndUpdate();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::draw()
	{
		::Effekseer::Manager* efkManager = mEffectCore->getEffectEngine()->getEffekseerManager();
		::EffekseerRenderer::Renderer* r = mEffectCore->getEffectEngine()->getEffekseerRenderer();

		r->BeginRendering();
		ln_foreach( ::Effekseer::Handle handle, mDrawHandleArray ) {
			efkManager->DrawHandle( handle );
		}
		r->EndRendering();
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void EffekseerEffectInstance::LNToEFKMatrix43( const LMatrix& mat, ::Effekseer::Matrix43* efkMat )
	{
		efkMat->Value[0][0]  = mat.m00;
		efkMat->Value[0][1]  = mat.m01;
		efkMat->Value[0][2]  = mat.m02;
		
		efkMat->Value[1][0]  = mat.m10;
		efkMat->Value[1][1]  = mat.m11;
		efkMat->Value[1][2]  = mat.m12;
		
		efkMat->Value[2][0]  = mat.m20;
		efkMat->Value[2][1]  = mat.m21;
		efkMat->Value[2][2]  = mat.m22;
		
		efkMat->Value[3][0]  = mat.m30;
		efkMat->Value[3][1]  = mat.m31;
		efkMat->Value[3][2]  = mat.m32;
	}

} // namespace Effect
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================