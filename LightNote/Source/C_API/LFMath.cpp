//==============================================================================
// LFMath
//==============================================================================

#pragma once

#include "stdafx.h"
#include "../Core/Math/LMath.h"
#include "LFInternal.h"
#include "LFFramework.h"
#include "LFMath.h"

//==============================================================================
// LNVector2
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector2_Set( LNVector2* vec, float x, float y )
	{
		LN_CHECK_ARG(vec != NULL);
		vec->X = x; vec->Y = y;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector2_GetLength(const LNVector2* vec, float* length)
	{
		LN_CHECK_ARG(vec != NULL);
		*length = ((const LVector2*)vec)->GetLength();
		return ::LN_OK;
	}
	LNResult LNVector2_GetLengthD(const LNVector2* vec, double* length)
	{
		float lengthF;
		LNResult r = LNVector2_GetLength(vec, &lengthF);
		*length = lengthF;
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector2_GetSquareLength(const LNVector2* vec, float* length)
	{
		LN_CHECK_ARG(vec != NULL);
		*length = (vec->X * vec->X + vec->Y * vec->Y);
		return ::LN_OK;
	}
	LNResult LNVector2_GetSquareLengthD(const LNVector2* vec, double* length)
	{
		float lengthF;
		LNResult r = LNVector2_GetSquareLength(vec, &lengthF);
		*length = lengthF;
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector2_Normalize(const LNVector2* vec, LNVector2* outVec)
	{
		LN_CHECK_ARG(vec != NULL);
		*TO_CORE_VEC2_PTR(outVec) = LVector2::Normalize(TO_CORE_VEC2_REF(vec));
		return ::LN_OK;
	}

//==============================================================================
// LNVector3
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Set(LNVector3* vec, float x, float y, float z)
	{
		LN_CHECK_ARG(vec != NULL);
		vec->X = x; vec->Y = y; vec->Z = z;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_SetVec2(LNVector3* vec, const LNVector2* vec2, float z)
	{
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(vec2 != NULL);
		vec->X = vec2->X; vec->Y = vec2->Y; vec->Z = z;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_GetLength(const LNVector3* vec, float* length)
	{
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(length != NULL);
		*length = ((const LVector3*)vec)->GetLength();
		return ::LN_OK;
	}
	LNResult LNVector3_GetLengthD(const LNVector3* vec, double* length)
	{
		float lengthF;
		LNResult r = LNVector3_GetLength(vec, &lengthF);
		*length = lengthF;
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_GetSquareLength(const LNVector3* vec, float* length)
	{
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(length != NULL);
		*length =(vec->X * vec->X + vec->Y * vec->Y + vec->Z * vec->Z);
		return ::LN_OK;
	}
	LNResult LNVector3_GetSquareLengthD(const LNVector3* vec, double* length)
	{
		float lengthF;
		LNResult r = LNVector3_GetSquareLength(vec, &lengthF);
		*length = lengthF;
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Normalize(const LNVector3* vec, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::Normalize(TO_CORE_VEC3_REF(vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Dot(const LNVector3* vec1, const LNVector3* vec2, float* dot)
	{
		LN_CHECK_ARG(vec1 != NULL);
		LN_CHECK_ARG(vec2 != NULL);
		LN_CHECK_ARG(dot != NULL);
		*dot = LVector3::Dot(TO_CORE_VEC3_REF(vec1), TO_CORE_VEC3_REF(vec2));
		return ::LN_OK;
	}
	LNResult LNVector3_DotD(const LNVector3* vec1, const LNVector3* vec2, double* dot)
	{
		float dotF;
		LNResult r = LNVector3_Dot(vec1, vec2, &dotF);
		*dot = dotF;
		return r;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Cross(const LNVector3* vec1, const LNVector3* vec2, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec1 != NULL);
		LN_CHECK_ARG(vec2 != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::Cross(TO_CORE_VEC3_REF(vec1), TO_CORE_VEC3_REF(vec2));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Reflect(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(normal != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::Reflect(TO_CORE_VEC3_REF(vec), TO_CORE_VEC3_REF(normal));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Slide(const LNVector3* vec, const LNVector3* normal, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(normal != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::Slide(TO_CORE_VEC3_REF(vec), TO_CORE_VEC3_REF(normal));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Lerp(const LNVector3* vec1, const LNVector3* vec2, float t, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec1 != NULL);
		LN_CHECK_ARG(vec2 != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::Lerp(TO_CORE_VEC3_REF(vec1), TO_CORE_VEC3_REF(vec2), t);
		return ::LN_OK;
	}
	LNResult LNVector3_LerpD(const LNVector3* vec1, const LNVector3* vec2, double t, LNVector3* outVec)
	{
		return LNVector3_Lerp(vec1, vec2, t, outVec);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_CatmullRom(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, float t, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec1 != NULL);
		LN_CHECK_ARG(vec2 != NULL);
		LN_CHECK_ARG(vec3 != NULL);
		LN_CHECK_ARG(vec4 != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::CatmullRom(
			TO_CORE_VEC3_REF( vec1 ), 
			TO_CORE_VEC3_REF( vec2 ), 
			TO_CORE_VEC3_REF( vec3 ), 
			TO_CORE_VEC3_REF( vec4 ),
			t );
		return ::LN_OK;
	}
	LNResult LNVector3_CatmullRomD(const LNVector3* vec1, const LNVector3* vec2, const LNVector3* vec3, const LNVector3* vec4, double t, LNVector3* outVec)
	{
		return LNVector3_CatmullRom(vec1, vec2, vec3, vec4, t, outVec);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_Transform(const LNVector3* vec, const LNMatrix* mat, LNVector4* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC4_PTR(outVec) = LVector3::Transform(TO_CORE_VEC3_REF(vec), TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_TransformCoord(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::TransformCoord(TO_CORE_VEC3_REF(vec), TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}

#if 0
	//----------------------------------------------------------------------
	///**
	//  @brief      任意軸周りの回転を行う
	//  @param[in]  vec		: 処理の基になる Vector3
	//  @param[in]  axis	: 回転軸を表す Vector3
	//  @param[in]  r		: 回転角度 (ラジアン)
	//  @param[out] outVec	: 演算結果を格納する Vector3
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_RotateAxis(const LNVector3* vec, const LNVector3* axis, float r, LNVector3* outVec);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_RotateAxis(const LNVector3* vec, const LNVector3* axis, float r, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(axis != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::RotateAxis(TO_CORE_VEC3_REF(vec), TO_CORE_VEC3_REF(axis), r);
		return ::LN_OK;
	}
	LNOTEINTERNALAPI LNResult LNVector3_RotateAxisD(const LNVector3* vec, const LNVector3* axis, double r, LNVector3* outVec);


	LNResult LNVector3_RotateAxisD(const LNVector3* vec, const LNVector3* axis, double r, LNVector3* outVec)
	{
		return LNVector3_RotateAxis(vec, axis, r, outVec);
	}
#endif
#if 0
	//----------------------------------------------------------------------
	///**
	//  @brief      行列の回転成分だけを使って座標変換する
	//  @param[in]  vec		: 処理の基になる Vector3
	//  @param[in]  mat		: 処理の基になる Matrix
	//  @param[out] outVec	: 演算結果を格納する Vector3
	//  @return		処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNVector3_RotateMatrix(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec);

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNVector3_RotateMatrix(const LNVector3* vec, const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(outVec != NULL);
		LN_CHECK_ARG(vec != NULL);
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR(outVec) = LVector3::rotate(TO_CORE_VEC3_REF(vec), TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}
#endif


//==============================================================================
// LNMatrix
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Identity(LNMatrix* mat)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_MAT_PTR(mat) = LMatrix::Identity;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Translate(LNMatrix* mat, float x, float y, float z)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->Translate(x, y, z);
		return ::LN_OK;
	}
	LNResult LNMatrix_TranslateD(LNMatrix* mat, double x, double y, double z)
	{
		return LNMatrix_Translate(mat, x, y, z);
	}
    
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_TranslateVec3(LNMatrix* mat, const LNVector3* vec)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->Translation(TO_CORE_VEC3_REF(vec));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateX(LNMatrix* mat, float radian)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->RotateX(radian);
		return ::LN_OK;
	}
	LNResult LNMatrix_RotateXD(LNMatrix* mat, double radian)
	{
		return LNMatrix_RotateX(mat, radian);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateY(LNMatrix* mat, float radian)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->RotateY(radian);
		return ::LN_OK;
	}
	LNResult LNMatrix_RotateYD(LNMatrix* mat, double radian)
	{
		return LNMatrix_RotateY(mat, radian);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateZ(LNMatrix* mat, float radian)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->RotateZ(radian);
		return ::LN_OK;
	}
	LNResult LNMatrix_RotateZD(LNMatrix* mat, double radian)
	{
		return LNMatrix_RotateZ(mat, radian);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Rotate(LNMatrix* mat, float xRad, float yRad, float zRad, LNRotationOrder rotOrder)
	{
		LN_CHECK_ARG(mat != NULL);
		LMatrix m = LMatrix::RotationEulerAngles(LVector3(xRad, yRad, zRad), (Lumino::RotationOrder)rotOrder);
		*(TO_CORE_MAT_PTR(mat)) *= m;
		//LMatrix::rotation( TO_CORE_MAT_PTR( mat ), xRad, yRad, zRad, (Core::Math::RotationOrder)rotOrder );
		return ::LN_OK;
	}
	LNResult LNMatrix_RotateD(LNMatrix* mat, double xRad, double yRad, double zRad, LNRotationOrder rotOrder)
	{
		return LNMatrix_Rotate(mat, xRad, yRad, zRad, rotOrder);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateVec3(LNMatrix* mat, const LNVector3* vec, LNRotationOrder rotOrder)
	{
		return LNMatrix_Rotate(mat, vec->X, vec->Y, vec->Z, rotOrder);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateAxis(LNMatrix* mat, const LNVector3* axis, float radian)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR( mat )->RotateAxis( TO_CORE_VEC3_REF( axis ), radian );
		return ::LN_OK;
	}
	LNResult LNMatrix_RotateAxisD(LNMatrix* mat, const LNVector3* axis, double radian)
	{
		return LNMatrix_RotateAxis(mat, axis, radian);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_RotateQuaternion(LNMatrix* mat, const LNQuaternion* qua)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->RotateQuaternion(TO_CORE_QUA_REF(qua));
		return ::LN_OK;
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Scale(LNMatrix* mat, float xyz)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR( mat )->Scaling( xyz );
		return ::LN_OK;
	}
	LNResult LNMatrix_ScaleD(LNMatrix* mat, double xyz)
	{
		return LNMatrix_Scale(mat, (float)xyz);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_ScaleXYZ(LNMatrix* mat, float x, float y, float z)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->Scaling(x, y, z);
		return ::LN_OK;
	}
	LNResult LNMatrix_ScaleXYZD(LNMatrix* mat, double x, double y, double z)
	{
		return LNMatrix_ScaleXYZ(mat, x, y, z);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_ScaleVec3(LNMatrix* mat, const LNVector3* scale)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_PTR(mat)->Scaling(TO_CORE_VEC3_REF(scale));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Multiply(const LNMatrix* mat1, const LNMatrix* mat2, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::Multiply(TO_CORE_MAT_REF(mat1), TO_CORE_MAT_REF(mat2));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Inverse(const LNMatrix* mat, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::Inverse(TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Transpose(const LNMatrix* mat, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::Transpose(TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}
 
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_ViewTransformLH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::LookAtLH(TO_CORE_VEC3_REF(pos), TO_CORE_VEC3_REF(lookAt), TO_CORE_VEC3_REF(upDir));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_ViewTransformRH(const LNVector3* pos, const LNVector3* lookAt, const LNVector3* upDir, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::LookAtRH(TO_CORE_VEC3_REF(pos), TO_CORE_VEC3_REF(lookAt), TO_CORE_VEC3_REF(upDir));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::PerspectiveFovLH(fovY, aspect, nearZ, farZ);
		return ::LN_OK;
	}
	LNResult LNMatrix_PerspectiveFovLHD(double fovY, double aspect, double nearZ, double farZ, LNMatrix* matOut)
	{
		return LNMatrix_PerspectiveFovLH(fovY, aspect, nearZ, farZ, matOut);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::PerspectiveFovRH(fovY, aspect, nearZ, farZ);
		return ::LN_OK;
	}
	LNResult LNMatrix_PerspectiveFovRHD(double fovY, double aspect, double nearZ, double farZ, LNMatrix* matOut)
	{
		return LNMatrix_PerspectiveFovRH(fovY, aspect, nearZ, farZ, matOut);
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_OrthoLH(float width, float height, float nearZ, float farZ, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::OrthoLH(width, height, nearZ, farZ);
		return ::LN_OK;
	}
	LNResult LNMatrix_OrthoLHD(double width, double height, double nearZ, double farZ, LNMatrix* matOut)
	{
		return LNMatrix_OrthoLH(width, height, nearZ, farZ, matOut);
	}
	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_OrthoRH(float width, float height, float nearZ, float farZ, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*TO_CORE_MAT_PTR(matOut) = LMatrix::OrthoRH(width, height, nearZ, farZ);
		return ::LN_OK;
	}
	LNResult LNMatrix_OrthoRHD(double width, double height, double nearZ, double farZ, LNMatrix* matOut)
	{
		return LNMatrix_OrthoRH(width, height, nearZ, farZ, matOut);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_GetRight(const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR( outVec ) = TO_CORE_MAT_REF( mat ).GetRight();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_GetUp(const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR( outVec ) = TO_CORE_MAT_REF( mat ).GetUp();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_GetFront(const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR( outVec ) = TO_CORE_MAT_REF( mat ).GetFront();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_GetPosition(const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR( outVec ) = TO_CORE_MAT_REF( mat ).GetPosition();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_ToEuler(const LNMatrix* mat, LNVector3* outVec)
	{
		LN_CHECK_ARG(mat != NULL);
		*TO_CORE_VEC3_PTR(outVec) = TO_CORE_MAT_REF(mat).ToEulerAngles();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_Decompose(const LNMatrix* mat, LNVector3* scale, LNQuaternion* rot, LNVector3* trans)
	{
		LN_CHECK_ARG(mat != NULL);
		TO_CORE_MAT_REF(mat).Decompose(TO_CORE_VEC3_PTR(scale), TO_CORE_QUA_PTR(rot), TO_CORE_VEC3_PTR(trans));
		return ::LN_OK;
	}

#if 0
	//----------------------------------------------------------------------
	///**
	//  @brief			右手系⇔左手系の変換
	//  @param[in]		mat		: 処理の元になる行列
	//  @param[out]		matOut	: 演算結果を格納する行列
	//  @return			処理結果 (LN_OK=成功 / それ以外=エラーコード)
	//*/
	//----------------------------------------------------------------------
	LNOTEAPI LNResult LNMatrix_TransformBasis(const LNMatrix* mat, LNMatrix* matOut);
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNMatrix_TransformBasis(const LNMatrix* mat, LNMatrix* matOut)
	{
		LN_CHECK_ARG(matOut != NULL);
		*matOut = *mat;
		LMatrix::transformBasis(TO_CORE_MAT_PTR(matOut));
		return ::LN_OK;
	}
#endif

//==============================================================================
// LNQuaternion
//==============================================================================

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Set(LNQuaternion* qua, float x, float y, float z, float w)
	{
		LN_CHECK_ARG(qua != NULL);
		qua->X = x; qua->Y = y; qua->Z = z; qua->W = w;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Identity(LNQuaternion* qua)
	{
		LN_CHECK_ARG(qua != NULL);
		*TO_CORE_QUA_PTR(qua) = LQuaternion::Identity;
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_RotationAxis(const LNVector3* axis, float r, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::RotationAxis(TO_CORE_VEC3_REF(axis), r);
		return ::LN_OK;
	}
	LNResult LNQuaternion_RotationAxisD(const LNVector3* axis, float r, LNQuaternion* outQua)
	{
		return LNQuaternion_RotationAxis(axis, r, outQua);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_RotationMatrix(const LNMatrix* mat, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::RotationMatrix(TO_CORE_MAT_REF(mat));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_RotationYawPitchRoll(float yaw, float pitch, float roll, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::RotationYawPitchRoll(yaw, pitch, roll);
		return ::LN_OK;
	}
	LNResult LNQuaternion_RotationYawPitchRollD(float yaw, float pitch, float roll, LNQuaternion* outQua)
	{
		return LNQuaternion_RotationYawPitchRoll(yaw, pitch, roll, outQua);
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Normalize(const LNQuaternion* qua, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::Normalize(TO_CORE_QUA_REF( qua ) );
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Conjugate(const LNQuaternion* qua, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::Conjugate(TO_CORE_QUA_REF(qua));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Multiply(const LNQuaternion* qua1, const LNQuaternion* qua2, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::Multiply(TO_CORE_QUA_REF(qua1), TO_CORE_QUA_REF(qua2));
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNQuaternion_Slerp(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua)
	{
		LN_CHECK_ARG(outQua != NULL);
		*TO_CORE_QUA_PTR(outQua) = LQuaternion::Slerp(TO_CORE_QUA_REF(qua1), TO_CORE_QUA_REF(qua2), t);
		return ::LN_OK;
	}
	LNResult LNQuaternion_SlerpD(const LNQuaternion* qua1, const LNQuaternion* qua2, float t, LNQuaternion* outQua)
	{
		return LNQuaternion_Slerp(qua1, qua2, t, outQua);
	}

//==============================================================================
// LNRandom
//==============================================================================

	LNOTE_TYPE_INFO_IMPL(LRandom, LNRandom);

	LRandom gRandom;

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_Create(LNHandle* random)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Math::Random> obj(LN_NEW Math::Random());
		obj.addRef();
		*random = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_CreateSeed(LNHandle* random, int seed)
	{
		LN_FUNC_TRY_BEGIN;
		LRefPtr<Math::Random> obj(LN_NEW Math::Random(seed));
		obj.addRef();
		*random = TO_HANDLE(FuncLibManager::registerReferenceObject(obj));
		LN_FUNC_TRY_END_RETURN;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_SetSeed(LNHandle random, int seed)
	{
		if (random)
			TO_REFOBJ(Math::Random, random)->SetSeed(seed);
		else
			gRandom.SetSeed(seed);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetInt(LNHandle random, int* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetInt();
		else
			*outValue = gRandom.GetInt();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetIntRange(LNHandle random, int minValue, int maxValue, int* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetIntRange(minValue, maxValue);
		else
			*outValue = gRandom.GetIntRange(minValue, maxValue);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetIntDeflect(LNHandle random, int center, int width, int* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetIntWidth(center, width);
		else
			*outValue = gRandom.GetIntWidth(center, width);
		return ::LN_OK;
	}


#if 1

	
	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetFloat(LNHandle random, float* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetFloat();
		else
			*outValue = gRandom.GetFloat();
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetFloatRange(LNHandle random, float minValue, float maxValue, float* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetFloatRange(minValue, maxValue);
		else
			*outValue = gRandom.GetFloatRange(minValue, maxValue);
		return ::LN_OK;
	}

	//----------------------------------------------------------------------
	//
	//----------------------------------------------------------------------
	LNResult LNRandom_GetFloatDeflect(LNHandle random, float center, float width, float* outValue)
	{
		if (random)
			*outValue = TO_REFOBJ(Math::Random, random)->GetFloatWidth(center, width);
		else
			*outValue = gRandom.GetFloatWidth(center, width);
		return ::LN_OK;
	}

#endif
