using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

/*
 * Ruby のクラスは以下の要素でワンセット
 * ・フィールドを保持する struct (wrapStruct)
 * ・rb_define_class_under() で登録したクラスを保持するグローバル変数
 * ・メモリ確保関数 (alloc)
 *      alloc → rubyオブジェクトにラップ(mark、delete 登録) → 0クリア の定型文
 * ・メモリ解放・デストラクト関数 (delete)
 *      RefObj なら Release → free
 * ・GCマーク関数 (mark)
 *      wrapStruct の VALUE 型を mark
 * ×初期化関数 (initialize)
 *      コンストラクタ扱い。必須ではない
 *      
 * ■制限事項
 *      LSprite_Create() 等、コンストラクタの引数でプロパティに格納するべき値を受け取っても格納しない。
 *      そのままだと LSprite_GetTexture() で取得するタイミングで新しい Texture クラスが作られることになる。
 *      とりあえず、
 *      ・get を使う機会自体ほとんど無い
 *      ・インスタンス生成による new は初回の GetTexuter() 呼び出し時のみ
 *      ということで、現状維持とする。
 *      ただし、== と eql? はオーバーライドしておいて、Handle が一致すれば同一オブジェクトとする。
 *      → というか、他の言語も含めて全体的にこんな感じの仕様でいい気がする。
 *          C言語側内部で Handle が変わった時でも、常に最新にできる。
 */

namespace BinderMaker
{
    class RubyBuilder : CLBuilderEngine
    {
        #region Templates
        private static string SourceTemplate = @"
#include ""ruby.h""
#include <stdio.h>
#include <vector>
#include <lnote.h>
#include ""lnote_ruby.h""

typedef VALUE (*RubyObjectFactory) (VALUE klass, lnHandle)  ;

struct TypeUserData
{
    RubyObjectFactory  Factory;
    VALUE*             Klass;
};

struct wrapReferenceObject
{
    lnHandle    Handle;
};
struct wrapObjectList
    : public wrapReferenceObject
{
    std::vector<VALUE> RefObjects;
};

__WRAPPER_STRUCTS__

VALUE gLNoteModule;
VALUE gLNoteError;  // exception
VALUE gReferenceObjectClass;
VALUE gObjectListClass;
__MODULE_GLOBALS__

//-----------------------------------------------------------------------------
// Utils
static lnHandle RbRefObjToHandle(VALUE v)
{
    if (v == Qnil) {
        return NULL;    
    }
	wrapReferenceObject* obj;
	Data_Get_Struct(v, wrapReferenceObject, obj);
	return obj->Handle;
}

bool checkEqualHandle(VALUE obj, lnHandle handle)
{
	if (obj == Qnil)
		return false;
	if (((wrapReferenceObject*)DATA_PTR(obj))->Handle != handle)
		return false;
	return true;
}

//-----------------------------------------------------------------------------
// LReferenceObject
static void LReferenceObject_delete(wrapSize* obj) { free(obj); }
static void LReferenceObject_mark(wrapSize* obj) {}
static VALUE LReferenceObject_allocate( VALUE klass )
{
    VALUE obj;
    wrapReferenceObject* internalObj;

    internalObj = (wrapReferenceObject*)malloc(sizeof(wrapSize));
    if (internalObj == NULL) rb_raise( gLNoteError, ""Faild alloc - LReferenceObject_allocate"" );
    obj = Data_Wrap_Struct(klass, LReferenceObject_mark, LReferenceObject_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapReferenceObject));
    return obj;
}
static VALUE LReferenceObject_initialize( int argc, VALUE *argv, VALUE self ) {}

//-----------------------------------------------------------------------------
// LObjectList
static void LObjectList_delete(wrapObjectList* obj)
{
    free(obj);
}
static void LObjectList_mark(wrapObjectList* obj) 
{
	std::vector<VALUE>::iterator itr = obj->RefObjects.begin();
	std::vector<VALUE>::iterator end = obj->RefObjects.end();
	for (; itr != end; ++itr) {
		rb_gc_mark(*itr);
	}
}
static VALUE LObjectList_allocate( VALUE klass )
{
    VALUE obj;
    wrapObjectList* internalObj;

    internalObj = (wrapObjectList*)malloc(sizeof(wrapObjectList));
    if (internalObj == NULL) rb_raise( gLNoteError, ""Faild alloc - LObjectList_allocate"" );
    obj = Data_Wrap_Struct(klass, LObjectList_mark, LObjectList_delete, internalObj);
    
    internalObj->Handle = NULL;
    //internalObj->Factory = NULL;
    return obj;
}
static VALUE LObjectList_allocate2( VALUE klass, lnHandle handle/*, VALUE* klassForFactory, RubyObjectFactory factory */)
{
    VALUE obj = LObjectList_allocate(klass);
    ((wrapObjectList*)DATA_PTR(obj))->Handle = handle;
    //((wrapObjectList*)DATA_PTR(obj))->Factory = factory;
    //((wrapObjectList*)DATA_PTR(obj))->Klass = klassForFactory;
    return obj;
}
static VALUE LObjectList_get_at(VALUE self, VALUE index) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);

