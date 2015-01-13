#include "ruby.h"
#include <stdio.h>
#include <vector>
#include <lnote.h>
#include "lnote_ruby.h"

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

struct wrapSize
{
    lnSize Value;

};
struct wrapRect
{
    lnRect Value;

};
struct wrapIntTable
    : public wrapReferenceObject
{

};
struct wrapVector2
{
    lnVector2 Value;

};
struct wrapVector3
{
    lnVector3 Value;

};
struct wrapVector4
{
    lnVector4 Value;

};
struct wrapMatrix
{
    lnMatrix Value;

};
struct wrapQuaternion
{
    lnQuaternion Value;

};
struct wrapRandom
    : public wrapReferenceObject
{

};
struct wrapFileStream
    : public wrapReferenceObject
{

};
struct wrapInput
{

};
struct wrapVirtualPad
    : public wrapReferenceObject
{

};
struct wrapAudio
{

};
struct wrapSoundListener
{

};
struct wrapSound
    : public wrapReferenceObject
{

};
struct wrapColor
{
    lnColor Value;

};
struct wrapColorF
{
    lnColorF Value;

};
struct wrapTone
{
    lnTone Value;

};
struct wrapTexture
    : public wrapReferenceObject
{

};
struct wrapFont
    : public wrapReferenceObject
{

};
struct wrapSceneGraph
{

};
struct wrapSceneNode
    : public wrapReferenceObject
{

};
struct wrapVisualNode
    : public wrapSceneNode
{
    VALUE Shader;

};
struct wrapSprite
    : public wrapVisualNode
{
    VALUE Texture;

};
struct wrapSpritePath
    : public wrapVisualNode
{

};
struct wrapModel
    : public wrapVisualNode
{

};
struct wrapEffect
    : public wrapVisualNode
{

};
struct wrapTilemap
    : public wrapVisualNode
{
    VALUE MapData;
    VALUE TilesetTexture;

};
struct wrapShader
    : public wrapReferenceObject
{

};
struct wrapViewPane
    : public wrapReferenceObject
{
    VALUE DefaultViewPane;
    VALUE Layers;

};
struct wrapLayer
    : public wrapReferenceObject
{

};
struct wrapViewFilter
    : public wrapLayer
{

};
struct wrapToneFilter
    : public wrapViewFilter
{

};
struct wrapTransitionFilter
    : public wrapViewFilter
{

};
struct wrapBlurFilter
    : public wrapViewFilter
{

};
struct wrapCamera
    : public wrapSceneNode
{
    VALUE Default3DCamera;
    VALUE Default2DCamera;

};
struct wrapConfig
{

};
struct wrapCore
{

};


VALUE gLNoteModule;
VALUE gLNoteError;  // exception
VALUE gReferenceObjectClass;
VALUE gObjectListClass;
VALUE gEnum_ErrorCode;
VALUE gEnum_FileAccessPriority;
VALUE gEnum_FileType;
VALUE gEnum_FileMode;
VALUE gEnum_FileAccess;
VALUE gEnum_SeekOrigin;
VALUE gEnum_AsyncIOState;
VALUE gEnum_RotationOrder;
VALUE gEnum_KeyCode;
VALUE gEnum_AcceleratorKey;
VALUE gEnum_Button;
VALUE gEnum_DeviceButtonType;
VALUE gEnum_DeviceButton;
VALUE gEnum_DirectMusicInitMode;
VALUE gEnum_SoundPlayingType;
VALUE gEnum_SoundPlayingState;
VALUE gEnum_SoundFadeState;
VALUE gEnum_InternalGameSound;
VALUE gEnum_BackbufferResizeMode;
VALUE gEnum_TextureFormat;
VALUE gEnum_BlendMode;
VALUE gEnum_CullingMode;
VALUE gEnum_TextAlign;
VALUE gEnum_PlacementType;
VALUE gEnum_FrontAxis;
VALUE gEnum_MeshCreateFlags;
VALUE gEnum_CameraType;
VALUE gEnum_Coord;
VALUE gEnum_ObjectType;
VALUE gSizeClass;
VALUE gRectClass;
VALUE gIntTableClass;
TypeUserData gIntTableClassTypeInfo;
VALUE gVector2Class;
VALUE gVector3Class;
VALUE gVector4Class;
VALUE gMatrixClass;
VALUE gQuaternionClass;
VALUE gRandomClass;
TypeUserData gRandomClassTypeInfo;
VALUE gFileStreamClass;
TypeUserData gFileStreamClassTypeInfo;
VALUE gInputClass;
VALUE gVirtualPadClass;
TypeUserData gVirtualPadClassTypeInfo;
VALUE gAudioClass;
VALUE gSoundListenerClass;
VALUE gSoundClass;
TypeUserData gSoundClassTypeInfo;
VALUE gColorClass;
VALUE gColorFClass;
VALUE gToneClass;
VALUE gTextureClass;
TypeUserData gTextureClassTypeInfo;
VALUE gFontClass;
TypeUserData gFontClassTypeInfo;
VALUE gSceneGraphClass;
VALUE gSceneNodeClass;
TypeUserData gSceneNodeClassTypeInfo;
VALUE gVisualNodeClass;
TypeUserData gVisualNodeClassTypeInfo;
VALUE gSpriteClass;
TypeUserData gSpriteClassTypeInfo;
VALUE gSpritePathClass;
TypeUserData gSpritePathClassTypeInfo;
VALUE gModelClass;
TypeUserData gModelClassTypeInfo;
VALUE gEffectClass;
TypeUserData gEffectClassTypeInfo;
VALUE gTilemapClass;
TypeUserData gTilemapClassTypeInfo;
VALUE gShaderClass;
TypeUserData gShaderClassTypeInfo;
VALUE gViewPaneClass;
TypeUserData gViewPaneClassTypeInfo;
VALUE LNViewPane_GetDefaultViewPane_default = Qnil;
VALUE gLayerClass;
TypeUserData gLayerClassTypeInfo;
VALUE gViewFilterClass;
TypeUserData gViewFilterClassTypeInfo;
VALUE gToneFilterClass;
TypeUserData gToneFilterClassTypeInfo;
VALUE gTransitionFilterClass;
TypeUserData gTransitionFilterClassTypeInfo;
VALUE gBlurFilterClass;
TypeUserData gBlurFilterClassTypeInfo;
VALUE gCameraClass;
TypeUserData gCameraClassTypeInfo;
VALUE LNCamera_GetDefault3DCamera_default = Qnil;
VALUE LNCamera_GetDefault2DCamera_default = Qnil;
VALUE gConfigClass;
VALUE gCoreClass;


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
    if (internalObj == NULL) rb_raise( gLNoteError, "Faild alloc - LReferenceObject_allocate" );
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
    if (internalObj == NULL) rb_raise( gLNoteError, "Faild alloc - LObjectList_allocate" );
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

static void LNSize_delete(wrapSize* obj)
{
    
    free(obj);
}

static void LNSize_mark(wrapSize* obj)
{

}

static VALUE LNSize_allocate( VALUE klass )
{
    VALUE obj;
    wrapSize* internalObj;

    internalObj = (wrapSize*)malloc(sizeof(wrapSize));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSize_allocate" );
    obj = Data_Wrap_Struct(klass, LNSize_mark, LNSize_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSize));

    return obj;
}

static VALUE LNSize_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapSize* selfObj;
    Data_Get_Struct(self, wrapSize, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE Width_;
    VALUE Height_;
    rb_scan_args(argc, argv, "2", &Width_, &Height_);
    selfObj->Value.Width = NUM2DBL(Width_);
    selfObj->Value.Height = NUM2DBL(Height_);
    return self;
}

static VALUE LNSize_Width_set( VALUE self, VALUE v )
{
    wrapSize* selfObj;
    Data_Get_Struct(self, wrapSize, selfObj);
    selfObj->Value.Width = NUM2DBL(v);
    return self;
}

static VALUE LNSize_Width_get( VALUE self)
{
    wrapSize* selfObj;
    Data_Get_Struct(self, wrapSize, selfObj);
    return toVALUE(selfObj->Value.Width);
}

static VALUE LNSize_Height_set( VALUE self, VALUE v )
{
    wrapSize* selfObj;
    Data_Get_Struct(self, wrapSize, selfObj);
    selfObj->Value.Height = NUM2DBL(v);
    return self;
}

static VALUE LNSize_Height_get( VALUE self)
{
    wrapSize* selfObj;
    Data_Get_Struct(self, wrapSize, selfObj);
    return toVALUE(selfObj->Value.Height);
}

static void LNRect_delete(wrapRect* obj)
{
    
    free(obj);
}

static void LNRect_mark(wrapRect* obj)
{

}

static VALUE LNRect_allocate( VALUE klass )
{
    VALUE obj;
    wrapRect* internalObj;

    internalObj = (wrapRect*)malloc(sizeof(wrapRect));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNRect_allocate" );
    obj = Data_Wrap_Struct(klass, LNRect_mark, LNRect_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapRect));

    return obj;
}

static VALUE LNRect_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Width_;
    VALUE Height_;
    rb_scan_args(argc, argv, "4", &X_, &Y_, &Width_, &Height_);
    selfObj->Value.X = NUM2DBL(X_);
    selfObj->Value.Y = NUM2DBL(Y_);
    selfObj->Value.Width = NUM2DBL(Width_);
    selfObj->Value.Height = NUM2DBL(Height_);
    return self;
}

static VALUE LNRect_X_set( VALUE self, VALUE v )
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    selfObj->Value.X = NUM2DBL(v);
    return self;
}

static VALUE LNRect_X_get( VALUE self)
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    return toVALUE(selfObj->Value.X);
}

static VALUE LNRect_Y_set( VALUE self, VALUE v )
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    selfObj->Value.Y = NUM2DBL(v);
    return self;
}

static VALUE LNRect_Y_get( VALUE self)
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    return toVALUE(selfObj->Value.Y);
}

static VALUE LNRect_Width_set( VALUE self, VALUE v )
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    selfObj->Value.Width = NUM2DBL(v);
    return self;
}

static VALUE LNRect_Width_get( VALUE self)
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    return toVALUE(selfObj->Value.Width);
}

static VALUE LNRect_Height_set( VALUE self, VALUE v )
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    selfObj->Value.Height = NUM2DBL(v);
    return self;
}

static VALUE LNRect_Height_get( VALUE self)
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);
    return toVALUE(selfObj->Value.Height);
}

static VALUE lnrbLNRect_Contains(int argc, VALUE *argv, VALUE self)
{
    wrapRect* selfObj;
    Data_Get_Struct(self, wrapRect, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE x;
        VALUE y;
        rb_scan_args(argc, argv, "2", &x, &y);
        if (isRbNumber(x) && isRbNumber(y)) {
            int _x = FIX2INT(x);
            int _y = FIX2INT(y);
            lnBool _result;
            LNRect_Contains(&selfObj->Value, _x, _y, &_result);
            return toVALUE(_result);    }
    }
    rb_raise(rb_eArgError, "LNote::Rect.contains - wrong argument type.");
    return Qnil;
}

static void LNIntTable_delete(wrapIntTable* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNIntTable_mark(wrapIntTable* obj)
{

}

static VALUE LNIntTable_allocate( VALUE klass )
{
    VALUE obj;
    wrapIntTable* internalObj;

    internalObj = (wrapIntTable*)malloc(sizeof(wrapIntTable));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNIntTable_allocate" );
    obj = Data_Wrap_Struct(klass, LNIntTable_mark, LNIntTable_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapIntTable));

    return obj;
}

static VALUE LNIntTable_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapIntTable* internalObj;

    internalObj = (wrapIntTable*)malloc(sizeof(wrapIntTable));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNIntTable_allocate" );
    obj = Data_Wrap_Struct(klass, LNIntTable_mark, LNIntTable_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapIntTable));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNIntTable_Create(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (1 <= argc && argc <= 3) {
        VALUE xSize;
        VALUE ySize;
        VALUE zSize;
        rb_scan_args(argc, argv, "12", &xSize, &ySize, &zSize);
        if (isRbNumber(xSize) && isRbNumber(ySize) && isRbNumber(zSize)) {
            int _xSize = FIX2INT(xSize);
            int _ySize = (ySize != Qnil) ? FIX2INT(ySize) : 1;
            int _zSize = (zSize != Qnil) ? FIX2INT(zSize) : 1;
            lnErrorCode errorCode = LNIntTable_Create(&selfObj->Handle, _xSize, _ySize, _zSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_SetValue(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (4 <= argc && argc <= 4) {
        VALUE xSize;
        VALUE ySize;
        VALUE zSize;
        VALUE value;
        rb_scan_args(argc, argv, "4", &xSize, &ySize, &zSize, &value);
        if (isRbNumber(xSize) && isRbNumber(ySize) && isRbNumber(zSize) && isRbNumber(value)) {
            int _xSize = FIX2INT(xSize);
            int _ySize = FIX2INT(ySize);
            int _zSize = FIX2INT(zSize);
            int _value = FIX2INT(value);
            lnErrorCode errorCode = LNIntTable_SetValue(selfObj->Handle, _xSize, _ySize, _zSize, _value);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.set_value - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_GetValue(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE xSize;
        VALUE ySize;
        VALUE zSize;
        rb_scan_args(argc, argv, "3", &xSize, &ySize, &zSize);
        if (isRbNumber(xSize) && isRbNumber(ySize) && isRbNumber(zSize)) {
            int _xSize = FIX2INT(xSize);
            int _ySize = FIX2INT(ySize);
            int _zSize = FIX2INT(zSize);
            int _value;
            lnErrorCode errorCode = LNIntTable_GetValue(selfObj->Handle, _xSize, _ySize, _zSize, &_value);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_value);    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.get_value - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_Resize(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE xSize;
        VALUE ySize;
        VALUE zSize;
        rb_scan_args(argc, argv, "3", &xSize, &ySize, &zSize);
        if (isRbNumber(xSize) && isRbNumber(ySize) && isRbNumber(zSize)) {
            int _xSize = FIX2INT(xSize);
            int _ySize = FIX2INT(ySize);
            int _zSize = FIX2INT(zSize);
            lnErrorCode errorCode = LNIntTable_Resize(selfObj->Handle, _xSize, _ySize, _zSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.resize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_GetXSize(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _xSize;
            lnErrorCode errorCode = LNIntTable_GetXSize(selfObj->Handle, &_xSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_xSize);    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.x_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_GetYSize(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _ySize;
            lnErrorCode errorCode = LNIntTable_GetYSize(selfObj->Handle, &_ySize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_ySize);    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.y_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNIntTable_GetZSize(int argc, VALUE *argv, VALUE self)
{
    wrapIntTable* selfObj;
    Data_Get_Struct(self, wrapIntTable, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _zSize;
            lnErrorCode errorCode = LNIntTable_GetZSize(selfObj->Handle, &_zSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_zSize);    }
    }
    rb_raise(rb_eArgError, "LNote::IntTable.z_size - wrong argument type.");
    return Qnil;
}

static void LNVector2_delete(wrapVector2* obj)
{
    
    free(obj);
}

static void LNVector2_mark(wrapVector2* obj)
{

}

static VALUE LNVector2_allocate( VALUE klass )
{
    VALUE obj;
    wrapVector2* internalObj;

    internalObj = (wrapVector2*)malloc(sizeof(wrapVector2));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVector2_allocate" );
    obj = Data_Wrap_Struct(klass, LNVector2_mark, LNVector2_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVector2));

    return obj;
}

static VALUE LNVector2_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE X_;
    VALUE Y_;
    rb_scan_args(argc, argv, "2", &X_, &Y_);
    selfObj->Value.X = NUM2DBL(X_);
    selfObj->Value.Y = NUM2DBL(Y_);
    return self;
}

static VALUE LNVector2_X_set( VALUE self, VALUE v )
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);
    selfObj->Value.X = NUM2DBL(v);
    return self;
}

static VALUE LNVector2_X_get( VALUE self)
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);
    return toVALUE(selfObj->Value.X);
}

static VALUE LNVector2_Y_set( VALUE self, VALUE v )
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);
    selfObj->Value.Y = NUM2DBL(v);
    return self;
}

static VALUE LNVector2_Y_get( VALUE self)
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);
    return toVALUE(selfObj->Value.Y);
}

static VALUE lnrbLNVector2_Set(int argc, VALUE *argv, VALUE self)
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE x;
        VALUE y;
        rb_scan_args(argc, argv, "2", &x, &y);
        if (isRbFloat(x) && isRbFloat(y)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            lnErrorCode errorCode = LNVector2_Set(&selfObj->Value, _x, _y);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Vector2.set - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_GetLength(int argc, VALUE *argv, VALUE self)
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            float _outLength;
            lnErrorCode errorCode = LNVector2_GetLength(&selfObj->Value, &_outLength);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outLength);    }
    }
    rb_raise(rb_eArgError, "LNote::Vector2.length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_GetSquareLength(int argc, VALUE *argv, VALUE self)
{
    wrapVector2* selfObj;
    Data_Get_Struct(self, wrapVector2, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            float _outLength;
            lnErrorCode errorCode = LNVector2_GetSquareLength(&selfObj->Value, &_outLength);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outLength);    }
    }
    rb_raise(rb_eArgError, "LNote::Vector2.square_length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector2_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector2* tmp__vec; Data_Get_Struct(vec, wrapVector2, tmp__vec);lnVector2& _vec = tmp__vec->Value;
            lnVector2 _outVec;
            lnErrorCode errorCode = LNVector2_Normalize(&_vec, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector2_allocate(gVector2Class);
            ((wrapVector2*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector2 _outVec;
            lnErrorCode errorCode = LNVector2_Normalize(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector2_allocate(gVector2Class);
            ((wrapVector2*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector2.normalize - wrong argument type.");
    return Qnil;
}

static void LNVector3_delete(wrapVector3* obj)
{
    
    free(obj);
}

static void LNVector3_mark(wrapVector3* obj)
{

}

static VALUE LNVector3_allocate( VALUE klass )
{
    VALUE obj;
    wrapVector3* internalObj;

    internalObj = (wrapVector3*)malloc(sizeof(wrapVector3));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVector3_allocate" );
    obj = Data_Wrap_Struct(klass, LNVector3_mark, LNVector3_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVector3));

    return obj;
}

static VALUE LNVector3_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    rb_scan_args(argc, argv, "3", &X_, &Y_, &Z_);
    selfObj->Value.X = NUM2DBL(X_);
    selfObj->Value.Y = NUM2DBL(Y_);
    selfObj->Value.Z = NUM2DBL(Z_);
    return self;
}

static VALUE LNVector3_X_set( VALUE self, VALUE v )
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    selfObj->Value.X = NUM2DBL(v);
    return self;
}

static VALUE LNVector3_X_get( VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    return toVALUE(selfObj->Value.X);
}

static VALUE LNVector3_Y_set( VALUE self, VALUE v )
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    selfObj->Value.Y = NUM2DBL(v);
    return self;
}

static VALUE LNVector3_Y_get( VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    return toVALUE(selfObj->Value.Y);
}

static VALUE LNVector3_Z_set( VALUE self, VALUE v )
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    selfObj->Value.Z = NUM2DBL(v);
    return self;
}

static VALUE LNVector3_Z_get( VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);
    return toVALUE(selfObj->Value.Z);
}

static VALUE lnrbLNVector3_Set(int argc, VALUE *argv, VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNVector3_Set(&selfObj->Value, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (2 <= argc && argc <= 2) {
        VALUE vec2;
        VALUE z;
        rb_scan_args(argc, argv, "2", &vec2, &z);
        if (isRbObject(vec2) && isRbFloat(z)) {
            wrapVector2* tmp__vec2; Data_Get_Struct(vec2, wrapVector2, tmp__vec2);lnVector2& _vec2 = tmp__vec2->Value;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNVector3_SetVec2(&selfObj->Value, &_vec2, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.set - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_GetLength(int argc, VALUE *argv, VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            float _outLength;
            lnErrorCode errorCode = LNVector3_GetLength(&selfObj->Value, &_outLength);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outLength);    }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_GetSquareLength(int argc, VALUE *argv, VALUE self)
{
    wrapVector3* selfObj;
    Data_Get_Struct(self, wrapVector3, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            float _outLength;
            lnErrorCode errorCode = LNVector3_GetSquareLength(&selfObj->Value, &_outLength);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outLength);    }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.square_length - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Normalize(&_vec, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Normalize(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.normalize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Dot(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec1;
        VALUE vec2;
        rb_scan_args(argc, argv, "2", &vec1, &vec2);
        if (isRbObject(vec1) && isRbObject(vec2)) {
            wrapVector3* tmp__vec1; Data_Get_Struct(vec1, wrapVector3, tmp__vec1);lnVector3& _vec1 = tmp__vec1->Value;
            wrapVector3* tmp__vec2; Data_Get_Struct(vec2, wrapVector3, tmp__vec2);lnVector3& _vec2 = tmp__vec2->Value;
            float _dot;
            lnErrorCode errorCode = LNVector3_Dot(&_vec1, &_vec2, &_dot);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_dot);    }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.dot - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Cross(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec1;
        VALUE vec2;
        rb_scan_args(argc, argv, "2", &vec1, &vec2);
        if (isRbObject(vec1) && isRbObject(vec2)) {
            wrapVector3* tmp__vec1; Data_Get_Struct(vec1, wrapVector3, tmp__vec1);lnVector3& _vec1 = tmp__vec1->Value;
            wrapVector3* tmp__vec2; Data_Get_Struct(vec2, wrapVector3, tmp__vec2);lnVector3& _vec2 = tmp__vec2->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Cross(&_vec1, &_vec2, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.cross - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Reflect(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE normal;
        rb_scan_args(argc, argv, "2", &vec, &normal);
        if (isRbObject(vec) && isRbObject(normal)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapVector3* tmp__normal; Data_Get_Struct(normal, wrapVector3, tmp__normal);lnVector3& _normal = tmp__normal->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Reflect(&_vec, &_normal, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.reflect - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Slide(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE normal;
        rb_scan_args(argc, argv, "2", &vec, &normal);
        if (isRbObject(vec) && isRbObject(normal)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapVector3* tmp__normal; Data_Get_Struct(normal, wrapVector3, tmp__normal);lnVector3& _normal = tmp__normal->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Slide(&_vec, &_normal, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.slide - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Lerp(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE vec1;
        VALUE vec2;
        VALUE t;
        rb_scan_args(argc, argv, "3", &vec1, &vec2, &t);
        if (isRbObject(vec1) && isRbObject(vec2) && isRbFloat(t)) {
            wrapVector3* tmp__vec1; Data_Get_Struct(vec1, wrapVector3, tmp__vec1);lnVector3& _vec1 = tmp__vec1->Value;
            wrapVector3* tmp__vec2; Data_Get_Struct(vec2, wrapVector3, tmp__vec2);lnVector3& _vec2 = tmp__vec2->Value;
            float _t = (float)(NUM2DBL(t));;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Lerp(&_vec1, &_vec2, _t, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.lerp - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_CatmullRom(int argc, VALUE *argv, VALUE self)
{
    if (5 <= argc && argc <= 5) {
        VALUE vec1;
        VALUE vec2;
        VALUE vec3;
        VALUE vec4;
        VALUE t;
        rb_scan_args(argc, argv, "5", &vec1, &vec2, &vec3, &vec4, &t);
        if (isRbObject(vec1) && isRbObject(vec2) && isRbObject(vec3) && isRbObject(vec4) && isRbFloat(t)) {
            wrapVector3* tmp__vec1; Data_Get_Struct(vec1, wrapVector3, tmp__vec1);lnVector3& _vec1 = tmp__vec1->Value;
            wrapVector3* tmp__vec2; Data_Get_Struct(vec2, wrapVector3, tmp__vec2);lnVector3& _vec2 = tmp__vec2->Value;
            wrapVector3* tmp__vec3; Data_Get_Struct(vec3, wrapVector3, tmp__vec3);lnVector3& _vec3 = tmp__vec3->Value;
            wrapVector3* tmp__vec4; Data_Get_Struct(vec4, wrapVector3, tmp__vec4);lnVector3& _vec4 = tmp__vec4->Value;
            float _t = (float)(NUM2DBL(t));;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_CatmullRom(&_vec1, &_vec2, &_vec3, &_vec4, _t, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.catmull_rom - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_Transform(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE mat;
        rb_scan_args(argc, argv, "2", &vec, &mat);
        if (isRbObject(vec) && isRbObject(mat)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_Transform(&_vec, &_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.transform - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_TransformCoord(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE mat;
        rb_scan_args(argc, argv, "2", &vec, &mat);
        if (isRbObject(vec) && isRbObject(mat)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_TransformCoord(&_vec, &_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.transform_coord - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_RotateAxis(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE vec;
        VALUE axis;
        VALUE r;
        rb_scan_args(argc, argv, "3", &vec, &axis, &r);
        if (isRbObject(vec) && isRbObject(axis) && isRbFloat(r)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapVector3* tmp__axis; Data_Get_Struct(axis, wrapVector3, tmp__axis);lnVector3& _axis = tmp__axis->Value;
            float _r = (float)(NUM2DBL(r));;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_RotateAxis(&_vec, &_axis, _r, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.rotate_axis - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVector3_RotateMatrix(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE vec;
        VALUE mat;
        rb_scan_args(argc, argv, "2", &vec, &mat);
        if (isRbObject(vec) && isRbObject(mat)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNVector3_RotateMatrix(&_vec, &_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Vector3.rotate_matrix - wrong argument type.");
    return Qnil;
}

static void LNVector4_delete(wrapVector4* obj)
{
    
    free(obj);
}

static void LNVector4_mark(wrapVector4* obj)
{

}

static VALUE LNVector4_allocate( VALUE klass )
{
    VALUE obj;
    wrapVector4* internalObj;

    internalObj = (wrapVector4*)malloc(sizeof(wrapVector4));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVector4_allocate" );
    obj = Data_Wrap_Struct(klass, LNVector4_mark, LNVector4_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVector4));

    return obj;
}

static VALUE LNVector4_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    VALUE W_;
    rb_scan_args(argc, argv, "4", &X_, &Y_, &Z_, &W_);
    selfObj->Value.X = NUM2DBL(X_);
    selfObj->Value.Y = NUM2DBL(Y_);
    selfObj->Value.Z = NUM2DBL(Z_);
    selfObj->Value.W = NUM2DBL(W_);
    return self;
}

static VALUE LNVector4_X_set( VALUE self, VALUE v )
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    selfObj->Value.X = NUM2DBL(v);
    return self;
}

static VALUE LNVector4_X_get( VALUE self)
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    return toVALUE(selfObj->Value.X);
}

static VALUE LNVector4_Y_set( VALUE self, VALUE v )
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    selfObj->Value.Y = NUM2DBL(v);
    return self;
}

static VALUE LNVector4_Y_get( VALUE self)
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    return toVALUE(selfObj->Value.Y);
}

static VALUE LNVector4_Z_set( VALUE self, VALUE v )
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    selfObj->Value.Z = NUM2DBL(v);
    return self;
}

static VALUE LNVector4_Z_get( VALUE self)
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    return toVALUE(selfObj->Value.Z);
}

static VALUE LNVector4_W_set( VALUE self, VALUE v )
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    selfObj->Value.W = NUM2DBL(v);
    return self;
}

static VALUE LNVector4_W_get( VALUE self)
{
    wrapVector4* selfObj;
    Data_Get_Struct(self, wrapVector4, selfObj);
    return toVALUE(selfObj->Value.W);
}

static void LNMatrix_delete(wrapMatrix* obj)
{
    
    free(obj);
}

static void LNMatrix_mark(wrapMatrix* obj)
{

}

static VALUE LNMatrix_allocate( VALUE klass )
{
    VALUE obj;
    wrapMatrix* internalObj;

    internalObj = (wrapMatrix*)malloc(sizeof(wrapMatrix));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNMatrix_allocate" );
    obj = Data_Wrap_Struct(klass, LNMatrix_mark, LNMatrix_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapMatrix));

    return obj;
}

static VALUE LNMatrix_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE M11_;
    VALUE M12_;
    VALUE M13_;
    VALUE M14_;
    VALUE M21_;
    VALUE M22_;
    VALUE M23_;
    VALUE M24_;
    VALUE M31_;
    VALUE M32_;
    VALUE M33_;
    VALUE M34_;
    VALUE M41_;
    VALUE M42_;
    VALUE M43_;
    VALUE M44_;
    rb_scan_args(argc, argv, "16", &M11_, &M12_, &M13_, &M14_, &M21_, &M22_, &M23_, &M24_, &M31_, &M32_, &M33_, &M34_, &M41_, &M42_, &M43_, &M44_);
    selfObj->Value.M11 = NUM2DBL(M11_);
    selfObj->Value.M12 = NUM2DBL(M12_);
    selfObj->Value.M13 = NUM2DBL(M13_);
    selfObj->Value.M14 = NUM2DBL(M14_);
    selfObj->Value.M21 = NUM2DBL(M21_);
    selfObj->Value.M22 = NUM2DBL(M22_);
    selfObj->Value.M23 = NUM2DBL(M23_);
    selfObj->Value.M24 = NUM2DBL(M24_);
    selfObj->Value.M31 = NUM2DBL(M31_);
    selfObj->Value.M32 = NUM2DBL(M32_);
    selfObj->Value.M33 = NUM2DBL(M33_);
    selfObj->Value.M34 = NUM2DBL(M34_);
    selfObj->Value.M41 = NUM2DBL(M41_);
    selfObj->Value.M42 = NUM2DBL(M42_);
    selfObj->Value.M43 = NUM2DBL(M43_);
    selfObj->Value.M44 = NUM2DBL(M44_);
    return self;
}

static VALUE LNMatrix_M11_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M11 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M11_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M11);
}

static VALUE LNMatrix_M12_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M12 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M12_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M12);
}

static VALUE LNMatrix_M13_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M13 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M13_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M13);
}

static VALUE LNMatrix_M14_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M14 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M14_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M14);
}

static VALUE LNMatrix_M21_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M21 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M21_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M21);
}

static VALUE LNMatrix_M22_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M22 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M22_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M22);
}

static VALUE LNMatrix_M23_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M23 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M23_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M23);
}

static VALUE LNMatrix_M24_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M24 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M24_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M24);
}

static VALUE LNMatrix_M31_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M31 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M31_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M31);
}

static VALUE LNMatrix_M32_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M32 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M32_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M32);
}

static VALUE LNMatrix_M33_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M33 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M33_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M33);
}

