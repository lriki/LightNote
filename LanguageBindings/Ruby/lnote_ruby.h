
#pragma once

#define LN_TO_RUBY_FUNC( f ) reinterpret_cast< VALUE(__cdecl *)(...) >( f )

inline bool isRbObject(VALUE v) { return (TYPE(v) == T_DATA/*T_OBJECT*/) || (v == Qnil); }
inline bool isRbNumber(VALUE v) { return (TYPE(v) == T_FIXNUM || TYPE(v) == T_BIGNUM) || (v == Qnil); }
inline bool isRbFloat(VALUE v) { return (TYPE(v) == T_FLOAT) || (isRbNumber(v)) || (v == Qnil); }
inline bool isRbString(VALUE v) { return (TYPE(v) == T_STRING) || (v == Qnil); }
inline bool isRbBool(VALUE v) { return (TYPE(v) == T_FALSE || TYPE(v) == T_TRUE) || (v == Qnil); }

inline lnBool RbBooltoBool(VALUE v) { return (TYPE(v) == T_TRUE) ? LN_TRUE : LN_FALSE; }

inline VALUE toVALUE(const char* v)		{ return rb_str_new2(v); }
inline VALUE toVALUE(lnBool v)			{ return (v != LN_FALSE) ? Qtrue : Qfalse; }
inline VALUE toVALUE(lnU8 v)			{ return INT2FIX(v); }
inline VALUE toVALUE(int v)				{ return INT2NUM(v); }
inline VALUE toVALUE(lnU32 v)			{ return INT2NUM(v); }
inline VALUE toVALUE(float v)			{ return DBL2NUM(v); }
inline VALUE toVALUE(double v)			{ return DBL2NUM(v); }
inline VALUE toVALUE(lnErrorCode v)		{ return INT2FIX(v); }
//inline VALUE toVALUE(lnIntPtr v)		{ return INT2NUM(v); }




