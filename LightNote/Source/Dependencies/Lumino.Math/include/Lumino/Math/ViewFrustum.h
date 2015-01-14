
#ifndef LUMINO_MATH_VIEWFRUSTUM_H
#define LUMINO_MATH_VIEWFRUSTUM_H

#include "Common.h"
#include "Plane.h"

namespace Lumino
{
class Matrix;

/**
	@brief		3D �̎�������`���܂��B

	@details	������͎�� 3D ��ԏ�̃J�����̎����\�����߂Ɏg�p���܂��B
				�J������\���r���[�v���W�F�N�V�����s�񂩂�쐬�ł��܂��B

				������̊O���ɂ���I�u�W�F�N�g�͒ʏ�A�����_�����O����K�v�͂���܂���B
				Intersects() ���g�p���Ď�����ƃI�u�W�F�N�g�̌������� (�����ɑ��݂��邩) ���s�����ƂŁA
				�����_�����O���K�v�����`�F�b�N�ł��܂��B
*/
class LUMINO_EXPORT ViewFrustum
{
public:

	/**
		@brief		�w�肳�ꂽ�r���[�v���W�F�N�V�����s����g�p���ď��������܂��B
		@param[in]	viewProjMatrix	: ������̐����Ɏg�p����r���[�v���W�F�N�V�����s��
	*/
	ViewFrustum(const Matrix& viewProjMatrix);

public:
	
	/**
		@brief		�w�肳�ꂽ�r���[�v���W�F�N�V�����s����g�p���Ă��̎�������č\�z���܂��B
		@param[in]	viewProjMatrix	: ������̐����Ɏg�p����r���[�v���W�F�N�V�����s��
	*/
	void SetViewProjMatrix(const Matrix& viewProjMatrix);

	/**
		@brief		�w�肵���_�����̎�����̓����ɂ��邩�𔻒肵�܂��B
		@param[in]	point	: ���肷��_
		@return		�_�������ɂ���ꍇ�� true�A�����łȂ��ꍇ�� false�B
	*/
	bool Intersects(const Vector3& point) const;

	/**
		@brief		�w�肵���������̎�����ƌ������邩�𔻒肵�܂��B
		@param[in]	center	: ���̒��S�_
		@param[in]	radius	: ���̔��a
		@return		��������ꍇ�� true�A�����łȂ��ꍇ�� false�B(�ڐG���Ă���ꍇ�� true)
	*/
	bool Intersects(const Vector3& center, float radius) const;

	/**
		@brief		����̊e���_���擾���܂��B
		@param[out]	points	: ���_���W���i�[����z�� (�v�f���� 8 �ł��邱��)
		@details	���_�̏��Ԃ́ANear���ʂ̍���A�E��A�E���A�����AFar���ʂ̍���A�E��A�E���A�����ł��B
	*/
	void GetCornerPoints(Vector3* points) const;

	/**
		@brief		����̎w�肵���ʂ��擾���܂��B
	*/
	const Plane& GetPlane(FrustumPlane plane) const { return mPlanes[plane]; }

private:
	Plane		mPlanes[FrustumPlane_Max];
	Matrix		mViewProjMatrix;
};

} // namespace Lumino

#endif // LUMINO_MATH_VIEWFRUSTUM_H
