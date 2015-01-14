﻿
#ifndef LUMINO_MATH_SQTTRANSFORM_H
#define LUMINO_MATH_SQTTRANSFORM_H

#include "Common.h"
#include "Vector3.h"
#include "Quaternion.h"

namespace Lumino
{

/**
	@brief		位置、回転、スケールを定義します。
*/
class LUMINO_EXPORT SQTTransform
{
public:

	Vector3		Scale;          ///< 拡大
	Quaternion	Rotation;       ///< 回転
	Vector3		Translation;    ///< 位置
	
public:
	
	static const SQTTransform   Identity;  ///< (Vector3::One, Quaternion::Identity, Vector3::Zero)
	
public:
	
	/**
		@brief		各要素を 拡縮1.0、単位クォータニオン、位置(0, 0) でインスタンスを初期化します。
	*/
    SQTTransform();

	/**
		@brief		各要素を指定してインスタンスを初期化します。
	*/
    SQTTransform(const Vector3& scale, const Quaternion& rotation, const Vector3& translation);
	
};

} // namespace Lumino

#endif // LUMINO_MATH_SQTTRANSFORM_H
