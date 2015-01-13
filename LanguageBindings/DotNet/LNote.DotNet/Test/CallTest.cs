using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LN;

/*
 * このソースファイルは、クラスライブラリ自動生成で生成されたクラスが
 * 想定通りのプロパティを持っているか、メソッドの修飾子、引数などが正しく、
 * 正常に呼び出すことができるかをテストする。
 * 
 * コンパイルエラーにならなくて、全ての API が呼び出せればOK。
 * (エントリポイントが見つかりませんとか例外発生しないこと)
 * 値が正しく計算されているかなどは C_API 側でテストする。
 */

namespace Test
{
    class CallTest
    {
        static void TestEQ(bool r)
        {
            if (!r) new Exception();
        }

        const string MediaDir = "../../../../../../LightNote/Samples/Media/";

        #region Test_Audio
        //-----------------------------------------------------
        public static void Test_Audio()
        {
            Audio.PlayBGM(MediaDir + "ln22.ogg", 100, 100, 0);
    
            var buf1 = System.IO.File.ReadAllBytes(MediaDir + "ln22.ogg");
            Audio.PlayBGMMem(buf1, buf1.Length, 100, 100, 0);
    
            Audio.StopBGM(0);
	
            Audio.PlayBGS(MediaDir + "ln22.ogg", 100, 100, 0 );
    
            Audio.PlayBGSMem(buf1, buf1.Length, 100, 100, 0);
	
            Audio.StopBGS(0);
	
            Audio.PlayME(MediaDir + "ln22.ogg", 100, 100);
    
            Audio.PlayMEMem(buf1, buf1.Length, 100, 100);
	
            Audio.StopME();
	
            Audio.PlaySE(MediaDir + "ln22.ogg", 100, 100);
    
            Audio.PlaySE3D(MediaDir + "ln22.ogg", new Vector3(), 1, 100, 100);
	
            Audio.PlaySE3D(MediaDir + "ln22.ogg", 10, 0, 0, 1, 100, 100);
    
            Audio.PlaySEMem(buf1, buf1.Length, 100, 100);

            Audio.PlaySE3DMem(buf1, buf1.Length, new Vector3(), 1, 100, 100);
	
            Audio.PlaySE3DMem(buf1, buf1.Length, 0, 0, 0, 1, 100, 100);
	
            Audio.StopSE();
	
            Audio.SetMetreUnitDistance(10);
	
            Audio.SetBGMVolume(100, 0);
	
            Audio.SetBGSVolume(100, 0);
        }
        #endregion

        #region Test_Core
        //-----------------------------------------------------
        public static void Test_Core()
        {
            Core.ResetFrameDelay();
        }
        #endregion

        #region Test_Size
        //-----------------------------------------------------
        public static void Test_Size()
        {
            var size1 = new Size();
            var size2 = new Size(10, 20);

            size1.Width = 10;
            var w = size1.Width;

            size1.Height = 10;
            var h = size1.Height;
        }
        #endregion

        //-----------------------------------------------------
        public static void Test_Rect()
        {
            var rect1 = new Rect();
            var rect2 = new Rect(1, 2, 3, 4);

            rect1.X = 10;
            var x = rect1.X;

            rect1.Y = 10;
            var y = rect1.Y;

            rect1.Width = 10;
            var w = rect1.Width;

            rect1.Height = 10;
            var h = rect1.Height;

            bool b = rect1.Contains(1, 2);
        }

        //-----------------------------------------------------
        public static void Test_Vector2()
        {
            var v1 = new Vector2();
            var v2 = new Vector2(1, 2);

            v1.X = 10;
            var x = v1.X;

            v1.Y = 10;
            var y = v1.Y;

            //
            var len1 = v1.Length;

            //
            var len2 = v1.SquareLength;

            //
            v1.Set(1, 2);

            //
            v1 = Vector2.Normalize(v1);
        }

        //-----------------------------------------------------
        public static void Test_Vector3()
        {
            var v1 = new Vector3();
            var v2 = new Vector3(1, 2, 3);

            v1.X = 10;
            var x = v1.X;

            v1.Y = 10;
            var y = v1.Y;

            v1.Z = 10;
            var z = v1.Z;

            var len1 = v1.Length;

            var len2 = v1.SquareLength;

            v1.Set(1, 2, 3);

            v1.Set(new Vector2(1, 2), 3);

            v1 = v1.Normalize();
            v1 = Vector3.Normalize(v1);

            float d = Vector3.Dot(v1, v2);

            var v3 = Vector3.Cross(v1, v2);

            v3 = Vector3.Reflect(v1, v2);

            v3 = Vector3.Slide(v1, v2);

            v3 = Vector3.Lerp(v1, v2, 0.5f);

            v3 = Vector3.CatmullRom(v1, v2, v1, v2, 0.5f);

            v3 = Vector3.Transform(v1, Matrix.Identity);

            v3 = Vector3.TransformCoord(v1, Matrix.Identity);

            v3 = Vector3.RotateAxis(v1, v2, 0.5f);

            v3 = Vector3.RotateMatrix(v1, Matrix.Identity);
        }

        //-----------------------------------------------------
        public static void Test_Vector4()
        {
            var v1 = new Vector4();
            var v2 = new Vector4(1, 2, 3, 4);

            v1.X = 10;
            var x = v1.X;

            v1.Y = 10;
            var y = v1.Y;

            v1.Z = 10;
            var z = v1.Z;

            v1.W = 10;
            var w = v1.W;
        }

