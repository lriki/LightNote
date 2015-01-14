//==============================================================================
// ModelIKLimitter
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../../../Math/LMath.h"
#include "ModelIKLimitter.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// ModelIKLimitter
//==============================================================================
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	ModelIKLimitter2::ModelIKLimitter2()
	{
		RotationMin.Set( -LMath::PI, -LMath::PI, -LMath::PI );
		RotationMax.Set( LMath::PI, LMath::PI, LMath::PI );
		Restitution.Set( 0.5f, 0.5f, 0.5f );
		
        for (int i = 0; i < 3; i++)
            Mirror[i] = false;

		setAxisLimits( false, false, false );

		EnableRotationLimit = false;
		EnableAxisLimit = false;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIKLimitter2::setRotationLimits( const LVector3& min, const LVector3& max )
	{
		RotationMin = min;
		RotationMax = max;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIKLimitter2::setRotationRestitution( bool x, bool y, bool z, const LVector3& restitution )
	{
		Mirror[0] = x;
		Mirror[1] = y;
		Mirror[2] = z;
		Restitution = restitution;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	float ModelIKLimitter2::adjustRotation( float value, int index )
    {
		float* minRot = (float*)&RotationMin;
		float* maxRot = (float*)&RotationMin;
		float* restitution = (float*)&Restitution;

        if (minRot[index] > maxRot[index])
        {
			//Šp“x‚ª‹t‚È‚ç“ü‚ê‘Ö‚¦‚Ä‚¨‚­
            float temp = minRot[index];
            minRot[index] = maxRot[index];
            maxRot[index] = temp;
        }
        if (maxRot[index] < value)
        {
            if (Mirror[index])
                return maxRot[index] * (1 + restitution[index]) - value * restitution[index];
            else
                return maxRot[index];
        }
        else if (minRot[index] > value)
        {
            if (Mirror[index])
                return minRot[index] * (1 + restitution[index]) - value * restitution[index];
            else
                return minRot[index];
        }
        else
            return value;
    }
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIKLimitter2::adjustRotation( LSQTTransform* localTransform )
	{
		if ( !EnableRotationLimit ) return;

		LVector3 euler = localTransform->Rotation.ToEulerAngles();

        euler.X = adjustRotation( euler.X, 0 );
        euler.Y = adjustRotation( euler.Y, 1 );
        euler.Z = adjustRotation( euler.Z, 2 );

		localTransform->Rotation = LQuaternion::RotationEulerAngles(euler);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIKLimitter2::setAxisLimits( bool x, bool y, bool z )
	{
		AxisLimits[0] = x;
		AxisLimits[1] = y;
		AxisLimits[2] = z;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	void ModelIKLimitter2::adjustAxisLimits( LVector3* rot )
	{
		if ( !EnableAxisLimit ) return;

		if ( AxisLimits[0] )
        {
            rot->X = 0.0f;
        }
        if ( AxisLimits[1] )
        {
            rot->Y = 0.0f;
        }
        if ( AxisLimits[2] )
        {
            rot->Z = 0.0f;
        }
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
