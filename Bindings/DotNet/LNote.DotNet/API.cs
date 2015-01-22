using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(Result errorCode);

    /// <summary>
    /// 結果・エラーコード
    /// </summary>
    public enum Result
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
        /// シェーダプログラム等のコンパイルエラーが発生した
        /// </summary>
        Compilation = -9,

        /// <summary>
        /// スレッド実行中に例外が発生した
        /// </summary>
        Thread = -10,

        /// <summary>
        /// 継続不能なエラー
        /// </summary>
        Fatal = -11,

        /// <summary>
        /// COM 呼び出しエラー
        /// </summary>
        ComCall = -12,

        /// <summary>
        /// OpenGL 関係のエラー
        /// </summary>
        Opengl = -13,

        Directx = -14,

        Win32api = -15,

    }

    /// <summary>
    /// ファイルへのアクセス制限
    /// </summary>
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

    /// <summary>
    /// ファイルオープンモード
    /// </summary>
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

    /// <summary>
    /// ファイルを開く方法または作成する方法
    /// </summary>
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

    }

    /// <summary>
    /// ファイルにアクセスする方法
    /// </summary>
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

    /// <summary>
    /// シーク位置移動の起点
    /// </summary>
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

    /// <summary>
    /// 非同期処理の状態
    /// </summary>
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

    /// <summary>
    /// 回転順序
    /// </summary>
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

    /// <summary>
    /// キーボードのキーコード
    /// </summary>
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

    /// <summary>
    /// アクセラレータキー
    /// </summary>
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

    /// <summary>
    /// 仮想ボタン
    /// </summary>
    public enum Button
    {
        /// <summary>
        /// ボタン0 (キーボード='Z' ジョイパッド=1番ボタン)
        /// </summary>
        Button0 = 0,

        /// <summary>
        /// ボタン1 (キーボード='X' ジョイパッド=2)
        /// </summary>
        Button1 = 1,

        /// <summary>
        /// ボタン2 (キーボード='C' ジョイパッド=3)
        /// </summary>
        Button2 = 2,

        /// <summary>
        /// ボタン3 (キーボード='A' ジョイパッド=4)
        /// </summary>
        Button3 = 3,

        /// <summary>
        /// ボタン4 (キーボード='S' ジョイパッド=5)
        /// </summary>
        Button4 = 4,

        /// <summary>
        /// ボタン5 (キーボード='D' ジョイパッド=6)
        /// </summary>
        Button5 = 5,

        /// <summary>
        /// ボタン6 (キーボード='Q' ジョイパッド=7)
        /// </summary>
        Button6 = 6,

        /// <summary>
        /// ボタン7 (キーボード='W' ジョイパッド=8)
        /// </summary>
        Button7 = 7,

        /// <summary>
        /// ボタン8 (キーボード=None ジョイパッド=9)
        /// </summary>
        Button8 = 8,

        /// <summary>
        /// ボタン9 (キーボード=None ジョイパッド=10)
        /// </summary>
        Button9 = 9,

        /// <summary>
        /// ボタン10 (キーボード=None ジョイパッド=11)
        /// </summary>
        Button10 = 10,

        /// <summary>
        /// ボタン11 (キーボード=None ジョイパッド=12)
        /// </summary>
        Button11 = 11,

        /// <summary>
        /// ボタン12 (キーボード=None ジョイパッド=13)
        /// </summary>
        Button12 = 12,

        /// <summary>
        /// ボタン13 (キーボード=None ジョイパッド=14)
        /// </summary>
        Button13 = 13,

        /// <summary>
        /// ボタン14 (キーボード=None ジョイパッド=15)
        /// </summary>
        Button14 = 14,

        /// <summary>
        /// ボタン15 (キーボード=None ジョイパッド=16)
        /// </summary>
        Button15 = 15,

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

    /// <summary>
    /// デバイスボタン番号の分類
    /// </summary>
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

    /// <summary>
    /// 全てのデバイス用の仮想ボタン番号 (デバイスボタン番号) (1 ～ 255 は LN_KEY_～と兼用)
    /// </summary>
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

    /// <summary>
    /// DirectMusic の初期化方法
    /// </summary>
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

    /// <summary>
    /// 再生方法 ( オーディオプレイヤーの種類 )
    /// </summary>
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

    /// <summary>
    /// 音声の再生状態
    /// </summary>
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

    /// <summary>
    /// 音量フェード完了時の動作の記述
    /// </summary>
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

    /// <summary>
    /// GameAudio 内部 Sound
    /// </summary>
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

    /// <summary>
    /// ウィンドウとバックバッファのリサイズモード
    /// </summary>
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

    /// <summary>
    /// テクスチャフォーマット
    /// </summary>
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

    /// <summary>
    /// 合成方法
    /// </summary>
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

    /// <summary>
    /// カリング方法
    /// </summary>
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

    /// <summary>
    /// テキストアライン
    /// </summary>
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

    /// <summary>
    /// ノードの配置方法
    /// </summary>
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

    /// <summary>
    /// 3D 空間での基準方向を表す値
    /// </summary>
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

    /// <summary>
    /// メッシュ生成オプション
    /// </summary>
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

    /// <summary>
    /// カメラ姿勢の設定方法
    /// </summary>
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

    /// <summary>
    /// 座標系
    /// </summary>
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

    /// <summary>
    /// ハンドルオブジェクトの種類
    /// </summary>
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
        public extern static void LNRect_Contains(ref Rect rect,  int x,  int y, out bool result);

        /// <summary>
        /// 例外発生時にコールバックされる関数を登録する
        /// </summary>
        /// <param name="handler"></param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNException_SetHandler( ExceptionCallback handler);

        /// <summary>
        /// 例外を発生させる
        /// </summary>
        /// <param name="errCode"></param>
        /// <param name="message"></param>
        /// <param name="file"></param>
        /// <param name="line"></param>
        /// <param name="func"></param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNException_Raise( Result errCode,  string message,  string file,  int line,  string func);

        /// <summary>
        /// 最後に発生した例外のエラーコードを取得する
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNException_GetLastErrorCode();

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
        public extern static Result LNObject_Release( IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウントをインクリメントする
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_AddRef( IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウント取得する
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        /// <param name="count">参照カウントを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_GetRefCount( IntPtr hadnleObject, out int count);

        /// <param name="hadnleObject"></param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static byte[] LNObject_GetTypeUserData( IntPtr hadnleObject);

        /// <param name="hadnleObject"></param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static byte[] LNObject_GetInternalObject( IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトリストに格納されているオブジェクトの数を取得する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="count">要素の数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_GetCount( IntPtr objectList, out int count);

        /// <summary>
        /// オブジェクトリストの指定したインデックスにオブジェクトを設定する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="index">インデックス(要素番号)</param>
        /// <param name="item">設定するオブジェクト</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_SetAt( IntPtr objectList,  int index,  IntPtr item);

        /// <summary>
        /// オブジェクトリストの指定したインデックスのオブジェクトを取得する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="index">インデックス(要素番号)</param>
        /// <param name="outItem">オブジェクトを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_GetAt( IntPtr objectList,  int index, out IntPtr outItem);

        /// <summary>
        /// オブジェクトリストの末尾にオブジェクトを追加する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="item">追加するオブジェクト</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_Add( IntPtr objectList,  IntPtr item);

        /// <summary>
        /// オブジェクトリストから全てのオブジェクトを削除する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_Clear( IntPtr objectList);

        /// <summary>
        /// オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="index">item を挿入するインデックス</param>
        /// <param name="item">挿入するオブジェクト</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_Insert( IntPtr objectList,  int index,  IntPtr item);

        /// <summary>
        /// オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="item">リストから削除するオブジェクト</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_Remove( IntPtr objectList,  IntPtr item);

        /// <summary>
        /// オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="item">リストから削除するオブジェクト</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_RemoveAll( IntPtr objectList,  IntPtr item);

        /// <summary>
        /// オブジェクトリストの指定したインデックスにあるオブジェクトを削除する
        /// </summary>
        /// <param name="objectList">オブジェクトリストハンドル</param>
        /// <param name="index">削除するオブジェクトのインデックス番号</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObjectList_RemoveAt( IntPtr objectList,  int index);

        /// <summary>
        /// 整数型の多次元配列を作成する
        /// </summary>
        /// <param name="intTable">作成されたテーブルハンドルを格納する変数</param>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_Create(out IntPtr intTable,  int xSize,  int ySize = 1,  int zSize = 1);

        /// <summary>
        /// 整数型の多次元配列を作成する
        /// </summary>
        /// <param name="intTable">作成されたテーブルハンドルを格納する変数</param>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        /// <param name="srcData">XXXX</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_CreateFromSrcData(out IntPtr intTable,  int xSize,  int ySize,  int zSize,  IntPtr srcData);

        /// <summary>
        /// 整数型テーブルの指定したインデックスに値を設定する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="xSize">X 要素インデックス</param>
        /// <param name="ySize">Y 要素インデックス</param>
        /// <param name="zSize">Z 要素インデックス</param>
        /// <param name="value">設定する整数値</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_SetValue( IntPtr intTable,  int xSize,  int ySize,  int zSize,  int value);

        /// <summary>
        /// 整数型テーブルの指定したインデックスに値を設定する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="xSize">X 要素インデックス</param>
        /// <param name="ySize">Y 要素インデックス</param>
        /// <param name="zSize">Z 要素インデックス</param>
        /// <param name="value">値を格納する整数型変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_GetValue( IntPtr intTable,  int xSize,  int ySize,  int zSize, out int value);

        /// <summary>
        /// 整数型テーブルの要素数を変更する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="xSize">X 要素数</param>
        /// <param name="ySize">Y 要素数</param>
        /// <param name="zSize">Z 要素数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_Resize( IntPtr intTable,  int xSize,  int ySize,  int zSize);

        /// <summary>
        /// 整数型テーブルの X 要素数を取得する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="xSize">X 要素数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_GetXSize( IntPtr intTable, out int xSize);

        /// <summary>
        /// 整数型テーブルの Y 要素数を取得する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="ySize">Y 要素数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_GetYSize( IntPtr intTable, out int ySize);

        /// <summary>
        /// 整数型テーブルの Z 要素数を取得する
        /// </summary>
        /// <param name="intTable">整数型テーブルハンドル</param>
        /// <param name="zSize">Z 要素数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNIntTable_GetZSize( IntPtr intTable, out int zSize);

        /// <summary>
        /// BGM を演奏する
        /// </summary>
        /// <param name="filePath">ファイル名</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGM( string filePath,  int volume = 100,  int pitch = 100,  int fadeTime = 0);

        /// <summary>
        /// メモリ上の音声データから BGM を演奏する
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGMMem( IntPtr data,  int dataSize,  int volume = 100,  int pitch = 100,  int fadeTime = 0);

        /// <summary>
        /// BGM の演奏を停止する
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopBGM( int fadeTime = 0);

        /// <summary>
        /// BGS を演奏する
        /// </summary>
        /// <param name="filePath">ファイル名</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGS( string filePath,  int volume = 100,  int pitch = 100,  int fadeTime = 0);

        /// <summary>
        /// メモリ上の音声データから BGS を演奏する
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGSMem( IntPtr data,  int dataSize,  int volume = 100,  int pitch = 100,  int fadeTime = 0);

        /// <summary>
        /// BGS の演奏を停止する
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopBGS( int fadeTime = 0);

        /// <summary>
        /// ME を演奏する
        /// </summary>
        /// <param name="filePath">ファイル名</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayME( string filePath,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声データから ME を演奏する
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayMEMem( IntPtr data,  int dataSize,  int volume = 100,  int pitch = 100);

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
        public extern static Result LNAudio_PlaySE( string filePath,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// SE を演奏する (3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイル名</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE3D( string filePath, ref Vector3 position,  float distance,  int volume = 100,  int pitch = 100);

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
        public extern static Result LNAudio_PlaySE3DXYZ( string filePath,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声データから SE を演奏する
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySEMem( IntPtr data,  int dataSize,  int volume = 100,  int pitch = 100);

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
        public extern static Result LNAudio_PlaySE3DMem( IntPtr data,  int dataSize, ref Vector3 position,  float distance,  int volume = 100,  int pitch = 100);

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
        public extern static Result LNAudio_PlaySE3DMemXYZ( IntPtr data,  int dataSize,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100);

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
        public extern static Result LNAudio_SetMetreUnitDistance( float distance);

        /// <summary>
        /// 再生中のBGMの音量を設定する (フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_SetBGMVolume( int volume,  int fadeTime = 0);

        /// <summary>
        /// 再生中のBGSの音量を設定する (フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (ミリ秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_SetBGSVolume( int volume,  int fadeTime = 0);

        /// <summary>
        /// リスナーの位置の設定
        /// </summary>
        /// <param name="position">3D 空間上の座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetPosition(ref Vector3 position);

        /// <summary>
        /// リスナーの位置の設定
        /// </summary>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNSoundListener_SetPositionXYZ( float x,  float y,  float z);

        /// <summary>
        /// リスナーの正面方向の設定
        /// </summary>
        /// <param name="direction">向き</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetDirection(ref Vector3 direction);

        /// <summary>
        /// リスナーの正面方向の設定
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNSoundListener_SetDirectionXYZ( float x,  float y,  float z);

        /// <summary>
        /// リスナーの上方向の設定 (正面方向とは直交であること)
        /// </summary>
        /// <param name="direction">上方向</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetUpDirection(ref Vector3 direction);

        /// <summary>
        /// リスナーの上方向の設定 (正面方向とは直交であること)
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNSoundListener_SetUpDirectionXYZ( float x,  float y,  float z);

        /// <summary>
        /// リスナーの速度の設定
        /// </summary>
        /// <param name="velocity">速度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_Velocity(ref Vector3 velocity);

        /// <summary>
        /// リスナーの速度の設定
        /// </summary>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNSoundListener_VelocityXYZ( float x,  float y,  float z);

        /// <summary>
        /// ファイルから音声を作成する
        /// </summary>
        /// <param name="sound">sound</param>
        /// <param name="filePath">ファイル名</param>
        /// <param name="enable3d">true の場合、3D 音源として作成する</param>
        /// <param name="playType">音声の再生方法</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_Create(out IntPtr sound,  string filePath,  bool enable3d = false,  SoundPlayingType playType = SoundPlayingType.Auto);

        /// <summary>
        /// メモリ上の音声ファイルデータから音声を作成する
        /// </summary>
        /// <param name="sound">sound</param>
        /// <param name="data">メモリ上の音声データへのポインタ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="enable3d">true の場合、3D 音源として作成する</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_CreateMem(out IntPtr sound,  IntPtr data,  int dataSize,  bool enable3d = false);

        /// <summary>
        /// ボリュームの設定
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="volume">ボリューム (0～100)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetVolume( IntPtr sound,  int volume);

        /// <summary>
        /// ボリュームの取得
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="volume">ボリュームを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetVolume( IntPtr sound, out int volume);

        /// <summary>
        /// ピッチの設定
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="pitch">ピッチ (50～200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetPitch( IntPtr sound,  int pitch);

        /// <summary>
        /// ピッチの取得
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="pitch">ピッチを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetPitch( IntPtr sound, out int pitch);

        /// <summary>
        /// ループ再生の有効設定
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="loopEnable">LN_TRUE = ループ再生する / LN_FALSE = しない</param>
        /// <param name="begin">ループ領域の先頭位置 (サンプル数単位)</param>
        /// <param name="length">ループ領域長さ (サンプル数単位)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetLoopState( IntPtr sound,  bool loopEnable,  int begin = 0,  int length = 0);

        /// <summary>
        /// ループ再生が有効かを調べる
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="enabled">ループ再生の有無状態を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_IsLoop( IntPtr sound, out bool enabled);

        /// <summary>
        /// サウンドの再生状態を取得する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="state">状態を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetPlayState( IntPtr sound, out SoundPlayingState state);

        /// <summary>
        /// 音声を再生する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_Play( IntPtr sound);

        /// <summary>
        /// 再生を停止する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_Stop( IntPtr sound);

        /// <summary>
        /// 一時停止
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="pause">LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_Pause( IntPtr sound,  bool pause);

        /// <summary>
        /// 音量のフェード
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="targetVolume">変更先の音量</param>
        /// <param name="time">フェードにかける時間 (ミリ秒)</param>
        /// <param name="fadeState">フェード完了後の動作の指定</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_FadeVolume( IntPtr sound,  int targetVolume,  int time,  SoundFadeState fadeState);

        /// <summary>
        /// 再生したサンプル数の取得
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="samples">再生したサンプル数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetUnitsPlayed( IntPtr sound, out int samples);

        /// <summary>
        /// 音声データ全体のサンプル数の取得
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="samples">音声データ全体のサンプル数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetTotalUnits( IntPtr sound, out int samples);

        /// <summary>
        /// サンプリング周波数の取得
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="frequency">サンプリング周波数を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_GetUnitsParSecond( IntPtr sound, out int frequency);

        /// <summary>
        /// 3D 音源かを調べる
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="enabled">3D 音源かを示す値を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_Is3DSound( IntPtr sound, out bool enabled);

        /// <summary>
        /// 3D 音源としての位置を設定する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="position">3D 空間上の座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetEmitterPosition( IntPtr sound, ref Vector3 position);

        /// <summary>
        /// 3D 音源としての位置を設定する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetEmitterPositionXYZ( IntPtr sound,  float x,  float y,  float z);

        /// <summary>
        /// 3D 音源としての速度を設定する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="velocity">速度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetEmitterVelocity( IntPtr sound, ref Vector3 velocity);

        /// <summary>
        /// 3D 音源としての速度を設定する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetEmitterVelocityXYZ( IntPtr sound,  float x,  float y,  float z);

        /// <summary>
        /// 3D 音源の減衰距離 (聴こえなくなる距離) を設定する
        /// </summary>
        /// <param name="sound">サウンドハンドル</param>
        /// <param name="distance">距離</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSound_SetEmitterDistance( IntPtr sound,  float distance);



        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
