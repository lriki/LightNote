//==============================================================================
// GLUtil 
//------------------------------------------------------------------------------
///**
//  @file       GLUtil.h
//  @brief      GLUtil
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Threading/EventFlag.h"
#include "../GraphicsResource.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// �� GLResource
//------------------------------------------------------------------------------
///**
//  @brief     ���\�[�X�̒x���\�z��⏕����N���X
//*/
//==============================================================================
class GLResource
    : /*public Base::ReferenceObject
	, */public GraphicsResource
{
public:

    GLResource()
        : mInitialized( false )
    {}

    virtual ~GLResource()
    {
        // �_���B�T�u�N���X�ŌĂԂ��ƁB
        //onDisposeGLResource();
    }

public:

	/// (called from GraphicsDevice::commitChangesGLResource())
    void commitChanges()
    {
        if (!mInitialized.isTrue())
        {
            onCommitGLResource();
            mInitialized.setTrue();
        }
    }

	/// ������`�悵�������\�[�X�̍X�V���K�v�Ȏ��ɌĂяo��
    void requestCommitGLResource() { mInitialized.setFalse(); }

protected:
	// GraphicsResource
	virtual void onPauseDevice() { onDisposeGLResource(); }
	virtual void onResumeDevice() { mInitialized.setFalse(); }

protected:

	/// GL���\�[�X�쐬
	///		Renderer::beginScene() ���炢�낢��o�R�ŌĂ΂��B
	///		���\�[�X�����쐬�ł���΍쐬�B�쐬�ς݂ł���΍X�V���s���B
    virtual void onCommitGLResource() = 0;

	/// GL���\�[�X���
	///		�����܂Ń��\�[�X�̉���B�q�[�v�ɂ���I�u�W�F�N�g���������K�v�͂Ȃ��B
    virtual void onDisposeGLResource() = 0;

private:
    Threading::EventFlag mInitialized;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================