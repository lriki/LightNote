using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    class CppBuilder : CLBuilderEngine
    {
        #region Templates
        string HeaderFileTemplate = @"
#pragma once
#include <vector>
#include ""lnote.h""

#ifndef LNOTE_NEW
    #define LNOTE_NEW new
#endif

namespace LN
{
class ReferenceObject;
PRE_DECL_CLASSES

ENUMS

enum _lnInternal
{
    LN_INTERNAL_BLOCK = 0,
};

struct TypeInfo
{
	typedef ReferenceObject*(*ReferenceObjectFactory) (lnHandle handle);

	ReferenceObjectFactory Factory;

FACTORY_DECLS

    static void _RegisterTypeInfo()
    {
REGISTER_TYPE_INFO
    }   
};

/**
 *	@brief		参照カウントを持つクラスのベースクラス
 */
class ReferenceObject
{
private:
	int mRefCount;

protected:
	lnHandle mHandle;

protected:
	ReferenceObject(_lnInternal t)
        : mHandle(NULL)
		, mRefCount(1)
	{
	}

public:
	virtual ~ReferenceObject()
	{
		if (mHandle != NULL)
			LObject_Release(mHandle);
		mHandle = NULL;
	}

	/**
	 *	@brief	    オブジェクトの解放
	 *	@details    指定されたオブジェクトの参照を解放します。(参照カウントをデクリメントします)
	 */
	void Release()
	{
		mRefCount--;
		if (mRefCount <= 0) {
			delete this;
		}
	}

	/**
	 *	@brief	参照カウントのインクリメント
	 */
	void AddRef()
	{
		++mRefCount;
	}

	/**
	 *	@brief	参照カウント取得する
	 *	@return	現在の参照カウント
	 */
	int GetRefCount() const
	{
		return mRefCount;
	}

    /**
	 *	@brief	ハンドルの取得
	 */
	lnHandle GetHandle() const
	{
		return mHandle;
	}

public:
    virtual void _AttachHandle(lnHandle handle, bool addRef)
	{
        mHandle = handle;
        if (addRef)
		    LObject_AddRef(mHandle);  
	}

	static bool _IsObjectList() { return false; }
};


/**
 *	@brief		ReferenceObject 用のスマートポインタ
 */
template <class T>
class RefPtr
{
public:

	/**
	 *	@brief		コンストラクタ
	 */
	RefPtr()
		: mPtr(NULL)
	{}

	/**
	 *	@brief		コンストラクタ
	 *	@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
	 *	@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	 */
	explicit RefPtr(T* ptr, bool addRef = false)
		: mPtr(ptr)
	{
		if (mPtr && addRef) mPtr->AddRef();
	}

	/**
	 *	@brief		コピーコンストラクタ
	 *	@param[in]	obj		:
	 */
	explicit RefPtr(const RefPtr<T>& obj)
		: mPtr(obj.mPtr)
	{
		if (mPtr) mPtr->AddRef();
	}

	/**
	 *	@brief		デストラクタ
	 */
	~RefPtr()
	{
		SafeRelease();
	}

public:

	/**
	 *	@brief		ReferenceObject インスタンスのポインタを管理対象としてセットする
	 *	@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
	 *	@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	 */
	void Attach(T* ptr, bool addRef = false)
	{
		SafeRelease();
		mPtr = ptr;
		if (mPtr && addRef) mPtr->AddRef();
	}

	/**
	 *	@brief		管理対象オブジェクトの参照カウントをインクリメントする
	 */
	void SafeAddRef()
	{ 
		if (mPtr) mPtr->AddRef();
	}

	/**
	 *	@brief		管理対象オブジェクトの参照カウントをデクリメントし、管理対象から外す
	 */
	void SafeRelease()
	{ 
		if (mPtr) mPtr->Release();
		mPtr = NULL;
	}

	/**
	 *	@brief		管理対象オブジェクトへのポインタを取得する
	 */
	T* GetPtr() { return mPtr; }

	/**
	 *	@brief		管理対象オブジェクトへのポインタを取得する
	 */
	const T* GetPtr() const { return mPtr; }

public:

	/// 管理対象オブジェクトへのポインタ型への変換を実装する
	operator T* () { return mPtr; }

	/// 管理対象オブジェクトへのポインタ型への変換を実装する
	operator const T* () const { return mPtr; }

	/// operator=
	T* operator= (T* ptr)
	{
		if (ptr) ptr->AddRef();
		SafeRelease();
		mPtr = ptr;
		return mPtr;
	}

	/// operator*
	T& operator* () { return *mPtr; }

	/// operator*
	const T& operator* () const { return *mPtr; }

	/// operator->
	T* operator-> () { return mPtr; }

	/// operator->
	const T* operator-> () const { return mPtr; }

	/// operator!
	bool operator! () const { return (mPtr == NULL); }

	/// operator== 
	bool operator== (const T* ptr) const { return (mPtr == ptr); }

	/// operator!=
	bool operator!= (const T* ptr) const { return (mPtr != ptr); }

	/// operator< (for STL cmp)
	bool operator< (const T* ptr) const { return mPtr < ptr; }

public:
	void _NewAndAttachHandleAddRef(lnHandle handle)
	{
        T* obj;
        if (T::_IsObjectList()) {
            // ObjectList の場合はここでインスタンスを作る。
            // (TypeUserData に登録できるのはひとつの型のファクトリのみであるため。ObjectList を継承したクラスを作られたら手におえないが…)
            obj = LNOTE_NEW T(LN_INTERNAL_BLOCK);
            obj->_AttachHandle(handle, true);
        }
        else {
            TypeInfo* info = (TypeInfo*)LObject_GetTypeUserData(handle);
    	    obj = static_cast<T*>(info->Factory(handle));
        }
        Attach(obj, false);
	}

private:
	T* mPtr;
};

/**
 *	@brief	ReferenceObject のコレクション (動的配列)
 */
template<class T>
class ObjectList : public ReferenceObject
{
private:
	std::vector<T>	mArray;

public:
	ObjectList()
	{
	}
	virtual ~ObjectList()
	{
	}
	
public:
	
	/**
	 *	@brief	格納されているオブジェクトの数を取得する
	 *	@return	要素の数
	 */
	int GetCount() const 
	{
		int count;
		LObjectList_GetCount(mHandle, &count);
		return count;
	}
	
	/**
	 *	@brief		指定したインデックスのオブジェクトを取得する
	 *	@param[in]	index		: インデックス(要素番号)
	 *	@param[in]	item		: 設定するオブジェクト
	 */
	void SetAt(int index, T item)
	{
		_SyncItems();
		mArray[index] = item;
		LObjectList_SetAt(mHandle, index, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したインデックスのオブジェクトを取得する
	 *	@param[in]	index		: インデックス(要素番号)
	 */
	T GetAt(int index)
	{
		_SyncItems();
		return mArray[index];
	}
	
	/**
	 *	@brief		末尾にオブジェクトを追加する
	 *	@param[in]	item		: 追加するオブジェクト
	 */
	void Add(T item)
	{
		_SyncItems();
		mArray.push_back(item);
	    LObjectList_Add(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		全てのオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void Clear()
	{
		mArray.clear();
    	LObjectList_Clear(mHandle);
	}
	
	/**
	 *	@brief		オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
	 *	@param[in]	index		: item を挿入するインデックス
	 *	@param[in]	item		: 挿入するオブジェクト
	 */
	void Insert(int index, T item)
	{
		_SyncItems();
		mArray.insert(mArray.begin() + index, item);
	    LObjectList_Insert(mHandle, index, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したオブジェクトと一致する最初のオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void RemoveAll(int index, T item)
	{
		_SyncItems();
		std::vector<T>::iterator itr = mArray.begin();
		std::vector<T>::iterator end = mArray.end();
		for (; itr != end; ++itr)
		{
			if (*itr == item) {
				mArray.erase(itr);
				break;
			}
		}
	    LObjectList_Remove(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したハンドルと一致する全てのオブジェクトを削除する
	 *	@param[in]	item		: リストから削除するオブジェクト
	 */
	void RemoveAt(int index, T item)
	{
		_SyncItems();
		std::vector<T>::iterator itr = mArray.begin();
		std::vector<T>::iterator end = mArray.end();
		for (; itr != end;)
		{
			if (*itr == item)
				itr = mArray.erase(itr);
			else
				++itr;
		}
	    LObjectList_RemoveAll(mHandle, item->GetHandle());
	}
	
	/**
	 *	@brief		指定したインデックスにあるオブジェクトを削除する
	 *	@param[in]	index		: 削除するオブジェクトのインデックス番号
	 */
	void RemoveAt(int index)
	{
		_SyncItems();
		mArray.erase(mArray.begin() + index);
		LObjectList_RemoveAt(mHandle, index);
	}

private:
	void _SyncItems()
	{
		int count = GetCount();
		if (((int)mArray.size()) < count) 
		{
			mArray.resize(count);

			for (int i = 0; i < count; ++i)
			{
				lnHandle item;
				LObjectList_GetAt(mHandle, i, &item);
				if (mArray[i] == NULL || mArray[i]->GetHandle() != item)
				{
					if (mArray[i]) mArray[i]->Release();
					TypeInfo* info = (TypeInfo*)LObject_GetTypeUserData(item);
					mArray[i] = static_cast<T>(info->Factory(item));
				}
			}
		}
	}

public:
	ObjectList(_lnInternal t) : ReferenceObject(t) {}
	static bool _IsObjectList() { return true; }
	friend struct TypeInfo;
};
CLASSES

METHOD_IMPLS

FACTORY_IMPLS
}
";

        const string FactoryDeclTemplate = @"
static ReferenceObject* FUNC_NAME(lnHandle handle);
";

        const string FactoryImplTemplate = @"
inline ReferenceObject* TypeInfo::FUNC_NAME(lnHandle handle)
{
    CLASS_NAME* obj = LNOTE_NEW CLASS_NAME(LN_INTERNAL_BLOCK);
    try {
        obj->_AttachHandle(handle, true);
    }
    catch (...) {
        obj->Release();
        throw;    
    }
    return obj;
}
";
        const string RefPtrClassTemplate = @"
class LCLASSNAME : public RefPtr<CLASSNAME>
{
public:
METHODS
};
";
        #endregion

        class CurrentClassInfo
        {
            public OutputBuffer FieldsText = new OutputBuffer(1);
            public OutputBuffer MethodDeclsText = new OutputBuffer();
            public OutputBuffer MethodImplsText = new OutputBuffer();
        }

        string classesText;
        OutputBuffer _preDeclClasses = new OutputBuffer();
        OutputBuffer _enumsText = new OutputBuffer();
        OutputBuffer _methodImplsText = new OutputBuffer();
        OutputBuffer _factoryDeclsText = new OutputBuffer(1);
        OutputBuffer _factoryImplsText = new OutputBuffer();
        OutputBuffer _typeInfoRegsterText = new OutputBuffer(2);
        OutputBuffer _refPtrClassesText = new OutputBuffer();
        CurrentClassInfo _currentClassInfo;

        /// <summary>
        /// ドキュメントの置換要素を作る
        /// </summary>
        /// <param name="entity"></param>
        public override string MakeCommentConvertItem(CLEntity entity)
        {
            return null;
        }

        /// <summary>
        /// クラス・構造体出力 (前処理)
        /// </summary>
        public override bool PreClassOrStructDecl(CLClassType type)
        {
            // これらは独自定義する
            if (type == Analyzer.ReferenceObjectClass ||
                type == Analyzer.ReferenceObjectListClass)
                return false;

            _currentClassInfo = new CurrentClassInfo();
            return true;
        }

        /// <summary>
        /// クラス定義出力
        /// </summary>
        public override void MakeClassOrStructDecl(CLClassType type, string body)
        {
            var output = new OutputBuffer();

            if (type.IsStruct)
            {
                // struct は C用の構造体を継承させる
                output.AppendWithIndent("struct {0} : public ln{0}", type.Name);
                // 前置宣言に追加
                _preDeclClasses.AppendWithIndent("struct {0};", type.Name).NewLine();
            }
            else
            {
                output.AppendWithIndent("class {0}", type.Name);
                if (type.BaseType != null)
                    output.Append(" : public " + type.BaseType.Name);
                // 前置宣言に追加
                _preDeclClasses.AppendWithIndent("class {0};", type.Name).NewLine();
                if (type.IsReferenceObject)
                {
                    // internal コンストラクタ
                    _currentClassInfo.MethodDeclsText.DecreaseIndent();
                    _currentClassInfo.MethodDeclsText.AppendWithIndent("public:");
                    _currentClassInfo.MethodDeclsText.IncreaseIndent();
                    _currentClassInfo.MethodDeclsText.AppendWithIndent("{0}(_lnInternal t) : {1}(t) {{}}", type.Name, type.BaseType.Name).NewLine(2);
                    // ファクトリ
                    var factoryName = type.Name + "Factory";
                    _factoryDeclsText.AppendWithIndent(FactoryDeclTemplate.Trim()
                        .Replace("FUNC_NAME", factoryName)).NewLine();
                    _factoryImplsText.AppendWithIndent(FactoryImplTemplate.Trim()
                        .Replace("FUNC_NAME", factoryName)
                        .Replace("CLASS_NAME", type.Name)).NewLine();
                    // TypeInfo 登録
                    _typeInfoRegsterText.AppendWithIndent("static TypeInfo _{0}TypeInfo;", type.Name).NewLine();
                    _typeInfoRegsterText.AppendWithIndent("_{0}TypeInfo.Factory = {1};", type.Name, factoryName).NewLine();
                    _typeInfoRegsterText.AppendWithIndent("L{0}_SetTypeUserData(&_{0}TypeInfo);", type.Name).NewLine();

                }
            }
            output.NewLine();
            output.AppendWithIndent("{").NewLine();
            output.AppendWithIndent("public:").NewLine();
            output.IncreaseIndent();
            output.AppendWithIndent(_currentClassInfo.MethodDeclsText.ToString()).NewLine();
            output.DecreaseIndent();
            output.AppendWithIndent("private:").NewLine();
            output.IncreaseIndent();
            output.AppendWithIndent(_currentClassInfo.FieldsText.ToString()).NewLine();
            output.AppendWithIndent("friend struct TypeInfo;").NewLine();
            output.DecreaseIndent();
            output.AppendWithIndent("};").NewLine();

            classesText += output.ToString();

            // 定義
            _methodImplsText.AppendWithIndent(_currentClassInfo.MethodImplsText.ToString());

            // RefPtr 派生クラス定義
            _refPtrClassesText.AppendWithIndent(
                RefPtrClassTemplate.Trim()
                .Replace("CLASSNAME", type.Name));
        }

        /// <summary>
        /// プロパティ出力
        /// </summary>
        public override void MakeProperty(CLProperty prop)
        {
        }

        /// <summary>
        /// メソッド出力
        /// </summary>
        public override void MakeMethod(CLMethod method, CLParam returnParam)
        {
            if (method.Option.Cpp_MethodOverride)
            {
                MakeOverrideMethod(method);
                return;
            }

            var commentText = new OutputBuffer();
            commentText.AppendWithIndent("/**").NewLine();
            commentText.AppendWithIndent(" * @brief  " + method.Summary).NewLine();

            var paramsText = new OutputBuffer();
            var paramsImplText = new OutputBuffer();
            var initStmtText = new OutputBuffer();
            var argsText = new OutputBuffer();
            var postStmtText = new OutputBuffer();
            var returnStmtText = new OutputBuffer();
            string returnTypeText = (method.Attribute == MethodAttribute.Constructor) ? "" : "void";    // コンストラクタは戻り値なし
            foreach (var param in method.Params)
            {
                // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                if (param == method.Params.First() &&
                    method.IsInstanceMethod)
                {
                    if (param.IsInReferenceObjectType)
                        argsText.Append("mHandle");
                    else if (param.IsOutReferenceObjectType)
                        argsText.Append("&mHandle");
                    else
                        argsText.Append("this");  // struct instance
                }
                // return として選択された引数
                else if (param == returnParam)
                {
                    returnTypeText = ConvertParamTypeToCppForReturn(param);

                    // string 型の return
                    /*if (param.IsOutString)
                    {
                    }
                    // 参照オブジェクト型の return
                    else*/ if (param.IsOutReferenceObjectType)
                    {
                        var fieldName = "m" + method.Name;
                        // 保持するフィールド定義
                        var fieldText = string.Format("{0} {1};", ConvertParamTypeToCppForField(param), fieldName);
                        if (method.IsStaticMethod)
                            initStmtText.AppendWithIndent("static " + fieldText).NewLine();   // static の場合はメソッドのローカルに定義する (ヘッダだけで使えるようにしたい)
                        else
                            _currentClassInfo.FieldsText.AppendWithIndent(fieldText).NewLine();
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("lnHandle {0};", param.Name).NewLine();
                        // 実引数
                        argsText.AppendCommad("&{0}", param.Name);
                        // 後処理でフィールドへ格納
                        postStmtText.AppendWithIndent(@"
if ({0} == NULL) {{
    {1}.SafeRelease();
}}
else if ({1} == NULL || {1}->GetHandle() != {0}) {{
    {1}._NewAndAttachHandleAddRef({0});
}}
                        ".Trim(), param.Name, fieldName).NewLine();
                        // return
                        returnStmtText.AppendCommad("return {0};", fieldName).NewLine();

                    }
                    // struct の return
                    else if (param.IsOutStructType)
                    {
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("{0} {1};", param.Type.Name, param.Name).NewLine();
                        // 実引数
                        argsText.AppendCommad("&{0}", param.Name);
                        // return
                        returnStmtText.AppendCommad("return {0};", param.Name).NewLine();
                    }
                    // その他の型の return
                    else
                    {
                        // 一時変数初期化
                        initStmtText.AppendWithIndent("{0} {1};", CppBuilderCommon.ConvertTypeToCName(param.Type), param.Name).NewLine();
                        // 実引数
                        argsText.AppendCommad("&{0}", param.Name);
                        // return
                        if (param.Type == CLPrimitiveType.Bool)
                            returnStmtText.AppendCommad("return {0} != LN_FALSE;", param.Name);           // lnBool 変換
                        else if (param.Type is CLEnumType)
                            returnStmtText.AppendCommad("return ({0}){1};", param.Type.Name, param.Name); // enum キャスト
                        else
                            returnStmtText.AppendCommad("return {0};", param.Name);
                        returnStmtText.NewLine();
                    }
                }
                // その他の引数
                else
                {
                    // メソッドの仮引数
                    paramsImplText.AppendCommad("{0} {1}", ConvertParamTypeToCppForParam(param), param.Name);
                    paramsText.AppendCommad("{0} {1}", ConvertParamTypeToCppForParam(param), param.Name);
                    if (!string.IsNullOrEmpty(param.DefaultValueSource))
                        paramsText.Append(" = " + ConvertDefaultArgsValue(param.DefaultValueSource));   // デフォルト引数
                    // API 実引数
                    var classType = param.Type as CLClassType;
                    if (classType != null)
                    {
                        if (classType.IsStruct)
                        {
                            if (param.Attribute == ParamAttribute.In)
                                argsText.AppendCommad("&{0}", param.Name);
                            else
                                argsText.AppendCommad("{0}", param.Name);   // ポインタ型で受け取っているので & 付ける必要はない
                        }
                        else
                            argsText.AppendCommad("({0} != NULL) ? {0}->GetHandle() : NULL", param.Name);
                    }
                    else
                    {
                        if (param.Type == CLPrimitiveType.Bool)
                            argsText.AppendCommad("({0}) ? LN_TRUE : LN_FALSE", param.Name);    // lnBool 変換
                        else if (param.Type is CLEnumType)
                            argsText.AppendCommad("({0}){1}", param.Type.CName, param.Name);    // enum キャスト
                        else
                            argsText.AppendCommad("{0}", param.Name);
                    }
                    // @param
                    var attr = "";
                    if ((param.Attribute & ParamAttribute.In) != 0)
                        if ((param.Attribute & ParamAttribute.Out) != 0)
                            attr = "[in,out]";
                        else
                            attr = "[in]";
                    else
                        attr = "[out]";
                    commentText.AppendWithIndent(" * @param  {0,-8} {1,-8} : {2}", attr, param.Name, param.Summary).NewLine();
                }
            }

            // @return @details
            if (returnParam != null)
                commentText.AppendWithIndent(" * @return " + returnParam.Summary).NewLine();
            if (!string.IsNullOrEmpty(method.Detail))
            {
                commentText.AppendWithIndent(" * @details").NewLine();
                string[] lines = method.Detail.Split('\n');
                foreach (var line in lines)
                    commentText.AppendWithIndent(" *         " + line).NewLine();
            }
            commentText.AppendWithIndent(" */").NewLine();

            // 修飾子
            var modifire = "";
            if (method.IsStaticMethod)
                modifire = "static ";

            // メソッド名
            var methodName = method.Name;
            if (method.Attribute == MethodAttribute.Constructor)    // コンストラクタの場合
                methodName = method.OwnerClass.Name;

            // コンストラクタの場合はイニシャライザ
            string initializer = "";
            if (method.OwnerClass.IsReferenceObject && method.Attribute == MethodAttribute.Constructor)
                initializer = string.Format("    : {0}(LN_INTERNAL_BLOCK)" + OutputBuffer.NewLineCode, method.OwnerClass.BaseType.Name);

            // API 呼び出し
            var apiCall = string.Format("{0}({1});", method.CName, argsText.ToString());

            // メソッド宣言
            //_currentClassInfo.MethodDeclsText.AppendWithIndent(commentText.ToString());
            _currentClassInfo.MethodDeclsText.AppendWithIndent("{0}{1} {2}({3});", modifire, returnTypeText, methodName, paramsText.ToString()).NewLine(2);

            // メソッド定義
            var t = _currentClassInfo.MethodImplsText;
            t.AppendWithIndent(commentText.ToString());
            t.AppendWithIndent("inline {0} {1}::{2}({3})", returnTypeText, method.OwnerClass.Name, methodName, paramsImplText.ToString()).NewLine();
            t.AppendWithIndent(initializer);
            t.AppendWithIndent("{").NewLine();
            t.IncreaseIndent();
            t.AppendWithIndent(initStmtText.ToString());
            t.AppendWithIndent(apiCall).NewLine();
            t.AppendWithIndent(postStmtText.ToString());
            t.AppendWithIndent(returnStmtText.ToString());
            t.DecreaseIndent();
            t.AppendWithIndent("}").NewLine();

        }

        /// <summary>
        /// enum 出力 (後処理)
        /// </summary>
        public override void PostMakeEnumDecl(CLEnumType enumType)
        {
            if (enumType.Name == "Bool")
                return;

            _enumsText.AppendWithIndent("enum " + enumType.Name).NewLine();
            _enumsText.AppendWithIndent("{").NewLine();
            _enumsText.IncreaseIndent();

            foreach (var member in enumType.Members)
            {
                _enumsText.AppendWithIndent("/// " + member.Comment).NewLine();
                _enumsText.AppendWithIndent("{0}_{1},", enumType.Name, member.CapitalizedName).NewLine(2);
            }

            _enumsText.DecreaseIndent();
            _enumsText.AppendWithIndent("};").NewLine();
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        public override string MakeOutoutFileText()
        {
            return HeaderFileTemplate.Trim()
                .Replace("PRE_DECL_CLASSES", _preDeclClasses.ToString())
                .Replace("ENUMS", _enumsText.ToString())
                .Replace("FACTORY_DECLS", _factoryDeclsText.ToString())
                .Replace("REGISTER_TYPE_INFO", _typeInfoRegsterText.ToString())
                .Replace("CLASSES", classesText)
                .Replace("METHOD_IMPLS", _methodImplsText.ToString())
                .Replace("FACTORY_IMPLS", _factoryImplsText.ToString());
        }

        /// <summary>
        /// C++の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertParamTypeToCppForParam(CLParam param)
        {
            // テーブル検索
            CppBuilderCommon.CTypeInfo info;
            if (CppBuilderCommon._typeNameTable.TryGetValue(param.Type, out info))
            {
                if ((param.Attribute & ParamAttribute.Out) != 0)
                    if ((param.Attribute & ParamAttribute.In) != 0)
                    return info.CppInOutType;
                else
                    return info.CppOutType;
                else
                    return info.CppInType;
            }

            // enum 型であるか
            if (param.Type is CLEnumType)
            {
                var name = ((CLEnumType)param.Type).Name;

                if ((param.Attribute & ParamAttribute.Out) != 0)
                    return name + "*";
                else
                    return name;
            }

            // 参照オブジェクトまたは struct 型であるか
            var classType = param.Type as CLClassType;
            if (classType != null)
            {
                string name = classType.Name;

                if ((param.Attribute & ParamAttribute.Out) != 0)
                    name += "*";
                else // in
                {
                    name = "const " + name;
                    if (classType.IsStruct)
                        name += "&";
                    else
                        name += "*";
                }
                return name;
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// C++の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertParamTypeToCppForReturn(CLParam param)
        {
            // テーブル検索
            CppBuilderCommon.CTypeInfo info;
            if (CppBuilderCommon._typeNameTable.TryGetValue(param.Type, out info)) 
                return info.CppInType;

            // enum 型であるか
            if (param.Type is CLEnumType)
                return ((CLEnumType)param.Type).Name;

            // 参照オブジェクトまたは struct 型であるか
            var classType = param.Type as CLClassType;
            if (classType != null)
            {
                string name = classType.Name;
                if (classType.IsGeneric)
                    name += "<" + param.GenericTypeArg.Name + "*>";
                if (!classType.IsStruct)
                    name += "*";

                return name;
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// C++の型名への変換
        /// </summary>
        /// <param name="param"></param>
        /// <returns></returns>
        public static string ConvertParamTypeToCppForField(CLParam param)
        {
            // テーブル検索
            CppBuilderCommon.CTypeInfo info;
            if (CppBuilderCommon._typeNameTable.TryGetValue(param.Type, out info))
                return info.CppInType;

            // enum 型であるか
            if (param.Type is CLEnumType)
                return ((CLEnumType)param.Type).Name;

            // 参照オブジェクトまたは struct 型であるか
            var classType = param.Type as CLClassType;
            if (classType != null)
            {
                if (classType.IsStruct)
                    return classType.Name;
                if (classType.IsGeneric)
                    return string.Format("RefPtr< {0}<{1}*> >", classType.Name, param.GenericTypeArg.Name);
                if (classType.IsReferenceObject)
                    return string.Format("RefPtr<{0}>", classType.Name);
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// デフォルト引数値の変換
        /// </summary>
        /// <returns></returns>
        public string ConvertDefaultArgsValue(string cValue)
        {
            // double に変換できれば、数値リテラル
            double tmp;
            if (double.TryParse(cValue, out tmp))
                return cValue;

            if (cValue == "LN_TRUE")
                return "true";
            if (cValue == "LN_FALSE")
                return "false";
            if (cValue == "NULL")
                return "NULL";

            // enum メンバかもしれない
            foreach (var enumType in Analyzer.AllEnumTypes)
            {
                foreach (var m in enumType.Members)
                {
                    if (m.Name == cValue)
                        return enumType.Name + "_" + m.CapitalizedName;
                }
            }
            throw new InvalidOperationException("Not Found : " + cValue);
        }

        /// <summary>
        /// メソッドの直接定義
        /// </summary>
        public void MakeOverrideMethod(CLMethod method)
        {
            var decls = _currentClassInfo.MethodDeclsText;
            var impls = _currentClassInfo.MethodImplsText;
            if (method.CName == "LNCore_Initialize")
            {
                decls.AppendWithIndent(@"static void Initialize();");
                impls.AppendWithIndent(@"
inline void Manager::Initialize()
{
    TypeInfo::_RegisterTypeInfo();
    LManager_Initialize();
}".Trim()).NewLine();
                return;
            }
            if (method.CName == "LNVirtualPad_GetVirtualPad")
            {
                decls.AppendWithIndent(@"static VirtualPad* GetVirtualPad(int index);");
                impls.AppendWithIndent(@"
inline VirtualPad* VirtualPad::GetVirtualPad(int index)
{
    static std::vector< RefPtr<VirtualPad> > pads;
    if (pads.size() < (size_t)(index + 1))
    {
        pads.resize(index);
    }
    if (pads[index] == NULL)
    {
        lnHandle handle;
        LNVirtualPad_GetVirtualPad(index, &handle);
        pads[index]._NewAndAttachHandleAddRef(handle);
    }   
    return pads[index];
}".Trim()).NewLine();
                return;
            }

            // オーバーライド記述忘れ
            throw new InvalidOperationException("オーバーライド記述忘れ");
        }
    }
}
