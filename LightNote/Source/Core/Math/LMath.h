
#pragma once

#include <LuminoMath.h>

namespace LNote
{

typedef Lumino::Math			LMath;
typedef Lumino::Vector2			LVector2;
typedef Lumino::Vector3			LVector3;
typedef Lumino::Vector4			LVector4;
typedef Lumino::Matrix			LMatrix;
typedef Lumino::Quaternion		LQuaternion;
typedef Lumino::SQTTransform	LSQTTransform;
//typedef Lumino::Randomizer		LRandom;

class LMatrixUtils
{
public:

	/// ピクセル単位の2D描画に使う射影行列の作成
	static void perspective2DLH(LMatrix* out_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_)
	{
		// ※このオフセット調整が必要なのは DX9 のみ。本来であればシェーダ内でたいおうするものなので、
		// 後々この処理は削除する。
#if 1
		out_->Set(
			2.0f / width_, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / height_, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
			-1.0f, 1.0f, near_ / (near_ - far_), 1.0f);
#else
		// 平行移動成分は、画面半分 + 0.5 ピクセルの調整
		out_->set(
			2.0f / width_, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / height_, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
			-1.0f - (0.5f / width_), 1.0f + (0.5f / height_), near_ / (near_ - far_), 1.0f);
#endif
	}
};

} // namespace LNote

#include "RandomEx.h"
