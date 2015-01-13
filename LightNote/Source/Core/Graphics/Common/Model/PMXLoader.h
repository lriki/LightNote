//==============================================================================
// PMXLoader 
//------------------------------------------------------------------------------
///**
//  @file       PMXLoader.h
//  @brief      PMXLoader
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "../../../Base/Misc.h"
#include "../../../FileIO/Interface.h"
#include "../../../FileIO/BinaryReader.h"
#include "Common.h"

namespace LNote
{
namespace Core
{
namespace Graphics
{

//==============================================================================
// PMXLoader
//------------------------------------------------------------------------------
///**
//  @brief
//*/
//==============================================================================
class PMXLoader
{
public:

#pragma pack( push, 1 )

	enum PMX_Encode
	{
		PMX_Encode_UTF16	= 0,
		PMX_Encode_UTF8		= 1,
	};

	struct PMX_Header
	{
		char	Magic[4];		// "PMX " (PMX1.0��"Pmx ")
		float	Version;		// PMD�o�[�W�����ԍ�
		lnU8	DataSize;		// �㑱����f�[�^��̃o�C�g�T�C�Y  PMX2.0�� 8 �ŌŒ�
		lnByte	Data[8];		// [0] - �G���R�[�h����  | 0:UTF16 1:UTF8
								// [1] - �ǉ�UV�� 	| 0�`4 �ڍׂ͒��_�Q��
								// [2] - ���_Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [3] - �e�N�X�`��Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [4] - �ގ�Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [5] - �{�[��Index�T�C�Y | 1,2,4 �̂����ꂩ
								// [6] - ���[�tIndex�T�C�Y | 1,2,4 �̂����ꂩ
								// [7] - ����Index�T�C�Y | 1,2,4 �̂����ꂩ
	};

#pragma pack( pop )

public:
	~PMXLoader();

public:

	/// �ǂݍ���
	ModelCore2* load(ModelManager* manager, FileIO::Stream* stream, const FileIO::PathNameW& baseDir, bool isDynamic);

private:
	PMX_Encode getEncode() { return (PMX_Encode)mPMXHeader.Data[0]; }
	int getAdditionalUVCount() { return mPMXHeader.Data[1]; }
	int getVertexIndexSize() { return mPMXHeader.Data[2]; }
	int getTextureIndexSize() { return mPMXHeader.Data[3]; }
	int getMaterialIndexSize() { return mPMXHeader.Data[4]; }
	int getBoneIndexSize() { return mPMXHeader.Data[5]; }
	int getMorphIndexSize() { return mPMXHeader.Data[6]; }
	int getRigidBodyIndexSize() { return mPMXHeader.Data[7]; }

private:
	void loadModelInfo( FileIO::BinaryReader* reader );
	void loadVertices( FileIO::BinaryReader* reader );
	void loadIndices( FileIO::BinaryReader* reader );
	void loadTextureTable(FileIO::BinaryReader* reader, const FileIO::PathNameW& baseDir);
	void loadMaterials( FileIO::BinaryReader* reader );
	void loadBones( FileIO::BinaryReader* reader );
	void loadMorphs( FileIO::BinaryReader* reader );
	void loadDisplayFrame( FileIO::BinaryReader* reader );
	void loadRigidBodys( FileIO::BinaryReader* reader );
	void loadJoints( FileIO::BinaryReader* reader );

	lnRefStringW readTextBuf( FileIO::BinaryReader* reader );
	lnRefStringW readString( FileIO::BinaryReader* reader, int byteSize );

private:
	ModelManager*				mManager;
	Graphics::IGraphicsDevice*	mGraphicsDevice;
	bool						mIsDynamic;
	lnU32						mFlags;
	Base::TempBuffer			mTempBuffer;
	ModelCore2*					mModelCore;
	PMX_Header					mPMXHeader;
	std::vector<Texture*>		mTextureTable;
};

} // namespace Graphics
} // namespace Core
} // namespace LNote