        //-----------------------------------------------------
        public static void Test_Matrix()
        {
            var m1 = new Matrix();

            //
            var v1 = m1.Right;
            //
            var v2 = m1.Up;
            //
            var v3 = m1.Front;
            //
            var v4 = m1.Position;
            //
            var v5 = Matrix.Identity;
            //
            var v6 = Matrix.Multiply(Matrix.Identity, Matrix.Identity);
            //
            m1.Translate(1, 2, 3);
            m1.Translate(new Vector3(1, 2, 3));
            //
            m1.RotateX(1);
            //
            m1.RotateY(1);
            //
            m1.RotateZ(1);
            //
            m1.Rotate(1, 2, 3, RotationOrder.XYZ);
            //
            m1.Rotate(new Vector3(1, 2, 3), RotationOrder.XYZ);
            //
            m1.RotateAxis(new Vector3(0, 1, 0), 1);
            //
            m1.RotateQuaternion(Quaternion.Identity);
            //
            m1.Scale(1);
            m1.Scale(new Vector3(1,1,1));
            m1.Scale(1, 2, 3);
            //
            m1 = Matrix.Multiply(m1, Matrix.Identity);
            //
            m1 = Matrix.Inverse(m1);
            m1 = m1.Inverse();
            //
            m1 = Matrix.Transpose(m1);
            m1 = m1.Transpose();
            //
            m1 = Matrix.ViewTransformLH(new Vector3(10, 10, 10), new Vector3(), new Vector3(0, 1, 0));
            //
            m1 = Matrix.ViewTransformRH(new Vector3(10, 10, 10), new Vector3(), new Vector3(0, 1, 0));
            //
            m1 = Matrix.PerspectiveFovLH(10, 0.5f, 1, 100);
            //
            m1 = Matrix.PerspectiveFovRH(10, 0.5f, 1, 100);
            //
            m1 = Matrix.OrthoLH(640, 480, 1, 100);
            //
            m1 = Matrix.OrthoRH(640, 480, 1, 100);
            //
            v1 = Matrix.ToEuler(m1);
            //
            v1 = m1.ToEuler();
            //
            Matrix.Decompose(m1, out v1, out m1, out v3);
            //
            m1 = Matrix.TransformBasis(m1);
            m1 = m1.TransformBasis();
        }

        //-----------------------------------------------------
        public static void Test_Quaternion()
        {
            // コンストラクタ
            var q1 = new Quaternion();
            q1 = new Quaternion(1, 2, 3, 4);

            // フィールドアクセス
            q1.X = q1.X = 0;
            q1.Y = q1.Y = 0;
            q1.Z = q1.Z = 0;
            q1.W = q1.W = 0;

            q1.Set(1, 2, 3, 4);


            q1 = Quaternion.Identity;

            q1 = Quaternion.RotationAxis(new Vector3(0, 1, 0), 1.0f);
            q1 = Quaternion.RotationMatrix(Matrix.Identity);
            q1 = Quaternion.RotationYawPitchRoll(1, 2, 3);

            q1 = Quaternion.Normalize(q1);
            q1 = q1.Normalize();

            q1 = Quaternion.Conjugate(q1);
            q1 = q1.Conjugate();

            q1 = Quaternion.Multiply(q1, Quaternion.Identity);
            q1 = Quaternion.Slerp(q1, Quaternion.Identity, 1.0f);
        }

        //-----------------------------------------------------
        public static void Test_Random()
        {
            var r1 = new LN.Random();
            var r2 = new LN.Random(10);

            r1.Seed = 10;

            var v1 = r1.GetInt();
            v1 = r1.GetIntRange(-10, 10);
            v1 = r1.GetIntDeflect(-10, 10);
        }

        //-----------------------------------------------------
        public static void Test_ObjectList()
        {
            var layers = ViewPane.DefaultViewPane.Layers;
            if (layers.Count != 2) throw new Exception();
        }
        
        //-----------------------------------------------------
        public static void Test_IntTable()
        {
            int[] ary = new int[] {
                1, 2, 3,
                4, 5, 6,

                7, 8, 9,
                10, 11, 12,
            };

            var table1 = new IntTable(3, 2, 2, ary);
            TestEQ(table1.XSize == 3);
            TestEQ(table1.YSize == 2);
            TestEQ(table1.ZSize == 2);
            TestEQ(table1.GetValue(0, 0, 0) == 1);
            TestEQ(table1.GetValue(1, 0, 0) == 2);
            TestEQ(table1.GetValue(2, 0, 0) == 3);
            TestEQ(table1.GetValue(0, 1, 0) == 4);
            TestEQ(table1.GetValue(1, 1, 0) == 5);
            TestEQ(table1.GetValue(2, 1, 0) == 6);
            TestEQ(table1.GetValue(0, 0, 1) == 7);
            TestEQ(table1.GetValue(1, 0, 1) == 8);
            TestEQ(table1.GetValue(2, 0, 1) == 9);
            TestEQ(table1.GetValue(0, 1, 1) == 10);
            TestEQ(table1.GetValue(1, 1, 1) == 11);
            TestEQ(table1.GetValue(2, 1, 1) == 12);
        }


        
    }
}
