using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    /// <summary>
    /// オブジェクトの横幅と縦幅の情報
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Size
    {
            /// <summary>
            /// 幅
            /// </summary>
            public int Width;

            /// <summary>
            /// 高さ
            /// </summary>
            public int Height;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Size(int width, int height)
            {
                Width = width;
                Height = height;
            }



    };

    /// <summary>
    /// 矩形を表す
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
            /// コンストラクタ
            /// </summary>
            public Rect(int x, int y, int width, int height)
            {
                X = x;
                Y = y;
                Width = width;
                Height = height;
            }


            /// <summary>
            /// 指定した x y 座標の点が矩形に含まれているかを判別する
            /// </summary>
            /// <param name="x">X 座標</param>
            /// <param name="y">Y 座標</param>
            /// <returns>結果</returns>
            public bool Contains(int x, int y)
            {
                var result = new bool();
                API.LNRect_Contains(ref  this, x, y, out result);
                return result;
            }


    };

    /// <summary>
    /// 整数型の多次元配列クラス
    /// </summary>
    public partial class IntTable : ReferenceObject
    {

            /// <summary>
            /// 整数型テーブルの X 要素数を取得する
            /// </summary>
            public int XSize
            {
                get
                {
                    var xSize = new int();
                    var errorCode = API.LNIntTable_GetXSize(_handle, out xSize);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return xSize;
                }
            }

            /// <summary>
            /// 整数型テーブルの Y 要素数を取得する
            /// </summary>
            public int YSize
            {
                get
                {
                    var ySize = new int();
                    var errorCode = API.LNIntTable_GetYSize(_handle, out ySize);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return ySize;
                }
            }

            /// <summary>
            /// 整数型テーブルの Z 要素数を取得する
            /// </summary>
            public int ZSize
            {
                get
                {
                    var zSize = new int();
                    var errorCode = API.LNIntTable_GetZSize(_handle, out zSize);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return zSize;
                }
            }


            /// <summary>
            /// 整数型の多次元配列を作成する
            /// </summary>
            /// <param name="xSize">X 要素数</param>
            /// <param name="ySize">Y 要素数</param>
            /// <param name="zSize">Z 要素数</param>
            public  IntTable(int xSize, int ySize = 1, int zSize = 1) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNIntTable_Create(out _handle, xSize, ySize, zSize);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 整数型の多次元配列を作成する
            /// </summary>
            /// <param name="xSize">X 要素数</param>
            /// <param name="ySize">Y 要素数</param>
            /// <param name="zSize">Z 要素数</param>
            /// <param name="srcData">XXXX</param>
            public  IntTable(int xSize, int ySize, int zSize, int[] srcData) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNIntTable_CreateFromSrcData(out _handle, xSize, ySize, zSize, srcData);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

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
            public void SetValue(int xSize, int ySize, int zSize, int value)
            {
                var errorCode = API.LNIntTable_SetValue(_handle, xSize, ySize, zSize, value);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 整数型テーブルの指定したインデックスに値を設定する
            /// </summary>
            /// <param name="xSize">X 要素インデックス</param>
            /// <param name="ySize">Y 要素インデックス</param>
            /// <param name="zSize">Z 要素インデックス</param>
            /// <returns>値</returns>
            /// <remarks>
            /// 変更前のデータは保持されます。
            /// </remarks>
            public int GetValue(int xSize, int ySize, int zSize)
            {
                var value = new int();
                var errorCode = API.LNIntTable_GetValue(_handle, xSize, ySize, zSize, out value);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
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
            public void Resize(int xSize, int ySize, int zSize)
            {
                var errorCode = API.LNIntTable_Resize(_handle, xSize, ySize, zSize);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal IntTable(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 2D ベクトル
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector2
    {
            public float X;

            public float Y;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Vector2(float x, float y)
            {
                X = x;
                Y = y;
            }

            /// <summary>
            /// 2D ベクトルの長さを計算する
            /// </summary>
            public float Length
            {
                get
                {
                    var outLength = new float();
                    var errorCode = API.LNVector2_GetLength(ref  this, out outLength);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outLength;
                }
            }

            /// <summary>
            /// 2D ベクトルの長さの2乗を計算する
            /// </summary>
            public float SquareLength
            {
                get
                {
                    var outLength = new float();
                    var errorCode = API.LNVector2_GetSquareLength(ref  this, out outLength);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outLength;
                }
            }


            /// <summary>
            /// 2D ベクトルに値を設定する
            /// </summary>
            /// <param name="x">X 値</param>
            /// <param name="y">Y 値</param>
            public void Set(float x, float y)
            {
                var errorCode = API.LNVector2_Set(ref  this, x, y);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 2D ベクトルを正規化する
            /// </summary>
            /// <param name="vec">処理の基になる 2D ベクトル</param>
            /// <returns>演算結果</returns>
            public static Vector2 Normalize(Vector2 vec)
            {
                var outVec = new Vector2();
                var errorCode = API.LNVector2_Normalize(ref vec, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 2D ベクトルを正規化する
            /// </summary>
            /// <returns>演算結果</returns>
            public Vector2 Normalize()
            {
                var outVec = new Vector2();
                var errorCode = API.LNVector2_Normalize(ref  this, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }


    };

    /// <summary>
    /// 3Dベクトル
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector3
    {
            public float X;

            public float Y;

            public float Z;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Vector3(float x, float y, float z)
            {
                X = x;
                Y = y;
                Z = z;
            }

            /// <summary>
            /// 3Dベクトルの長さを計算する
            /// </summary>
            public float Length
            {
                get
                {
                    var outLength = new float();
                    var errorCode = API.LNVector3_GetLength(ref  this, out outLength);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outLength;
                }
            }

            /// <summary>
            /// 3D ベクトルの長さの2乗を計算する
            /// </summary>
            public float SquareLength
            {
                get
                {
                    var outLength = new float();
                    var errorCode = API.LNVector3_GetSquareLength(ref  this, out outLength);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outLength;
                }
            }


            /// <summary>
            /// 3Dベクトルに値を設定する
            /// </summary>
            /// <param name="x">X 値</param>
            /// <param name="y">Y 値</param>
            /// <param name="z">Z 値</param>
            public void Set(float x, float y, float z)
            {
                var errorCode = API.LNVector3_Set(out  this, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 3Dベクトルに値を設定する
            /// </summary>
            /// <param name="vec2">2D ベクトル</param>
            /// <param name="z">Z 値</param>
            public void Set(Vector2 vec2, float z)
            {
                var errorCode = API.LNVector3_SetVec2(out  this, ref vec2, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 3D ベクトルを正規化する
            /// </summary>
            /// <param name="vec">処理の基になる 3D ベクトル</param>
            /// <returns>演算結果</returns>
            public static Vector3 Normalize(Vector3 vec)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Normalize(ref vec, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 3D ベクトルを正規化する
            /// </summary>
            /// <returns>演算結果</returns>
            public Vector3 Normalize()
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Normalize(ref  this, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 2 つの 3D ベクトルの内積を計算する
            /// </summary>
            /// <param name="vec1">処理の基になる 3D ベクトル</param>
            /// <param name="vec2">処理の基になる 3D ベクトル</param>
            /// <returns>ベクトルの内積</returns>
            public static float Dot(Vector3 vec1, Vector3 vec2)
            {
                var dot = new float();
                var errorCode = API.LNVector3_Dot(ref vec1, ref vec2, out dot);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return dot;
            }

            /// <summary>
            /// 2 つの 3D ベクトルの外積を計算する
            /// </summary>
            /// <param name="vec1">処理の基になる 3D ベクトル</param>
            /// <param name="vec2">処理の基になる 3D ベクトル</param>
            /// <returns>演算結果</returns>
            public static Vector3 Cross(Vector3 vec1, Vector3 vec2)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Cross(ref vec1, ref vec2, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 指定された法線で反射させたベクトルを計算する
            /// </summary>
            /// <param name="vec">処理の基になる 3D ベクトル</param>
            /// <param name="normal">面方向を表す法線 3D ベクトル</param>
            /// <returns>演算結果</returns>
            public static Vector3 Reflect(Vector3 vec, Vector3 normal)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Reflect(ref vec, ref normal, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 指定された法線と方向から滑りベクトルを計算する
            /// </summary>
            /// <param name="vec">処理の基になる 3D ベクトル</param>
            /// <param name="normal">面方向を表す法線 3D ベクトル</param>
            /// <returns>演算結果</returns>
            public static Vector3 Slide(Vector3 vec, Vector3 normal)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Slide(ref vec, ref normal, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 2 つの 3D ベクトル間を線形補間する
            /// </summary>
            /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
            /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
            /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
            /// <returns>演算結果</returns>
            public static Vector3 Lerp(Vector3 vec1, Vector3 vec2, float t)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Lerp(ref vec1, ref vec2, t, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
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
            /// <returns>演算結果</returns>
            public static Vector3 CatmullRom(Vector3 vec1, Vector3 vec2, Vector3 vec3, Vector3 vec4, float t)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_CatmullRom(ref vec1, ref vec2, ref vec3, ref vec4, t, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 指定された行列を使用して座標変換する
            /// </summary>
            /// <param name="vec">処理の基になる 3D ベクトル</param>
            /// <param name="mat">処理の基になる 行列</param>
            /// <returns>演算結果</returns>
            /// <remarks>
            /// ベクトルを ( x, y, z, 1 ) として座標変換します。
            /// 結果の w は出力されません。
            /// 結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
            /// Vector3TransformCoord() を使用してください。
            /// </remarks>
            public static Vector3 Transform(Vector3 vec, Matrix mat)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_Transform(ref vec, ref mat, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 指定された行列を使用して座標変換し、結果を w = 1 に射影する
            /// </summary>
            /// <param name="vec">処理の基になる 3D ベクトル</param>
            /// <param name="mat">処理の基になる 行列</param>
            /// <returns>演算結果</returns>
            /// <remarks>
            /// ベクトルを ( x, y, z, 1 ) として座標変換し、
            /// 結果を w = 1 に射影 ( x y z を w で除算する ) します。
            /// </remarks>
            public static Vector3 TransformCoord(Vector3 vec, Matrix mat)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_TransformCoord(ref vec, ref mat, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 任意軸周りの回転を行う
            /// </summary>
            /// <param name="vec">処理の基になる Vector3</param>
            /// <param name="axis">回転軸を表す Vector3</param>
            /// <param name="r">回転角度 (ラジアン)</param>
            /// <returns>演算結果</returns>
            public static Vector3 RotateAxis(Vector3 vec, Vector3 axis, float r)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_RotateAxis(ref vec, ref axis, r, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 行列の回転成分だけを使って座標変換する
            /// </summary>
            /// <param name="vec">処理の基になる Vector3</param>
            /// <param name="mat">処理の基になる Matrix</param>
            /// <returns>演算結果</returns>
            public static Vector3 RotateMatrix(Vector3 vec, Matrix mat)
            {
                var outVec = new Vector3();
                var errorCode = API.LNVector3_RotateMatrix(ref vec, ref mat, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }


    };

    /// <summary>
    /// 4Dベクトル
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Vector4
    {
            public float X;

            public float Y;

            public float Z;

            public float W;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Vector4(float x, float y, float z, float w)
            {
                X = x;
                Y = y;
                Z = z;
                W = w;
            }



    };

    /// <summary>
    /// 4x4行列
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
            /// コンストラクタ
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
            public static Matrix Identity
            {
                get
                {
                    var mat = new Matrix();
                    var errorCode = API.LNMatrix_Identity(out mat);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return mat;
                }
            }

            /// <summary>
            /// 右方向を示す 3D ベクトル
            /// </summary>
            public Vector3 Right
            {
                get
                {
                    var outVec = new Vector3();
                    var errorCode = API.LNMatrix_GetRight(ref  this, out outVec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outVec;
                }
            }

            /// <summary>
            /// 上方向を示す 3D ベクトル
            /// </summary>
            public Vector3 Up
            {
                get
                {
                    var outVec = new Vector3();
                    var errorCode = API.LNMatrix_GetUp(ref  this, out outVec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outVec;
                }
            }

            /// <summary>
            /// 正面方向を示す 3D ベクトル
            /// </summary>
            public Vector3 Front
            {
                get
                {
                    var outVec = new Vector3();
                    var errorCode = API.LNMatrix_GetFront(ref  this, out outVec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outVec;
                }
            }

            /// <summary>
            /// 位置を示す 3D ベクトル
            /// </summary>
            public Vector3 Position
            {
                get
                {
                    var outVec = new Vector3();
                    var errorCode = API.LNMatrix_GetPosition(ref  this, out outVec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outVec;
                }
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
            public void Translate(float x, float y, float z)
            {
                var errorCode = API.LNMatrix_Translate(ref  this, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 行列を平行移動する
            /// </summary>
            /// <param name="vec">移動量</param>
            /// <remarks>
            /// 与えられた引数から平行移動行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void Translate(Vector3 vec)
            {
                var errorCode = API.LNMatrix_TranslateVec3(ref  this, ref vec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// X 軸を回転軸にして行列を回転する ( ラジアン単位 )
            /// </summary>
            /// <param name="radian">回転角度 ( 軸方向に対して反時計回り )</param>
            /// <remarks>
            /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void RotateX(float radian)
            {
                var errorCode = API.LNMatrix_RotateX(ref  this, radian);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
            /// </summary>
            /// <param name="radian">回転角度</param>
            /// <remarks>
            /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void RotateY(float radian)
            {
                var errorCode = API.LNMatrix_RotateY(ref  this, radian);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
            /// </summary>
            /// <param name="radian">回転角度</param>
            /// <remarks>
            /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void RotateZ(float radian)
            {
                var errorCode = API.LNMatrix_RotateZ(ref  this, radian);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

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
            public void Rotate(float xRad, float yRad, float zRad, RotationOrder rotOrder = RotationOrder.XYZ)
            {
                var errorCode = API.LNMatrix_Rotate(ref  this, xRad, yRad, zRad, rotOrder);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 行列を回転する ( ラジアン単位 )
            /// </summary>
            /// <param name="vec">各軸の回転角度</param>
            /// <param name="rotOrder">回転順序の指定</param>
            /// <remarks>
            /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void Rotate(Vector3 vec, RotationOrder rotOrder = RotationOrder.XYZ)
            {
                var errorCode = API.LNMatrix_RotateVec3(ref  this, ref vec, rotOrder);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
            /// </summary>
            /// <param name="axis">回転軸を示す Vector3</param>
            /// <param name="radian">回転角度</param>
            /// <remarks>
            /// 与えられた引数から回転行列を作り、現在の行列に乗算します。
            /// 与えられた軸が正規化されていない場合は正規化したものを使って計算します。
            /// </remarks>
            public void RotateAxis(Vector3 axis, float radian)
            {
                var errorCode = API.LNMatrix_RotateAxis(ref  this, ref axis, radian);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// クォータニオンを使って行列を回転する
            /// </summary>
            /// <param name="qua">処理の基になる Quaternion</param>
            /// <remarks>
            /// クォータニオンから回転行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void RotateQuaternion(Quaternion qua)
            {
                var errorCode = API.LNMatrix_RotateQuaternion(ref  this, ref qua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 行列をスケーリングする
            /// </summary>
            /// <param name="xyz">X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)</param>
            /// <remarks>
            /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void Scale(float xyz)
            {
                var errorCode = API.LNMatrix_Scale(ref  this, xyz);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

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
            public void Scale(float x, float y, float z)
            {
                var errorCode = API.LNMatrix_ScaleXYZ(ref  this, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 行列をスケーリングする
            /// </summary>
            /// <param name="scale">各方向の拡大率</param>
            /// <remarks>
            /// 与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
            /// </remarks>
            public void Scale(Vector3 scale)
            {
                var errorCode = API.LNMatrix_ScaleVec3(ref  this, ref scale);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 行列を乗算する
            /// </summary>
            /// <param name="mat1">処理の基になる行列</param>
            /// <param name="mat2">処理の基になる行列</param>
            /// <returns>結果</returns>
            public static Matrix Multiply(Matrix mat1, Matrix mat2)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_Multiply(ref mat1, ref mat2, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 逆行列を求める
            /// </summary>
            /// <param name="mat">処理の基になる行列</param>
            /// <returns>演算結果</returns>
            public static Matrix Inverse(Matrix mat)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_Inverse(ref mat, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 逆行列を求める
            /// </summary>
            /// <returns>演算結果</returns>
            public Matrix Inverse()
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_Inverse(ref  this, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 転置行列を求める
            /// </summary>
            /// <param name="mat">処理の基になる行列</param>
            /// <returns>演算結果</returns>
            public static Matrix Transpose(Matrix mat)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_Transpose(ref mat, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 転置行列を求める
            /// </summary>
            /// <returns>演算結果</returns>
            public Matrix Transpose()
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_Transpose(ref  this, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 左手座標系ビュー行列を作成する
            /// </summary>
            /// <param name="pos">視点の位置を示す Vector3</param>
            /// <param name="lookAt">注視点を示す Vector3</param>
            /// <param name="upDir">上方向を示す Vector3</param>
            /// <returns>演算結果</returns>
            public static Matrix ViewTransformLH(Vector3 pos, Vector3 lookAt, Vector3 upDir)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_ViewTransformLH(ref pos, ref lookAt, ref upDir, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 右手座標系ビュー行列を作成する
            /// </summary>
            /// <param name="pos">視点の位置を示す Vector3</param>
            /// <param name="lookAt">注視点を示す Vector3</param>
            /// <param name="upDir">上方向を示す Vector3</param>
            /// <returns>演算結果</returns>
            public static Matrix ViewTransformRH(Vector3 pos, Vector3 lookAt, Vector3 upDir)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_ViewTransformRH(ref pos, ref lookAt, ref upDir, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 左手座標系射影行列の作成
            /// </summary>
            /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
            /// <param name="aspect">アスペクト比</param>
            /// <param name="nearZ">近くのビュー平面の Z 値</param>
            /// <param name="farZ">遠くのビュー平面の Z 値</param>
            /// <returns>演算結果</returns>
            public static Matrix PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_PerspectiveFovLH(fovY, aspect, nearZ, farZ, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 右手座標系射影行列の作成
            /// </summary>
            /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
            /// <param name="aspect">アスペクト比</param>
            /// <param name="nearZ">近くのビュー平面の Z 値</param>
            /// <param name="farZ">遠くのビュー平面の Z 値</param>
            /// <returns>演算結果</returns>
            public static Matrix PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_PerspectiveFovRH(fovY, aspect, nearZ, farZ, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 左手座標系正射影行列の作成
            /// </summary>
            /// <param name="width">ビューの幅</param>
            /// <param name="height">ビューの高さ</param>
            /// <param name="nearZ">近くのビュー平面の Z 値</param>
            /// <param name="farZ">遠くのビュー平面の Z 値</param>
            /// <returns>演算結果</returns>
            public static Matrix OrthoLH(float width, float height, float nearZ, float farZ)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_OrthoLH(width, height, nearZ, farZ, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 右手座標系正射影行列の作成
            /// </summary>
            /// <param name="width">ビューの幅</param>
            /// <param name="height">ビューの高さ</param>
            /// <param name="nearZ">近くのビュー平面の Z 値</param>
            /// <param name="farZ">遠くのビュー平面の Z 値</param>
            /// <returns>演算結果</returns>
            public static Matrix OrthoRH(float width, float height, float nearZ, float farZ)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_OrthoRH(width, height, nearZ, farZ, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 回転行列からオイラー角を計算する
            /// </summary>
            /// <param name="mat">処理の基になる行列</param>
            /// <returns>各軸の回転角度(ラジアン)</returns>
            public static Vector3 ToEuler(Matrix mat)
            {
                var outVec = new Vector3();
                var errorCode = API.LNMatrix_ToEuler(ref mat, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 回転行列からオイラー角を計算する
            /// </summary>
            /// <returns>各軸の回転角度(ラジアン)</returns>
            public Vector3 ToEuler()
            {
                var outVec = new Vector3();
                var errorCode = API.LNMatrix_ToEuler(ref  this, out outVec);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outVec;
            }

            /// <summary>
            /// 行列をスケーリング、回転、移動成分に分解する
            /// </summary>
            /// <param name="mat">処理の基になる行列</param>
            /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
            /// <param name="rot">各軸の回転角度(ラジアン)を格納する Vector3 変数</param>
            /// <param name="trans">各軸の平行移動量を格納する Vector3 変数</param>
            public static void Decompose(Matrix mat, out Vector3 scale, out Matrix rot, out Vector3 trans)
            {
                var errorCode = API.LNMatrix_Decompose(ref mat, out scale, out rot, out trans);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 右手系⇔左手系の変換
            /// </summary>
            /// <param name="mat">処理の元になる行列</param>
            /// <returns>演算結果</returns>
            public static Matrix TransformBasis(Matrix mat)
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_TransformBasis(ref mat, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }

            /// <summary>
            /// 右手系⇔左手系の変換
            /// </summary>
            /// <returns>演算結果</returns>
            public Matrix TransformBasis()
            {
                var matOut = new Matrix();
                var errorCode = API.LNMatrix_TransformBasis(ref  this, out matOut);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return matOut;
            }


    };

    /// <summary>
    /// クォータニオン
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Quaternion
    {
            public float X;

            public float Y;

            public float Z;

            public float W;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Quaternion(float x, float y, float z, float w)
            {
                X = x;
                Y = y;
                Z = z;
                W = w;
            }

            /// <summary>
            /// 単位クォータニオンを作成する
            /// </summary>
            public static Quaternion Identity
            {
                get
                {
                    var qua = new Quaternion();
                    var errorCode = API.LNQuaternion_Identity(out qua);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return qua;
                }
            }


            /// <summary>
            /// クォータニオンに値を設定する
            /// </summary>
            /// <param name="x">X 値</param>
            /// <param name="y">Y 値</param>
            /// <param name="z">Z 値</param>
            /// <param name="w">W 値</param>
            public void Set(float x, float y, float z, float w)
            {
                var errorCode = API.LNQuaternion_Set(out  this, x, y, z, w);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 任意の軸を回転軸にして回転するクォータニオンを作成する
            /// </summary>
            /// <param name="axis">回転軸を示す Vector3 変数</param>
            /// <param name="r">回転角度 (ラジアン)</param>
            /// <returns>演算結果</returns>
            /// <remarks>
            /// 与えられた軸が正規化されていない場合は正規化された軸を使用します。
            /// </remarks>
            public static Quaternion RotationAxis(Vector3 axis, float r)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_RotationAxis(ref axis, r, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// 回転行列からクォータニオンを作成する
            /// </summary>
            /// <param name="mat">処理の基になる行列</param>
            /// <returns>演算結果</returns>
            public static Quaternion RotationMatrix(Matrix mat)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_RotationMatrix(ref mat, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
            /// </summary>
            /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
            /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
            /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
            /// <returns>演算結果</returns>
            public static Quaternion RotationYawPitchRoll(float yaw, float pitch, float roll)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_RotationYawPitchRoll(yaw, pitch, roll, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// クォータニオンを正規化する
            /// </summary>
            /// <param name="qua">処理の基になるクォータニオン</param>
            /// <returns>演算結果</returns>
            public static Quaternion Normalize(Quaternion qua)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Normalize(ref qua, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// クォータニオンを正規化する
            /// </summary>
            /// <returns>演算結果</returns>
            public Quaternion Normalize()
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Normalize(ref  this, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// クォータニオンの共役を計算する
            /// </summary>
            /// <param name="qua">処理の基になるクォータニオン</param>
            /// <returns>演算結果</returns>
            public static Quaternion Conjugate(Quaternion qua)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Conjugate(ref qua, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// クォータニオンの共役を計算する
            /// </summary>
            /// <returns>演算結果</returns>
            public Quaternion Conjugate()
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Conjugate(ref  this, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// 2 つのクォータニオンの積を計算する
            /// </summary>
            /// <param name="qua1">処理の基になるクォータニオン</param>
            /// <param name="qua2">処理の基になるクォータニオン</param>
            /// <returns>演算結果</returns>
            public static Quaternion Multiply(Quaternion qua1, Quaternion qua2)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Multiply(ref qua1, ref qua2, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }

            /// <summary>
            /// 2 つのクォータニオンを球面線形補間する
            /// </summary>
            /// <param name="qua1">処理の基になるクォータニオン</param>
            /// <param name="qua2">処理の基になるクォータニオン</param>
            /// <param name="t">補間値</param>
            /// <returns>演算結果</returns>
            public static Quaternion Slerp(Quaternion qua1, Quaternion qua2, float t)
            {
                var outQua = new Quaternion();
                var errorCode = API.LNQuaternion_Slerp(ref qua1, ref qua2, t, out outQua);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outQua;
            }


    };

    /// <summary>
    /// 長周期乱数を高速で生成する乱数クラス
    /// </summary>
    public partial class Random : ReferenceObject
    {

            /// <summary>
            /// 乱数オブジェクトに乱数シードを設定する
            /// </summary>
            /// <remarks>
            /// random が null の場合、グローバル乱数オブジェクトに設定します。
            /// </remarks>
            public int Seed
            {
                set
                {
                    var errorCode = API.LNRandom_SetSeed(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// 現在の時間を乱数シードとして、乱数オブジェクトを作成する
            /// </summary>
            public  Random() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNRandom_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 乱数シードを指定し、乱数オブジェクトを作成する
            /// </summary>
            /// <param name="seed">シード値</param>
            public  Random(int seed) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNRandom_CreateSeed(out _handle, seed);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 整数値乱数の取得
            /// </summary>
            /// <returns>整数値</returns>
            /// <remarks>
            /// random が null の場合、グローバル乱数オブジェクトから取得します。
            /// </remarks>
            public int GetInt()
            {
                var outValue = new int();
                var errorCode = API.LNRandom_GetInt(_handle, out outValue);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outValue;
            }

            /// <summary>
            /// 範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
            /// </summary>
            /// <param name="minValue">生成される乱数の下限値</param>
            /// <param name="maxValue">生成される乱数の上限値</param>
            /// <returns>整数値</returns>
            /// <remarks>
            /// random が null の場合、グローバル乱数オブジェクトから取得します。
            /// </remarks>
            public int GetIntRange(int minValue, int maxValue)
            {
                var outValue = new int();
                var errorCode = API.LNRandom_GetIntRange(_handle, minValue, maxValue, out outValue);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outValue;
            }

            /// <summary>
            /// 範囲を指定して、整数値乱数を取得する (center ± width/2)
            /// </summary>
            /// <param name="center">生成される乱数の中央値</param>
            /// <param name="width">生成される乱数の振れ幅</param>
            /// <returns>整数値</returns>
            /// <remarks>
            /// random が null の場合、グローバル乱数オブジェクトから取得します。
            /// </remarks>
            public int GetIntDeflect(int center, int width)
            {
                var outValue = new int();
                var errorCode = API.LNRandom_GetIntDeflect(_handle, center, width, out outValue);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outValue;
            }

            internal Random(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// ディレクトリ上のファイルまたは暗号化アーカイブ内のファイルにアクセスするためのモジュールです
    /// </summary>
    public partial class FileStream : ReferenceObject
    {

            /// <summary>
            /// ファイルサイズを取得する
            /// </summary>
            public int Size
            {
                get
                {
                    var size = new int();
                    var errorCode = API.LNFileStream_GetSize(_handle, out size);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return size;
                }
            }

            /// <summary>
            /// ファイルポインタの位置を取得する
            /// </summary>
            public int Position
            {
                get
                {
                    var position = new int();
                    var errorCode = API.LNFileStream_GetPosition(_handle, out position);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return position;
                }
            }


            /// <summary>
            /// ファイルを開く
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="mode">ファイルを開く方法または作成する方法</param>
            /// <param name="access">ファイルにアクセスする方法</param>
            /// <remarks>
            /// ファイルの読み込みが終了したら FileStream.Close または ReferenceObject.Release でファイルを閉じる必要があります。
            /// </remarks>
            public  FileStream(string filePath, FileMode mode = FileMode.Open, FileAccess access = FileAccess.Read) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNFileStream_Create(out _handle, filePath, mode, access);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ファイルを閉じる
            /// </summary>
            public void Close()
            {
                var errorCode = API.LNFileStream_Close(_handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ファイルポインタの位置を設定する
            /// </summary>
            /// <param name="offset">読み込んだデータを格納するバッファ</param>
            /// <param name="origin">起点</param>
            public void Seek(int offset, SeekOrigin origin)
            {
                var errorCode = API.LNFileStream_Seek(_handle, offset, origin);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ファイルからデータを読み込む
            /// </summary>
            /// <param name="buffer">読み込んだデータを格納するバッファ</param>
            /// <param name="readSize">読み込むサイズ (バイト数)</param>
            /// <returns>読み込んだサイズ (バイト数) </returns>
            public int Read(ref byte[] buffer, int readSize)
            {
                var validSize = new int();
                var errorCode = API.LNFileStream_Read(_handle, ref buffer, readSize, out validSize);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return validSize;
            }

            /// <summary>
            /// ファイルの存在を調べる
            /// </summary>
            /// <param name="filePath">ファイルパス</param>
            /// <returns>結果</returns>
            /// <remarks>
            /// ディレクトリと全ての暗号化アーカイブを対象として確認します。
            /// </remarks>
            public static bool Exists(string filePath)
            {
                var result = new bool();
                var errorCode = API.LNFileStream_Exists(filePath, out result);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return result;
            }

            internal FileStream(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 入力処理
    /// </summary>
    public partial class Input
    {


            /// <summary>
            /// ボタンが現在押されているかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static bool IsPress(int button)
            {
                var state = new bool();
                var errorCode = API.LNInput_IsPress(button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが新しく押されたかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static bool IsTrigger(int button)
            {
                var state = new bool();
                var errorCode = API.LNInput_IsTrigger(button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが離されたかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static bool IsOffTrigger(int button)
            {
                var state = new bool();
                var errorCode = API.LNInput_IsOffTrigger(button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static bool IsRepeat(int button)
            {
                var state = new bool();
                var errorCode = API.LNInput_IsRepeat(button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>アナログ値</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static float GetPower(int button)
            {
                var state = new float();
                var errorCode = API.LNInput_GetPower(button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
            /// </summary>
            /// <param name="axis">軸番号 (0～1)</param>
            /// <returns>軸の状態</returns>
            /// <remarks>
            /// 0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
            /// </remarks>
            public static Vector2 GetAxisState(int axis)
            {
                var state = new Vector2();
                var errorCode = API.LNInput_GetAxisState(axis, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }


    };

    /// <summary>
    /// 仮想ゲームコントローラ
    /// </summary>
    public partial class VirtualPad : ReferenceObject
    {
            private static VirtualPad[] _pads = new VirtualPad[0];

            /// <summary>
            /// 仮想ゲームコントローラの新しく押されたデバイスボタン番号
            /// </summary>
            /// <remarks>
            /// キーボード、マウス、ジョイパッド(スティックを含む)の
            /// 何らかのボタンが押された場合、そのボタンを識別するための
            /// 番号を返します。(デバイスボタン)
            /// この番号は主にキーコンフィグで使用します。
            /// この関数は isTrigger() 同様に動作し、ボタンが押された瞬間だけ
            /// ボタン番号を返します。
            /// それ以外は 0 を返します。
            /// </remarks>
            public int PressedDeviceButton
            {
                get
                {
                    var deviceButton = new int();
                    var errorCode = API.LNVirtualPad_GetPressedDeviceButton(_handle, out deviceButton);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return deviceButton;
                }
            }


            /// <summary>
            /// 仮想ゲームコントローラハンドルを取得する
            /// </summary>
            /// <param name="index">取得する仮想ゲームコントローラの番号 (0～3)</param>
            /// <returns>仮想ゲームコントローラハンドル</returns>
            public static VirtualPad GetVirtualPad(int index)
            {
                if (_pads.Length < index + 1)
                                    {
                                        IntPtr handle;
                                        API.LNVirtualPad_GetVirtualPad(index, out handle);
                                        Array.Resize(ref _pads, index + 1);
                                        _pads[index] = new VirtualPad(_LNInternal.InternalBlock);
                                        _pads[index].SetHandle(handle);
                                    }
                                    return _pads[index];
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが現在押されているかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            public bool IsPress(int button)
            {
                var state = new bool();
                var errorCode = API.LNVirtualPad_IsPress(_handle, button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが新しく押されたかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            public bool IsTrigger(int button)
            {
                var state = new bool();
                var errorCode = API.LNVirtualPad_IsTrigger(_handle, button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが離されたかを判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            public bool IsOffTrigger(int button)
            {
                var state = new bool();
                var errorCode = API.LNVirtualPad_IsOffTrigger(_handle, button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>状態</returns>
            public bool IsRepeat(int button)
            {
                var state = new bool();
                var errorCode = API.LNVirtualPad_IsRepeat(_handle, button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
            /// </summary>
            /// <param name="button">仮想ボタン番号 (0～19)</param>
            /// <returns>アナログ値</returns>
            public float GetPower(int button)
            {
                var state = new float();
                var errorCode = API.LNVirtualPad_GetPower(_handle, button, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
            /// </summary>
            /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
            /// <param name="axis">軸番号 (0～1)</param>
            /// <returns>軸の状態</returns>
            public static Vector2 GetAxisState(VirtualPad virtualPad, int axis)
            {
                var state = new Vector2();
                var errorCode = API.LNVirtualPad_GetAxisState((virtualPad != null) ? virtualPad.Handle : default(IntPtr), axis, out state);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return state;
            }

            /// <summary>
            /// デバイスボタン番号に対応するデバイスの種類を取得する
            /// </summary>
            /// <param name="deviceButton">デバイスボタン番号</param>
            /// <returns>デバイスボタンの種類</returns>
            public static DeviceButtonType GetDeviceButtonType(int deviceButton)
            {
                var type = new DeviceButtonType();
                var errorCode = API.LNVirtualPad_GetDeviceButtonType(deviceButton, out type);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return type;
            }

            /// <summary>
            /// 仮想ゲームコントローラのボタンにデバイスボタンを割り当てる
            /// </summary>
            /// <param name="virtualButton">割り当て先の仮想ボタン番号</param>
            /// <param name="deviceButton">割り当てるデバイスボタン番号</param>
            /// <remarks>
            /// この関数はキーコンフィグ用の関数です。
            /// デバイスボタン(キーボードのキーや、ジョイパッドのボタン)を仮想ボタンに割り当てます。
            /// 
            /// 基本的に VirtualPad.GetPressedDeviceButton() とセットで使い、VirtualPad.GetDeviceButtonType()から
            /// 受け取ったデバイスボタン番号をこの関数の deviceButton に渡します。
            /// </remarks>
            public void AttachVirtualButton(int virtualButton, int deviceButton)
            {
                var errorCode = API.LNVirtualPad_AttachVirtualButton(_handle, virtualButton, deviceButton);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 仮想ゲームコントローラのデバイスボタンの割り当てを解除する
            /// </summary>
            /// <param name="virtualButton">解除対象の仮想ボタン番号</param>
            /// <param name="deviceButton">解除するデバイスボタン番号</param>
            /// <remarks>
            /// 解除対象の仮想ボタンに deviceButton で示されるデバイスボタンが割り当てられている場合、解除します。
            /// 割り当てられていない場合は何もしません。
            /// deviceButton に 0 を渡すと、virtualButton の全ての割り当てを解除します。
            /// </remarks>
            public void DetachVirtualButton(int virtualButton, int deviceButton)
            {
                var errorCode = API.LNVirtualPad_DetachVirtualButton(_handle, virtualButton, deviceButton);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal VirtualPad(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 音声管理及びゲームオーディオ機能
    /// </summary>
    public partial class Audio
    {


            /// <summary>
            /// BGM を演奏する
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
            public static void PlayBGM(string filePath, int volume = 100, int pitch = 100, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_PlayBGM(filePath, volume, pitch, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから BGM を演奏する
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
            public static void PlayBGMMem(byte[] data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_PlayBGMMem(data, dataSize, volume, pitch, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// BGM の演奏を停止する
            /// </summary>
            /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
            public static void StopBGM(int fadeTime = 0)
            {
                var errorCode = API.LNAudio_StopBGM(fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// BGS を演奏する
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
            public static void PlayBGS(string filePath, int volume = 100, int pitch = 100, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_PlayBGS(filePath, volume, pitch, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから BGS を演奏する
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
            public static void PlayBGSMem(byte[] data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_PlayBGSMem(data, dataSize, volume, pitch, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// BGS の演奏を停止する
            /// </summary>
            /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
            public static void StopBGS(int fadeTime = 0)
            {
                var errorCode = API.LNAudio_StopBGS(fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ME を演奏する
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlayME(string filePath, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlayME(filePath, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから ME を演奏する
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlayMEMem(byte[] data, int dataSize, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlayMEMem(data, dataSize, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ME の演奏を停止する
            /// </summary>
            public static void StopME()
            {
                API.LNAudio_StopME();

            }

            /// <summary>
            /// SE を演奏する
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySE(string filePath, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySE(filePath, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// SE を演奏する (3D サウンド)
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="position">3D 空間上の座標</param>
            /// <param name="distance">減衰距離</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySE3D(string filePath, Vector3 position, float distance, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySE3D(filePath, ref position, distance, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// SE を演奏する (3D サウンド)
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="x">3D 空間上の X 座標</param>
            /// <param name="y">3D 空間上の Y 座標</param>
            /// <param name="z">3D 空間上の Z 座標</param>
            /// <param name="distance">減衰距離</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySE3D(string filePath, float x, float y, float z, float distance, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySE3DXYZ(filePath, x, y, z, distance, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから SE を演奏する
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySEMem(byte[] data, int dataSize, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySEMem(data, dataSize, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから SE を演奏する (3D サウンド)
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="position">3D 空間上の座標</param>
            /// <param name="distance">減衰距離</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySE3DMem(byte[] data, int dataSize, Vector3 position, float distance, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySE3DMem(data, dataSize, ref position, distance, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声データから SE を演奏する (3D サウンド)
            /// </summary>
            /// <param name="data">メモリ上の音声ファイルデータ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="x">3D 空間上の X 座標</param>
            /// <param name="y">3D 空間上の Y 座標</param>
            /// <param name="z">3D 空間上の Z 座標</param>
            /// <param name="distance">減衰距離</param>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="pitch">ピッチ (50 ～ 200)</param>
            public static void PlaySE3DMem(byte[] data, int dataSize, float x, float y, float z, float distance, int volume = 100, int pitch = 100)
            {
                var errorCode = API.LNAudio_PlaySE3DMemXYZ(data, dataSize, x, y, z, distance, volume, pitch);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// すべての SE の演奏を停止する
            /// </summary>
            public static void StopSE()
            {
                API.LNAudio_StopSE();

            }

            /// <summary>
            /// 3D 空間の1メートル相当の距離の設定
            /// </summary>
            /// <param name="distance">距離</param>
            public static void SetMetreUnitDistance(float distance)
            {
                var errorCode = API.LNAudio_SetMetreUnitDistance(distance);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 再生中のBGMの音量を設定する (フェードアウト中は無効)
            /// </summary>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
            public static void SetBGMVolume(int volume, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_SetBGMVolume(volume, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 再生中のBGSの音量を設定する (フェードアウト中は無効)
            /// </summary>
            /// <param name="volume">ボリューム (0 ～ 100)</param>
            /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
            public static void SetBGSVolume(int volume, int fadeTime = 0)
            {
                var errorCode = API.LNAudio_SetBGSVolume(volume, fadeTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
            /// </summary>
            /// <param name="type">取得するサウンドの種類</param>
            /// <remarks>
            /// この関数は、BGM、BGS、ME の音量、ピッチの変更、及び
            /// 再生位置の取得のために用意されています。
            /// この関数から取得したサウンドハンドルに対して
            /// Release、Play、Stop、Pause を行わないでください。
            /// </remarks>
            public static void GetInternalGameSound(InternalGameSound type)
            {
                API.LNAudio_GetInternalGameSound(type);

            }


    };

    /// <summary>
    /// 3D音声リスナー
    /// </summary>
    public partial class SoundListener
    {


            /// <summary>
            /// リスナーの位置の設定
            /// </summary>
            /// <param name="position">3D 空間上の座標</param>
            public static void SetPosition(Vector3 position)
            {
                var errorCode = API.LNSoundListener_SetPosition(ref position);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// リスナーの位置の設定
            /// </summary>
            /// <param name="x">3D 空間上の X 座標</param>
            /// <param name="y">3D 空間上の Y 座標</param>
            /// <param name="z">3D 空間上の Z 座標</param>
            public static void SetPosition(float x, float y, float z)
            {
                API.LNSoundListener_SetPositionXYZ(x, y, z);

            }

            /// <summary>
            /// リスナーの正面方向の設定
            /// </summary>
            /// <param name="direction">向き</param>
            public static void SetDirection(Vector3 direction)
            {
                var errorCode = API.LNSoundListener_SetDirection(ref direction);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// リスナーの正面方向の設定
            /// </summary>
            /// <param name="x">向きの X 成分</param>
            /// <param name="y">向きの Y 成分</param>
            /// <param name="z">向きの Z 成分</param>
            public static void SetDirection(float x, float y, float z)
            {
                API.LNSoundListener_SetDirectionXYZ(x, y, z);

            }

            /// <summary>
            /// リスナーの上方向の設定 (正面方向とは直交であること)
            /// </summary>
            /// <param name="direction">上方向</param>
            public static void SetUpDirection(Vector3 direction)
            {
                var errorCode = API.LNSoundListener_SetUpDirection(ref direction);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// リスナーの上方向の設定 (正面方向とは直交であること)
            /// </summary>
            /// <param name="x">向きの X 成分</param>
            /// <param name="y">向きの Y 成分</param>
            /// <param name="z">向きの Z 成分</param>
            public static void SetUpDirection(float x, float y, float z)
            {
                API.LNSoundListener_SetUpDirectionXYZ(x, y, z);

            }

            /// <summary>
            /// リスナーの速度の設定
            /// </summary>
            /// <param name="velocity">速度</param>
            public static void Velocity(Vector3 velocity)
            {
                var errorCode = API.LNSoundListener_Velocity(ref velocity);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// リスナーの速度の設定
            /// </summary>
            /// <param name="x">速度の X 成分</param>
            /// <param name="y">速度の Y 成分</param>
            /// <param name="z">速度の Z 成分</param>
            public static void Velocity(float x, float y, float z)
            {
                API.LNSoundListener_VelocityXYZ(x, y, z);

            }


    };

    /// <summary>
    /// 音声機能
    /// </summary>
    public partial class Sound : ReferenceObject
    {

            /// <summary>
            /// ボリューム
            /// </summary>
            public int Volume
            {
                get
                {
                    var volume = new int();
                    var errorCode = API.LNSound_GetVolume(_handle, out volume);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return volume;
                }
                set
                {
                    var errorCode = API.LNSound_SetVolume(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// ピッチ
            /// </summary>
            public int Pitch
            {
                get
                {
                    var pitch = new int();
                    var errorCode = API.LNSound_GetPitch(_handle, out pitch);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return pitch;
                }
                set
                {
                    var errorCode = API.LNSound_SetPitch(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// ループ再生が有効かを調べる
            /// </summary>
            public bool IsLoop
            {
                get
                {
                    var enabled = new bool();
                    var errorCode = API.LNSound_IsLoop(_handle, out enabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return enabled;
                }
            }

            /// <summary>
            /// サウンドの再生状態を取得する
            /// </summary>
            public SoundPlayingState PlayState
            {
                get
                {
                    var state = new SoundPlayingState();
                    var errorCode = API.LNSound_GetPlayState(_handle, out state);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return state;
                }
            }

            /// <summary>
            /// 再生したサンプル数
            /// </summary>
            public int UnitsPlayed
            {
                get
                {
                    var samples = new int();
                    var errorCode = API.LNSound_GetUnitsPlayed(_handle, out samples);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return samples;
                }
            }

            /// <summary>
            /// 音声データ全体のサンプル数
            /// </summary>
            public int TotalUnits
            {
                get
                {
                    var samples = new int();
                    var errorCode = API.LNSound_GetTotalUnits(_handle, out samples);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return samples;
                }
            }

            /// <summary>
            /// サンプリング周波数
            /// </summary>
            public int UnitsParSecond
            {
                get
                {
                    var frequency = new int();
                    var errorCode = API.LNSound_GetUnitsParSecond(_handle, out frequency);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return frequency;
                }
            }

            /// <summary>
            /// 3D 音源かを調べる
            /// </summary>
            public bool Is3DSound
            {
                get
                {
                    var enabled = new bool();
                    var errorCode = API.LNSound_Is3DSound(_handle, out enabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return enabled;
                }
            }

            /// <summary>
            /// 3D 音源としての位置を設定する
            /// </summary>
            public Vector3 EmitterPosition
            {
                set
                {
                    var errorCode = API.LNSound_SetEmitterPosition(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 3D 音源としての速度を設定する
            /// </summary>
            public Vector3 EmitterVelocity
            {
                set
                {
                    var errorCode = API.LNSound_SetEmitterVelocity(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 3D 音源の減衰距離 (聴こえなくなる距離) を設定する
            /// </summary>
            public float EmitterDistance
            {
                set
                {
                    var errorCode = API.LNSound_SetEmitterDistance(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// ファイルから音声を作成する
            /// </summary>
            /// <param name="filePath">ファイル名</param>
            /// <param name="enable3d">true の場合、3D 音源として作成する</param>
            /// <param name="playType">音声の再生方法</param>
            public  Sound(string filePath, bool enable3d = false, SoundPlayingType playType = SoundPlayingType.Auto) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNSound_Create(out _handle, filePath, enable3d, playType);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// メモリ上の音声ファイルデータから音声を作成する
            /// </summary>
            /// <param name="data">メモリ上の音声データへのポインタ</param>
            /// <param name="dataSize">データサイズ (バイト単位)</param>
            /// <param name="enable3d">true の場合、3D 音源として作成する</param>
            public  Sound(byte[] data, int dataSize, bool enable3d = false) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNSound_CreateMem(out _handle, data, dataSize, enable3d);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ループ再生の有効設定
            /// </summary>
            /// <param name="loopEnable">true = ループ再生する / false = しない</param>
            /// <param name="begin">ループ領域の先頭位置 (サンプル数単位)</param>
            /// <param name="length">ループ領域長さ (サンプル数単位)</param>
            /// <remarks>
            /// begin と length に 0 を指定すると、全体をループ領域として設定します。
            /// </remarks>
            public void SetLoopState(bool loopEnable, int begin = 0, int length = 0)
            {
                var errorCode = API.LNSound_SetLoopState(_handle, loopEnable, begin, length);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 音声を再生する
            /// </summary>
            public void Play()
            {
                var errorCode = API.LNSound_Play(_handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 再生を停止する
            /// </summary>
            public void Stop()
            {
                var errorCode = API.LNSound_Stop(_handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 一時停止
            /// </summary>
            /// <param name="pause">true = 一時停止 / false = 一時停止解除</param>
            public void Pause(bool pause)
            {
                var errorCode = API.LNSound_Pause(_handle, pause);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 音量のフェード
            /// </summary>
            /// <param name="targetVolume">変更先の音量</param>
            /// <param name="time">フェードにかける時間 (ミリ秒)</param>
            /// <param name="fadeState">フェード完了後の動作の指定</param>
            public void FadeVolume(int targetVolume, int time, SoundFadeState fadeState)
            {
                var errorCode = API.LNSound_FadeVolume(_handle, targetVolume, time, fadeState);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 3D 音源としての位置を設定する
            /// </summary>
            /// <param name="x">3D 空間上の X 座標</param>
            /// <param name="y">3D 空間上の Y 座標</param>
            /// <param name="z">3D 空間上の Z 座標</param>
            public void SetEmitterPosition(float x, float y, float z)
            {
                var errorCode = API.LNSound_SetEmitterPositionXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 3D 音源としての速度を設定する
            /// </summary>
            /// <param name="x">速度の X 成分</param>
            /// <param name="y">速度の Y 成分</param>
            /// <param name="z">速度の Z 成分</param>
            public void SetEmitterVelocity(float x, float y, float z)
            {
                var errorCode = API.LNSound_SetEmitterVelocityXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Sound(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 色
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Color
    {
            /// <summary>
            /// 赤成分 (0～255)
            /// </summary>
            public byte R;

            /// <summary>
            /// 緑成分 (0～255)
            /// </summary>
            public byte G;

            /// <summary>
            /// 青成分 (0～255)
            /// </summary>
            public byte B;

            /// <summary>
            /// アルファ成分 (0～255)
            /// </summary>
            public byte A;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Color(byte r, byte g, byte b, byte a)
            {
                R = r;
                G = g;
                B = b;
                A = a;
            }



    };

    /// <summary>
    /// //
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct ColorF
    {
            /// <summary>
            /// 赤成分 (0.0～1.0)
            /// </summary>
            public float R;

            /// <summary>
            /// 緑成分 (0.0～1.0)
            /// </summary>
            public float G;

            /// <summary>
            /// 青成分 (0.0～1.0)
            /// </summary>
            public float B;

            /// <summary>
            /// アルファ成分 (0.0～1.0)
            /// </summary>
            public float A;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public ColorF(float r, float g, float b, float a)
            {
                R = r;
                G = g;
                B = b;
                A = a;
            }



    };

    /// <summary>
    /// //
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public struct Tone
    {
            /// <summary>
            /// 赤成分 (-1.0～1.0)
            /// </summary>
            public float R;

            /// <summary>
            /// 緑成分 (-1.0～1.0)
            /// </summary>
            public float G;

            /// <summary>
            /// 青成分 (-1.0～1.0)
            /// </summary>
            public float B;

            /// <summary>
            /// モノクロ成分 (0.0～1.0)
            /// </summary>
            public float Mono;

            /// <summary>
            /// コンストラクタ
            /// </summary>
            public Tone(float r, float g, float b, float mono)
            {
                R = r;
                G = g;
                B = b;
                Mono = mono;
            }



    };

    /// <summary>
    /// テクスチャ
    /// </summary>
    public partial class Texture : ReferenceObject
    {

            /// <summary>
            /// テクスチャの幅 (ピクセル単位)
            /// </summary>
            public int Width
            {
                get
                {
                    var width = new int();
                    var errorCode = API.LNTexture_GetWidth(_handle, out width);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return width;
                }
            }

            /// <summary>
            /// テクスチャの高さ (ピクセル単位)
            /// </summary>
            public int Height
            {
                get
                {
                    var height = new int();
                    var errorCode = API.LNTexture_GetHeight(_handle, out height);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return height;
                }
            }

            /// <summary>
            /// テクスチャの実際の幅 (ピクセル単位)
            /// </summary>
            public int RealWidth
            {
                get
                {
                    var width = new int();
                    var errorCode = API.LNTexture_GetRealWidth(_handle, out width);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return width;
                }
            }

            /// <summary>
            /// テクスチャの実際の高さ (ピクセル単位)
            /// </summary>
            public int RealHeight
            {
                get
                {
                    var height = new int();
                    var errorCode = API.LNTexture_GetRealHeight(_handle, out height);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return height;
                }
            }


            /// <summary>
            /// テクスチャの作成
            /// </summary>
            /// <param name="width">幅 (ピクセル数)</param>
            /// <param name="height">高さ (ピクセル数)</param>
            /// <remarks>
            /// ミップマップレベル 1、フォーマット A8R8B8G8(透明色付フォーマット) で作成します。
            /// </remarks>
            public  Texture(int width, int height) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTexture_Create(out _handle, width, height);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャの作成
            /// </summary>
            /// <param name="filePath">テクスチャファイル名</param>
            /// <remarks>
            /// サポートしているフォーマットは 
            /// .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。
            /// ミップマップレベル 1 で作成します。
            /// </remarks>
            public  Texture(string filePath) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTexture_CreateFromFile(out _handle, filePath);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャの作成
            /// </summary>
            /// <param name="width">幅 (ピクセル数)</param>
            /// <param name="height">高さ (ピクセル数)</param>
            /// <param name="levels">ミップマップレベル (0で最大レベルまで作成)</param>
            /// <param name="format">サーフェイスフォーマット</param>
            public  Texture(int width, int height, int levels, TextureFormat format) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTexture_CreateEx(out _handle, width, height, levels, format);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャの作成
            /// </summary>
            /// <param name="filePath">テクスチャファイル名</param>
            /// <param name="colorKey">透明色として扱う色</param>
            /// <param name="levels">ミップマップレベル (0で最大レベルまで作成)</param>
            /// <param name="format">サーフェイスフォーマット</param>
            /// <remarks>
            /// サポートしているフォーマットは 
            /// .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。
            /// </remarks>
            public  Texture(string filePath, uint colorKey, int levels, TextureFormat format) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTexture_CreateFromFileEx(out _handle, filePath, colorKey, levels, format);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャをクリアする
            /// </summary>
            /// <param name="color">塗りつぶす色</param>
            public void Clear(Color color)
            {
                var errorCode = API.LNTexture_Clear(_handle, ref color);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャをクリアする
            /// </summary>
            /// <param name="r">塗りつぶす色の R 値</param>
            /// <param name="g">塗りつぶす色の G 値</param>
            /// <param name="b">塗りつぶす色の B 値</param>
            /// <param name="a">塗りつぶす色の A 値</param>
            public void ClearRGBA(int r, int g, int b, int a)
            {
                var errorCode = API.LNTexture_ClearRGBA(_handle, r, g, b, a);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャへ文字列を描画する
            /// </summary>
            /// <param name="text">描画する文字列</param>
            /// <param name="rect">描画領域 (はみだした場合にクリッピングされます)</param>
            /// <param name="font">描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)</param>
            /// <param name="length">描画文字数 (-1 の場合、文字列全体を描画する)</param>
            /// <param name="align">描画基準位置の指定</param>
            /// <remarks>
            /// レンダリングターゲットと深度バッファテクスチャには描画できません。
            /// </remarks>
            public void DrawText(string text, Rect rect, Font font = null, int length = -1, TextAlign align = TextAlign.Left)
            {
                var errorCode = API.LNTexture_DrawText(_handle, text, ref rect, (font != null) ? font.Handle : default(IntPtr), length, align);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャへ文字列を描画する
            /// </summary>
            /// <param name="text">描画する文字列</param>
            /// <param name="x">描画先の左上 X 座標</param>
            /// <param name="y">描画先の左上 Y 座標</param>
            /// <param name="font">描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)</param>
            /// <param name="length">描画文字数 (-1 の場合、文字列全体を描画する)</param>
            /// <param name="align">描画基準位置の指定</param>
            /// <remarks>
            /// レンダリングターゲットと深度バッファテクスチャには描画できません。
            /// </remarks>
            public void DrawText(string text, int x, int y, Font font = null, int length = -1, TextAlign align = TextAlign.Left)
            {
                var errorCode = API.LNTexture_DrawTextXY(_handle, text, x, y, (font != null) ? font.Handle : default(IntPtr), length, align);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャのブロック転送
            /// </summary>
            /// <param name="x">転送先 X 座標</param>
            /// <param name="y">転送先 Y 座標</param>
            /// <param name="srcTexture">転送元テクスチャハンドル</param>
            /// <param name="srcRect">転送元領域</param>
            /// <remarks>
            /// srcTexture で指定されたテクスチャから texture へブロック転送を行います。
            /// レンダリングターゲットと深度バッファテクスチャは
            /// 転送元・先のどちらにも指定できません。
            /// </remarks>
            public void BitBlt(int x, int y, Texture srcTexture, Rect srcRect)
            {
                var errorCode = API.LNTexture_BitBlt(_handle, x, y, (srcTexture != null) ? srcTexture.Handle : default(IntPtr), ref srcRect);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Texture(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// フォントオブジェクト
    /// </summary>
    public partial class Font : ReferenceObject
    {

            /// <summary>
            /// フォント名
            /// </summary>
            public string Name
            {
                get
                {
                    IntPtr strPtr;
                    var errorCode = API.LNFont_GetName(_handle, out strPtr);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    int len;
                    API.LCSInternal_GetIntPtrStringLength(strPtr, out len);
                    var sb = new StringBuilder(len + 1);
                    API.LCSInternal_GetIntPtrString(strPtr, sb);
                    return sb.ToString();
                }
                set
                {
                    var errorCode = API.LNFont_SetName(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// フォントサイズ
            /// </summary>
            public int Size
            {
                get
                {
                    var size = new int();
                    var errorCode = API.LNFont_GetSize(_handle, out size);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return size;
                }
                set
                {
                    var errorCode = API.LNFont_SetSize(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// フォントカラー
            /// </summary>
            public Color Color
            {
                get
                {
                    var color = new Color();
                    var errorCode = API.LNFont_GetColor(_handle, out color);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return color;
                }
                set
                {
                    var errorCode = API.LNFont_SetColor(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// エッジカラー
            /// </summary>
            public Color EdgeColor
            {
                get
                {
                    var color = new Color();
                    var errorCode = API.LNFont_GetEdgeColor(_handle, out color);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return color;
                }
                set
                {
                    var errorCode = API.LNFont_SetEdgeColor(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// エッジの幅
            /// </summary>
            public int EdgeSize
            {
                get
                {
                    var size = new int();
                    var errorCode = API.LNFont_GetEdgeSize(_handle, out size);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return size;
                }
                set
                {
                    var errorCode = API.LNFont_SetEdgeSize(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 太文字有無
            /// </summary>
            public bool IsBold
            {
                get
                {
                    var enabled = new bool();
                    var errorCode = API.LNFont_IsBold(_handle, out enabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return enabled;
                }
                set
                {
                    var errorCode = API.LNFont_SetBold(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// イタリック体有無
            /// </summary>
            public bool IsItalic
            {
                get
                {
                    var enabled = new bool();
                    var errorCode = API.LNFont_IsItalic(_handle, out enabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return enabled;
                }
                set
                {
                    var errorCode = API.LNFont_SetItalic(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// アンチエイリアス有無
            /// </summary>
            public bool IsAntiAlias
            {
                get
                {
                    var enabled = new bool();
                    var errorCode = API.LNFont_IsAntiAlias(_handle, out enabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return enabled;
                }
                set
                {
                    var errorCode = API.LNFont_SetAntiAlias(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// フォントの作成
            /// </summary>
            /// <remarks>
            /// デフォルトのフォントを使用してフォントを作成します。
            /// </remarks>
            public  Font() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNFont_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ビットマップフォントの作成
            /// </summary>
            /// <param name="filePath">画像ファイル名</param>
            public  Font(string filePath) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNFont_CreateBitmapFontFromFile(out _handle, filePath);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// フォントのコピーの作成
            /// </summary>
            /// <param name="srcFont">コピー元のフォントハンドル</param>
            public  Font(Font srcFont) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNFont_CreateCopy(out _handle, (srcFont != null) ? srcFont.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 指定フォントをデフォルトフォントに設定する
            /// </summary>
            /// <param name="font">フォントハンドル</param>
            /// <remarks>
            /// テクスチャの DrawText 等でフォントハンドルを null にした場合、
            /// このフォントが使われるようになります。
            /// </remarks>
            public static void SetDefaultFont(Font font)
            {
                var errorCode = API.LNFont_SetDefaultFont((font != null) ? font.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 文字列を描画したときのサイズ (ピクセル単位) の取得
            /// </summary>
            /// <param name="font">フォントハンドル</param>
            /// <param name="text">サイズ計算に使用する文字列</param>
            /// <param name="length">文字数 (-1 で文字列全体)</param>
            /// <returns>結果</returns>
            public static Size GetTextSize(Font font, string text, int length)
            {
                var outSize = new Size();
                var errorCode = API.LNFont_GetTextSize((font != null) ? font.Handle : default(IntPtr), text, length, out outSize);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return outSize;
            }

            internal Font(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// シーングラフ
    /// </summary>
    /// <remarks>
    /// シーングラフは2D、または3Dの描画空間内に配置されるオブジェクトを
    /// 階層構造にまとめたものです。
    /// </remarks>
    public partial class SceneGraph
    {



    };

    /// <summary>
    /// シーングラフノードのベースクラス
    /// </summary>
    public partial class SceneNode : ReferenceObject
    {

            /// <summary>
            /// ノード名
            /// </summary>
            public string Name
            {
                get
                {
                    IntPtr strPtr;
                    var errorCode = API.LNSceneNode_GetName(_handle, out strPtr);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    int len;
                    API.LCSInternal_GetIntPtrStringLength(strPtr, out len);
                    var sb = new StringBuilder(len + 1);
                    API.LCSInternal_GetIntPtrString(strPtr, sb);
                    return sb.ToString();
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetName(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 座標
            /// </summary>
            public virtual Vector3 Position
            {
                get
                {
                    var vec = new Vector3();
                    var errorCode = API.LNSceneNode_GetPosition(_handle, out vec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return vec;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetPosition(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 回転角度 (ラジアン単位)
            /// </summary>
            public virtual Vector3 Angle
            {
                get
                {
                    var vec = new Vector3();
                    var errorCode = API.LNSceneNode_GetAngle(_handle, out vec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return vec;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetAngle(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 拡大率 (ラジアン単位)
            /// </summary>
            public Vector3 Scale
            {
                get
                {
                    var vec = new Vector3();
                    var errorCode = API.LNSceneNode_GetScale(_handle, out vec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return vec;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetScale(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 移動・回転・拡大の原点
            /// </summary>
            public Vector3 Center
            {
                get
                {
                    var vec = new Vector3();
                    var errorCode = API.LNSceneNode_GetCenter(_handle, out vec);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return vec;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetCenter(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 座標変換行列
            /// </summary>
            public virtual Matrix Matrix
            {
                get
                {
                    var mat = new Matrix();
                    var errorCode = API.LNSceneNode_GetMatrix(_handle, out mat);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return mat;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetMatrix(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 回転順序
            /// </summary>
            public RotationOrder RotateOrder
            {
                get
                {
                    var outOrder = new RotationOrder();
                    var errorCode = API.LNSceneNode_GetRotateOrder(_handle, out outOrder);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outOrder;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetRotateOrder(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 配置方法
            /// </summary>
            public PlacementType PlacementType
            {
                get
                {
                    var outType = new PlacementType();
                    var errorCode = API.LNSceneNode_GetPlacementType(_handle, out outType);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outType;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetPlacementType(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 自動更新有無
            /// </summary>
            public bool IsEnableAutoUpdate
            {
                get
                {
                    var outEnabled = new bool();
                    var errorCode = API.LNSceneNode_IsEnableAutoUpdate(_handle, out outEnabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outEnabled;
                }
                set
                {
                    var errorCode = API.LNSceneNode_SetEnableAutoUpdate(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// 座標の設定
            /// </summary>
            /// <param name="x">X 座標</param>
            /// <param name="y">Y 座標</param>
            /// <param name="z">Z 座標</param>
            public virtual void SetPosition(float x, float y, float z)
            {
                var errorCode = API.LNSceneNode_SetPositionXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 座標の取得
            /// </summary>
            /// <param name="x">X 座標を格納する変数</param>
            /// <param name="y">Y 座標を格納する変数</param>
            /// <param name="z">Z 座標を格納する変数</param>
            public void GetPosition(out float x, out float y, out float z)
            {
                var errorCode = API.LNSceneNode_GetPositionXYZ(_handle, out x, out y, out z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 回転角度の設定 (ラジアン単位)
            /// </summary>
            /// <param name="x">X 軸の回転角度</param>
            /// <param name="y">Y 軸の回転角度</param>
            /// <param name="z">Z 軸の回転角度</param>
            public virtual void SetAngle(float x, float y, float z)
            {
                var errorCode = API.LNSceneNode_SetAngleXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 回転角度の取得 (ラジアン単位)
            /// </summary>
            /// <param name="x">X 軸の回転角度を格納する変数</param>
            /// <param name="y">Y 軸の回転角度を格納する変数</param>
            /// <param name="z">Z 軸の回転角度を格納する変数</param>
            public void GetAngle(out float x, out float y, out float z)
            {
                var errorCode = API.LNSceneNode_GetAngleXYZ(_handle, out x, out y, out z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 拡大率の設定 (ラジアン単位)
            /// </summary>
            /// <param name="x">X 方向の拡大率</param>
            /// <param name="y">Y 方向の拡大率</param>
            /// <param name="z">Z 方向の拡大率</param>
            public void SetScale(float x, float y, float z)
            {
                var errorCode = API.LNSceneNode_SetScaleXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 拡大率の取得 (ラジアン単位)
            /// </summary>
            /// <param name="x">X 方向の拡大率を格納する変数</param>
            /// <param name="y">Y 方向の拡大率を格納する変数</param>
            /// <param name="z">Z 方向の拡大率を格納する変数</param>
            public void GetScale(out float x, out float y, out float z)
            {
                var errorCode = API.LNSceneNode_GetScaleXYZ(_handle, out x, out y, out z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 移動・回転・拡大の原点の設定
            /// </summary>
            /// <param name="x">X 方向のオフセット</param>
            /// <param name="y">Y 方向のオフセット</param>
            /// <param name="z">Z 方向のオフセット</param>
            public void SetCenter(float x, float y, float z)
            {
                var errorCode = API.LNSceneNode_SetCenterXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 移動・回転・拡大の原点の取得
            /// </summary>
            /// <param name="x">X 方向のオフセットを格納する変数</param>
            /// <param name="y">Y 方向のオフセットを格納する変数</param>
            /// <param name="z">Z 方向のオフセットを格納する変数</param>
            public void GetCenter(out float x, out float y, out float z)
            {
                var errorCode = API.LNSceneNode_GetCenterXYZ(_handle, out x, out y, out z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 子ノードを追加する
            /// </summary>
            /// <param name="childSceneNode">子ノードとして追加するノード</param>
            public void AddChild(SceneNode childSceneNode)
            {
                var errorCode = API.LNSceneNode_AddChild(_handle, (childSceneNode != null) ? childSceneNode.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 子ノードを取り除く
            /// </summary>
            /// <param name="childSceneNode">取り外す子ノード</param>
            public void RemoveChild(SceneNode childSceneNode)
            {
                var errorCode = API.LNSceneNode_RemoveChild(_handle, (childSceneNode != null) ? childSceneNode.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// フレーム更新
            /// </summary>
            /// <param name="elapsedTime">経過させる時間</param>
            /// <remarks>
            /// エフェクトやアニメーションの状態を elapsedTime 分進めます。
            /// 通常、1フレームに1度呼び出します。
            /// 自動更新が有効になっている場合はライブラリの
            /// フレーム更新を行った時に自動的に呼ばれるため、
            /// この関数を直接呼び出す必要はありません。
            /// 自動更新はデフォルトで有効です。
            /// </remarks>
            public void Update(float elapsedTime)
            {
                var errorCode = API.LNSceneNode_Update(_handle, elapsedTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal SceneNode(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 描画機能を持つシーンノードのスーパークラス
    /// </summary>
    /// <remarks>
    /// VisualNode は SceneNode のサブクラスです。
    /// </remarks>
    public partial class VisualNode : SceneNode
    {

            /// <summary>
            /// 可視状態
            /// </summary>
            public bool Visible
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetVisible(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 不透明度
            /// </summary>
            public float Opacity
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetOpacity(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 合成方法
            /// </summary>
            public BlendMode BlendMode
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetBlendMode(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カリング方法
            /// </summary>
            public CullingMode CullingMode
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetCullingMode(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 深度テストの有効設定
            /// </summary>
            public bool EnableDepthTest
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetEnableDepthTest(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 深度書き込みの有効設定
            /// </summary>
            public bool EnableDepthWrite
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetEnableDepthWrite(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// シェーダ
            /// </summary>
            public Shader Shader
            {
                set
                {
                    var errorCode = API.LNVisualNode_SetShader(_handle, (value != null) ? value.Handle : default(IntPtr));
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// 乗算色の設定
            /// </summary>
            /// <param name="materialIndex">設定先のマテリアル番号</param>
            /// <param name="color">設定する色</param>
            public void SetColorScale(int materialIndex, ColorF color)
            {
                var errorCode = API.LNVisualNode_SetColorScale(_handle, materialIndex, ref color);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ブレンドカラーの設定
            /// </summary>
            /// <param name="materialIndex">設定先のマテリアル番号</param>
            /// <param name="color">設定する色</param>
            public void SetBlendColor(int materialIndex, ColorF color)
            {
                var errorCode = API.LNVisualNode_SetBlendColor(_handle, materialIndex, ref color);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 色調の設定
            /// </summary>
            /// <param name="materialIndex">設定先のマテリアル番号</param>
            /// <param name="tone">設定する色調</param>
            public void SetTone(int materialIndex, Tone tone)
            {
                var errorCode = API.LNVisualNode_SetTone(_handle, materialIndex, ref tone);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// UV 変換行列の設定
            /// </summary>
            /// <param name="materialIndex">設定先のマテリアル番号</param>
            /// <param name="matrix">設定する行列</param>
            public void SetUVTransform(int materialIndex, Matrix matrix)
            {
                var errorCode = API.LNVisualNode_SetUVTransform(_handle, materialIndex, ref matrix);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal VisualNode(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// スプライトノード
    /// </summary>
    /// <remarks>
    /// スプライトは VisualNode のサブクラスです。
    /// </remarks>
    public partial class Sprite : VisualNode
    {

            /// <summary>
            /// スプライトが表示するテクスチャ
            /// </summary>
            public Texture Texture
            {
                set
                {
                    var errorCode = API.LNSprite_SetTexture(_handle, (value != null) ? value.Handle : default(IntPtr));
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// テクスチャの転送矩形
            /// </summary>
            public Rect SrcRect
            {
                get
                {
                    var rect = new Rect();
                    var errorCode = API.LNSprite_GetSrcRect(_handle, out rect);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return rect;
                }
                set
                {
                    var errorCode = API.LNSprite_SetSrcRect(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 左右反転有無
            /// </summary>
            public bool IsFlipH
            {
                get
                {
                    var outEnabled = new bool();
                    var errorCode = API.LNSprite_IsFlipH(_handle, out outEnabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outEnabled;
                }
                set
                {
                    var errorCode = API.LNSprite_SetFlipH(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 上下反転有無
            /// </summary>
            public bool IsFlipV
            {
                get
                {
                    var outEnabled = new bool();
                    var errorCode = API.LNSprite_IsFlipV(_handle, out outEnabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outEnabled;
                }
                set
                {
                    var errorCode = API.LNSprite_SetFlipV(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// スプライトオブジェクトの作成 (2D空間へ配置する)
            /// </summary>
            /// <param name="texture">スプライトが表示するテクスチャのハンドル</param>
            public  Sprite(Texture texture) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNSprite_Create(out _handle, (texture != null) ? texture.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// スプライトのサイズの設定
            /// </summary>
            /// <param name="width">設定する幅</param>
            /// <param name="height">設定する高さ</param>
            public void SetSize(float width, float height)
            {
                var errorCode = API.LNSprite_SetSize(_handle, width, height);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャの転送矩形の設定
            /// </summary>
            /// <param name="x">転送元の左上の X 座標</param>
            /// <param name="y">転送元の左上の Y 座標</param>
            /// <param name="w">転送元の幅</param>
            /// <param name="h">転送元の高さ</param>
            public void SetSrcRect(int x, int y, int w, int h)
            {
                var errorCode = API.LNSprite_SetSrcRectXYWH(_handle, x, y, w, h);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// テクスチャの転送矩形の取得
            /// </summary>
            /// <param name="x">転送元の左上の X 座標 を格納する変数</param>
            /// <param name="y">転送元の左上の Y 座標 を格納する変数</param>
            /// <param name="w">転送元の幅 を格納する変数</param>
            /// <param name="h">転送元の高さ を格納する変数</param>
            public void GetSrcRect(out int x, out int y, out int w, out int h)
            {
                var errorCode = API.LNSprite_GetSrcRectXYWH(_handle, out x, out y, out w, out h);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Sprite(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// スプライトパスノード
    /// </summary>
    /// <remarks>
    /// SpritePath は VisualNode のサブクラスです。
    /// </remarks>
    public partial class SpritePath : VisualNode
    {

            /// <summary>
            /// 以降のスプライトの描画で使用するブレンドモード
            /// </summary>
            public BlendMode PathBlendMode
            {
                set
                {
                    var errorCode = API.LNSpritePath_SetPathBlendMode(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// 以降のスプライトの描画で使用する座標変換行列
            /// </summary>
            public Matrix PathTransform
            {
                set
                {
                    var errorCode = API.LNSpritePath_SetPathTransform(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// スプライトパスオブジェクトの作成
            /// </summary>
            /// <param name="maxSpriteCount">このパスが一度の画面更新で描画できるスプライトの数</param>
            /// <param name="coord">描画先の座標空間</param>
            public  SpritePath(int maxSpriteCount, Coord coord = Coord.Space2D) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNSpritePath_Create(out _handle, maxSpriteCount, coord);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// スプライトの描画
            /// </summary>
            /// <param name="position">描画先座標</param>
            /// <param name="center">位置や回転の中心となるオフセット</param>
            /// <param name="size">描画先スプライトのサイズ</param>
            /// <param name="texture">描画するテクスチャハンドル</param>
            /// <param name="srcRect">テクスチャからの転送矩形</param>
            /// <param name="vertexColors">四角形の各頂点の乗算色 (要素数 4 の配列。null の場合は考慮しない)</param>
            /// <param name="frontAxis">スプライトの正面方法</param>
            /// <remarks>
            /// center は 2D/3D で基準位置が異なります。
            /// 2D空間へ描画するときはスプライトの左上、
            /// 3D空間へ描画するときはスプライトの中央となります。
            /// front は、2Dの時は通常 FrontAxis.Rz を指定します。
            /// </remarks>
            public void DrawSprite(Vector3 position, Vector3 center, Vector2 size, Texture texture, Rect srcRect, ColorF vertexColors, FrontAxis frontAxis = FrontAxis.Rz)
            {
                var errorCode = API.LNSpritePath_DrawSprite(_handle, ref position, ref center, ref size, (texture != null) ? texture.Handle : default(IntPtr), ref srcRect, ref vertexColors, frontAxis);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal SpritePath(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// モデルノード
    /// </summary>
    public partial class Model : VisualNode
    {


            /// <summary>
            /// モデルオブジェクトの作成
            /// </summary>
            /// <param name="filePath">メッシュファイル名</param>
            /// <param name="meshCreateFlags"></param>
            public  Model(string filePath, MeshCreateFlags meshCreateFlags) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNModel_Create(out _handle, filePath, meshCreateFlags);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Model(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// エフェクトオブジェクト
    /// </summary>
    /// <remarks>
    /// エフェクトオブジェクトは VisualNode のサブクラスです。
    /// </remarks>
    public partial class Effect : VisualNode
    {

            /// <summary>
            /// エフェクトが再生中かを確認する
            /// </summary>
            public bool IsPlaying
            {
                get
                {
                    var playing = new bool();
                    var errorCode = API.LNEffect_IsPlaying(_handle, out playing);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return playing;
                }
            }


            /// <summary>
            /// エフェクトオブジェクトの作成 (3D空間へ配置する)
            /// </summary>
            /// <param name="filePath">エフェクトファイル名 (Effekseer 形式)</param>
            public  Effect(string filePath) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNEffect_Create(out _handle, filePath);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// エフェクトの再生を開始する
            /// </summary>
            /// <param name="overlap">true = 再生中のエフェクトを停止しない / false = 再生中のエフェクトを停止する</param>
            public void Play(bool overlap)
            {
                var errorCode = API.LNEffect_Play(_handle, overlap);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// エフェクトを停止する
            /// </summary>
            public void Stop()
            {
                var errorCode = API.LNEffect_Stop(_handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Effect(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 大量のスプライトを格子状に表示するためのクラスです。主に 2D ゲームのマップ表示に使用します。
    /// </summary>
    public partial class Tilemap : VisualNode
    {

            /// <summary>
            /// タイルマップのマップデータを設定する
            /// </summary>
            public IntTable MapData
            {
                set
                {
                    var errorCode = API.LNTilemap_SetMapData(out _handle, (value != null) ? value.Handle : default(IntPtr));
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// タイルマップのタイルセットとして使用するテクスチャを設定する
            /// </summary>
            public Texture TilesetTexture
            {
                set
                {
                    var errorCode = API.LNTilemap_SetTilesetTexture(out _handle, (value != null) ? value.Handle : default(IntPtr));
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// タイルセットテクスチャの横方向のタイル数を設定する
            /// </summary>
            public int TilesetHorizontalTileCount
            {
                set
                {
                    var errorCode = API.LNTilemap_SetTilesetHorizontalTileCount(out _handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// タイルマップのタイルひとつ分のピクセルサイズを設定する
            /// </summary>
            public int TilePixelSize
            {
                set
                {
                    var errorCode = API.LNTilemap_SetTilePixelSize(out _handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// タイルマップの作成
            /// </summary>
            public  Tilemap() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTilemap_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Tilemap(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// シェーダ
    /// </summary>
    public partial class Shader : ReferenceObject
    {


            /// <summary>
            /// シェーダの作成
            /// </summary>
            /// <param name="filePath">シェーダファイル名</param>
            public  Shader(string filePath) : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNShader_CreateFromFile(out _handle, filePath);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Shader(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// ビューペイン
    /// </summary>
    /// <remarks>
    /// ビューペインは、シーングラフの最終的な描画結果が書き込まれる
    /// レンダリングターゲットを表すオブジェクトです。
    /// また、ウィンドウへの描画領域の定義を行います。
    /// </remarks>
    public partial class ViewPane : ReferenceObject
    {
            static ViewPane _GetDefaultViewPane;

             ObjectList<Layer> _GetLayers;


            /// <summary>
            /// デフォルトで生成されるビューペイン
            /// </summary>
            public static ViewPane DefaultViewPane
            {
                get
                {
                    IntPtr viewPane;
                    var errorCode = API.LNViewPane_GetDefaultViewPane(out viewPane);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    if (viewPane == null) {
                        _GetDefaultViewPane = null;
                    }
                    else if (_GetDefaultViewPane == null || _GetDefaultViewPane.Handle != viewPane) {
                        _GetDefaultViewPane = new ViewPane(_LNInternal.InternalBlock);
                        _GetDefaultViewPane.SetHandle(viewPane);
                    }
                    return _GetDefaultViewPane;
                }
            }

            /// <summary>
            /// ViewPane の背景色を設定する
            /// </summary>
            public ColorF BackgroundColor
            {
                set
                {
                    var errorCode = API.LNViewPane_SetBackgroundColor(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// ViewPane に設定されている Layer のリストを取得する
            /// </summary>
            public ObjectList<Layer> Layers
            {
                get
                {
                    IntPtr objectList;
                    var errorCode = API.LNViewPane_GetLayers(_handle, out objectList);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    if (objectList == null) {
                        _GetLayers = null;
                    }
                    else if (_GetLayers == null || _GetLayers.Handle != objectList) {
                        _GetLayers = new ObjectList<Layer>(_LNInternal.InternalBlock);
                        _GetLayers.SetHandle(objectList);
                    }
                    return _GetLayers;
                }
            }


            internal ViewPane(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// レイヤー
    /// </summary>
    public partial class Layer : ReferenceObject
    {

            /// <summary>
            /// 【この機能は現在使用できません】
            /// </summary>
            public int Priority
            {
                get
                {
                    var outPriority = new int();
                    var errorCode = API.LNLayer_GetPriority(_handle, out outPriority);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outPriority;
                }
                set
                {
                    var errorCode = API.LNLayer_SetPriority(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            internal Layer(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// ビューフィルタ
    /// </summary>
    /// <remarks>
    /// ビューフィルタはビューペインに描き込まれるイメージに
    /// 特殊効果を適用するための機能です。
    /// いわゆるポストエフェクト処理を行います。
    /// </remarks>
    public partial class ViewFilter : Layer
    {

            /// <summary>
            /// 自動更新有無
            /// </summary>
            public bool IsEnableAutoUpdate
            {
                get
                {
                    var outEnabled = new bool();
                    var errorCode = API.LNViewFilter_IsEnableAutoUpdate(_handle, out outEnabled);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outEnabled;
                }
                set
                {
                    var errorCode = API.LNViewFilter_SetEnableAutoUpdate(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }


            /// <summary>
            /// フレーム更新
            /// </summary>
            /// <param name="elapsedTime">経過させる時間</param>
            /// <remarks>
            /// エフェクトやアニメーションの状態を elapsedTime 分進めます。
            /// 通常、1フレームに1度呼び出します。
            /// 自動更新が有効になっている場合はライブラリの
            /// フレーム更新を行った時に自動的に呼ばれるため、
            /// この関数を直接呼び出す必要はありません。
            /// 自動更新はデフォルトで有効です。
            /// </remarks>
            public void Update(float elapsedTime)
            {
                var errorCode = API.LNViewFilter_Update(_handle, elapsedTime);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal ViewFilter(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 色調フィルタ
    /// </summary>
    public partial class ToneFilter : ViewFilter
    {


            /// <summary>
            /// 色調を変更するビューフィルタを作成する
            /// </summary>
            public  ToneFilter() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNToneFilter_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 色調を設定する
            /// </summary>
            /// <param name="tone">設定する色調</param>
            /// <param name="duration">変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
            public void SetTone(Tone tone, float duration)
            {
                var errorCode = API.LNToneFilter_SetTone(_handle, ref tone, duration);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal ToneFilter(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// トランジションフィルタ
    /// </summary>
    public partial class TransitionFilter : ViewFilter
    {


            /// <summary>
            /// トランジションを行うビューフィルタを作成する
            /// </summary>
            public  TransitionFilter() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNTransitionFilter_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// トランジションの準備として、現在の画面を固定する
            /// </summary>
            /// <remarks>
            /// これ以降、TransitionFilter.Transition によるトランジション実行まで
            /// フィルタされた画面は更新されなくなります。
            /// </remarks>
            public void Freeze()
            {
                var errorCode = API.LNTransitionFilter_Freeze(_handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 固定された画面から現在の画面へのトランジションを実行する
            /// </summary>
            /// <param name="duration">変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
            /// <param name="maskTexture">トランジションに使用するマスク画像</param>
            /// <param name="vague">マスク画像の境界のあいまいさ</param>
            /// <remarks>
            /// maskTexture が null の場合は通常のフェードになります。
            /// </remarks>
            public void Transition(double duration, Texture maskTexture, float vague)
            {
                var errorCode = API.LNTransitionFilter_Transition(_handle, duration, (maskTexture != null) ? maskTexture.Handle : default(IntPtr), vague);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal TransitionFilter(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// ブラーフィルタ
    /// </summary>
    public partial class BlurFilter : ViewFilter
    {


            /// <summary>
            /// ブラ―(残像)を適用するビューフィルタを作成する
            /// </summary>
            public  BlurFilter() : base(_LNInternal.InternalBlock)
            {
                var errorCode = API.LNBlurFilter_Create(out _handle);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 色調を設定する
            /// </summary>
            /// <param name="duration">適用時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
            /// <param name="power">ブラーの強度 (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)</param>
            /// <param name="scale">ブラーの大きさ (長さ)</param>
            /// <param name="center">scale を使用して放射ブラーを作るときの中心座標</param>
            /// <param name="baseCamera">center の座標空間の基準となるカメラのハンドル</param>
            /// <remarks>
            /// duration が 0 より大きい場合、その時間かけて徐々に強度を 0.0 に戻します。
            /// duration が 0 の場合は強度を自動で変化させません。
            /// scale は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。
            /// 1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。
            /// 通常は 1.0～1.1 の範囲で指定するのが無難です。
            /// 1.0 より小さい値を指定すると内側向きのブラーになります。
            /// center は baseCamera が 2Dカメラであれば 2D 空間上の点、3Dカメラであれば3D空間上の点を指定します。
            /// baseCamera が null の場合、center は -1.0 ～ 1.0 で指定します。(0.0 が画面中央です)
            /// </remarks>
            public void Blur(double duration, float power, float scale, Vector3 center, Camera baseCamera)
            {
                var errorCode = API.LNBlurFilter_Blur(_handle, duration, power, scale, ref center, (baseCamera != null) ? baseCamera.Handle : default(IntPtr));
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal BlurFilter(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 3D または 2D シーンのカメラ
    /// </summary>
    public partial class Camera : SceneNode
    {
            static Camera _GetDefault3DCamera;

            static Camera _GetDefault2DCamera;


            /// <summary>
            /// デフォルトで作成される3Dカメラを取得する
            /// </summary>
            public static Camera Default3DCamera
            {
                get
                {
                    IntPtr camera;
                    var errorCode = API.LNCamera_GetDefault3DCamera(out camera);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    if (camera == null) {
                        _GetDefault3DCamera = null;
                    }
                    else if (_GetDefault3DCamera == null || _GetDefault3DCamera.Handle != camera) {
                        _GetDefault3DCamera = new Camera(_LNInternal.InternalBlock);
                        _GetDefault3DCamera.SetHandle(camera);
                    }
                    return _GetDefault3DCamera;
                }
            }

            /// <summary>
            /// デフォルトで作成される3Dカメラを取得する
            /// </summary>
            public static Camera Default2DCamera
            {
                get
                {
                    IntPtr camera;
                    var errorCode = API.LNCamera_GetDefault2DCamera(out camera);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    if (camera == null) {
                        _GetDefault2DCamera = null;
                    }
                    else if (_GetDefault2DCamera == null || _GetDefault2DCamera.Handle != camera) {
                        _GetDefault2DCamera = new Camera(_LNInternal.InternalBlock);
                        _GetDefault2DCamera.SetHandle(camera);
                    }
                    return _GetDefault2DCamera;
                }
            }

            /// <summary>
            /// カメラ姿勢方法
            /// </summary>
            public CameraType CameraType
            {
                set
                {
                    var errorCode = API.LNCamera_SetCameraType(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの位置を設定する
            /// </summary>
            public override Vector3 Position
            {
                set
                {
                    var errorCode = API.LNCamera_SetPosition(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの注視点を設定する
            /// </summary>
            public Vector3 LookAt
            {
                set
                {
                    var errorCode = API.LNCamera_SetLookAt(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの上方向を設定する
            /// </summary>
            public Vector3 UpDirection
            {
                set
                {
                    var errorCode = API.LNCamera_SetUpDirection(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの回転角度を設定する
            /// </summary>
            public override Vector3 Angle
            {
                set
                {
                    var errorCode = API.LNCamera_SetAngle(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの姿勢行列を設定する
            /// </summary>
            /// <remarks>
            /// 行列を使用し、位置(0,0,0)、正面(0,0,1)、上方向(0,1,0) を
            /// 変換して姿勢を算出します。
            /// </remarks>
            public override Matrix Matrix
            {
                set
                {
                    var errorCode = API.LNCamera_SetMatrix(_handle, ref value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラの視野角を設定する
            /// </summary>
            public float FovY
            {
                set
                {
                    var errorCode = API.LNCamera_SetFovY(_handle, value);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

                }
            }

            /// <summary>
            /// カメラのビュー行列を取得する
            /// </summary>
            public Matrix ViewMatrix
            {
                get
                {
                    var outMatrix = new Matrix();
                    var errorCode = API.LNCamera_GetViewMatrix(_handle, out outMatrix);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outMatrix;
                }
            }

            /// <summary>
            /// カメラのプロジェクション行列を取得する
            /// </summary>
            public Matrix ProjectionMatrix
            {
                get
                {
                    var outMatrix = new Matrix();
                    var errorCode = API.LNCamera_GetProjectionMatrix(_handle, out outMatrix);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outMatrix;
                }
            }

            /// <summary>
            /// ビュー行列とプロジェクション行列の積を取得する
            /// </summary>
            public Matrix ViewProjectionMatrix
            {
                get
                {
                    var outMatrix = new Matrix();
                    var errorCode = API.LNCamera_GetViewProjectionMatrix(_handle, out outMatrix);
                    if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                    return outMatrix;
                }
            }


            /// <summary>
            /// カメラの位置を設定する
            /// </summary>
            /// <param name="x">X 座標</param>
            /// <param name="y">Y 座標</param>
            /// <param name="z">Z 座標</param>
            public override void SetPosition(float x, float y, float z)
            {
                var errorCode = API.LNCamera_SetPositionXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// カメラの注視点を設定する
            /// </summary>
            /// <param name="x">注視点 X 座標</param>
            /// <param name="y">注視点 Y 座標</param>
            /// <param name="z">注視点 Z 座標</param>
            public void SetLookAt(float x, float y, float z)
            {
                var errorCode = API.LNCamera_SetLookAtXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// カメラの上方向を設定する
            /// </summary>
            /// <param name="x">上方向ベクトルの X 成分</param>
            /// <param name="y">上方向ベクトルの Y 成分</param>
            /// <param name="z">上方向ベクトルの Z 成分</param>
            public void SetUpDirection(float x, float y, float z)
            {
                var errorCode = API.LNCamera_SetUpDirectionXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// カメラの回転角度を設定する
            /// </summary>
            /// <param name="x">X 軸の回転角度 (ラジアン単位)</param>
            /// <param name="y">Y 軸の回転角度 (ラジアン単位)</param>
            /// <param name="z">Z 軸の回転角度 (ラジアン単位)</param>
            public override void SetAngle(float x, float y, float z)
            {
                var errorCode = API.LNCamera_SetAngleXYZ(_handle, x, y, z);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// カメラの最手前クリップ距離と最奥クリップ距離を設定する
            /// </summary>
            /// <param name="nearClip">手前クリップ距離</param>
            /// <param name="farClip">奥クリップ距離</param>
            public void SetNearFarClip(float nearClip, float farClip)
            {
                var errorCode = API.LNCamera_SetNearFarClip(_handle, nearClip, farClip);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            internal Camera(_LNInternal i) : base(i) {}


    };

    /// <summary>
    /// 初期設定
    /// </summary>
    public partial class Config
    {


            /// <summary>
            /// ウィンドウタイトルの設定
            /// </summary>
            /// <param name="title">ウィンドウタイトル文字列</param>
            public static void SetWindowTitle(string title)
            {
                API.LNConfig_SetWindowTitle(title);

            }

            /// <summary>
            /// ウィンドウサイズの設定
            /// </summary>
            /// <param name="width">クライアント領域の幅 (default:640)</param>
            /// <param name="height">クライアント領域の高さ (default:480)</param>
            public static void SetWindowSize(int width, int height)
            {
                API.LNConfig_SetWindowSize(width, height);

            }

            /// <summary>
            /// バックバッファサイズの設定
            /// </summary>
            /// <param name="width">バックバッファの幅 (default:0)</param>
            /// <param name="height">バックバッファの高さ (default:0)</param>
            /// <remarks>
            /// ウィンドウサイズとは異なるサイズでバックバッファを作成する時、
            /// バックバッファのサイズを指定します。
            /// サイズが 0 の場合、ウィンドウサイズと同じサイズで作成します。
            /// </remarks>
            public static void SetBackbufferSize(int width, int height)
            {
                API.LNConfig_SetBackbufferSize(width, height);

            }

            /// <summary>
            /// ウィンドウリサイズの有効設定
            /// </summary>
            /// <param name="enabled">true の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:true)</param>
            public static void SetWindowResizeEnabled(bool enabled)
            {
                API.LNConfig_SetWindowResizeEnabled(enabled);

            }

            /// <summary>
            /// アクセラレータキーとして割り当てるキーの設定
            /// </summary>
            /// <param name="acceleratorKey">アクセラレータの種別</param>
            /// <param name="keyCode">割り当てるキーコード</param>
            /// <param name="isShift">true を指定すると、Shift キーを修飾子とする</param>
            /// <param name="isCtrl">true を指定すると、Ctrl キーを修飾子とする</param>
            /// <param name="isAlt">true を指定すると、Alt キーを修飾子とする</param>
            /// <remarks>
            /// 初期状態では以下のように割り当てられています。
            /// - AcceleratorKey.ToggleFullscreen
            /// Alt + Enter キー
            /// - AcceleratorKey.ShowDebugStatistics
            /// F3 キー
            /// </remarks>
            public static void SetAcceleratorKey(AcceleratorKey acceleratorKey, KeyCode keyCode, bool isShift, bool isCtrl, bool isAlt)
            {
                API.LNConfig_SetAcceleratorKey(acceleratorKey, keyCode, isShift, isCtrl, isAlt);

            }

            /// <summary>
            /// ログファイルの出力設定
            /// </summary>
            /// <param name="flag">true=ログファイルを出力する / false=しない (default:false)</param>
            public static void SetLogFileEnabled(bool flag)
            {
                API.LNConfig_SetLogFileEnabled(flag);

            }

            /// <summary>
            /// コンソールの割り当て設定
            /// </summary>
            /// <param name="flag">true=割り当てる / false=割り当てない (default:false)</param>
            public static void SetConsoleEnabled(bool flag)
            {
                API.LNConfig_SetConsoleEnabled(flag);

            }

            /// <summary>
            /// ファイルを開くときにアクセスする暗号化アーカイブの登録
            /// </summary>
            /// <param name="filePath">アーカイブファイル名</param>
            /// <param name="password">アーカイブファイルを開くためのパスワード</param>
            public static void RegisterArchive(string filePath, string password)
            {
                API.LNConfig_RegisterArchive(filePath, password);

            }

            /// <summary>
            /// ファイルへのアクセス制限の設定
            /// </summary>
            /// <param name="priority">制限方法 (default:FileAccessPriority.DirectoryFirst)</param>
            public static void SetFileAccessPriority(FileAccessPriority priority)
            {
                API.LNConfig_SetFileAccessPriority(priority);

            }

            /// <summary>
            /// DirectMusic の使用設定
            /// </summary>
            /// <param name="mode">DirectMusic の使用方法 (default:DirectMusicInitMode.NotUse)</param>
            public static void SetDirectMusicInitMode(DirectMusicInitMode mode)
            {
                API.LNConfig_SetDirectMusicInitMode(mode);

            }

            /// <summary>
            /// 音声データの最大キャッシュ数の設定
            /// </summary>
            /// <param name="count">最大数 (default:32)</param>
            public static void SetSoundMaxCacheCount(int count)
            {
                API.LNConfig_SetSoundMaxCacheCount(count);

            }

            /// <summary>
            /// 音声再生方法の自動選択の音声データバイト数閾値
            /// </summary>
            /// <param name="bytes">最大数 (default:100000)</param>
            /// <remarks>
            /// 再生方法(オンメモリorストリーミング)の自動選択時、
            /// 音声データのバイト数がこの閾値よりも大きい場合ストリーミング、
            /// 以下の場合はオンメモリで再生されます。
            /// MP3等の圧縮フォーマットではデコード後のサイズでチェックします。
            /// </remarks>
            public static void SoundPlayTypeThreshold(int bytes)
            {
                API.LNConfig_SoundPlayTypeThreshold(bytes);

            }

            /// <summary>
            /// フレームレートの設定
            /// </summary>
            /// <param name="frameRate">フレームレート (default:60)</param>
            public static void SetFrameRate(int frameRate)
            {
                API.LNConfig_SetFrameRate(frameRate);

            }

            /// <summary>
            /// ライブラリ更新時時のフレーム待ち処理の有効設定
            /// </summary>
            /// <param name="enabled">true=有効 / false=無効 (default:true)</param>
            public static void SetEnableFrameWait(bool enabled)
            {
                API.LNConfig_SetEnableFrameWait(enabled);

            }

            /// <summary>
            /// 自動更新時のタイムスケールの設定
            /// </summary>
            /// <param name="timeScale">タイムスケール (default:0.0)</param>
            /// <remarks>
            /// SceneNode や ViewFilter の自動フレーム更新の際、
            /// 前回フレームからの経過時間に乗算される値を設定します。
            /// この値を 1.0 にすると、エフェクト等の遷移時間を時間単位、
            /// フレームレートと同じ値にするとフレーム単位で指定できるようになります。
            /// 0.0 の場合、フレームレートの値を使用します。
            /// </remarks>
            public static void SetAutoUpdateTimeScale(float timeScale)
            {
                API.LNConfig_SetAutoUpdateTimeScale(timeScale);

            }

            /// <summary>
            /// ライブラリで使用するフォントファイルを登録する
            /// </summary>
            /// <param name="filePath">フォントファイル名</param>
            /// <remarks>
            /// 登録したフォントファイルはフォントオブジェクト作成時に
            /// ファミリ名を指定することで使用できます。
            /// </remarks>
            public static void RegisterFontFile(string filePath)
            {
                API.LNConfig_RegisterFontFile(filePath);

            }

            /// <summary>
            /// テクスチャの最大キャッシュ数の設定
            /// </summary>
            /// <param name="count">最大数 (default:32)</param>
            public static void SetTextureCacheSize(int count)
            {
                API.LNConfig_SetTextureCacheSize(count);

            }

            /// <summary>
            /// シェーダの最大キャッシュ数の設定
            /// </summary>
            /// <param name="count">最大数 (default:32)</param>
            public static void SetShaderCacheSize(int count)
            {
                API.LNConfig_SetShaderCacheSize(count);

            }

            /// <summary>
            /// 例外エラー発生時のメッセージボックス表示設定
            /// </summary>
            /// <param name="enabled">true の場合、表示する (default:true)</param>
            public static void SetExceptionMessageBoxEnabled(bool enabled)
            {
                API.LNConfig_SetExceptionMessageBoxEnabled(enabled);

            }

            /// <summary>
            /// ユーザー定義のウィンドウハンドルの設定
            /// </summary>
            /// <param name="hWnd">ユーザー定義のウィンドウハンドル (default:null)</param>
            public static void SetWin32UserWindowHandle(IntPtr hWnd)
            {
                API.LNConfig_SetWin32UserWindowHandle(hWnd);

            }


    };

    /// <summary>
    /// ライブラリ管理
    /// </summary>
    public partial class Core
    {


            /// <summary>
            /// ライブラリの初期化
            /// </summary>
            /// <remarks>
            /// ライブラリのすべての機能を使用できるように初期化を行います。
            /// </remarks>
            public static void Initialize()
            {
                TypeInfo.Register();
                var errorCode = API.LNCore_Initialize();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ファイル入出力機能の初期化
            /// </summary>
            public static void InitializeFileIO()
            {
                TypeInfo.Register();
                var errorCode = API.LNCore_InitializeFileIO();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 音声機能の初期化
            /// </summary>
            /// <remarks>
            /// ファイル入出力機能も同時に初期化されます。
            /// </remarks>
            public static void InitializeAudio()
            {
                TypeInfo.Register();
                var errorCode = API.LNCore_InitializeAudio();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 入力機能の初期化
            /// </summary>
            public static void InitializeInput()
            {
                TypeInfo.Register();
                var errorCode = API.LNCore_InitializeInput();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// ライブラリのフレーム更新
            /// </summary>
            /// <remarks>
            /// ライブラリの状態を更新し、時間を1フレーム分進めます。
            /// この関数は必ず1フレームに1度だけ呼び出す必要があります。
            /// </remarks>
            public static void Update()
            {
                var errorCode = API.LNCore_Update();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// 画面の更新タイミングのリセット
            /// </summary>
            /// <remarks>
            /// 時間のかかる処理の後にこのメソッドを呼ぶことで、
            /// 極端なフレームスキップが発生しないようにすることができます。
            /// </remarks>
            public static void ResetFrameDelay()
            {
                var errorCode = API.LNCore_ResetFrameDelay();
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);

            }

            /// <summary>
            /// アプリケーションを終了するべきかを確認する
            /// </summary>
            /// <returns>終了要求の有無</returns>
            /// <remarks>
            /// ウィンドウのクローズボタンが押された場合等、
            /// アプリケーションを終了するべき時には false を返します。
            /// </remarks>
            public static bool IsEndRequested()
            {
                var requested = new bool();
                var errorCode = API.LNCore_IsEndRequested(out requested);
                if (errorCode != ErrorCode.OK) throw new LNoteException(errorCode);
                return requested;
            }

            /// <summary>
            /// ライブラリの終了処理
            /// </summary>
            public static void Terminate()
            {
                API.LNCore_Terminate();

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
            
            var _FileStream = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new FileStream(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_FileStream);
            LNFileStream_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _VirtualPad = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new VirtualPad(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_VirtualPad);
            LNVirtualPad_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Sound = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Sound(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Sound);
            LNSound_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Texture = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Texture(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Texture);
            LNTexture_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Font = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Font(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Font);
            LNFont_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _SceneNode = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new SceneNode(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_SceneNode);
            LNSceneNode_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _VisualNode = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new VisualNode(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_VisualNode);
            LNVisualNode_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Sprite = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Sprite(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Sprite);
            LNSprite_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _SpritePath = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new SpritePath(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_SpritePath);
            LNSpritePath_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Model = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Model(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Model);
            LNModel_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Effect = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Effect(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Effect);
            LNEffect_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Tilemap = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Tilemap(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Tilemap);
            LNTilemap_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Shader = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Shader(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Shader);
            LNShader_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _ViewPane = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new ViewPane(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_ViewPane);
            LNViewPane_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Layer = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Layer(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Layer);
            LNLayer_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _ViewFilter = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new ViewFilter(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_ViewFilter);
            LNViewFilter_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _ToneFilter = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new ToneFilter(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_ToneFilter);
            LNToneFilter_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _TransitionFilter = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new TransitionFilter(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_TransitionFilter);
            LNTransitionFilter_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _BlurFilter = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new BlurFilter(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_BlurFilter);
            LNBlurFilter_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));
            
            var _Camera = new TypeInfo(){ Factory = (handle) =>
                {
                    var obj = new Camera(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Camera);
            LNCamera_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));

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

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNFileStream_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNVirtualPad_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSound_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNTexture_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNFont_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSceneNode_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNVisualNode_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSprite_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSpritePath_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNModel_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNEffect_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNTilemap_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNShader_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNViewPane_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNLayer_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNViewFilter_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNToneFilter_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNTransitionFilter_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNBlurFilter_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNCamera_SetTypeUserData(IntPtr data);


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
