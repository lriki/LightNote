
#pragma once

#include "../Base/Unicode.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

/**
	@brief
	@details
*/
template<typename TChar>
class BasicPathName
{
public:
	static const TChar Separator = ((TCHAR)'/');

public:
	typedef BasicPathName<TChar> PathNameT;

public:
	BasicPathName() {}
	explicit BasicPathName(const char* path) { assign(path); }
	explicit BasicPathName(const wchar_t* path) { assign(path); }
	BasicPathName(const PathNameT& basePath, const char* relativePath) { assignUnderBasePath(basePath, relativePath); }		// relativePath がフルパスの場合は basePath を無視する
	BasicPathName(const PathNameT& basePath, const wchar_t* relativePath) { assignUnderBasePath(basePath, relativePath); }	// relativePath がフルパスの場合は basePath を無視する
	BasicPathName(const PathNameT& basePath, const PathNameT& relativePath) { assignUnderBasePath(basePath, relativePath); }// relativePath がフルパスの場合は basePath を無視する

public:

	void assign(const char* path);
	void assign(const wchar_t* path);

	void assignUnderBasePath(const PathNameT& basePath, const char* relativePath);
	void assignUnderBasePath(const PathNameT& basePath, const wchar_t* relativePath);
	void assignUnderBasePath(const PathNameT& basePath, const PathNameT& relativePath) { assignUnderBasePath(basePath, relativePath.getPath()); }


	/// ディレクトリまたはファイル名を追加する (path が絶対パスの場合はそれで上書きする)
	void append(const char* path);

	/// ディレクトリまたはファイル名を追加する (path が絶対パスの場合はそれで上書きする)
	void append(const wchar_t* path);

	const TChar* getPath() const { return mPath.c_str(); }
	bool isEmpty() const { return mPath.empty(); }

	/// フルパスであるか
	bool isFullPath() const;

	/// ルートディレクトリであるか ("C:" や "/")
	bool isRoot() const;

	/// 親ディレクトリの PathName を返す
	BasicPathName<TChar> getParent() const;

	/// パスを単純化し、フルパスにしたものを返す
	BasicPathName<TChar> canonicalizePath(Manager* manager) const;

	/// ローカルの char 文字列表現として返す
	std::string toLocalChar() const;

public:
	bool equals(const TChar* path) const;		// / \ の違いを区別しない
	bool operator==(const BasicPathName<TChar>& path) const { return equals(path.mPath.c_str()); }
	bool operator==(const TChar* path) const { return equals(path); }
	bool operator!=(const BasicPathName<TChar>& path) const { return !equals(path.mPath.c_str()); }
	bool operator!=(const TChar* path) const { return !equals(path); }

private:
	void internalCanonicalizePath(const char* srcPath, BasicPathName<char>* outPath) const;
	void internalCanonicalizePath(const wchar_t* srcPath, BasicPathName<wchar_t>* outPath) const;

private:
	typedef Base::BasicString<TChar> BasicStringT;
	BasicStringT	mPath;
	//std::wstring	mPath;	///< パス本体。絶対または相対パス
};

typedef BasicPathName<lnChar>	PathName;
typedef BasicPathName<char>		PathNameA;
typedef BasicPathName<wchar_t>	PathNameW;

} // namespace FileIO
} // namespace Core
} // namespace LNote
