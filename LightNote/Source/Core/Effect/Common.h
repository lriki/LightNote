//==============================================================================
// Common 
//------------------------------------------------------------------------------
///**
//  @file       Common.h
//  @brief      Common
//*/
//==============================================================================

#pragma once

namespace LNote
{
namespace Core
{
namespace Effect
{
//class IEffect;
class Effect;
class Manager;

class EffectBase;

class EffekseerEffectEngine;
class EffekseerEffectCore;
class EffekseerEffectInstance;

} // namespace Effect
} // namespace Core
} // namespace LNote

#if 0

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
#include <vector>
#include "../Base/Container/NodeList.h"
#include "../Math/Random.h"
#include "../Animation/Scalar.h"
#include "../Graphics/Common/Common.h"

#define LN_EFFECT_USING_NODE_LIST 0	//�������C���X�^���X�𐶐��������� ~EffectNodeInstanceGroup() �ŁA���X�g�T�C�Y�� 1 �ɂ�������炸 2�񃋁[�v����o�O����

namespace tinyxml2
{
    class XMLElement;
    class XMLDocument;
}

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
enum LNEffectCommonTrack
{
    LN_EFFECT_TRACK_POSITION_X = 0,
    LN_EFFECT_TRACK_POSITION_Y,
    LN_EFFECT_TRACK_POSITION_Z,

    LN_EFFECT_TRACK_ANGLE_X,
    LN_EFFECT_TRACK_ANGLE_Y,
    LN_EFFECT_TRACK_ANGLE_Z,
    LN_EFFECT_TRACK_ANGLE_R,

    LN_EFFECT_TRACK_SCALE_X,
    LN_EFFECT_TRACK_SCALE_Y,
    LN_EFFECT_TRACK_SCALE_Z,

    LN_EFFECT_TRACK_COLOR_R,
    LN_EFFECT_TRACK_COLOR_G,
    LN_EFFECT_TRACK_COLOR_B,
    LN_EFFECT_TRACK_COLOR_A,

    LN_EFFECT_TRACK_TEXTURE_U,
    LN_EFFECT_TRACK_TEXTURE_V,

    LN_EFFECT_MAX_COMMON_TRACKS,
};

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------
namespace LNote
{
namespace Core
{
namespace Effect
{
class EffectCore;
class EffectNodeCore;
class IAnimationTrack;
class EffectAnimationTrackGroup;
class EffectInstance;
class EffectNodeInstance;
class EffectNodeInstanceContainer;
class EffectNodeInstanceGroup;
class EmitterCoreBase;
class EmitterInstanceBase;

enum AnimationTrackType
{
    ANIMTRACKTYPE_VALUE = 0,
    ANIMTRACKTYPE_EVENT,
};

/// (�����_���l�̎w��� �����l - �}��  10, 10 ���� 5�`15)
struct ValueAnimationKey
{
	// TODO: union �Ń������팸

    double          FramePos;           ///< �t���[���ʒu

    lnFloat         Value;              ///< �l
    lnFloat         ValueRand;          ///< Value �����_����

	lnFloat         Velocity;           ///< ���x
	lnFloat         VelocityRand;       ///< Velocity �����_����

	lnFloat         Accel;              ///< �����x
	lnFloat         AccelRand;          ///< Accel �����_����

	lnFloat         LeftSlope;          ///< ���� (�I��) ���x
	lnFloat         LeftSlopeRand;      ///< LeftSlope �����_����

	lnFloat         RightSlope;         ///< �E�� (�J�n) ���x
	lnFloat         RightSlopeRand;     ///< RightSlope �����_����

    LNAnimInterMode InterMode;          ///< ���̃L�[�Ƃ̕�ԕ��@ ()

	/// �����l
    ValueAnimationKey()
        : FramePos          ( 0 )
        , Value             ( 0 )
        , ValueRand         ( 0 )
        , Velocity          ( 0 )
        , VelocityRand      ( 0 )
        , Accel             ( 0 )
        , AccelRand         ( 0 )
        , LeftSlope         ( 0 )
        , LeftSlopeRand     ( 0 )
        , RightSlope        ( 0 )
        , RightSlopeRand    ( 0 )
        , InterMode         ( LN_ANIM_INTER_CATMULLROM )
    {}
};

struct TrackElement
{
    LRefTString         Name;
    LColorF              Color;
    AnimationTrackType  Type;
};

struct TrackGroupElement
{
    LRefTString         Name;
    LColorF              Color;
};

enum CommonTracks
{
    COMMONTRACKS_POSITION_X = 0,
    COMMONTRACKS_POSITION_Y,
    COMMONTRACKS_POSITION_Z,

