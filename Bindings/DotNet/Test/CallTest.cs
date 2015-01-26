using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LN;

namespace Test
{
    class CallTest
    {
        static void TestEQ(bool r)
        {
            if (!r) new Exception();
        }

        #region Rect
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
        #endregion

        #region IntTable
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
        #endregion
    }
}