static VALUE LNMatrix_M34_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M34 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M34_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M34);
}

static VALUE LNMatrix_M41_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M41 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M41_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M41);
}

static VALUE LNMatrix_M42_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M42 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M42_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M42);
}

static VALUE LNMatrix_M43_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M43 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M43_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M43);
}

static VALUE LNMatrix_M44_set( VALUE self, VALUE v )
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    selfObj->Value.M44 = NUM2DBL(v);
    return self;
}

static VALUE LNMatrix_M44_get( VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);
    return toVALUE(selfObj->Value.M44);
}

static VALUE lnrbLNMatrix_Identity(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _mat;
            lnErrorCode errorCode = LNMatrix_Identity(&_mat);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _mat;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.tity - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Translate(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNMatrix_Translate(&selfObj->Value, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnErrorCode errorCode = LNMatrix_TranslateVec3(&selfObj->Value, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.translate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateX(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = (float)(NUM2DBL(radian));;
            lnErrorCode errorCode = LNMatrix_RotateX(&selfObj->Value, _radian);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate_x - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateY(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = (float)(NUM2DBL(radian));;
            lnErrorCode errorCode = LNMatrix_RotateY(&selfObj->Value, _radian);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate_y - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateZ(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE radian;
        rb_scan_args(argc, argv, "1", &radian);
        if (isRbFloat(radian)) {
            float _radian = (float)(NUM2DBL(radian));;
            lnErrorCode errorCode = LNMatrix_RotateZ(&selfObj->Value, _radian);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate_z - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Rotate(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (3 <= argc && argc <= 4) {
        VALUE xRad;
        VALUE yRad;
        VALUE zRad;
        VALUE rotOrder;
        rb_scan_args(argc, argv, "31", &xRad, &yRad, &zRad, &rotOrder);
        if (isRbFloat(xRad) && isRbFloat(yRad) && isRbFloat(zRad) && isRbNumber(rotOrder)) {
            float _xRad = (float)(NUM2DBL(xRad));;
            float _yRad = (float)(NUM2DBL(yRad));;
            float _zRad = (float)(NUM2DBL(zRad));;
            lnRotationOrder _rotOrder = (rotOrder != Qnil) ? (lnRotationOrder)FIX2INT(rotOrder) : LN_ROTATIONORDER_XYZ;
            lnErrorCode errorCode = LNMatrix_Rotate(&selfObj->Value, _xRad, _yRad, _zRad, _rotOrder);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 2) {
        VALUE vec;
        VALUE rotOrder;
        rb_scan_args(argc, argv, "11", &vec, &rotOrder);
        if (isRbObject(vec) && isRbNumber(rotOrder)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnRotationOrder _rotOrder = (rotOrder != Qnil) ? (lnRotationOrder)FIX2INT(rotOrder) : LN_ROTATIONORDER_XYZ;
            lnErrorCode errorCode = LNMatrix_RotateVec3(&selfObj->Value, &_vec, _rotOrder);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateAxis(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE axis;
        VALUE radian;
        rb_scan_args(argc, argv, "2", &axis, &radian);
        if (isRbObject(axis) && isRbFloat(radian)) {
            wrapVector3* tmp__axis; Data_Get_Struct(axis, wrapVector3, tmp__axis);lnVector3& _axis = tmp__axis->Value;
            float _radian = (float)(NUM2DBL(radian));;
            lnErrorCode errorCode = LNMatrix_RotateAxis(&selfObj->Value, &_axis, _radian);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate_axis - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_RotateQuaternion(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            wrapQuaternion* tmp__qua; Data_Get_Struct(qua, wrapQuaternion, tmp__qua);lnQuaternion& _qua = tmp__qua->Value;
            lnErrorCode errorCode = LNMatrix_RotateQuaternion(&selfObj->Value, &_qua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.rotate_quaternion - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Scale(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE xyz;
        rb_scan_args(argc, argv, "1", &xyz);
        if (isRbFloat(xyz)) {
            float _xyz = (float)(NUM2DBL(xyz));;
            lnErrorCode errorCode = LNMatrix_Scale(&selfObj->Value, _xyz);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNMatrix_ScaleXYZ(&selfObj->Value, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE scale;
        rb_scan_args(argc, argv, "1", &scale);
        if (isRbObject(scale)) {
            wrapVector3* tmp__scale; Data_Get_Struct(scale, wrapVector3, tmp__scale);lnVector3& _scale = tmp__scale->Value;
            lnErrorCode errorCode = LNMatrix_ScaleVec3(&selfObj->Value, &_scale);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.scale - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Multiply(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE mat1;
        VALUE mat2;
        rb_scan_args(argc, argv, "2", &mat1, &mat2);
        if (isRbObject(mat1) && isRbObject(mat2)) {
            wrapMatrix* tmp__mat1; Data_Get_Struct(mat1, wrapMatrix, tmp__mat1);lnMatrix& _mat1 = tmp__mat1->Value;
            wrapMatrix* tmp__mat2; Data_Get_Struct(mat2, wrapMatrix, tmp__mat2);lnMatrix& _mat2 = tmp__mat2->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_Multiply(&_mat1, &_mat2, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.multiply - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Inverse(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_Inverse(&_mat, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_Inverse(&selfObj->Value, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.inverse - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_Transpose(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_Transpose(&_mat, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_Transpose(&selfObj->Value, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.transpose - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_ViewTransformLH(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE pos;
        VALUE lookAt;
        VALUE upDir;
        rb_scan_args(argc, argv, "3", &pos, &lookAt, &upDir);
        if (isRbObject(pos) && isRbObject(lookAt) && isRbObject(upDir)) {
            wrapVector3* tmp__pos; Data_Get_Struct(pos, wrapVector3, tmp__pos);lnVector3& _pos = tmp__pos->Value;
            wrapVector3* tmp__lookAt; Data_Get_Struct(lookAt, wrapVector3, tmp__lookAt);lnVector3& _lookAt = tmp__lookAt->Value;
            wrapVector3* tmp__upDir; Data_Get_Struct(upDir, wrapVector3, tmp__upDir);lnVector3& _upDir = tmp__upDir->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_ViewTransformLH(&_pos, &_lookAt, &_upDir, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.view_transform_lh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_ViewTransformRH(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE pos;
        VALUE lookAt;
        VALUE upDir;
        rb_scan_args(argc, argv, "3", &pos, &lookAt, &upDir);
        if (isRbObject(pos) && isRbObject(lookAt) && isRbObject(upDir)) {
            wrapVector3* tmp__pos; Data_Get_Struct(pos, wrapVector3, tmp__pos);lnVector3& _pos = tmp__pos->Value;
            wrapVector3* tmp__lookAt; Data_Get_Struct(lookAt, wrapVector3, tmp__lookAt);lnVector3& _lookAt = tmp__lookAt->Value;
            wrapVector3* tmp__upDir; Data_Get_Struct(upDir, wrapVector3, tmp__upDir);lnVector3& _upDir = tmp__upDir->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_ViewTransformRH(&_pos, &_lookAt, &_upDir, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.view_transform_rh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_PerspectiveFovLH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE fovY;
        VALUE aspect;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &fovY, &aspect, &nearZ, &farZ);
        if (isRbFloat(fovY) && isRbFloat(aspect) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _fovY = (float)(NUM2DBL(fovY));;
            float _aspect = (float)(NUM2DBL(aspect));;
            float _nearZ = (float)(NUM2DBL(nearZ));;
            float _farZ = (float)(NUM2DBL(farZ));;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_PerspectiveFovLH(_fovY, _aspect, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.perspective_fov_lh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_PerspectiveFovRH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE fovY;
        VALUE aspect;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &fovY, &aspect, &nearZ, &farZ);
        if (isRbFloat(fovY) && isRbFloat(aspect) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _fovY = (float)(NUM2DBL(fovY));;
            float _aspect = (float)(NUM2DBL(aspect));;
            float _nearZ = (float)(NUM2DBL(nearZ));;
            float _farZ = (float)(NUM2DBL(farZ));;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_PerspectiveFovRH(_fovY, _aspect, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.perspective_fov_rh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_OrthoLH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &width, &height, &nearZ, &farZ);
        if (isRbFloat(width) && isRbFloat(height) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _width = (float)(NUM2DBL(width));;
            float _height = (float)(NUM2DBL(height));;
            float _nearZ = (float)(NUM2DBL(nearZ));;
            float _farZ = (float)(NUM2DBL(farZ));;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_OrthoLH(_width, _height, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.ortho_lh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_OrthoRH(int argc, VALUE *argv, VALUE self)
{
    if (4 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE nearZ;
        VALUE farZ;
        rb_scan_args(argc, argv, "4", &width, &height, &nearZ, &farZ);
        if (isRbFloat(width) && isRbFloat(height) && isRbFloat(nearZ) && isRbFloat(farZ)) {
            float _width = (float)(NUM2DBL(width));;
            float _height = (float)(NUM2DBL(height));;
            float _nearZ = (float)(NUM2DBL(nearZ));;
            float _farZ = (float)(NUM2DBL(farZ));;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_OrthoRH(_width, _height, _nearZ, _farZ, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.ortho_rh - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetRight(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_GetRight(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.right - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetUp(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_GetUp(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.up - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetFront(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_GetFront(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.front - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_GetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapMatrix* selfObj;
    Data_Get_Struct(self, wrapMatrix, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_GetPosition(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_ToEuler(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_ToEuler(&_mat, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _outVec;
            lnErrorCode errorCode = LNMatrix_ToEuler(&selfObj->Value, &_outVec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _outVec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.to_euler - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNMatrix_TransformBasis(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_TransformBasis(&_mat, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _matOut;
            lnErrorCode errorCode = LNMatrix_TransformBasis(&selfObj->Value, &_matOut);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _matOut;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Matrix.transform_basis - wrong argument type.");
    return Qnil;
}

static void LNQuaternion_delete(wrapQuaternion* obj)
{
    
    free(obj);
}

static void LNQuaternion_mark(wrapQuaternion* obj)
{

}

static VALUE LNQuaternion_allocate( VALUE klass )
{
    VALUE obj;
    wrapQuaternion* internalObj;

    internalObj = (wrapQuaternion*)malloc(sizeof(wrapQuaternion));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNQuaternion_allocate" );
    obj = Data_Wrap_Struct(klass, LNQuaternion_mark, LNQuaternion_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapQuaternion));

    return obj;
}

static VALUE LNQuaternion_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE X_;
    VALUE Y_;
    VALUE Z_;
    VALUE W_;
    rb_scan_args(argc, argv, "4", &X_, &Y_, &Z_, &W_);
    selfObj->Value.X = NUM2DBL(X_);
    selfObj->Value.Y = NUM2DBL(Y_);
    selfObj->Value.Z = NUM2DBL(Z_);
    selfObj->Value.W = NUM2DBL(W_);
    return self;
}

static VALUE LNQuaternion_X_set( VALUE self, VALUE v )
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    selfObj->Value.X = NUM2DBL(v);
    return self;
}

static VALUE LNQuaternion_X_get( VALUE self)
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    return toVALUE(selfObj->Value.X);
}

static VALUE LNQuaternion_Y_set( VALUE self, VALUE v )
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    selfObj->Value.Y = NUM2DBL(v);
    return self;
}

static VALUE LNQuaternion_Y_get( VALUE self)
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    return toVALUE(selfObj->Value.Y);
}

static VALUE LNQuaternion_Z_set( VALUE self, VALUE v )
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    selfObj->Value.Z = NUM2DBL(v);
    return self;
}

static VALUE LNQuaternion_Z_get( VALUE self)
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    return toVALUE(selfObj->Value.Z);
}

static VALUE LNQuaternion_W_set( VALUE self, VALUE v )
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    selfObj->Value.W = NUM2DBL(v);
    return self;
}

static VALUE LNQuaternion_W_get( VALUE self)
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);
    return toVALUE(selfObj->Value.W);
}

static VALUE lnrbLNQuaternion_Set(int argc, VALUE *argv, VALUE self)
{
    wrapQuaternion* selfObj;
    Data_Get_Struct(self, wrapQuaternion, selfObj);

    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE w;
        rb_scan_args(argc, argv, "4", &x, &y, &z, &w);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(w)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            float _w = (float)(NUM2DBL(w));;
            lnErrorCode errorCode = LNQuaternion_Set(&selfObj->Value, _x, _y, _z, _w);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.set - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_Identity(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnQuaternion _qua;
            lnErrorCode errorCode = LNQuaternion_Identity(&_qua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _qua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.tity - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_RotationAxis(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE axis;
        VALUE r;
        rb_scan_args(argc, argv, "2", &axis, &r);
        if (isRbObject(axis) && isRbFloat(r)) {
            wrapVector3* tmp__axis; Data_Get_Struct(axis, wrapVector3, tmp__axis);lnVector3& _axis = tmp__axis->Value;
            float _r = (float)(NUM2DBL(r));;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_RotationAxis(&_axis, _r, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.rotation_axis - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_RotationMatrix(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_RotationMatrix(&_mat, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.rotation_matrix - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_RotationYawPitchRoll(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE yaw;
        VALUE pitch;
        VALUE roll;
        rb_scan_args(argc, argv, "3", &yaw, &pitch, &roll);
        if (isRbFloat(yaw) && isRbFloat(pitch) && isRbFloat(roll)) {
            float _yaw = (float)(NUM2DBL(yaw));;
            float _pitch = (float)(NUM2DBL(pitch));;
            float _roll = (float)(NUM2DBL(roll));;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_RotationYawPitchRoll(_yaw, _pitch, _roll, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.rotation_yaw_pitch_roll - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_Normalize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            wrapQuaternion* tmp__qua; Data_Get_Struct(qua, wrapQuaternion, tmp__qua);lnQuaternion& _qua = tmp__qua->Value;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Normalize(&_qua, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Normalize(&selfObj->Value, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.normalize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_Conjugate(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE qua;
        rb_scan_args(argc, argv, "1", &qua);
        if (isRbObject(qua)) {
            wrapQuaternion* tmp__qua; Data_Get_Struct(qua, wrapQuaternion, tmp__qua);lnQuaternion& _qua = tmp__qua->Value;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Conjugate(&_qua, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Conjugate(&selfObj->Value, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.conjugate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_Multiply(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE qua1;
        VALUE qua2;
        rb_scan_args(argc, argv, "2", &qua1, &qua2);
        if (isRbObject(qua1) && isRbObject(qua2)) {
            wrapQuaternion* tmp__qua1; Data_Get_Struct(qua1, wrapQuaternion, tmp__qua1);lnQuaternion& _qua1 = tmp__qua1->Value;
            wrapQuaternion* tmp__qua2; Data_Get_Struct(qua2, wrapQuaternion, tmp__qua2);lnQuaternion& _qua2 = tmp__qua2->Value;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Multiply(&_qua1, &_qua2, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.multiply - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNQuaternion_Slerp(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE qua1;
        VALUE qua2;
        VALUE t;
        rb_scan_args(argc, argv, "3", &qua1, &qua2, &t);
        if (isRbObject(qua1) && isRbObject(qua2) && isRbFloat(t)) {
            wrapQuaternion* tmp__qua1; Data_Get_Struct(qua1, wrapQuaternion, tmp__qua1);lnQuaternion& _qua1 = tmp__qua1->Value;
            wrapQuaternion* tmp__qua2; Data_Get_Struct(qua2, wrapQuaternion, tmp__qua2);lnQuaternion& _qua2 = tmp__qua2->Value;
            float _t = (float)(NUM2DBL(t));;
            lnQuaternion _outQua;
            lnErrorCode errorCode = LNQuaternion_Slerp(&_qua1, &_qua2, _t, &_outQua);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNQuaternion_allocate(gQuaternionClass);
            ((wrapQuaternion*)DATA_PTR(retObj))->Value = _outQua;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Quaternion.slerp - wrong argument type.");
    return Qnil;
}

static void LNRandom_delete(wrapRandom* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNRandom_mark(wrapRandom* obj)
{

}

static VALUE LNRandom_allocate( VALUE klass )
{
    VALUE obj;
    wrapRandom* internalObj;

    internalObj = (wrapRandom*)malloc(sizeof(wrapRandom));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNRandom_allocate" );
    obj = Data_Wrap_Struct(klass, LNRandom_mark, LNRandom_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapRandom));

    return obj;
}

static VALUE LNRandom_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapRandom* internalObj;

    internalObj = (wrapRandom*)malloc(sizeof(wrapRandom));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNRandom_allocate" );
    obj = Data_Wrap_Struct(klass, LNRandom_mark, LNRandom_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapRandom));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNRandom_Create(int argc, VALUE *argv, VALUE self)
{
    wrapRandom* selfObj;
    Data_Get_Struct(self, wrapRandom, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNRandom_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE seed;
        rb_scan_args(argc, argv, "1", &seed);
        if (isRbNumber(seed)) {
            int _seed = FIX2INT(seed);
            lnErrorCode errorCode = LNRandom_CreateSeed(&selfObj->Handle, _seed);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Random.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNRandom_SetSeed(int argc, VALUE *argv, VALUE self)
{
    wrapRandom* selfObj;
    Data_Get_Struct(self, wrapRandom, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE seed;
        rb_scan_args(argc, argv, "1", &seed);
        if (isRbNumber(seed)) {
            int _seed = FIX2INT(seed);
            lnErrorCode errorCode = LNRandom_SetSeed(selfObj->Handle, _seed);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Random.seed= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNRandom_GetInt(int argc, VALUE *argv, VALUE self)
{
    wrapRandom* selfObj;
    Data_Get_Struct(self, wrapRandom, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _outValue;
            lnErrorCode errorCode = LNRandom_GetInt(selfObj->Handle, &_outValue);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outValue);    }
    }
    rb_raise(rb_eArgError, "LNote::Random.get_int - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNRandom_GetIntRange(int argc, VALUE *argv, VALUE self)
{
    wrapRandom* selfObj;
    Data_Get_Struct(self, wrapRandom, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE minValue;
        VALUE maxValue;
        rb_scan_args(argc, argv, "2", &minValue, &maxValue);
        if (isRbNumber(minValue) && isRbNumber(maxValue)) {
            int _minValue = FIX2INT(minValue);
            int _maxValue = FIX2INT(maxValue);
            int _outValue;
            lnErrorCode errorCode = LNRandom_GetIntRange(selfObj->Handle, _minValue, _maxValue, &_outValue);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outValue);    }
    }
    rb_raise(rb_eArgError, "LNote::Random.get_int_range - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNRandom_GetIntDeflect(int argc, VALUE *argv, VALUE self)
{
    wrapRandom* selfObj;
    Data_Get_Struct(self, wrapRandom, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE center;
        VALUE width;
        rb_scan_args(argc, argv, "2", &center, &width);
        if (isRbNumber(center) && isRbNumber(width)) {
            int _center = FIX2INT(center);
            int _width = FIX2INT(width);
            int _outValue;
            lnErrorCode errorCode = LNRandom_GetIntDeflect(selfObj->Handle, _center, _width, &_outValue);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outValue);    }
    }
    rb_raise(rb_eArgError, "LNote::Random.get_int_deflect - wrong argument type.");
    return Qnil;
}

static void LNFileStream_delete(wrapFileStream* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNFileStream_mark(wrapFileStream* obj)
{

}

static VALUE LNFileStream_allocate( VALUE klass )
{
    VALUE obj;
    wrapFileStream* internalObj;

    internalObj = (wrapFileStream*)malloc(sizeof(wrapFileStream));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNFileStream_allocate" );
    obj = Data_Wrap_Struct(klass, LNFileStream_mark, LNFileStream_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapFileStream));

    return obj;
}

static VALUE LNFileStream_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapFileStream* internalObj;

    internalObj = (wrapFileStream*)malloc(sizeof(wrapFileStream));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNFileStream_allocate" );
    obj = Data_Wrap_Struct(klass, LNFileStream_mark, LNFileStream_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapFileStream));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNFileStream_Create(int argc, VALUE *argv, VALUE self)
{
    wrapFileStream* selfObj;
    Data_Get_Struct(self, wrapFileStream, selfObj);

    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE mode;
        VALUE access;
        rb_scan_args(argc, argv, "12", &filePath, &mode, &access);
        if (isRbString(filePath) && isRbNumber(mode) && isRbNumber(access)) {
            char* _filePath = StringValuePtr(filePath);
            lnFileMode _mode = (mode != Qnil) ? (lnFileMode)FIX2INT(mode) : LN_FILEMODE_OPEN;
            lnFileAccess _access = (access != Qnil) ? (lnFileAccess)FIX2INT(access) : LN_FILEMODE_READ;
            lnErrorCode errorCode = LNFileStream_Create(&selfObj->Handle, _filePath, _mode, _access);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFileStream_Close(int argc, VALUE *argv, VALUE self)
{
    wrapFileStream* selfObj;
    Data_Get_Struct(self, wrapFileStream, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNFileStream_Close(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.close - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFileStream_GetSize(int argc, VALUE *argv, VALUE self)
{
    wrapFileStream* selfObj;
    Data_Get_Struct(self, wrapFileStream, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _size;
            lnErrorCode errorCode = LNFileStream_GetSize(selfObj->Handle, &_size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_size);    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFileStream_GetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapFileStream* selfObj;
    Data_Get_Struct(self, wrapFileStream, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _position;
            lnErrorCode errorCode = LNFileStream_GetPosition(selfObj->Handle, &_position);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_position);    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFileStream_Seek(int argc, VALUE *argv, VALUE self)
{
    wrapFileStream* selfObj;
    Data_Get_Struct(self, wrapFileStream, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE offset;
        VALUE origin;
        rb_scan_args(argc, argv, "2", &offset, &origin);
        if (isRbNumber(offset) && isRbNumber(origin)) {
            int _offset = FIX2INT(offset);
            lnSeekOrigin _origin = (lnSeekOrigin)FIX2INT(origin);
            lnErrorCode errorCode = LNFileStream_Seek(selfObj->Handle, _offset, _origin);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.seek - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFileStream_Exists(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            lnBool _result;
            lnErrorCode errorCode = LNFileStream_Exists(_filePath, &_result);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_result);    }
    }
    rb_raise(rb_eArgError, "LNote::FileStream.exists - wrong argument type.");
    return Qnil;
}

static void LNInput_delete(wrapInput* obj)
{
    
    free(obj);
}

static void LNInput_mark(wrapInput* obj)
{

}

static VALUE LNInput_allocate( VALUE klass )
{
    VALUE obj;
    wrapInput* internalObj;

    internalObj = (wrapInput*)malloc(sizeof(wrapInput));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNInput_allocate" );
    obj = Data_Wrap_Struct(klass, LNInput_mark, LNInput_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapInput));

    return obj;
}

static VALUE lnrbLNInput_IsPress(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNInput_IsPress(_button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Input.press? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNInput_IsTrigger(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNInput_IsTrigger(_button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Input.trigger? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNInput_IsOffTrigger(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNInput_IsOffTrigger(_button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Input.off_trigger? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNInput_IsRepeat(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNInput_IsRepeat(_button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Input.repeat? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNInput_GetPower(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            float _state;
            lnErrorCode errorCode = LNInput_GetPower(_button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Input.get_power - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNInput_GetAxisState(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE axis;
        rb_scan_args(argc, argv, "1", &axis);
        if (isRbNumber(axis)) {
            int _axis = FIX2INT(axis);
            lnVector2 _state;
            lnErrorCode errorCode = LNInput_GetAxisState(_axis, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector2_allocate(gVector2Class);
            ((wrapVector2*)DATA_PTR(retObj))->Value = _state;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Input.get_axis_state - wrong argument type.");
    return Qnil;
}

static void LNVirtualPad_delete(wrapVirtualPad* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNVirtualPad_mark(wrapVirtualPad* obj)
{

}

static VALUE LNVirtualPad_allocate( VALUE klass )
{
    VALUE obj;
    wrapVirtualPad* internalObj;

    internalObj = (wrapVirtualPad*)malloc(sizeof(wrapVirtualPad));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVirtualPad_allocate" );
    obj = Data_Wrap_Struct(klass, LNVirtualPad_mark, LNVirtualPad_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVirtualPad));

    return obj;
}

static VALUE LNVirtualPad_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapVirtualPad* internalObj;

    internalObj = (wrapVirtualPad*)malloc(sizeof(wrapVirtualPad));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVirtualPad_allocate" );
    obj = Data_Wrap_Struct(klass, LNVirtualPad_mark, LNVirtualPad_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVirtualPad));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNVirtualPad_IsPress(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNVirtualPad_IsPress(selfObj->Handle, _button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.press? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_IsTrigger(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNVirtualPad_IsTrigger(selfObj->Handle, _button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.trigger? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_IsOffTrigger(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNVirtualPad_IsOffTrigger(selfObj->Handle, _button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.off_trigger? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_IsRepeat(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            lnBool _state;
            lnErrorCode errorCode = LNVirtualPad_IsRepeat(selfObj->Handle, _button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.repeat? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_GetPower(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE button;
        rb_scan_args(argc, argv, "1", &button);
        if (isRbNumber(button)) {
            int _button = FIX2INT(button);
            float _state;
            lnErrorCode errorCode = LNVirtualPad_GetPower(selfObj->Handle, _button, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.get_power - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_GetAxisState(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE virtualPad;
        VALUE axis;
        rb_scan_args(argc, argv, "2", &virtualPad, &axis);
        if (isRbObject(virtualPad) && isRbNumber(axis)) {
            lnHandle _virtualPad = RbRefObjToHandle(virtualPad);
            int _axis = FIX2INT(axis);
            lnVector2 _state;
            lnErrorCode errorCode = LNVirtualPad_GetAxisState(_virtualPad, _axis, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector2_allocate(gVector2Class);
            ((wrapVector2*)DATA_PTR(retObj))->Value = _state;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.get_axis_state - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_GetPressedDeviceButton(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _deviceButton;
            lnErrorCode errorCode = LNVirtualPad_GetPressedDeviceButton(selfObj->Handle, &_deviceButton);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_deviceButton);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.pressed_device_button - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_GetDeviceButtonType(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE deviceButton;
        rb_scan_args(argc, argv, "1", &deviceButton);
        if (isRbNumber(deviceButton)) {
            int _deviceButton = FIX2INT(deviceButton);
            lnDeviceButtonType _type;
            lnErrorCode errorCode = LNVirtualPad_GetDeviceButtonType(_deviceButton, &_type);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return INT2FIX(_type);    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.get_device_button_type - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_AttachVirtualButton(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE virtualButton;
        VALUE deviceButton;
        rb_scan_args(argc, argv, "2", &virtualButton, &deviceButton);
        if (isRbNumber(virtualButton) && isRbNumber(deviceButton)) {
            int _virtualButton = FIX2INT(virtualButton);
            int _deviceButton = FIX2INT(deviceButton);
            lnErrorCode errorCode = LNVirtualPad_AttachVirtualButton(selfObj->Handle, _virtualButton, _deviceButton);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.attach_virtual_button - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVirtualPad_DetachVirtualButton(int argc, VALUE *argv, VALUE self)
{
    wrapVirtualPad* selfObj;
    Data_Get_Struct(self, wrapVirtualPad, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE virtualButton;
        VALUE deviceButton;
        rb_scan_args(argc, argv, "2", &virtualButton, &deviceButton);
        if (isRbNumber(virtualButton) && isRbNumber(deviceButton)) {
            int _virtualButton = FIX2INT(virtualButton);
            int _deviceButton = FIX2INT(deviceButton);
            lnErrorCode errorCode = LNVirtualPad_DetachVirtualButton(selfObj->Handle, _virtualButton, _deviceButton);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VirtualPad.detach_virtual_button - wrong argument type.");
    return Qnil;
}

static void LNAudio_delete(wrapAudio* obj)
{
    
    free(obj);
}

static void LNAudio_mark(wrapAudio* obj)
{

}

static VALUE LNAudio_allocate( VALUE klass )
{
    VALUE obj;
    wrapAudio* internalObj;

    internalObj = (wrapAudio*)malloc(sizeof(wrapAudio));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNAudio_allocate" );
    obj = Data_Wrap_Struct(klass, LNAudio_mark, LNAudio_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapAudio));

    return obj;
}

static VALUE lnrbLNAudio_PlayBGM(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch) && isRbNumber(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_PlayBGM(_filePath, _volume, _pitch, _fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.play_bgm - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_StopBGM(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbNumber(fadeTime)) {
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_StopBGM(_fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.stop_bgm - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_PlayBGS(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "13", &filePath, &volume, &pitch, &fadeTime);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch) && isRbNumber(fadeTime)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_PlayBGS(_filePath, _volume, _pitch, _fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.play_bgs - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_StopBGS(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 1) {
        VALUE fadeTime;
        rb_scan_args(argc, argv, "01", &fadeTime);
        if (isRbNumber(fadeTime)) {
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_StopBGS(_fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.stop_bgs - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_PlayME(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            lnErrorCode errorCode = LNAudio_PlayME(_filePath, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.play_me - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_StopME(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            LNAudio_StopME();
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.stop_me - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_PlaySE(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "12", &filePath, &volume, &pitch);
        if (isRbString(filePath) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            lnErrorCode errorCode = LNAudio_PlaySE(_filePath, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.play_se - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_PlaySE3D(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 5) {
        VALUE filePath;
        VALUE position;
        VALUE distance;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "32", &filePath, &position, &distance, &volume, &pitch);
        if (isRbString(filePath) && isRbObject(position) && isRbFloat(distance) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            wrapVector3* tmp__position; Data_Get_Struct(position, wrapVector3, tmp__position);lnVector3& _position = tmp__position->Value;
            float _distance = (float)(NUM2DBL(distance));;
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            lnErrorCode errorCode = LNAudio_PlaySE3D(_filePath, &_position, _distance, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (5 <= argc && argc <= 7) {
        VALUE filePath;
        VALUE x;
        VALUE y;
        VALUE z;
        VALUE distance;
        VALUE volume;
        VALUE pitch;
        rb_scan_args(argc, argv, "52", &filePath, &x, &y, &z, &distance, &volume, &pitch);
        if (isRbString(filePath) && isRbFloat(x) && isRbFloat(y) && isRbFloat(z) && isRbFloat(distance) && isRbNumber(volume) && isRbNumber(pitch)) {
            char* _filePath = StringValuePtr(filePath);
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            float _distance = (float)(NUM2DBL(distance));;
            int _volume = (volume != Qnil) ? FIX2INT(volume) : 100;
            int _pitch = (pitch != Qnil) ? FIX2INT(pitch) : 100;
            lnErrorCode errorCode = LNAudio_PlaySE3DXYZ(_filePath, _x, _y, _z, _distance, _volume, _pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.play_se3d - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_StopSE(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            LNAudio_StopSE();
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.stop_se - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_SetMetreUnitDistance(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE distance;
        rb_scan_args(argc, argv, "1", &distance);
        if (isRbFloat(distance)) {
            float _distance = (float)(NUM2DBL(distance));;
            lnErrorCode errorCode = LNAudio_SetMetreUnitDistance(_distance);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.set_metre_unit_distance - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_SetBGMVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbNumber(volume) && isRbNumber(fadeTime)) {
            int _volume = FIX2INT(volume);
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_SetBGMVolume(_volume, _fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.set_bgm_volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_SetBGSVolume(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 2) {
        VALUE volume;
        VALUE fadeTime;
        rb_scan_args(argc, argv, "11", &volume, &fadeTime);
        if (isRbNumber(volume) && isRbNumber(fadeTime)) {
            int _volume = FIX2INT(volume);
            int _fadeTime = (fadeTime != Qnil) ? FIX2INT(fadeTime) : 0;
            lnErrorCode errorCode = LNAudio_SetBGSVolume(_volume, _fadeTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.set_bgs_volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNAudio_GetInternalGameSound(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE type;
        rb_scan_args(argc, argv, "1", &type);
        if (isRbNumber(type)) {
            lnInternalGameSound _type = (lnInternalGameSound)FIX2INT(type);
            LNAudio_GetInternalGameSound(_type);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Audio.get_internal_game_sound - wrong argument type.");
    return Qnil;
}

static void LNSoundListener_delete(wrapSoundListener* obj)
{
    
    free(obj);
}

static void LNSoundListener_mark(wrapSoundListener* obj)
{

}

static VALUE LNSoundListener_allocate( VALUE klass )
{
    VALUE obj;
    wrapSoundListener* internalObj;

    internalObj = (wrapSoundListener*)malloc(sizeof(wrapSoundListener));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSoundListener_allocate" );
    obj = Data_Wrap_Struct(klass, LNSoundListener_mark, LNSoundListener_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSoundListener));

    return obj;
}

static VALUE lnrbLNSoundListener_SetPosition(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            wrapVector3* tmp__position; Data_Get_Struct(position, wrapVector3, tmp__position);lnVector3& _position = tmp__position->Value;
            lnErrorCode errorCode = LNSoundListener_SetPosition(&_position);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            LNSoundListener_SetPositionXYZ(_x, _y, _z);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SoundListener.set_position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSoundListener_SetDirection(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE direction;
        rb_scan_args(argc, argv, "1", &direction);
        if (isRbObject(direction)) {
            wrapVector3* tmp__direction; Data_Get_Struct(direction, wrapVector3, tmp__direction);lnVector3& _direction = tmp__direction->Value;
            lnErrorCode errorCode = LNSoundListener_SetDirection(&_direction);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            LNSoundListener_SetDirectionXYZ(_x, _y, _z);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SoundListener.set_direction - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSoundListener_SetUpDirection(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE direction;
        rb_scan_args(argc, argv, "1", &direction);
        if (isRbObject(direction)) {
            wrapVector3* tmp__direction; Data_Get_Struct(direction, wrapVector3, tmp__direction);lnVector3& _direction = tmp__direction->Value;
            lnErrorCode errorCode = LNSoundListener_SetUpDirection(&_direction);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            LNSoundListener_SetUpDirectionXYZ(_x, _y, _z);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SoundListener.set_up_direction - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSoundListener_Velocity(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE velocity;
        rb_scan_args(argc, argv, "1", &velocity);
        if (isRbObject(velocity)) {
            wrapVector3* tmp__velocity; Data_Get_Struct(velocity, wrapVector3, tmp__velocity);lnVector3& _velocity = tmp__velocity->Value;
            lnErrorCode errorCode = LNSoundListener_Velocity(&_velocity);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            LNSoundListener_VelocityXYZ(_x, _y, _z);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SoundListener.velocity - wrong argument type.");
    return Qnil;
}

static void LNSound_delete(wrapSound* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNSound_mark(wrapSound* obj)
{

}

static VALUE LNSound_allocate( VALUE klass )
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = (wrapSound*)malloc(sizeof(wrapSound));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSound));

    return obj;
}

static VALUE LNSound_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapSound* internalObj;

    internalObj = (wrapSound*)malloc(sizeof(wrapSound));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSound_allocate" );
    obj = Data_Wrap_Struct(klass, LNSound_mark, LNSound_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSound));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSound_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 3) {
        VALUE filePath;
        VALUE enable3d;
        VALUE playType;
        rb_scan_args(argc, argv, "12", &filePath, &enable3d, &playType);
        if (isRbString(filePath) && isRbBool(enable3d) && isRbNumber(playType)) {
            char* _filePath = StringValuePtr(filePath);
            lnBool _enable3d = (enable3d != Qnil) ? RbBooltoBool(enable3d) : LN_FALSE;
            lnSoundPlayingType _playType = (playType != Qnil) ? (lnSoundPlayingType)FIX2INT(playType) : LN_SOUNDPLAYINGTYPE_AUTO;
            lnErrorCode errorCode = LNSound_Create(&selfObj->Handle, _filePath, _enable3d, _playType);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE volume;
        rb_scan_args(argc, argv, "1", &volume);
        if (isRbNumber(volume)) {
            int _volume = FIX2INT(volume);
            lnErrorCode errorCode = LNSound_SetVolume(selfObj->Handle, _volume);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.volume= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _volume;
            lnErrorCode errorCode = LNSound_GetVolume(selfObj->Handle, &_volume);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_volume);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE pitch;
        rb_scan_args(argc, argv, "1", &pitch);
        if (isRbNumber(pitch)) {
            int _pitch = FIX2INT(pitch);
            lnErrorCode errorCode = LNSound_SetPitch(selfObj->Handle, _pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.pitch= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPitch(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _pitch;
            lnErrorCode errorCode = LNSound_GetPitch(selfObj->Handle, &_pitch);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_pitch);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.pitch - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetLoopState(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 3) {
        VALUE loopEnable;
        VALUE begin;
        VALUE length;
        rb_scan_args(argc, argv, "12", &loopEnable, &begin, &length);
        if (isRbBool(loopEnable) && isRbNumber(begin) && isRbNumber(length)) {
            lnBool _loopEnable = RbBooltoBool(loopEnable);
            int _begin = (begin != Qnil) ? FIX2INT(begin) : 0;
            int _length = (length != Qnil) ? FIX2INT(length) : 0;
            lnErrorCode errorCode = LNSound_SetLoopState(selfObj->Handle, _loopEnable, _begin, _length);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.set_loop_state - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_IsLoop(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _enabled;
            lnErrorCode errorCode = LNSound_IsLoop(selfObj->Handle, &_enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_enabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.loop? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetPlayState(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnSoundPlayingState _state;
            lnErrorCode errorCode = LNSound_GetPlayState(selfObj->Handle, &_state);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return INT2FIX(_state);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.play_state - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Play(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNSound_Play(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.play - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Stop(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNSound_Stop(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.stop - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Pause(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE pause;
        rb_scan_args(argc, argv, "1", &pause);
        if (isRbBool(pause)) {
            lnBool _pause = RbBooltoBool(pause);
            lnErrorCode errorCode = LNSound_Pause(selfObj->Handle, _pause);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.pause - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_FadeVolume(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE targetVolume;
        VALUE time;
        VALUE fadeState;
        rb_scan_args(argc, argv, "3", &targetVolume, &time, &fadeState);
        if (isRbNumber(targetVolume) && isRbNumber(time) && isRbNumber(fadeState)) {
            int _targetVolume = FIX2INT(targetVolume);
            int _time = FIX2INT(time);
            lnSoundFadeState _fadeState = (lnSoundFadeState)FIX2INT(fadeState);
            lnErrorCode errorCode = LNSound_FadeVolume(selfObj->Handle, _targetVolume, _time, _fadeState);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.fade_volume - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetUnitsPlayed(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _samples;
            lnErrorCode errorCode = LNSound_GetUnitsPlayed(selfObj->Handle, &_samples);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_samples);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.units_played - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetTotalUnits(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _samples;
            lnErrorCode errorCode = LNSound_GetTotalUnits(selfObj->Handle, &_samples);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_samples);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.total_units - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_GetUnitsParSecond(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _frequency;
            lnErrorCode errorCode = LNSound_GetUnitsParSecond(selfObj->Handle, &_frequency);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_frequency);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.units_par_second - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_Is3DSound(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _enabled;
            lnErrorCode errorCode = LNSound_Is3DSound(selfObj->Handle, &_enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_enabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.is_3d_sound? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            wrapVector3* tmp__position; Data_Get_Struct(position, wrapVector3, tmp__position);lnVector3& _position = tmp__position->Value;
            lnErrorCode errorCode = LNSound_SetEmitterPosition(selfObj->Handle, &_position);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSound_SetEmitterPositionXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.emitter_position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterVelocity(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE velocity;
        rb_scan_args(argc, argv, "1", &velocity);
        if (isRbObject(velocity)) {
            wrapVector3* tmp__velocity; Data_Get_Struct(velocity, wrapVector3, tmp__velocity);lnVector3& _velocity = tmp__velocity->Value;
            lnErrorCode errorCode = LNSound_SetEmitterVelocity(selfObj->Handle, &_velocity);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSound_SetEmitterVelocityXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.emitter_velocity= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSound_SetEmitterDistance(int argc, VALUE *argv, VALUE self)
{
    wrapSound* selfObj;
    Data_Get_Struct(self, wrapSound, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE distance;
        rb_scan_args(argc, argv, "1", &distance);
        if (isRbFloat(distance)) {
            float _distance = (float)(NUM2DBL(distance));;
            lnErrorCode errorCode = LNSound_SetEmitterDistance(selfObj->Handle, _distance);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sound.emitter_distance= - wrong argument type.");
    return Qnil;
}

static void LNColor_delete(wrapColor* obj)
{
    
    free(obj);
}

static void LNColor_mark(wrapColor* obj)
{

}

static VALUE LNColor_allocate( VALUE klass )
{
    VALUE obj;
    wrapColor* internalObj;

    internalObj = (wrapColor*)malloc(sizeof(wrapColor));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNColor_allocate" );
    obj = Data_Wrap_Struct(klass, LNColor_mark, LNColor_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapColor));

    return obj;
}

static VALUE LNColor_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE R_;
    VALUE G_;
    VALUE B_;
    VALUE A_;
    rb_scan_args(argc, argv, "4", &R_, &G_, &B_, &A_);
    selfObj->Value.R = NUM2DBL(R_);
    selfObj->Value.G = NUM2DBL(G_);
    selfObj->Value.B = NUM2DBL(B_);
    selfObj->Value.A = NUM2DBL(A_);
    return self;
}

static VALUE LNColor_R_set( VALUE self, VALUE v )
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    selfObj->Value.R = NUM2DBL(v);
    return self;
}

static VALUE LNColor_R_get( VALUE self)
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    return toVALUE(selfObj->Value.R);
}

static VALUE LNColor_G_set( VALUE self, VALUE v )
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    selfObj->Value.G = NUM2DBL(v);
    return self;
}

static VALUE LNColor_G_get( VALUE self)
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    return toVALUE(selfObj->Value.G);
}

static VALUE LNColor_B_set( VALUE self, VALUE v )
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    selfObj->Value.B = NUM2DBL(v);
    return self;
}

static VALUE LNColor_B_get( VALUE self)
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    return toVALUE(selfObj->Value.B);
}

static VALUE LNColor_A_set( VALUE self, VALUE v )
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    selfObj->Value.A = NUM2DBL(v);
    return self;
}

static VALUE LNColor_A_get( VALUE self)
{
    wrapColor* selfObj;
    Data_Get_Struct(self, wrapColor, selfObj);
    return toVALUE(selfObj->Value.A);
}

static void LNColorF_delete(wrapColorF* obj)
{
    
    free(obj);
}

static void LNColorF_mark(wrapColorF* obj)
{

}

static VALUE LNColorF_allocate( VALUE klass )
{
    VALUE obj;
    wrapColorF* internalObj;

    internalObj = (wrapColorF*)malloc(sizeof(wrapColorF));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNColorF_allocate" );
    obj = Data_Wrap_Struct(klass, LNColorF_mark, LNColorF_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapColorF));

    return obj;
}

static VALUE LNColorF_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE R_;
    VALUE G_;
    VALUE B_;
    VALUE A_;
    rb_scan_args(argc, argv, "4", &R_, &G_, &B_, &A_);
    selfObj->Value.R = NUM2DBL(R_);
    selfObj->Value.G = NUM2DBL(G_);
    selfObj->Value.B = NUM2DBL(B_);
    selfObj->Value.A = NUM2DBL(A_);
    return self;
}

static VALUE LNColorF_R_set( VALUE self, VALUE v )
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    selfObj->Value.R = NUM2DBL(v);
    return self;
}

static VALUE LNColorF_R_get( VALUE self)
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    return toVALUE(selfObj->Value.R);
}

static VALUE LNColorF_G_set( VALUE self, VALUE v )
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    selfObj->Value.G = NUM2DBL(v);
    return self;
}

static VALUE LNColorF_G_get( VALUE self)
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    return toVALUE(selfObj->Value.G);
}

static VALUE LNColorF_B_set( VALUE self, VALUE v )
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    selfObj->Value.B = NUM2DBL(v);
    return self;
}

static VALUE LNColorF_B_get( VALUE self)
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    return toVALUE(selfObj->Value.B);
}

static VALUE LNColorF_A_set( VALUE self, VALUE v )
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    selfObj->Value.A = NUM2DBL(v);
    return self;
}

static VALUE LNColorF_A_get( VALUE self)
{
    wrapColorF* selfObj;
    Data_Get_Struct(self, wrapColorF, selfObj);
    return toVALUE(selfObj->Value.A);
}

static void LNTone_delete(wrapTone* obj)
{
    
    free(obj);
}

static void LNTone_mark(wrapTone* obj)
{

}

static VALUE LNTone_allocate( VALUE klass )
{
    VALUE obj;
    wrapTone* internalObj;

    internalObj = (wrapTone*)malloc(sizeof(wrapTone));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTone_allocate" );
    obj = Data_Wrap_Struct(klass, LNTone_mark, LNTone_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTone));

    return obj;
}

static VALUE LNTone_struct_initialize( int argc, VALUE *argv, VALUE self )
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    if (argc == 0) {
        return self;
    }
    VALUE R_;
    VALUE G_;
    VALUE B_;
    VALUE Mono_;
    rb_scan_args(argc, argv, "4", &R_, &G_, &B_, &Mono_);
    selfObj->Value.R = NUM2DBL(R_);
    selfObj->Value.G = NUM2DBL(G_);
    selfObj->Value.B = NUM2DBL(B_);
    selfObj->Value.Mono = NUM2DBL(Mono_);
    return self;
}

static VALUE LNTone_R_set( VALUE self, VALUE v )
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    selfObj->Value.R = NUM2DBL(v);
    return self;
}

static VALUE LNTone_R_get( VALUE self)
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    return toVALUE(selfObj->Value.R);
}

static VALUE LNTone_G_set( VALUE self, VALUE v )
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    selfObj->Value.G = NUM2DBL(v);
    return self;
}

static VALUE LNTone_G_get( VALUE self)
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    return toVALUE(selfObj->Value.G);
}

static VALUE LNTone_B_set( VALUE self, VALUE v )
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    selfObj->Value.B = NUM2DBL(v);
    return self;
}

static VALUE LNTone_B_get( VALUE self)
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    return toVALUE(selfObj->Value.B);
}

static VALUE LNTone_Mono_set( VALUE self, VALUE v )
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    selfObj->Value.Mono = NUM2DBL(v);
    return self;
}

static VALUE LNTone_Mono_get( VALUE self)
{
    wrapTone* selfObj;
    Data_Get_Struct(self, wrapTone, selfObj);
    return toVALUE(selfObj->Value.Mono);
}

static void LNTexture_delete(wrapTexture* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNTexture_mark(wrapTexture* obj)
{

}

static VALUE LNTexture_allocate( VALUE klass )
{
    VALUE obj;
    wrapTexture* internalObj;

    internalObj = (wrapTexture*)malloc(sizeof(wrapTexture));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTexture_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture_mark, LNTexture_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTexture));

    return obj;
}

static VALUE LNTexture_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapTexture* internalObj;

    internalObj = (wrapTexture*)malloc(sizeof(wrapTexture));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTexture_allocate" );
    obj = Data_Wrap_Struct(klass, LNTexture_mark, LNTexture_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTexture));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNTexture_Create(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (isRbNumber(width) && isRbNumber(height)) {
            int _width = FIX2INT(width);
            int _height = FIX2INT(height);
            lnErrorCode errorCode = LNTexture_Create(&selfObj->Handle, _width, _height);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            lnErrorCode errorCode = LNTexture_CreateFromFile(&selfObj->Handle, _filePath);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (4 <= argc && argc <= 4) {
        VALUE width;
        VALUE height;
        VALUE levels;
        VALUE format;
        rb_scan_args(argc, argv, "4", &width, &height, &levels, &format);
        if (isRbNumber(width) && isRbNumber(height) && isRbNumber(levels) && isRbNumber(format)) {
            int _width = FIX2INT(width);
            int _height = FIX2INT(height);
            int _levels = FIX2INT(levels);
            lnTextureFormat _format = (lnTextureFormat)FIX2INT(format);
            lnErrorCode errorCode = LNTexture_CreateEx(&selfObj->Handle, _width, _height, _levels, _format);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (4 <= argc && argc <= 4) {
        VALUE filePath;
        VALUE colorKey;
        VALUE levels;
        VALUE format;
        rb_scan_args(argc, argv, "4", &filePath, &colorKey, &levels, &format);
        if (isRbString(filePath) && isRbNumber(colorKey) && isRbNumber(levels) && isRbNumber(format)) {
            char* _filePath = StringValuePtr(filePath);
            lnU32 _colorKey = FIX2INT(colorKey);
            int _levels = FIX2INT(levels);
            lnTextureFormat _format = (lnTextureFormat)FIX2INT(format);
            lnErrorCode errorCode = LNTexture_CreateFromFileEx(&selfObj->Handle, _filePath, _colorKey, _levels, _format);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_GetWidth(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _width;
            lnErrorCode errorCode = LNTexture_GetWidth(selfObj->Handle, &_width);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_width);    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.width - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_GetHeight(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _height;
            lnErrorCode errorCode = LNTexture_GetHeight(selfObj->Handle, &_height);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_height);    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.height - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_GetRealWidth(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _width;
            lnErrorCode errorCode = LNTexture_GetRealWidth(selfObj->Handle, &_width);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_width);    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.real_width - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_GetRealHeight(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _height;
            lnErrorCode errorCode = LNTexture_GetRealHeight(selfObj->Handle, &_height);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_height);    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.real_height - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_Clear(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE color;
        rb_scan_args(argc, argv, "1", &color);
        if (isRbObject(color)) {
            wrapColor* tmp__color; Data_Get_Struct(color, wrapColor, tmp__color);lnColor& _color = tmp__color->Value;
            lnErrorCode errorCode = LNTexture_Clear(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.clear - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_ClearRGBA(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (4 <= argc && argc <= 4) {
        VALUE r;
        VALUE g;
        VALUE b;
        VALUE a;
        rb_scan_args(argc, argv, "4", &r, &g, &b, &a);
        if (isRbNumber(r) && isRbNumber(g) && isRbNumber(b) && isRbNumber(a)) {
            int _r = FIX2INT(r);
            int _g = FIX2INT(g);
            int _b = FIX2INT(b);
            int _a = FIX2INT(a);
            lnErrorCode errorCode = LNTexture_ClearRGBA(selfObj->Handle, _r, _g, _b, _a);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.clear_rgba - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_DrawText(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (2 <= argc && argc <= 5) {
        VALUE text;
        VALUE rect;
        VALUE font;
        VALUE length;
        VALUE align;
        rb_scan_args(argc, argv, "23", &text, &rect, &font, &length, &align);
        if (isRbString(text) && isRbObject(rect) && isRbObject(font) && isRbNumber(length) && isRbNumber(align)) {
            char* _text = StringValuePtr(text);
            wrapRect* tmp__rect; Data_Get_Struct(rect, wrapRect, tmp__rect);lnRect& _rect = tmp__rect->Value;
            lnHandle _font = (font != Qnil) ? RbRefObjToHandle(font) : NULL;
            int _length = (length != Qnil) ? FIX2INT(length) : -1;
            lnTextAlign _align = (align != Qnil) ? (lnTextAlign)FIX2INT(align) : LN_TEXTALIGN_LEFT;
            lnErrorCode errorCode = LNTexture_DrawText(selfObj->Handle, _text, &_rect, _font, _length, _align);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 6) {
        VALUE text;
        VALUE x;
        VALUE y;
        VALUE font;
        VALUE length;
        VALUE align;
        rb_scan_args(argc, argv, "33", &text, &x, &y, &font, &length, &align);
        if (isRbString(text) && isRbNumber(x) && isRbNumber(y) && isRbObject(font) && isRbNumber(length) && isRbNumber(align)) {
            char* _text = StringValuePtr(text);
            int _x = FIX2INT(x);
            int _y = FIX2INT(y);
            lnHandle _font = (font != Qnil) ? RbRefObjToHandle(font) : NULL;
            int _length = (length != Qnil) ? FIX2INT(length) : -1;
            lnTextAlign _align = (align != Qnil) ? (lnTextAlign)FIX2INT(align) : LN_TEXTALIGN_LEFT;
            lnErrorCode errorCode = LNTexture_DrawTextXY(selfObj->Handle, _text, _x, _y, _font, _length, _align);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.draw_text - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTexture_BitBlt(int argc, VALUE *argv, VALUE self)
{
    wrapTexture* selfObj;
    Data_Get_Struct(self, wrapTexture, selfObj);

    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE srcTexture;
        VALUE srcRect;
        rb_scan_args(argc, argv, "4", &x, &y, &srcTexture, &srcRect);
        if (isRbNumber(x) && isRbNumber(y) && isRbObject(srcTexture) && isRbObject(srcRect)) {
            int _x = FIX2INT(x);
            int _y = FIX2INT(y);
            lnHandle _srcTexture = RbRefObjToHandle(srcTexture);
            wrapRect* tmp__srcRect; Data_Get_Struct(srcRect, wrapRect, tmp__srcRect);lnRect& _srcRect = tmp__srcRect->Value;
            lnErrorCode errorCode = LNTexture_BitBlt(selfObj->Handle, _x, _y, _srcTexture, &_srcRect);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Texture.bit_blt - wrong argument type.");
    return Qnil;
}

static void LNFont_delete(wrapFont* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNFont_mark(wrapFont* obj)
{

}

static VALUE LNFont_allocate( VALUE klass )
{
    VALUE obj;
    wrapFont* internalObj;

    internalObj = (wrapFont*)malloc(sizeof(wrapFont));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNFont_allocate" );
    obj = Data_Wrap_Struct(klass, LNFont_mark, LNFont_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapFont));

    return obj;
}

static VALUE LNFont_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapFont* internalObj;

    internalObj = (wrapFont*)malloc(sizeof(wrapFont));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNFont_allocate" );
    obj = Data_Wrap_Struct(klass, LNFont_mark, LNFont_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapFont));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNFont_Create(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNFont_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            lnErrorCode errorCode = LNFont_CreateBitmapFontFromFile(&selfObj->Handle, _filePath);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (1 <= argc && argc <= 1) {
        VALUE srcFont;
        rb_scan_args(argc, argv, "1", &srcFont);
        if (isRbObject(srcFont)) {
            lnHandle _srcFont = RbRefObjToHandle(srcFont);
            lnErrorCode errorCode = LNFont_CreateCopy(&selfObj->Handle, _srcFont);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetDefaultFont(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE font;
        rb_scan_args(argc, argv, "1", &font);
        if (isRbObject(font)) {
            lnHandle _font = RbRefObjToHandle(font);
            lnErrorCode errorCode = LNFont_SetDefaultFont(_font);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.set_default_font - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetName(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE fontName;
        rb_scan_args(argc, argv, "1", &fontName);
        if (isRbString(fontName)) {
            char* _fontName = StringValuePtr(fontName);
            lnErrorCode errorCode = LNFont_SetName(selfObj->Handle, _fontName);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.name= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetName(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            const lnChar* _fontName;
            lnErrorCode errorCode = LNFont_GetName(selfObj->Handle, &_fontName);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_fontName);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.name - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetSize(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE size;
        rb_scan_args(argc, argv, "1", &size);
        if (isRbNumber(size)) {
            int _size = FIX2INT(size);
            lnErrorCode errorCode = LNFont_SetSize(selfObj->Handle, _size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.size= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetSize(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _size;
            lnErrorCode errorCode = LNFont_GetSize(selfObj->Handle, &_size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_size);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetColor(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE color;
        rb_scan_args(argc, argv, "1", &color);
        if (isRbObject(color)) {
            wrapColor* tmp__color; Data_Get_Struct(color, wrapColor, tmp__color);lnColor& _color = tmp__color->Value;
            lnErrorCode errorCode = LNFont_SetColor(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.color= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetColor(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnColor _color;
            lnErrorCode errorCode = LNFont_GetColor(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNColor_allocate(gColorClass);
            ((wrapColor*)DATA_PTR(retObj))->Value = _color;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Font.color - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetEdgeColor(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE color;
        rb_scan_args(argc, argv, "1", &color);
        if (isRbObject(color)) {
            wrapColor* tmp__color; Data_Get_Struct(color, wrapColor, tmp__color);lnColor& _color = tmp__color->Value;
            lnErrorCode errorCode = LNFont_SetEdgeColor(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.edge_color= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetEdgeColor(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnColor _color;
            lnErrorCode errorCode = LNFont_GetEdgeColor(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNColor_allocate(gColorClass);
            ((wrapColor*)DATA_PTR(retObj))->Value = _color;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Font.edge_color - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetEdgeSize(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE size;
        rb_scan_args(argc, argv, "1", &size);
        if (isRbNumber(size)) {
            int _size = FIX2INT(size);
            lnErrorCode errorCode = LNFont_SetEdgeSize(selfObj->Handle, _size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.edge_size= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetEdgeSize(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _size;
            lnErrorCode errorCode = LNFont_GetEdgeSize(selfObj->Handle, &_size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_size);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.edge_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetBold(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNFont_SetBold(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.bold= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_IsBold(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _enabled;
            lnErrorCode errorCode = LNFont_IsBold(selfObj->Handle, &_enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_enabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.bold? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetItalic(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNFont_SetItalic(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.italic= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_IsItalic(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _enabled;
            lnErrorCode errorCode = LNFont_IsItalic(selfObj->Handle, &_enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_enabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.italic? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_SetAntiAlias(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNFont_SetAntiAlias(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Font.anti_alias= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_IsAntiAlias(int argc, VALUE *argv, VALUE self)
{
    wrapFont* selfObj;
    Data_Get_Struct(self, wrapFont, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _enabled;
            lnErrorCode errorCode = LNFont_IsAntiAlias(selfObj->Handle, &_enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_enabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Font.anti_alias? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNFont_GetTextSize(int argc, VALUE *argv, VALUE self)
{
    if (3 <= argc && argc <= 3) {
        VALUE font;
        VALUE text;
        VALUE length;
        rb_scan_args(argc, argv, "3", &font, &text, &length);
        if (isRbObject(font) && isRbString(text) && isRbNumber(length)) {
            lnHandle _font = RbRefObjToHandle(font);
            char* _text = StringValuePtr(text);
            int _length = FIX2INT(length);
            lnSize _outSize;
            lnErrorCode errorCode = LNFont_GetTextSize(_font, _text, _length, &_outSize);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNSize_allocate(gSizeClass);
            ((wrapSize*)DATA_PTR(retObj))->Value = _outSize;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Font.get_text_size - wrong argument type.");
    return Qnil;
}

static void LNSceneGraph_delete(wrapSceneGraph* obj)
{
    
    free(obj);
}

static void LNSceneGraph_mark(wrapSceneGraph* obj)
{

}

static VALUE LNSceneGraph_allocate( VALUE klass )
{
    VALUE obj;
    wrapSceneGraph* internalObj;

    internalObj = (wrapSceneGraph*)malloc(sizeof(wrapSceneGraph));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSceneGraph_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneGraph_mark, LNSceneGraph_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSceneGraph));

    return obj;
}

static void LNSceneNode_delete(wrapSceneNode* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNSceneNode_mark(wrapSceneNode* obj)
{

}

static VALUE LNSceneNode_allocate( VALUE klass )
{
    VALUE obj;
    wrapSceneNode* internalObj;

    internalObj = (wrapSceneNode*)malloc(sizeof(wrapSceneNode));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSceneNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNode_mark, LNSceneNode_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSceneNode));

    return obj;
}

static VALUE LNSceneNode_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapSceneNode* internalObj;

    internalObj = (wrapSceneNode*)malloc(sizeof(wrapSceneNode));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSceneNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNSceneNode_mark, LNSceneNode_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSceneNode));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSceneNode_GetName(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            const lnChar* _outName;
            lnErrorCode errorCode = LNSceneNode_GetName(selfObj->Handle, &_outName);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outName);    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.name - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetName(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE name;
        rb_scan_args(argc, argv, "1", &name);
        if (isRbString(name)) {
            char* _name = StringValuePtr(name);
            lnErrorCode errorCode = LNSceneNode_SetName(selfObj->Handle, _name);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.name= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnErrorCode errorCode = LNSceneNode_SetPosition(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSceneNode_SetPositionXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _vec;
            lnErrorCode errorCode = LNSceneNode_GetPosition(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _vec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.position - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetAngle(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnErrorCode errorCode = LNSceneNode_SetAngle(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSceneNode_SetAngleXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.angle= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetAngle(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _vec;
            lnErrorCode errorCode = LNSceneNode_GetAngle(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _vec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.angle - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetScale(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnErrorCode errorCode = LNSceneNode_SetScale(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSceneNode_SetScaleXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.scale= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetScale(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _vec;
            lnErrorCode errorCode = LNSceneNode_GetScale(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _vec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.scale - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetCenter(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE vec;
        rb_scan_args(argc, argv, "1", &vec);
        if (isRbObject(vec)) {
            wrapVector3* tmp__vec; Data_Get_Struct(vec, wrapVector3, tmp__vec);lnVector3& _vec = tmp__vec->Value;
            lnErrorCode errorCode = LNSceneNode_SetCenter(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNSceneNode_SetCenterXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.center= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetCenter(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnVector3 _vec;
            lnErrorCode errorCode = LNSceneNode_GetCenter(selfObj->Handle, &_vec);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNVector3_allocate(gVector3Class);
            ((wrapVector3*)DATA_PTR(retObj))->Value = _vec;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.center - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE mat;
        rb_scan_args(argc, argv, "1", &mat);
        if (isRbObject(mat)) {
            wrapMatrix* tmp__mat; Data_Get_Struct(mat, wrapMatrix, tmp__mat);lnMatrix& _mat = tmp__mat->Value;
            lnErrorCode errorCode = LNSceneNode_SetMatrix(selfObj->Handle, &_mat);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.matrix= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _mat;
            lnErrorCode errorCode = LNSceneNode_GetMatrix(selfObj->Handle, &_mat);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _mat;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.matrix - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetRotateOrder(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE rotOrder;
        rb_scan_args(argc, argv, "1", &rotOrder);
        if (isRbNumber(rotOrder)) {
            lnRotationOrder _rotOrder = (lnRotationOrder)FIX2INT(rotOrder);
            lnErrorCode errorCode = LNSceneNode_SetRotateOrder(selfObj->Handle, _rotOrder);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.rotate_order= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetRotateOrder(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnRotationOrder _outOrder;
            lnErrorCode errorCode = LNSceneNode_GetRotateOrder(selfObj->Handle, &_outOrder);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return INT2FIX(_outOrder);    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.rotate_order - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetPlacementType(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE type;
        rb_scan_args(argc, argv, "1", &type);
        if (isRbNumber(type)) {
            lnPlacementType _type = (lnPlacementType)FIX2INT(type);
            lnErrorCode errorCode = LNSceneNode_SetPlacementType(selfObj->Handle, _type);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.placement_type= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_GetPlacementType(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnPlacementType _outType;
            lnErrorCode errorCode = LNSceneNode_GetPlacementType(selfObj->Handle, &_outType);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return INT2FIX(_outType);    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.placement_type - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_AddChild(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE childSceneNode;
        rb_scan_args(argc, argv, "1", &childSceneNode);
        if (isRbObject(childSceneNode)) {
            lnHandle _childSceneNode = RbRefObjToHandle(childSceneNode);
            lnErrorCode errorCode = LNSceneNode_AddChild(selfObj->Handle, _childSceneNode);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.add_child - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_RemoveChild(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE childSceneNode;
        rb_scan_args(argc, argv, "1", &childSceneNode);
        if (isRbObject(childSceneNode)) {
            lnHandle _childSceneNode = RbRefObjToHandle(childSceneNode);
            lnErrorCode errorCode = LNSceneNode_RemoveChild(selfObj->Handle, _childSceneNode);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.remove_child - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_Update(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE elapsedTime;
        rb_scan_args(argc, argv, "1", &elapsedTime);
        if (isRbFloat(elapsedTime)) {
            float _elapsedTime = (float)(NUM2DBL(elapsedTime));;
            lnErrorCode errorCode = LNSceneNode_Update(selfObj->Handle, _elapsedTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.update - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_SetEnableAutoUpdate(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNSceneNode_SetEnableAutoUpdate(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.enable_auto_update= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSceneNode_IsEnableAutoUpdate(int argc, VALUE *argv, VALUE self)
{
    wrapSceneNode* selfObj;
    Data_Get_Struct(self, wrapSceneNode, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _outEnabled;
            lnErrorCode errorCode = LNSceneNode_IsEnableAutoUpdate(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outEnabled);    }
    }
    rb_raise(rb_eArgError, "LNote::SceneNode.enable_auto_update? - wrong argument type.");
    return Qnil;
}

static void LNVisualNode_delete(wrapVisualNode* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNVisualNode_mark(wrapVisualNode* obj)
{
    rb_gc_mark(obj->Shader);

}

static VALUE LNVisualNode_allocate( VALUE klass )
{
    VALUE obj;
    wrapVisualNode* internalObj;

    internalObj = (wrapVisualNode*)malloc(sizeof(wrapVisualNode));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVisualNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNVisualNode_mark, LNVisualNode_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVisualNode));
    internalObj->Shader = Qnil;

    return obj;
}

static VALUE LNVisualNode_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapVisualNode* internalObj;

    internalObj = (wrapVisualNode*)malloc(sizeof(wrapVisualNode));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNVisualNode_allocate" );
    obj = Data_Wrap_Struct(klass, LNVisualNode_mark, LNVisualNode_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapVisualNode));
    internalObj->Shader = Qnil;

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNVisualNode_SetVisible(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNVisualNode_SetVisible(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.visible= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetOpacity(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE opacity;
        rb_scan_args(argc, argv, "1", &opacity);
        if (isRbFloat(opacity)) {
            float _opacity = (float)(NUM2DBL(opacity));;
            lnErrorCode errorCode = LNVisualNode_SetOpacity(selfObj->Handle, _opacity);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.opacity= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetColorScale(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE materialIndex;
        VALUE color;
        rb_scan_args(argc, argv, "2", &materialIndex, &color);
        if (isRbNumber(materialIndex) && isRbObject(color)) {
            int _materialIndex = FIX2INT(materialIndex);
            wrapColorF* tmp__color; Data_Get_Struct(color, wrapColorF, tmp__color);lnColorF& _color = tmp__color->Value;
            lnErrorCode errorCode = LNVisualNode_SetColorScale(selfObj->Handle, _materialIndex, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.set_color_scale - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetBlendColor(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE materialIndex;
        VALUE color;
        rb_scan_args(argc, argv, "2", &materialIndex, &color);
        if (isRbNumber(materialIndex) && isRbObject(color)) {
            int _materialIndex = FIX2INT(materialIndex);
            wrapColorF* tmp__color; Data_Get_Struct(color, wrapColorF, tmp__color);lnColorF& _color = tmp__color->Value;
            lnErrorCode errorCode = LNVisualNode_SetBlendColor(selfObj->Handle, _materialIndex, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.set_blend_color - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetTone(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE materialIndex;
        VALUE tone;
        rb_scan_args(argc, argv, "2", &materialIndex, &tone);
        if (isRbNumber(materialIndex) && isRbObject(tone)) {
            int _materialIndex = FIX2INT(materialIndex);
            wrapTone* tmp__tone; Data_Get_Struct(tone, wrapTone, tmp__tone);lnTone& _tone = tmp__tone->Value;
            lnErrorCode errorCode = LNVisualNode_SetTone(selfObj->Handle, _materialIndex, &_tone);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.set_tone - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetUVTransform(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE materialIndex;
        VALUE matrix;
        rb_scan_args(argc, argv, "2", &materialIndex, &matrix);
        if (isRbNumber(materialIndex) && isRbObject(matrix)) {
            int _materialIndex = FIX2INT(materialIndex);
            wrapMatrix* tmp__matrix; Data_Get_Struct(matrix, wrapMatrix, tmp__matrix);lnMatrix& _matrix = tmp__matrix->Value;
            lnErrorCode errorCode = LNVisualNode_SetUVTransform(selfObj->Handle, _materialIndex, &_matrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.set_uv_transform - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetBlendMode(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            lnBlendMode _mode = (lnBlendMode)FIX2INT(mode);
            lnErrorCode errorCode = LNVisualNode_SetBlendMode(selfObj->Handle, _mode);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.blend_mode= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetCullingMode(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            lnCullingMode _mode = (lnCullingMode)FIX2INT(mode);
            lnErrorCode errorCode = LNVisualNode_SetCullingMode(selfObj->Handle, _mode);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.culling_mode= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetEnableDepthTest(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNVisualNode_SetEnableDepthTest(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.enable_depth_test= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetEnableDepthWrite(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNVisualNode_SetEnableDepthWrite(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.enable_depth_write= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNVisualNode_SetShader(int argc, VALUE *argv, VALUE self)
{
    wrapVisualNode* selfObj;
    Data_Get_Struct(self, wrapVisualNode, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE shader;
        rb_scan_args(argc, argv, "1", &shader);
        if (isRbObject(shader)) {
            lnHandle _shader = RbRefObjToHandle(shader);
            lnErrorCode errorCode = LNVisualNode_SetShader(selfObj->Handle, _shader);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            selfObj->Shader = shader;
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::VisualNode.shader= - wrong argument type.");
    return Qnil;
}

static void LNSprite_delete(wrapSprite* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNSprite_mark(wrapSprite* obj)
{
    rb_gc_mark(obj->Texture);

}

static VALUE LNSprite_allocate( VALUE klass )
{
    VALUE obj;
    wrapSprite* internalObj;

    internalObj = (wrapSprite*)malloc(sizeof(wrapSprite));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSprite_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite_mark, LNSprite_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSprite));
    internalObj->Texture = Qnil;

    return obj;
}

static VALUE LNSprite_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapSprite* internalObj;

    internalObj = (wrapSprite*)malloc(sizeof(wrapSprite));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSprite_allocate" );
    obj = Data_Wrap_Struct(klass, LNSprite_mark, LNSprite_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSprite));
    internalObj->Texture = Qnil;

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSprite_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (isRbObject(texture)) {
            lnHandle _texture = RbRefObjToHandle(texture);
            lnErrorCode errorCode = LNSprite_Create(&selfObj->Handle, _texture);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_SetTexture(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (isRbObject(texture)) {
            lnHandle _texture = RbRefObjToHandle(texture);
            lnErrorCode errorCode = LNSprite_SetTexture(selfObj->Handle, _texture);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            selfObj->Texture = texture;
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.texture= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_SetSize(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (isRbFloat(width) && isRbFloat(height)) {
            float _width = (float)(NUM2DBL(width));;
            float _height = (float)(NUM2DBL(height));;
            lnErrorCode errorCode = LNSprite_SetSize(selfObj->Handle, _width, _height);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.set_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_SetSrcRect(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE rect;
        rb_scan_args(argc, argv, "1", &rect);
        if (isRbObject(rect)) {
            wrapRect* tmp__rect; Data_Get_Struct(rect, wrapRect, tmp__rect);lnRect& _rect = tmp__rect->Value;
            lnErrorCode errorCode = LNSprite_SetSrcRect(selfObj->Handle, &_rect);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (4 <= argc && argc <= 4) {
        VALUE x;
        VALUE y;
        VALUE w;
        VALUE h;
        rb_scan_args(argc, argv, "4", &x, &y, &w, &h);
        if (isRbNumber(x) && isRbNumber(y) && isRbNumber(w) && isRbNumber(h)) {
            int _x = FIX2INT(x);
            int _y = FIX2INT(y);
            int _w = FIX2INT(w);
            int _h = FIX2INT(h);
            lnErrorCode errorCode = LNSprite_SetSrcRectXYWH(selfObj->Handle, _x, _y, _w, _h);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.src_rect= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_GetSrcRect(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnRect _rect;
            lnErrorCode errorCode = LNSprite_GetSrcRect(selfObj->Handle, &_rect);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNRect_allocate(gRectClass);
            ((wrapRect*)DATA_PTR(retObj))->Value = _rect;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.src_rect - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_SetFlipH(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNSprite_SetFlipH(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.flip_h= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_IsFlipH(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _outEnabled;
            lnErrorCode errorCode = LNSprite_IsFlipH(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outEnabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.flip_h? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_SetFlipV(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNSprite_SetFlipV(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.flip_v= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSprite_IsFlipV(int argc, VALUE *argv, VALUE self)
{
    wrapSprite* selfObj;
    Data_Get_Struct(self, wrapSprite, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _outEnabled;
            lnErrorCode errorCode = LNSprite_IsFlipV(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outEnabled);    }
    }
    rb_raise(rb_eArgError, "LNote::Sprite.flip_v? - wrong argument type.");
    return Qnil;
}

static void LNSpritePath_delete(wrapSpritePath* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNSpritePath_mark(wrapSpritePath* obj)
{

}

static VALUE LNSpritePath_allocate( VALUE klass )
{
    VALUE obj;
    wrapSpritePath* internalObj;

    internalObj = (wrapSpritePath*)malloc(sizeof(wrapSpritePath));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSpritePath_allocate" );
    obj = Data_Wrap_Struct(klass, LNSpritePath_mark, LNSpritePath_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSpritePath));

    return obj;
}

static VALUE LNSpritePath_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapSpritePath* internalObj;

    internalObj = (wrapSpritePath*)malloc(sizeof(wrapSpritePath));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNSpritePath_allocate" );
    obj = Data_Wrap_Struct(klass, LNSpritePath_mark, LNSpritePath_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapSpritePath));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNSpritePath_Create(int argc, VALUE *argv, VALUE self)
{
    wrapSpritePath* selfObj;
    Data_Get_Struct(self, wrapSpritePath, selfObj);

    if (1 <= argc && argc <= 2) {
        VALUE maxSpriteCount;
        VALUE coord;
        rb_scan_args(argc, argv, "11", &maxSpriteCount, &coord);
        if (isRbNumber(maxSpriteCount) && isRbNumber(coord)) {
            int _maxSpriteCount = FIX2INT(maxSpriteCount);
            lnCoord _coord = (coord != Qnil) ? (lnCoord)FIX2INT(coord) : LN_COORD_SPACE_2D;
            lnErrorCode errorCode = LNSpritePath_Create(&selfObj->Handle, _maxSpriteCount, _coord);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SpritePath.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSpritePath_SetPathBlendMode(int argc, VALUE *argv, VALUE self)
{
    wrapSpritePath* selfObj;
    Data_Get_Struct(self, wrapSpritePath, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE blendMode;
        rb_scan_args(argc, argv, "1", &blendMode);
        if (isRbNumber(blendMode)) {
            lnBlendMode _blendMode = (lnBlendMode)FIX2INT(blendMode);
            lnErrorCode errorCode = LNSpritePath_SetPathBlendMode(selfObj->Handle, _blendMode);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SpritePath.path_blend_mode= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSpritePath_SetPathTransform(int argc, VALUE *argv, VALUE self)
{
    wrapSpritePath* selfObj;
    Data_Get_Struct(self, wrapSpritePath, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE matrix;
        rb_scan_args(argc, argv, "1", &matrix);
        if (isRbObject(matrix)) {
            wrapMatrix* tmp__matrix; Data_Get_Struct(matrix, wrapMatrix, tmp__matrix);lnMatrix& _matrix = tmp__matrix->Value;
            lnErrorCode errorCode = LNSpritePath_SetPathTransform(selfObj->Handle, &_matrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SpritePath.path_transform= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNSpritePath_DrawSprite(int argc, VALUE *argv, VALUE self)
{
    wrapSpritePath* selfObj;
    Data_Get_Struct(self, wrapSpritePath, selfObj);

    if (6 <= argc && argc <= 7) {
        VALUE position;
        VALUE center;
        VALUE size;
        VALUE texture;
        VALUE srcRect;
        VALUE vertexColors;
        VALUE frontAxis;
        rb_scan_args(argc, argv, "61", &position, &center, &size, &texture, &srcRect, &vertexColors, &frontAxis);
        if (isRbObject(position) && isRbObject(center) && isRbObject(size) && isRbObject(texture) && isRbObject(srcRect) && isRbObject(vertexColors) && isRbNumber(frontAxis)) {
            wrapVector3* tmp__position; Data_Get_Struct(position, wrapVector3, tmp__position);lnVector3& _position = tmp__position->Value;
            wrapVector3* tmp__center; Data_Get_Struct(center, wrapVector3, tmp__center);lnVector3& _center = tmp__center->Value;
            wrapVector2* tmp__size; Data_Get_Struct(size, wrapVector2, tmp__size);lnVector2& _size = tmp__size->Value;
            lnHandle _texture = RbRefObjToHandle(texture);
            wrapRect* tmp__srcRect; Data_Get_Struct(srcRect, wrapRect, tmp__srcRect);lnRect& _srcRect = tmp__srcRect->Value;
            wrapColorF* tmp__vertexColors; Data_Get_Struct(vertexColors, wrapColorF, tmp__vertexColors);lnColorF& _vertexColors = tmp__vertexColors->Value;
            lnFrontAxis _frontAxis = (frontAxis != Qnil) ? (lnFrontAxis)FIX2INT(frontAxis) : LN_FRONTAXIS_RZ;
            lnErrorCode errorCode = LNSpritePath_DrawSprite(selfObj->Handle, &_position, &_center, &_size, _texture, &_srcRect, &_vertexColors, _frontAxis);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::SpritePath.draw_sprite - wrong argument type.");
    return Qnil;
}

static void LNModel_delete(wrapModel* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNModel_mark(wrapModel* obj)
{

}

static VALUE LNModel_allocate( VALUE klass )
{
    VALUE obj;
    wrapModel* internalObj;

    internalObj = (wrapModel*)malloc(sizeof(wrapModel));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNModel_allocate" );
    obj = Data_Wrap_Struct(klass, LNModel_mark, LNModel_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapModel));

    return obj;
}

static VALUE LNModel_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapModel* internalObj;

    internalObj = (wrapModel*)malloc(sizeof(wrapModel));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNModel_allocate" );
    obj = Data_Wrap_Struct(klass, LNModel_mark, LNModel_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapModel));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNModel_Create(int argc, VALUE *argv, VALUE self)
{
    wrapModel* selfObj;
    Data_Get_Struct(self, wrapModel, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE meshCreateFlags;
        rb_scan_args(argc, argv, "2", &filePath, &meshCreateFlags);
        if (isRbString(filePath) && isRbNumber(meshCreateFlags)) {
            char* _filePath = StringValuePtr(filePath);
            lnMeshCreateFlags _meshCreateFlags = (lnMeshCreateFlags)FIX2INT(meshCreateFlags);
            lnErrorCode errorCode = LNModel_Create(&selfObj->Handle, _filePath, _meshCreateFlags);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Model.create - wrong argument type.");
    return Qnil;
}

static void LNEffect_delete(wrapEffect* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNEffect_mark(wrapEffect* obj)
{

}

static VALUE LNEffect_allocate( VALUE klass )
{
    VALUE obj;
    wrapEffect* internalObj;

    internalObj = (wrapEffect*)malloc(sizeof(wrapEffect));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNEffect_allocate" );
    obj = Data_Wrap_Struct(klass, LNEffect_mark, LNEffect_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapEffect));

    return obj;
}

static VALUE LNEffect_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapEffect* internalObj;

    internalObj = (wrapEffect*)malloc(sizeof(wrapEffect));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNEffect_allocate" );
    obj = Data_Wrap_Struct(klass, LNEffect_mark, LNEffect_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapEffect));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNEffect_Create(int argc, VALUE *argv, VALUE self)
{
    wrapEffect* selfObj;
    Data_Get_Struct(self, wrapEffect, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            lnErrorCode errorCode = LNEffect_Create(&selfObj->Handle, _filePath);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Effect.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNEffect_Play(int argc, VALUE *argv, VALUE self)
{
    wrapEffect* selfObj;
    Data_Get_Struct(self, wrapEffect, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE overlap;
        rb_scan_args(argc, argv, "1", &overlap);
        if (isRbBool(overlap)) {
            lnBool _overlap = RbBooltoBool(overlap);
            lnErrorCode errorCode = LNEffect_Play(selfObj->Handle, _overlap);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Effect.play - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNEffect_Stop(int argc, VALUE *argv, VALUE self)
{
    wrapEffect* selfObj;
    Data_Get_Struct(self, wrapEffect, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNEffect_Stop(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Effect.stop - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNEffect_IsPlaying(int argc, VALUE *argv, VALUE self)
{
    wrapEffect* selfObj;
    Data_Get_Struct(self, wrapEffect, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _playing;
            lnErrorCode errorCode = LNEffect_IsPlaying(selfObj->Handle, &_playing);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_playing);    }
    }
    rb_raise(rb_eArgError, "LNote::Effect.playing? - wrong argument type.");
    return Qnil;
}

static void LNTilemap_delete(wrapTilemap* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNTilemap_mark(wrapTilemap* obj)
{
    rb_gc_mark(obj->MapData);
    rb_gc_mark(obj->TilesetTexture);

}

static VALUE LNTilemap_allocate( VALUE klass )
{
    VALUE obj;
    wrapTilemap* internalObj;

    internalObj = (wrapTilemap*)malloc(sizeof(wrapTilemap));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTilemap_allocate" );
    obj = Data_Wrap_Struct(klass, LNTilemap_mark, LNTilemap_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTilemap));
    internalObj->MapData = Qnil;
    internalObj->TilesetTexture = Qnil;

    return obj;
}

static VALUE LNTilemap_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapTilemap* internalObj;

    internalObj = (wrapTilemap*)malloc(sizeof(wrapTilemap));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTilemap_allocate" );
    obj = Data_Wrap_Struct(klass, LNTilemap_mark, LNTilemap_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTilemap));
    internalObj->MapData = Qnil;
    internalObj->TilesetTexture = Qnil;

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNTilemap_Create(int argc, VALUE *argv, VALUE self)
{
    wrapTilemap* selfObj;
    Data_Get_Struct(self, wrapTilemap, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNTilemap_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Tilemap.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTilemap_SetMapData(int argc, VALUE *argv, VALUE self)
{
    wrapTilemap* selfObj;
    Data_Get_Struct(self, wrapTilemap, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE intTable;
        rb_scan_args(argc, argv, "1", &intTable);
        if (isRbObject(intTable)) {
            lnHandle _intTable = RbRefObjToHandle(intTable);
            lnErrorCode errorCode = LNTilemap_SetMapData(&selfObj->Handle, _intTable);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            selfObj->MapData = intTable;
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Tilemap.map_data= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTilemap_SetTilesetTexture(int argc, VALUE *argv, VALUE self)
{
    wrapTilemap* selfObj;
    Data_Get_Struct(self, wrapTilemap, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE texture;
        rb_scan_args(argc, argv, "1", &texture);
        if (isRbObject(texture)) {
            lnHandle _texture = RbRefObjToHandle(texture);
            lnErrorCode errorCode = LNTilemap_SetTilesetTexture(&selfObj->Handle, _texture);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            selfObj->TilesetTexture = texture;
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Tilemap.tileset_texture= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTilemap_SetTilesetHorizontalTileCount(int argc, VALUE *argv, VALUE self)
{
    wrapTilemap* selfObj;
    Data_Get_Struct(self, wrapTilemap, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE count;
        rb_scan_args(argc, argv, "1", &count);
        if (isRbNumber(count)) {
            int _count = FIX2INT(count);
            lnErrorCode errorCode = LNTilemap_SetTilesetHorizontalTileCount(&selfObj->Handle, _count);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Tilemap.tileset_horizontal_tile_count= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTilemap_SetTilePixelSize(int argc, VALUE *argv, VALUE self)
{
    wrapTilemap* selfObj;
    Data_Get_Struct(self, wrapTilemap, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE size;
        rb_scan_args(argc, argv, "1", &size);
        if (isRbNumber(size)) {
            int _size = FIX2INT(size);
            lnErrorCode errorCode = LNTilemap_SetTilePixelSize(&selfObj->Handle, _size);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Tilemap.tile_pixel_size= - wrong argument type.");
    return Qnil;
}

static void LNShader_delete(wrapShader* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNShader_mark(wrapShader* obj)
{

}

static VALUE LNShader_allocate( VALUE klass )
{
    VALUE obj;
    wrapShader* internalObj;

    internalObj = (wrapShader*)malloc(sizeof(wrapShader));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNShader_allocate" );
    obj = Data_Wrap_Struct(klass, LNShader_mark, LNShader_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapShader));

    return obj;
}

static VALUE LNShader_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapShader* internalObj;

    internalObj = (wrapShader*)malloc(sizeof(wrapShader));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNShader_allocate" );
    obj = Data_Wrap_Struct(klass, LNShader_mark, LNShader_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapShader));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNShader_CreateFromFile(int argc, VALUE *argv, VALUE self)
{
    wrapShader* selfObj;
    Data_Get_Struct(self, wrapShader, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            lnErrorCode errorCode = LNShader_CreateFromFile(&selfObj->Handle, _filePath);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Shader.create_from_file - wrong argument type.");
    return Qnil;
}

static void LNViewPane_delete(wrapViewPane* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNViewPane_mark(wrapViewPane* obj)
{
    rb_gc_mark(obj->DefaultViewPane);
    rb_gc_mark(obj->Layers);

}

static VALUE LNViewPane_allocate( VALUE klass )
{
    VALUE obj;
    wrapViewPane* internalObj;

    internalObj = (wrapViewPane*)malloc(sizeof(wrapViewPane));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNViewPane_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewPane_mark, LNViewPane_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapViewPane));
    internalObj->DefaultViewPane = Qnil;
    internalObj->Layers = Qnil;

    return obj;
}

static VALUE LNViewPane_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapViewPane* internalObj;

    internalObj = (wrapViewPane*)malloc(sizeof(wrapViewPane));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNViewPane_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewPane_mark, LNViewPane_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapViewPane));
    internalObj->DefaultViewPane = Qnil;
    internalObj->Layers = Qnil;

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNViewPane_GetDefaultViewPane(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnHandle _viewPane;
            lnErrorCode errorCode = LNViewPane_GetDefaultViewPane(&_viewPane);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            if (_viewPane == NULL) {
                LNViewPane_GetDefaultViewPane_default = Qnil;
            }
            else if (LNViewPane_GetDefaultViewPane_default == Qnil || ((wrapReferenceObject*)DATA_PTR(LNViewPane_GetDefaultViewPane_default))->Handle != _viewPane) {
                LNViewPane_GetDefaultViewPane_default = LNViewPane_allocate2(gViewPaneClass, _viewPane);
            }
            return LNViewPane_GetDefaultViewPane_default;
        }
    }
    rb_raise(rb_eArgError, "LNote::ViewPane.default_view_pane - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNViewPane_SetBackgroundColor(int argc, VALUE *argv, VALUE self)
{
    wrapViewPane* selfObj;
    Data_Get_Struct(self, wrapViewPane, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE color;
        rb_scan_args(argc, argv, "1", &color);
        if (isRbObject(color)) {
            wrapColorF* tmp__color; Data_Get_Struct(color, wrapColorF, tmp__color);lnColorF& _color = tmp__color->Value;
            lnErrorCode errorCode = LNViewPane_SetBackgroundColor(selfObj->Handle, &_color);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::ViewPane.background_color= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNViewPane_GetLayers(int argc, VALUE *argv, VALUE self)
{
    wrapViewPane* selfObj;
    Data_Get_Struct(self, wrapViewPane, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnHandle _objectList;
            lnErrorCode errorCode = LNViewPane_GetLayers(selfObj->Handle, &_objectList);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            if (_objectList == NULL) {
                selfObj->Layers = Qnil;
            }
            else if (selfObj->Layers == Qnil || ((wrapObjectList*)DATA_PTR(selfObj->Layers))->Handle != _objectList) {
                selfObj->Layers = LNObjectList_allocate2(gObjectListClass, _objectList );
            }
            return selfObj->Layers;
        }
    }
    rb_raise(rb_eArgError, "LNote::ViewPane.layers - wrong argument type.");
    return Qnil;
}

static void LNLayer_delete(wrapLayer* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNLayer_mark(wrapLayer* obj)
{

}

static VALUE LNLayer_allocate( VALUE klass )
{
    VALUE obj;
    wrapLayer* internalObj;

    internalObj = (wrapLayer*)malloc(sizeof(wrapLayer));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNLayer_allocate" );
    obj = Data_Wrap_Struct(klass, LNLayer_mark, LNLayer_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapLayer));

    return obj;
}

static VALUE LNLayer_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapLayer* internalObj;

    internalObj = (wrapLayer*)malloc(sizeof(wrapLayer));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNLayer_allocate" );
    obj = Data_Wrap_Struct(klass, LNLayer_mark, LNLayer_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapLayer));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNLayer_SetPriority(int argc, VALUE *argv, VALUE self)
{
    wrapLayer* selfObj;
    Data_Get_Struct(self, wrapLayer, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE priority;
        rb_scan_args(argc, argv, "1", &priority);
        if (isRbNumber(priority)) {
            int _priority = FIX2INT(priority);
            lnErrorCode errorCode = LNLayer_SetPriority(selfObj->Handle, _priority);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Layer.priority= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNLayer_GetPriority(int argc, VALUE *argv, VALUE self)
{
    wrapLayer* selfObj;
    Data_Get_Struct(self, wrapLayer, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            int _outPriority;
            lnErrorCode errorCode = LNLayer_GetPriority(selfObj->Handle, &_outPriority);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outPriority);    }
    }
    rb_raise(rb_eArgError, "LNote::Layer.priority - wrong argument type.");
    return Qnil;
}

static void LNViewFilter_delete(wrapViewFilter* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNViewFilter_mark(wrapViewFilter* obj)
{

}

static VALUE LNViewFilter_allocate( VALUE klass )
{
    VALUE obj;
    wrapViewFilter* internalObj;

    internalObj = (wrapViewFilter*)malloc(sizeof(wrapViewFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNViewFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewFilter_mark, LNViewFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapViewFilter));

    return obj;
}

static VALUE LNViewFilter_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapViewFilter* internalObj;

    internalObj = (wrapViewFilter*)malloc(sizeof(wrapViewFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNViewFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNViewFilter_mark, LNViewFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapViewFilter));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNViewFilter_Update(int argc, VALUE *argv, VALUE self)
{
    wrapViewFilter* selfObj;
    Data_Get_Struct(self, wrapViewFilter, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE elapsedTime;
        rb_scan_args(argc, argv, "1", &elapsedTime);
        if (isRbFloat(elapsedTime)) {
            float _elapsedTime = (float)(NUM2DBL(elapsedTime));;
            lnErrorCode errorCode = LNViewFilter_Update(selfObj->Handle, _elapsedTime);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::ViewFilter.update - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNViewFilter_SetEnableAutoUpdate(int argc, VALUE *argv, VALUE self)
{
    wrapViewFilter* selfObj;
    Data_Get_Struct(self, wrapViewFilter, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            lnErrorCode errorCode = LNViewFilter_SetEnableAutoUpdate(selfObj->Handle, _enabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::ViewFilter.enable_auto_update= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNViewFilter_IsEnableAutoUpdate(int argc, VALUE *argv, VALUE self)
{
    wrapViewFilter* selfObj;
    Data_Get_Struct(self, wrapViewFilter, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _outEnabled;
            lnErrorCode errorCode = LNViewFilter_IsEnableAutoUpdate(selfObj->Handle, &_outEnabled);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_outEnabled);    }
    }
    rb_raise(rb_eArgError, "LNote::ViewFilter.enable_auto_update? - wrong argument type.");
    return Qnil;
}

static void LNToneFilter_delete(wrapToneFilter* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNToneFilter_mark(wrapToneFilter* obj)
{

}

static VALUE LNToneFilter_allocate( VALUE klass )
{
    VALUE obj;
    wrapToneFilter* internalObj;

    internalObj = (wrapToneFilter*)malloc(sizeof(wrapToneFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNToneFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNToneFilter_mark, LNToneFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapToneFilter));

    return obj;
}

static VALUE LNToneFilter_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapToneFilter* internalObj;

    internalObj = (wrapToneFilter*)malloc(sizeof(wrapToneFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNToneFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNToneFilter_mark, LNToneFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapToneFilter));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNToneFilter_Create(int argc, VALUE *argv, VALUE self)
{
    wrapToneFilter* selfObj;
    Data_Get_Struct(self, wrapToneFilter, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNToneFilter_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::ToneFilter.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNToneFilter_SetTone(int argc, VALUE *argv, VALUE self)
{
    wrapToneFilter* selfObj;
    Data_Get_Struct(self, wrapToneFilter, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE tone;
        VALUE duration;
        rb_scan_args(argc, argv, "2", &tone, &duration);
        if (isRbObject(tone) && isRbFloat(duration)) {
            wrapTone* tmp__tone; Data_Get_Struct(tone, wrapTone, tmp__tone);lnTone& _tone = tmp__tone->Value;
            float _duration = (float)(NUM2DBL(duration));;
            lnErrorCode errorCode = LNToneFilter_SetTone(selfObj->Handle, &_tone, _duration);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::ToneFilter.set_tone - wrong argument type.");
    return Qnil;
}

static void LNTransitionFilter_delete(wrapTransitionFilter* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNTransitionFilter_mark(wrapTransitionFilter* obj)
{

}

static VALUE LNTransitionFilter_allocate( VALUE klass )
{
    VALUE obj;
    wrapTransitionFilter* internalObj;

    internalObj = (wrapTransitionFilter*)malloc(sizeof(wrapTransitionFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTransitionFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNTransitionFilter_mark, LNTransitionFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTransitionFilter));

    return obj;
}

static VALUE LNTransitionFilter_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapTransitionFilter* internalObj;

    internalObj = (wrapTransitionFilter*)malloc(sizeof(wrapTransitionFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNTransitionFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNTransitionFilter_mark, LNTransitionFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapTransitionFilter));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNTransitionFilter_Create(int argc, VALUE *argv, VALUE self)
{
    wrapTransitionFilter* selfObj;
    Data_Get_Struct(self, wrapTransitionFilter, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNTransitionFilter_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::TransitionFilter.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTransitionFilter_Freeze(int argc, VALUE *argv, VALUE self)
{
    wrapTransitionFilter* selfObj;
    Data_Get_Struct(self, wrapTransitionFilter, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNTransitionFilter_Freeze(selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::TransitionFilter.freeze - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNTransitionFilter_Transition(int argc, VALUE *argv, VALUE self)
{
    wrapTransitionFilter* selfObj;
    Data_Get_Struct(self, wrapTransitionFilter, selfObj);

    if (3 <= argc && argc <= 3) {
        VALUE duration;
        VALUE maskTexture;
        VALUE vague;
        rb_scan_args(argc, argv, "3", &duration, &maskTexture, &vague);
        if (isRbFloat(duration) && isRbObject(maskTexture) && isRbFloat(vague)) {
            double _duration = NUM2DBL(duration);
            lnHandle _maskTexture = RbRefObjToHandle(maskTexture);
            float _vague = (float)(NUM2DBL(vague));;
            lnErrorCode errorCode = LNTransitionFilter_Transition(selfObj->Handle, _duration, _maskTexture, _vague);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::TransitionFilter.transition - wrong argument type.");
    return Qnil;
}

static void LNBlurFilter_delete(wrapBlurFilter* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNBlurFilter_mark(wrapBlurFilter* obj)
{

}

static VALUE LNBlurFilter_allocate( VALUE klass )
{
    VALUE obj;
    wrapBlurFilter* internalObj;

    internalObj = (wrapBlurFilter*)malloc(sizeof(wrapBlurFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNBlurFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNBlurFilter_mark, LNBlurFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapBlurFilter));

    return obj;
}

static VALUE LNBlurFilter_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapBlurFilter* internalObj;

    internalObj = (wrapBlurFilter*)malloc(sizeof(wrapBlurFilter));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNBlurFilter_allocate" );
    obj = Data_Wrap_Struct(klass, LNBlurFilter_mark, LNBlurFilter_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapBlurFilter));

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNBlurFilter_Create(int argc, VALUE *argv, VALUE self)
{
    wrapBlurFilter* selfObj;
    Data_Get_Struct(self, wrapBlurFilter, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNBlurFilter_Create(&selfObj->Handle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::BlurFilter.create - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNBlurFilter_Blur(int argc, VALUE *argv, VALUE self)
{
    wrapBlurFilter* selfObj;
    Data_Get_Struct(self, wrapBlurFilter, selfObj);

    if (5 <= argc && argc <= 5) {
        VALUE duration;
        VALUE power;
        VALUE scale;
        VALUE center;
        VALUE baseCamera;
        rb_scan_args(argc, argv, "5", &duration, &power, &scale, &center, &baseCamera);
        if (isRbFloat(duration) && isRbFloat(power) && isRbFloat(scale) && isRbObject(center) && isRbObject(baseCamera)) {
            double _duration = NUM2DBL(duration);
            float _power = (float)(NUM2DBL(power));;
            float _scale = (float)(NUM2DBL(scale));;
            wrapVector3* tmp__center; Data_Get_Struct(center, wrapVector3, tmp__center);lnVector3& _center = tmp__center->Value;
            lnHandle _baseCamera = RbRefObjToHandle(baseCamera);
            lnErrorCode errorCode = LNBlurFilter_Blur(selfObj->Handle, _duration, _power, _scale, &_center, _baseCamera);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::BlurFilter.blur - wrong argument type.");
    return Qnil;
}

static void LNCamera_delete(wrapCamera* obj)
{
    if (obj->Handle != 0) LObject_Release(obj->Handle);
    free(obj);
}

static void LNCamera_mark(wrapCamera* obj)
{
    rb_gc_mark(obj->Default3DCamera);
    rb_gc_mark(obj->Default2DCamera);

}

static VALUE LNCamera_allocate( VALUE klass )
{
    VALUE obj;
    wrapCamera* internalObj;

    internalObj = (wrapCamera*)malloc(sizeof(wrapCamera));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNCamera_allocate" );
    obj = Data_Wrap_Struct(klass, LNCamera_mark, LNCamera_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapCamera));
    internalObj->Default3DCamera = Qnil;
    internalObj->Default2DCamera = Qnil;

    return obj;
}

static VALUE LNCamera_allocate2( VALUE klass, lnHandle handle )
{
    VALUE obj;
    wrapCamera* internalObj;

    internalObj = (wrapCamera*)malloc(sizeof(wrapCamera));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNCamera_allocate" );
    obj = Data_Wrap_Struct(klass, LNCamera_mark, LNCamera_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapCamera));
    internalObj->Default3DCamera = Qnil;
    internalObj->Default2DCamera = Qnil;

    internalObj->Handle = handle;
    return obj;
}

static VALUE lnrbLNCamera_GetDefault3DCamera(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnHandle _camera;
            lnErrorCode errorCode = LNCamera_GetDefault3DCamera(&_camera);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            if (_camera == NULL) {
                LNCamera_GetDefault3DCamera_default = Qnil;
            }
            else if (LNCamera_GetDefault3DCamera_default == Qnil || ((wrapReferenceObject*)DATA_PTR(LNCamera_GetDefault3DCamera_default))->Handle != _camera) {
                LNCamera_GetDefault3DCamera_default = LNCamera_allocate2(gCameraClass, _camera);
            }
            return LNCamera_GetDefault3DCamera_default;
        }
    }
    rb_raise(rb_eArgError, "LNote::Camera.default_3d_camera - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_GetDefault2DCamera(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnHandle _camera;
            lnErrorCode errorCode = LNCamera_GetDefault2DCamera(&_camera);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            if (_camera == NULL) {
                LNCamera_GetDefault2DCamera_default = Qnil;
            }
            else if (LNCamera_GetDefault2DCamera_default == Qnil || ((wrapReferenceObject*)DATA_PTR(LNCamera_GetDefault2DCamera_default))->Handle != _camera) {
                LNCamera_GetDefault2DCamera_default = LNCamera_allocate2(gCameraClass, _camera);
            }
            return LNCamera_GetDefault2DCamera_default;
        }
    }
    rb_raise(rb_eArgError, "LNote::Camera.default_2d_camera - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetCameraType(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE type;
        rb_scan_args(argc, argv, "1", &type);
        if (isRbNumber(type)) {
            lnCameraType _type = (lnCameraType)FIX2INT(type);
            lnErrorCode errorCode = LNCamera_SetCameraType(selfObj->Handle, _type);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.camera_type= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetPosition(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE position;
        rb_scan_args(argc, argv, "1", &position);
        if (isRbObject(position)) {
            wrapVector3* tmp__position; Data_Get_Struct(position, wrapVector3, tmp__position);lnVector3& _position = tmp__position->Value;
            lnErrorCode errorCode = LNCamera_SetPosition(selfObj->Handle, &_position);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNCamera_SetPositionXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.position= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetLookAt(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE at;
        rb_scan_args(argc, argv, "1", &at);
        if (isRbObject(at)) {
            wrapVector3* tmp__at; Data_Get_Struct(at, wrapVector3, tmp__at);lnVector3& _at = tmp__at->Value;
            lnErrorCode errorCode = LNCamera_SetLookAt(selfObj->Handle, &_at);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNCamera_SetLookAtXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.look_at= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetUpDirection(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE up;
        rb_scan_args(argc, argv, "1", &up);
        if (isRbObject(up)) {
            wrapVector3* tmp__up; Data_Get_Struct(up, wrapVector3, tmp__up);lnVector3& _up = tmp__up->Value;
            lnErrorCode errorCode = LNCamera_SetUpDirection(selfObj->Handle, &_up);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNCamera_SetUpDirectionXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.up_direction= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetAngle(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE angle;
        rb_scan_args(argc, argv, "1", &angle);
        if (isRbObject(angle)) {
            wrapVector3* tmp__angle; Data_Get_Struct(angle, wrapVector3, tmp__angle);lnVector3& _angle = tmp__angle->Value;
            lnErrorCode errorCode = LNCamera_SetAngle(selfObj->Handle, &_angle);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    if (3 <= argc && argc <= 3) {
        VALUE x;
        VALUE y;
        VALUE z;
        rb_scan_args(argc, argv, "3", &x, &y, &z);
        if (isRbFloat(x) && isRbFloat(y) && isRbFloat(z)) {
            float _x = (float)(NUM2DBL(x));;
            float _y = (float)(NUM2DBL(y));;
            float _z = (float)(NUM2DBL(z));;
            lnErrorCode errorCode = LNCamera_SetAngleXYZ(selfObj->Handle, _x, _y, _z);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.angle= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE matrix;
        rb_scan_args(argc, argv, "1", &matrix);
        if (isRbObject(matrix)) {
            wrapMatrix* tmp__matrix; Data_Get_Struct(matrix, wrapMatrix, tmp__matrix);lnMatrix& _matrix = tmp__matrix->Value;
            lnErrorCode errorCode = LNCamera_SetMatrix(selfObj->Handle, &_matrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.matrix= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetFovY(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (1 <= argc && argc <= 1) {
        VALUE fov;
        rb_scan_args(argc, argv, "1", &fov);
        if (isRbFloat(fov)) {
            float _fov = (float)(NUM2DBL(fov));;
            lnErrorCode errorCode = LNCamera_SetFovY(selfObj->Handle, _fov);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.fov_y= - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_SetNearFarClip(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (2 <= argc && argc <= 2) {
        VALUE nearClip;
        VALUE farClip;
        rb_scan_args(argc, argv, "2", &nearClip, &farClip);
        if (isRbFloat(nearClip) && isRbFloat(farClip)) {
            float _nearClip = (float)(NUM2DBL(nearClip));;
            float _farClip = (float)(NUM2DBL(farClip));;
            lnErrorCode errorCode = LNCamera_SetNearFarClip(selfObj->Handle, _nearClip, _farClip);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Camera.set_near_far_clip - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_GetViewMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _outMatrix;
            lnErrorCode errorCode = LNCamera_GetViewMatrix(selfObj->Handle, &_outMatrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _outMatrix;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Camera.view_matrix - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_GetProjectionMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _outMatrix;
            lnErrorCode errorCode = LNCamera_GetProjectionMatrix(selfObj->Handle, &_outMatrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _outMatrix;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Camera.projection_matrix - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCamera_GetViewProjectionMatrix(int argc, VALUE *argv, VALUE self)
{
    wrapCamera* selfObj;
    Data_Get_Struct(self, wrapCamera, selfObj);

    if (0 <= argc && argc <= 0) {

        if (true) {
            lnMatrix _outMatrix;
            lnErrorCode errorCode = LNCamera_GetViewProjectionMatrix(selfObj->Handle, &_outMatrix);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            VALUE retObj = LNMatrix_allocate(gMatrixClass);
            ((wrapMatrix*)DATA_PTR(retObj))->Value = _outMatrix;
            return retObj;
        }
    }
    rb_raise(rb_eArgError, "LNote::Camera.view_projection_matrix - wrong argument type.");
    return Qnil;
}

static void LNConfig_delete(wrapConfig* obj)
{
    
    free(obj);
}

static void LNConfig_mark(wrapConfig* obj)
{

}

static VALUE LNConfig_allocate( VALUE klass )
{
    VALUE obj;
    wrapConfig* internalObj;

    internalObj = (wrapConfig*)malloc(sizeof(wrapConfig));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNConfig_allocate" );
    obj = Data_Wrap_Struct(klass, LNConfig_mark, LNConfig_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapConfig));

    return obj;
}

static VALUE lnrbLNConfig_SetWindowTitle(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE title;
        rb_scan_args(argc, argv, "1", &title);
        if (isRbString(title)) {
            char* _title = StringValuePtr(title);
            LNConfig_SetWindowTitle(_title);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_window_title - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetWindowSize(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (isRbNumber(width) && isRbNumber(height)) {
            int _width = FIX2INT(width);
            int _height = FIX2INT(height);
            LNConfig_SetWindowSize(_width, _height);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_window_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetBackbufferSize(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE width;
        VALUE height;
        rb_scan_args(argc, argv, "2", &width, &height);
        if (isRbNumber(width) && isRbNumber(height)) {
            int _width = FIX2INT(width);
            int _height = FIX2INT(height);
            LNConfig_SetBackbufferSize(_width, _height);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_backbuffer_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetWindowResizeEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetWindowResizeEnabled(_enabled);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_window_resize_enabled - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetAcceleratorKey(int argc, VALUE *argv, VALUE self)
{
    if (5 <= argc && argc <= 5) {
        VALUE acceleratorKey;
        VALUE keyCode;
        VALUE isShift;
        VALUE isCtrl;
        VALUE isAlt;
        rb_scan_args(argc, argv, "5", &acceleratorKey, &keyCode, &isShift, &isCtrl, &isAlt);
        if (isRbNumber(acceleratorKey) && isRbNumber(keyCode) && isRbBool(isShift) && isRbBool(isCtrl) && isRbBool(isAlt)) {
            lnAcceleratorKey _acceleratorKey = (lnAcceleratorKey)FIX2INT(acceleratorKey);
            lnKeyCode _keyCode = (lnKeyCode)FIX2INT(keyCode);
            lnBool _isShift = RbBooltoBool(isShift);
            lnBool _isCtrl = RbBooltoBool(isCtrl);
            lnBool _isAlt = RbBooltoBool(isAlt);
            LNConfig_SetAcceleratorKey(_acceleratorKey, _keyCode, _isShift, _isCtrl, _isAlt);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_accelerator_key - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetLogFileEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE flag;
        rb_scan_args(argc, argv, "1", &flag);
        if (isRbBool(flag)) {
            lnBool _flag = RbBooltoBool(flag);
            LNConfig_SetLogFileEnabled(_flag);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_log_file_enabled - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetConsoleEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE flag;
        rb_scan_args(argc, argv, "1", &flag);
        if (isRbBool(flag)) {
            lnBool _flag = RbBooltoBool(flag);
            LNConfig_SetConsoleEnabled(_flag);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_console_enabled - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_RegisterArchive(int argc, VALUE *argv, VALUE self)
{
    if (2 <= argc && argc <= 2) {
        VALUE filePath;
        VALUE password;
        rb_scan_args(argc, argv, "2", &filePath, &password);
        if (isRbString(filePath) && isRbString(password)) {
            char* _filePath = StringValuePtr(filePath);
            char* _password = StringValuePtr(password);
            LNConfig_RegisterArchive(_filePath, _password);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.register_archive - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetFileAccessPriority(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE priority;
        rb_scan_args(argc, argv, "1", &priority);
        if (isRbNumber(priority)) {
            lnFileAccessPriority _priority = (lnFileAccessPriority)FIX2INT(priority);
            LNConfig_SetFileAccessPriority(_priority);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_file_access_priority - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetDirectMusicInitMode(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE mode;
        rb_scan_args(argc, argv, "1", &mode);
        if (isRbNumber(mode)) {
            lnDirectMusicInitMode _mode = (lnDirectMusicInitMode)FIX2INT(mode);
            LNConfig_SetDirectMusicInitMode(_mode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_direct_music_init_mode - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetSoundMaxCacheCount(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE count;
        rb_scan_args(argc, argv, "1", &count);
        if (isRbNumber(count)) {
            int _count = FIX2INT(count);
            LNConfig_SetSoundMaxCacheCount(_count);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_sound_max_cache_count - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SoundPlayTypeThreshold(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE bytes;
        rb_scan_args(argc, argv, "1", &bytes);
        if (isRbNumber(bytes)) {
            int _bytes = FIX2INT(bytes);
            LNConfig_SoundPlayTypeThreshold(_bytes);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.sound_play_type_threshold - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetFrameRate(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE frameRate;
        rb_scan_args(argc, argv, "1", &frameRate);
        if (isRbNumber(frameRate)) {
            int _frameRate = FIX2INT(frameRate);
            LNConfig_SetFrameRate(_frameRate);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_frame_rate - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetEnableFrameWait(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetEnableFrameWait(_enabled);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_enable_frame_wait - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetAutoUpdateTimeScale(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE timeScale;
        rb_scan_args(argc, argv, "1", &timeScale);
        if (isRbFloat(timeScale)) {
            float _timeScale = (float)(NUM2DBL(timeScale));;
            LNConfig_SetAutoUpdateTimeScale(_timeScale);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_auto_update_time_scale - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_RegisterFontFile(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE filePath;
        rb_scan_args(argc, argv, "1", &filePath);
        if (isRbString(filePath)) {
            char* _filePath = StringValuePtr(filePath);
            LNConfig_RegisterFontFile(_filePath);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.register_font_file - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetTextureCacheSize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE count;
        rb_scan_args(argc, argv, "1", &count);
        if (isRbNumber(count)) {
            int _count = FIX2INT(count);
            LNConfig_SetTextureCacheSize(_count);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_texture_cache_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetShaderCacheSize(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE count;
        rb_scan_args(argc, argv, "1", &count);
        if (isRbNumber(count)) {
            int _count = FIX2INT(count);
            LNConfig_SetShaderCacheSize(_count);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_shader_cache_size - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNConfig_SetExceptionMessageBoxEnabled(int argc, VALUE *argv, VALUE self)
{
    if (1 <= argc && argc <= 1) {
        VALUE enabled;
        rb_scan_args(argc, argv, "1", &enabled);
        if (isRbBool(enabled)) {
            lnBool _enabled = RbBooltoBool(enabled);
            LNConfig_SetExceptionMessageBoxEnabled(_enabled);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Config.set_exception_message_box_enabled - wrong argument type.");
    return Qnil;
}

static void LNCore_delete(wrapCore* obj)
{
    
    free(obj);
}

static void LNCore_mark(wrapCore* obj)
{

}

static VALUE LNCore_allocate( VALUE klass )
{
    VALUE obj;
    wrapCore* internalObj;

    internalObj = (wrapCore*)malloc(sizeof(wrapCore));
    if (internalObj == NULL) rb_raise( gLNoteModule, "Faild alloc - LNCore_allocate" );
    obj = Data_Wrap_Struct(klass, LNCore_mark, LNCore_delete, internalObj);
    
    memset(internalObj, 0, sizeof(wrapCore));

    return obj;
}

static VALUE lnrbLNCore_Initialize(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_Initialize();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.initialize - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_InitializeFileIO(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_InitializeFileIO();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.initialize_file_io - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_InitializeAudio(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_InitializeAudio();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.initialize_audio - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_InitializeInput(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_InitializeInput();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.initialize_input - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_Update(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_Update();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.update - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_ResetFrameDelay(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnErrorCode errorCode = LNCore_ResetFrameDelay();
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.reset_frame_delay - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_IsEndRequested(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            lnBool _requested;
            lnErrorCode errorCode = LNCore_IsEndRequested(&_requested);
            if (errorCode != LN_OK) rb_raise(gLNoteError, "internal error. code:%d", errorCode);
            return toVALUE(_requested);    }
    }
    rb_raise(rb_eArgError, "LNote::Core.end_requested? - wrong argument type.");
    return Qnil;
}

static VALUE lnrbLNCore_Terminate(int argc, VALUE *argv, VALUE self)
{
    if (0 <= argc && argc <= 0) {

        if (true) {
            LNCore_Terminate();
            return Qnil;    }
    }
    rb_raise(rb_eArgError, "LNote::Core.terminate - wrong argument type.");
    return Qnil;
}




void LNote_Terminate(VALUE self)
{
	LManager_Terminate();
}

extern "C" void Init_lnote()
{
    // ルートモジュール
    gLNoteModule = rb_define_module("LN");

    // 例外定義
    gLNoteError = rb_define_class_under( gLNoteModule, "LNoteError", rb_eRuntimeError );
    
    // 参照オブジェクトクラス
    gReferenceObjectClass = rb_define_class_under(gLNoteModule, "ReferenceObject", rb_cObject);
    rb_define_alloc_func(gReferenceObjectClass, LReferenceObject_allocate);
    rb_define_private_method(gReferenceObjectClass, "initialize", LN_TO_RUBY_FUNC(LReferenceObject_initialize), -1);

    // 参照オブジェクトリストクラス
    gObjectListClass = rb_define_class_under(gLNoteModule, "ReferenceObjectList", gReferenceObjectClass);
	rb_define_method(gObjectListClass, "[]", LN_TO_RUBY_FUNC(LObjectList_get_at), 1);
	rb_define_method(gObjectListClass, "[]=", LN_TO_RUBY_FUNC(LObjectList_set_at), 2);
	rb_define_method(gObjectListClass, "count", LN_TO_RUBY_FUNC(LObjectList_get_count), 0);
	rb_define_method(gObjectListClass, "size", LN_TO_RUBY_FUNC(LObjectList_get_count), 0);
	rb_define_method(gObjectListClass, "add", LN_TO_RUBY_FUNC(LObjectList_add), 1);
	rb_define_method(gObjectListClass, "push", LN_TO_RUBY_FUNC(LObjectList_add), 1);
	rb_define_method(gObjectListClass, "clear", LN_TO_RUBY_FUNC(LObjectList_clear), 0);
	rb_define_method(gObjectListClass, "insert", LN_TO_RUBY_FUNC(LObjectList_insert), 2);
	rb_define_method(gObjectListClass, "remove", LN_TO_RUBY_FUNC(LObjectList_remove), 1);
	rb_define_method(gObjectListClass, "remove_all", LN_TO_RUBY_FUNC(LObjectList_remove_all), 1);
	rb_define_method(gObjectListClass, "remove_at", LN_TO_RUBY_FUNC(LObjectList_remove_at), 1);

    gSizeClass = rb_define_class_under(gLNoteModule, "Size", rb_cObject);
    rb_define_alloc_func(gSizeClass, LNSize_allocate);
rb_define_private_method(gSizeClass, "initialize", LN_TO_RUBY_FUNC(LNSize_struct_initialize), -1);
rb_define_method(gSizeClass, "width=", LN_TO_RUBY_FUNC(LNSize_Width_set), 1);
rb_define_method(gSizeClass, "width", LN_TO_RUBY_FUNC(LNSize_Width_get), 0);
rb_define_method(gSizeClass, "height=", LN_TO_RUBY_FUNC(LNSize_Height_set), 1);
rb_define_method(gSizeClass, "height", LN_TO_RUBY_FUNC(LNSize_Height_get), 0);

    gRectClass = rb_define_class_under(gLNoteModule, "Rect", rb_cObject);
    rb_define_alloc_func(gRectClass, LNRect_allocate);
rb_define_private_method(gRectClass, "initialize", LN_TO_RUBY_FUNC(LNRect_struct_initialize), -1);
rb_define_method(gRectClass, "x=", LN_TO_RUBY_FUNC(LNRect_X_set), 1);
rb_define_method(gRectClass, "x", LN_TO_RUBY_FUNC(LNRect_X_get), 0);
rb_define_method(gRectClass, "y=", LN_TO_RUBY_FUNC(LNRect_Y_set), 1);
rb_define_method(gRectClass, "y", LN_TO_RUBY_FUNC(LNRect_Y_get), 0);
rb_define_method(gRectClass, "width=", LN_TO_RUBY_FUNC(LNRect_Width_set), 1);
rb_define_method(gRectClass, "width", LN_TO_RUBY_FUNC(LNRect_Width_get), 0);
rb_define_method(gRectClass, "height=", LN_TO_RUBY_FUNC(LNRect_Height_set), 1);
rb_define_method(gRectClass, "height", LN_TO_RUBY_FUNC(LNRect_Height_get), 0);
    rb_define_method(gRectClass, "contains", LN_TO_RUBY_FUNC(lnrbLNRect_Contains), -1);

    gIntTableClass = rb_define_class_under(gLNoteModule, "IntTable", gReferenceObjectClass);
    rb_define_alloc_func(gIntTableClass, LNIntTable_allocate);
    gIntTableClassTypeInfo.Factory = LNIntTable_allocate2;
    gIntTableClassTypeInfo.Klass = &gIntTableClass;
    LNIntTable_SetTypeUserData((void*)(&gIntTableClassTypeInfo));
    rb_define_private_method(gIntTableClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNIntTable_Create), -1);
    rb_define_method(gIntTableClass, "set_value", LN_TO_RUBY_FUNC(lnrbLNIntTable_SetValue), -1);
    rb_define_method(gIntTableClass, "get_value", LN_TO_RUBY_FUNC(lnrbLNIntTable_GetValue), -1);
    rb_define_method(gIntTableClass, "resize", LN_TO_RUBY_FUNC(lnrbLNIntTable_Resize), -1);
    rb_define_method(gIntTableClass, "x_size", LN_TO_RUBY_FUNC(lnrbLNIntTable_GetXSize), -1);
    rb_define_method(gIntTableClass, "y_size", LN_TO_RUBY_FUNC(lnrbLNIntTable_GetYSize), -1);
    rb_define_method(gIntTableClass, "z_size", LN_TO_RUBY_FUNC(lnrbLNIntTable_GetZSize), -1);

    gVector2Class = rb_define_class_under(gLNoteModule, "Vector2", rb_cObject);
    rb_define_alloc_func(gVector2Class, LNVector2_allocate);
rb_define_private_method(gVector2Class, "initialize", LN_TO_RUBY_FUNC(LNVector2_struct_initialize), -1);
rb_define_method(gVector2Class, "x=", LN_TO_RUBY_FUNC(LNVector2_X_set), 1);
rb_define_method(gVector2Class, "x", LN_TO_RUBY_FUNC(LNVector2_X_get), 0);
rb_define_method(gVector2Class, "y=", LN_TO_RUBY_FUNC(LNVector2_Y_set), 1);
rb_define_method(gVector2Class, "y", LN_TO_RUBY_FUNC(LNVector2_Y_get), 0);
    rb_define_method(gVector2Class, "set", LN_TO_RUBY_FUNC(lnrbLNVector2_Set), -1);
    rb_define_method(gVector2Class, "length", LN_TO_RUBY_FUNC(lnrbLNVector2_GetLength), -1);
    rb_define_method(gVector2Class, "square_length", LN_TO_RUBY_FUNC(lnrbLNVector2_GetSquareLength), -1);
    rb_define_singleton_method(gVector2Class, "normalize", LN_TO_RUBY_FUNC(lnrbLNVector2_Normalize), -1);

    gVector3Class = rb_define_class_under(gLNoteModule, "Vector3", rb_cObject);
    rb_define_alloc_func(gVector3Class, LNVector3_allocate);
rb_define_private_method(gVector3Class, "initialize", LN_TO_RUBY_FUNC(LNVector3_struct_initialize), -1);
rb_define_method(gVector3Class, "x=", LN_TO_RUBY_FUNC(LNVector3_X_set), 1);
rb_define_method(gVector3Class, "x", LN_TO_RUBY_FUNC(LNVector3_X_get), 0);
rb_define_method(gVector3Class, "y=", LN_TO_RUBY_FUNC(LNVector3_Y_set), 1);
rb_define_method(gVector3Class, "y", LN_TO_RUBY_FUNC(LNVector3_Y_get), 0);
rb_define_method(gVector3Class, "z=", LN_TO_RUBY_FUNC(LNVector3_Z_set), 1);
rb_define_method(gVector3Class, "z", LN_TO_RUBY_FUNC(LNVector3_Z_get), 0);
    rb_define_method(gVector3Class, "set", LN_TO_RUBY_FUNC(lnrbLNVector3_Set), -1);
    rb_define_method(gVector3Class, "length", LN_TO_RUBY_FUNC(lnrbLNVector3_GetLength), -1);
    rb_define_method(gVector3Class, "square_length", LN_TO_RUBY_FUNC(lnrbLNVector3_GetSquareLength), -1);
    rb_define_singleton_method(gVector3Class, "normalize", LN_TO_RUBY_FUNC(lnrbLNVector3_Normalize), -1);
    rb_define_singleton_method(gVector3Class, "dot", LN_TO_RUBY_FUNC(lnrbLNVector3_Dot), -1);
    rb_define_singleton_method(gVector3Class, "cross", LN_TO_RUBY_FUNC(lnrbLNVector3_Cross), -1);
    rb_define_singleton_method(gVector3Class, "reflect", LN_TO_RUBY_FUNC(lnrbLNVector3_Reflect), -1);
    rb_define_singleton_method(gVector3Class, "slide", LN_TO_RUBY_FUNC(lnrbLNVector3_Slide), -1);
    rb_define_singleton_method(gVector3Class, "lerp", LN_TO_RUBY_FUNC(lnrbLNVector3_Lerp), -1);
    rb_define_singleton_method(gVector3Class, "catmull_rom", LN_TO_RUBY_FUNC(lnrbLNVector3_CatmullRom), -1);
    rb_define_singleton_method(gVector3Class, "transform", LN_TO_RUBY_FUNC(lnrbLNVector3_Transform), -1);
    rb_define_singleton_method(gVector3Class, "transform_coord", LN_TO_RUBY_FUNC(lnrbLNVector3_TransformCoord), -1);
    rb_define_singleton_method(gVector3Class, "rotate_axis", LN_TO_RUBY_FUNC(lnrbLNVector3_RotateAxis), -1);
    rb_define_singleton_method(gVector3Class, "rotate_matrix", LN_TO_RUBY_FUNC(lnrbLNVector3_RotateMatrix), -1);

    gVector4Class = rb_define_class_under(gLNoteModule, "Vector4", rb_cObject);
    rb_define_alloc_func(gVector4Class, LNVector4_allocate);
rb_define_private_method(gVector4Class, "initialize", LN_TO_RUBY_FUNC(LNVector4_struct_initialize), -1);
rb_define_method(gVector4Class, "x=", LN_TO_RUBY_FUNC(LNVector4_X_set), 1);
rb_define_method(gVector4Class, "x", LN_TO_RUBY_FUNC(LNVector4_X_get), 0);
rb_define_method(gVector4Class, "y=", LN_TO_RUBY_FUNC(LNVector4_Y_set), 1);
rb_define_method(gVector4Class, "y", LN_TO_RUBY_FUNC(LNVector4_Y_get), 0);
rb_define_method(gVector4Class, "z=", LN_TO_RUBY_FUNC(LNVector4_Z_set), 1);
rb_define_method(gVector4Class, "z", LN_TO_RUBY_FUNC(LNVector4_Z_get), 0);
rb_define_method(gVector4Class, "w=", LN_TO_RUBY_FUNC(LNVector4_W_set), 1);
rb_define_method(gVector4Class, "w", LN_TO_RUBY_FUNC(LNVector4_W_get), 0);

    gMatrixClass = rb_define_class_under(gLNoteModule, "Matrix", rb_cObject);
    rb_define_alloc_func(gMatrixClass, LNMatrix_allocate);
rb_define_private_method(gMatrixClass, "initialize", LN_TO_RUBY_FUNC(LNMatrix_struct_initialize), -1);
rb_define_method(gMatrixClass, "m11=", LN_TO_RUBY_FUNC(LNMatrix_M11_set), 1);
rb_define_method(gMatrixClass, "m11", LN_TO_RUBY_FUNC(LNMatrix_M11_get), 0);
rb_define_method(gMatrixClass, "m12=", LN_TO_RUBY_FUNC(LNMatrix_M12_set), 1);
rb_define_method(gMatrixClass, "m12", LN_TO_RUBY_FUNC(LNMatrix_M12_get), 0);
rb_define_method(gMatrixClass, "m13=", LN_TO_RUBY_FUNC(LNMatrix_M13_set), 1);
rb_define_method(gMatrixClass, "m13", LN_TO_RUBY_FUNC(LNMatrix_M13_get), 0);
rb_define_method(gMatrixClass, "m14=", LN_TO_RUBY_FUNC(LNMatrix_M14_set), 1);
rb_define_method(gMatrixClass, "m14", LN_TO_RUBY_FUNC(LNMatrix_M14_get), 0);
rb_define_method(gMatrixClass, "m21=", LN_TO_RUBY_FUNC(LNMatrix_M21_set), 1);
rb_define_method(gMatrixClass, "m21", LN_TO_RUBY_FUNC(LNMatrix_M21_get), 0);
rb_define_method(gMatrixClass, "m22=", LN_TO_RUBY_FUNC(LNMatrix_M22_set), 1);
rb_define_method(gMatrixClass, "m22", LN_TO_RUBY_FUNC(LNMatrix_M22_get), 0);
rb_define_method(gMatrixClass, "m23=", LN_TO_RUBY_FUNC(LNMatrix_M23_set), 1);
rb_define_method(gMatrixClass, "m23", LN_TO_RUBY_FUNC(LNMatrix_M23_get), 0);
rb_define_method(gMatrixClass, "m24=", LN_TO_RUBY_FUNC(LNMatrix_M24_set), 1);
rb_define_method(gMatrixClass, "m24", LN_TO_RUBY_FUNC(LNMatrix_M24_get), 0);
rb_define_method(gMatrixClass, "m31=", LN_TO_RUBY_FUNC(LNMatrix_M31_set), 1);
rb_define_method(gMatrixClass, "m31", LN_TO_RUBY_FUNC(LNMatrix_M31_get), 0);
rb_define_method(gMatrixClass, "m32=", LN_TO_RUBY_FUNC(LNMatrix_M32_set), 1);
rb_define_method(gMatrixClass, "m32", LN_TO_RUBY_FUNC(LNMatrix_M32_get), 0);
rb_define_method(gMatrixClass, "m33=", LN_TO_RUBY_FUNC(LNMatrix_M33_set), 1);
rb_define_method(gMatrixClass, "m33", LN_TO_RUBY_FUNC(LNMatrix_M33_get), 0);
rb_define_method(gMatrixClass, "m34=", LN_TO_RUBY_FUNC(LNMatrix_M34_set), 1);
rb_define_method(gMatrixClass, "m34", LN_TO_RUBY_FUNC(LNMatrix_M34_get), 0);
rb_define_method(gMatrixClass, "m41=", LN_TO_RUBY_FUNC(LNMatrix_M41_set), 1);
rb_define_method(gMatrixClass, "m41", LN_TO_RUBY_FUNC(LNMatrix_M41_get), 0);
rb_define_method(gMatrixClass, "m42=", LN_TO_RUBY_FUNC(LNMatrix_M42_set), 1);
rb_define_method(gMatrixClass, "m42", LN_TO_RUBY_FUNC(LNMatrix_M42_get), 0);
rb_define_method(gMatrixClass, "m43=", LN_TO_RUBY_FUNC(LNMatrix_M43_set), 1);
rb_define_method(gMatrixClass, "m43", LN_TO_RUBY_FUNC(LNMatrix_M43_get), 0);
rb_define_method(gMatrixClass, "m44=", LN_TO_RUBY_FUNC(LNMatrix_M44_set), 1);
rb_define_method(gMatrixClass, "m44", LN_TO_RUBY_FUNC(LNMatrix_M44_get), 0);
    rb_define_singleton_method(gMatrixClass, "tity", LN_TO_RUBY_FUNC(lnrbLNMatrix_Identity), -1);
    rb_define_method(gMatrixClass, "translate", LN_TO_RUBY_FUNC(lnrbLNMatrix_Translate), -1);
    rb_define_method(gMatrixClass, "rotate_x", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateX), -1);
    rb_define_method(gMatrixClass, "rotate_y", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateY), -1);
    rb_define_method(gMatrixClass, "rotate_z", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateZ), -1);
    rb_define_method(gMatrixClass, "rotate", LN_TO_RUBY_FUNC(lnrbLNMatrix_Rotate), -1);
    rb_define_method(gMatrixClass, "rotate_axis", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateAxis), -1);
    rb_define_method(gMatrixClass, "rotate_quaternion", LN_TO_RUBY_FUNC(lnrbLNMatrix_RotateQuaternion), -1);
    rb_define_method(gMatrixClass, "scale", LN_TO_RUBY_FUNC(lnrbLNMatrix_Scale), -1);
    rb_define_singleton_method(gMatrixClass, "multiply", LN_TO_RUBY_FUNC(lnrbLNMatrix_Multiply), -1);
    rb_define_singleton_method(gMatrixClass, "inverse", LN_TO_RUBY_FUNC(lnrbLNMatrix_Inverse), -1);
    rb_define_singleton_method(gMatrixClass, "transpose", LN_TO_RUBY_FUNC(lnrbLNMatrix_Transpose), -1);
    rb_define_singleton_method(gMatrixClass, "view_transform_lh", LN_TO_RUBY_FUNC(lnrbLNMatrix_ViewTransformLH), -1);
    rb_define_singleton_method(gMatrixClass, "view_transform_rh", LN_TO_RUBY_FUNC(lnrbLNMatrix_ViewTransformRH), -1);
    rb_define_singleton_method(gMatrixClass, "perspective_fov_lh", LN_TO_RUBY_FUNC(lnrbLNMatrix_PerspectiveFovLH), -1);
    rb_define_singleton_method(gMatrixClass, "perspective_fov_rh", LN_TO_RUBY_FUNC(lnrbLNMatrix_PerspectiveFovRH), -1);
    rb_define_singleton_method(gMatrixClass, "ortho_lh", LN_TO_RUBY_FUNC(lnrbLNMatrix_OrthoLH), -1);
    rb_define_singleton_method(gMatrixClass, "ortho_rh", LN_TO_RUBY_FUNC(lnrbLNMatrix_OrthoRH), -1);
    rb_define_method(gMatrixClass, "right", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetRight), -1);
    rb_define_method(gMatrixClass, "up", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetUp), -1);
    rb_define_method(gMatrixClass, "front", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetFront), -1);
    rb_define_method(gMatrixClass, "position", LN_TO_RUBY_FUNC(lnrbLNMatrix_GetPosition), -1);
    rb_define_singleton_method(gMatrixClass, "to_euler", LN_TO_RUBY_FUNC(lnrbLNMatrix_ToEuler), -1);
    rb_define_singleton_method(gMatrixClass, "transform_basis", LN_TO_RUBY_FUNC(lnrbLNMatrix_TransformBasis), -1);

    gQuaternionClass = rb_define_class_under(gLNoteModule, "Quaternion", rb_cObject);
    rb_define_alloc_func(gQuaternionClass, LNQuaternion_allocate);
rb_define_private_method(gQuaternionClass, "initialize", LN_TO_RUBY_FUNC(LNQuaternion_struct_initialize), -1);
rb_define_method(gQuaternionClass, "x=", LN_TO_RUBY_FUNC(LNQuaternion_X_set), 1);
rb_define_method(gQuaternionClass, "x", LN_TO_RUBY_FUNC(LNQuaternion_X_get), 0);
rb_define_method(gQuaternionClass, "y=", LN_TO_RUBY_FUNC(LNQuaternion_Y_set), 1);
rb_define_method(gQuaternionClass, "y", LN_TO_RUBY_FUNC(LNQuaternion_Y_get), 0);
rb_define_method(gQuaternionClass, "z=", LN_TO_RUBY_FUNC(LNQuaternion_Z_set), 1);
rb_define_method(gQuaternionClass, "z", LN_TO_RUBY_FUNC(LNQuaternion_Z_get), 0);
rb_define_method(gQuaternionClass, "w=", LN_TO_RUBY_FUNC(LNQuaternion_W_set), 1);
rb_define_method(gQuaternionClass, "w", LN_TO_RUBY_FUNC(LNQuaternion_W_get), 0);
    rb_define_method(gQuaternionClass, "set", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Set), -1);
    rb_define_singleton_method(gQuaternionClass, "tity", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Identity), -1);
    rb_define_singleton_method(gQuaternionClass, "rotation_axis", LN_TO_RUBY_FUNC(lnrbLNQuaternion_RotationAxis), -1);
    rb_define_singleton_method(gQuaternionClass, "rotation_matrix", LN_TO_RUBY_FUNC(lnrbLNQuaternion_RotationMatrix), -1);
    rb_define_singleton_method(gQuaternionClass, "rotation_yaw_pitch_roll", LN_TO_RUBY_FUNC(lnrbLNQuaternion_RotationYawPitchRoll), -1);
    rb_define_singleton_method(gQuaternionClass, "normalize", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Normalize), -1);
    rb_define_singleton_method(gQuaternionClass, "conjugate", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Conjugate), -1);
    rb_define_singleton_method(gQuaternionClass, "multiply", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Multiply), -1);
    rb_define_singleton_method(gQuaternionClass, "slerp", LN_TO_RUBY_FUNC(lnrbLNQuaternion_Slerp), -1);

    gRandomClass = rb_define_class_under(gLNoteModule, "Random", gReferenceObjectClass);
    rb_define_alloc_func(gRandomClass, LNRandom_allocate);
    gRandomClassTypeInfo.Factory = LNRandom_allocate2;
    gRandomClassTypeInfo.Klass = &gRandomClass;
    LNRandom_SetTypeUserData((void*)(&gRandomClassTypeInfo));
    rb_define_private_method(gRandomClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNRandom_Create), -1);
    rb_define_method(gRandomClass, "seed=", LN_TO_RUBY_FUNC(lnrbLNRandom_SetSeed), -1);
    rb_define_method(gRandomClass, "get_int", LN_TO_RUBY_FUNC(lnrbLNRandom_GetInt), -1);
    rb_define_method(gRandomClass, "get_int_range", LN_TO_RUBY_FUNC(lnrbLNRandom_GetIntRange), -1);
    rb_define_method(gRandomClass, "get_int_deflect", LN_TO_RUBY_FUNC(lnrbLNRandom_GetIntDeflect), -1);

    gFileStreamClass = rb_define_class_under(gLNoteModule, "FileStream", gReferenceObjectClass);
    rb_define_alloc_func(gFileStreamClass, LNFileStream_allocate);
    gFileStreamClassTypeInfo.Factory = LNFileStream_allocate2;
    gFileStreamClassTypeInfo.Klass = &gFileStreamClass;
    LNFileStream_SetTypeUserData((void*)(&gFileStreamClassTypeInfo));
    rb_define_private_method(gFileStreamClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNFileStream_Create), -1);
    rb_define_method(gFileStreamClass, "close", LN_TO_RUBY_FUNC(lnrbLNFileStream_Close), -1);
    rb_define_method(gFileStreamClass, "size", LN_TO_RUBY_FUNC(lnrbLNFileStream_GetSize), -1);
    rb_define_method(gFileStreamClass, "position", LN_TO_RUBY_FUNC(lnrbLNFileStream_GetPosition), -1);
    rb_define_method(gFileStreamClass, "seek", LN_TO_RUBY_FUNC(lnrbLNFileStream_Seek), -1);
    rb_define_singleton_method(gFileStreamClass, "exists", LN_TO_RUBY_FUNC(lnrbLNFileStream_Exists), -1);

    gInputClass = rb_define_module_under(gLNoteModule, "Input");
    rb_define_singleton_method(gInputClass, "press?", LN_TO_RUBY_FUNC(lnrbLNInput_IsPress), -1);
    rb_define_singleton_method(gInputClass, "trigger?", LN_TO_RUBY_FUNC(lnrbLNInput_IsTrigger), -1);
    rb_define_singleton_method(gInputClass, "off_trigger?", LN_TO_RUBY_FUNC(lnrbLNInput_IsOffTrigger), -1);
    rb_define_singleton_method(gInputClass, "repeat?", LN_TO_RUBY_FUNC(lnrbLNInput_IsRepeat), -1);
    rb_define_singleton_method(gInputClass, "get_power", LN_TO_RUBY_FUNC(lnrbLNInput_GetPower), -1);
    rb_define_singleton_method(gInputClass, "get_axis_state", LN_TO_RUBY_FUNC(lnrbLNInput_GetAxisState), -1);

    gVirtualPadClass = rb_define_class_under(gLNoteModule, "VirtualPad", gReferenceObjectClass);
    rb_define_alloc_func(gVirtualPadClass, LNVirtualPad_allocate);
    gVirtualPadClassTypeInfo.Factory = LNVirtualPad_allocate2;
    gVirtualPadClassTypeInfo.Klass = &gVirtualPadClass;
    LNVirtualPad_SetTypeUserData((void*)(&gVirtualPadClassTypeInfo));
    rb_define_method(gVirtualPadClass, "press?", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_IsPress), -1);
    rb_define_method(gVirtualPadClass, "trigger?", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_IsTrigger), -1);
    rb_define_method(gVirtualPadClass, "off_trigger?", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_IsOffTrigger), -1);
    rb_define_method(gVirtualPadClass, "repeat?", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_IsRepeat), -1);
    rb_define_method(gVirtualPadClass, "get_power", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_GetPower), -1);
    rb_define_singleton_method(gVirtualPadClass, "get_axis_state", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_GetAxisState), -1);
    rb_define_method(gVirtualPadClass, "pressed_device_button", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_GetPressedDeviceButton), -1);
    rb_define_singleton_method(gVirtualPadClass, "get_device_button_type", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_GetDeviceButtonType), -1);
    rb_define_method(gVirtualPadClass, "attach_virtual_button", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_AttachVirtualButton), -1);
    rb_define_method(gVirtualPadClass, "detach_virtual_button", LN_TO_RUBY_FUNC(lnrbLNVirtualPad_DetachVirtualButton), -1);

    gAudioClass = rb_define_module_under(gLNoteModule, "Audio");
    rb_define_singleton_method(gAudioClass, "play_bgm", LN_TO_RUBY_FUNC(lnrbLNAudio_PlayBGM), -1);
    rb_define_singleton_method(gAudioClass, "stop_bgm", LN_TO_RUBY_FUNC(lnrbLNAudio_StopBGM), -1);
    rb_define_singleton_method(gAudioClass, "play_bgs", LN_TO_RUBY_FUNC(lnrbLNAudio_PlayBGS), -1);
    rb_define_singleton_method(gAudioClass, "stop_bgs", LN_TO_RUBY_FUNC(lnrbLNAudio_StopBGS), -1);
    rb_define_singleton_method(gAudioClass, "play_me", LN_TO_RUBY_FUNC(lnrbLNAudio_PlayME), -1);
    rb_define_singleton_method(gAudioClass, "stop_me", LN_TO_RUBY_FUNC(lnrbLNAudio_StopME), -1);
    rb_define_singleton_method(gAudioClass, "play_se", LN_TO_RUBY_FUNC(lnrbLNAudio_PlaySE), -1);
    rb_define_singleton_method(gAudioClass, "play_se3d", LN_TO_RUBY_FUNC(lnrbLNAudio_PlaySE3D), -1);
    rb_define_singleton_method(gAudioClass, "stop_se", LN_TO_RUBY_FUNC(lnrbLNAudio_StopSE), -1);
    rb_define_singleton_method(gAudioClass, "set_metre_unit_distance", LN_TO_RUBY_FUNC(lnrbLNAudio_SetMetreUnitDistance), -1);
    rb_define_singleton_method(gAudioClass, "set_bgm_volume", LN_TO_RUBY_FUNC(lnrbLNAudio_SetBGMVolume), -1);
    rb_define_singleton_method(gAudioClass, "set_bgs_volume", LN_TO_RUBY_FUNC(lnrbLNAudio_SetBGSVolume), -1);
    rb_define_singleton_method(gAudioClass, "get_internal_game_sound", LN_TO_RUBY_FUNC(lnrbLNAudio_GetInternalGameSound), -1);

    gSoundListenerClass = rb_define_module_under(gLNoteModule, "SoundListener");
    rb_define_singleton_method(gSoundListenerClass, "set_position", LN_TO_RUBY_FUNC(lnrbLNSoundListener_SetPosition), -1);
    rb_define_singleton_method(gSoundListenerClass, "set_direction", LN_TO_RUBY_FUNC(lnrbLNSoundListener_SetDirection), -1);
    rb_define_singleton_method(gSoundListenerClass, "set_up_direction", LN_TO_RUBY_FUNC(lnrbLNSoundListener_SetUpDirection), -1);
    rb_define_singleton_method(gSoundListenerClass, "velocity", LN_TO_RUBY_FUNC(lnrbLNSoundListener_Velocity), -1);

    gSoundClass = rb_define_class_under(gLNoteModule, "Sound", gReferenceObjectClass);
    rb_define_alloc_func(gSoundClass, LNSound_allocate);
    gSoundClassTypeInfo.Factory = LNSound_allocate2;
    gSoundClassTypeInfo.Klass = &gSoundClass;
    LNSound_SetTypeUserData((void*)(&gSoundClassTypeInfo));
    rb_define_private_method(gSoundClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNSound_Create), -1);
    rb_define_method(gSoundClass, "volume=", LN_TO_RUBY_FUNC(lnrbLNSound_SetVolume), -1);
    rb_define_method(gSoundClass, "volume", LN_TO_RUBY_FUNC(lnrbLNSound_GetVolume), -1);
    rb_define_method(gSoundClass, "pitch=", LN_TO_RUBY_FUNC(lnrbLNSound_SetPitch), -1);
    rb_define_method(gSoundClass, "pitch", LN_TO_RUBY_FUNC(lnrbLNSound_GetPitch), -1);
    rb_define_method(gSoundClass, "set_loop_state", LN_TO_RUBY_FUNC(lnrbLNSound_SetLoopState), -1);
    rb_define_method(gSoundClass, "loop?", LN_TO_RUBY_FUNC(lnrbLNSound_IsLoop), -1);
    rb_define_method(gSoundClass, "play_state", LN_TO_RUBY_FUNC(lnrbLNSound_GetPlayState), -1);
    rb_define_method(gSoundClass, "play", LN_TO_RUBY_FUNC(lnrbLNSound_Play), -1);
    rb_define_method(gSoundClass, "stop", LN_TO_RUBY_FUNC(lnrbLNSound_Stop), -1);
    rb_define_method(gSoundClass, "pause", LN_TO_RUBY_FUNC(lnrbLNSound_Pause), -1);
    rb_define_method(gSoundClass, "fade_volume", LN_TO_RUBY_FUNC(lnrbLNSound_FadeVolume), -1);
    rb_define_method(gSoundClass, "units_played", LN_TO_RUBY_FUNC(lnrbLNSound_GetUnitsPlayed), -1);
    rb_define_method(gSoundClass, "total_units", LN_TO_RUBY_FUNC(lnrbLNSound_GetTotalUnits), -1);
    rb_define_method(gSoundClass, "units_par_second", LN_TO_RUBY_FUNC(lnrbLNSound_GetUnitsParSecond), -1);
    rb_define_method(gSoundClass, "is_3d_sound?", LN_TO_RUBY_FUNC(lnrbLNSound_Is3DSound), -1);
    rb_define_method(gSoundClass, "emitter_position=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterPosition), -1);
    rb_define_method(gSoundClass, "emitter_velocity=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterVelocity), -1);
    rb_define_method(gSoundClass, "emitter_distance=", LN_TO_RUBY_FUNC(lnrbLNSound_SetEmitterDistance), -1);

    gColorClass = rb_define_class_under(gLNoteModule, "Color", rb_cObject);
    rb_define_alloc_func(gColorClass, LNColor_allocate);
rb_define_private_method(gColorClass, "initialize", LN_TO_RUBY_FUNC(LNColor_struct_initialize), -1);
rb_define_method(gColorClass, "r=", LN_TO_RUBY_FUNC(LNColor_R_set), 1);
rb_define_method(gColorClass, "r", LN_TO_RUBY_FUNC(LNColor_R_get), 0);
rb_define_method(gColorClass, "g=", LN_TO_RUBY_FUNC(LNColor_G_set), 1);
rb_define_method(gColorClass, "g", LN_TO_RUBY_FUNC(LNColor_G_get), 0);
rb_define_method(gColorClass, "b=", LN_TO_RUBY_FUNC(LNColor_B_set), 1);
rb_define_method(gColorClass, "b", LN_TO_RUBY_FUNC(LNColor_B_get), 0);
rb_define_method(gColorClass, "a=", LN_TO_RUBY_FUNC(LNColor_A_set), 1);
rb_define_method(gColorClass, "a", LN_TO_RUBY_FUNC(LNColor_A_get), 0);

    gColorFClass = rb_define_class_under(gLNoteModule, "ColorF", rb_cObject);
    rb_define_alloc_func(gColorFClass, LNColorF_allocate);
rb_define_private_method(gColorFClass, "initialize", LN_TO_RUBY_FUNC(LNColorF_struct_initialize), -1);
rb_define_method(gColorFClass, "r=", LN_TO_RUBY_FUNC(LNColorF_R_set), 1);
rb_define_method(gColorFClass, "r", LN_TO_RUBY_FUNC(LNColorF_R_get), 0);
rb_define_method(gColorFClass, "g=", LN_TO_RUBY_FUNC(LNColorF_G_set), 1);
rb_define_method(gColorFClass, "g", LN_TO_RUBY_FUNC(LNColorF_G_get), 0);
rb_define_method(gColorFClass, "b=", LN_TO_RUBY_FUNC(LNColorF_B_set), 1);
rb_define_method(gColorFClass, "b", LN_TO_RUBY_FUNC(LNColorF_B_get), 0);
rb_define_method(gColorFClass, "a=", LN_TO_RUBY_FUNC(LNColorF_A_set), 1);
rb_define_method(gColorFClass, "a", LN_TO_RUBY_FUNC(LNColorF_A_get), 0);

    gToneClass = rb_define_class_under(gLNoteModule, "Tone", rb_cObject);
    rb_define_alloc_func(gToneClass, LNTone_allocate);
rb_define_private_method(gToneClass, "initialize", LN_TO_RUBY_FUNC(LNTone_struct_initialize), -1);
rb_define_method(gToneClass, "r=", LN_TO_RUBY_FUNC(LNTone_R_set), 1);
rb_define_method(gToneClass, "r", LN_TO_RUBY_FUNC(LNTone_R_get), 0);
rb_define_method(gToneClass, "g=", LN_TO_RUBY_FUNC(LNTone_G_set), 1);
rb_define_method(gToneClass, "g", LN_TO_RUBY_FUNC(LNTone_G_get), 0);
rb_define_method(gToneClass, "b=", LN_TO_RUBY_FUNC(LNTone_B_set), 1);
rb_define_method(gToneClass, "b", LN_TO_RUBY_FUNC(LNTone_B_get), 0);
rb_define_method(gToneClass, "mono=", LN_TO_RUBY_FUNC(LNTone_Mono_set), 1);
rb_define_method(gToneClass, "mono", LN_TO_RUBY_FUNC(LNTone_Mono_get), 0);

    gTextureClass = rb_define_class_under(gLNoteModule, "Texture", gReferenceObjectClass);
    rb_define_alloc_func(gTextureClass, LNTexture_allocate);
    gTextureClassTypeInfo.Factory = LNTexture_allocate2;
    gTextureClassTypeInfo.Klass = &gTextureClass;
    LNTexture_SetTypeUserData((void*)(&gTextureClassTypeInfo));
    rb_define_private_method(gTextureClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNTexture_Create), -1);
    rb_define_method(gTextureClass, "width", LN_TO_RUBY_FUNC(lnrbLNTexture_GetWidth), -1);
    rb_define_method(gTextureClass, "height", LN_TO_RUBY_FUNC(lnrbLNTexture_GetHeight), -1);
    rb_define_method(gTextureClass, "real_width", LN_TO_RUBY_FUNC(lnrbLNTexture_GetRealWidth), -1);
    rb_define_method(gTextureClass, "real_height", LN_TO_RUBY_FUNC(lnrbLNTexture_GetRealHeight), -1);
    rb_define_method(gTextureClass, "clear", LN_TO_RUBY_FUNC(lnrbLNTexture_Clear), -1);
    rb_define_method(gTextureClass, "clear_rgba", LN_TO_RUBY_FUNC(lnrbLNTexture_ClearRGBA), -1);
    rb_define_method(gTextureClass, "draw_text", LN_TO_RUBY_FUNC(lnrbLNTexture_DrawText), -1);
    rb_define_method(gTextureClass, "bit_blt", LN_TO_RUBY_FUNC(lnrbLNTexture_BitBlt), -1);

    gFontClass = rb_define_class_under(gLNoteModule, "Font", gReferenceObjectClass);
    rb_define_alloc_func(gFontClass, LNFont_allocate);
    gFontClassTypeInfo.Factory = LNFont_allocate2;
    gFontClassTypeInfo.Klass = &gFontClass;
    LNFont_SetTypeUserData((void*)(&gFontClassTypeInfo));
    rb_define_private_method(gFontClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNFont_Create), -1);
    rb_define_singleton_method(gFontClass, "set_default_font", LN_TO_RUBY_FUNC(lnrbLNFont_SetDefaultFont), -1);
    rb_define_method(gFontClass, "name=", LN_TO_RUBY_FUNC(lnrbLNFont_SetName), -1);
    rb_define_method(gFontClass, "name", LN_TO_RUBY_FUNC(lnrbLNFont_GetName), -1);
    rb_define_method(gFontClass, "size=", LN_TO_RUBY_FUNC(lnrbLNFont_SetSize), -1);
    rb_define_method(gFontClass, "size", LN_TO_RUBY_FUNC(lnrbLNFont_GetSize), -1);
    rb_define_method(gFontClass, "color=", LN_TO_RUBY_FUNC(lnrbLNFont_SetColor), -1);
    rb_define_method(gFontClass, "color", LN_TO_RUBY_FUNC(lnrbLNFont_GetColor), -1);
    rb_define_method(gFontClass, "edge_color=", LN_TO_RUBY_FUNC(lnrbLNFont_SetEdgeColor), -1);
    rb_define_method(gFontClass, "edge_color", LN_TO_RUBY_FUNC(lnrbLNFont_GetEdgeColor), -1);
    rb_define_method(gFontClass, "edge_size=", LN_TO_RUBY_FUNC(lnrbLNFont_SetEdgeSize), -1);
    rb_define_method(gFontClass, "edge_size", LN_TO_RUBY_FUNC(lnrbLNFont_GetEdgeSize), -1);
    rb_define_method(gFontClass, "bold=", LN_TO_RUBY_FUNC(lnrbLNFont_SetBold), -1);
    rb_define_method(gFontClass, "bold?", LN_TO_RUBY_FUNC(lnrbLNFont_IsBold), -1);
    rb_define_method(gFontClass, "italic=", LN_TO_RUBY_FUNC(lnrbLNFont_SetItalic), -1);
    rb_define_method(gFontClass, "italic?", LN_TO_RUBY_FUNC(lnrbLNFont_IsItalic), -1);
    rb_define_method(gFontClass, "anti_alias=", LN_TO_RUBY_FUNC(lnrbLNFont_SetAntiAlias), -1);
    rb_define_method(gFontClass, "anti_alias?", LN_TO_RUBY_FUNC(lnrbLNFont_IsAntiAlias), -1);
    rb_define_singleton_method(gFontClass, "get_text_size", LN_TO_RUBY_FUNC(lnrbLNFont_GetTextSize), -1);

    gSceneGraphClass = rb_define_module_under(gLNoteModule, "SceneGraph");

    gSceneNodeClass = rb_define_class_under(gLNoteModule, "SceneNode", gReferenceObjectClass);
    rb_define_alloc_func(gSceneNodeClass, LNSceneNode_allocate);
    gSceneNodeClassTypeInfo.Factory = LNSceneNode_allocate2;
    gSceneNodeClassTypeInfo.Klass = &gSceneNodeClass;
    LNSceneNode_SetTypeUserData((void*)(&gSceneNodeClassTypeInfo));
    rb_define_method(gSceneNodeClass, "name", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetName), -1);
    rb_define_method(gSceneNodeClass, "name=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetName), -1);
    rb_define_method(gSceneNodeClass, "position=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetPosition), -1);
    rb_define_method(gSceneNodeClass, "position", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetPosition), -1);
    rb_define_method(gSceneNodeClass, "angle=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetAngle), -1);
    rb_define_method(gSceneNodeClass, "angle", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetAngle), -1);
    rb_define_method(gSceneNodeClass, "scale=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetScale), -1);
    rb_define_method(gSceneNodeClass, "scale", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetScale), -1);
    rb_define_method(gSceneNodeClass, "center=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetCenter), -1);
    rb_define_method(gSceneNodeClass, "center", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetCenter), -1);
    rb_define_method(gSceneNodeClass, "matrix=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetMatrix), -1);
    rb_define_method(gSceneNodeClass, "matrix", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetMatrix), -1);
    rb_define_method(gSceneNodeClass, "rotate_order=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetRotateOrder), -1);
    rb_define_method(gSceneNodeClass, "rotate_order", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetRotateOrder), -1);
    rb_define_method(gSceneNodeClass, "placement_type=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetPlacementType), -1);
    rb_define_method(gSceneNodeClass, "placement_type", LN_TO_RUBY_FUNC(lnrbLNSceneNode_GetPlacementType), -1);
    rb_define_method(gSceneNodeClass, "add_child", LN_TO_RUBY_FUNC(lnrbLNSceneNode_AddChild), -1);
    rb_define_method(gSceneNodeClass, "remove_child", LN_TO_RUBY_FUNC(lnrbLNSceneNode_RemoveChild), -1);
    rb_define_method(gSceneNodeClass, "update", LN_TO_RUBY_FUNC(lnrbLNSceneNode_Update), -1);
    rb_define_method(gSceneNodeClass, "enable_auto_update=", LN_TO_RUBY_FUNC(lnrbLNSceneNode_SetEnableAutoUpdate), -1);
    rb_define_method(gSceneNodeClass, "enable_auto_update?", LN_TO_RUBY_FUNC(lnrbLNSceneNode_IsEnableAutoUpdate), -1);

    gVisualNodeClass = rb_define_class_under(gLNoteModule, "VisualNode", gSceneNodeClass);
    rb_define_alloc_func(gVisualNodeClass, LNVisualNode_allocate);
    gVisualNodeClassTypeInfo.Factory = LNVisualNode_allocate2;
    gVisualNodeClassTypeInfo.Klass = &gVisualNodeClass;
    LNVisualNode_SetTypeUserData((void*)(&gVisualNodeClassTypeInfo));
    rb_define_method(gVisualNodeClass, "visible=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetVisible), -1);
    rb_define_method(gVisualNodeClass, "opacity=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetOpacity), -1);
    rb_define_method(gVisualNodeClass, "set_color_scale", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetColorScale), -1);
    rb_define_method(gVisualNodeClass, "set_blend_color", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetBlendColor), -1);
    rb_define_method(gVisualNodeClass, "set_tone", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetTone), -1);
    rb_define_method(gVisualNodeClass, "set_uv_transform", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetUVTransform), -1);
    rb_define_method(gVisualNodeClass, "blend_mode=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetBlendMode), -1);
    rb_define_method(gVisualNodeClass, "culling_mode=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetCullingMode), -1);
    rb_define_method(gVisualNodeClass, "enable_depth_test=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetEnableDepthTest), -1);
    rb_define_method(gVisualNodeClass, "enable_depth_write=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetEnableDepthWrite), -1);
    rb_define_method(gVisualNodeClass, "shader=", LN_TO_RUBY_FUNC(lnrbLNVisualNode_SetShader), -1);

    gSpriteClass = rb_define_class_under(gLNoteModule, "Sprite", gVisualNodeClass);
    rb_define_alloc_func(gSpriteClass, LNSprite_allocate);
    gSpriteClassTypeInfo.Factory = LNSprite_allocate2;
    gSpriteClassTypeInfo.Klass = &gSpriteClass;
    LNSprite_SetTypeUserData((void*)(&gSpriteClassTypeInfo));
    rb_define_private_method(gSpriteClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNSprite_Create), -1);
    rb_define_method(gSpriteClass, "texture=", LN_TO_RUBY_FUNC(lnrbLNSprite_SetTexture), -1);
    rb_define_method(gSpriteClass, "set_size", LN_TO_RUBY_FUNC(lnrbLNSprite_SetSize), -1);
    rb_define_method(gSpriteClass, "src_rect=", LN_TO_RUBY_FUNC(lnrbLNSprite_SetSrcRect), -1);
    rb_define_method(gSpriteClass, "src_rect", LN_TO_RUBY_FUNC(lnrbLNSprite_GetSrcRect), -1);
    rb_define_method(gSpriteClass, "flip_h=", LN_TO_RUBY_FUNC(lnrbLNSprite_SetFlipH), -1);
    rb_define_method(gSpriteClass, "flip_h?", LN_TO_RUBY_FUNC(lnrbLNSprite_IsFlipH), -1);
    rb_define_method(gSpriteClass, "flip_v=", LN_TO_RUBY_FUNC(lnrbLNSprite_SetFlipV), -1);
    rb_define_method(gSpriteClass, "flip_v?", LN_TO_RUBY_FUNC(lnrbLNSprite_IsFlipV), -1);

    gSpritePathClass = rb_define_class_under(gLNoteModule, "SpritePath", gVisualNodeClass);
    rb_define_alloc_func(gSpritePathClass, LNSpritePath_allocate);
    gSpritePathClassTypeInfo.Factory = LNSpritePath_allocate2;
    gSpritePathClassTypeInfo.Klass = &gSpritePathClass;
    LNSpritePath_SetTypeUserData((void*)(&gSpritePathClassTypeInfo));
    rb_define_private_method(gSpritePathClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNSpritePath_Create), -1);
    rb_define_method(gSpritePathClass, "path_blend_mode=", LN_TO_RUBY_FUNC(lnrbLNSpritePath_SetPathBlendMode), -1);
    rb_define_method(gSpritePathClass, "path_transform=", LN_TO_RUBY_FUNC(lnrbLNSpritePath_SetPathTransform), -1);
    rb_define_method(gSpritePathClass, "draw_sprite", LN_TO_RUBY_FUNC(lnrbLNSpritePath_DrawSprite), -1);

    gModelClass = rb_define_class_under(gLNoteModule, "Model", gVisualNodeClass);
    rb_define_alloc_func(gModelClass, LNModel_allocate);
    gModelClassTypeInfo.Factory = LNModel_allocate2;
    gModelClassTypeInfo.Klass = &gModelClass;
    LNModel_SetTypeUserData((void*)(&gModelClassTypeInfo));
    rb_define_private_method(gModelClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNModel_Create), -1);

    gEffectClass = rb_define_class_under(gLNoteModule, "Effect", gVisualNodeClass);
    rb_define_alloc_func(gEffectClass, LNEffect_allocate);
    gEffectClassTypeInfo.Factory = LNEffect_allocate2;
    gEffectClassTypeInfo.Klass = &gEffectClass;
    LNEffect_SetTypeUserData((void*)(&gEffectClassTypeInfo));
    rb_define_private_method(gEffectClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNEffect_Create), -1);
    rb_define_method(gEffectClass, "play", LN_TO_RUBY_FUNC(lnrbLNEffect_Play), -1);
    rb_define_method(gEffectClass, "stop", LN_TO_RUBY_FUNC(lnrbLNEffect_Stop), -1);
    rb_define_method(gEffectClass, "playing?", LN_TO_RUBY_FUNC(lnrbLNEffect_IsPlaying), -1);

    gTilemapClass = rb_define_class_under(gLNoteModule, "Tilemap", gVisualNodeClass);
    rb_define_alloc_func(gTilemapClass, LNTilemap_allocate);
    gTilemapClassTypeInfo.Factory = LNTilemap_allocate2;
    gTilemapClassTypeInfo.Klass = &gTilemapClass;
    LNTilemap_SetTypeUserData((void*)(&gTilemapClassTypeInfo));
    rb_define_private_method(gTilemapClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNTilemap_Create), -1);
    rb_define_method(gTilemapClass, "map_data=", LN_TO_RUBY_FUNC(lnrbLNTilemap_SetMapData), -1);
    rb_define_method(gTilemapClass, "tileset_texture=", LN_TO_RUBY_FUNC(lnrbLNTilemap_SetTilesetTexture), -1);
    rb_define_method(gTilemapClass, "tileset_horizontal_tile_count=", LN_TO_RUBY_FUNC(lnrbLNTilemap_SetTilesetHorizontalTileCount), -1);
    rb_define_method(gTilemapClass, "tile_pixel_size=", LN_TO_RUBY_FUNC(lnrbLNTilemap_SetTilePixelSize), -1);

    gShaderClass = rb_define_class_under(gLNoteModule, "Shader", gReferenceObjectClass);
    rb_define_alloc_func(gShaderClass, LNShader_allocate);
    gShaderClassTypeInfo.Factory = LNShader_allocate2;
    gShaderClassTypeInfo.Klass = &gShaderClass;
    LNShader_SetTypeUserData((void*)(&gShaderClassTypeInfo));
    rb_define_private_method(gShaderClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNShader_CreateFromFile), -1);

    gViewPaneClass = rb_define_class_under(gLNoteModule, "ViewPane", gReferenceObjectClass);
    rb_define_alloc_func(gViewPaneClass, LNViewPane_allocate);
    gViewPaneClassTypeInfo.Factory = LNViewPane_allocate2;
    gViewPaneClassTypeInfo.Klass = &gViewPaneClass;
    LNViewPane_SetTypeUserData((void*)(&gViewPaneClassTypeInfo));
    rb_define_singleton_method(gViewPaneClass, "default_view_pane", LN_TO_RUBY_FUNC(lnrbLNViewPane_GetDefaultViewPane), -1);
    rb_define_method(gViewPaneClass, "background_color=", LN_TO_RUBY_FUNC(lnrbLNViewPane_SetBackgroundColor), -1);
    rb_define_method(gViewPaneClass, "layers", LN_TO_RUBY_FUNC(lnrbLNViewPane_GetLayers), -1);

    gLayerClass = rb_define_class_under(gLNoteModule, "Layer", gReferenceObjectClass);
    rb_define_alloc_func(gLayerClass, LNLayer_allocate);
    gLayerClassTypeInfo.Factory = LNLayer_allocate2;
    gLayerClassTypeInfo.Klass = &gLayerClass;
    LNLayer_SetTypeUserData((void*)(&gLayerClassTypeInfo));
    rb_define_method(gLayerClass, "priority=", LN_TO_RUBY_FUNC(lnrbLNLayer_SetPriority), -1);
    rb_define_method(gLayerClass, "priority", LN_TO_RUBY_FUNC(lnrbLNLayer_GetPriority), -1);

    gViewFilterClass = rb_define_class_under(gLNoteModule, "ViewFilter", gLayerClass);
    rb_define_alloc_func(gViewFilterClass, LNViewFilter_allocate);
    gViewFilterClassTypeInfo.Factory = LNViewFilter_allocate2;
    gViewFilterClassTypeInfo.Klass = &gViewFilterClass;
    LNViewFilter_SetTypeUserData((void*)(&gViewFilterClassTypeInfo));
    rb_define_method(gViewFilterClass, "update", LN_TO_RUBY_FUNC(lnrbLNViewFilter_Update), -1);
    rb_define_method(gViewFilterClass, "enable_auto_update=", LN_TO_RUBY_FUNC(lnrbLNViewFilter_SetEnableAutoUpdate), -1);
    rb_define_method(gViewFilterClass, "enable_auto_update?", LN_TO_RUBY_FUNC(lnrbLNViewFilter_IsEnableAutoUpdate), -1);

    gToneFilterClass = rb_define_class_under(gLNoteModule, "ToneFilter", gViewFilterClass);
    rb_define_alloc_func(gToneFilterClass, LNToneFilter_allocate);
    gToneFilterClassTypeInfo.Factory = LNToneFilter_allocate2;
    gToneFilterClassTypeInfo.Klass = &gToneFilterClass;
    LNToneFilter_SetTypeUserData((void*)(&gToneFilterClassTypeInfo));
    rb_define_private_method(gToneFilterClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNToneFilter_Create), -1);
    rb_define_method(gToneFilterClass, "set_tone", LN_TO_RUBY_FUNC(lnrbLNToneFilter_SetTone), -1);

    gTransitionFilterClass = rb_define_class_under(gLNoteModule, "TransitionFilter", gViewFilterClass);
    rb_define_alloc_func(gTransitionFilterClass, LNTransitionFilter_allocate);
    gTransitionFilterClassTypeInfo.Factory = LNTransitionFilter_allocate2;
    gTransitionFilterClassTypeInfo.Klass = &gTransitionFilterClass;
    LNTransitionFilter_SetTypeUserData((void*)(&gTransitionFilterClassTypeInfo));
    rb_define_private_method(gTransitionFilterClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNTransitionFilter_Create), -1);
    rb_define_method(gTransitionFilterClass, "freeze", LN_TO_RUBY_FUNC(lnrbLNTransitionFilter_Freeze), -1);
    rb_define_method(gTransitionFilterClass, "transition", LN_TO_RUBY_FUNC(lnrbLNTransitionFilter_Transition), -1);

    gBlurFilterClass = rb_define_class_under(gLNoteModule, "BlurFilter", gViewFilterClass);
    rb_define_alloc_func(gBlurFilterClass, LNBlurFilter_allocate);
    gBlurFilterClassTypeInfo.Factory = LNBlurFilter_allocate2;
    gBlurFilterClassTypeInfo.Klass = &gBlurFilterClass;
    LNBlurFilter_SetTypeUserData((void*)(&gBlurFilterClassTypeInfo));
    rb_define_private_method(gBlurFilterClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNBlurFilter_Create), -1);
    rb_define_method(gBlurFilterClass, "blur", LN_TO_RUBY_FUNC(lnrbLNBlurFilter_Blur), -1);

    gCameraClass = rb_define_class_under(gLNoteModule, "Camera", gSceneNodeClass);
    rb_define_alloc_func(gCameraClass, LNCamera_allocate);
    gCameraClassTypeInfo.Factory = LNCamera_allocate2;
    gCameraClassTypeInfo.Klass = &gCameraClass;
    LNCamera_SetTypeUserData((void*)(&gCameraClassTypeInfo));
    rb_define_singleton_method(gCameraClass, "default_3d_camera", LN_TO_RUBY_FUNC(lnrbLNCamera_GetDefault3DCamera), -1);
    rb_define_singleton_method(gCameraClass, "default_2d_camera", LN_TO_RUBY_FUNC(lnrbLNCamera_GetDefault2DCamera), -1);
    rb_define_method(gCameraClass, "camera_type=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetCameraType), -1);
    rb_define_method(gCameraClass, "position=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetPosition), -1);
    rb_define_method(gCameraClass, "look_at=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetLookAt), -1);
    rb_define_method(gCameraClass, "up_direction=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetUpDirection), -1);
    rb_define_method(gCameraClass, "angle=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetAngle), -1);
    rb_define_method(gCameraClass, "matrix=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetMatrix), -1);
    rb_define_method(gCameraClass, "fov_y=", LN_TO_RUBY_FUNC(lnrbLNCamera_SetFovY), -1);
    rb_define_method(gCameraClass, "set_near_far_clip", LN_TO_RUBY_FUNC(lnrbLNCamera_SetNearFarClip), -1);
    rb_define_method(gCameraClass, "view_matrix", LN_TO_RUBY_FUNC(lnrbLNCamera_GetViewMatrix), -1);
    rb_define_method(gCameraClass, "projection_matrix", LN_TO_RUBY_FUNC(lnrbLNCamera_GetProjectionMatrix), -1);
    rb_define_method(gCameraClass, "view_projection_matrix", LN_TO_RUBY_FUNC(lnrbLNCamera_GetViewProjectionMatrix), -1);

    gConfigClass = rb_define_module_under(gLNoteModule, "Config");
    rb_define_singleton_method(gConfigClass, "set_window_title", LN_TO_RUBY_FUNC(lnrbLNConfig_SetWindowTitle), -1);
    rb_define_singleton_method(gConfigClass, "set_window_size", LN_TO_RUBY_FUNC(lnrbLNConfig_SetWindowSize), -1);
    rb_define_singleton_method(gConfigClass, "set_backbuffer_size", LN_TO_RUBY_FUNC(lnrbLNConfig_SetBackbufferSize), -1);
    rb_define_singleton_method(gConfigClass, "set_window_resize_enabled", LN_TO_RUBY_FUNC(lnrbLNConfig_SetWindowResizeEnabled), -1);
    rb_define_singleton_method(gConfigClass, "set_accelerator_key", LN_TO_RUBY_FUNC(lnrbLNConfig_SetAcceleratorKey), -1);
    rb_define_singleton_method(gConfigClass, "set_log_file_enabled", LN_TO_RUBY_FUNC(lnrbLNConfig_SetLogFileEnabled), -1);
    rb_define_singleton_method(gConfigClass, "set_console_enabled", LN_TO_RUBY_FUNC(lnrbLNConfig_SetConsoleEnabled), -1);
    rb_define_singleton_method(gConfigClass, "register_archive", LN_TO_RUBY_FUNC(lnrbLNConfig_RegisterArchive), -1);
    rb_define_singleton_method(gConfigClass, "set_file_access_priority", LN_TO_RUBY_FUNC(lnrbLNConfig_SetFileAccessPriority), -1);
    rb_define_singleton_method(gConfigClass, "set_direct_music_init_mode", LN_TO_RUBY_FUNC(lnrbLNConfig_SetDirectMusicInitMode), -1);
    rb_define_singleton_method(gConfigClass, "set_sound_max_cache_count", LN_TO_RUBY_FUNC(lnrbLNConfig_SetSoundMaxCacheCount), -1);
    rb_define_singleton_method(gConfigClass, "sound_play_type_threshold", LN_TO_RUBY_FUNC(lnrbLNConfig_SoundPlayTypeThreshold), -1);
    rb_define_singleton_method(gConfigClass, "set_frame_rate", LN_TO_RUBY_FUNC(lnrbLNConfig_SetFrameRate), -1);
    rb_define_singleton_method(gConfigClass, "set_enable_frame_wait", LN_TO_RUBY_FUNC(lnrbLNConfig_SetEnableFrameWait), -1);
    rb_define_singleton_method(gConfigClass, "set_auto_update_time_scale", LN_TO_RUBY_FUNC(lnrbLNConfig_SetAutoUpdateTimeScale), -1);
    rb_define_singleton_method(gConfigClass, "register_font_file", LN_TO_RUBY_FUNC(lnrbLNConfig_RegisterFontFile), -1);
    rb_define_singleton_method(gConfigClass, "set_texture_cache_size", LN_TO_RUBY_FUNC(lnrbLNConfig_SetTextureCacheSize), -1);
    rb_define_singleton_method(gConfigClass, "set_shader_cache_size", LN_TO_RUBY_FUNC(lnrbLNConfig_SetShaderCacheSize), -1);
    rb_define_singleton_method(gConfigClass, "set_exception_message_box_enabled", LN_TO_RUBY_FUNC(lnrbLNConfig_SetExceptionMessageBoxEnabled), -1);

    gCoreClass = rb_define_module_under(gLNoteModule, "Core");
    rb_define_singleton_method(gCoreClass, "initialize", LN_TO_RUBY_FUNC(lnrbLNCore_Initialize), -1);
    rb_define_singleton_method(gCoreClass, "initialize_file_io", LN_TO_RUBY_FUNC(lnrbLNCore_InitializeFileIO), -1);
    rb_define_singleton_method(gCoreClass, "initialize_audio", LN_TO_RUBY_FUNC(lnrbLNCore_InitializeAudio), -1);
    rb_define_singleton_method(gCoreClass, "initialize_input", LN_TO_RUBY_FUNC(lnrbLNCore_InitializeInput), -1);
    rb_define_singleton_method(gCoreClass, "update", LN_TO_RUBY_FUNC(lnrbLNCore_Update), -1);
    rb_define_singleton_method(gCoreClass, "reset_frame_delay", LN_TO_RUBY_FUNC(lnrbLNCore_ResetFrameDelay), -1);
    rb_define_singleton_method(gCoreClass, "end_requested?", LN_TO_RUBY_FUNC(lnrbLNCore_IsEndRequested), -1);
    rb_define_singleton_method(gCoreClass, "terminate", LN_TO_RUBY_FUNC(lnrbLNCore_Terminate), -1);



    gEnum_ErrorCode = rb_define_module_under(gLNoteModule, "ErrorCode");
    rb_define_const(gEnum_ErrorCode, "OK", INT2FIX(0));
    rb_define_const(gEnum_ErrorCode, "UNKNOWN", INT2FIX(-1));
    rb_define_const(gEnum_ErrorCode, "INVALID_OPERATION", INT2FIX(-2));
    rb_define_const(gEnum_ErrorCode, "ARGUMENT", INT2FIX(-3));
    rb_define_const(gEnum_ErrorCode, "OUT_OF_MEMORY", INT2FIX(-2));
    rb_define_const(gEnum_ErrorCode, "FILE_NOT_FOUND", INT2FIX(-3));
    rb_define_const(gEnum_ErrorCode, "SYSTEM_CALL", INT2FIX(-6));
    rb_define_const(gEnum_ErrorCode, "NOT_IMPLEMENTED", INT2FIX(-7));
    rb_define_const(gEnum_ErrorCode, "INVALID_FORMAT", INT2FIX(-8));
    rb_define_const(gEnum_ErrorCode, "THREAD", INT2FIX(-9));
    rb_define_const(gEnum_ErrorCode, "FATAL", INT2FIX(-10));
    rb_define_const(gEnum_ErrorCode, "COM_CALL", INT2FIX(-11));
    rb_define_const(gEnum_ErrorCode, "OPENGL", INT2FIX(-12));
    rb_define_const(gEnum_ErrorCode, "DIRECTX", INT2FIX(-13));
    rb_define_const(gEnum_ErrorCode, "WIN32API", INT2FIX(-14));

    gEnum_FileAccessPriority = rb_define_module_under(gLNoteModule, "FileAccessPriority");
    rb_define_const(gEnum_FileAccessPriority, "ARCHIVE_FIRST", INT2FIX(0));
    rb_define_const(gEnum_FileAccessPriority, "DIRECTORY_FIRST", INT2FIX(1));
    rb_define_const(gEnum_FileAccessPriority, "ARCHIVE_ONLY", INT2FIX(2));

    gEnum_FileType = rb_define_module_under(gLNoteModule, "FileType");
    rb_define_const(gEnum_FileType, "BINARY", INT2FIX(0));
    rb_define_const(gEnum_FileType, "TEXT", INT2FIX(1));

    gEnum_FileMode = rb_define_module_under(gLNoteModule, "FileMode");
    rb_define_const(gEnum_FileMode, "OPEN", INT2FIX(0));
    rb_define_const(gEnum_FileMode, "CREATE", INT2FIX(1));
    rb_define_const(gEnum_FileMode, "APPEND", INT2FIX(2));
    rb_define_const(gEnum_FileMode, "MAX", INT2FIX(3));

    gEnum_FileAccess = rb_define_module_under(gLNoteModule, "FileAccess");
    rb_define_const(gEnum_FileAccess, "READ", INT2FIX(0));
    rb_define_const(gEnum_FileAccess, "READWRITE", INT2FIX(1));
    rb_define_const(gEnum_FileAccess, "WRITE", INT2FIX(2));

    gEnum_SeekOrigin = rb_define_module_under(gLNoteModule, "SeekOrigin");
    rb_define_const(gEnum_SeekOrigin, "BEGIN", INT2FIX(0));
    rb_define_const(gEnum_SeekOrigin, "CURRENT", INT2FIX(1));
    rb_define_const(gEnum_SeekOrigin, "END", INT2FIX(2));

    gEnum_AsyncIOState = rb_define_module_under(gLNoteModule, "AsyncIOState");
    rb_define_const(gEnum_AsyncIOState, "IDLE", INT2FIX(0));
    rb_define_const(gEnum_AsyncIOState, "READY", INT2FIX(1));
    rb_define_const(gEnum_AsyncIOState, "PROCESSING", INT2FIX(2));
    rb_define_const(gEnum_AsyncIOState, "COMPLETED", INT2FIX(3));
    rb_define_const(gEnum_AsyncIOState, "FAILED", INT2FIX(4));

    gEnum_RotationOrder = rb_define_module_under(gLNoteModule, "RotationOrder");
    rb_define_const(gEnum_RotationOrder, "XYZ", INT2FIX(0));
    rb_define_const(gEnum_RotationOrder, "XZY", INT2FIX(1));
    rb_define_const(gEnum_RotationOrder, "YXZ", INT2FIX(2));
    rb_define_const(gEnum_RotationOrder, "YZX", INT2FIX(3));
    rb_define_const(gEnum_RotationOrder, "ZXY", INT2FIX(4));
    rb_define_const(gEnum_RotationOrder, "ZYX", INT2FIX(5));

    gEnum_KeyCode = rb_define_module_under(gLNoteModule, "KeyCode");
    rb_define_const(gEnum_KeyCode, "UNKNOWN", INT2FIX(0));
    rb_define_const(gEnum_KeyCode, "A", INT2FIX(1));
    rb_define_const(gEnum_KeyCode, "B", INT2FIX(2));
    rb_define_const(gEnum_KeyCode, "C", INT2FIX(3));
    rb_define_const(gEnum_KeyCode, "D", INT2FIX(4));
    rb_define_const(gEnum_KeyCode, "E", INT2FIX(5));
    rb_define_const(gEnum_KeyCode, "F", INT2FIX(6));
    rb_define_const(gEnum_KeyCode, "G", INT2FIX(7));
    rb_define_const(gEnum_KeyCode, "H", INT2FIX(8));
    rb_define_const(gEnum_KeyCode, "I", INT2FIX(9));
    rb_define_const(gEnum_KeyCode, "J", INT2FIX(10));
    rb_define_const(gEnum_KeyCode, "K", INT2FIX(11));
    rb_define_const(gEnum_KeyCode, "L", INT2FIX(12));
    rb_define_const(gEnum_KeyCode, "M", INT2FIX(13));
    rb_define_const(gEnum_KeyCode, "N", INT2FIX(14));
    rb_define_const(gEnum_KeyCode, "O", INT2FIX(15));
    rb_define_const(gEnum_KeyCode, "P", INT2FIX(16));
    rb_define_const(gEnum_KeyCode, "Q", INT2FIX(17));
    rb_define_const(gEnum_KeyCode, "R", INT2FIX(18));
    rb_define_const(gEnum_KeyCode, "S", INT2FIX(19));
    rb_define_const(gEnum_KeyCode, "T", INT2FIX(20));
    rb_define_const(gEnum_KeyCode, "U", INT2FIX(21));
    rb_define_const(gEnum_KeyCode, "V", INT2FIX(22));
    rb_define_const(gEnum_KeyCode, "W", INT2FIX(23));
    rb_define_const(gEnum_KeyCode, "X", INT2FIX(24));
    rb_define_const(gEnum_KeyCode, "Y", INT2FIX(25));
    rb_define_const(gEnum_KeyCode, "Z", INT2FIX(26));
    rb_define_const(gEnum_KeyCode, "D0", INT2FIX(27));
    rb_define_const(gEnum_KeyCode, "D1", INT2FIX(28));
    rb_define_const(gEnum_KeyCode, "D2", INT2FIX(29));
    rb_define_const(gEnum_KeyCode, "D3", INT2FIX(30));
    rb_define_const(gEnum_KeyCode, "D4", INT2FIX(31));
    rb_define_const(gEnum_KeyCode, "D5", INT2FIX(32));
    rb_define_const(gEnum_KeyCode, "D6", INT2FIX(33));
    rb_define_const(gEnum_KeyCode, "D7", INT2FIX(34));
    rb_define_const(gEnum_KeyCode, "D8", INT2FIX(35));
    rb_define_const(gEnum_KeyCode, "D9", INT2FIX(36));
    rb_define_const(gEnum_KeyCode, "F1", INT2FIX(37));
    rb_define_const(gEnum_KeyCode, "F2", INT2FIX(38));
    rb_define_const(gEnum_KeyCode, "F3", INT2FIX(39));
    rb_define_const(gEnum_KeyCode, "F4", INT2FIX(40));
    rb_define_const(gEnum_KeyCode, "F5", INT2FIX(41));
    rb_define_const(gEnum_KeyCode, "F6", INT2FIX(42));
    rb_define_const(gEnum_KeyCode, "F7", INT2FIX(43));
    rb_define_const(gEnum_KeyCode, "F8", INT2FIX(44));
    rb_define_const(gEnum_KeyCode, "F9", INT2FIX(45));
    rb_define_const(gEnum_KeyCode, "F10", INT2FIX(46));
    rb_define_const(gEnum_KeyCode, "F11", INT2FIX(47));
    rb_define_const(gEnum_KeyCode, "F12", INT2FIX(48));
    rb_define_const(gEnum_KeyCode, "SPACE", INT2FIX(49));
    rb_define_const(gEnum_KeyCode, "ESCAPE", INT2FIX(50));
    rb_define_const(gEnum_KeyCode, "UP", INT2FIX(51));
    rb_define_const(gEnum_KeyCode, "DOWN", INT2FIX(52));
    rb_define_const(gEnum_KeyCode, "LEFT", INT2FIX(53));
    rb_define_const(gEnum_KeyCode, "RIGHT", INT2FIX(54));
    rb_define_const(gEnum_KeyCode, "LSHIFT", INT2FIX(55));
    rb_define_const(gEnum_KeyCode, "RSHIFT", INT2FIX(56));
    rb_define_const(gEnum_KeyCode, "LCTRL", INT2FIX(57));
    rb_define_const(gEnum_KeyCode, "RCTRL", INT2FIX(58));
    rb_define_const(gEnum_KeyCode, "LALT", INT2FIX(59));
    rb_define_const(gEnum_KeyCode, "RALT", INT2FIX(60));
    rb_define_const(gEnum_KeyCode, "TAB", INT2FIX(61));
    rb_define_const(gEnum_KeyCode, "ENTER", INT2FIX(62));
    rb_define_const(gEnum_KeyCode, "BACKSPACE", INT2FIX(63));
    rb_define_const(gEnum_KeyCode, "INSERT", INT2FIX(64));
    rb_define_const(gEnum_KeyCode, "DELETE", INT2FIX(65));
    rb_define_const(gEnum_KeyCode, "PAGEUP", INT2FIX(66));
    rb_define_const(gEnum_KeyCode, "PAGEDOWN", INT2FIX(67));
    rb_define_const(gEnum_KeyCode, "HOME", INT2FIX(68));
    rb_define_const(gEnum_KeyCode, "END", INT2FIX(69));
    rb_define_const(gEnum_KeyCode, "COLON", INT2FIX(70));
    rb_define_const(gEnum_KeyCode, "SEMICOLON", INT2FIX(71));
    rb_define_const(gEnum_KeyCode, "COMMA", INT2FIX(72));
    rb_define_const(gEnum_KeyCode, "PERIOD", INT2FIX(73));
    rb_define_const(gEnum_KeyCode, "SLASH", INT2FIX(74));
    rb_define_const(gEnum_KeyCode, "MINUS", INT2FIX(75));
    rb_define_const(gEnum_KeyCode, "BACKSLASH", INT2FIX(76));
    rb_define_const(gEnum_KeyCode, "YEN", INT2FIX(77));
    rb_define_const(gEnum_KeyCode, "CARET", INT2FIX(78));
    rb_define_const(gEnum_KeyCode, "LBRACKET", INT2FIX(79));
    rb_define_const(gEnum_KeyCode, "RBRACKET", INT2FIX(80));
    rb_define_const(gEnum_KeyCode, "ANDROID_PLUS", INT2FIX(81));

    gEnum_AcceleratorKey = rb_define_module_under(gLNoteModule, "AcceleratorKey");
    rb_define_const(gEnum_AcceleratorKey, "TOGGLE_FULLSCREEN", INT2FIX(0));
    rb_define_const(gEnum_AcceleratorKey, "SHOW_DEBUG_STATISTICS", INT2FIX(1));

    gEnum_Button = rb_define_module_under(gLNoteModule, "Button");
    rb_define_const(gEnum_Button, "A", INT2FIX(0));
    rb_define_const(gEnum_Button, "B", INT2FIX(1));
    rb_define_const(gEnum_Button, "C", INT2FIX(2));
    rb_define_const(gEnum_Button, "X", INT2FIX(3));
    rb_define_const(gEnum_Button, "Y", INT2FIX(4));
    rb_define_const(gEnum_Button, "Z", INT2FIX(5));
    rb_define_const(gEnum_Button, "L", INT2FIX(6));
    rb_define_const(gEnum_Button, "R", INT2FIX(7));
    rb_define_const(gEnum_Button, "L2", INT2FIX(8));
    rb_define_const(gEnum_Button, "R2", INT2FIX(9));
    rb_define_const(gEnum_Button, "L3", INT2FIX(10));
    rb_define_const(gEnum_Button, "R3", INT2FIX(11));
    rb_define_const(gEnum_Button, "START", INT2FIX(12));
    rb_define_const(gEnum_Button, "SELECT", INT2FIX(13));
    rb_define_const(gEnum_Button, "SUB1", INT2FIX(14));
    rb_define_const(gEnum_Button, "SUB2", INT2FIX(15));
    rb_define_const(gEnum_Button, "LEFT", INT2FIX(16));
    rb_define_const(gEnum_Button, "RIGHT", INT2FIX(17));
    rb_define_const(gEnum_Button, "UP", INT2FIX(18));
    rb_define_const(gEnum_Button, "DOWN", INT2FIX(19));
    rb_define_const(gEnum_Button, "AXIS_1X_MINUIS", INT2FIX(20));
    rb_define_const(gEnum_Button, "AXIS_1X_PLUS", INT2FIX(21));
    rb_define_const(gEnum_Button, "AXIS_1Y_MINUIS", INT2FIX(22));
    rb_define_const(gEnum_Button, "AXIS_1Y_PLUS", INT2FIX(23));
    rb_define_const(gEnum_Button, "AXIS_2X_MINUIS", INT2FIX(24));
    rb_define_const(gEnum_Button, "AXIS_2X_PLUS", INT2FIX(25));
    rb_define_const(gEnum_Button, "AXIS_2Y_MINUIS", INT2FIX(26));
    rb_define_const(gEnum_Button, "AXIS_2Y_PLUS", INT2FIX(27));

    gEnum_DeviceButtonType = rb_define_module_under(gLNoteModule, "DeviceButtonType");
    rb_define_const(gEnum_DeviceButtonType, "UNKNOWN", INT2FIX(0));
    rb_define_const(gEnum_DeviceButtonType, "KEYBOARD", INT2FIX(1));
    rb_define_const(gEnum_DeviceButtonType, "MOUSE", INT2FIX(2));
    rb_define_const(gEnum_DeviceButtonType, "JOYPAD_BUTTON", INT2FIX(3));
    rb_define_const(gEnum_DeviceButtonType, "JOYPAD_POV", INT2FIX(4));
    rb_define_const(gEnum_DeviceButtonType, "JOYPAD_AXIS", INT2FIX(5));

    gEnum_DeviceButton = rb_define_module_under(gLNoteModule, "DeviceButton");
    rb_define_const(gEnum_DeviceButton, "UNKNOWN", INT2FIX(0));
    rb_define_const(gEnum_DeviceButton, "MOUSE_1", INT2FIX(256));
    rb_define_const(gEnum_DeviceButton, "MOUSE_2", INT2FIX(257));
    rb_define_const(gEnum_DeviceButton, "MOUSE_3", INT2FIX(258));
    rb_define_const(gEnum_DeviceButton, "MOUSE_4", INT2FIX(259));
    rb_define_const(gEnum_DeviceButton, "MOUSE_5", INT2FIX(260));
    rb_define_const(gEnum_DeviceButton, "MOUSE_6", INT2FIX(261));
    rb_define_const(gEnum_DeviceButton, "MOUSE_7", INT2FIX(262));
    rb_define_const(gEnum_DeviceButton, "MOUSE_8", INT2FIX(263));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_1", INT2FIX(264));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_2", INT2FIX(265));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_3", INT2FIX(266));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_4", INT2FIX(267));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_5", INT2FIX(268));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_6", INT2FIX(269));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_7", INT2FIX(270));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_8", INT2FIX(271));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_9", INT2FIX(272));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_10", INT2FIX(273));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_11", INT2FIX(274));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_12", INT2FIX(275));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_13", INT2FIX(276));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_14", INT2FIX(277));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_15", INT2FIX(278));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_BUTTON_16", INT2FIX(279));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_POV_UP", INT2FIX(280));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_POV_RIGHT", INT2FIX(281));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_POV_DOWN", INT2FIX(282));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_POV_LEFT", INT2FIX(283));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_1_MINUIS", INT2FIX(284));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_1_PLUS", INT2FIX(285));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_2_MINUIS", INT2FIX(286));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_2_PLUS", INT2FIX(287));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_3_MINUIS", INT2FIX(288));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_3_PLUS", INT2FIX(289));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_4_MINUIS", INT2FIX(290));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_4_PLUS", INT2FIX(291));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_5_MINUIS", INT2FIX(292));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_5_PLUS", INT2FIX(293));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_6_MINUIS", INT2FIX(294));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_6_PLUS", INT2FIX(295));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_7_MINUIS", INT2FIX(296));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_7_PLUS", INT2FIX(297));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_8_MINUIS", INT2FIX(298));
    rb_define_const(gEnum_DeviceButton, "JOYPAD_AXIS_8_PLUS", INT2FIX(299));

    gEnum_DirectMusicInitMode = rb_define_module_under(gLNoteModule, "DirectMusicInitMode");
    rb_define_const(gEnum_DirectMusicInitMode, "NOT_USE", INT2FIX(0));
    rb_define_const(gEnum_DirectMusicInitMode, "NORMAL", INT2FIX(1));
    rb_define_const(gEnum_DirectMusicInitMode, "THREAD_WAIT", INT2FIX(2));
    rb_define_const(gEnum_DirectMusicInitMode, "THREAD_REQUEST", INT2FIX(3));

    gEnum_SoundPlayingType = rb_define_module_under(gLNoteModule, "SoundPlayingType");
    rb_define_const(gEnum_SoundPlayingType, "UNKNOWN", INT2FIX(0));
    rb_define_const(gEnum_SoundPlayingType, "AUTO", INT2FIX(1));
    rb_define_const(gEnum_SoundPlayingType, "ONMEMORY", INT2FIX(2));
    rb_define_const(gEnum_SoundPlayingType, "STREAMING", INT2FIX(3));
    rb_define_const(gEnum_SoundPlayingType, "MIDI", INT2FIX(4));

    gEnum_SoundPlayingState = rb_define_module_under(gLNoteModule, "SoundPlayingState");
    rb_define_const(gEnum_SoundPlayingState, "STOPPED", INT2FIX(0));
    rb_define_const(gEnum_SoundPlayingState, "PLAYING", INT2FIX(1));
    rb_define_const(gEnum_SoundPlayingState, "PAUSING", INT2FIX(2));

    gEnum_SoundFadeState = rb_define_module_under(gLNoteModule, "SoundFadeState");
    rb_define_const(gEnum_SoundFadeState, "CONTINUE", INT2FIX(0));
    rb_define_const(gEnum_SoundFadeState, "STOP", INT2FIX(1));
    rb_define_const(gEnum_SoundFadeState, "STOP_RESET", INT2FIX(2));
    rb_define_const(gEnum_SoundFadeState, "PAUSE", INT2FIX(3));
    rb_define_const(gEnum_SoundFadeState, "PAUSE_RESET", INT2FIX(4));

    gEnum_InternalGameSound = rb_define_module_under(gLNoteModule, "InternalGameSound");
    rb_define_const(gEnum_InternalGameSound, "BGM", INT2FIX(0));
    rb_define_const(gEnum_InternalGameSound, "BGS", INT2FIX(1));
    rb_define_const(gEnum_InternalGameSound, "ME", INT2FIX(2));

    gEnum_BackbufferResizeMode = rb_define_module_under(gLNoteModule, "BackbufferResizeMode");
    rb_define_const(gEnum_BackbufferResizeMode, "SCALING", INT2FIX(0));
    rb_define_const(gEnum_BackbufferResizeMode, "SCALING_WITH_LETTER_BOX", INT2FIX(1));
    rb_define_const(gEnum_BackbufferResizeMode, "RESIZE", INT2FIX(2));

    gEnum_TextureFormat = rb_define_module_under(gLNoteModule, "TextureFormat");
    rb_define_const(gEnum_TextureFormat, "UNKNOWN", INT2FIX(0));
    rb_define_const(gEnum_TextureFormat, "A8R8G8B8", INT2FIX(1));
    rb_define_const(gEnum_TextureFormat, "X8R8G8B8", INT2FIX(2));
    rb_define_const(gEnum_TextureFormat, "A16B16G16R16F", INT2FIX(3));
    rb_define_const(gEnum_TextureFormat, "A32B32G32R32F", INT2FIX(4));
    rb_define_const(gEnum_TextureFormat, "D24S8", INT2FIX(5));

    gEnum_BlendMode = rb_define_module_under(gLNoteModule, "BlendMode");
    rb_define_const(gEnum_BlendMode, "NORMAL", INT2FIX(0));
    rb_define_const(gEnum_BlendMode, "ALPHA", INT2FIX(1));
    rb_define_const(gEnum_BlendMode, "ADD", INT2FIX(2));
    rb_define_const(gEnum_BlendMode, "ADD_ALPHA_DISABLE", INT2FIX(3));
    rb_define_const(gEnum_BlendMode, "SUB", INT2FIX(4));
    rb_define_const(gEnum_BlendMode, "SUB_ALPHA_DISABLE", INT2FIX(5));
    rb_define_const(gEnum_BlendMode, "MUL", INT2FIX(6));
    rb_define_const(gEnum_BlendMode, "SCREEN", INT2FIX(7));
    rb_define_const(gEnum_BlendMode, "REVERSE", INT2FIX(8));

    gEnum_CullingMode = rb_define_module_under(gLNoteModule, "CullingMode");
    rb_define_const(gEnum_CullingMode, "FRONT", INT2FIX(0));
    rb_define_const(gEnum_CullingMode, "BACK", INT2FIX(1));
    rb_define_const(gEnum_CullingMode, "DOUBLE", INT2FIX(2));

    gEnum_TextAlign = rb_define_module_under(gLNoteModule, "TextAlign");
    rb_define_const(gEnum_TextAlign, "LEFT", INT2FIX(0));
    rb_define_const(gEnum_TextAlign, "CENTER", INT2FIX(1));
    rb_define_const(gEnum_TextAlign, "RIGHT", INT2FIX(2));

    gEnum_PlacementType = rb_define_module_under(gLNoteModule, "PlacementType");
    rb_define_const(gEnum_PlacementType, "NORMAL", INT2FIX(0));
    rb_define_const(gEnum_PlacementType, "BILLBOARD", INT2FIX(1));
    rb_define_const(gEnum_PlacementType, "Y_FIXED", INT2FIX(2));

    gEnum_FrontAxis = rb_define_module_under(gLNoteModule, "FrontAxis");
    rb_define_const(gEnum_FrontAxis, "X", INT2FIX(0));
    rb_define_const(gEnum_FrontAxis, "Y", INT2FIX(1));
    rb_define_const(gEnum_FrontAxis, "Z", INT2FIX(2));
    rb_define_const(gEnum_FrontAxis, "RX", INT2FIX(3));
    rb_define_const(gEnum_FrontAxis, "RY", INT2FIX(4));
    rb_define_const(gEnum_FrontAxis, "RZ", INT2FIX(5));

    gEnum_MeshCreateFlags = rb_define_module_under(gLNoteModule, "MeshCreateFlags");
    rb_define_const(gEnum_MeshCreateFlags, "SOFTWARE_SKINNING", INT2FIX(0x01));
    rb_define_const(gEnum_MeshCreateFlags, "COLLISION_MESH", INT2FIX(0x02));
    rb_define_const(gEnum_MeshCreateFlags, "IGNORE_TEXTURE_NOT_FOUND", INT2FIX(0x04));

    gEnum_CameraType = rb_define_module_under(gLNoteModule, "CameraType");
    rb_define_const(gEnum_CameraType, "POSITION_AND_LOOKAT", INT2FIX(0));
    rb_define_const(gEnum_CameraType, "POSITION_AND_ANGLE", INT2FIX(1));
    rb_define_const(gEnum_CameraType, "TRANSFORM_MATRIX", INT2FIX(2));

    gEnum_Coord = rb_define_module_under(gLNoteModule, "Coord");
    rb_define_const(gEnum_Coord, "SPACE_2D", INT2FIX(0));
    rb_define_const(gEnum_Coord, "SPACE_3D", INT2FIX(1));

    gEnum_ObjectType = rb_define_module_under(gLNoteModule, "ObjectType");
    rb_define_const(gEnum_ObjectType, "SOUND", INT2FIX(0));



    // 終了時に実行する関数
    rb_set_end_proc( LNote_Terminate, Qnil );

    // using namespace LNote;
	//rb_include_module( rb_cObject, gLNoteModule );
}