//    int count;
//    LObjectList_GetCount(selfWrap->Handle, &count);
//
//    // サイズ同期
//    if (selfWrap->RefObjects.size() < count) {
//    	selfWrap->RefObjects.resize(count, Qnil);
//    }
//
//    // Handle 取得
//    int idx = INT2FIX(index);
//    lnHandle handle;
//    LObjectList_GetAt(selfWrap->Handle, idx, &handle);
//
//    // オブジェクト作成
//    if (!checkEqualHandle(selfWrap->RefObjects[idx], handle)) {
//        TypeUserData* info = (TypeUserData*)LObject_GetTypeUserData(handle);
//    	selfWrap->RefObjects[idx] = info->Factory(*(info->Klass), handle);//selfWrap->Factory(*(selfWrap->Klass), handle);
//    }
    return selfWrap->RefObjects[idx];
}
static VALUE LObjectList_set_at(VALUE self, VALUE index, VALUE item) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    int idx = INT2FIX(index);
	selfWrap->RefObjects[idx] = item;
    LObjectList_SetAt(RbRefObjToHandle(self), idx, RbRefObjToHandle(item));
    return self;
}
static VALUE LObjectList_get_count(VALUE self) 
{
    int count;
    LObjectList_GetCount(RbRefObjToHandle(self), &count);
	return INT2FIX(count);
}
static VALUE LObjectList_add(VALUE self, VALUE item) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	selfWrap->RefObjects.push_back(item);
    LObjectList_Add(RbRefObjToHandle(self), RbRefObjToHandle(item));
	return self;
}
static VALUE LObjectList_clear(VALUE self) 
{
    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	selfWrap->RefObjects.clear();
    LObjectList_Clear(RbRefObjToHandle(self));
	return self;
}
static VALUE LObjectList_insert(VALUE self, VALUE index, VALUE item) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
    int idx = INT2FIX(index);
	selfWrap->RefObjects.insert(selfWrap->RefObjects.begin() + idx, item);
    LObjectList_Insert(RbRefObjToHandle(self), idx, RbRefObjToHandle(item));
	return self;
}
static VALUE LObjectList_remove(VALUE self, VALUE item) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	std::vector<VALUE>::iterator itr = selfWrap->RefObjects.begin();
	std::vector<VALUE>::iterator end = selfWrap->RefObjects.end();
	for (; itr != end; ++itr)
	{
		if (*itr == item) {
			selfWrap->RefObjects.erase(itr);
			break;
		}
	}
    LObjectList_Remove(RbRefObjToHandle(self), RbRefObjToHandle(item));
	return self;
}
static VALUE LObjectList_remove_all(VALUE self, VALUE item) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	std::vector<VALUE>::iterator itr = selfWrap->RefObjects.begin();
	std::vector<VALUE>::iterator end = selfWrap->RefObjects.end();
	for (; itr != end;)
	{
		if (*itr == item) {
			itr = selfWrap->RefObjects.erase(itr);
		}
		else {
			++itr;
		}
	}
    LObjectList_RemoveAll(RbRefObjToHandle(self), RbRefObjToHandle(item));
	return self;
}
static VALUE LObjectList_remove_at(VALUE self, VALUE index) 
{
    LObjectList_SyncItems();

    wrapObjectList* selfWrap;
    Data_Get_Struct(self, wrapObjectList, selfWrap);
	int idx = INT2FIX(index);
	selfWrap->RefObjects.erase(selfWrap->RefObjects.begin() + idx);
    LObjectList_RemoveAt(RbRefObjToHandle(self), idx);
	return self;
}
static void LObjectList_SyncItems(wrapObjectList* objList) 
{
    std::vector<VALUE>& list = selfWrap->RefObjects;
    int count;
    LObjectList_GetCount(objList->Handle, &count);

	if (((int)list.size()) < count) 
	{
		list.resize(count);

		for (int i = 0; i < count; ++i)
		{
			lnHandle item;
			LObjectList_GetAt(objList->Handle, i, &item);
			if (list[i] == Qnil || (!checkEqualHandle(list[i], item))
			{
				TypeUserData* info = (TypeUserData*)LObject_GetTypeUserData(item);
				list[i] = info->Factory(*(info->Klass), item);
			}
		}
	}
}

//-----------------------------------------------------------------------------

__FUNC_DEFINES__


void LNote_Terminate(VALUE self)
{
	LManager_Terminate();
}

extern ""C"" void Init_lnote()
{
    // ルートモジュール
    gLNoteModule = rb_define_module(""LN"");

    // 例外定義
    gLNoteError = rb_define_class_under( gLNoteModule, ""LNoteError"", rb_eRuntimeError );
    
    // 参照オブジェクトクラス
    gReferenceObjectClass = rb_define_class_under(gLNoteModule, ""ReferenceObject"", rb_cObject);
    rb_define_alloc_func(gReferenceObjectClass, LReferenceObject_allocate);
    rb_define_private_method(gReferenceObjectClass, ""initialize"", LN_TO_RUBY_FUNC(LReferenceObject_initialize), -1);

    // 参照オブジェクトリストクラス
    gObjectListClass = rb_define_class_under(gLNoteModule, ""ReferenceObjectList"", gReferenceObjectClass);
	rb_define_method(gObjectListClass, ""[]"", LN_TO_RUBY_FUNC(LObjectList_get_at), 1);
	rb_define_method(gObjectListClass, ""[]="", LN_TO_RUBY_FUNC(LObjectList_set_at), 2);
	rb_define_method(gObjectListClass, ""count"", LN_TO_RUBY_FUNC(LObjectList_get_count), 0);
	rb_define_method(gObjectListClass, ""size"", LN_TO_RUBY_FUNC(LObjectList_get_count), 0);
	rb_define_method(gObjectListClass, ""add"", LN_TO_RUBY_FUNC(LObjectList_add), 1);
	rb_define_method(gObjectListClass, ""push"", LN_TO_RUBY_FUNC(LObjectList_add), 1);
	rb_define_method(gObjectListClass, ""clear"", LN_TO_RUBY_FUNC(LObjectList_clear), 0);
	rb_define_method(gObjectListClass, ""insert"", LN_TO_RUBY_FUNC(LObjectList_insert), 2);
	rb_define_method(gObjectListClass, ""remove"", LN_TO_RUBY_FUNC(LObjectList_remove), 1);
	rb_define_method(gObjectListClass, ""remove_all"", LN_TO_RUBY_FUNC(LObjectList_remove_all), 1);
	rb_define_method(gObjectListClass, ""remove_at"", LN_TO_RUBY_FUNC(LObjectList_remove_at), 1);

__TYPE_DEFINES__

__CONST_DEFINES__

    // 終了時に実行する関数
    rb_set_end_proc( LNote_Terminate, Qnil );

    // using namespace LNote;
	//rb_include_module( rb_cObject, gLNoteModule );
}
";
        private static string FuncDeclTemplate = @"
static VALUE __FUNC_NAME__(int argc, VALUE *argv, VALUE self)
{
__CONTENTS__
}
";
        private static string AllocFuncDeclTemplate = @"
static VALUE __MODULE_NAME___allocate( VALUE klass )
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( gLNoteModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, __MODULE_NAME___mark, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));
__INIT__
    return obj;
}
";
        private static string Alloc2FuncDeclTemplate = @"
