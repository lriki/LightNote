//==============================================================================
// ModelIKLimitter
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../../../../Math/Vector3.h"
#include "../../../../Math/SQTTransform.h"
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
		RotationMin.set( -LMath::PI, -LMath::PI, -LMath::PI );
		RotationMax.set( LMath::PI, LMath::PI, LMath::PI );
		Restitution.set( 0.5f, 0.5f, 0.5f );
		
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

		LVector3 euler;
		LQuaternion::toEuler( &euler, localTransform->Rotation );

        euler.x = adjustRotation( euler.x, 0 );
        euler.y = adjustRotation( euler.y, 1 );
        euler.z = adjustRotation( euler.z, 2 );

        LQuaternion::fromEuler( &localTransform->Rotation, euler );
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
            rot->x = 0.0f;
        }
        if ( AxisLimits[1] )
        {
            rot->y = 0.0f;
        }
        if ( AxisLimits[2] )
        {
            rot->z = 0.0f;
        }
	}

} // namespace Graphics
} // namespace Core
} // namespace LNote