    COMMONTRACKS_ROTATION_X,
    COMMONTRACKS_ROTATION_Y,
    COMMONTRACKS_ROTATION_Z,
    COMMONTRACKS_ROTATION_R,

    COMMONTRACKS_SCALING_X,
    COMMONTRACKS_SCALING_Y,
    COMMONTRACKS_SCALING_Z,

    COMMONTRACKS_COLOR_R,
    COMMONTRACKS_COLOR_G,
    COMMONTRACKS_COLOR_B,
    COMMONTRACKS_COLOR_A,

    COMMONTRACKS_TEXUV_X,
    COMMONTRACKS_TEXUV_Y,

    COMMONTRACKS_MAX,
};

enum CommonTrackGroup
{
    COMMONTRACKGROUP_POSITION = 0,
    COMMONTRACKGROUP_ROTATION,
    COMMONTRACKGROUP_SCALING,
    COMMONTRACKGROUP_COLOR,
    COMMONTRACKGROUP_TEXUV,
    COMMONTRACKGROUP_EVENT,

	LN_EFFECT_COMMONTRACK_GROUPE_NUM,
};

enum TrackGroupType
{
	TRACKGROUP_UNKNOWN = 0,
    TRACKGROUP_POSITION,
    TRACKGROUP_ROTATION,
    TRACKGROUP_SCALING,
    TRACKGROUP_COLOR,
    TRACKGROUP_TEXUV,
    TRACKGROUP_EVENT,

	TRACKGROUP_RING_EMITTER_ANGLE,
    TRACKGROUP_RING_EMITTER_OUTER_LOCATION,
    TRACKGROUP_RING_EMITTER_INNER_LOCATION,
    TRACKGROUP_RING_EMITTER_CENTER_RATIO,
    TRACKGROUP_RING_EMITTER_OUTER_COLOR,
    TRACKGROUP_RING_EMITTER_CENTER_COLOR,
    TRACKGROUP_RING_EMITTER_INNER_COLOR,
};

//typedef Animation::Scalar               AnimationTrack;
typedef std::vector< IAnimationTrack* >  AnimationTrackArray;
typedef std::vector< EffectAnimationTrackGroup* >   AnimationTrackGroupArray;

typedef std::vector< LRefTString >      StringArray;

typedef std::vector< EmitterInstanceBase* >       EmitterInstanceArray;
typedef std::vector< ValueAnimationKey >             AnimationKeyArray;
//typedef std::vector< CommonAnimationKeyArray >  MasterCommonAnimationTrackArray;
typedef std::vector< EffectNodeCore* >              EffectNodeCoreArray;






typedef std::vector< EffectNodeInstanceContainer* > EffectNodeInstanceContainerArray;
typedef std::vector< EffectNodeInstanceGroup* >     EffectNodeInstanceGroupArray;
typedef std::vector< EffectNodeInstance* >          EffectNodeInstanceArray;

#if LN_EFFECT_USING_NODE_LIST
typedef Base::NodeList< EffectNodeInstanceGroup >   NodeInstanceGroupDrawList;
typedef Base::NodeList< EffectNodeInstance >        NodeInstanceDrawList;
#else
typedef std::vector< EffectNodeInstanceGroup* >       NodeInstanceGroupDrawList;
typedef std::vector< EffectNodeInstance* >            NodeInstanceDrawList;
#endif

typedef double lnTime;

/*
// EffectNode ���� Animation::Scalar �́A�����_�������̒l���G�f�B�^�Ŏg�����߂̂��́B�Q�[�����ł͕K�v�Ȃ��B
struct AnimationTracks
{
    Animation::Scalar   PositionX;
    Animation::Scalar   PositionY;
    Animation::Scalar   PositionZ;

    Animation::Scalar   RotationX;
    Animation::Scalar   RotationY;
    Animation::Scalar   RotationZ;
    Animation::Scalar   RotationR;

    Animation::Scalar   ScaleX;
    Animation::Scalar   ScaleY;
    Animation::Scalar   ScaleZ;

    Animation::Scalar   ColorR;
    Animation::Scalar   ColorG;
    Animation::Scalar   ColorB;
    Animation::Scalar   ColorA;

    Animation::Scalar   TextureU;
    Animation::Scalar   TextureV;

	// �ȉ��A�l��ۑ����邽�߂Ɋ܂߂� (�����Ë�����Ȃ�G�~�b�^���Ƃ�union�ł�OK)
    Animation::Scalar   RingInner;
    Animation::Scalar   RingOuter;
    Animation::Scalar   RingCenterRatio;
};
*/

//-------------------------------------------------------------------------
//
//-------------------------------------------------------------------------

} // namespace Effect
} // namespace Core

} // namespace LNote

#endif

//==============================================================================
//
//==============================================================================