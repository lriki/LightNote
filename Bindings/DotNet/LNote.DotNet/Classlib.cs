using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    /// <summary>
    /// 2次元のベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float X;

        public float Y;


        /// <summary>
        /// すべての要素をデフォルト値 (0) で初期化します。
        /// </summary>
        public Vector2()
        {
            X = default(float);
            Y = default(float);
        }
        /// <summary>
        /// 2D ベクトルに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        public void Set( float x,  float y)
        {
        }

        /// <summary>
        /// 2D ベクトルの長さを計算する
        /// </summary>
        public float GetLength()
        {
        }

        /// <summary>
        /// 2D ベクトルの長さの2乗を計算する
        /// </summary>
        public float GetSquareLength()
        {
        }

        /// <summary>
        /// 2D ベクトルを正規化する
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        public static Vector2 Normalize( Vector2 vec)
        {
        }


    };

    /// <summary>
    /// 3 次元ベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
        public float X;

        public float Y;

        public float Z;


        /// <summary>
        /// すべての要素をデフォルト値 (0) で初期化します。
        /// </summary>
        public Vector3()
        {
            X = default(float);
            Y = default(float);
            Z = default(float);
        }
        /// <summary>
        /// 3Dベクトルに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        public void Set( float x,  float y,  float z)
        {
        }

        /// <summary>
        /// 3Dベクトルに値を設定する
        /// </summary>
        /// <param name="vec2">2D ベクトル</param>
        /// <param name="z">Z 値</param>
        public void Set( Vector2 vec2,  float z)
        {
        }

        /// <summary>
        /// 3Dベクトルの長さを計算する
        /// </summary>
        public float GetLength()
        {
        }

        /// <summary>
        /// 3D ベクトルの長さの2乗を計算する
        /// </summary>
        public float GetSquareLength()
        {
        }

        /// <summary>
        /// 3D ベクトルを正規化する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        public static Vector3 Normalize( Vector3 vec)
        {
        }

        /// <summary>
        /// 2 つの 3D ベクトルの内積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static float Dot( Vector3 vec1,  Vector3 vec2)
        {
        }

        /// <summary>
        /// 2 つの 3D ベクトルの外積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static Vector3 Cross( Vector3 vec1,  Vector3 vec2)
        {
        }

        /// <summary>
        /// 指定された法線で反射させたベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Reflect( Vector3 vec,  Vector3 normal)
        {
        }

        /// <summary>
        /// 指定された法線と方向から滑りベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Slide( Vector3 vec,  Vector3 normal)
        {
        }

        /// <summary>
        /// 2 つの 3D ベクトル間を線形補間する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        public static Vector3 Lerp( Vector3 vec1,  Vector3 vec2,  float t)
        {
        }

        /// <summary>
        /// 指定された 3D ベクトルを Catmull-Rom 補間する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec3">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="vec4">処理の基になる 3D ベクトル</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        public static Vector3 CatmullRom( Vector3 vec1,  Vector3 vec2,  Vector3 vec3,  Vector3 vec4,  float t)
        {
        }

        /// <summary>
        /// 指定された行列を使用して座標変換する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <remarks>
        /// ベクトルを ( x, y, z, 1 ) として座標変換します。
        /// 		            結果の w は出力されません。
        /// 		            結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
        /// 		            Vector3TransformCoord() を使用してください。
        /// </remarks>
        public static Vector4 Transform( Vector3 vec,  Matrix mat)
        {
        }

        /// <summary>
        /// 指定された行列を使用して座標変換し、結果を w = 1 に射影する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <remarks>
        /// ベクトルを ( x, y, z, 1 ) として座標変換し、
        /// 		            結果を w = 1 に射影 ( x y z を w で除算する ) します。
        /// </remarks>
        public static Vector3 TransformCoord( Vector3 vec,  Matrix mat)
        {
        }


    };

    /// <summary>
    /// 4 次元ベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
        public float X;

        public float Y;

        public float Z;

        public float W;


        /// <summary>
        /// すべての要素をデフォルト値 (0) で初期化します。
        /// </summary>
        public Vector4()
        {
            X = default(float);
            Y = default(float);
            Z = default(float);
            W = default(float);
        }
        /// <summary>
        /// 3Dベクトルに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        public void Set( float x,  float y,  float z,  float w)
        {
        }


    };

    /// <summary>
    /// 4x4 行列のクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Matrix
    {
        public float M11;

        public float M12;

        public float M13;

        public float M14;

        public float M21;

        public float M22;

        public float M23;

        public float M24;

        public float M31;

        public float M32;

        public float M33;

        public float M34;

        public float M41;

        public float M42;

        public float M43;

        public float M44;


        /// <summary>
        /// すべての要素をデフォルト値 (0) で初期化します。
        /// </summary>
        public Matrix()
        {
            M11 = default(float);
            M12 = default(float);
            M13 = default(float);
            M14 = default(float);
            M21 = default(float);
            M22 = default(float);
            M23 = default(float);
            M24 = default(float);
            M31 = default(float);
            M32 = default(float);
            M33 = default(float);
            M34 = default(float);
            M41 = default(float);
            M42 = default(float);
            M43 = default(float);
            M44 = default(float);
        }
        /// <summary>
        /// 単位行列を作成する
        /// </summary>
        public void Identity()
        {
        }

        /// <summary>
        /// 行列を平行移動する
        /// </summary>
        /// <param name="x">X 方向の移動量</param>
        /// <param name="y">Y 方向の移動量</param>
        /// <param name="z">Z 方向の移動量</param>
        /// <remarks>
        /// 与えられた引数から平行移動行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Translate( float x,  float y,  float z)
        {
        }

        /// <summary>
        /// 行列を平行移動する
        /// </summary>
        /// <param name="vec">移動量</param>
        /// <remarks>
        /// 与えられた引数から平行移動行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Translate( Vector3 vec)
        {
        }

        /// <summary>
        /// X 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="radian">回転角度 ( 軸方向に対して反時計回り )</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateX( float radian)
        {
        }

        /// <summary>
        /// Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateY( float radian)
        {
        }

        /// <summary>
        /// Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateZ( float radian)
        {
        }

        /// <summary>
        /// 行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="xRad">X 軸を回転軸とした角度</param>
        /// <param name="yRad">Y 軸を回転軸とした角度</param>
        /// <param name="zRad">Z 軸を回転軸とした角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Rotate( float xRad,  float yRad,  float zRad,  RotationOrder rotOrder = RotationOrder.XYZ)
        {
        }

        /// <summary>
        /// 行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="vec">各軸の回転角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Rotate( Vector3 vec,  RotationOrder rotOrder = RotationOrder.XYZ)
        {
        }

        /// <summary>
        /// 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="axis">回転軸を示す Vector3</param>
        /// <param name="radian">回転角度</param>
        /// <remarks>
        /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
        /// 						与えられた軸が正規化されていない場合は正規化したものを使って計算します。
        /// </remarks>
        public void RotateAxis( Vector3 axis,  float radian)
        {
        }

        /// <summary>
        /// クォータニオンを使って行列を回転する
        /// </summary>
        /// <param name="qua">処理の基になる Quaternion</param>
        /// <remarks>
        /// クォータニオンから回転行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void RotateQuaternion( Quaternion qua)
        {
        }

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="xyz">X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Scale( float xyz)
        {
        }

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="x">X 方向の拡大率</param>
        /// <param name="y">Y 方向の拡大率</param>
        /// <param name="z">Z 方向の拡大率</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Scale( float x,  float y,  float z)
        {
        }

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="scale">各方向の拡大率</param>
        /// <remarks>
        /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
        /// </remarks>
        public void Scale( Vector3 scale)
        {
        }

        /// <summary>
        /// 行列を乗算する
        /// </summary>
        /// <param name="mat1">処理の基になる行列</param>
        /// <param name="mat2">処理の基になる行列</param>
        public static Matrix Multiply( Matrix mat1,  Matrix mat2)
        {
        }

        /// <summary>
        /// 逆行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Inverse( Matrix mat)
        {
        }

        /// <summary>
        /// 転置行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Transpose( Matrix mat)
        {
        }

        /// <summary>
        /// 左手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformLH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
        }

        /// <summary>
        /// 右手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformRH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
        }

        /// <summary>
        /// 左手座標系射影行列の作成
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix PerspectiveFovLH( float fovY,  float aspect,  float nearZ,  float farZ)
        {
        }

        /// <summary>
        /// 右手座標系射影行列の作成
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix PerspectiveFovRH( float fovY,  float aspect,  float nearZ,  float farZ)
        {
        }

        /// <summary>
        /// 左手座標系正射影行列の作成
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix OrthoLH( float width,  float height,  float nearZ,  float farZ)
        {
        }

        /// <summary>
        /// 右手座標系正射影行列の作成
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        public static Matrix OrthoRH( float width,  float height,  float nearZ,  float farZ)
        {
        }

        /// <summary>
        /// 右方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetRight()
        {
        }

        /// <summary>
        /// 上方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetUp()
        {
        }

        /// <summary>
        /// 正面方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetFront()
        {
        }

        /// <summary>
        /// 位置を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetPosition()
        {
        }

        /// <summary>
        /// 回転行列からオイラー角を計算する
        /// </summary>
        public Vector3 GetEulerAngles()
        {
        }

        /// <summary>
        /// 行列をスケーリング、回転、移動成分に分解する
        /// </summary>
        /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
        /// <param name="rot">各軸の回転情報を格納する Quaternion 変数</param>
        public Vector3 Decompose(out Vector3 scale, out Quaternion rot)
        {
        }


    };

    /// <summary>
    /// クォータニオンのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
        public float X;

        public float Y;

        public float Z;

        public float W;


        /// <summary>
        /// すべての要素をデフォルト値 (0) で初期化します。
        /// </summary>
        public Quaternion()
        {
            X = default(float);
            Y = default(float);
            Z = default(float);
            W = default(float);
        }
        /// <summary>
        /// クォータニオンに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        public void Set( float x,  float y,  float z,  float w)
        {
        }

        /// <summary>
        /// 単位クォータニオンを作成する
        /// </summary>
        public static Quaternion Identity()
        {
        }

        /// <summary>
        /// 任意の軸を回転軸にして回転するクォータニオンを作成する
        /// </summary>
        /// <param name="axis">回転軸を示す Vector3 変数</param>
        /// <param name="r">回転角度 (ラジアン)</param>
        /// <remarks>
        /// 与えられた軸が正規化されていない場合は正規化された軸を使用します。
        /// </remarks>
        public static Quaternion RotationAxis( Vector3 axis,  float r)
        {
        }

        /// <summary>
        /// 回転行列からクォータニオンを作成する
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Quaternion RotationMatrix( Matrix mat)
        {
        }

        /// <summary>
        /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
        /// </summary>
        /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
        /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
        /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
        public static Quaternion RotationYawPitchRoll( float yaw,  float pitch,  float roll)
        {
        }

        /// <summary>
        /// クォータニオンを正規化する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Normalize( Quaternion qua)
        {
        }

        /// <summary>
        /// クォータニオンの共役を計算する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Conjugate( Quaternion qua)
        {
        }

        /// <summary>
        /// 2 つのクォータニオンの積を計算する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        public static Quaternion Multiply( Quaternion qua1,  Quaternion qua2)
        {
        }

        /// <summary>
        /// 2 つのクォータニオンを球面線形補間する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        /// <param name="t">補間値</param>
        public static Quaternion Slerp( Quaternion qua1,  Quaternion qua2,  float t)
        {
        }


    };

    /// <summary>
    /// 乱数のクラスです。
    /// </summary>
    /// <remarks>
    /// 長周期乱数を高速に生成します。
    /// </remarks>

    {

        /// <summary>
        /// 現在の時間を乱数シードとして、乱数オブジェクトを作成する
        /// </summary>
        public void Random() : base(_LNInternal.InternalBlock)
        {
        }

        /// <summary>
        /// 乱数シードを指定し、乱数オブジェクトを作成する
        /// </summary>
        public void Random() : base(_LNInternal.InternalBlock)
        {
        }

        /// <summary>
        /// 乱数オブジェクトに乱数シードを設定する
        /// </summary>
        /// <param name="seed">シード値</param>
        /// <remarks>
        /// random が NULL の場合、グローバル乱数オブジェクトに設定します。
        /// </remarks>
        public void SetSeed( int seed)
        {
        }

        /// <summary>
        /// 整数値乱数の取得
        /// </summary>
        /// <remarks>
        /// random が NULL の場合、グローバル乱数オブジェクトから取得します。
        /// </remarks>
        public int GetInt()
        {
        }

        /// <summary>
        /// 範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
        /// </summary>
        /// <param name="minValue">生成される乱数の下限値</param>
        /// <param name="maxValue">生成される乱数の上限値</param>
        /// <remarks>
        /// random が NULL の場合、グローバル乱数オブジェクトから取得します。
        /// </remarks>
        public int GetIntRange( int minValue,  int maxValue)
        {
        }

        /// <summary>
        /// 範囲を指定して、整数値乱数を取得する (center ± width/2)
        /// </summary>
        /// <param name="center">生成される乱数の中央値</param>
        /// <param name="width">生成される乱数の振れ幅</param>
        /// <remarks>
        /// random が NULL の場合、グローバル乱数オブジェクトから取得します。
        /// </remarks>
        public int GetIntWidth( int center,  int width)
        {
        }

        /// <summary>
        /// 実数値乱数の取得
        /// </summary>
        public float GetFloat()
        {
        }

        /// <summary>
        /// 実数値乱数の取得 ( minValue <= x < maxValue )
        /// </summary>
        /// <param name="minValue">生成される乱数の下限値</param>
        /// <param name="maxValue">生成される乱数の上限値</param>
        public float GetFloatRange( float minValue,  float maxValue)
        {
        }

        /// <summary>
        /// 実数値乱数の取得 (center ± width/2)
        /// </summary>
        /// <param name="center">生成される乱数の中央値</param>
        /// <param name="width">生成される乱数の振れ幅</param>
        public float GetFloatWidth( float center,  float width)
        {
        }


    };

	

    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LNoteException : Exception
    {
        /// <summary>
        /// エラーコード
        /// </summary>
        public ErrorCode ErrorCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""errorCode"">エラーコード</param>
        public LNoteException(ErrorCode errorCode)
        {
            ErrorCode = errorCode;
        }
    }

    internal enum _LNInternal
    {
        InternalBlock,
    }   

    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class ReferenceObject : IDisposable
    {
        internal static bool SystemInitialized = false;
        internal IntPtr _handle;

        internal ReferenceObject() {}
        internal ReferenceObject(_LNInternal i) {}

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~ReferenceObject() { Dispose(); }

        internal virtual void SetHandle(IntPtr handle)
        {
            _handle = handle;
        }       

        /// <summary>
        /// Dispose
        /// </summary>
        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                if (SystemInitialized)
                    API.LNObject_Release(_handle);
                _handle = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }
    }

    internal class TypeInfo
    {
        public delegate ReferenceObject ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);

    }

    /// <summary>
    /// オブジェクトのコレクション
    /// </summary>
    public class ObjectList<T> : ReferenceObject
        where T : ReferenceObject
    {
        private List<ReferenceObject> _list;
        
        internal ObjectList(_LNInternal i) {}

        internal override void SetHandle(IntPtr handle)
        {
            _list = new List<ReferenceObject>();
            _handle = handle;
            int count = Count;
            for (int i = 0; i < count; i++)
            {
                IntPtr item;
                API.LNObjectList_GetAt(_handle, i, out item);
                var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                t.SetHandle(item);
                _list.Add(t);
            }
        }
        
        public int Count
        {
            get
            {
                int count;
                API.LNObjectList_GetCount(_handle, out count);
                return count;
            }
        }
        
        public T this[int index]
        {
            get
            {
                SyncItems();
                return (T)_list[index];
            }
            set
            {
                SyncItems();
                _list[index] = value;
                API.LNObjectList_SetAt(_handle, index, value.Handle);
            }
        }
        
        public void Add(T item)
        {
            SyncItems();
            _list.Add(item);
            API.LNObjectList_Add(_handle, item.Handle);
        }
        
        public void Clear()
        {
            _list.Clear();
            API.LNObjectList_Clear(_handle);
        }
        
        public void Insert(int index, T item)
        {
            SyncItems();
            _list.Insert(index, item);
            API.LNObjectList_Insert(_handle, index, item.Handle);
        }
        
        public void Remove(T item)
        {
            SyncItems();
            _list.Remove(item);
            API.LNObjectList_Remove(_handle, item.Handle);
        }
        
        public void RemoveAll(T item)
        {
            SyncItems();
            _list.RemoveAll((i) => i == item);
            API.LNObjectList_RemoveAll(_handle, item.Handle);
        }
        
        public void RemoveAt(int index)
        {
            SyncItems();
            _list.RemoveAt(index);
            API.LNObjectList_RemoveAt(_handle, index);
        }

        private void SyncItems() 
        {
            int count = Count;
            if (_list.Count < count)
            {
                // 足りない分を詰める
                for (int i = 0; i < count - _list.Count; ++i)
		        {
                    _list.Add(null);
                }

                // リスト内容を同期する
                for (int i = 0; i < count; ++i)
		        {
                    IntPtr item;
                    API.LNObjectList_GetAt(_handle, i, out item);
			        if (_list[i] == null || _list[i].Handle != item)
			        {
                        var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                        t.SetHandle(item);
                        _list.Add(t);
			        }
		        }
            }
        }
    }

}
