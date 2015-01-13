using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    static class Utils
    {
        public static bool IsHexString(string hx)
        {
            foreach (char c in hx.ToCharArray())
            {
                if (!Uri.IsHexDigit(c)) return false;
            }
            return true;
        }
    }
}
