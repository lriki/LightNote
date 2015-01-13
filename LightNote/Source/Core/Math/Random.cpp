﻿//==============================================================================
// Random 
//==============================================================================

#include "stdafx.h"
#include <time.h>
#include "Math.h"
#include "Random.h"

namespace LNote
{
namespace Core
{
namespace Math
{

//==============================================================================
// ■ Random
//==============================================================================

	LN_TYPE_INFO_ACCESS_IMPL(Random);

#define DEFAULT_X 123456789
#define DEFAULT_Y 362436069
#define DEFAULT_Z 521288629
#define DEFAULT_W 88675123

    static int gDefaultSeed = 0;

	//----------------------------------------------------------------------
	// ● デフォルトの乱数シードの設定
	//----------------------------------------------------------------------
    void Random::setDefaultSeed( int seed_ )
    {
        gDefaultSeed = seed_;
    }

	//----------------------------------------------------------------------
	// ● デフォルトの乱数シードの取得
	//----------------------------------------------------------------------
    int Random::getDefaultSeed()
    {
        return gDefaultSeed;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    int Random::getIntHash( int seed_ )
    {
        int w = ( seed_ <= 0 ) ? DEFAULT_W : seed_;
        unsigned t = DEFAULT_X ^ ( DEFAULT_X << 11 );
		w = ( w ^ ( w >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
		return static_cast< int >( w );
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float Random::getFloatHash( int seed_ )
    {
        int r = getIntHash( seed_ );
		r = ( r & 0x007fffff ) | 0x3f800000;
		float f = *reinterpret_cast< float* >( &r );
		return f - 1.f;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float Random::getFloatHash( int seed_, float a_, float b_ )
    {
        float r = getFloatHash( seed_ );
		r *= ( b_ - a_ );
		r += a_;
		return r;
    }

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
    float Random::getFloatHashEx( int seed_, ExRandType rand_type_, float v0_, float v1_ )
    {
        if ( rand_type_ == ExRandType_Range )
        {
            return getFloatHash( seed_, v0_ - v1_, v0_ + v1_ );
        }
        else if ( rand_type_ == ExRandType_MinMax )
        {
            return getFloatHash( seed_, v0_, v1_ );
        }
        else
        {
            return v0_;
        }
    }

	//----------------------------------------------------------------------
	// ● デフォルトコンストラクタ
	//----------------------------------------------------------------------
	Random::Random()
        : x( DEFAULT_X )
        , y( DEFAULT_Y )
        , z( DEFAULT_Z )
        , w( (int)::time(NULL)/*gDefaultSeed*/ )
    {
    }

	//----------------------------------------------------------------------
	// ● コンストラクタ (乱数シード値を指定)
	//----------------------------------------------------------------------
	Random::Random( int seed_ )
        : x( DEFAULT_X )
        , y( DEFAULT_Y )
        , z( DEFAULT_Z )
        , w( gDefaultSeed )
    {
        w = ( seed_ <= 0 ) ? 88675123 : seed_;
    }

	//----------------------------------------------------------------------
	// ● 乱数シードの設定
	//----------------------------------------------------------------------
    void Random::setSeed( int seed_ )
	{
		x = DEFAULT_X;
		y = DEFAULT_Y;
		z = DEFAULT_Z;
		w = ( seed_ <= 0 ) ? 88675123 : seed_;
	}

	//----------------------------------------------------------------------
	// ● 整数値乱数の取得
	//----------------------------------------------------------------------
    int Random::getInt()
	{
		unsigned t = x ^ ( x << 11 );
		x = y;
		y = z;
		z = w;
		w = ( w ^ ( w >> 19 ) ) ^ ( t ^ ( t >> 8 ) );
		return static_cast< int >( w );
	}

	//----------------------------------------------------------------------
	// ● 整数値乱数の取得 ( 0 <= x < a_ )
	//----------------------------------------------------------------------
    int Random::getInt( int a_ )
	{
		if ( a_ == 0 ) { return 0; }
		int r = getInt() & 0x7fffffff;
		r %= a_;
		return r;
	}

	//----------------------------------------------------------------------
	// ● 整数値乱数の取得 ( _a <= x < b_ )
	//----------------------------------------------------------------------
    int Random::getInt( int a_, int b_ )
	{
		if ( b_ - a_ == 0 ) { return 0; }
		int r = getInt() & 0x7fffffff;
		r %= b_ - a_;
		r += a_;
		return r;
	}

	//----------------------------------------------------------------------
	// ● 実数値乱数の取得
	//----------------------------------------------------------------------
    float Random::getFloat()
	{
		int r = getInt();
		r = ( r & 0x007fffff ) | 0x3f800000;
		float f = *reinterpret_cast< float* >( &r );
		return f - 1.f;
	}

	//----------------------------------------------------------------------
	// ● 実数値乱数の取得 ( 0 <= x < a_ )
	//----------------------------------------------------------------------
    float Random::getFloat( float a_ )
	{
		float r = getFloat();
		r *= a_;
		return r;
	}

	//----------------------------------------------------------------------
	// ● 実数値乱数の取得 ( _a <= x < b_ )
	//----------------------------------------------------------------------
    float Random::getFloat( float a_, float b_ )
	{
		float r = getFloat();
		r *= ( b_ - a_ );
		r += a_;
		return r;
	}

	//----------------------------------------------------------------------
	// ● 実数値乱数の取得
	//----------------------------------------------------------------------
    float Random::getFloatEx( float v0_, float v1_, ExRandType rand_type_, float rate_, ExRandRateType rate_type_ ) 
    {
        if ( rand_type_ == ExRandType_None )
        {
            return v0_;
        }
        else
        {
            float min, max;

            if ( rand_type_ == ExRandType_Range )
            {
                min = v0_ - v1_;
                max = v0_ + v1_;
            }
            else // if ( rand_type_ == ExRandType_MinMax )
            {
                min = v0_;
                max = v1_;
            }

            switch ( rate_type_ )
            {

                default:
                {
                    return getFloat( min, max );
                }
                // rate = 1.0 のときに最小値
			    case ExRandRateType_Min:
                {
				    return ( max - min ) * ( 1.0f - rate_ ) + min;
                }
			    // rate = 1.0 のときに最大値
			    case ExRandRateType_Max:
                {
				    return ( max - min ) * rate_ + min;
                }
                // rate = 1.0 のときに中央値
                case ExRandRateType_Median:
                {
                    float median = ( max + min ) * 0.5f;

                    if ( getInt( 2 ) & 1 )
                    {
                        return LMath::lerp( max, median, rate_ ); // 0.0 のときに max
                    }
                    else
                    {
                        return LMath::lerp( min, median, rate_ ); // 0.0 のときに min
                    }
                }
                // rate = 1.0 のときに min または max
                case ExRandRateType_Extreme:
                {
                    float median = ( max + min ) * 0.5f;

                    if ( getInt( 2 ) & 1 )
                    {
                        return LMath::lerp( median, max, rate_ ); // 1.0 のときに max
                    }
                    else
                    {
                        return LMath::lerp( median, min, rate_ ); // 1.0 のときに max
                    }
                }
            }
        }
        return v0_;
    }

} // namespace Math
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================