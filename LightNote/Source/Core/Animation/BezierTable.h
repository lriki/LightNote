//==============================================================================
// BezierTable 
//------------------------------------------------------------------------------
///**
//  @file       BezierTable.h
//  @brief      BezierTable
//  @author     Riki
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// Å° BezierTable
//------------------------------------------------------------------------------
///**
//  @brief      VMDópBezier
//*/
//==============================================================================
class BezierTable
{
public:

	void initialize( float pt_x1, float pt_y1, float pt_x2, float pt_y2 );

	/// t = 0.0Å`1.0
	float getInterValue( float t );

private:

	float getYValue( float x, float x1, float y1, float x2, float y2 );

private:

	static const int	YVAL_DIV_NUM = 16;
	float	            mYValue[YVAL_DIV_NUM + 1];
	bool	            mIsLinear;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================