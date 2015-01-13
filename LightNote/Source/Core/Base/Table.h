//==============================================================================
// Table
//==============================================================================

#pragma once

#include <vector>
#include <algorithm>
#include "ReferenceObject.h"

namespace LNote
{
namespace Core
{
namespace Base
{

//==============================================================================
// Table
//------------------------------------------------------------------------------
///**
//	@brief 
//*/
//==============================================================================
template<typename T>
class Table 
	: public ReferenceObject
{
public:
	Table()
		: mBuffer(NULL)
		, mXSize(0)
		, mYSize(0)
		, mZSize(0)
	{
	}

	Table(size_t xSize, size_t ySize, size_t zSize)
		: mBuffer(NULL)
		, mXSize(0)
		, mYSize(0)
		, mZSize(0)
	{
		resize(xSize, ySize, zSize);
	}

	Table(size_t xSize, size_t ySize, size_t zSize, const T* srcData)
		: mBuffer(NULL)
		, mXSize(0)
		, mYSize(0)
		, mZSize(0)
	{
		resize(xSize, ySize, zSize);
		memcpy(mBuffer, srcData, sizeof(T) * xSize * ySize * zSize);
	}

	~Table()
	{
		SAFE_DELETE_ARRAY(mBuffer);
	}

public:
	/// 値の設定
	void setValue(size_t x, size_t y, size_t z, T value)
	{
		mBuffer[((mYSize * mXSize) * z) + (mYSize * y) + x] = value;
		//mBufferArray[z][y][x] = value;
	}

	/// 値の取得
	T getValue(size_t x, size_t y, size_t z)
	{
		return mBuffer[((mYSize * mXSize) * z) + (mYSize * y) + x];
		//return mBufferArray[z][y][x];
	}

	/// X 要素数の取得
	size_t getXSize() const { return mXSize; }//{ return mDataArray[0][0].size(); }

	/// Y 要素数の取得
	size_t getYSize() const { return mYSize; }//{ return mDataArray[0].size(); }

	/// Z 要素数の取得
	size_t getZSize() const { return mZSize; }//{ return mDataArray.size(); }

	/// 配列サイズの変更
	void resize(size_t xSize, size_t ySize, size_t zSize)
	{
		SAFE_DELETE_ARRAY(mBuffer);

		T* oldBuffer = mBuffer;
		size_t oldXSize = mXSize;
		size_t oldYSize = mYSize;
		size_t oldZSize = mZSize;

		mXSize = xSize;
		mYSize = ySize;
		mZSize = zSize;
		mBuffer = LN_NEW T[mXSize * mYSize * mZSize];

		// 0 クリア
		memset(mBuffer, 0, sizeof(T) * mXSize * mYSize * mZSize);

		// old からコピーする
		if (oldBuffer != NULL)
		{
			size_t xCount = std::min(oldXSize, mXSize);
			size_t yCount = std::min(oldYSize, mYSize);
			size_t zCount = std::min(oldZSize, mZSize);

			for (size_t z = 0; z < zCount; ++z) {
				for (size_t y = 0; y < yCount; ++y) {
					memcpy(
						&(mBuffer[((mYSize * mXSize) * z) + (mYSize * y)]),
						&(oldBuffer[((mYSize * mXSize) * z) + (mYSize * y)]),
						sizeof(T) * xCount);
					
					//for (int x = 0; x < xCount; ++x) {
					//	setValue(x, y, z, b[zOffset * z + xSize * y + x]);
					//}
				}
			}

			delete[] oldBuffer;
		}
	}

	/// データコピー
	void copy(const Table<T>* srcTable)
	{
		if (mXSize != srcTable->mXSize ||
			mYSize != srcTable->mYSize ||
			mZSize != srcTable->mZSize) 
		{
			resize(srcTable->mXSize, srcTable->mYSize, srcTable->mZSize);
		}
		memcpy(mBuffer, srcTable->mBuffer, sizeof(T) * mXSize * mYSize * mZSize);
	}

private:
	//typename typedef std::vector<T>				DataArray;
	//typename typedef std::vector<DataArray>		DataArray2;
	//typename typedef std::vector<DataArray2>	DataArray3;
	//DataArray3	mDataArray;

	T*		mBuffer;
	size_t	mXSize;
	size_t	mYSize;
	size_t	mZSize;
};

//==============================================================================
// IntTable
//------------------------------------------------------------------------------
///**
//	@brief 
//*/
//==============================================================================
class IntTable : public Table<int>
{
	LN_TYPE_INFO_ACCESS_DECL;
public:
	IntTable()
		: Table<int>()
	{}

	IntTable(size_t xSize, size_t ySize, size_t zSize)
		: Table<int>(xSize, ySize, zSize)
	{}

	IntTable(size_t xSize, size_t ySize, size_t zSize, const int* srcData)
		: Table<int>(xSize, ySize, zSize, srcData)
	{}
};

} // namespace Base
} // namespace Core
} // namespace LNote
