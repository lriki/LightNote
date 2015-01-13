//==============================================================================
// GraphicsResource 
//------------------------------------------------------------------------------
///**
//  @file       GraphicsResource.h
//  @brief      GraphicsResource
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "GraphicsDeviceBase.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ■ GraphicsResource
//------------------------------------------------------------------------------
///**
//  @brief		デバイスリセット前後の通知
//*/
//==============================================================================
class GraphicsResource
{
public:
	GraphicsResource() 
		: mGraphicsDevice	( NULL )
	{}

	virtual ~GraphicsResource() 
	{
		if ( mGraphicsDevice ) {
			mGraphicsDevice->removeGraphicsResource( this );
			mGraphicsDevice->release();
		}
	}

public:
	void registerResource( GraphicsDeviceBase* device ) 
	{  
		LN_THROW_InvalidOperation(mGraphicsDevice == NULL);
		LN_REFOBJ_SET( mGraphicsDevice, device );
		if ( mGraphicsDevice ) {
			mGraphicsDevice->addGraphicsResource( this );
		}
	}

public:
    virtual void onPauseDevice() = 0;
    virtual void onResumeDevice() = 0;

protected:
	GraphicsDeviceBase*	mGraphicsDevice;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================