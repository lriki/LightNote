
#pragma once

#include "../../Lumino/Lumino.Math/include/LuminoMath.h"

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

class LMathUtils
{
public:

#if LNOTE_INCLUDED_DIRECTX_HEADER
	// D3DXVECTOR3 �ɕϊ�
	static D3DXVECTOR3 toD3DXVECTOR3(const LVector3& v)
	{
		return D3DXVECTOR3(
			static_cast< float >(v.X),
			static_cast< float >(v.Y),
			static_cast< float >(v.Z));
	}
#endif
};

class LMatrixUtils
{
public:

	/// �s�N�Z���P�ʂ�2D�`��Ɏg���ˉe�s��̍쐬
	static void perspective2DLH(LMatrix* out_, lnFloat width_, lnFloat height_, lnFloat near_, lnFloat far_)
	{
		// �����̃I�t�Z�b�g�������K�v�Ȃ̂� DX9 �̂݁B�{���ł���΃V�F�[�_���ł�������������̂Ȃ̂ŁA
		// ��X���̏����͍폜����B
#if 1
		out_->Set(
			2.0f / width_, 0.0f, 0.0f, 0.0f,
			0.0f, -2.0f / height_, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
			-1.0f, 1.0f, near_ / (near_ - far_), 1.0f);
#else
		// ���s�ړ������́A��ʔ��� + 0.5 �s�N�Z���̒���
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
