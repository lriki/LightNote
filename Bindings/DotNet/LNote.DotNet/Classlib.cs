using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    /// <summary>
    /// 矩形を表します。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Rect
    {
        /// <summary>
        /// 左上 X 座標
        /// </summary>
        public int X;

        /// <summary>
        /// 左上 Y 座標
        /// </summary>
        public int Y;

        /// <summary>
        /// 幅
        /// </summary>
        public int Width;

        /// <summary>
        /// 高さ
        /// </summary>
        public int Height;


        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Rect(int x, int y, int width, int height)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        /// <summary>
        /// 矩形の各要素を設定します。
        /// </summary>
        /// <param name="x">左上 X 座標</param>
        /// <param name="y">左上 Y 座標</param>
        /// <param name="width">幅</param>
        /// <param name="height">高さ</param>
        public void Set( int x,  int y,  int width,  int height)
        {
            API.LNRect_Set(ref this,  x,  y,  width,  height);
        }

        /// <summary>
        /// 指定した x y 座標の点が矩形に含まれているかを判別する
        /// </summary>
        /// <param name="x">X 座標</param>
        /// <param name="y">Y 座標</param>
        public bool Contains( int x,  int y)
        {
            var result = new bool();
            API.LNRect_Contains(ref this,  x,  y, out result);
            return result;
        }


    };

    /// <summary>
    /// 整数型の多次元配列クラスです。
    /// </summary>
    public partial class IntTable : ReferenceObject
    {

        internal IntTable(_LNInternal i) : base(i) {}

        /// <summary>
        /// 整数型の多次元配列を作成する
        /// </summary>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        public  IntTable( int xSize,  int ySize = 1,  int zSize = 1) : base(_LNInternal.InternalBlock)
        {
            IntPtr intTable;
            var result = API.LNIntTable_Create(out intTable,  xSize,  ySize,  zSize);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, intTable);
        }

        /// <summary>
        /// 整数型の多次元配列を作成する
        /// </summary>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        /// <param name="srcData">XXXX</param>
        public  IntTable( int xSize,  int ySize,  int zSize,  int[] srcData) : base(_LNInternal.InternalBlock)
        {
            IntPtr intTable;
            var result = API.LNIntTable_CreateFromSrcData(out intTable,  xSize,  ySize,  zSize,  srcData);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, intTable);
        }

        /// <summary>
        /// 整数型テーブルの指定したインデックスに値を設定する
        /// </summary>
        /// <param name="xSize">X 要素インデックス</param>
        /// <param name="ySize">Y 要素インデックス</param>
        /// <param name="zSize">Z 要素インデックス</param>
        /// <param name="value">設定する整数値</param>
        /// <remarks>
        /// 変更前のデータは保持されます。
        /// </remarks>
        public void SetValue( int xSize,  int ySize,  int zSize,  int value)
        {
            var result = API.LNIntTable_SetValue( _handle,  xSize,  ySize,  zSize,  value);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 整数型テーブルの指定したインデックスに値を設定する
        /// </summary>
        /// <param name="xSize">X 要素インデックス</param>
        /// <param name="ySize">Y 要素インデックス</param>
        /// <param name="zSize">Z 要素インデックス</param>
        /// <remarks>
        /// 変更前のデータは保持されます。
        /// </remarks>
        public int GetValue( int xSize,  int ySize,  int zSize)
        {
            var value = new int();
            var result = API.LNIntTable_GetValue( _handle,  xSize,  ySize,  zSize, out value);
            if (result != Result.OK) throw new LNoteException(result);
            return value;
        }

        /// <summary>
        /// 整数型テーブルの要素数を変更する
        /// </summary>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        /// <remarks>
        /// 変更前のデータは保持されます。
        /// </remarks>
        public void Resize( int xSize,  int ySize,  int zSize)
        {
            var result = API.LNIntTable_Resize( _handle,  xSize,  ySize,  zSize);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 整数型テーブルの X 要素数を取得する
        /// </summary>
        public int GetXSize()
        {
            var xSize = new int();
            var result = API.LNIntTable_GetXSize( _handle, out xSize);
            if (result != Result.OK) throw new LNoteException(result);
            return xSize;
        }

        /// <summary>
        /// 整数型テーブルの Y 要素数を取得する
        /// </summary>
        public int GetYSize()
        {
            var ySize = new int();
            var result = API.LNIntTable_GetYSize( _handle, out ySize);
            if (result != Result.OK) throw new LNoteException(result);
            return ySize;
        }

        /// <summary>
        /// 整数型テーブルの Z 要素数を取得する
        /// </summary>
        public int GetZSize()
        {
            var zSize = new int();
            var result = API.LNIntTable_GetZSize( _handle, out zSize);
            if (result != Result.OK) throw new LNoteException(result);
            return zSize;
        }


    };

    /// <summary>
    /// 2次元のベクトルのクラスです。
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
        public float X;

        public float Y;


        /// <summary>
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector2(float x, float y)
        {
            X = x;
            Y = y;
        }

        /// <summary>
        /// 2D ベクトルに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        public void Set( float x,  float y)
        {
            var result = API.LNVector2_Set(ref this,  x,  y);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 2D ベクトルの長さを計算する
        /// </summary>
        public float GetLength()
        {
            var outLength = new float();
            var result = API.LNVector2_GetLength(ref this, out outLength);
            if (result != Result.OK) throw new LNoteException(result);
            return outLength;
        }

        /// <summary>
        /// 2D ベクトルの長さの2乗を計算する
        /// </summary>
        public float GetSquareLength()
        {
            var outLength = new float();
            var result = API.LNVector2_GetSquareLength(ref this, out outLength);
            if (result != Result.OK) throw new LNoteException(result);
            return outLength;
        }

        /// <summary>
        /// 2D ベクトルを正規化する
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        public static Vector2 Normalize( Vector2 vec)
        {
            var outVec = new Vector2();
            var result = API.LNVector2_Normalize(ref vec, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
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
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        /// <summary>
        /// 3Dベクトルに値を設定する
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        public void Set( float x,  float y,  float z)
        {
            var result = API.LNVector3_Set(out this,  x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 3Dベクトルに値を設定する
        /// </summary>
        /// <param name="vec2">2D ベクトル</param>
        /// <param name="z">Z 値</param>
        public void Set( Vector2 vec2,  float z)
        {
            var result = API.LNVector3_SetVec2(out this, ref vec2,  z);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 3Dベクトルの長さを計算する
        /// </summary>
        public float GetLength()
        {
            var outLength = new float();
            var result = API.LNVector3_GetLength(ref this, out outLength);
            if (result != Result.OK) throw new LNoteException(result);
            return outLength;
        }

        /// <summary>
        /// 3D ベクトルの長さの2乗を計算する
        /// </summary>
        public float GetSquareLength()
        {
            var outLength = new float();
            var result = API.LNVector3_GetSquareLength(ref this, out outLength);
            if (result != Result.OK) throw new LNoteException(result);
            return outLength;
        }

        /// <summary>
        /// 3D ベクトルを正規化する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        public static Vector3 Normalize( Vector3 vec)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Normalize(ref vec, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 2 つの 3D ベクトルの内積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static float Dot( Vector3 vec1,  Vector3 vec2)
        {
            var dot = new float();
            var result = API.LNVector3_Dot(ref vec1, ref vec2, out dot);
            if (result != Result.OK) throw new LNoteException(result);
            return dot;
        }

        /// <summary>
        /// 2 つの 3D ベクトルの外積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        public static Vector3 Cross( Vector3 vec1,  Vector3 vec2)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Cross(ref vec1, ref vec2, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 指定された法線で反射させたベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Reflect( Vector3 vec,  Vector3 normal)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Reflect(ref vec, ref normal, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 指定された法線と方向から滑りベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        public static Vector3 Slide( Vector3 vec,  Vector3 normal)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Slide(ref vec, ref normal, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 2 つの 3D ベクトル間を線形補間する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        public static Vector3 Lerp( Vector3 vec1,  Vector3 vec2,  float t)
        {
            var outVec = new Vector3();
            var result = API.LNVector3_Lerp(ref vec1, ref vec2,  t, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
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
            var outVec = new Vector3();
            var result = API.LNVector3_CatmullRom(ref vec1, ref vec2, ref vec3, ref vec4,  t, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
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
            var outVec = new Vector4();
            var result = API.LNVector3_Transform(ref vec, ref mat, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
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
            var outVec = new Vector3();
            var result = API.LNVector3_TransformCoord(ref vec, ref mat, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
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
        /// 各要素を指定して初期化します。
        /// </summary>
        public Vector4(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
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
            var result = API.LNVector4_Set(out this,  x,  y,  z,  w);
            if (result != Result.OK) throw new LNoteException(result);
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
        /// 各要素を指定して初期化します。
        /// </summary>
        public Matrix(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24, float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44)
        {
            M11 = m11;
            M12 = m12;
            M13 = m13;
            M14 = m14;
            M21 = m21;
            M22 = m22;
            M23 = m23;
            M24 = m24;
            M31 = m31;
            M32 = m32;
            M33 = m33;
            M34 = m34;
            M41 = m41;
            M42 = m42;
            M43 = m43;
            M44 = m44;
        }

        /// <summary>
        /// 単位行列を作成する
        /// </summary>
        public void Identity()
        {
            var result = API.LNMatrix_Identity(out this);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_Translate(ref this,  x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_TranslateVec3(ref this, ref vec);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateX(ref this,  radian);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateY(ref this,  radian);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateZ(ref this,  radian);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_Rotate(ref this,  xRad,  yRad,  zRad,  rotOrder);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateVec3(ref this, ref vec,  rotOrder);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateAxis(ref this, ref axis,  radian);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_RotateQuaternion(ref this, ref qua);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_Scale(ref this,  xyz);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_ScaleXYZ(ref this,  x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
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
            var result = API.LNMatrix_ScaleVec3(ref this, ref scale);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 行列を乗算する
        /// </summary>
        /// <param name="mat1">処理の基になる行列</param>
        /// <param name="mat2">処理の基になる行列</param>
        public static Matrix Multiply( Matrix mat1,  Matrix mat2)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Multiply(ref mat1, ref mat2, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
        }

        /// <summary>
        /// 逆行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Inverse( Matrix mat)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Inverse(ref mat, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
        }

        /// <summary>
        /// 転置行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Matrix Transpose( Matrix mat)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_Transpose(ref mat, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
        }

        /// <summary>
        /// 左手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformLH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_ViewTransformLH(ref pos, ref lookAt, ref upDir, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
        }

        /// <summary>
        /// 右手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        public static Matrix ViewTransformRH( Vector3 pos,  Vector3 lookAt,  Vector3 upDir)
        {
            var matOut = new Matrix();
            var result = API.LNMatrix_ViewTransformRH(ref pos, ref lookAt, ref upDir, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
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
            var matOut = new Matrix();
            var result = API.LNMatrix_PerspectiveFovLH( fovY,  aspect,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
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
            var matOut = new Matrix();
            var result = API.LNMatrix_PerspectiveFovRH( fovY,  aspect,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
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
            var matOut = new Matrix();
            var result = API.LNMatrix_OrthoLH( width,  height,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
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
            var matOut = new Matrix();
            var result = API.LNMatrix_OrthoRH( width,  height,  nearZ,  farZ, out matOut);
            if (result != Result.OK) throw new LNoteException(result);
            return matOut;
        }

        /// <summary>
        /// 右方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetRight()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetRight(ref this, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 上方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetUp()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetUp(ref this, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 正面方向を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetFront()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetFront(ref this, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 位置を示す 3D ベクトルの取得
        /// </summary>
        public Vector3 GetPosition()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetPosition(ref this, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 回転行列からオイラー角を計算する
        /// </summary>
        public Vector3 GetEulerAngles()
        {
            var outVec = new Vector3();
            var result = API.LNMatrix_GetEulerAngles(ref this, out outVec);
            if (result != Result.OK) throw new LNoteException(result);
            return outVec;
        }

        /// <summary>
        /// 行列をスケーリング、回転、移動成分に分解する
        /// </summary>
        /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
        /// <param name="rot">各軸の回転情報を格納する Quaternion 変数</param>
        public Vector3 Decompose(out Vector3 scale, out Quaternion rot)
        {
            var trans = new Vector3();
            var result = API.LNMatrix_Decompose(ref this, out scale, out rot, out trans);
            if (result != Result.OK) throw new LNoteException(result);
            return trans;
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
        /// 各要素を指定して初期化します。
        /// </summary>
        public Quaternion(float x, float y, float z, float w)
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
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
            var result = API.LNQuaternion_Set(out this,  x,  y,  z,  w);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 単位クォータニオンを作成する
        /// </summary>
        public static Quaternion Identity()
        {
            var qua = new Quaternion();
            var result = API.LNQuaternion_Identity(out qua);
            if (result != Result.OK) throw new LNoteException(result);
            return qua;
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
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationAxis(ref axis,  r, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// 回転行列からクォータニオンを作成する
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        public static Quaternion RotationMatrix( Matrix mat)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationMatrix(ref mat, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
        /// </summary>
        /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
        /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
        /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
        public static Quaternion RotationYawPitchRoll( float yaw,  float pitch,  float roll)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_RotationYawPitchRoll( yaw,  pitch,  roll, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// クォータニオンを正規化する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Normalize( Quaternion qua)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Normalize(ref qua, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// クォータニオンの共役を計算する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        public static Quaternion Conjugate( Quaternion qua)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Conjugate(ref qua, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// 2 つのクォータニオンの積を計算する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        public static Quaternion Multiply( Quaternion qua1,  Quaternion qua2)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Multiply(ref qua1, ref qua2, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }

        /// <summary>
        /// 2 つのクォータニオンを球面線形補間する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        /// <param name="t">補間値</param>
        public static Quaternion Slerp( Quaternion qua1,  Quaternion qua2,  float t)
        {
            var outQua = new Quaternion();
            var result = API.LNQuaternion_Slerp(ref qua1, ref qua2,  t, out outQua);
            if (result != Result.OK) throw new LNoteException(result);
            return outQua;
        }


    };

    /// <summary>
    /// 乱数のクラスです。
    /// </summary>
    /// <remarks>
    /// 長周期乱数を高速に生成します。
    /// </remarks>
    public partial class Random : ReferenceObject
    {

        internal Random(_LNInternal i) : base(i) {}

        /// <summary>
        /// 現在の時間を乱数シードとして、乱数オブジェクトを作成する
        /// </summary>
        public  Random() : base(_LNInternal.InternalBlock)
        {
            IntPtr random;
            var result = API.LNRandom_Create(out random);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, random);
        }

        /// <summary>
        /// 乱数シードを指定し、乱数オブジェクトを作成する
        /// </summary>
        /// <param name="seed">シード値</param>
        public  Random( int seed) : base(_LNInternal.InternalBlock)
        {
            IntPtr random;
            var result = API.LNRandom_CreateSeed(out random,  seed);
            if (result != Result.OK) throw new LNoteException(result);
            InternalManager.RegisterWrapperObject(this, random);
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
            var result = API.LNRandom_SetSeed( _handle,  seed);
            if (result != Result.OK) throw new LNoteException(result);
        }

        /// <summary>
        /// 整数値乱数の取得
        /// </summary>
        /// <remarks>
        /// random が NULL の場合、グローバル乱数オブジェクトから取得します。
        /// </remarks>
        public int GetInt()
        {
            var outValue = new int();
            var result = API.LNRandom_GetInt( _handle, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
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
            var outValue = new int();
            var result = API.LNRandom_GetIntRange( _handle,  minValue,  maxValue, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
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
            var outValue = new int();
            var result = API.LNRandom_GetIntWidth( _handle,  center,  width, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
        }

        /// <summary>
        /// 実数値乱数の取得
        /// </summary>
        public float GetFloat()
        {
            var outValue = new float();
            var result = API.LNRandom_GetFloat( _handle, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
        }

        /// <summary>
        /// 実数値乱数の取得 ( minValue <= x < maxValue )
        /// </summary>
        /// <param name="minValue">生成される乱数の下限値</param>
        /// <param name="maxValue">生成される乱数の上限値</param>
        public float GetFloatRange( float minValue,  float maxValue)
        {
            var outValue = new float();
            var result = API.LNRandom_GetFloatRange( _handle,  minValue,  maxValue, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
        }

        /// <summary>
        /// 実数値乱数の取得 (center ± width/2)
        /// </summary>
        /// <param name="center">生成される乱数の中央値</param>
        /// <param name="width">生成される乱数の振れ幅</param>
        public float GetFloatWidth( float center,  float width)
        {
            var outValue = new float();
            var result = API.LNRandom_GetFloatWidth( _handle,  center,  width, out outValue);
            if (result != Result.OK) throw new LNoteException(result);
            return outValue;
        }


    };

	

    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LNoteException : Exception
    {
        /// <summary>
        /// 結果コード
        /// </summary>
        public Result ResultCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""resultCode"">結果コード</param>
        public LNoteException(Result resultCode)
        {
            ResultCode = resultCode;
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
                {
                    InternalManager.UnregisterWrapperObject(this);
                    API.LNObject_Release(_handle);
                }
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
            
            var _IntTable = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new IntTable(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_IntTable);
            LNIntTable_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Random = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Random(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Random);
            LNRandom_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNIntTable_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNRandom_SetTypeUserData(IntPtr data);


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

	internal class UserData
    {
        public WeakReference     RefObject;
    }

    internal static class InternalManager
    {
        private const int InitialListSize = 1024;
        
        //private static  List<ReferenceObject> _objectList;
        private static  Stack<int> _userDataListIndexStack;
        private static  List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)

		
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern void LNObject_SetUserData(IntPtr handle, IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern IntPtr LNObject_GetUserData(IntPtr handle);

        // コンストラクタ
        static InternalManager()
        {
            _userDataListIndexStack = new Stack<int>(InitialListSize);
            _userDataList = new List<UserData>(InitialListSize); 

            // 空き番号を詰める
            for (int i = 0; i < InitialListSize; i++)
                _userDataListIndexStack.Push(i);
        }

        // ハンドルからラップオブジェクトを返す
        public static T GetWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            int index = (int)LNObject_GetUserData(handle);
            if (index == 0) // 新しく登録する
                return CreateWrapperObject<T>(handle);
            else            // 登録済みのオブジェクトを返す
                return (T)_userDataList[index].RefObject.Target;
        }

        // 新しいハンドルを登録し、ラップオブジェクトを生成する
        public static T CreateWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
            RegisterWrapperObject(obj, handle);
            return (T)obj;
        }

        // handle を refObj にセットし、refObj を管理リストに追加する
        //  (ReferenceObject のコンストラクタからも呼ばれる)
        public static void RegisterWrapperObject(ReferenceObject refObj, IntPtr handle)
        {
            // 管理リストが一杯の時は、倍のサイズに拡張する
            if (_userDataListIndexStack.Count == 0)
            {
                int growCount = _userDataList.Count;
                _userDataList.Capacity = growCount + growCount;
                for (int i = 0; i < growCount; i++)
                {
                    _userDataList.Add(null);
                    _userDataListIndexStack.Push(growCount + i);
                }
            }

            // 空きインデックスに new する
            //      new T ではなくちゃんとファクトリから作る。 new T ではサブクラスを考慮できない。
            //      コレクションの取得で Sprite を作りたいのに SceneNode を作ってしまうかもしれない。
            int index = _userDataListIndexStack.Pop();
            refObj.SetHandle(handle);
            _userDataList[index].RefObject = new WeakReference(refObj);

            // 管理配列のインデックスをユーザーデータとして関連付ける
            LNObject_SetUserData(handle, (IntPtr)index);
        }

        // RefObject の Dispose から呼ばれ、管理リストのインデックスを解放する
        //      (ユーザーデータを取得するため、LNObject_Release より前に呼ぶこと)
        public static void UnregisterWrapperObject(ReferenceObject refObj)
        {
            int index = (int)LNObject_GetUserData(refObj.Handle);
            _userDataList[index] = null;
            _userDataListIndexStack.Push(index);
        }
    }

}
