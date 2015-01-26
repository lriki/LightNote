using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LN;

namespace Test
{
    class Program
    {
        #region Config
        void Test_Config()
        {
            Config.SetWindowTitle("test");
            Config.SetWindowSize(640, 480);
            Config.SetBackbufferSize(640, 480);
            Config.SetFrameRate(60);
            Config.SetEnableFrameWait(true);
            Config.SetAutoUpdateTimeScale(0.0f);
            Config.SetWindowResizeEnabled(true);
            Config.SetAcceleratorKey(AcceleratorKey.ToggleFullscreen, KeyCode.Enter, false, false, true);
            Config.SetLogFileEnabled(true);
            Config.SetConsoleEnabled(true);
            Config.RegisterArchive("TestArchive.lna", "test");
            Config.SetFileAccessPriority(FileAccessPriority.DirectoryFirst);
            Config.SetDirectMusicInitMode(DirectMusicInitMode.NotUse);
            Config.SetSoundMaxCacheCount(32);
            Config.SoundPlayTypeThreshold(100000);
            Config.RegisterFontFile("VLGothic.ttf");
            Config.SetTextureCacheSize(32);
            Config.SetShaderCacheSize(32);
            Config.SetExceptionMessageBoxEnabled(true);
            Config.SetWin32UserWindowHandle(IntPtr.Zero);
        }
        #endregion

        #region Application
        void Test_Application()
        {
        }
        #endregion

        static void Main(string[] args)
        {
            CallTest.Test_Rect();
        }
    }
}
