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
        static void Main(string[] args)
        {
            Core.Initialize();

            CallTest.Test_Audio();

            CallTest.Test_Core();
            CallTest.Test_Size();
            CallTest.Test_Rect();
            CallTest.Test_Vector2();
            CallTest.Test_Vector3();
            CallTest.Test_Vector4();
            CallTest.Test_Matrix();
            CallTest.Test_Quaternion();
            CallTest.Test_Random();
            CallTest.Test_ObjectList();
            CallTest.Test_IntTable();

            Core.Terminate();
#if false
            Manager.Initialize();

            var sound1 = new Sound(@"D:\Proj\lightnote\LanguageBindings\Cpp\LNote_Cpp\ZIGG-ZAGG.mp3");
            sound1.Pitch = 105;
            sound1.Play();

            var tex1 = new Texture(@"../../../../../../LightNote/Samples/Media/0129.png");
            var sprite1 = new Sprite(tex1);

            var transition = new TransitionFilter();
            var viewPane = ViewPane.DefaultViewPane;
            viewPane.Layers.Add(transition);

            var transitionTex = new Texture(@"../../../../../../LightNote/Samples/Media/Transition1.png");
            transition.Freeze();
            transition.Transition(600, transitionTex, 40);

            do
            {
                Manager.Update();
            } while(!Manager.IsEndRequested());

            Manager.Terminate();
#endif

#if false
            API.LManager_Initialize();


            IntPtr texture;
            API.LTexture_CreateFromFile(out texture, "../../../../../../LightNote/Samples/Media/0129.png");
            IntPtr sprite;
            API.LSprite_Create(out sprite, texture);

            IntPtr viewPane;
            API.LViewPane_GetDefaultViewPane(out viewPane);

            IntPtr layerList;
            API.LViewPane_GetLayers(viewPane, out layerList);

            IntPtr transition1;
            API.LTransitionFilter_Create(out transition1);

            API.LObjectList_Add(layerList, transition1);


            IntPtr filterTexture;
            API.LTexture_CreateFromFile(out filterTexture, "../../../../../../LightNote/Samples/Media/Transition1.png");
            //API.LTransitionFilter_Transition(transition1, 60, filterTexture, 40);

            bool state;
            do
            {
                var c = Controller.GetController(0);
                if (c.IsPress(0))
                    break;

                API.LManager_Update();
                API.LManager_IsEndRequested(out state);
            } while (!state);


            API.LManager_Terminate();
#endif

#if false
            Manager.Initialize();

            var tex = new Texture(@"D:\Proj\lightnote\LightNote\Samples\Media\20140309.png");

            tex.DrawText("てきすと", new Rect(0, 0, 256, 256), null, -1, TextAlign.Left);
            var sprite = new Sprite(tex);
            sprite.IsFlipH = true;
            Console.WriteLine(sprite.IsFlipH);
            sprite.SetPosition(10, 20, 0);

            var mat1 = new Matrix();
            mat1 = Matrix.Identity();
            //Matrix.RotateY();

            var sound = new Sound(@"D:\Media\Music\Material\ep-melody\summers.mp3", false, SoundPlayingType.Auto);
            sound.Play();
            sound.Pitch = 120;

            ViewPane.DefaultViewPane.BackgroundColor = new Color(0, 255, 255, 255);


            CallTest.Test_Size();
            CallTest.Test_Rect();
            CallTest.Test_Vector2();
            CallTest.Test_Vector3();

            do
            {

            } while (Manager.Update());

            Manager.Terminate();
#endif
        }
    }
}
