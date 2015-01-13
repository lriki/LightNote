//=============================================================================
//�y link �z
//=============================================================================

#pragma once


//-------------------------------------------------------------------------
// 

// ���C�u�����f�B���N�g���܂ł̃p�X���w�肳��Ă��Ȃ��ꍇ�̓R���p�C����ʂ����߂ɋ�Œ�`����
#ifdef LNOTE_LIB_DIR
    #define LNOTE_LIB_DIR_T LNOTE_LIB_DIR"/"
#else
	#define LNOTE_LIB_DIR_T ""
#endif

// VC2008
#if ( 1500 <= _MSC_VER ) && ( _MSC_VER < 1600 )
    #define LNOTE_MSVC_VER_STR "vc2008"
#endif

// VC2010
#if ( 1600 <= _MSC_VER ) && ( _MSC_VER < 1700 )
	#define LNOTE_MSVC_VER_STR "vc2010"
#endif

// lib �t�@�C���̖��O�����
#ifdef LNOTE_DEBUG
    #define LNOTE_DEPEND_LIB_NAME   LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_dependencies_d.lib"
    #define LNOTE_LIB_NAME 		    LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_static_d.lib"
    #define LNOTE_DLL_LIB_NAME 		LNOTE_LIB_DIR_T"LNote_d.lib"
    #define LNOTE_UTILITY_LIB_NAME 	LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_utility_d.lib"
#else
    #define LNOTE_DEPEND_LIB_NAME   LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_dependencies.lib"
    #define LNOTE_LIB_NAME 		    LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_static.lib"
    #define LNOTE_DLL_LIB_NAME 		LNOTE_LIB_DIR_T"LNote.lib"
    #define LNOTE_UTILITY_LIB_NAME 	LNOTE_LIB_DIR_T"LNote_"LNOTE_MSVC_VER_STR"_utility.lib"
#endif


//=============================================================================
//
//=============================================================================