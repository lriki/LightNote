//==============================================================================
// MMEBaseClasses 
//==============================================================================

#pragma once

#include "stdafx.h"
#include "SceneShaderManager.h"
#include "SceneShader.h"
#include "OffscreenScene.h"
#include "BaseClasses.h"

namespace LNote
{
namespace Core
{
namespace Scene
{

//==============================================================================
// Å° MMESceneObjectBase
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMESceneObjectBase::MMESceneObjectBase()
	{
		memset( mOffscreenSceneFilterGroup, 0, sizeof(mOffscreenSceneFilterGroup) );
	}

//==============================================================================
// Å° MMEOffscreenSubsetInfo
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMEOffscreenSubsetInfo::MMEOffscreenSubsetInfo()
	{
		memset( mOffscreenSubsets, 0, sizeof(mOffscreenSubsets) );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	MMEOffscreenSubsetInfo::~MMEOffscreenSubsetInfo()
	{
		for ( int i = 0 ; i < MaxOffscreenScenes; ++i )
		{
			_deleteOffscreenSubsetArray( i );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMEOffscreenSubsetInfo::setOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex, SceneShader* shader )
	{
		int sceneIndex = scene->getOffscreenSceneID();
		if ( mOffscreenSubsets[sceneIndex] == NULL )
		{
			mOffscreenSubsets[sceneIndex] = LN_NEW OffscreenSubsetArray();
			mOffscreenSubsets[sceneIndex]->resize( getSubsetCount() );
		}
		LN_REFOBJ_SET(
			(*mOffscreenSubsets[sceneIndex])[subsetIndex].Shader,
			shader );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	SceneShader* MMEOffscreenSubsetInfo::getOffscreenSubsetShader( OffscreenScene* scene, int subsetIndex )
	{
		if ( mOffscreenSubsets[scene->getOffscreenSceneID()] )
		{
			return (*mOffscreenSubsets[scene->getOffscreenSceneID()])[subsetIndex].Shader;
		}
		return NULL;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMEOffscreenSubsetInfo::copyOffscreenSubsetInfo( const MMEOffscreenSubsetInfo& info )
	{
		for ( int i = 0 ; i < MaxOffscreenScenes; ++i )
		{
			if ( info.mOffscreenSubsets[i] == NULL )
			{
				_deleteOffscreenSubsetArray( i );
			}
			else
			{
				if ( this->mOffscreenSubsets[i] == NULL )
				{
					this->mOffscreenSubsets[i] = LN_NEW OffscreenSubsetArray();
					this->mOffscreenSubsets[i]->resize( info.mOffscreenSubsets[i]->size() );
				}

				int count = this->mOffscreenSubsets[i]->size();
				for ( int iSubset = 0; iSubset < count; ++iSubset )
				{
					LN_REFOBJ_SET(
						(*this->mOffscreenSubsets[i])[iSubset].Shader,
						(*info.mOffscreenSubsets[i])[iSubset].Shader );
				}
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void MMEOffscreenSubsetInfo::_deleteOffscreenSubsetArray( int index )
	{
		if ( mOffscreenSubsets[index] )
		{
			ln_foreach( OffscreenSubset& subset, (*(mOffscreenSubsets[index])) )
			{
				LN_SAFE_RELEASE( subset.Shader );
			}
			SAFE_DELETE( mOffscreenSubsets[index] );
		}
	}

} // namespace Scene
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================