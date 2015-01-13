
#pragma once

#include "stdafx.h"
#include "../Base/StringUtils.h"
#include "../Base/Unicode.h"
#include "Manager.h"
#include "FileUtil.h"
#include "PathName.h"

namespace LNote
{
namespace Core
{
namespace FileIO
{

//==============================================================================
// PathName
//==============================================================================


//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::assign(const char* path)
{
	mPath.assignNativeString(path, 0, BasicStringT::npos);
	Path::normalizePath(&mPath);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::assign(const wchar_t* path)
{
	mPath.assignNativeString(path, 0, BasicStringT::npos);
	Path::normalizePath(&mPath);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::assignUnderBasePath(const PathNameT& basePath, const char* relativePath)
{
	if (Path::isFullPath(relativePath))
	{
		mPath.assignNativeString(relativePath);
	}
	else
	{
		mPath = basePath.mPath;
		if ((*mPath.rbegin()) != Separator) {	// 末尾セパレータ
			mPath += Separator;
		}

		BasicStringT rel;
		rel.assignNativeString(relativePath);
		mPath += rel;
	}

	Path::normalizePath(&mPath);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::assignUnderBasePath(const PathNameT& basePath, const wchar_t* relativePath)
{
	if (Path::isFullPath(relativePath))
	{
		mPath.assignNativeString(relativePath);
	}
	else
	{
		mPath = basePath.mPath;
		if ((*mPath.rbegin()) != Separator) {	// 末尾セパレータ
			mPath += Separator;
		}

		BasicStringT rel;
		rel.assignNativeString(relativePath);
		mPath += rel;
	}

	Path::normalizePath(&mPath);
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::append(const char* path)
{
	BasicStringT tmp;
	tmp.assignNativeString(path);

	if (Path::isFullPath(path)) {
		mPath += tmp;
	}
	else {
		mPath += Separator;
		mPath += tmp;
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::append(const wchar_t* path)
{
	BasicStringT tmp;
	tmp.assignNativeString(path);

	if (Path::isFullPath(path)) {
		mPath = tmp;
	}
	else {
		if (mPath.size() > 0 && (*mPath.rbegin()) != Separator) {	// 末尾セパレータ
			mPath += Separator;
		}
		mPath += tmp;
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
bool BasicPathName<TChar>::isFullPath() const
{
	return Path::isFullPath(mPath.c_str());
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
bool BasicPathName<TChar>::isRoot() const
{
#ifdef _WIN32
	// windows の場合、絶対パスかつパス末尾が ":/" であるか
	if (isFullPath() && mPath.size() >= 2)
	{
		if (mPath[mPath.size() - 2] == ':' &&
			mPath[mPath.size() - 1] == '/')
		{
			return true;
		}
	}
	return false;
#else
	// UNIX の場合、/ だけであるか
	if (mPath.size() == 1 && mPath[0] == L"/") {
		return true;
	}
	return false;
#endif
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
BasicPathName<TChar> BasicPathName<TChar>::getParent() const
{
	LN_THROW_InvalidOperation(!isRoot());	// ルートディレクトリの親は無効

	lnRefStringW path = Path::getDirectoryPath(mPath.c_str());
	return BasicPathName<TChar>(path.c_str());
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
BasicPathName<TChar> BasicPathName<TChar>::canonicalizePath(Manager* manager) const
{
	BasicPathName<TChar> newPath;

	// フルパスである場合は単純化するだけ
	if (isFullPath())
	{
		internalCanonicalizePath(mPath.c_str(), &newPath);
	}
	// フルパスでなければ、Manager のカレントディレクトリからフルパスを作って単純化
	// (内部カレントディレクトリ対応のため)
	else
	{
		BasicPathName<TChar> path(manager->getCurrentDirectory().getPath());
		path.append(mPath.c_str());
		path.internalCanonicalizePath(mPath.c_str(), &newPath);
	}

	return newPath;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
std::string BasicPathName<TChar>::toLocalChar() const
{
	Base::BasicString<char> tmp;
	tmp.assignNativeString(mPath.c_str());
	return tmp;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
//void BasicPathName::normalizePath()
//{
//	wchar_t tmpPath[LN_MAX_PATH];
//	wchar_t* canonPath;
//#ifdef _WIN32
//	canonPath = _wfullpath(tmpPath, mPath.c_str(), LN_MAX_PATH);
//#else
//	canonPath = realpath(path.c_str(), pathbuf);
//#endif
//	if (canonPath == NULL) {
//		mPath = L"";
//	}
//	else {
//		mPath = tmpPath;
//	}
//	Path::normalizePath(&mPath);
//}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
bool BasicPathName<TChar>::equals(const TChar* path) const
{
	int len = Base::StringUtils::GetLength(path);
	if (mPath.size() != len) {
		return false;
	}

	const TChar* s1 = mPath.c_str();
	const TChar* s2 = path;
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		{
			if ((*s1 == ((TChar)'/') || *s1 == ((TChar)'\\')) &&
				(*s2 == ((TChar)'/') || *s2 == ((TChar)'\\')))
			{
				// / と \ は同一とみなす
			}
			else {
				return false;
			}
		}
		s1++;
		s2++;
	}

	return true;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::internalCanonicalizePath(const char* srcPath, BasicPathName<char>* outPath) const
{
	char tmpPath[LN_MAX_PATH + 1] = { 0 };
	char* canonPath;
#ifdef _WIN32
	canonPath = _fullpath(tmpPath, srcPath, LN_MAX_PATH);
#else
	canonPath = realpath(mPath.c_str(), pathbuf);
#endif
	if (canonPath == NULL) {	// 変換失敗
		*outPath = BasicPathName<char>();
	}
	else {
		*outPath = BasicPathName<char>(tmpPath);
	}
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
template<typename TChar>
void BasicPathName<TChar>::internalCanonicalizePath(const wchar_t* srcPath, BasicPathName<wchar_t>* outPath) const
{
	wchar_t tmpPath[LN_MAX_PATH + 1] = { 0 };
	wchar_t* canonPath;
#ifdef _WIN32
	canonPath = _wfullpath(tmpPath, srcPath, LN_MAX_PATH);
#else
	canonPath = realpath(mPath.c_str(), pathbuf);
#endif
	if (canonPath == NULL) {	// 変換失敗
		*outPath = BasicPathName<wchar_t>();
	}
	else {
		*outPath = BasicPathName<wchar_t>(tmpPath);
	}
}

#if 0
template<typename TChar>
void BasicPathName<TChar>::internalCanonicalizePath(BasicPathName<TChar>* outPath) const
{
	wchar_t tmpPath[LN_MAX_PATH];
	wchar_t* canonPath;
#ifdef _WIN32
	canonPath = _wfullpath(tmpPath, mPath.c_str(), LN_MAX_PATH);
#else
	canonPath = realpath(mPath.c_str(), pathbuf);
#endif
	if (canonPath == NULL) {
		*outPath = PathName();
	}
	else {
		*outPath = PathName(tmpPath);
	}
}
#endif

// テンプレートのインスタンス化
template class BasicPathName<char>;
template class BasicPathName<wchar_t>;

} // namespace FileIO
} // namespace Core
} // namespace LNote
