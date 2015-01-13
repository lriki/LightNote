//==============================================================================
// ScalarAnimation 
//==============================================================================

#include "stdafx.h"
#include <algorithm>
#include <tinyxml2/tinyxml2.h>
#include <Dependencies/cpp-TimSort/timsort.hpp>
#include "../Math/Math.h"
#include "../Math/Random.h"
#include "ScalarAnimation.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ ScalarAnimationKey
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ScalarAnimationKey::ScalarAnimationKey()
		: FramePos      ( 0.0f )
		, Value         ( 0.0f )
		, Velocity      ( 0.0f )
		, Accel         ( 0.0f )
		, LeftSlope     ( 0.0f )
		, RightSlope    ( 0.0f )
		, InterMode     ( LN_ANIM_INTER_LINEAR )
	{
	}

//==============================================================================
// ■ ScalarAnimationSourceKey
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ScalarAnimationSourceKey::ScalarAnimationSourceKey()
		: FramePos          ( 0 )
		, Value             ( 0 )
		, ValueRand         ( 0 )
		, Velocity          ( 0 )
		, VelocityRand      ( 0 )
		, Accel             ( 0 )
		, AccelRand         ( 0 )
		, LeftSlope         ( 0 )
		, LeftSlopeRand     ( 0 )
		, RightSlope        ( 0 )
		, RightSlopeRand    ( 0 )
		, InterMode         ( LN_ANIM_INTER_CATMULLROM )
	{
	}