static VALUE __MODULE_NAME___allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    __STRUCT_NAME__* internalObj;

    internalObj = (__STRUCT_NAME__*)malloc(sizeof(__STRUCT_NAME__));
    if (internalObj == NULL) rb_raise( gLNoteModule, ""Faild alloc - __MODULE_NAME___allocate"" );
    obj = Data_Wrap_Struct(klass, __MODULE_NAME___mark, __MODULE_NAME___delete, internalObj);
    
    memset(internalObj, 0, sizeof(__STRUCT_NAME__));
__INIT__
    internalObj->Handle = handle;
    return obj;
}
";
        private static string DeleteFuncDeclTemplate = @"
static void __MODULE_NAME___delete(__STRUCT_NAME__* obj)
{
    __CONTENTS__
    free(obj);
}
";
        private static string MarkFuncDeclTemplate = @"
static void __MODULE_NAME___mark(__STRUCT_NAME__* obj)
{
__CONTENTS__
}
";
        private static string InitializeFuncDeclTemplate = @"
static VALUE __MODULE_NAME___initialize( int argc, VALUE *argv, VALUE self )
{
    __CONTENTS__
}
";
        private static string StructInitializeFuncDeclTemplate = @"
static VALUE __MODULE_NAME___struct_initialize( int argc, VALUE *argv, VALUE self )
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    if (argc == 0) {
        return self;
    }
__CONTENTS__
}
";
//        private static string StructDefaultInitializeFuncDeclTemplate = @"
//static VALUE __MODULE_NAME___struct_defaultInitialize( VALUE self )
//{
//    return self;
//}
//";
        private static string StructMemberSetFuncDeclTemplate = @"
static VALUE __MODULE_NAME_____MEMBER___set( VALUE self, VALUE v )
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    selfObj->Value.__MEMBER__ = NUM2DBL(v);
    return self;
}
";
        private static string StructMemberGetFuncDeclTemplate = @"
