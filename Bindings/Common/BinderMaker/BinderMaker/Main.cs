using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Sprache;
using System.Runtime.InteropServices;
using System.Reflection;

namespace BinderMaker
{
    class Program
    {
        class Test : IDisposable
        {
            ~Test()
            {
                Console.WriteLine("~");
                Dispose();
            }

            public void Dispose()
            {
                Console.WriteLine("Dispose");
            }
        }


        static void Main(string[] args)
        {
            Test ttt = new Test();

            var a = new List<int>(10);
            var b = new Stack<int>();
           // a[5] = 0;
            ttt = null;

            GC.Collect();
            


            CLManager.Instance = new CLManager();
            CLManager.Instance.Initialize();

            var typedefParser = new Parser.CLTypedefHeaderParser();
            var typedefs = typedefParser.Analyze("../../../../../../LightNote/Source/C_API/LFTypedef2.h");

            var apiHeaders = new string[]
            {
                "../../../../../../LightNote/Source/C_API/LFBase.h",
                //"../../../../../../LightNote/Source/C_API/LFMath.h",
                //"../../../../../../LightNote/Source/C_API/LFAudio2.h",
                //"../../../../../../LightNote/Source/C_API/LFFramework.h",
            };

            var parser = new Parser.CLAPIHeaderParser();
            var modules = parser.Analyze(apiHeaders);

            CLManager.Instance.RegisterEntities(typedefs, modules);
            CLManager.Instance.LinkEntities();

            //Console.WriteLine(modules);

            //var a = File.ReadAllText();

            //IEnumerable<char> b = new char[] { 'a', 'b' };
            //Console.WriteLine(new string(b));//b.ToString());

            var csPInvoleBuilder = new Builder.CSPInvokeBuilder();
            csPInvoleBuilder.Build(CLManager.Instance, "../../../../../DotNet/LNote.DotNet/API.cs");

            var csClassLibBuilder = new Builder.CSClassLibBuilder();
            csClassLibBuilder.Build(CLManager.Instance, "../../../../../DotNet/LNote.DotNet/Classlib.cs");
        }
    }
}
