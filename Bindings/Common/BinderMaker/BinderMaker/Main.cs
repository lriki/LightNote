using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Sprache;
using System.Runtime.InteropServices;

namespace BinderMaker
{
    class Program
    {
        static void Main(string[] args)
        {
            CLManager.Instance = new CLManager();

            var typedefParser = new Parser.CLTypedefHeaderParser();
            typedefParser.Analyze("../../../../../../LightNote/Source/C_API/LFTypedef2.h");

            var apiHeaders = new string[]
            {
                "../../../../../../LightNote/Source/C_API/LFAudio2.h",
            };

            var parser = new Parser.CLAPIHeaderParser();
            var modules = parser.Analyze(apiHeaders);

            Console.WriteLine(modules);

            //var a = File.ReadAllText();

            //IEnumerable<char> b = new char[] { 'a', 'b' };
            //Console.WriteLine(new string(b));//b.ToString());
        }
    }
}
