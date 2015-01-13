//==============================================================================
// ScalarAnimation 
//------------------------------------------------------------------------------
///**
//  @file       ScalarAnimation.h
//  @brief      ScalarAnimation
//  @author     Riki
//*/
//==============================================================================

#pragma once

#include "Types.h"
#include "AnimationTimelineBase.h"

namespace LNote
{
namespace Core
{
namespace Animation
{

//==============================================================================
// ■ ScalarAnimationBase
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScalarAnimationBase
	: public AnimationTimelineBase
{
public:
	virtual ~ScalarAnimationBase() {}

public:

	/// 値の取得
    virtual lnFloat getValue() const = 0;

public:
	virtual ValueType getValueType() const { return ValueType_Float; }
};

//==============================================================================
// ■ ScalarAnimationKey
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
struct ScalarAnimationKey
{
public:

	double      FramePos;       ///< フレーム位置
    lnFloat     Value;          ///< 値
    lnFloat     Velocity;       ///< 速度
    lnFloat     Accel;          ///< 加速度
    lnFloat     LeftSlope;      ///< 左辺 (終了) 速度
    lnFloat     RightSlope;     ///< 右辺 (開始) 速度
    lnU8		InterMode;      ///< 次のキーとの補間方法 ( LNAnimInterMode の値 )
    
public:

	/// 初期化 ( InterMode は LN_ANIM_INTER_LINEAR、それ以外は 0.0 で初期化 )
    ScalarAnimationKey();
};

//==============================================================================
// ■ ScalarAnimationSourceKey
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
struct ScalarAnimationSourceKey
{
public:

	double          FramePos;           ///< フレーム位置

    lnFloat         Value;              ///< 値
    lnFloat         ValueRand;          ///< Value ランダム幅

	lnFloat         Velocity;           ///< 速度
	lnFloat         VelocityRand;       ///< Velocity ランダム幅

	lnFloat         Accel;              ///< 加速度
	lnFloat         AccelRand;          ///< Accel ランダム幅

	lnFloat         LeftSlope;          ///< 左辺 (終了) 速度
	lnFloat         LeftSlopeRand;      ///< LeftSlope ランダム幅

	lnFloat         RightSlope;         ///< 右辺 (開始) 速度
	lnFloat         RightSlopeRand;     ///< RightSlope ランダム幅

    LNAnimInterMode InterMode;          ///< 次のキーとの補間方法 ()

public:

	/// 初期化 ( InterMode は LN_ANIM_INTER_LINEAR、それ以外は 0.0 で初期化 )
    ScalarAnimationSourceKey();
};

//==============================================================================
// ■ ScalarAnimation
//------------------------------------------------------------------------------
///**
//  @brief      
//*/
//==============================================================================
class ScalarAnimation
	: public ScalarAnimationBase
{
public:
	static const lnChar* XML_ELEMENT_NAME;
	static const lnChar* XML_ELEMENT_NAME_KEY;

public:
	ScalarAnimation();
	virtual ~ScalarAnimation();

public:

	/// キーがひとつもないか、時間が最初のフレーム位置より前のときに返す値の設定
    void setDefaultValue( lnFloat value ) { mDefaultValue = value; }

	/// キーの追加 (終端より前に追加した場合はソートを行う)
    void addKeyFrame( double frame_pos, lnFloat value );

	/// キーの追加 (終端より前に追加した場合はソートを行う)
    void addKeyFrame( const ScalarAnimationKey& key );

	/// キーの数の取得
    int getKeyNum() const { return mKeyFrameList.size(); }

	/// 値の取得
    virtual lnFloat getValue() const { return mValue; }

	/// キーを全て削除する
	void clear() { mKeyFrameList.clear(); }

	/// キーにアクセスする
    ScalarAnimationKey& getKeyAt( int index );

	/// ソースキーの追加 (終端より前に追加した場合はソートを行う)
    void addSourceKeyFrame( const ScalarAnimationSourceKey& key );

	/// ソースキーのクリア
    void clearSourceKeys() { mSourceKeyFrameList.clear(); }

	/// ソースキーの取得
    ScalarAnimationSourceKey& getSourceKeyAt( int index ) { return mSourceKeyFrameList[ index ]; }

	/// ソースキー数の取得
    int getSourceKeyCount() const { return mSourceKeyFrameList.size(); }

	/// 全てのソースキーをキーフレームに反映させる (rand_seed が 0 の場合はランダム値を適用しない)
	void applySourceKeys( int rand_seed );

public:
	virtual void setTime( double time );
	virtual double getLastFramePos();
    virtual XML::Element* serializeXMLElement( XML::Document* doc );	// ソースキーを保存
    virtual void deserializeXMLElement( XML::Element* element );

private:
	typedef std::vector<ScalarAnimationKey>			KeyFrameList;  
	typedef std::vector<ScalarAnimationSourceKey>	SourceKeyFrameList;  

	KeyFrameList		mKeyFrameList;
	SourceKeyFrameList	mSourceKeyFrameList;
	lnFloat				mDefaultValue;
    lnFloat				mValue;
};

} // namespace Animation
} // namespace Core
} // namespace LNote

//==============================================================================
//
//==============================================================================