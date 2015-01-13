using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace BinderMaker
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] apiHeaderPathes = new string[]
            {
                "../../../../../../LightNote/Source/C_API/LFBase.h",
                "../../../../../../LightNote/Source/C_API/LFMath.h",
                "../../../../../../LightNote/Source/C_API/LFFile.h",
                "../../../../../../LightNote/Source/C_API/LFInput.h",
                "../../../../../../LightNote/Source/C_API/LFAudio.h",
                "../../../../../../LightNote/Source/C_API/LFGraphics.h",
                "../../../../../../LightNote/Source/C_API/LFScene.h",
                "../../../../../../LightNote/Source/C_API/LFFramework.h",
            };

            var tt2 = typeof(int);
            var tt = typeof(C_APIAnalyzer);

            var analyzer = new C_APIAnalyzer();
            analyzer.Analyze(
                "../../../../../../LightNote/Source/Core/Common.h",
                "../../../../../../LightNote/Source/C_API/LFTypedef.h", 
                apiHeaderPathes);

            var clAnalyzer = analyzer.CLAnalyzer;
            CppBuilderCommon.Analyzer = clAnalyzer;
            CSBuilderCommon.Analyzer = clAnalyzer;

            var cppBuiler = new CppBuilder();
            cppBuiler.Build(clAnalyzer, "../../../../../Cpp/LNote/lnote.hpp");
            foreach (var file in apiHeaderPathes)
                File.Copy(file, "../../../../../Cpp/LNote/" + Path.GetFileName(file), true);
            File.Copy("../../../../../../LightNote/Source/C_API/lnote.h", "../../../../../Cpp/LNote/lnote.h", true);
            File.Copy("../../../../../../LightNote/Source/C_API/LFTypedef.h", "../../../../../Cpp/LNote/LFTypedef.h", true);
            File.Copy("../../../../../../LightNote/Source/C_API/LFCommon.h", "../../../../../Cpp/LNote/LFCommon.h", true);

            var csPInvokeBuiler = new CSPInvokeBuilder();
            csPInvokeBuiler.Build(clAnalyzer, "../../../../../DotNet/LNote.DotNet/LNote.DotNet/LNAPILib.cs");

            var csBuiler = new CSBuilder();
            csBuiler.Build(clAnalyzer, "../../../../../DotNet/LNote.DotNet/LNote.DotNet/LNClassLib.cs");

            var rubyBuiler = new RubyBuilder();
            rubyBuiler.Build(clAnalyzer, "../../../../../Ruby/lnote_ruby.cpp");

            var rubyDocBuilder = new RubyDocBuilder();
            rubyDocBuilder.Build(clAnalyzer, "../../../../../Ruby/lnote_ruby_doc.rb");


#if false

            var csPInvokeBuilder = new CSPInvokeBuilder();
            csPInvokeBuilder.BuildAPILib(analyzer, "../../../../../DotNet/LNote.DotNet/LNote.DotNet/LNAPILib.cs");

            //var csBuilder = new CSClassBuilder2();
            //csBuilder.BuildClassLib(analyzer, "../../../../../DotNet/LNote.DotNet/LNote.DotNet/LNClassLib.cs");
            var csBuilder = new CSClassBuilder3();
            csBuilder.Build(analyzer, "../../../../../DotNet/LNote.DotNet/LNote.DotNet/LNClassLib.cs");
#endif
              
#if false
            var rubyBuilder = new RubyExtBuilder();
            rubyBuilder.Build(analyzer, "../../../../../Ruby/lnote_ruby.cpp");
#endif

#if false
            var hspHeaderBuilder = new HSPHeaderBuilder();
            hspHeaderBuilder.Build(analyzer);
            
            var hspHelpBuilder = new HSPHelpBuilder();
            //hspHelpBuilder.AnalyzeUnitTestFile("../../../../../HSP/CallTest.hsp");
            hspHelpBuilder.Build(analyzer, "../../../../../HSP/lnote.hs");
            //hspHelpBuilder.BuildCallTestSource("../../../../../HSP/CallTest.hsp", "../../../../../HSP/CallTest_.hsp");
#endif
        }
    }
}