//==============================================================================
// ■ ScalarAnimation
//==============================================================================

	const lnChar* ScalarAnimation::XML_ELEMENT_NAME     = _T( "ScalarAnimation" );
	const lnChar* ScalarAnimation::XML_ELEMENT_NAME_KEY = _T( "ScalarAnimationKey" );

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ScalarAnimation::ScalarAnimation()
		: mDefaultValue	( 0 )
		, mValue		( 0 )
	{
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ScalarAnimation::~ScalarAnimation() 
	{
	
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::addKeyFrame( double frame_pos, lnFloat value )
	{
		ScalarAnimationKey key;
		key.FramePos = frame_pos;
		key.Value    = value;
		addKeyFrame( key );
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::addKeyFrame( const ScalarAnimationKey& key )
	{
		// そのまま追加できる
		if ( mKeyFrameList.empty() || mKeyFrameList.back().FramePos <= key.FramePos )
		{
			mKeyFrameList.push_back( key );
		}
		// 追加後のソートが必要
		else
		{
			// 比較用ファンクタ
			struct compare
			{
				bool operator()( const ScalarAnimationKey& l, const ScalarAnimationKey& r )
				{
					return l.FramePos < r.FramePos;
				}
			};

			mKeyFrameList.push_back( key );
			gfx::timsort( mKeyFrameList.begin(), mKeyFrameList.end(), compare() );
		}
	}

	static int _cmpKey( const void* a_, const void* b_ )
	{
		ScalarAnimationKey* l = (ScalarAnimationKey*)a_;
		ScalarAnimationKey* r = (ScalarAnimationKey*)b_;

		if ( (l)->FramePos < (r)->FramePos )
			return -1;
		else if ( (l)->FramePos >= (r)->FramePos && (l)->FramePos < ((r+1))->FramePos )
			return 0;
		else
			return 1;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::setTime( double time )
	{
		AnimationTimelineBase::setTime( time );

		if ( getKeyNum() > 0 )
		{
			// time_ が最初のフレーム位置より前の場合はデフォルト値
			if ( mCurrentFramePos < mKeyFrameList.front().FramePos )
			{
				mValue = mDefaultValue;
			}
			// キーがひとつだけの場合はそのキーの値
			else if ( getKeyNum() == 1 )
			{
				mValue = mKeyFrameList.front().Value;
			}
			// time_ が終端以降の場合は終端の値
			else if ( mCurrentFramePos >= mKeyFrameList.back().FramePos )
			{
				mValue = mKeyFrameList.back().Value;
			}
			// 以上以外の場合は補間する
			else
			{
				const ScalarAnimationKey* key0 = (ScalarAnimationKey*)bsearch( &mCurrentFramePos, &(mKeyFrameList[0]), mKeyFrameList.size(), sizeof( ScalarAnimationKey ), _cmpKey );
				const ScalarAnimationKey* key1 = key0 + 1;

				lnFloat p0 = key0->Value;
				lnFloat p1 = key1->Value;
				lnFloat t0 = static_cast< lnFloat >( key0->FramePos );
				lnFloat t1 = static_cast< lnFloat >( key1->FramePos );
				lnFloat t  = static_cast< lnFloat >( mCurrentFramePos - t0 ) / ( t1 - t0 );

				switch ( key0->InterMode )
				{
					// 補間無し
					case LN_ANIM_INTER_NONE:
					{
						mValue = p0;
						break;
					}
					// 線形
					case LN_ANIM_INTER_LINEAR:
					{
						mValue = p0 + ( p1 - p0 ) * t;
						break;
					}
					// 等加速度
					case LN_ANIM_INTER_ACCEL:
					{
						mValue = LMath::accel( p0, key0->Velocity, key0->Accel, static_cast< lnFloat >( mCurrentFramePos - key0->FramePos )  );
						break;
					}
					// 三次補間
					case LN_ANIM_INTER_CUBIC:
					{
						mValue = LMath::cubic(
							p0, p1, 
							key0->RightSlope,
							key1->LeftSlope,
							t );
						break;
					}
					// Catmull-Rom
					case LN_ANIM_INTER_CATMULLROM:
					{
						// ループ再生で time が終端を超えている場合、
						// この時点でkey の値は ループ開始位置のひとつ前のキーを指している

						const ScalarAnimationKey& begin = mKeyFrameList.front();
						const ScalarAnimationKey& end   = mKeyFrameList.back();
                        
						// この補間には、begin のひとつ前と end のひとつ後の値が必要。
						// それぞれが始点、終点の場合はループするように補間する
						mValue = LMath::catmullRom(
							( ( key0->FramePos == begin.FramePos ) ? end.Value : (key0 - 1)->Value ),
							p0,
							p1,
							( ( key1->FramePos == end.FramePos ) ? begin.Value : (key0 + 2)->Value ),
							t );
						break;
					}
				}
			}
		}
		else
		{
			mValue = mDefaultValue;
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::addSourceKeyFrame( const ScalarAnimationSourceKey& key )
	{
		// そのまま追加できる
		if ( mSourceKeyFrameList.empty() || mSourceKeyFrameList.back().FramePos <= key.FramePos )
		{
			mSourceKeyFrameList.push_back( key );
		}
		// 追加後のソートが必要
		else
		{
			// 比較用ファンクタ
			struct compare
			{
				bool operator()( const ScalarAnimationSourceKey& l, const ScalarAnimationSourceKey& r )
				{
					return l.FramePos < r.FramePos;
				}
			};

			mSourceKeyFrameList.push_back( key );
			gfx::timsort( mSourceKeyFrameList.begin(), mSourceKeyFrameList.end(), compare() );
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::applySourceKeys( int rand_seed )
	{
		if ( mSourceKeyFrameList.empty() ) return;

		if ( mKeyFrameList.size() != mSourceKeyFrameList.size() )
		{
			mKeyFrameList.resize( mSourceKeyFrameList.size(), ScalarAnimationKey() );
		}

		if ( rand_seed != 0 )
		{
			Math::Random random( rand_seed );

			int key_count = mSourceKeyFrameList.size();
			for ( int i = 0; i < key_count; ++i )
			{
				ScalarAnimationKey& k = mKeyFrameList[i];
				ScalarAnimationSourceKey& ek = mSourceKeyFrameList[i];

				k.FramePos    = ek.FramePos;
				k.Value       = random.getFloatRange( ek.Value, ek.ValueRand );
				k.Velocity    = random.getFloatRange( ek.Velocity, ek.VelocityRand );
				k.Accel       = random.getFloatRange( ek.Accel, ek.AccelRand );
				k.LeftSlope   = random.getFloatRange( ek.LeftSlope, ek.LeftSlopeRand );
				k.RightSlope  = random.getFloatRange( ek.RightSlope, ek.RightSlopeRand );
			}
		}
		else
		{
			int key_count = mSourceKeyFrameList.size();
			for ( int i = 0; i < key_count; ++i )
			{
				ScalarAnimationKey& k = mKeyFrameList[i];
				ScalarAnimationSourceKey& ek = mSourceKeyFrameList[i];

				k.FramePos    = ek.FramePos;
				k.Value       = ek.Value;
				k.Velocity    = ek.Velocity;
				k.Accel       = ek.Accel;
				k.LeftSlope   = ek.LeftSlope;
				k.RightSlope  = ek.RightSlope;
			}
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	XML::Element* ScalarAnimation::serializeXMLElement( XML::Document* doc )
	{
		tinyxml2::XMLElement* elm = doc->NewElement( XML_ELEMENT_NAME );

		ln_foreach( ScalarAnimationSourceKey& k, mSourceKeyFrameList )
		{
			tinyxml2::XMLElement* key_elm = doc->NewElement( XML_ELEMENT_NAME_KEY );
			key_elm->SetAttribute( "FramePos",      k.FramePos );
			key_elm->SetAttribute( "Value",         k.Value );
			key_elm->SetAttribute( "ValueRand",     k.ValueRand );
			key_elm->SetAttribute( "Velocity",      k.Velocity );
			key_elm->SetAttribute( "VelocityRand",  k.VelocityRand );
			key_elm->SetAttribute( "Accel",         k.Accel );
			key_elm->SetAttribute( "AccelRand",     k.AccelRand );
			key_elm->SetAttribute( "LeftSlope",     k.LeftSlope );
			key_elm->SetAttribute( "LeftSlopeRand", k.LeftSlopeRand );
			key_elm->SetAttribute( "RightSlope",    k.RightSlope );
			key_elm->SetAttribute( "RightSlopeRand",k.RightSlopeRand );
			key_elm->SetAttribute( "InterMode",     (int)k.InterMode );

			elm->InsertEndChild( key_elm );
		}
		return elm;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ScalarAnimation::deserializeXMLElement( XML::Element* element )
	{
		mSourceKeyFrameList.clear();

		tinyxml2::XMLElement* elm = element->FirstChildElement();
		while ( elm )
		{
			// キーフレーム
			//if ( _tcscmp( elm->Name(), XML_ELEMENT_NAME_KEY ) == 0 )  // 高速化。1種類しかないので…
			{
				ScalarAnimationSourceKey k;
				k.FramePos          = elm->DoubleAttribute( "FramePos" );
				k.Value             = elm->FloatAttribute( "Value" );
				k.ValueRand         = elm->FloatAttribute( "ValueRand" );
				k.Velocity          = elm->FloatAttribute( "Velocity" );
				k.VelocityRand      = elm->FloatAttribute( "VelocityRand" );
				k.Accel             = elm->FloatAttribute( "Accel" );
				k.AccelRand         = elm->FloatAttribute( "AccelRand" );
				k.LeftSlope         = elm->FloatAttribute( "LeftSlope" );
				k.LeftSlopeRand     = elm->FloatAttribute( "LeftSlopeRand" );
				k.RightSlope        = elm->FloatAttribute( "RightSlope" );
				k.RightSlopeRand    = elm->FloatAttribute( "RightSlopeRand" );
				k.InterMode         = (LNAnimInterMode)elm->IntAttribute( "InterMode" );

				addSourceKeyFrame( k );
			}
            
			elm = elm->NextSiblingElement();
		}
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	double ScalarAnimation::getLastFramePos()
	{
		if ( mKeyFrameList.empty() ) return 0;
		return mKeyFrameList.back().FramePos;
	}

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================