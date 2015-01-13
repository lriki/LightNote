
using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(ErrorCode errorCode);

    public enum ErrorCode
    {
        /// <summary>
        /// 成功
        /// </summary>
        OK = 0,

        /// <summary>
        /// 不明なエラー
        /// </summary>
        Unknown = -1,

        /// <summary>
        /// オブジェクトの現在の状態に対して無効な呼び出しが行われた
        /// </summary>
        InvalidOperation = -2,

        /// <summary>
        /// 渡された引数のいずれかが無効
        /// </summary>
        Argument = -3,

        /// <summary>
        /// メモリ不足
        /// </summary>
        OutOfMemory = -2,

        /// <summary>
        /// ファイルが存在しない、またはアクセスできない
        /// </summary>
        FileNotFound = -3,

        /// <summary>
        /// システム API 呼び出しエラー
        /// </summary>
        SystemCall = -6,

        /// <summary>
        /// 未実装の機能を呼び出そうとした
        /// </summary>
        NotImplemented = -7,

        /// <summary>
        /// ファイル形式などの入力データのフォーマットが無効、またはサポートされていない
        /// </summary>
        InvalidFormat = -8,

        /// <summary>
        /// スレッド実行中に例外が発生した
        /// </summary>
        Thread = -9,

        /// <summary>
        /// 継続不能なエラー
        /// </summary>
        Fatal = -10,

        /// <summary>
        /// COM 呼び出しエラー
        /// </summary>
        ComCall = -11,

        /// <summary>
        /// OpenGL 関係のエラー
        /// </summary>
        Opengl = -12,

        Directx = -13,

        Win32api = -14,

    }

    public enum FileAccessPriority
    {
        /// <summary>
        /// アーカイブ優先
        /// </summary>
        ArchiveFirst = 0,

        /// <summary>
        /// ディレクトリ優先
        /// </summary>
        DirectoryFirst = 1,

        /// <summary>
        /// アーカイブのみ
        /// </summary>
        ArchiveOnly = 2,

    }

    public enum FileType
    {
        /// <summary>
        /// バイナリモードで開く
        /// </summary>
        Binary = 0,

        /// <summary>
        /// テキストモードで開く
        /// </summary>
        Text = 1,

    }

    public enum FileMode
    {
        /// <summary>
        /// 既存のファイルを開く
        /// </summary>
        Open = 0,

        /// <summary>
        /// 新しいファイルを作成する。既に存在する場合は上書きされる
        /// </summary>
        Create = 1,

        /// <summary>
        /// ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する
        /// </summary>
        Append = 2,

        Max = 3,

    }

    public enum FileAccess
    {
        /// <summary>
        /// 読み取りアクセス
        /// </summary>
        Read = 0,

        /// <summary>
        /// 読み取り及び書き込みアクセス
        /// </summary>
        Readwrite = 1,

        /// <summary>
        /// 書き込みアクセス
        /// </summary>
        Write = 2,

    }

    public enum SeekOrigin
    {
        /// <summary>
        /// ファイルの先頭
        /// </summary>
        Begin = 0,

        /// <summary>
        /// 現在のシーク位置
        /// </summary>
        Current = 1,

        /// <summary>
        /// ファイルの末尾
        /// </summary>
        End = 2,

    }

    public enum AsyncIOState
    {
        /// <summary>
        /// 待機状態 (初期状態)
        /// </summary>
        Idle = 0,

        /// <summary>
        /// 実行可能状態 (実際の処理開始を待っている)
        /// </summary>
        Ready = 1,

        /// <summary>
        /// 実行状態 (処理中)
        /// </summary>
        Processing = 2,

        /// <summary>
        /// 終了状態 (成功)
        /// </summary>
        Completed = 3,

        /// <summary>
        /// 終了状態 (処理終了後・処理に失敗した)
        /// </summary>
        Failed = 4,

    }

    public enum RotationOrder
    {
        /// <summary>
        /// X → Y → Z
        /// </summary>
        XYZ = 0,

        /// <summary>
        /// X → Z → Y
        /// </summary>
        XZY = 1,

        /// <summary>
        /// Y → X → Z
        /// </summary>
        YXZ = 2,

        /// <summary>
        /// Y → Z → X
        /// </summary>
        YZX = 3,

        /// <summary>
        /// Z → X → Y
        /// </summary>
        ZXY = 4,

        /// <summary>
        /// Z → Y → X
        /// </summary>
        ZYX = 5,

    }

    public enum KeyCode
    {
        Unknown = 0,

        A = 1,

        B = 2,

        C = 3,

        D = 4,

        E = 5,

        F = 6,

        G = 7,

        H = 8,

        I = 9,

        J = 10,

        K = 11,

        L = 12,

        M = 13,

        N = 14,

        O = 15,

        P = 16,

        Q = 17,

        R = 18,

        S = 19,

        T = 20,

        U = 21,

        V = 22,

        W = 23,

        X = 24,

        Y = 25,

        Z = 26,

        /// <summary>
        /// '0' (NumPad ではない)
        /// </summary>
        D0 = 27,

        /// <summary>
        /// '1' (NumPad ではない)
        /// </summary>
        D1 = 28,

        /// <summary>
        /// '2' (NumPad ではない)
        /// </summary>
        D2 = 29,

        /// <summary>
        /// '3' (NumPad ではない)
        /// </summary>
        D3 = 30,

        /// <summary>
        /// '4' (NumPad ではない)
        /// </summary>
        D4 = 31,

        /// <summary>
        /// '5' (NumPad ではない)
        /// </summary>
        D5 = 32,

        /// <summary>
        /// '6' (NumPad ではない)
        /// </summary>
        D6 = 33,

        /// <summary>
        /// '7' (NumPad ではない)
        /// </summary>
        D7 = 34,

        /// <summary>
        /// '8' (NumPad ではない)
        /// </summary>
        D8 = 35,

        /// <summary>
        /// '9' (NumPad ではない)
        /// </summary>
        D9 = 36,

        F1 = 37,

        F2 = 38,

        F3 = 39,

        F4 = 40,

        F5 = 41,

        F6 = 42,

        F7 = 43,

        F8 = 44,

        F9 = 45,

        F10 = 46,

        F11 = 47,

        F12 = 48,

        Space = 49,

        Escape = 50,

        Up = 51,

        Down = 52,

        Left = 53,

        Right = 54,

        Lshift = 55,

        Rshift = 56,

        Lctrl = 57,

        Rctrl = 58,

        Lalt = 59,

        Ralt = 60,

        Tab = 61,

        Enter = 62,

        Backspace = 63,

        Insert = 64,

        Delete = 65,

        Pageup = 66,

        Pagedown = 67,

        Home = 68,

        End = 69,

        /// <summary>
        /// :	[非推奨 GLFWでは;]
        /// </summary>
        Colon = 70,

        /// <summary>
        /// ;	[非推奨 GLFWでは=]
        /// </summary>
        Semicolon = 71,

        /// <summary>
        /// ,
        /// </summary>
        Comma = 72,

        /// <summary>
        /// .
        /// </summary>
        Period = 73,

        /// <summary>
        /// /
        /// </summary>
        Slash = 74,

        /// <summary>
        /// -
        /// </summary>
        Minus = 75,

        /// <summary>
        /// \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
        /// </summary>
        Backslash = 76,

        /// <summary>
        /// \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
        /// </summary>
        Yen = 77,

        /// <summary>
        /// ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
        /// </summary>
        Caret = 78,

        /// <summary>
        /// [
        /// </summary>
        Lbracket = 79,

        /// <summary>
        /// ]
        /// </summary>
        Rbracket = 80,

        /// <summary>
        /// Android '+'
        /// </summary>
        AndroidPlus = 81,

    }

    public enum AcceleratorKey
    {
        /// <summary>
        /// フルスクリーン切り替えを行うアクセラレータキー
        /// </summary>
        ToggleFullscreen = 0,

        /// <summary>
        /// デバッグ用の情報を表示切替を行うアクセラレータキー
        /// </summary>
        ShowDebugStatistics = 1,

    }

    public enum Button
    {
        /// <summary>
        /// Aボタン (キーボード='Z' ジョイパッド=1番ボタン)
        /// </summary>
        A = 0,

        /// <summary>
        /// Bボタン (キーボード='X' ジョイパッド=2)
        /// </summary>
        B = 1,

        /// <summary>
        /// Cボタン (キーボード='C' ジョイパッド=3)
        /// </summary>
        C = 2,

        /// <summary>
        /// Xボタン (キーボード='A' ジョイパッド=4)
        /// </summary>
        X = 3,

        /// <summary>
        /// Yボタン (キーボード='S' ジョイパッド=5)
        /// </summary>
        Y = 4,

        /// <summary>
        /// Zボタン (キーボード='D' ジョイパッド=6)
        /// </summary>
        Z = 5,

        /// <summary>
        /// Lボタン (キーボード='Q' ジョイパッド=7)
        /// </summary>
        L = 6,

        /// <summary>
        /// Rボタン (キーボード='W' ジョイパッド=8)
        /// </summary>
        R = 7,

        /// <summary>
        /// L2ボタン (キーボード=None ジョイパッド=9)
        /// </summary>
        L2 = 8,

        /// <summary>
        /// R2ボタン (キーボード=None ジョイパッド=10)
        /// </summary>
        R2 = 9,

        /// <summary>
        /// L3ボタン (キーボード=None ジョイパッド=11)
        /// </summary>
        L3 = 10,

        /// <summary>
        /// R3ボタン (キーボード=None ジョイパッド=12)
        /// </summary>
        R3 = 11,

        /// <summary>
        /// Startボタン (キーボード=None ジョイパッド=13)
        /// </summary>
        Start = 12,

        /// <summary>
        /// Selectボタン (キーボード=None ジョイパッド=14)
        /// </summary>
        Select = 13,

        /// <summary>
        /// Sub1ボタン (キーボード=None ジョイパッド=15)
        /// </summary>
        Sub1 = 14,

        /// <summary>
        /// Sub2ボタン (キーボード=None ジョイパッド=16)
        /// </summary>
        Sub2 = 15,

        /// <summary>
        /// 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
        /// </summary>
        Left = 16,

        /// <summary>
        /// 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
        /// </summary>
        Right = 17,

        /// <summary>
        /// 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)
        /// </summary>
        Up = 18,

        /// <summary>
        /// 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)
        /// </summary>
        Down = 19,

        /// <summary>
        /// 第1レバー X- 方向
        /// </summary>
        Axis1XMinuis = 20,

        /// <summary>
        /// 第1レバー X+ 方向
        /// </summary>
        Axis1XPlus = 21,

        /// <summary>
        /// 第1レバー Y- 方向
        /// </summary>
        Axis1YMinuis = 22,

        /// <summary>
        /// 第1レバー Y+ 方向
        /// </summary>
        Axis1YPlus = 23,

        /// <summary>
        /// 第2レバー X- 方向
        /// </summary>
        Axis2XMinuis = 24,

        /// <summary>
        /// 第2レバー X+ 方向
        /// </summary>
        Axis2XPlus = 25,

        /// <summary>
        /// 第2レバー Y- 方向
        /// </summary>
        Axis2YMinuis = 26,

        /// <summary>
        /// 第2レバー Y+ 方向
        /// </summary>
        Axis2YPlus = 27,

    }

    public enum DeviceButtonType
    {
        /// <summary>
        /// 不明なデバイス種類
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// キーボード
        /// </summary>
        Keyboard = 1,

        /// <summary>
        /// マウスボタン
        /// </summary>
        Mouse = 2,

        /// <summary>
        /// ジョイパッドのボタン
        /// </summary>
        JoypadButton = 3,

        /// <summary>
        /// ジョイパッドのPOV
        /// </summary>
        JoypadPov = 4,

        /// <summary>
        /// ジョイパッドの軸
        /// </summary>
        JoypadAxis = 5,

    }

    public enum DeviceButton
    {
        Unknown = 0,

        Mouse1 = 256,

        Mouse2 = 257,

        Mouse3 = 258,

        Mouse4 = 259,

        Mouse5 = 260,

        Mouse6 = 261,

        Mouse7 = 262,

        Mouse8 = 263,

        JoypadButton1 = 264,

        JoypadButton2 = 265,

        JoypadButton3 = 266,

        JoypadButton4 = 267,

        JoypadButton5 = 268,

        JoypadButton6 = 269,

        JoypadButton7 = 270,

        JoypadButton8 = 271,

        JoypadButton9 = 272,

        JoypadButton10 = 273,

        JoypadButton11 = 274,

        JoypadButton12 = 275,

        JoypadButton13 = 276,

        JoypadButton14 = 277,

        JoypadButton15 = 278,

        JoypadButton16 = 279,

        JoypadPovUp = 280,

        JoypadPovRight = 281,

        JoypadPovDown = 282,

        JoypadPovLeft = 283,

        /// <summary>
        /// X1 -1.0
        /// </summary>
        JoypadAxis1Minuis = 284,

        /// <summary>
        /// X1  1.0
        /// </summary>
        JoypadAxis1Plus = 285,

        /// <summary>
        /// Y1 -1.0
        /// </summary>
        JoypadAxis2Minuis = 286,

        /// <summary>
        /// Y1  1.0
        /// </summary>
        JoypadAxis2Plus = 287,

        /// <summary>
        /// X2 -1.0
        /// </summary>
        JoypadAxis3Minuis = 288,

        /// <summary>
        /// X2  1.0
        /// </summary>
        JoypadAxis3Plus = 289,

        /// <summary>
        /// Y2 -1.0
        /// </summary>
        JoypadAxis4Minuis = 290,

        /// <summary>
        /// Y2  1.0
        /// </summary>
        JoypadAxis4Plus = 291,

        /// <summary>
        /// X3 -1.0
        /// </summary>
        JoypadAxis5Minuis = 292,

        /// <summary>
        /// X3  1.0     XInput LT
        /// </summary>
        JoypadAxis5Plus = 293,

        /// <summary>
        /// Y3 -1.0
        /// </summary>
        JoypadAxis6Minuis = 294,

        /// <summary>
        /// Y3  1.0     XInput RT
        /// </summary>
        JoypadAxis6Plus = 295,

        /// <summary>
        /// X4 -1.0
        /// </summary>
        JoypadAxis7Minuis = 296,

        /// <summary>
        /// X4  1.0
        /// </summary>
        JoypadAxis7Plus = 297,

        /// <summary>
        /// Y4 -1.0
        /// </summary>
        JoypadAxis8Minuis = 298,

        /// <summary>
        /// Y4  1.0
        /// </summary>
        JoypadAxis8Plus = 299,

    }

    public enum DirectMusicInitMode
    {
        /// <summary>
        /// DirectMusic を使用しない
        /// </summary>
        NotUse = 0,

        /// <summary>
        /// 通常
        /// </summary>
        Normal = 1,

        /// <summary>
        /// 別スレッドで初期化して、再生時に未完了の場合は待つ
        /// </summary>
        ThreadWait = 2,

        /// <summary>
        /// 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
        /// </summary>
        ThreadRequest = 3,

    }

    public enum SoundPlayingType
    {
        /// <summary>
        /// 不明な再生方法
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
        /// </summary>
        Auto = 1,

        /// <summary>
        /// オンメモリ
        /// </summary>
        Onmemory = 2,

        /// <summary>
        /// ストリーミング
        /// </summary>
        Streaming = 3,

        /// <summary>
        /// SMF
        /// </summary>
        Midi = 4,

    }

    public enum SoundPlayingState
    {
        /// <summary>
        /// 停止中
        /// </summary>
        Stopped = 0,

        /// <summary>
        /// 再生中
        /// </summary>
        Playing = 1,

        /// <summary>
        /// 一時停止中
        /// </summary>
        Pausing = 2,

    }

    public enum SoundFadeState
    {
        /// <summary>
        /// 継続(なにもしない)
        /// </summary>
        Continue = 0,

        /// <summary>
        /// 停止する (stop())
        /// </summary>
        Stop = 1,

        /// <summary>
        /// 停止して、音量を元の値に戻す
        /// </summary>
        StopReset = 2,

        /// <summary>
        /// 一時停止する (pause( true ))
        /// </summary>
        Pause = 3,

        /// <summary>
        /// 一時停止して、音量を元の値に戻す
        /// </summary>
        PauseReset = 4,

    }

    public enum InternalGameSound
    {
        /// <summary>
        /// BGM
        /// </summary>
        BGM = 0,

        /// <summary>
        /// BGS
        /// </summary>
        BGS = 1,

        /// <summary>
        /// ME
        /// </summary>
        ME = 2,

    }

    public enum BackbufferResizeMode
    {
        /// <summary>
        /// バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
        /// </summary>
        Scaling = 0,

        /// <summary>
        /// バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
        /// </summary>
        ScalingWithLetterBox = 1,

        /// <summary>
        /// バックバッファをウィンドウサイズに合わせてリサイズする
        /// </summary>
        Resize = 2,

    }

    public enum TextureFormat
    {
        /// <summary>
        /// 不明なフォーマット
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 32 ビットのアルファ付きフォーマット
        /// </summary>
        A8R8G8B8 = 1,

        /// <summary>
        /// 32 ビットのアルファ無しフォーマット
        /// </summary>
        X8R8G8B8 = 2,

        /// <summary>
        /// 64 ビットの浮動小数点フォーマット
        /// </summary>
        A16B16G16R16F = 3,

        /// <summary>
        /// 128 ビットの浮動小数点フォーマット
        /// </summary>
        A32B32G32R32F = 4,

        /// <summary>
        /// 32 ビットの Z バッファフォーマット
        /// </summary>
        D24S8 = 5,

    }

    public enum BlendMode
    {
        /// <summary>
        /// 通常 (アルファ無視)
        /// </summary>
        Normal = 0,

        /// <summary>
        /// アルファブレンド (デフォルト)
        /// </summary>
        Alpha = 1,

        /// <summary>
        /// 加算
        /// </summary>
        Add = 2,

        /// <summary>
        /// 加算（アルファチャンネル無視）
        /// </summary>
        AddAlphaDisable = 3,

        /// <summary>
        /// 減算
        /// </summary>
        Sub = 4,

        /// <summary>
        /// 減算（アルファチャンネル無視）
        /// </summary>
        SubAlphaDisable = 5,

        /// <summary>
        /// 乗算（アルファチャンネル無視）
        /// </summary>
        Mul = 6,

        /// <summary>
        /// スクリーン（アルファチャンネル無視）
        /// </summary>
        Screen = 7,

        /// <summary>
        /// リバース
        /// </summary>
        Reverse = 8,

    }

    public enum CullingMode
    {
        /// <summary>
        /// 前面表示
        /// </summary>
        Front = 0,

        /// <summary>
        /// 裏面表示
        /// </summary>
        Back = 1,

        /// <summary>
        /// 両面表示
        /// </summary>
        Double = 2,

    }

    public enum TextAlign
    {
        /// <summary>
        /// 左揃え
        /// </summary>
        Left = 0,

        /// <summary>
        /// 中央揃え
        /// </summary>
        Center = 1,

        /// <summary>
        /// 右揃え
        /// </summary>
        Right = 2,

    }

    public enum PlacementType
    {
        /// <summary>
        /// 通常の配置方法
        /// </summary>
        Normal = 0,

        /// <summary>
        /// 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
        /// </summary>
        Billboard = 1,

        /// <summary>
        /// Y 軸固定
        /// </summary>
        YFixed = 2,

    }

    public enum FrontAxis
    {
        /// <summary>
        /// X+ 方向 (右向き)
        /// </summary>
        X = 0,

        /// <summary>
        /// Y+ 方向 (上向き)
        /// </summary>
        Y = 1,

        /// <summary>
        /// Z+ 方向 (奥向き)
        /// </summary>
        Z = 2,

        /// <summary>
        /// X- 方向 (左向き)
        /// </summary>
        Rx = 3,

        /// <summary>
        /// Y- 方向 (下向き)
        /// </summary>
        Ry = 4,

        /// <summary>
        /// Z- 方向 (手前向き)
        /// </summary>
        Rz = 5,

    }

    [Flags]
    public enum MeshCreateFlags
    {
        /// <summary>
        /// ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
        /// </summary>
        SoftwareSkinning = 0x01,

        /// <summary>
        /// 物理演算のメッシュボディとして扱うメッシュを生成する
        /// </summary>
        CollisionMesh = 0x02,

        /// <summary>
        /// マテリアルに含まれるテクスチャファイルが見つからなくても無視する
        /// </summary>
        IgnoreTextureNotFound = 0x04,

    }

    public enum CameraType
    {
        /// <summary>
        /// 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
        /// </summary>
        PositionAndLookat = 0,

        /// <summary>
        /// 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
        /// </summary>
        PositionAndAngle = 1,

        /// <summary>
        /// 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する
        /// </summary>
        TransformMatrix = 2,

    }

    public enum Coord
    {
        /// <summary>
        /// 2D 空間
        /// </summary>
        Space2D = 0,

        /// <summary>
        /// 3D 空間
        /// </summary>
        Space3D = 1,

    }

    public enum ObjectType
    {
        Sound = 0,

    }


    public class API
    {
    
#if DEBUG
    	internal const string DLLName = "LNote.Debug.dll";
#else
    	internal const string DLLName = "LNote.dll";
#endif
        internal const CharSet DLLCharSet = CharSet.Ansi;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
    /// <summary>
    /// 指定した x y 座標の点が矩形に含まれているかを判別する
    /// </summary>
    /// <param name="rect">処理の基になる Rect</param>
    /// <param name="x">X 座標</param>
    /// <param name="y">Y 座標</param>
    /// <param name="result">結果を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNRect_Contains(ref Rect rect, int x, int y, out bool result);

    /// <summary>
    /// 例外発生時にコールバックされる関数を登録する
    /// </summary>
    /// <param name="handler"></param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNException_SetHandler(ExceptionCallback handler);

    /// <summary>
    /// 例外を発生させる
    /// </summary>
    /// <param name="errCode"></param>
    /// <param name="message"></param>
    /// <param name="file"></param>
    /// <param name="line"></param>
    /// <param name="func"></param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNException_Raise(ErrorCode errCode, string message, string file, int line, string func);

    /// <summary>
    /// 最後に発生した例外のエラーコードを取得する
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNException_GetLastErrorCode();

    /// <summary>
    /// 最後に発生した例外に対して、デフォルトの処理を行う (メッセージボックスを表示し、LNException_SetHandled(true))
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNException_ProcDefault();

    /// <summary>
    /// オブジェクトの解放
    /// </summary>
    /// <param name="hadnleObject">オブジェクトハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObject_Release(IntPtr hadnleObject);

    /// <summary>
    /// オブジェクトの参照カウントをインクリメントする
    /// </summary>
    /// <param name="hadnleObject">オブジェクトハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObject_AddRef(IntPtr hadnleObject);

    /// <summary>
    /// オブジェクトの参照カウント取得する
    /// </summary>
    /// <param name="hadnleObject">オブジェクトハンドル</param>
    /// <param name="count">参照カウントを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObject_GetRefCount(IntPtr hadnleObject, out int count);

    /// <summary>
    /// オブジェクトリストに格納されているオブジェクトの数を取得する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="count">要素の数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_GetCount(IntPtr objectList, out int count);

    /// <summary>
    /// オブジェクトリストの指定したインデックスにオブジェクトを設定する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="index">インデックス(要素番号)</param>
    /// <param name="item">設定するオブジェクト</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_SetAt(IntPtr objectList, int index, IntPtr item);

    /// <summary>
    /// オブジェクトリストの指定したインデックスのオブジェクトを取得する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="index">インデックス(要素番号)</param>
    /// <param name="outItem">オブジェクトを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_GetAt(IntPtr objectList, int index, out IntPtr outItem);

    /// <summary>
    /// オブジェクトリストの末尾にオブジェクトを追加する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="item">追加するオブジェクト</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_Add(IntPtr objectList, IntPtr item);

    /// <summary>
    /// オブジェクトリストから全てのオブジェクトを削除する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_Clear(IntPtr objectList);

    /// <summary>
    /// オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="index">item を挿入するインデックス</param>
    /// <param name="item">挿入するオブジェクト</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_Insert(IntPtr objectList, int index, IntPtr item);

    /// <summary>
    /// オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="item">リストから削除するオブジェクト</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_Remove(IntPtr objectList, IntPtr item);

    /// <summary>
    /// オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="item">リストから削除するオブジェクト</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_RemoveAll(IntPtr objectList, IntPtr item);

    /// <summary>
    /// オブジェクトリストの指定したインデックスにあるオブジェクトを削除する
    /// </summary>
    /// <param name="objectList">オブジェクトリストハンドル</param>
    /// <param name="index">削除するオブジェクトのインデックス番号</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNObjectList_RemoveAt(IntPtr objectList, int index);

    /// <summary>
    /// 整数型の多次元配列を作成する
    /// </summary>
    /// <param name="intTable">作成されたテーブルハンドルを格納する変数</param>
    /// <param name="xSize">X 要素数</param>
    /// <param name="ySize">Y 要素数</param>
    /// <param name="zSize">Z 要素数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_Create(out IntPtr intTable, int xSize, int ySize = 1, int zSize = 1);

    /// <summary>
    /// 整数型の多次元配列を作成する
    /// </summary>
    /// <param name="intTable">作成されたテーブルハンドルを格納する変数</param>
    /// <param name="xSize">X 要素数</param>
    /// <param name="ySize">Y 要素数</param>
    /// <param name="zSize">Z 要素数</param>
    /// <param name="srcData">XXXX</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_CreateFromSrcData(out IntPtr intTable, int xSize, int ySize, int zSize, int[] srcData);

    /// <summary>
    /// 整数型テーブルの指定したインデックスに値を設定する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="xSize">X 要素インデックス</param>
    /// <param name="ySize">Y 要素インデックス</param>
    /// <param name="zSize">Z 要素インデックス</param>
    /// <param name="value">設定する整数値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_SetValue(IntPtr intTable, int xSize, int ySize, int zSize, int value);

    /// <summary>
    /// 整数型テーブルの指定したインデックスに値を設定する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="xSize">X 要素インデックス</param>
    /// <param name="ySize">Y 要素インデックス</param>
    /// <param name="zSize">Z 要素インデックス</param>
    /// <param name="value">値を格納する整数型変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_GetValue(IntPtr intTable, int xSize, int ySize, int zSize, out int value);

    /// <summary>
    /// 整数型テーブルの要素数を変更する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="xSize">X 要素数</param>
    /// <param name="ySize">Y 要素数</param>
    /// <param name="zSize">Z 要素数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_Resize(IntPtr intTable, int xSize, int ySize, int zSize);

    /// <summary>
    /// 整数型テーブルの X 要素数を取得する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="xSize">X 要素数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_GetXSize(IntPtr intTable, out int xSize);

    /// <summary>
    /// 整数型テーブルの Y 要素数を取得する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="ySize">Y 要素数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_GetYSize(IntPtr intTable, out int ySize);

    /// <summary>
    /// 整数型テーブルの Z 要素数を取得する
    /// </summary>
    /// <param name="intTable">整数型テーブルハンドル</param>
    /// <param name="zSize">Z 要素数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNIntTable_GetZSize(IntPtr intTable, out int zSize);

    /// <summary>
    /// 2D ベクトルに値を設定する
    /// </summary>
    /// <param name="vec">値を格納する 2D ベクトル</param>
    /// <param name="x">X 値</param>
    /// <param name="y">Y 値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector2_Set(ref Vector2 vec, float x, float y);

    /// <summary>
    /// 2D ベクトルの長さを計算する
    /// </summary>
    /// <param name="vec">処理の基になる 2D ベクトル</param>
    /// <param name="outLength">ベクトルの長さを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector2_GetLength(ref Vector2 vec, out float outLength);

    /// <summary>
    /// 2D ベクトルの長さの2乗を計算する
    /// </summary>
    /// <param name="vec">処理の基になる 2D ベクトル</param>
    /// <param name="outLength">ベクトルの長さの2乗を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector2_GetSquareLength(ref Vector2 vec, out float outLength);

    /// <summary>
    /// 2D ベクトルを正規化する
    /// </summary>
    /// <param name="vec">処理の基になる 2D ベクトル</param>
    /// <param name="outVec">演算結果を格納する 2D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector2_Normalize(ref Vector2 vec, out Vector2 outVec);

    /// <summary>
    /// 3Dベクトルに値を設定する
    /// </summary>
    /// <param name="vec">値を格納するベクトル</param>
    /// <param name="x">X 値</param>
    /// <param name="y">Y 値</param>
    /// <param name="z">Z 値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Set(out Vector3 vec, float x, float y, float z);

    /// <summary>
    /// 3Dベクトルに値を設定する
    /// </summary>
    /// <param name="vec">値を格納するベクトル</param>
    /// <param name="vec2">2D ベクトル</param>
    /// <param name="z">Z 値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_SetVec2(out Vector3 vec, ref Vector2 vec2, float z);

    /// <summary>
    /// 3Dベクトルの長さを計算する
    /// </summary>
    /// <param name="vec">処理の基になるベクトル</param>
    /// <param name="outLength">ベクトルの長さを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_GetLength(ref Vector3 vec, out float outLength);

    /// <summary>
    /// 3D ベクトルの長さの2乗を計算する
    /// </summary>
    /// <param name="vec">処理の基になる Vector3</param>
    /// <param name="outLength">ベクトルの長さの2乗を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_GetSquareLength(ref Vector3 vec, out float outLength);

    /// <summary>
    /// 3D ベクトルを正規化する
    /// </summary>
    /// <param name="vec">処理の基になる 3D ベクトル</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Normalize(ref Vector3 vec, out Vector3 outVec);

    /// <summary>
    /// 2 つの 3D ベクトルの内積を計算する
    /// </summary>
    /// <param name="vec1">処理の基になる 3D ベクトル</param>
    /// <param name="vec2">処理の基になる 3D ベクトル</param>
    /// <param name="dot">ベクトルの内積を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Dot(ref Vector3 vec1, ref Vector3 vec2, out float dot);

    /// <summary>
    /// 2 つの 3D ベクトルの外積を計算する
    /// </summary>
    /// <param name="vec1">処理の基になる 3D ベクトル</param>
    /// <param name="vec2">処理の基になる 3D ベクトル</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Cross(ref Vector3 vec1, ref Vector3 vec2, out Vector3 outVec);

    /// <summary>
    /// 指定された法線で反射させたベクトルを計算する
    /// </summary>
    /// <param name="vec">処理の基になる 3D ベクトル</param>
    /// <param name="normal">面方向を表す法線 3D ベクトル</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Reflect(ref Vector3 vec, ref Vector3 normal, out Vector3 outVec);

    /// <summary>
    /// 指定された法線と方向から滑りベクトルを計算する
    /// </summary>
    /// <param name="vec">処理の基になる 3D ベクトル</param>
    /// <param name="normal">面方向を表す法線 3D ベクトル</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Slide(ref Vector3 vec, ref Vector3 normal, out Vector3 outVec);

    /// <summary>
    /// 2 つの 3D ベクトル間を線形補間する
    /// </summary>
    /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
    /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
    /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Lerp(ref Vector3 vec1, ref Vector3 vec2, float t, out Vector3 outVec);

    /// <summary>
    /// 指定された 3D ベクトルを Catmull-Rom 補間する
    /// </summary>
    /// <param name="vec1">処理の基になる 3D ベクトル</param>
    /// <param name="vec2">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
    /// <param name="vec3">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
    /// <param name="vec4">処理の基になる 3D ベクトル</param>
    /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_CatmullRom(ref Vector3 vec1, ref Vector3 vec2, ref Vector3 vec3, ref Vector3 vec4, float t, out Vector3 outVec);

    /// <summary>
    /// 指定された行列を使用して座標変換する
    /// </summary>
    /// <param name="vec">処理の基になる 3D ベクトル</param>
    /// <param name="mat">処理の基になる 行列</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_Transform(ref Vector3 vec, ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 指定された行列を使用して座標変換し、結果を w = 1 に射影する
    /// </summary>
    /// <param name="vec">処理の基になる 3D ベクトル</param>
    /// <param name="mat">処理の基になる 行列</param>
    /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_TransformCoord(ref Vector3 vec, ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 任意軸周りの回転を行う
    /// </summary>
    /// <param name="vec">処理の基になる Vector3</param>
    /// <param name="axis">回転軸を表す Vector3</param>
    /// <param name="r">回転角度 (ラジアン)</param>
    /// <param name="outVec">演算結果を格納する Vector3</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_RotateAxis(ref Vector3 vec, ref Vector3 axis, float r, out Vector3 outVec);

    /// <summary>
    /// 行列の回転成分だけを使って座標変換する
    /// </summary>
    /// <param name="vec">処理の基になる Vector3</param>
    /// <param name="mat">処理の基になる Matrix</param>
    /// <param name="outVec">演算結果を格納する Vector3</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVector3_RotateMatrix(ref Vector3 vec, ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 単位行列を作成する
    /// </summary>
    /// <param name="mat">結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Identity(out Matrix mat);

    /// <summary>
    /// 行列を平行移動する
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="x">X 方向の移動量</param>
    /// <param name="y">Y 方向の移動量</param>
    /// <param name="z">Z 方向の移動量</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Translate(ref Matrix mat, float x, float y, float z);

    /// <summary>
    /// 行列を平行移動する
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="vec">移動量</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_TranslateVec3(ref Matrix mat, ref Vector3 vec);

    /// <summary>
    /// X 軸を回転軸にして行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="radian">回転角度 ( 軸方向に対して反時計回り )</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateX(ref Matrix mat, float radian);

    /// <summary>
    /// Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="radian">回転角度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateY(ref Matrix mat, float radian);

    /// <summary>
    /// Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="radian">回転角度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateZ(ref Matrix mat, float radian);

    /// <summary>
    /// 行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="xRad">X 軸を回転軸とした角度</param>
    /// <param name="yRad">Y 軸を回転軸とした角度</param>
    /// <param name="zRad">Z 軸を回転軸とした角度</param>
    /// <param name="rotOrder">回転順序の指定</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Rotate(ref Matrix mat, float xRad, float yRad, float zRad, RotationOrder rotOrder = RotationOrder.XYZ);

    /// <summary>
    /// 行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="vec">各軸の回転角度</param>
    /// <param name="rotOrder">回転順序の指定</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateVec3(ref Matrix mat, ref Vector3 vec, RotationOrder rotOrder = RotationOrder.XYZ);

    /// <summary>
    /// 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="axis">回転軸を示す Vector3</param>
    /// <param name="radian">回転角度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateAxis(ref Matrix mat, ref Vector3 axis, float radian);

    /// <summary>
    /// クォータニオンを使って行列を回転する
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="qua">処理の基になる Quaternion</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_RotateQuaternion(ref Matrix mat, ref Quaternion qua);

    /// <summary>
    /// 行列をスケーリングする
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="xyz">X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Scale(ref Matrix mat, float xyz);

    /// <summary>
    /// 行列をスケーリングする
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="x">X 方向の拡大率</param>
    /// <param name="y">Y 方向の拡大率</param>
    /// <param name="z">Z 方向の拡大率</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_ScaleXYZ(ref Matrix mat, float x, float y, float z);

    /// <summary>
    /// 行列をスケーリングする
    /// </summary>
    /// <param name="mat">演算結果を格納する行列</param>
    /// <param name="scale">各方向の拡大率</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_ScaleVec3(ref Matrix mat, ref Vector3 scale);

    /// <summary>
    /// 行列を乗算する
    /// </summary>
    /// <param name="mat1">処理の基になる行列</param>
    /// <param name="mat2">処理の基になる行列</param>
    /// <param name="matOut">結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Multiply(ref Matrix mat1, ref Matrix mat2, out Matrix matOut);

    /// <summary>
    /// 逆行列を求める
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Inverse(ref Matrix mat, out Matrix matOut);

    /// <summary>
    /// 転置行列を求める
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Transpose(ref Matrix mat, out Matrix matOut);

    /// <summary>
    /// 左手座標系ビュー行列を作成する
    /// </summary>
    /// <param name="pos">視点の位置を示す Vector3</param>
    /// <param name="lookAt">注視点を示す Vector3</param>
    /// <param name="upDir">上方向を示す Vector3</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_ViewTransformLH(ref Vector3 pos, ref Vector3 lookAt, ref Vector3 upDir, out Matrix matOut);

    /// <summary>
    /// 右手座標系ビュー行列を作成する
    /// </summary>
    /// <param name="pos">視点の位置を示す Vector3</param>
    /// <param name="lookAt">注視点を示す Vector3</param>
    /// <param name="upDir">上方向を示す Vector3</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_ViewTransformRH(ref Vector3 pos, ref Vector3 lookAt, ref Vector3 upDir, out Matrix matOut);

    /// <summary>
    /// 左手座標系射影行列の作成
    /// </summary>
    /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
    /// <param name="aspect">アスペクト比</param>
    /// <param name="nearZ">近くのビュー平面の Z 値</param>
    /// <param name="farZ">遠くのビュー平面の Z 値</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_PerspectiveFovLH(float fovY, float aspect, float nearZ, float farZ, out Matrix matOut);

    /// <summary>
    /// 右手座標系射影行列の作成
    /// </summary>
    /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
    /// <param name="aspect">アスペクト比</param>
    /// <param name="nearZ">近くのビュー平面の Z 値</param>
    /// <param name="farZ">遠くのビュー平面の Z 値</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_PerspectiveFovRH(float fovY, float aspect, float nearZ, float farZ, out Matrix matOut);

    /// <summary>
    /// 左手座標系正射影行列の作成
    /// </summary>
    /// <param name="width">ビューの幅</param>
    /// <param name="height">ビューの高さ</param>
    /// <param name="nearZ">近くのビュー平面の Z 値</param>
    /// <param name="farZ">遠くのビュー平面の Z 値</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_OrthoLH(float width, float height, float nearZ, float farZ, out Matrix matOut);

    /// <summary>
    /// 右手座標系正射影行列の作成
    /// </summary>
    /// <param name="width">ビューの幅</param>
    /// <param name="height">ビューの高さ</param>
    /// <param name="nearZ">近くのビュー平面の Z 値</param>
    /// <param name="farZ">遠くのビュー平面の Z 値</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_OrthoRH(float width, float height, float nearZ, float farZ, out Matrix matOut);

    /// <summary>
    /// 右方向を示す 3D ベクトルの取得
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outVec">結果を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_GetRight(ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 上方向を示す 3D ベクトルの取得
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outVec">結果を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_GetUp(ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 正面方向を示す 3D ベクトルの取得
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outVec">結果を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_GetFront(ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 位置を示す 3D ベクトルの取得
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outVec">結果を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_GetPosition(ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 回転行列からオイラー角を計算する
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outVec">各軸の回転角度(ラジアン)を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_ToEuler(ref Matrix mat, out Vector3 outVec);

    /// <summary>
    /// 行列をスケーリング、回転、移動成分に分解する
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
    /// <param name="rot">各軸の回転角度(ラジアン)を格納する Vector3 変数</param>
    /// <param name="trans">各軸の平行移動量を格納する Vector3 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_Decompose(ref Matrix mat, out Vector3 scale, out Matrix rot, out Vector3 trans);

    /// <summary>
    /// 右手系⇔左手系の変換
    /// </summary>
    /// <param name="mat">処理の元になる行列</param>
    /// <param name="matOut">演算結果を格納する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNMatrix_TransformBasis(ref Matrix mat, out Matrix matOut);

    /// <summary>
    /// クォータニオンに値を設定する
    /// </summary>
    /// <param name="qua">値を格納する Quaternion 変数</param>
    /// <param name="x">X 値</param>
    /// <param name="y">Y 値</param>
    /// <param name="z">Z 値</param>
    /// <param name="w">W 値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Set(out Quaternion qua, float x, float y, float z, float w);

    /// <summary>
    /// 単位クォータニオンを作成する
    /// </summary>
    /// <param name="qua">結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Identity(out Quaternion qua);

    /// <summary>
    /// 任意の軸を回転軸にして回転するクォータニオンを作成する
    /// </summary>
    /// <param name="axis">回転軸を示す Vector3 変数</param>
    /// <param name="r">回転角度 (ラジアン)</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_RotationAxis(ref Vector3 axis, float r, out Quaternion outQua);

    /// <summary>
    /// 回転行列からクォータニオンを作成する
    /// </summary>
    /// <param name="mat">処理の基になる行列</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_RotationMatrix(ref Matrix mat, out Quaternion outQua);

    /// <summary>
    /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
    /// </summary>
    /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
    /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
    /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_RotationYawPitchRoll(float yaw, float pitch, float roll, out Quaternion outQua);

    /// <summary>
    /// クォータニオンを正規化する
    /// </summary>
    /// <param name="qua">処理の基になるクォータニオン</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Normalize(ref Quaternion qua, out Quaternion outQua);

    /// <summary>
    /// クォータニオンの共役を計算する
    /// </summary>
    /// <param name="qua">処理の基になるクォータニオン</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Conjugate(ref Quaternion qua, out Quaternion outQua);

    /// <summary>
    /// 2 つのクォータニオンの積を計算する
    /// </summary>
    /// <param name="qua1">処理の基になるクォータニオン</param>
    /// <param name="qua2">処理の基になるクォータニオン</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Multiply(ref Quaternion qua1, ref Quaternion qua2, out Quaternion outQua);

    /// <summary>
    /// 2 つのクォータニオンを球面線形補間する
    /// </summary>
    /// <param name="qua1">処理の基になるクォータニオン</param>
    /// <param name="qua2">処理の基になるクォータニオン</param>
    /// <param name="t">補間値</param>
    /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNQuaternion_Slerp(ref Quaternion qua1, ref Quaternion qua2, float t, out Quaternion outQua);

    /// <summary>
    /// 現在の時間を乱数シードとして、乱数オブジェクトを作成する
    /// </summary>
    /// <param name="random">作成された乱数オブジェクトハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_Create(out IntPtr random);

    /// <summary>
    /// 乱数シードを指定し、乱数オブジェクトを作成する
    /// </summary>
    /// <param name="random">作成された乱数オブジェクトハンドルを格納する変数</param>
    /// <param name="seed">シード値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_CreateSeed(out IntPtr random, int seed);

    /// <summary>
    /// 乱数オブジェクトに乱数シードを設定する
    /// </summary>
    /// <param name="random">乱数オブジェクトハンドル</param>
    /// <param name="seed">シード値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_SetSeed(IntPtr random, int seed);

    /// <summary>
    /// 整数値乱数の取得
    /// </summary>
    /// <param name="random">乱数オブジェクトハンドル</param>
    /// <param name="outValue">整数値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_GetInt(IntPtr random, out int outValue);

    /// <summary>
    /// 範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
    /// </summary>
    /// <param name="random">乱数オブジェクトハンドル</param>
    /// <param name="minValue">生成される乱数の下限値</param>
    /// <param name="maxValue">生成される乱数の上限値</param>
    /// <param name="outValue">整数値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_GetIntRange(IntPtr random, int minValue, int maxValue, out int outValue);

    /// <summary>
    /// 範囲を指定して、整数値乱数を取得する (center ± width/2)
    /// </summary>
    /// <param name="random">乱数オブジェクトハンドル</param>
    /// <param name="center">生成される乱数の中央値</param>
    /// <param name="width">生成される乱数の振れ幅</param>
    /// <param name="outValue">整数値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNRandom_GetIntDeflect(IntPtr random, int center, int width, out int outValue);

    /// <summary>
    /// ファイルを開く
    /// </summary>
    /// <param name="fileStream">作成されたファイルハンドルを格納する変数</param>
    /// <param name="filePath">ファイル名</param>
    /// <param name="mode">ファイルを開く方法または作成する方法</param>
    /// <param name="access">ファイルにアクセスする方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_Create(out IntPtr fileStream, string filePath, FileMode mode = FileMode.Open, FileAccess access = FileAccess.Read);

    /// <summary>
    /// ファイルを閉じる
    /// </summary>
    /// <param name="fileStream">ファイルハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_Close(IntPtr fileStream);

    /// <summary>
    /// ファイルサイズを取得する
    /// </summary>
    /// <param name="fileStream">ファイルハンドル</param>
    /// <param name="size">ファイルサイズ (バイト単位) を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_GetSize(IntPtr fileStream, out int size);

    /// <summary>
    /// ファイルポインタの位置を取得する
    /// </summary>
    /// <param name="fileStream">ファイルハンドル</param>
    /// <param name="position">ファイルポインタの位置 (バイト単位) を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_GetPosition(IntPtr fileStream, out int position);

    /// <summary>
    /// ファイルポインタの位置を設定する
    /// </summary>
    /// <param name="fileStream">ファイルハンドル</param>
    /// <param name="offset">読み込んだデータを格納するバッファ</param>
    /// <param name="origin">起点</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_Seek(IntPtr fileStream, int offset, SeekOrigin origin);

    /// <summary>
    /// ファイルからデータを読み込む
    /// </summary>
    /// <param name="fileStream">ファイルハンドル</param>
    /// <param name="buffer">読み込んだデータを格納するバッファ</param>
    /// <param name="readSize">読み込むサイズ (バイト数)</param>
    /// <param name="validSize">読み込んだサイズ (バイト数) を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_Read(IntPtr fileStream, ref byte[] buffer, int readSize, out int validSize);

    /// <summary>
    /// ファイルの存在を調べる
    /// </summary>
    /// <param name="filePath">ファイルパス</param>
    /// <param name="result">結果を格納する変数 (LN_TRUE = 存在する / LN_FALSE = 存在しない)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFileStream_Exists(string filePath, out bool result);

    /// <summary>
    /// ボタンが現在押されているかを判定する
    /// </summary>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_IsPress(int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが新しく押されたかを判定する
    /// </summary>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_IsTrigger(int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが離されたかを判定する
    /// </summary>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_IsOffTrigger(int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
    /// </summary>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_IsRepeat(int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
    /// </summary>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">アナログ値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_GetPower(int button, out float state);

    /// <summary>
    /// 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
    /// </summary>
    /// <param name="axis">軸番号 (0～1)</param>
    /// <param name="state">軸の状態を格納する2Dベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNInput_GetAxisState(int axis, out Vector2 state);

    /// <summary>
    /// 仮想ゲームコントローラハンドルを取得する
    /// </summary>
    /// <param name="index">取得する仮想ゲームコントローラの番号 (0～3)</param>
    /// <param name="virtualPad">仮想ゲームコントローラハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_GetVirtualPad(int index, out IntPtr virtualPad);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが現在押されているかを判定する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_IsPress(IntPtr virtualPad, int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが新しく押されたかを判定する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_IsTrigger(IntPtr virtualPad, int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが離されたかを判定する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_IsOffTrigger(IntPtr virtualPad, int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_IsRepeat(IntPtr virtualPad, int button, out bool state);

    /// <summary>
    /// 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="button">仮想ボタン番号 (0～19)</param>
    /// <param name="state">アナログ値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_GetPower(IntPtr virtualPad, int button, out float state);

    /// <summary>
    /// 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="axis">軸番号 (0～1)</param>
    /// <param name="state">軸の状態を格納する2Dベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_GetAxisState(IntPtr virtualPad, int axis, out Vector2 state);

    /// <summary>
    /// 仮想ゲームコントローラの新しく押されたデバイスボタン番号の取得
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="deviceButton">デバイスボタン番号を格納する構造体</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_GetPressedDeviceButton(IntPtr virtualPad, out int deviceButton);

    /// <summary>
    /// デバイスボタン番号に対応するデバイスの種類を取得する
    /// </summary>
    /// <param name="deviceButton">デバイスボタン番号</param>
    /// <param name="type">デバイスボタンの種類を格納する構造体</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_GetDeviceButtonType(int deviceButton, out DeviceButtonType type);

    /// <summary>
    /// 仮想ゲームコントローラのボタンにデバイスボタンを割り当てる
    /// </summary>
    /// <param name="virtualPad">ゲームコントローラハンドル</param>
    /// <param name="virtualButton">割り当て先の仮想ボタン番号</param>
    /// <param name="deviceButton">割り当てるデバイスボタン番号</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_AttachVirtualButton(IntPtr virtualPad, int virtualButton, int deviceButton);

    /// <summary>
    /// 仮想ゲームコントローラのデバイスボタンの割り当てを解除する
    /// </summary>
    /// <param name="virtualPad">仮想ゲームコントローラハンドル</param>
    /// <param name="virtualButton">解除対象の仮想ボタン番号</param>
    /// <param name="deviceButton">解除するデバイスボタン番号</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVirtualPad_DetachVirtualButton(IntPtr virtualPad, int virtualButton, int deviceButton);

    /// <summary>
    /// BGM を演奏する
    /// </summary>
    /// <param name="filePath">ファイル名</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayBGM(string filePath, int volume = 100, int pitch = 100, int fadeTime = 0);

    /// <summary>
    /// メモリ上の音声データから BGM を演奏する
    /// </summary>
    /// <param name="data">メモリ上の音声ファイルデータ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayBGMMem(byte[] data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0);

    /// <summary>
    /// BGM の演奏を停止する
    /// </summary>
    /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_StopBGM(int fadeTime = 0);

    /// <summary>
    /// BGS を演奏する
    /// </summary>
    /// <param name="filePath">ファイル名</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayBGS(string filePath, int volume = 100, int pitch = 100, int fadeTime = 0);

    /// <summary>
    /// メモリ上の音声データから BGS を演奏する
    /// </summary>
    /// <param name="data">メモリ上の音声ファイルデータ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayBGSMem(byte[] data, int dataSize, int volume = 100, int pitch = 100, int fadeTime = 0);

    /// <summary>
    /// BGS の演奏を停止する
    /// </summary>
    /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_StopBGS(int fadeTime = 0);

    /// <summary>
    /// ME を演奏する
    /// </summary>
    /// <param name="filePath">ファイル名</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayME(string filePath, int volume = 100, int pitch = 100);

    /// <summary>
    /// メモリ上の音声データから ME を演奏する
    /// </summary>
    /// <param name="data">メモリ上の音声ファイルデータ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlayMEMem(byte[] data, int dataSize, int volume = 100, int pitch = 100);

    /// <summary>
    /// ME の演奏を停止する
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNAudio_StopME();

    /// <summary>
    /// SE を演奏する
    /// </summary>
    /// <param name="filePath">ファイル名</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySE(string filePath, int volume = 100, int pitch = 100);

    /// <summary>
    /// SE を演奏する (3D サウンド)
    /// </summary>
    /// <param name="filePath">ファイル名</param>
    /// <param name="position">3D 空間上の座標</param>
    /// <param name="distance">減衰距離</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySE3D(string filePath, ref Vector3 position, float distance, int volume = 100, int pitch = 100);

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
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySE3DXYZ(string filePath, float x, float y, float z, float distance, int volume = 100, int pitch = 100);

    /// <summary>
    /// メモリ上の音声データから SE を演奏する
    /// </summary>
    /// <param name="data">メモリ上の音声ファイルデータ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySEMem(byte[] data, int dataSize, int volume = 100, int pitch = 100);

    /// <summary>
    /// メモリ上の音声データから SE を演奏する (3D サウンド)
    /// </summary>
    /// <param name="data">メモリ上の音声ファイルデータ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="position">3D 空間上の座標</param>
    /// <param name="distance">減衰距離</param>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="pitch">ピッチ (50 ～ 200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySE3DMem(byte[] data, int dataSize, ref Vector3 position, float distance, int volume = 100, int pitch = 100);

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
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_PlaySE3DMemXYZ(byte[] data, int dataSize, float x, float y, float z, float distance, int volume = 100, int pitch = 100);

    /// <summary>
    /// すべての SE の演奏を停止する
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNAudio_StopSE();

    /// <summary>
    /// 3D 空間の1メートル相当の距離の設定
    /// </summary>
    /// <param name="distance">距離</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_SetMetreUnitDistance(float distance);

    /// <summary>
    /// 再生中のBGMの音量を設定する (フェードアウト中は無効)
    /// </summary>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_SetBGMVolume(int volume, int fadeTime = 0);

    /// <summary>
    /// 再生中のBGSの音量を設定する (フェードアウト中は無効)
    /// </summary>
    /// <param name="volume">ボリューム (0 ～ 100)</param>
    /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNAudio_SetBGSVolume(int volume, int fadeTime = 0);

    /// <summary>
    /// [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
    /// </summary>
    /// <param name="type">取得するサウンドの種類</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static IntPtr LNAudio_GetInternalGameSound(InternalGameSound type);

    /// <summary>
    /// リスナーの位置の設定
    /// </summary>
    /// <param name="position">3D 空間上の座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSoundListener_SetPosition(ref Vector3 position);

    /// <summary>
    /// リスナーの位置の設定
    /// </summary>
    /// <param name="x">3D 空間上の X 座標</param>
    /// <param name="y">3D 空間上の Y 座標</param>
    /// <param name="z">3D 空間上の Z 座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNSoundListener_SetPositionXYZ(float x, float y, float z);

    /// <summary>
    /// リスナーの正面方向の設定
    /// </summary>
    /// <param name="direction">向き</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSoundListener_SetDirection(ref Vector3 direction);

    /// <summary>
    /// リスナーの正面方向の設定
    /// </summary>
    /// <param name="x">向きの X 成分</param>
    /// <param name="y">向きの Y 成分</param>
    /// <param name="z">向きの Z 成分</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNSoundListener_SetDirectionXYZ(float x, float y, float z);

    /// <summary>
    /// リスナーの上方向の設定 (正面方向とは直交であること)
    /// </summary>
    /// <param name="direction">上方向</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSoundListener_SetUpDirection(ref Vector3 direction);

    /// <summary>
    /// リスナーの上方向の設定 (正面方向とは直交であること)
    /// </summary>
    /// <param name="x">向きの X 成分</param>
    /// <param name="y">向きの Y 成分</param>
    /// <param name="z">向きの Z 成分</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNSoundListener_SetUpDirectionXYZ(float x, float y, float z);

    /// <summary>
    /// リスナーの速度の設定
    /// </summary>
    /// <param name="velocity">速度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSoundListener_Velocity(ref Vector3 velocity);

    /// <summary>
    /// リスナーの速度の設定
    /// </summary>
    /// <param name="x">速度の X 成分</param>
    /// <param name="y">速度の Y 成分</param>
    /// <param name="z">速度の Z 成分</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNSoundListener_VelocityXYZ(float x, float y, float z);

    /// <summary>
    /// ファイルから音声を作成する
    /// </summary>
    /// <param name="sound">sound</param>
    /// <param name="filePath">ファイル名</param>
    /// <param name="enable3d">true の場合、3D 音源として作成する</param>
    /// <param name="playType">音声の再生方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_Create(out IntPtr sound, string filePath, bool enable3d = false, SoundPlayingType playType = SoundPlayingType.Auto);

    /// <summary>
    /// メモリ上の音声ファイルデータから音声を作成する
    /// </summary>
    /// <param name="sound">sound</param>
    /// <param name="data">メモリ上の音声データへのポインタ</param>
    /// <param name="dataSize">データサイズ (バイト単位)</param>
    /// <param name="enable3d">true の場合、3D 音源として作成する</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_CreateMem(out IntPtr sound, byte[] data, int dataSize, bool enable3d = false);

    /// <summary>
    /// ボリュームの設定
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="volume">ボリューム (0～100)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetVolume(IntPtr sound, int volume);

    /// <summary>
    /// ボリュームの取得
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="volume">ボリュームを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetVolume(IntPtr sound, out int volume);

    /// <summary>
    /// ピッチの設定
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="pitch">ピッチ (50～200)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetPitch(IntPtr sound, int pitch);

    /// <summary>
    /// ピッチの取得
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="pitch">ピッチを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetPitch(IntPtr sound, out int pitch);

    /// <summary>
    /// ループ再生の有効設定
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="loopEnable">LN_TRUE = ループ再生する / LN_FALSE = しない</param>
    /// <param name="begin">ループ領域の先頭位置 (サンプル数単位)</param>
    /// <param name="length">ループ領域長さ (サンプル数単位)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetLoopState(IntPtr sound, bool loopEnable, int begin = 0, int length = 0);

    /// <summary>
    /// ループ再生が有効かを調べる
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="enabled">ループ再生の有無状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_IsLoop(IntPtr sound, out bool enabled);

    /// <summary>
    /// サウンドの再生状態を取得する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="state">状態を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetPlayState(IntPtr sound, out SoundPlayingState state);

    /// <summary>
    /// 音声を再生する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_Play(IntPtr sound);

    /// <summary>
    /// 再生を停止する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_Stop(IntPtr sound);

    /// <summary>
    /// 一時停止
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="pause">LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_Pause(IntPtr sound, bool pause);

    /// <summary>
    /// 音量のフェード
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="targetVolume">変更先の音量</param>
    /// <param name="time">フェードにかける時間 (ミリ秒)</param>
    /// <param name="fadeState">フェード完了後の動作の指定</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_FadeVolume(IntPtr sound, int targetVolume, int time, SoundFadeState fadeState);

    /// <summary>
    /// 再生したサンプル数の取得
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="samples">再生したサンプル数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetUnitsPlayed(IntPtr sound, out int samples);

    /// <summary>
    /// 音声データ全体のサンプル数の取得
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="samples">音声データ全体のサンプル数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetTotalUnits(IntPtr sound, out int samples);

    /// <summary>
    /// サンプリング周波数の取得
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="frequency">サンプリング周波数を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_GetUnitsParSecond(IntPtr sound, out int frequency);

    /// <summary>
    /// 3D 音源かを調べる
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="enabled">3D 音源かを示す値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_Is3DSound(IntPtr sound, out bool enabled);

    /// <summary>
    /// 3D 音源としての位置を設定する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="position">3D 空間上の座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetEmitterPosition(IntPtr sound, ref Vector3 position);

    /// <summary>
    /// 3D 音源としての位置を設定する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="x">3D 空間上の X 座標</param>
    /// <param name="y">3D 空間上の Y 座標</param>
    /// <param name="z">3D 空間上の Z 座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetEmitterPositionXYZ(IntPtr sound, float x, float y, float z);

    /// <summary>
    /// 3D 音源としての速度を設定する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="velocity">速度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetEmitterVelocity(IntPtr sound, ref Vector3 velocity);

    /// <summary>
    /// 3D 音源としての速度を設定する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="x">速度の X 成分</param>
    /// <param name="y">速度の Y 成分</param>
    /// <param name="z">速度の Z 成分</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetEmitterVelocityXYZ(IntPtr sound, float x, float y, float z);

    /// <summary>
    /// 3D 音源の減衰距離 (聴こえなくなる距離) を設定する
    /// </summary>
    /// <param name="sound">サウンドハンドル</param>
    /// <param name="distance">距離</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSound_SetEmitterDistance(IntPtr sound, float distance);

    /// <summary>
    /// テクスチャの作成
    /// </summary>
    /// <param name="texture">作成されたテクスチャハンドルを格納する変数</param>
    /// <param name="width">幅 (ピクセル数)</param>
    /// <param name="height">高さ (ピクセル数)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_Create(out IntPtr texture, int width, int height);

    /// <summary>
    /// テクスチャの作成
    /// </summary>
    /// <param name="texture">作成されたテクスチャハンドルを格納する変数</param>
    /// <param name="filePath">テクスチャファイル名</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_CreateFromFile(out IntPtr texture, string filePath);

    /// <summary>
    /// テクスチャの作成
    /// </summary>
    /// <param name="texture">作成されたテクスチャハンドルを格納する変数</param>
    /// <param name="width">幅 (ピクセル数)</param>
    /// <param name="height">高さ (ピクセル数)</param>
    /// <param name="levels">ミップマップレベル (0で最大レベルまで作成)</param>
    /// <param name="format">サーフェイスフォーマット</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_CreateEx(out IntPtr texture, int width, int height, int levels, TextureFormat format);

    /// <summary>
    /// テクスチャの作成
    /// </summary>
    /// <param name="texture">作成されたテクスチャハンドルを格納する変数</param>
    /// <param name="filePath">テクスチャファイル名</param>
    /// <param name="colorKey">透明色として扱う色</param>
    /// <param name="levels">ミップマップレベル (0で最大レベルまで作成)</param>
    /// <param name="format">サーフェイスフォーマット</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_CreateFromFileEx(out IntPtr texture, string filePath, uint colorKey, int levels, TextureFormat format);

    /// <summary>
    /// テクスチャの幅の取得 (ピクセル単位)
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="width">テクスチャの幅を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_GetWidth(IntPtr texture, out int width);

    /// <summary>
    /// テクスチャの高さの取得 (ピクセル単位)
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="height">テクスチャの高さを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_GetHeight(IntPtr texture, out int height);

    /// <summary>
    /// テクスチャの実際の幅の取得 (ピクセル単位)
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="width">テクスチャの実際の幅を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_GetRealWidth(IntPtr texture, out int width);

    /// <summary>
    /// テクスチャの実際の高さの取得 (ピクセル単位)
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="height">テクスチャの実際の高さを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_GetRealHeight(IntPtr texture, out int height);

    /// <summary>
    /// テクスチャをクリアする
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="color">塗りつぶす色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_Clear(IntPtr texture, ref Color color);

    /// <summary>
    /// テクスチャをクリアする
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="r">塗りつぶす色の R 値</param>
    /// <param name="g">塗りつぶす色の G 値</param>
    /// <param name="b">塗りつぶす色の B 値</param>
    /// <param name="a">塗りつぶす色の A 値</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_ClearRGBA(IntPtr texture, int r, int g, int b, int a);

    /// <summary>
    /// テクスチャへ文字列を描画する
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="text">描画する文字列</param>
    /// <param name="rect">描画領域 (はみだした場合にクリッピングされます)</param>
    /// <param name="font">描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)</param>
    /// <param name="length">描画文字数 (-1 の場合、文字列全体を描画する)</param>
    /// <param name="align">描画基準位置の指定</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_DrawText(IntPtr texture, string text, ref Rect rect, IntPtr font = default(IntPtr), int length = -1, TextAlign align = TextAlign.Left);

    /// <summary>
    /// テクスチャへ文字列を描画する
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="text">描画する文字列</param>
    /// <param name="x">描画先の左上 X 座標</param>
    /// <param name="y">描画先の左上 Y 座標</param>
    /// <param name="font">描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)</param>
    /// <param name="length">描画文字数 (-1 の場合、文字列全体を描画する)</param>
    /// <param name="align">描画基準位置の指定</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_DrawTextXY(IntPtr texture, string text, int x, int y, IntPtr font = default(IntPtr), int length = -1, TextAlign align = TextAlign.Left);

    /// <summary>
    /// テクスチャのブロック転送
    /// </summary>
    /// <param name="texture">テクスチャハンドル</param>
    /// <param name="x">転送先 X 座標</param>
    /// <param name="y">転送先 Y 座標</param>
    /// <param name="srcTexture">転送元テクスチャハンドル</param>
    /// <param name="srcRect">転送元領域</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTexture_BitBlt(IntPtr texture, int x, int y, IntPtr srcTexture, ref Rect srcRect);

    /// <summary>
    /// フォントの作成
    /// </summary>
    /// <param name="font">作成されたフォントハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_Create(out IntPtr font);

    /// <summary>
    /// ビットマップフォントの作成
    /// </summary>
    /// <param name="font">作成されたフォントハンドルを格納する変数</param>
    /// <param name="filePath">画像ファイル名</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_CreateBitmapFontFromFile(out IntPtr font, string filePath);

    /// <summary>
    /// フォントのコピーの作成
    /// </summary>
    /// <param name="font">作成されたフォントハンドルを格納する変数</param>
    /// <param name="srcFont">コピー元のフォントハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_CreateCopy(out IntPtr font, IntPtr srcFont);

    /// <summary>
    /// 指定フォントをデフォルトフォントに設定する
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetDefaultFont(IntPtr font);

    /// <summary>
    /// フォント名の設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="fontName">フォント名</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetName(IntPtr font, string fontName);

    /// <summary>
    /// フォント名の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="fontName">フォント名を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetName(IntPtr font, out IntPtr fontName);

    /// <summary>
    /// フォントサイズの設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="size">設定するサイズ</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetSize(IntPtr font, int size);

    /// <summary>
    /// フォントサイズの取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="size">フォントサイズを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetSize(IntPtr font, out int size);

    /// <summary>
    /// フォントカラーの設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="color">設定する色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetColor(IntPtr font, ref Color color);

    /// <summary>
    /// フォントカラーの取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="color">色を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetColor(IntPtr font, out Color color);

    /// <summary>
    /// エッジカラーの設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="color">設定する色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetEdgeColor(IntPtr font, ref Color color);

    /// <summary>
    /// エッジカラーの取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="color">色を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetEdgeColor(IntPtr font, out Color color);

    /// <summary>
    /// エッジの幅の設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="size">エッジの幅 (0 でエッジ無効)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetEdgeSize(IntPtr font, int size);

    /// <summary>
    /// エッジの幅の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="size">エッジの幅を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetEdgeSize(IntPtr font, out int size);

    /// <summary>
    /// 太文字有無の設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">太文字の有無</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetBold(IntPtr font, bool enabled);

    /// <summary>
    /// 太文字有無の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">太文字の有無を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_IsBold(IntPtr font, out bool enabled);

    /// <summary>
    /// イタリック体有無の設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">イタリック体の有無</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetItalic(IntPtr font, bool enabled);

    /// <summary>
    /// イタリック体有無の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">イタリック体の有無を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_IsItalic(IntPtr font, out bool enabled);

    /// <summary>
    /// アンチエイリアス有無の設定
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">アンチエイリアスの有無</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_SetAntiAlias(IntPtr font, bool enabled);

    /// <summary>
    /// アンチエイリアス有無の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="enabled">アンチエイリアス有無を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_IsAntiAlias(IntPtr font, out bool enabled);

    /// <summary>
    /// 文字列を描画したときのサイズ (ピクセル単位) の取得
    /// </summary>
    /// <param name="font">フォントハンドル</param>
    /// <param name="text">サイズ計算に使用する文字列</param>
    /// <param name="length">文字数 (-1 で文字列全体)</param>
    /// <param name="outSize">結果を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNFont_GetTextSize(IntPtr font, string text, int length, out Size outSize);

    /// <summary>
    /// ノード名の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="outName">ノードの名前を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetName(IntPtr sceneNode, out IntPtr outName);

    /// <summary>
    /// ノード名の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="name">新しい名前</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetName(IntPtr sceneNode, string name);

    /// <summary>
    /// 座標の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetPosition(IntPtr sceneNode, ref Vector3 vec);

    /// <summary>
    /// 座標の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 座標</param>
    /// <param name="y">Y 座標</param>
    /// <param name="z">Z 座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetPositionXYZ(IntPtr sceneNode, float x, float y, float z);

    /// <summary>
    /// 座標の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">座標を格納するベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetPosition(IntPtr sceneNode, out Vector3 vec);

    /// <summary>
    /// 座標の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 座標を格納する変数</param>
    /// <param name="y">Y 座標を格納する変数</param>
    /// <param name="z">Z 座標を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetPositionXYZ(IntPtr sceneNode, out float x, out float y, out float z);

    /// <summary>
    /// 回転角度の設定 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">各軸の回転角度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetAngle(IntPtr sceneNode, ref Vector3 vec);

    /// <summary>
    /// 回転角度の設定 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 軸の回転角度</param>
    /// <param name="y">Y 軸の回転角度</param>
    /// <param name="z">Z 軸の回転角度</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetAngleXYZ(IntPtr sceneNode, float x, float y, float z);

    /// <summary>
    /// 回転角度の取得 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">回転角度を格納するベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetAngle(IntPtr sceneNode, out Vector3 vec);

    /// <summary>
    /// 回転角度の取得 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 軸の回転角度を格納する変数</param>
    /// <param name="y">Y 軸の回転角度を格納する変数</param>
    /// <param name="z">Z 軸の回転角度を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetAngleXYZ(IntPtr sceneNode, out float x, out float y, out float z);

    /// <summary>
    /// 拡大率の設定 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">拡大率</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetScale(IntPtr sceneNode, ref Vector3 vec);

    /// <summary>
    /// 拡大率の設定 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 方向の拡大率</param>
    /// <param name="y">Y 方向の拡大率</param>
    /// <param name="z">Z 方向の拡大率</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetScaleXYZ(IntPtr sceneNode, float x, float y, float z);

    /// <summary>
    /// 拡大率の取得 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">拡大率を格納するベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetScale(IntPtr sceneNode, out Vector3 vec);

    /// <summary>
    /// 拡大率の取得 (ラジアン単位)
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 方向の拡大率を格納する変数</param>
    /// <param name="y">Y 方向の拡大率を格納する変数</param>
    /// <param name="z">Z 方向の拡大率を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetScaleXYZ(IntPtr sceneNode, out float x, out float y, out float z);

    /// <summary>
    /// 移動・回転・拡大の原点の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">各軸のオフセット</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetCenter(IntPtr sceneNode, ref Vector3 vec);

    /// <summary>
    /// 移動・回転・拡大の原点の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 方向のオフセット</param>
    /// <param name="y">Y 方向のオフセット</param>
    /// <param name="z">Z 方向のオフセット</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetCenterXYZ(IntPtr sceneNode, float x, float y, float z);

    /// <summary>
    /// 移動・回転・拡大の原点の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="vec">各軸のオフセットを格納するベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetCenter(IntPtr sceneNode, out Vector3 vec);

    /// <summary>
    /// 移動・回転・拡大の原点の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="x">X 方向のオフセットを格納する変数</param>
    /// <param name="y">Y 方向のオフセットを格納する変数</param>
    /// <param name="z">Z 方向のオフセットを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetCenterXYZ(IntPtr sceneNode, out float x, out float y, out float z);

    /// <summary>
    /// 座標変換行列の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="mat">座標変換行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetMatrix(IntPtr sceneNode, ref Matrix mat);

    /// <summary>
    /// 座標変換行列の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="mat">座標変換行列を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetMatrix(IntPtr sceneNode, out Matrix mat);

    /// <summary>
    /// 回転順序の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="rotOrder">回転順序</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetRotateOrder(IntPtr sceneNode, RotationOrder rotOrder);

    /// <summary>
    /// 回転順序の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="outOrder">回転順序を示す値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetRotateOrder(IntPtr sceneNode, out RotationOrder outOrder);

    /// <summary>
    /// 配置方法の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="type">配置方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetPlacementType(IntPtr sceneNode, PlacementType type);

    /// <summary>
    /// 配置方法の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="outType">配置方法を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_GetPlacementType(IntPtr sceneNode, out PlacementType outType);

    /// <summary>
    /// 子ノードを追加する
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="childSceneNode">子ノードとして追加するノード</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_AddChild(IntPtr sceneNode, IntPtr childSceneNode);

    /// <summary>
    /// 子ノードを取り除く
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="childSceneNode">取り外す子ノード</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_RemoveChild(IntPtr sceneNode, IntPtr childSceneNode);

    /// <summary>
    /// フレーム更新
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="elapsedTime">経過させる時間</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_Update(IntPtr sceneNode, float elapsedTime);

    /// <summary>
    /// 自動更新有無の設定
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="enabled">LN_TRUE=有効 / LN_FALSE=無効</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_SetEnableAutoUpdate(IntPtr sceneNode, bool enabled);

    /// <summary>
    /// 自動更新有無の取得
    /// </summary>
    /// <param name="sceneNode">シーングラフノードハンドル</param>
    /// <param name="outEnabled">自動更新の有無を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSceneNode_IsEnableAutoUpdate(IntPtr sceneNode, out bool outEnabled);

    /// <summary>
    /// 可視状態の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="enabled">LN_TRUE=表示する / LN_FALSE=表示しない</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetVisible(IntPtr visualNode, bool enabled);

    /// <summary>
    /// 不透明度の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="opacity">不透明度 (0.0～1.0)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetOpacity(IntPtr visualNode, float opacity);

    /// <summary>
    /// 乗算色の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="materialIndex">設定先のマテリアル番号</param>
    /// <param name="color">設定する色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetColorScale(IntPtr visualNode, int materialIndex, ref ColorF color);

    /// <summary>
    /// ブレンドカラーの設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="materialIndex">設定先のマテリアル番号</param>
    /// <param name="color">設定する色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetBlendColor(IntPtr visualNode, int materialIndex, ref ColorF color);

    /// <summary>
    /// 色調の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="materialIndex">設定先のマテリアル番号</param>
    /// <param name="tone">設定する色調</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetTone(IntPtr visualNode, int materialIndex, ref Tone tone);

    /// <summary>
    /// UV 変換行列の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="materialIndex">設定先のマテリアル番号</param>
    /// <param name="matrix">設定する行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetUVTransform(IntPtr visualNode, int materialIndex, ref Matrix matrix);

    /// <summary>
    /// 合成方法の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="mode">合成方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetBlendMode(IntPtr visualNode, BlendMode mode);

    /// <summary>
    /// カリング方法の設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="mode">カリング方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetCullingMode(IntPtr visualNode, CullingMode mode);

    /// <summary>
    /// 深度テストの有効設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="enabled">深度テストの有無</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetEnableDepthTest(IntPtr visualNode, bool enabled);

    /// <summary>
    /// 深度書き込みの有効設定
    /// </summary>
    /// <param name="visualNode">ビジュアルノードハンドル</param>
    /// <param name="enabled">深度書き込みの有無</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetEnableDepthWrite(IntPtr visualNode, bool enabled);

    /// <summary>
    /// シェーダの設定
    /// </summary>
    /// <param name="visualNode">シーングラフノードハンドル</param>
    /// <param name="shader">シェーダハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNVisualNode_SetShader(IntPtr visualNode, IntPtr shader);

    /// <summary>
    /// スプライトオブジェクトの作成 (2D空間へ配置する)
    /// </summary>
    /// <param name="sprite">作成されたスプライトハンドルを格納する変数</param>
    /// <param name="texture">スプライトが表示するテクスチャのハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_Create(out IntPtr sprite, IntPtr texture);

    /// <summary>
    /// スプライトが表示するテクスチャの設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="texture">テクスチャハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetTexture(IntPtr sprite, IntPtr texture);

    /// <summary>
    /// スプライトのサイズの設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="width">設定する幅</param>
    /// <param name="height">設定する高さ</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetSize(IntPtr sprite, float width, float height);

    /// <summary>
    /// テクスチャの転送矩形の設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="rect">転送元矩形</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetSrcRect(IntPtr sprite, ref Rect rect);

    /// <summary>
    /// テクスチャの転送矩形の設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="x">転送元の左上の X 座標</param>
    /// <param name="y">転送元の左上の Y 座標</param>
    /// <param name="w">転送元の幅</param>
    /// <param name="h">転送元の高さ</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetSrcRectXYWH(IntPtr sprite, int x, int y, int w, int h);

    /// <summary>
    /// テクスチャの転送矩形の取得
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="rect">矩形を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_GetSrcRect(IntPtr sprite, out Rect rect);

    /// <summary>
    /// テクスチャの転送矩形の取得
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="x">転送元の左上の X 座標 を格納する変数</param>
    /// <param name="y">転送元の左上の Y 座標 を格納する変数</param>
    /// <param name="w">転送元の幅 を格納する変数</param>
    /// <param name="h">転送元の高さ を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_GetSrcRectXYWH(IntPtr sprite, out int x, out int y, out int w, out int h);

    /// <summary>
    /// 左右反転有無の設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="enabled">LN_TRUE=反転する / LN_FALSE=反転しない</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetFlipH(IntPtr sprite, bool enabled);

    /// <summary>
    /// 左右反転有無の取得
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="outEnabled">左右反転の有無を示す値を格納する変数 (LN_TRUE=反転する / LN_FALSE=反転しない)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_IsFlipH(IntPtr sprite, out bool outEnabled);

    /// <summary>
    /// 上下反転有無の設定
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="enabled">LN_TRUE=反転する / LN_FALSE=反転しない</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_SetFlipV(IntPtr sprite, bool enabled);

    /// <summary>
    /// 上下反転有無の取得
    /// </summary>
    /// <param name="sprite">スプライトハンドル</param>
    /// <param name="outEnabled">左右反転の有無を示す値を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSprite_IsFlipV(IntPtr sprite, out bool outEnabled);

    /// <summary>
    /// スプライトパスオブジェクトの作成
    /// </summary>
    /// <param name="spritePath">作成されたスプライトパスハンドルを格納する変数</param>
    /// <param name="maxSpriteCount">このパスが一度の画面更新で描画できるスプライトの数</param>
    /// <param name="coord">描画先の座標空間</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSpritePath_Create(out IntPtr spritePath, int maxSpriteCount, Coord coord = Coord.Space2D);

    /// <summary>
    /// 以降のスプライトの描画で使用するブレンドモードの設定
    /// </summary>
    /// <param name="spritePath">スプライトパスハンドル</param>
    /// <param name="blendMode">合成方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSpritePath_SetPathBlendMode(IntPtr spritePath, BlendMode blendMode);

    /// <summary>
    /// 以降のスプライトの描画で使用する座標変換行列の設定
    /// </summary>
    /// <param name="spritePath">スプライトパスハンドル</param>
    /// <param name="matrix">座標変換行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSpritePath_SetPathTransform(IntPtr spritePath, ref Matrix matrix);

    /// <summary>
    /// スプライトの描画
    /// </summary>
    /// <param name="spritePath">スプライトパスハンドル</param>
    /// <param name="position">描画先座標</param>
    /// <param name="center">位置や回転の中心となるオフセット</param>
    /// <param name="size">描画先スプライトのサイズ</param>
    /// <param name="texture">描画するテクスチャハンドル</param>
    /// <param name="srcRect">テクスチャからの転送矩形</param>
    /// <param name="vertexColors">四角形の各頂点の乗算色 (要素数 4 の配列。NULL の場合は考慮しない)</param>
    /// <param name="frontAxis">スプライトの正面方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNSpritePath_DrawSprite(IntPtr spritePath, ref Vector3 position, ref Vector3 center, ref Vector2 size, IntPtr texture, ref Rect srcRect, ref ColorF vertexColors, FrontAxis frontAxis = FrontAxis.Rz);

    /// <summary>
    /// モデルオブジェクトの作成
    /// </summary>
    /// <param name="model">作成されたモデルハンドルを格納する変数</param>
    /// <param name="filePath">メッシュファイル名</param>
    /// <param name="meshCreateFlags"></param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNModel_Create(out IntPtr model, string filePath, MeshCreateFlags meshCreateFlags);

    /// <summary>
    /// エフェクトオブジェクトの作成 (3D空間へ配置する)
    /// </summary>
    /// <param name="effect">作成されたエフェクトハンドルを格納する変数</param>
    /// <param name="filePath">エフェクトファイル名 (Effekseer 形式)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNEffect_Create(out IntPtr effect, string filePath);

    /// <summary>
    /// エフェクトの再生を開始する
    /// </summary>
    /// <param name="effect">エフェクトハンドル</param>
    /// <param name="overlap">LN_TRUE = 再生中のエフェクトを停止しない / LN_FALSE = 再生中のエフェクトを停止する</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNEffect_Play(IntPtr effect, bool overlap);

    /// <summary>
    /// エフェクトを停止する
    /// </summary>
    /// <param name="effect">エフェクトハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNEffect_Stop(IntPtr effect);

    /// <summary>
    /// エフェクトが再生中かを確認する
    /// </summary>
    /// <param name="effect">エフェクトハンドル</param>
    /// <param name="playing">再生中かを示す値を格納する変数 (LN_TRUE=再生中 / LN_FALSE=停止中)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNEffect_IsPlaying(IntPtr effect, out bool playing);

    /// <summary>
    /// タイルマップの作成
    /// </summary>
    /// <param name="tilemap">作成されたタイルマップハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTilemap_Create(out IntPtr tilemap);

    /// <summary>
    /// タイルマップのマップデータを設定する
    /// </summary>
    /// <param name="tilemap">タイルマップハンドル</param>
    /// <param name="intTable">マップデータとする整数テーブルハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTilemap_SetMapData(out IntPtr tilemap, IntPtr intTable);

    /// <summary>
    /// タイルマップのタイルセットとして使用するテクスチャを設定する
    /// </summary>
    /// <param name="tilemap">タイルマップハンドル</param>
    /// <param name="texture">設定するテクスチャハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTilemap_SetTilesetTexture(out IntPtr tilemap, IntPtr texture);

    /// <summary>
    /// タイルセットテクスチャの横方向のタイル数を設定する
    /// </summary>
    /// <param name="tilemap">タイルマップハンドル</param>
    /// <param name="count">タイル数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTilemap_SetTilesetHorizontalTileCount(out IntPtr tilemap, int count);

    /// <summary>
    /// タイルマップのタイルひとつ分のピクセルサイズを設定する
    /// </summary>
    /// <param name="tilemap">タイルマップハンドル</param>
    /// <param name="size">ピクセル数 (正方形。縦横同一サイズ)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTilemap_SetTilePixelSize(out IntPtr tilemap, int size);

    /// <summary>
    /// シェーダの作成
    /// </summary>
    /// <param name="shader">作成されたシェーダハンドルを格納する変数</param>
    /// <param name="filePath">シェーダファイル名</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNShader_CreateFromFile(out IntPtr shader, string filePath);

    /// <summary>
    /// デフォルトで生成されるビューペインの取得
    /// </summary>
    /// <param name="viewPane">ビューペインハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewPane_GetDefaultViewPane(out IntPtr viewPane);

    /// <summary>
    /// ViewPane の背景色を設定する
    /// </summary>
    /// <param name="viewPane">ビューペインハンドル</param>
    /// <param name="color">背景色に設定する色</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewPane_SetBackgroundColor(IntPtr viewPane, ref ColorF color);

    /// <summary>
    /// ViewPane に設定されている Layer のリストを取得する
    /// </summary>
    /// <param name="viewPane">ビューペインハンドル</param>
    /// <param name="objectList">オブジェクトリストハンドル(Layer)を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewPane_GetLayers(IntPtr viewPane, out IntPtr objectList);

    /// <summary>
    /// 【この機能は現在使用できません】
    /// </summary>
    /// <param name="layer">レイヤーハンドル</param>
    /// <param name="priority">優先度 (大きいほど手前(後から)に描画されます)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNLayer_SetPriority(IntPtr layer, int priority);

    /// <summary>
    /// 【この機能は現在使用できません】
    /// </summary>
    /// <param name="layer">レイヤーハンドル</param>
    /// <param name="outPriority">優先度を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNLayer_GetPriority(IntPtr layer, out int outPriority);

    /// <summary>
    /// フレーム更新
    /// </summary>
    /// <param name="viewFilter">ビューフィルタハンドル</param>
    /// <param name="elapsedTime">経過させる時間</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewFilter_Update(IntPtr viewFilter, float elapsedTime);

    /// <summary>
    /// 自動更新有無の設定
    /// </summary>
    /// <param name="viewFilter">ビューフィルタハンドル</param>
    /// <param name="enabled">LN_TRUE=有効 / LN_FALSE=無効</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewFilter_SetEnableAutoUpdate(IntPtr viewFilter, bool enabled);

    /// <summary>
    /// 自動更新有無の取得
    /// </summary>
    /// <param name="viewFilter">ビューフィルタハンドル</param>
    /// <param name="outEnabled">有無を示す値を格納する変数 (LN_TRUE=有効 / LN_FALSE=無効)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNViewFilter_IsEnableAutoUpdate(IntPtr viewFilter, out bool outEnabled);

    /// <summary>
    /// 色調を変更するビューフィルタを作成する
    /// </summary>
    /// <param name="toneFilter">作成された色調ビューフィルタハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNToneFilter_Create(out IntPtr toneFilter);

    /// <summary>
    /// 色調を設定する
    /// </summary>
    /// <param name="toneFilter">色調ビューフィルタハンドル</param>
    /// <param name="tone">設定する色調</param>
    /// <param name="duration">変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNToneFilter_SetTone(IntPtr toneFilter, ref Tone tone, float duration);

    /// <summary>
    /// トランジションを行うビューフィルタを作成する
    /// </summary>
    /// <param name="transitionFilter">作成されたトランジションビューフィルタハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTransitionFilter_Create(out IntPtr transitionFilter);

    /// <summary>
    /// トランジションの準備として、現在の画面を固定する
    /// </summary>
    /// <param name="transitionFilter">トランジションビューフィルタハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTransitionFilter_Freeze(IntPtr transitionFilter);

    /// <summary>
    /// 固定された画面から現在の画面へのトランジションを実行する
    /// </summary>
    /// <param name="transitionFilter">トランジションビューフィルタハンドル</param>
    /// <param name="duration">変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
    /// <param name="maskTexture">トランジションに使用するマスク画像</param>
    /// <param name="vague">マスク画像の境界のあいまいさ</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNTransitionFilter_Transition(IntPtr transitionFilter, double duration, IntPtr maskTexture, float vague);

    /// <summary>
    /// ブラ―(残像)を適用するビューフィルタを作成する
    /// </summary>
    /// <param name="blurFilter">作成されたブラ―ビューフィルタハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNBlurFilter_Create(out IntPtr blurFilter);

    /// <summary>
    /// 色調を設定する
    /// </summary>
    /// <param name="blurFilter">ブラ―ビューフィルタハンドル</param>
    /// <param name="duration">適用時間 (デフォルトではフレーム単位。タイムスケール依存)</param>
    /// <param name="power">ブラーの強度 (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)</param>
    /// <param name="scale">ブラーの大きさ (長さ)</param>
    /// <param name="center">scale を使用して放射ブラーを作るときの中心座標</param>
    /// <param name="baseCamera">center の座標空間の基準となるカメラのハンドル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNBlurFilter_Blur(IntPtr blurFilter, double duration, float power, float scale, ref Vector3 center, IntPtr baseCamera);

    /// <summary>
    /// デフォルトで作成される3Dカメラを取得する
    /// </summary>
    /// <param name="camera">カメラハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_GetDefault3DCamera(out IntPtr camera);

    /// <summary>
    /// デフォルトで作成される3Dカメラを取得する
    /// </summary>
    /// <param name="camera">カメラハンドルを格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_GetDefault2DCamera(out IntPtr camera);

    /// <summary>
    /// カメラ姿勢の設定方法
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="type">姿勢の設定方法</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetCameraType(IntPtr camera, CameraType type);

    /// <summary>
    /// カメラの位置を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="position">座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetPosition(IntPtr camera, ref Vector3 position);

    /// <summary>
    /// カメラの位置を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="x">X 座標</param>
    /// <param name="y">Y 座標</param>
    /// <param name="z">Z 座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetPositionXYZ(IntPtr camera, float x, float y, float z);

    /// <summary>
    /// カメラの注視点を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="at">注視点座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetLookAt(IntPtr camera, ref Vector3 at);

    /// <summary>
    /// カメラの注視点を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="x">注視点 X 座標</param>
    /// <param name="y">注視点 Y 座標</param>
    /// <param name="z">注視点 Z 座標</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetLookAtXYZ(IntPtr camera, float x, float y, float z);

    /// <summary>
    /// カメラの上方向を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="up">上方向ベクトル</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetUpDirection(IntPtr camera, ref Vector3 up);

    /// <summary>
    /// カメラの上方向を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="x">上方向ベクトルの X 成分</param>
    /// <param name="y">上方向ベクトルの Y 成分</param>
    /// <param name="z">上方向ベクトルの Z 成分</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetUpDirectionXYZ(IntPtr camera, float x, float y, float z);

    /// <summary>
    /// カメラの回転角度を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="angle">各軸の回転角度 (ラジアン単位)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetAngle(IntPtr camera, ref Vector3 angle);

    /// <summary>
    /// カメラの回転角度を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="x">X 軸の回転角度 (ラジアン単位)</param>
    /// <param name="y">Y 軸の回転角度 (ラジアン単位)</param>
    /// <param name="z">Z 軸の回転角度 (ラジアン単位)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetAngleXYZ(IntPtr camera, float x, float y, float z);

    /// <summary>
    /// カメラの姿勢行列を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="matrix">変換行列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetMatrix(IntPtr camera, ref Matrix matrix);

    /// <summary>
    /// カメラの視野角を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="fov">視野角</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetFovY(IntPtr camera, float fov);

    /// <summary>
    /// カメラの最手前クリップ距離と最奥クリップ距離を設定する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="nearClip">手前クリップ距離</param>
    /// <param name="farClip">奥クリップ距離</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_SetNearFarClip(IntPtr camera, float nearClip, float farClip);

    /// <summary>
    /// カメラのビュー行列を取得する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="outMatrix">ビュー行列を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_GetViewMatrix(IntPtr camera, out Matrix outMatrix);

    /// <summary>
    /// カメラのプロジェクション行列を取得する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="outMatrix">プロジェクション行列を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_GetProjectionMatrix(IntPtr camera, out Matrix outMatrix);

    /// <summary>
    /// ビュー行列とプロジェクション行列の積を取得する
    /// </summary>
    /// <param name="camera">カメラハンドル</param>
    /// <param name="outMatrix">ビュー行列とプロジェクション行列の積を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCamera_GetViewProjectionMatrix(IntPtr camera, out Matrix outMatrix);

    /// <summary>
    /// ウィンドウタイトルの設定
    /// </summary>
    /// <param name="title">ウィンドウタイトル文字列</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetWindowTitle(string title);

    /// <summary>
    /// ウィンドウサイズの設定
    /// </summary>
    /// <param name="width">クライアント領域の幅 (default:640)</param>
    /// <param name="height">クライアント領域の高さ (default:480)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetWindowSize(int width, int height);

    /// <summary>
    /// バックバッファサイズの設定
    /// </summary>
    /// <param name="width">バックバッファの幅 (default:0)</param>
    /// <param name="height">バックバッファの高さ (default:0)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetBackbufferSize(int width, int height);

    /// <summary>
    /// ウィンドウリサイズの有効設定
    /// </summary>
    /// <param name="enabled">LN_TRUE の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:LN_TRUE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetWindowResizeEnabled(bool enabled);

    /// <summary>
    /// アクセラレータキーとして割り当てるキーの設定
    /// </summary>
    /// <param name="acceleratorKey">アクセラレータの種別</param>
    /// <param name="keyCode">割り当てるキーコード</param>
    /// <param name="isShift">LN_TRUE を指定すると、Shift キーを修飾子とする</param>
    /// <param name="isCtrl">LN_TRUE を指定すると、Ctrl キーを修飾子とする</param>
    /// <param name="isAlt">LN_TRUE を指定すると、Alt キーを修飾子とする</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetAcceleratorKey(AcceleratorKey acceleratorKey, KeyCode keyCode, bool isShift, bool isCtrl, bool isAlt);

    /// <summary>
    /// ログファイルの出力設定
    /// </summary>
    /// <param name="flag">LN_TRUE=ログファイルを出力する / LN_FALSE=しない (default:LN_FALSE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetLogFileEnabled(bool flag);

    /// <summary>
    /// コンソールの割り当て設定
    /// </summary>
    /// <param name="flag">LN_TRUE=割り当てる / LN_FALSE=割り当てない (default:LN_FALSE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetConsoleEnabled(bool flag);

    /// <summary>
    /// ファイルを開くときにアクセスする暗号化アーカイブの登録
    /// </summary>
    /// <param name="filePath">アーカイブファイル名</param>
    /// <param name="password">アーカイブファイルを開くためのパスワード</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_RegisterArchive(string filePath, string password);

    /// <summary>
    /// ファイルへのアクセス制限の設定
    /// </summary>
    /// <param name="priority">制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetFileAccessPriority(FileAccessPriority priority);

    /// <summary>
    /// DirectMusic の使用設定
    /// </summary>
    /// <param name="mode">DirectMusic の使用方法 (default:LN_DMINITMODE_NOT_USE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetDirectMusicInitMode(DirectMusicInitMode mode);

    /// <summary>
    /// 音声データの最大キャッシュ数の設定
    /// </summary>
    /// <param name="count">最大数 (default:32)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetSoundMaxCacheCount(int count);

    /// <summary>
    /// 音声再生方法の自動選択の音声データバイト数閾値
    /// </summary>
    /// <param name="bytes">最大数 (default:100000)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SoundPlayTypeThreshold(int bytes);

    /// <summary>
    /// フレームレートの設定
    /// </summary>
    /// <param name="frameRate">フレームレート (default:60)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetFrameRate(int frameRate);

    /// <summary>
    /// ライブラリ更新時時のフレーム待ち処理の有効設定
    /// </summary>
    /// <param name="enabled">LN_TRUE=有効 / LN_FALSE=無効 (default:LN_TRUE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetEnableFrameWait(bool enabled);

    /// <summary>
    /// 自動更新時のタイムスケールの設定
    /// </summary>
    /// <param name="timeScale">タイムスケール (default:0.0)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetAutoUpdateTimeScale(float timeScale);

    /// <summary>
    /// ライブラリで使用するフォントファイルを登録する
    /// </summary>
    /// <param name="filePath">フォントファイル名</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_RegisterFontFile(string filePath);

    /// <summary>
    /// テクスチャの最大キャッシュ数の設定
    /// </summary>
    /// <param name="count">最大数 (default:32)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetTextureCacheSize(int count);

    /// <summary>
    /// シェーダの最大キャッシュ数の設定
    /// </summary>
    /// <param name="count">最大数 (default:32)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetShaderCacheSize(int count);

    /// <summary>
    /// 例外エラー発生時のメッセージボックス表示設定
    /// </summary>
    /// <param name="enabled">LN_TRUE の場合、表示する (default:LN_TRUE)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetExceptionMessageBoxEnabled(bool enabled);

    /// <summary>
    /// ユーザー定義のウィンドウハンドルの設定
    /// </summary>
    /// <param name="hWnd">ユーザー定義のウィンドウハンドル (default:NULL)</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNConfig_SetWin32UserWindowHandle(IntPtr hWnd);

    /// <summary>
    /// ライブラリの初期化
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_Initialize();

    /// <summary>
    /// ファイル入出力機能の初期化
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_InitializeFileIO();

    /// <summary>
    /// 音声機能の初期化
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_InitializeAudio();

    /// <summary>
    /// 入力機能の初期化
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_InitializeInput();

    /// <summary>
    /// ライブラリのフレーム更新
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_Update();

    /// <summary>
    /// 画面の更新タイミングのリセット
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_ResetFrameDelay();

    /// <summary>
    /// アプリケーションを終了するべきかを確認する
    /// </summary>
    /// <param name="requested">終了要求の有無を格納する変数</param>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static ErrorCode LNCore_IsEndRequested(out bool requested);

    /// <summary>
    /// ライブラリの終了処理
    /// </summary>
    [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
    public extern static void LNCore_Terminate();



        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