static VALUE __MODULE_NAME_____MEMBER___get( VALUE self)
{
    __STRUCT_NAME__* selfObj;
    Data_Get_Struct(self, __STRUCT_NAME__, selfObj);
    return toVALUE(selfObj->Value.__MEMBER__);
}
";
        #endregion
        
        class OverloadMethodInfo
        {
            public CLMethod Method;
            public CLParam ReturnParam;
        }

        class CurrentClassInfo
        {
            /// <summary>
            /// オーバーロードメソッドの集計 (コンストラクタは Constructor という名前で登録する)
            /// </summary>
            public Dictionary<string, List<OverloadMethodInfo>> OverloadTable = new Dictionary<string, List<OverloadMethodInfo>>();

            /// <summary>
            /// 追加のラップ構造体メンバ
            /// </summary>
            public OutputBuffer AdditionalWrapStructMember = new OutputBuffer();

            /// <summary>
            /// 追加のラップ構造体メンバの初期化式
            /// </summary>
            public OutputBuffer AdditionalWrapStructMemberInit = new OutputBuffer();

            /// <summary>
            /// 追加のラップ構造体メンバのマーク
            /// </summary>
            public OutputBuffer AdditionalWrapStructMemberMark = new OutputBuffer();

            public CurrentClassInfo()
            {
                AdditionalWrapStructMember.IncreaseIndent();
                AdditionalWrapStructMemberInit.IncreaseIndent();
                AdditionalWrapStructMemberMark.IncreaseIndent();
            }
        }
        
        string _allTypeDefineGlobalVals;
        OutputBuffer _allWrapperStructText = new OutputBuffer();
        OutputBuffer _allRBDefinesText = new OutputBuffer();        // LNote_init で行われる rb_... の登録処理
        OutputBuffer _allFuncDeclText = new OutputBuffer();
        OutputBuffer _allRBConstsText = new OutputBuffer(1);

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
            _allRBDefinesText.IncreaseIndent();

            // これらは独自定義する
            if (CheckAutoGenerateClass(type))
            {
                // ReferenceObject の wrap struct は独自定義する
            }
            else
            {
                //-------------------------------------------------
                // Init で行う登録とグローバル変数
                var typeValName = string.Format(@"g{0}Class", type.Name);
                if (type.Attribute == CLModuleAttr.StaticClass)
                {
                    // rb_define_module_under
                    string t = string.Format(@"{0} = rb_define_module_under(gLNoteModule, ""{1}"");", typeValName, type.Name);
                    _allRBDefinesText.AppendWithIndent(t).NewLine();
                }
                else
                {
                    string baseClass = "rb_cObject";
                    if (type.BaseType != null) // 親クラス
                        baseClass = string.Format("g{0}Class", type.BaseType.Name);
                    string t = string.Format(@"{0} = rb_define_class_under(gLNoteModule, ""{1}"", {2});", typeValName, type.Name, baseClass);
                    _allRBDefinesText.AppendWithIndent(t).NewLine();

                    // alloc 関数
                    _allRBDefinesText.AppendWithIndent(string.Format("rb_define_alloc_func({0}, {1}_allocate);", typeValName, type.CName)).NewLine();

                    // initialize メソッド
                    //_allRBDefinesText.AppendWithIndent(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({1}_initialize), -1);", typeValName, type.CName).NewLine(2);

                }
                _allTypeDefineGlobalVals += string.Format("VALUE {0};\n", typeValName);
            }


            _allRBDefinesText.DecreaseIndent();

            // リセット
            _currentClassInfo = new CurrentClassInfo();
            return !CheckAutoGenerateClass(type);
        }

        /// <summary>
        /// クラス・構造体出力 (後処理)
        /// </summary>
        public override void MakeClassOrStructDecl(CLClassType type, string body)
        {
            if (CheckAutoGenerateClass(type))
                return;

            //-------------------------------------------------
            // モジュールをラップする構造体
            _allWrapperStructText.AppendWithIndent("struct {0}", GetWrapStructName(type)).NewLine();
            if (type.BaseType != null)  // 継承
                _allWrapperStructText.Append("    : public " + GetWrapStructName((CLClassType)type.BaseType)).NewLine();
            _allWrapperStructText.AppendWithIndent("{").NewLine();
            // 構造体の場合はメンバを作る
            if (type.IsStruct)
            {
                _allWrapperStructText.IncreaseIndent();
                _allWrapperStructText.AppendWithIndent("ln{0} Value;", type.Name).NewLine();
                _allWrapperStructText.DecreaseIndent();
            }
            // プロパティフィールド
            _allWrapperStructText.AppendWithIndent(_currentClassInfo.AdditionalWrapStructMember.ToString()).NewLine();
            _allWrapperStructText.AppendWithIndent("};").NewLine();

            //-------------------------------------------------
            // フレームワーク関数
            {
                var typeValName = string.Format(@"g{0}Class", type.Name);
                // delete
                string deleteFuncContents = "";
                if (type.IsReferenceObject)   // Object であれば Release する
                    deleteFuncContents = "if (obj->Handle != 0) LObject_Release(obj->Handle);";
                string t = DeleteFuncDeclTemplate.Trim();
                t = t.Replace("__MODULE_NAME__", type.CName);
                t = t.Replace("__STRUCT_NAME__", GetWrapStructName(type));
                t = t.Replace("__CONTENTS__", deleteFuncContents);
                _allFuncDeclText.AppendWithIndent(t).NewLine(2);

                // mark
                t = MarkFuncDeclTemplate.Trim();
                t = t.Replace("__MODULE_NAME__", type.CName);
                t = t.Replace("__STRUCT_NAME__", GetWrapStructName(type));
                t = t.Replace("__CONTENTS__",  _currentClassInfo.AdditionalWrapStructMemberMark.ToString());
                _allFuncDeclText.AppendWithIndent(t).NewLine(2);

                // allocate
                t = AllocFuncDeclTemplate.Trim();
                t = t.Replace("__MODULE_NAME__", type.CName);
                t = t.Replace("__STRUCT_NAME__", GetWrapStructName(type));
                t = t.Replace("__INIT__", _currentClassInfo.AdditionalWrapStructMemberInit.ToString());
                _allFuncDeclText.AppendWithIndent(t).NewLine(2);

                // initialize
                if (type.IsStruct)
                {
                    #region 構造体の場合はコンストラクタ・アクセサを定義する
                    var wrapStructName = GetWrapStructName(type);

                    var initStmtText = new OutputBuffer();
                    var scan_argsText = new OutputBuffer();
                    var setStmtText = new OutputBuffer();
                    foreach (var memberDecl in type.StructMembers)
                    {
                        initStmtText.AppendWithIndent("VALUE {0}_;", memberDecl.Name).NewLine();
                        scan_argsText.AppendCommad("&{0}_", memberDecl.Name);
                        setStmtText.AppendWithIndent("selfObj->Value.{0} = NUM2DBL({0}_);", memberDecl.Name).NewLine();
                    }
                    var contentsText = new OutputBuffer();
                    contentsText.IncreaseIndent();
                    contentsText.AppendWithIndent(initStmtText.ToString());
                    contentsText.AppendWithIndent(@"rb_scan_args(argc, argv, ""{0}"", {1});", type.StructMembers.Count().ToString(), scan_argsText.ToString()).NewLine();
                    contentsText.AppendWithIndent(setStmtText.ToString());
                    contentsText.AppendWithIndent("return self;");

                    t = StructInitializeFuncDeclTemplate.Trim()
                        .Replace("__MODULE_NAME__", type.CName)
                        .Replace("__STRUCT_NAME__", wrapStructName)
                        .Replace("__CONTENTS__", contentsText.ToString());
                    _allFuncDeclText.AppendWithIndent(t).NewLine(2);

                    t = string.Format(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({1}_struct_initialize), -1);", typeValName, type.CName);
                    _allRBDefinesText.AppendWithIndent(t).NewLine();

                    //// デフォルトコンストラクタ
                    //t = StructDefaultInitializeFuncDeclTemplate.Trim()
                    //   .Replace("__MODULE_NAME__", type.CName)
                    //   .Replace("__STRUCT_NAME__", wrapStructName);
                    //_allFuncDeclText.AppendWithIndent(t).NewLine(2);
                    //t = string.Format(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({1}_struct_defaultInitialize), 0);", typeValName, type.CName, type.StructMembers.Count());
                    //_allRBDefinesText.AppendWithIndent(t).NewLine();

                    // アクセサ
                    foreach (var memberDecl in type.StructMembers)
                    {
                        // set
                        t = StructMemberSetFuncDeclTemplate.Trim()
                            .Replace("__MODULE_NAME__", type.CName)
                            .Replace("__STRUCT_NAME__", wrapStructName)
                            .Replace("__MEMBER__", memberDecl.Name);
                        _allFuncDeclText.AppendWithIndent(t).NewLine(2);
                        t = string.Format(@"rb_define_method({0}, ""{1}="", LN_TO_RUBY_FUNC({2}_{3}_set), 1);", typeValName, RubyCommon.GetSnakeStyleName(memberDecl.Name), type.CName, memberDecl.Name);
                        _allRBDefinesText.AppendWithIndent(t).NewLine();

                        // get
                        t = StructMemberGetFuncDeclTemplate.Trim()
                            .Replace("__MODULE_NAME__", type.CName)
                            .Replace("__STRUCT_NAME__", wrapStructName)
                            .Replace("__MEMBER__", memberDecl.Name);
                        _allFuncDeclText.AppendWithIndent(t).NewLine(2);
                        t = string.Format(@"rb_define_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}_{3}_get), 0);", typeValName, RubyCommon.GetSnakeStyleName(memberDecl.Name), type.CName, memberDecl.Name);
                        _allRBDefinesText.AppendWithIndent(t).NewLine();
                    }
                    #endregion
                }
                else if (type.IsReferenceObject)
                {
                    // allocate2
                    t = Alloc2FuncDeclTemplate.Trim();
                    t = t.Replace("__MODULE_NAME__", type.CName);
                    t = t.Replace("__STRUCT_NAME__", GetWrapStructName(type));
                    t = t.Replace("__INIT__", _currentClassInfo.AdditionalWrapStructMemberInit.ToString());
                    _allFuncDeclText.AppendWithIndent(t).NewLine(2);

                    // TypeInfo を作る
                    _allRBDefinesText.IncreaseIndent();
                    var typeInfoGlobalVarName = string.Format(@"g{0}ClassTypeInfo", type.Name);
                    _allRBDefinesText.AppendWithIndent("{0}.Factory = {1}_allocate2;", typeInfoGlobalVarName, type.CName).NewLine();
                    _allRBDefinesText.AppendWithIndent("{0}.Klass = &{1};", typeInfoGlobalVarName, typeValName).NewLine();
                    // グローバル変数定義
                    _allTypeDefineGlobalVals += string.Format("TypeUserData {0};\n", typeInfoGlobalVarName);
                    // TypeUserData に仕込む
                    _allRBDefinesText.AppendWithIndent("{0}_SetTypeUserData((void*)(&{1}));", type.CName, typeInfoGlobalVarName).NewLine();
                    _allRBDefinesText.DecreaseIndent();
                }
            }

            //-------------------------------------------------
            // メソッド出力
            foreach (var overloads in _currentClassInfo.OverloadTable)
            {
                MakeFuncDecl(overloads.Value);
            }

            _allRBDefinesText.NewLine();

        }

        /// <summary>
        /// プロパティ出力
        /// </summary>
        public override void MakeProperty(CLProperty prop)
        {
            // RefObj型のプロパティは、フィールドに保持したい
            if (prop.Type.Type is CLClassType &&
                ((CLClassType)prop.Type.Type).IsReferenceObject)
            {
                _currentClassInfo.AdditionalWrapStructMember.AppendWithIndent("VALUE {0};", prop.Name).NewLine();
                _currentClassInfo.AdditionalWrapStructMemberInit.AppendWithIndent("internalObj->{0} = Qnil;", prop.Name).NewLine();
                _currentClassInfo.AdditionalWrapStructMemberMark.AppendWithIndent("rb_gc_mark(obj->{0});", prop.Name).NewLine();
            }
        }

        /// <summary>
        /// メソッド出力
        /// </summary>
        public override void MakeMethod(CLMethod method, CLParam returnParam)
        {
            if (!method.Option.Ruby_Enabled)
                return;

            // これらは独自定義する
            if (method.OwnerClass.Name == "ReferenceObject" ||
                method.OwnerClass.Name == "ObjectList" ||
                method.OwnerClass.Name == "Exception")
            {
                return;
            }

            // Ruby として出力できるメソッドであるか
            if (!RubyCommon.CheckInvalidMethod(method))
                return;


            // オーバーロードの集計
            List<OverloadMethodInfo> overloads;
            string key = (method.Attribute == MethodAttribute.Constructor) ? "Constructor" : method.Name;
            if (_currentClassInfo.OverloadTable.TryGetValue(key, out overloads))
            {
                var info = new OverloadMethodInfo();
                info.Method = method;
                info.ReturnParam = returnParam;
                _currentClassInfo.OverloadTable[key].Add(info);
            }
            else
            {
                // 新しく登録
                var info = new OverloadMethodInfo();
                info.Method = method;
                info.ReturnParam = returnParam;
                _currentClassInfo.OverloadTable[key] = new List<OverloadMethodInfo>() { info };
            }
        }

        /// <summary>
        /// Enum 出力
        /// </summary>
        /// <param name="enumType"></param>
        public override void PostMakeEnumDecl(CLEnumType enumType)
        {
            // 出力しないもの
            if (enumType.Name == "Bool")
                return;

            // Module 用グローバル変数
            var varName = "gEnum_" + enumType.Name;
            _allTypeDefineGlobalVals += string.Format("VALUE {0};", varName) + OutputBuffer.NewLineCode;

            // Module 定義
            _allRBConstsText.AppendWithIndent(@"{0} = rb_define_module_under(gLNoteModule, ""{1}"");", varName, enumType.Name).NewLine();

            // const 定義
            foreach (var member in enumType.Members)
            {
                if (member.IsTerminator) continue;  // ターミネータは出力しない

                string name = member.CommonSnakeName.ToUpper();
                _allRBConstsText.AppendWithIndent(@"rb_define_const({0}, ""{1}"", INT2FIX({2}));", varName, name, member.Value).NewLine();
            }

            _allRBConstsText.NewLine();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override string MakeOutoutFileText()
        {
            string tmp = SourceTemplate.Trim();
            tmp = tmp.Replace("__WRAPPER_STRUCTS__", _allWrapperStructText.ToString());
            tmp = tmp.Replace("__MODULE_GLOBALS__", _allTypeDefineGlobalVals);
            tmp = tmp.Replace("__FUNC_DEFINES__", _allFuncDeclText.ToString());
            tmp = tmp.Replace("__TYPE_DEFINES__", _allRBDefinesText.ToString());
            tmp = tmp.Replace("__CONST_DEFINES__", _allRBConstsText.ToString());
            return tmp;
        }

        /// <summary>
        /// オーバーロードを考慮しつつ、関数定義を作成する
        /// </summary>
        private void MakeFuncDecl(List<OverloadMethodInfo> overloadMethods)
        {
            var baseMethod = overloadMethods[0].Method;
            if (baseMethod.Option.CS_MethodOverride)
            {
                MakeOverrideMethod(baseMethod);
                return;
            }

            // メソッドを持つクラスを表現するグローバル変数
            string typeValName = string.Format(@"g{0}Class", baseMethod.OwnerClass.Name);

            // 関数名
            string funcName = "lnrb" + baseMethod.CName;
            var funcBody = new OutputBuffer();
            funcBody.IncreaseIndent();

            // wrapStruct
            if (baseMethod.IsInstanceMethod)
            {
                funcBody.AppendWithIndent("{0}* selfObj;", GetWrapStructName(baseMethod.OwnerClass)).NewLine();
                funcBody.AppendWithIndent("Data_Get_Struct(self, {0}, selfObj);", GetWrapStructName(baseMethod.OwnerClass)).NewLine(2);
            }

            foreach (var info in overloadMethods)
            {
                var callBody = new OutputBuffer();

                int normalArgsCount = 0;
                int defaultArgsCount = 0;
                var scan_args_Inits = new OutputBuffer();
                var scan_args_Args = new OutputBuffer();
                string typeCheckExp = "";
                var initStmt = new OutputBuffer();
                var argsText = new OutputBuffer();
                var postStmt = new OutputBuffer();
                var returnStmt = new OutputBuffer();

                foreach (var param in info.Method.Params)
                {
                    // 第1引数かつインスタンスメソッドの場合は特殊な実引数になる
                    if (param == info.Method.Params.First() &&
                        info.Method.IsInstanceMethod)
                    {
                        if (param.IsInReferenceObjectType)
                            argsText.Append("selfObj->Handle");
                        else if (param.IsOutReferenceObjectType)
                            argsText.Append("&selfObj->Handle");
                        else
                            argsText.Append("&selfObj->Value");  // struct instance
                    }
                    // return として選択されている引数である場合
                    else if (param == info.ReturnParam)
                    {
                        var varName = "_" + param.Name;
                        // 宣言
                        initStmt.AppendWithIndent("{0} {1};", CppBuilderCommon.ConvertTypeToCName(param.Type), varName).NewLine();
                        // API実引数
                        argsText.AppendCommad("&" + varName);
                        // デフォルトオブジェクト return
                        if (info.Method.Attribute == MethodAttribute.DefaultInstanceGetter)
                        {
                            var globalVarName = info.Method.CName + "_default";
                            var handleAccess = string.Format("((wrapReferenceObject*)DATA_PTR({0}))->Handle", globalVarName);
                            var memberClassName = param.Type.CName;
                            var memberTypeVarName = string.Format(@"g{0}Class", param.Type.Name);

                            returnStmt.AppendWithIndent("if ({0} == NULL) {{", varName).NewLine();
                            returnStmt.IncreaseIndent();
                            returnStmt.AppendWithIndent("{0} = Qnil;", globalVarName).NewLine();
                            returnStmt.DecreaseIndent();
                            returnStmt.AppendWithIndent("}").NewLine();
                            returnStmt.AppendWithIndent("else if ({0} == Qnil || {1} != {2}) {{", globalVarName, handleAccess, varName).NewLine();
                            returnStmt.IncreaseIndent();
                            returnStmt.AppendWithIndent("{0} = {1}_allocate2({2}, {3});", globalVarName, memberClassName, memberTypeVarName, varName).NewLine();
                            returnStmt.DecreaseIndent();
                            returnStmt.AppendWithIndent("}").NewLine();
                            returnStmt.AppendWithIndent("return {0};", globalVarName).NewLine();

                            _allTypeDefineGlobalVals += string.Format("VALUE {0} = Qnil;\n", globalVarName);
                        }
                        // プロパティで参照オブジェクト型の return
                        else if (param.IsOutReferenceObjectType &&
                            info.Method.OwnerProperty != null)
                        {
                            var propName = info.Method.OwnerProperty.Name;
                            var memberClassName = param.Type.CName;
                            var memberTypeVarName = string.Format(@"g{0}Class", param.Type.Name);
                            var handleAccess = string.Format("(({0}*)DATA_PTR(selfObj->{1}))->Handle", GetWrapStructName((CLClassType)param.Type), propName);
                            var genericAllocArgs = "";
                            //if (param.Type is CLClassType && ((CLClassType)param.Type).IsGeneric)
                            //    genericAllocArgs = string.Format(", &g{0}Class, {1}_allocate", param.Type.Name, param.GenericTypeArg.CName);

                            returnStmt.AppendWithIndent("if ({0} == NULL) {{", varName).NewLine();
                            returnStmt.IncreaseIndent();
                            returnStmt.AppendWithIndent("selfObj->{0} = Qnil;", propName).NewLine();
                            returnStmt.DecreaseIndent();
                            returnStmt.AppendWithIndent("}").NewLine();
                            returnStmt.AppendWithIndent("else if (selfObj->{0} == Qnil || {1} != {2}) {{", propName, handleAccess, varName).NewLine();
                            returnStmt.IncreaseIndent();
                            returnStmt.AppendWithIndent("selfObj->{0} = {1}_allocate2({2}, {3} {4});", propName, memberClassName, memberTypeVarName, varName, genericAllocArgs).NewLine();
                            //returnStmt.AppendWithIndent("{0} = {1};", handleAccess, varName).NewLine();
                            returnStmt.DecreaseIndent();
                            returnStmt.AppendWithIndent("}").NewLine();
                            returnStmt.AppendWithIndent("return selfObj->{0};", propName).NewLine();
                            /*
                             * if (_objectList == NULL) {
        		                    selfObj->Layers = Qnil;
        	                    }
        	                    else if (selfObj->Layers == Qnil || ((wrapReferenceObjectList*)DATA_PTR(selfObj->Layers))->Handle != _objectList) {
        		                    selfObj->Layers = LReferenceObjectList_allocate(gReferenceObjectList);
        		                    ((wrapReferenceObjectList*)DATA_PTR(selfObj->Layers))->Handle = _objectList;
        	                    }
        	                    return self;*/

                        }
                        // その他の型の return
                        else
                        {
                            // return
                            MakeCastExpCToVALUE(param.Type, varName, returnStmt);
                        }
                    }
                    // return として選択されていない引数である場合
                    else
                    {
                        // 通常引数とデフォルト引数のカウント
                        if (string.IsNullOrEmpty(param.DefaultValueSource))
                            normalArgsCount++;
                        else
                            defaultArgsCount++;

                        // out の場合は C++ 型で受け取るための変数を定義
                        if (param.Attribute == ParamAttribute.Out)
                        {
                            // ruby は複数 out を扱えないためここに来ることはないはず
                            throw new InvalidOperationException();
                        }
                        // 入力引数 (in とinout)
                        else
                        {
                            // rb_scan_args の格納先 VALUE 宣言
                            scan_args_Inits.AppendWithIndent("VALUE {0};", param.Name).NewLine();
                            // rb_scan_args の引数
                            scan_args_Args.AppendCommad("&{0}", param.Name);
                            // 型チェック条件式
                            if (!string.IsNullOrEmpty(typeCheckExp))
                                typeCheckExp += " && ";
                            typeCheckExp += GetTypeCheckExp(param.Type, param.Name);
                            // C変数宣言 & 初期化代入
                            initStmt.AppendWithIndent(GetCastExpVALUEtoC(param.Type, "_" + param.Name, param.Name, param.DefaultValueSource)).NewLine();
                            // API実引数
                            if (param.Type is CLClassType &&
                                ((CLClassType)param.Type).IsStruct)
                                argsText.AppendCommad("&_" + param.Name);   // struct は 参照渡し
                            else
                                argsText.AppendCommad("_" + param.Name);
                            // RefObj 型のプロパティの setter ならメンバに保存する
                            if (param.Type is CLClassType && ((CLClassType)param.Type).IsReferenceObject &&
                                info.Method.OwnerProperty != null && info.Method.OwnerProperty.Setter == info.Method)
                                postStmt.AppendWithIndent("selfObj->{0} = {1};", info.Method.OwnerProperty.Name, param.Name).NewLine();
                        }
                    }
                }

                // rb_scan_args の呼び出し
                string rb_scan_args_Text = "";
                if (!scan_args_Args.IsEmpty)
                    rb_scan_args_Text = string.Format(@"rb_scan_args(argc, argv, ""{0}{1}"", {2});", normalArgsCount, (defaultArgsCount > 0) ? defaultArgsCount.ToString() : "", scan_args_Args);

                // 型チェック式が空なら true にしておく
                if (string.IsNullOrEmpty(typeCheckExp))
                    typeCheckExp = "true";

                // エラーコードと throw
                string preErrorStmt = "";
                string postErrorStmt = "";
                if (info.Method.ReturnType.Type == CLPrimitiveType.ResultCode)
                {
                    preErrorStmt = "lnErrorCode errorCode = ";
                    postErrorStmt = @"if (errorCode != LN_OK) rb_raise(gLNoteError, ""internal error. code:%d"", errorCode);" + OutputBuffer.NewLineCode;
                }

                // API 呼び出し
                var apiCall = string.Format("{0}({1});", info.Method.CName, argsText.ToString());

                callBody.AppendWithIndent("if ({0} <= argc && argc <= {1}) {{", normalArgsCount.ToString(), (normalArgsCount + defaultArgsCount).ToString()).NewLine();
                callBody.IncreaseIndent();
                callBody.AppendWithIndent(scan_args_Inits.ToString());
                callBody.AppendWithIndent(rb_scan_args_Text).NewLine();
                callBody.AppendWithIndent("if ({0}) {{", typeCheckExp).NewLine();
                callBody.IncreaseIndent();
                callBody.AppendWithIndent(initStmt.ToString());
                callBody.AppendWithIndent(preErrorStmt + apiCall + OutputBuffer.NewLineCode);
                callBody.AppendWithIndent(postErrorStmt);
                callBody.AppendWithIndent(postStmt.ToString());
                callBody.AppendWithIndent((returnStmt.IsEmpty) ? "return Qnil;" : returnStmt.ToString());
                callBody.DecreaseIndent();
                callBody.AppendWithIndent("}").NewLine();
                callBody.DecreaseIndent();
                callBody.AppendWithIndent("}").NewLine();

                funcBody.AppendWithIndent(callBody.ToString());
            }

            // メソッド名
            string rubyMethodName = RubyCommon.ConvertCommonNameToRubyMethodName(baseMethod);

            // 関数終端まで到達することはない。仮の return
            funcBody.AppendWithIndent(@"rb_raise(rb_eArgError, ""LNote::{0}.{1} - wrong argument type."");", baseMethod.OwnerClass.Name, rubyMethodName).NewLine();
            funcBody.AppendWithIndent("return Qnil;");

            // Init_lnote 登録処理
            string def;
            if (baseMethod.Attribute == MethodAttribute.Constructor)
                def = string.Format(@"rb_define_private_method({0}, ""initialize"", LN_TO_RUBY_FUNC({1}), -1);", typeValName, funcName);
            else if (baseMethod.IsInstanceMethod)
                def = string.Format(@"rb_define_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}), -1);", typeValName, rubyMethodName, funcName);
            else
                def = string.Format(@"rb_define_singleton_method({0}, ""{1}"", LN_TO_RUBY_FUNC({2}), -1);", typeValName, rubyMethodName, funcName);
            _allRBDefinesText.IncreaseIndent();
            _allRBDefinesText.AppendWithIndent(def).NewLine();
            _allRBDefinesText.DecreaseIndent();

            // 関数作成
            string t = FuncDeclTemplate.Trim()
                .Replace("__FUNC_NAME__", funcName)
                .Replace("__CONTENTS__", funcBody.ToString());
            _allFuncDeclText.AppendWithIndent(t).NewLine(2);
        }

        /// <summary>
        /// オーバーライドオプション指定の実装
        /// </summary>
        /// <param name="method"></param>
        private void MakeOverrideMethod(CLMethod method)
        {
            if (method.CName == "LNVirtualPad_GetVirtualPad")
            {
                return;
            }
            throw new InvalidOperationException();
        }

        /// <summary>
        /// wrapStruct 名の取得
        /// </summary>
        /// <param name="classType"></param>
        /// <returns></returns>
        private string GetWrapStructName(CLClassType classType)
        {
            return "wrap" + classType.Name;
        }


        //---------------------------------------------------------------------

        public class RubyTypeInfo
        {
            public string RubyType;
            public string CheckExp;     // 型判別式
            public string CVarType;     // 変数型
            public string ConvRToCExp;  // Ruby → C 変換式
            public string CVarInitExp;  // C言語宣言文変換式
            public RubyTypeInfo(string rubyType, string checkExp, string cVarType, string convRToCExp, string cVarInitExp)
            {
                RubyType = rubyType;
                CheckExp = checkExp;
                CVarType = cVarType;
                ConvRToCExp = convRToCExp;
                CVarInitExp = cVarInitExp;
            }
        }

        public static Dictionary<CLType, RubyTypeInfo> _rubyTypeTable = new Dictionary<CLType, RubyTypeInfo>()
        {
            //{ CLPrimitiveType.Void }

            { CLPrimitiveType.ByteArray,    new RubyTypeInfo("Array",   "", "", "", "") },
            { CLPrimitiveType.String,       new RubyTypeInfo("String",  "isRbString({0})", "char*",        "StringValuePtr({0})", "char* {0} = StringValuePtr({1});") },

            { CLPrimitiveType.Bool,         new RubyTypeInfo("Bool",    "isRbBool({0})",   "lnBool",       "RbBooltoBool({0})",    "lnBool {0} = RbBooltoBool({1});") },
            { CLPrimitiveType.Byte,         new RubyTypeInfo("Integer", "isRbNumber({0})", "lnU8",         "FIX2INT({0})",         "lnU8 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Int,          new RubyTypeInfo("Integer", "isRbNumber({0})", "int",          "FIX2INT({0})",         "int {0} = FIX2INT({1});") },
            { CLPrimitiveType.UInt32,       new RubyTypeInfo("Integer", "isRbNumber({0})", "lnU32",        "FIX2INT({0})",         "lnU32 {0} = FIX2INT({1});") },
            { CLPrimitiveType.Float,        new RubyTypeInfo("Float",   "isRbFloat({0})",  "float",        "(float)(NUM2DBL({0}));", "float {0} = static_cast<float>(NUM2DBL({1}));") },
            { CLPrimitiveType.Double,       new RubyTypeInfo("Float",   "isRbFloat({0})",  "double",       "NUM2DBL({0})",         "double {0} = NUM2DBL({1});") },

            { CLPrimitiveType.ResultCode,   new RubyTypeInfo("Integer", "isRbNumber({0})", "lnResultCode", "(lnResultCode)FIX2INT({0})",   "lnResultCode {0} = (lnResultCode)FIX2INT({1});") },
            { CLPrimitiveType.ExceptionCallback, new RubyTypeInfo("",   "isRbNumber({0})", "lnExceptionCallback", null, null) },
            { CLPrimitiveType.IntPtr,       new RubyTypeInfo("",        "isRbNumber({0})", "lnIntPtr", null, null) },    // TODO:bignum?
            { CLPrimitiveType.HWND,         new RubyTypeInfo("",        "isRbNumber({0})", "HWND",         "FIX2INT({0})", "int {0} = FIX2INT({1});") },      // TODO:bignum?

            //{ CLPrimitiveType.Handle = new CLPrimitiveType("Handle");
            { CLPrimitiveType.Generic, new RubyTypeInfo("",             "isRbObject({0})",      "lnHandle",     "FIX2INT({0})", "lnHandle {0} = FIX2INT({1});") },

        };

        /// <summary>
        /// Ruby の VALUE が type であるかを識別するための式を返す
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        private string GetTypeCheckExp(CLType type, string rubyValueName)
        {
            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(type, out info))
                return string.Format(info.CheckExp, rubyValueName);

            // 定数は T_FIXNUM
            if (type is CLEnumType)
                return string.Format("isRbNumber({0})", rubyValueName);

            // struct / class
            if (type is CLClassType)
                return string.Format("isRbObject({0})", rubyValueName);

            throw new InvalidOperationException();
        }

        /// <summary>
        /// VALUE → C宣言式
        /// </summary>
        private string GetCastExpVALUEtoC(CLType type, string cVarName, string rubyVarName, string defaultValueSource)
        {
            string decl = null;
            string exp = null;

            // プリミティブ型
            RubyTypeInfo info;
            if (_rubyTypeTable.TryGetValue(type, out info))
            {
                decl = string.Format("{0} {1}", info.CVarType, cVarName);
                exp = string.Format(info.ConvRToCExp, rubyVarName);
            }

            // enum
            if (type is CLEnumType)
            {
                decl = string.Format("{0} {1}", type.CName, cVarName);
                exp = string.Format("({0})FIX2INT({1})", type.CName, rubyVarName);
            }

            if (type is CLClassType)
            {
                // struct 型
                if (((CLClassType)type).IsStruct)
                {
                    if (!string.IsNullOrEmpty(defaultValueSource))
                        throw new InvalidOperationException();  // struct 型のデフォルト引数は未実装
                    string t = string.Format("{0}* tmp_{1}; Data_Get_Struct({2}, {0}, tmp_{1});", GetWrapStructName((CLClassType)type), cVarName, rubyVarName);
                    t = t + string.Format("ln{0}& {1} = tmp_{1}->Value;", type.Name, cVarName);
                    return t;
                }
                // RefObj 型
                else
                {
                    decl = string.Format("lnHandle {0}", cVarName);
                    exp = string.Format("RbRefObjToHandle({0})", rubyVarName);
                }
            }

            if (!string.IsNullOrEmpty(decl) && !string.IsNullOrEmpty(exp))
            {
                // デフォルト引数があれば、省略されている場合に格納する
                if (!string.IsNullOrEmpty(defaultValueSource))
                    return string.Format("{0} = ({1} != Qnil) ? {2} : {3};", decl, rubyVarName, exp, defaultValueSource);
                else
                    return string.Format("{0} = {1};", decl, exp);
            }

            throw new InvalidOperationException();
        }

        /// <summary>
        /// C言語変数 → VALUE 変換式の作成
        /// </summary>
        /// <returns></returns>
        private void MakeCastExpCToVALUE(CLType cType, string varName, OutputBuffer output)
        {
            // プリミティブ型
            if (cType is CLPrimitiveType)
            {
                output.AppendWithIndent("return toVALUE({0});", varName);
                return;
            }

            // enum 型
            if (cType is CLEnumType)
            {
                output.AppendWithIndent("return INT2FIX({0});", varName);
                return;
            }

            var classType = cType as CLClassType;
            if (classType != null)
            {
                var typeValName = string.Format(@"g{0}Class", classType.Name);

                // struct 型
                if (((CLClassType)cType).IsStruct)
                {
                    // 新しいインスタンスを作って返す
                    output.AppendWithIndent("VALUE retObj = {0}_allocate({1});", classType.CName, typeValName).NewLine();
                    output.AppendWithIndent("(({0}*)DATA_PTR(retObj))->Value = {1};", GetWrapStructName(classType), varName).NewLine();
                    output.AppendWithIndent("return retObj;").NewLine();
                    return;
                }
                // RefObj 型
                else
                {
                    throw new InvalidOperationException();
                }
            }
            throw new InvalidOperationException();
        }

        /// <summary>
        /// 自動定義するクラスであるかのチェック
        /// </summary>
        /// <param name="type"></param>
        /// <returns></returns>
        private bool CheckAutoGenerateClass(CLClassType type)
        {
            // これらは独自定義する
            if (type.Name == "ReferenceObject" ||
                type.Name == "ObjectList" ||
                type.Name == "Exception")
            {
                return true;
            }
            return false;
        }


        
    }
}
