# ERRORCODE
module LN::ErrorCode
    # 成功
    OK = 0
    # 不明なエラー
    UNKNOWN = -1
    # オブジェクトの現在の状態に対して無効な呼び出しが行われた
    INVALID_OPERATION = -2
    # 渡された引数のいずれかが無効
    ARGUMENT = -3
    # メモリ不足
    OUT_OF_MEMORY = -2
    # ファイルが存在しない、またはアクセスできない
    FILE_NOT_FOUND = -3
    # システム API 呼び出しエラー
    SYSTEM_CALL = -6
    # 未実装の機能を呼び出そうとした
    NOT_IMPLEMENTED = -7
    # ファイル形式などの入力データのフォーマットが無効、またはサポートされていない
    INVALID_FORMAT = -8
    # スレッド実行中に例外が発生した
    THREAD = -9
    # 継続不能なエラー
    FATAL = -10
    # COM 呼び出しエラー
    COM_CALL = -11
    # OpenGL 関係のエラー
    OPENGL = -12
    # 
    DIRECTX = -13
    # 
    WIN32API = -14
end
# FILEACCESSPRIORITY
module LN::FileAccessPriority
    # アーカイブ優先
    ARCHIVE_FIRST = 0
    # ディレクトリ優先
    DIRECTORY_FIRST = 1
    # アーカイブのみ
    ARCHIVE_ONLY = 2
end
# FILETYPE
module LN::FileType
    # バイナリモードで開く
    BINARY = 0
    # テキストモードで開く
    TEXT = 1
end
# FILEMODE
module LN::FileMode
    # 既存のファイルを開く
    OPEN = 0
    # 新しいファイルを作成する。既に存在する場合は上書きされる
    CREATE = 1
    # ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する
    APPEND = 2
    # 
    MAX = 3
end
# FILEACCESS
module LN::FileAccess
    # 読み取りアクセス
    READ = 0
    # 読み取り及び書き込みアクセス
    READWRITE = 1
    # 書き込みアクセス
    WRITE = 2
end
# SEEKORIGIN
module LN::SeekOrigin
    # ファイルの先頭
    SEEK_BEGIN = 0
    # 現在のシーク位置
    SEEK_CURRENT = 1
    # ファイルの末尾
    SEEK_END = 2
end
# ASYNCIOSTATE
module LN::AsyncIOState
    # 待機状態 (初期状態)
    IDLE = 0
    # 実行可能状態 (実際の処理開始を待っている)
    READY = 1
    # 実行状態 (処理中)
    PROCESSING = 2
    # 終了状態 (成功)
    COMPLETED = 3
    # 終了状態 (処理終了後・処理に失敗した)
    FAILED = 4
end
# ROTATIONORDER
module LN::RotationOrder
    # X → Y → Z
    XYZ = 0
    # X → Z → Y
    XZY = 1
    # Y → X → Z
    YXZ = 2
    # Y → Z → X
    YZX = 3
    # Z → X → Y
    ZXY = 4
    # Z → Y → X
    ZYX = 5
end
# KEYCODE
module LN::KeyCode
    # 
    KEY_UNKNOWN = 0
    # 
    KEY_A = 1
    # 
    KEY_B = 2
    # 
    KEY_C = 3
    # 
    KEY_D = 4
    # 
    KEY_E = 5
    # 
    KEY_F = 6
    # 
    KEY_G = 7
    # 
    KEY_H = 8
    # 
    KEY_I = 9
    # 
    KEY_J = 10
    # 
    KEY_K = 11
    # 
    KEY_L = 12
    # 
    KEY_M = 13
    # 
    KEY_N = 14
    # 
    KEY_O = 15
    # 
    KEY_P = 16
    # 
    KEY_Q = 17
    # 
    KEY_R = 18
    # 
    KEY_S = 19
    # 
    KEY_T = 20
    # 
    KEY_U = 21
    # 
    KEY_V = 22
    # 
    KEY_W = 23
    # 
    KEY_X = 24
    # 
    KEY_Y = 25
    # 
    KEY_Z = 26
    # '0' (NumPad ではない)
    KEY_D0 = 27
    # '1' (NumPad ではない)
    KEY_D1 = 28
    # '2' (NumPad ではない)
    KEY_D2 = 29
    # '3' (NumPad ではない)
    KEY_D3 = 30
    # '4' (NumPad ではない)
    KEY_D4 = 31
    # '5' (NumPad ではない)
    KEY_D5 = 32
    # '6' (NumPad ではない)
    KEY_D6 = 33
    # '7' (NumPad ではない)
    KEY_D7 = 34
    # '8' (NumPad ではない)
    KEY_D8 = 35
    # '9' (NumPad ではない)
    KEY_D9 = 36
    # 
    KEY_F1 = 37
    # 
    KEY_F2 = 38
    # 
    KEY_F3 = 39
    # 
    KEY_F4 = 40
    # 
    KEY_F5 = 41
    # 
    KEY_F6 = 42
    # 
    KEY_F7 = 43
    # 
    KEY_F8 = 44
    # 
    KEY_F9 = 45
    # 
    KEY_F10 = 46
    # 
    KEY_F11 = 47
    # 
    KEY_F12 = 48
    # 
    KEY_SPACE = 49
    # 
    KEY_ESCAPE = 50
    # 
    KEY_UP = 51
    # 
    KEY_DOWN = 52
    # 
    KEY_LEFT = 53
    # 
    KEY_RIGHT = 54
    # 
    KEY_LSHIFT = 55
    # 
    KEY_RSHIFT = 56
    # 
    KEY_LCTRL = 57
    # 
    KEY_RCTRL = 58
    # 
    KEY_LALT = 59
    # 
    KEY_RALT = 60
    # 
    KEY_TAB = 61
    # 
    KEY_ENTER = 62
    # 
    KEY_BACKSPACE = 63
    # 
    KEY_INSERT = 64
    # 
    KEY_DELETE = 65
    # 
    KEY_PAGEUP = 66
    # 
    KEY_PAGEDOWN = 67
    # 
    KEY_HOME = 68
    # 
    KEY_END = 69
    # :	[非推奨 GLFWでは;]
    KEY_COLON = 70
    # ;	[非推奨 GLFWでは=]
    KEY_SEMICOLON = 71
    # ,
    KEY_COMMA = 72
    # .
    KEY_PERIOD = 73
    # /
    KEY_SLASH = 74
    # -
    KEY_MINUS = 75
    # \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
    KEY_BACKSLASH = 76
    # \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
    KEY_YEN = 77
    # ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
    KEY_CARET = 78
    # [
    KEY_LBRACKET = 79
    # ]
    KEY_RBRACKET = 80
    # Android '+'
    KEY_ANDROID_PLUS = 81
end
# ACCELERATORKEY
module LN::AcceleratorKey
    # フルスクリーン切り替えを行うアクセラレータキー
    TOGGLE_FULLSCREEN = 0
    # デバッグ用の情報を表示切替を行うアクセラレータキー
    SHOW_DEBUG_STATISTICS = 1
end
# BUTTON
module LN::Button
    # Aボタン (キーボード='Z' ジョイパッド=1番ボタン)
    A = 0
    # Bボタン (キーボード='X' ジョイパッド=2)
    B = 1
    # Cボタン (キーボード='C' ジョイパッド=3)
    C = 2
    # Xボタン (キーボード='A' ジョイパッド=4)
    X = 3
    # Yボタン (キーボード='S' ジョイパッド=5)
    Y = 4
    # Zボタン (キーボード='D' ジョイパッド=6)
    Z = 5
    # Lボタン (キーボード='Q' ジョイパッド=7)
    L = 6
    # Rボタン (キーボード='W' ジョイパッド=8)
    R = 7
    # L2ボタン (キーボード=None ジョイパッド=9)
    L2 = 8
    # R2ボタン (キーボード=None ジョイパッド=10)
    R2 = 9
    # L3ボタン (キーボード=None ジョイパッド=11)
    L3 = 10
    # R3ボタン (キーボード=None ジョイパッド=12)
    R3 = 11
    # Startボタン (キーボード=None ジョイパッド=13)
    START = 12
    # Selectボタン (キーボード=None ジョイパッド=14)
    SELECT = 13
    # Sub1ボタン (キーボード=None ジョイパッド=15)
    SUB1 = 14
    # Sub2ボタン (キーボード=None ジョイパッド=16)
    SUB2 = 15
    # 左ボタン (キーボード=LN_KEY_LEFT  ジョイパッド=POV左&第1X軸-)
    LEFT = 16
    # 右ボタン (キーボード=LN_KEY_RIGHT ジョイパッド=POV右&第1X軸+)
    RIGHT = 17
    # 上ボタン (キーボード=LN_KEY_UP    ジョイパッド=POV上&第1Y軸-)
    UP = 18
    # 下ボタン (キーボード=LN_KEY_DOWN  ジョイパッド=POV下&第1Y軸+)
    DOWN = 19
    # 第1レバー X- 方向
    AXIS_1X_MINUIS = 20
    # 第1レバー X+ 方向
    AXIS_1X_PLUS = 21
    # 第1レバー Y- 方向
    AXIS_1Y_MINUIS = 22
    # 第1レバー Y+ 方向
    AXIS_1Y_PLUS = 23
    # 第2レバー X- 方向
    AXIS_2X_MINUIS = 24
    # 第2レバー X+ 方向
    AXIS_2X_PLUS = 25
    # 第2レバー Y- 方向
    AXIS_2Y_MINUIS = 26
    # 第2レバー Y+ 方向
    AXIS_2Y_PLUS = 27
end
# DEVICEBUTTONTYPE
module LN::DeviceButtonType
    # 不明なデバイス種類
    UNKNOWN = 0
    # キーボード
    KEYBOARD = 1
    # マウスボタン
    MOUSE = 2
    # ジョイパッドのボタン
    JOYPAD_BUTTON = 3
    # ジョイパッドのPOV
    JOYPAD_POV = 4
    # ジョイパッドの軸
    JOYPAD_AXIS = 5
end
# DEVICEBUTTON
module LN::DeviceButton
    # 
    UNKNOWN = 0
    # 
    MOUSE_1 = 256
    # 
    MOUSE_2 = 257
    # 
    MOUSE_3 = 258
    # 
    MOUSE_4 = 259
    # 
    MOUSE_5 = 260
    # 
    MOUSE_6 = 261
    # 
    MOUSE_7 = 262
    # 
    MOUSE_8 = 263
    # 
    JOYPAD_BUTTON_1 = 264
    # 
    JOYPAD_BUTTON_2 = 265
    # 
    JOYPAD_BUTTON_3 = 266
    # 
    JOYPAD_BUTTON_4 = 267
    # 
    JOYPAD_BUTTON_5 = 268
    # 
    JOYPAD_BUTTON_6 = 269
    # 
    JOYPAD_BUTTON_7 = 270
    # 
    JOYPAD_BUTTON_8 = 271
    # 
    JOYPAD_BUTTON_9 = 272
    # 
    JOYPAD_BUTTON_10 = 273
    # 
    JOYPAD_BUTTON_11 = 274
    # 
    JOYPAD_BUTTON_12 = 275
    # 
    JOYPAD_BUTTON_13 = 276
    # 
    JOYPAD_BUTTON_14 = 277
    # 
    JOYPAD_BUTTON_15 = 278
    # 
    JOYPAD_BUTTON_16 = 279
    # 
    JOYPAD_POV_UP = 280
    # 
    JOYPAD_POV_RIGHT = 281
    # 
    JOYPAD_POV_DOWN = 282
    # 
    JOYPAD_POV_LEFT = 283
    # X1 -1.0
    JOYPAD_AXIS_1_MINUIS = 284
    # X1  1.0
    JOYPAD_AXIS_1_PLUS = 285
    # Y1 -1.0
    JOYPAD_AXIS_2_MINUIS = 286
    # Y1  1.0
    JOYPAD_AXIS_2_PLUS = 287
    # X2 -1.0
    JOYPAD_AXIS_3_MINUIS = 288
    # X2  1.0
    JOYPAD_AXIS_3_PLUS = 289
    # Y2 -1.0
    JOYPAD_AXIS_4_MINUIS = 290
    # Y2  1.0
    JOYPAD_AXIS_4_PLUS = 291
    # X3 -1.0
    JOYPAD_AXIS_5_MINUIS = 292
    # X3  1.0     XInput LT
    JOYPAD_AXIS_5_PLUS = 293
    # Y3 -1.0
    JOYPAD_AXIS_6_MINUIS = 294
    # Y3  1.0     XInput RT
    JOYPAD_AXIS_6_PLUS = 295
    # X4 -1.0
    JOYPAD_AXIS_7_MINUIS = 296
    # X4  1.0
    JOYPAD_AXIS_7_PLUS = 297
    # Y4 -1.0
    JOYPAD_AXIS_8_MINUIS = 298
    # Y4  1.0
    JOYPAD_AXIS_8_PLUS = 299
end
# DIRECTMUSICINITMODE
module LN::DirectMusicInitMode
    # DirectMusic を使用しない
    NOT_USE = 0
    # 通常
    NORMAL = 1
    # 別スレッドで初期化して、再生時に未完了の場合は待つ
    THREAD_WAIT = 2
    # 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
    THREAD_REQUEST = 3
end
# SOUNDPLAYINGTYPE
module LN::SoundPlayingType
    # 不明な再生方法
    UNKNOWN = 0
    # 自動選択 ( デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります )
    AUTO = 1
    # オンメモリ
    ONMEMORY = 2
    # ストリーミング
    STREAMING = 3
    # SMF
    MIDI = 4
end
# SOUNDPLAYINGSTATE
module LN::SoundPlayingState
    # 停止中
    STOPPED = 0
    # 再生中
    PLAYING = 1
    # 一時停止中
    PAUSING = 2
end
# SOUNDFADESTATE
module LN::SoundFadeState
    # 継続(なにもしない)
    CONTINUE = 0
    # 停止する (stop())
    STOP = 1
    # 停止して、音量を元の値に戻す
    STOP_RESET = 2
    # 一時停止する (pause( true ))
    PAUSE = 3
    # 一時停止して、音量を元の値に戻す
    PAUSE_RESET = 4
end
# INTERNALGAMESOUND
module LN::InternalGameSound
    # BGM
    BGM = 0
    # BGS
    BGS = 1
    # ME
    ME = 2
end
# BACKBUFFERRESIZEMODE
module LN::BackbufferResizeMode
    # バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
    SCALING = 0
    # バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
    SCALING_WITH_LETTER_BOX = 1
    # バックバッファをウィンドウサイズに合わせてリサイズする
    RESIZE = 2
end
# TEXTUREFORMAT
module LN::TextureFormat
    # 不明なフォーマット
    UNKNOWN = 0
    # 32 ビットのアルファ付きフォーマット
    A8R8G8B8 = 1
    # 32 ビットのアルファ無しフォーマット
    X8R8G8B8 = 2
    # 64 ビットの浮動小数点フォーマット
    A16B16G16R16F = 3
    # 128 ビットの浮動小数点フォーマット
    A32B32G32R32F = 4
    # 32 ビットの Z バッファフォーマット
    D24S8 = 5
end
# BLENDMODE
module LN::BlendMode
    # 通常 (アルファ無視)
    NORMAL = 0
    # アルファブレンド (デフォルト)
    ALPHA = 1
    # 加算
    ADD = 2
    # 加算（アルファチャンネル無視）
    ADD_ALPHA_DISABLE = 3
    # 減算
    SUB = 4
    # 減算（アルファチャンネル無視）
    SUB_ALPHA_DISABLE = 5
    # 乗算（アルファチャンネル無視）
    MUL = 6
    # スクリーン（アルファチャンネル無視）
    SCREEN = 7
    # リバース
    REVERSE = 8
end
# CULLINGMODE
module LN::CullingMode
    # 前面表示
    FRONT = 0
    # 裏面表示
    BACK = 1
    # 両面表示
    DOUBLE = 2
end
# TEXTALIGN
module LN::TextAlign
    # 左揃え
    LEFT = 0
    # 中央揃え
    CENTER = 1
    # 右揃え
    RIGHT = 2
end
# PLACEMENTTYPE
module LN::PlacementType
    # 通常の配置方法
    NORMAL = 0
    # 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
    BILLBOARD = 1
    # Y 軸固定
    Y_FIXED = 2
end
# FRONTAXIS
module LN::FrontAxis
    # X+ 方向 (右向き)
    X = 0
    # Y+ 方向 (上向き)
    Y = 1
    # Z+ 方向 (奥向き)
    Z = 2
    # X- 方向 (左向き)
    RX = 3
    # Y- 方向 (下向き)
    RY = 4
    # Z- 方向 (手前向き)
    RZ = 5
end
# MESHCREATEFLAGS
module LN::MeshCreateFlags
    # ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
    SOFTWARE_SKINNING = 0x01
    # 物理演算のメッシュボディとして扱うメッシュを生成する
    COLLISION_MESH = 0x02
    # マテリアルに含まれるテクスチャファイルが見つからなくても無視する
    IGNORE_TEXTURE_NOT_FOUND = 0x04
end
# CAMERATYPE
module LN::CameraType
    # 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
    POSITION_AND_LOOKAT = 0
    # 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
    POSITION_AND_ANGLE = 1
    # 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する
    TRANSFORM_MATRIX = 2
end
# COORD
module LN::Coord
    # 2D 空間
    SPACE_2D = 0
    # 3D 空間
    SPACE_3D = 1
end
# OBJECTTYPE
module LN::ObjectType
    # 
    SOUND = 0
end
# オブジェクトの横幅と縦幅の情報
class LN::Size
    # 初期化
    # @overload initialize(width, height)
    #   @param [int] width 幅
    #   @param [int] height 高さ
    def initialize
    end

    # 幅
    # @overload width=(v)
    #   @param [int] v 
    def width=(v)
    end

    # 幅
    # @overload width
    #   @return [int]
    def width
    end

    # 高さ
    # @overload height=(v)
    #   @param [int] v 
    def height=(v)
    end

    # 高さ
    # @overload height
    #   @return [int]
    def height
    end

end
# 矩形を表す
class LN::Rect
    # 初期化
    # @overload initialize(x, y, width, height)
    #   @param [int] x 左上 X 座標
    #   @param [int] y 左上 Y 座標
    #   @param [int] width 幅
    #   @param [int] height 高さ
    def initialize
    end

    # 左上 X 座標
    # @overload x=(v)
    #   @param [int] v 
    def x=(v)
    end

    # 左上 X 座標
    # @overload x
    #   @return [int]
    def x
    end

    # 左上 Y 座標
    # @overload y=(v)
    #   @param [int] v 
    def y=(v)
    end

    # 左上 Y 座標
    # @overload y
    #   @return [int]
    def y
    end

    # 幅
    # @overload width=(v)
    #   @param [int] v 
    def width=(v)
    end

    # 幅
    # @overload width
    #   @return [int]
    def width
    end

    # 高さ
    # @overload height=(v)
    #   @param [int] v 
    def height=(v)
    end

    # 高さ
    # @overload height
    #   @return [int]
    def height
    end

    # 指定した x y 座標の点が矩形に含まれているかを判別する
    # @overload contains(x, y)
    #   @param [Integer] x X 座標
    #   @param [Integer] y Y 座標
    #   @return [Bool] 結果を格納する変数
    def contains
    end
end
# 例外処理
class LN::Exception
    # 例外発生時にコールバックされる関数を登録する
    # @overload set_handler(handler)
    #   @param [] handler 
    def set_handler
    end
    # 例外を発生させる
    # @overload raise(errCode, message, file, line, func)
    #   @param [Integer] errCode 
    #   @param [String] message 
    #   @param [String] file 
    #   @param [Integer] line 
    #   @param [String] func 
    def raise
    end
    # 最後に発生した例外のエラーコードを取得する
    # @overload get_last_error_code()
    def get_last_error_code
    end
    # 最後に発生した例外に対して、デフォルトの処理を行う (メッセージボックスを表示し、LNException_SetHandled(true))
    # @overload proc_default()
    def proc_default
    end
end
# ベースオブジェクト
class LN::ReferenceObject
    # オブジェクトの解放
    # @overload release()
    #   　
    #     指定されたオブジェクトの参照を解放します。(参照カウントをデクリメントします)<br>
    #     Create～という名前の付く関数でオブジェクトを生成した場合は
    #     基本的にこの関数でオブジェクトを解放する必要があります。
    def release
    end
    # オブジェクトの参照カウントをインクリメントする
    # @overload add_ref()
    def add_ref
    end
    # オブジェクトの参照カウント取得する
    # @overload ref_count()
    #   @return [Integer] 参照カウントを格納する変数
    def ref_count
    end
end
# オブジェクトリスト
class LN::ObjectList < ReferenceObject
    # オブジェクトリストに格納されているオブジェクトの数を取得する
    # @overload count()
    #   @return [Integer] 要素の数を格納する変数
    def count
    end
    # オブジェクトリストの指定したインデックスにオブジェクトを設定する
    # @overload set_at(index, item)
    #   @param [Integer] index インデックス(要素番号)
    #   @param [] item 設定するオブジェクト
    def set_at
    end
    # オブジェクトリストの指定したインデックスのオブジェクトを取得する
    # @overload get_at(index)
    #   @param [Integer] index インデックス(要素番号)
    #   @return [] オブジェクトを格納する変数
    def get_at
    end
    # オブジェクトリストの末尾にオブジェクトを追加する
    # @overload add(item)
    #   @param [] item 追加するオブジェクト
    def add
    end
    # オブジェクトリストから全てのオブジェクトを削除する
    # @overload clear()
    def clear
    end
    # オブジェクトリストの指定したインデックスの位置にオブジェクトを挿入する
    # @overload insert(index, item)
    #   @param [Integer] index item を挿入するインデックス
    #   @param [] item 挿入するオブジェクト
    def insert
    end
    # オブジェクトリスト内で指定したハンドルと一致する最初のオブジェクトを削除する
    # @overload remove(item)
    #   @param [] item リストから削除するオブジェクト
    def remove
    end
    # オブジェクトリスト内で指定したハンドルと一致する全てのオブジェクトを削除する
    # @overload remove_all(item)
    #   @param [] item リストから削除するオブジェクト
    def remove_all
    end
    # オブジェクトリストの指定したインデックスにあるオブジェクトを削除する
    # @overload remove_at(index)
    #   @param [Integer] index 削除するオブジェクトのインデックス番号
    def remove_at
    end
end
# 整数型の多次元配列クラス
class LN::IntTable < ReferenceObject
    # 整数型の多次元配列を作成する
    # @overload initialize(xSize, ySize=1, zSize=1)
    #   @param [Integer] xSize X 要素数
    #   @param [Integer] ySize Y 要素数
    #   @param [Integer] zSize Z 要素数
    def initialize
    end
    # 整数型テーブルの指定したインデックスに値を設定する
    # @overload set_value(xSize, ySize, zSize, value)
    #   　
    #     変更前のデータは保持されます。
    #   @param [Integer] xSize X 要素インデックス
    #   @param [Integer] ySize Y 要素インデックス
    #   @param [Integer] zSize Z 要素インデックス
    #   @param [Integer] value 設定する整数値
    def set_value
    end
    # 整数型テーブルの指定したインデックスに値を設定する
    # @overload get_value(xSize, ySize, zSize)
    #   　
    #     変更前のデータは保持されます。
    #   @param [Integer] xSize X 要素インデックス
    #   @param [Integer] ySize Y 要素インデックス
    #   @param [Integer] zSize Z 要素インデックス
    #   @return [Integer] 値を格納する整数型変数
    def get_value
    end
    # 整数型テーブルの要素数を変更する
    # @overload resize(xSize, ySize, zSize)
    #   　
    #     変更前のデータは保持されます。
    #   @param [Integer] xSize X 要素数
    #   @param [Integer] ySize Y 要素数
    #   @param [Integer] zSize Z 要素数
    def resize
    end
    # 整数型テーブルの X 要素数を取得する
    # @overload x_size()
    #   @return [Integer] X 要素数を格納する変数
    def x_size
    end
    # 整数型テーブルの Y 要素数を取得する
    # @overload y_size()
    #   @return [Integer] Y 要素数を格納する変数
    def y_size
    end
    # 整数型テーブルの Z 要素数を取得する
    # @overload z_size()
    #   @return [Integer] Z 要素数を格納する変数
    def z_size
    end
end
# 2D ベクトル
class LN::Vector2
    # 初期化
    # @overload initialize(x, y)
    #   @param [float] x 
    #   @param [float] y 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [float]
    def y
    end

    # 2D ベクトルに値を設定する
    # @overload set(x, y)
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    def set
    end
    # 2D ベクトルの長さを計算する
    # @overload length()
    #   @return [Float] ベクトルの長さを格納する変数
    def length
    end
    # 2D ベクトルの長さの2乗を計算する
    # @overload square_length()
    #   @return [Float] ベクトルの長さの2乗を格納する変数
    def square_length
    end
    # 2D ベクトルを正規化する
    # @overload normalize(vec)
    #   2D ベクトルを正規化する
    #   @param [LN::Vector2] vec 処理の基になる 2D ベクトル
    #   @return [LN::Vector2] 演算結果を格納する 2D ベクトル
    # @overload normalize()
    #   2D ベクトルを正規化する
    #   @return [LN::Vector2] 演算結果を格納する 2D ベクトル
    def normalize
    end
end
# 3Dベクトル
class LN::Vector3
    # 初期化
    # @overload initialize(x, y, z)
    #   @param [float] x 
    #   @param [float] y 
    #   @param [float] z 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [float]
    def z
    end

    # 3Dベクトルに値を設定する
    # @overload set(x, y, z)
    #   3Dベクトルに値を設定する
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    #   @param [Float] z Z 値
    # @overload set(vec2, z)
    #   3Dベクトルに値を設定する
    #   @param [LN::Vector2] vec2 2D ベクトル
    #   @param [Float] z Z 値
    def set
    end
    # 3Dベクトルの長さを計算する
    # @overload length()
    #   @return [Float] ベクトルの長さを格納する変数
    def length
    end
    # 3D ベクトルの長さの2乗を計算する
    # @overload square_length()
    #   @return [Float] ベクトルの長さの2乗を格納する変数
    def square_length
    end
    # 3D ベクトルを正規化する
    # @overload normalize(vec)
    #   3D ベクトルを正規化する
    #   @param [LN::Vector3] vec 処理の基になる 3D ベクトル
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    # @overload normalize()
    #   3D ベクトルを正規化する
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def normalize
    end
    # 2 つの 3D ベクトルの内積を計算する
    # @overload dot(vec1, vec2)
    #   @param [LN::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [LN::Vector3] vec2 処理の基になる 3D ベクトル
    #   @return [Float] ベクトルの内積を格納する変数
    def dot
    end
    # 2 つの 3D ベクトルの外積を計算する
    # @overload cross(vec1, vec2)
    #   @param [LN::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [LN::Vector3] vec2 処理の基になる 3D ベクトル
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def cross
    end
    # 指定された法線で反射させたベクトルを計算する
    # @overload reflect(vec, normal)
    #   @param [LN::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [LN::Vector3] normal 面方向を表す法線 3D ベクトル
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def reflect
    end
    # 指定された法線と方向から滑りベクトルを計算する
    # @overload slide(vec, normal)
    #   @param [LN::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [LN::Vector3] normal 面方向を表す法線 3D ベクトル
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def slide
    end
    # 2 つの 3D ベクトル間を線形補間する
    # @overload lerp(vec1, vec2, t)
    #   @param [LN::Vector3] vec1 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
    #   @param [LN::Vector3] vec2 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
    #   @param [Float] t 補間値 ( 0.0 ～ 1.0 )
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def lerp
    end
    # 指定された 3D ベクトルを Catmull-Rom 補間する
    # @overload catmull_rom(vec1, vec2, vec3, vec4, t)
    #   @param [LN::Vector3] vec1 処理の基になる 3D ベクトル
    #   @param [LN::Vector3] vec2 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
    #   @param [LN::Vector3] vec3 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
    #   @param [LN::Vector3] vec4 処理の基になる 3D ベクトル
    #   @param [Float] t 補間値 ( 0.0 ～ 1.0 )
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def catmull_rom
    end
    # 指定された行列を使用して座標変換する
    # @overload transform(vec, mat)
    #   　
    #     ベクトルを ( x, y, z, 1 ) として座標変換します。
    #     結果の w は出力されません。<br>
    #     結果を w = 1 に射影する ( x y z を w で除算する ) 場合は
    #     Vector3TransformCoord() を使用してください。
    #   @param [LN::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [LN::Matrix] mat 処理の基になる 行列
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def transform
    end
    # 指定された行列を使用して座標変換し、結果を w = 1 に射影する
    # @overload transform_coord(vec, mat)
    #   　
    #     ベクトルを ( x, y, z, 1 ) として座標変換し、
    #     結果を w = 1 に射影 ( x y z を w で除算する ) します。
    #   @param [LN::Vector3] vec 処理の基になる 3D ベクトル
    #   @param [LN::Matrix] mat 処理の基になる 行列
    #   @return [LN::Vector3] 演算結果を格納する 3D ベクトル
    def transform_coord
    end
    # 任意軸周りの回転を行う
    # @overload rotate_axis(vec, axis, r)
    #   @param [LN::Vector3] vec 処理の基になる Vector3
    #   @param [LN::Vector3] axis 回転軸を表す Vector3
    #   @param [Float] r 回転角度 (ラジアン)
    #   @return [LN::Vector3] 演算結果を格納する Vector3
    def rotate_axis
    end
    # 行列の回転成分だけを使って座標変換する
    # @overload rotate_matrix(vec, mat)
    #   @param [LN::Vector3] vec 処理の基になる Vector3
    #   @param [LN::Matrix] mat 処理の基になる Matrix
    #   @return [LN::Vector3] 演算結果を格納する Vector3
    def rotate_matrix
    end
end
# 4Dベクトル
class LN::Vector4
    # 初期化
    # @overload initialize(x, y, z, w)
    #   @param [float] x 
    #   @param [float] y 
    #   @param [float] z 
    #   @param [float] w 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [float]
    def z
    end

    # 
    # @overload w=(v)
    #   @param [float] v 
    def w=(v)
    end

    # 
    # @overload w
    #   @return [float]
    def w
    end

end
# 4x4行列
class LN::Matrix
    # 初期化
    # @overload initialize(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44)
    #   @param [float] m11 
    #   @param [float] m12 
    #   @param [float] m13 
    #   @param [float] m14 
    #   @param [float] m21 
    #   @param [float] m22 
    #   @param [float] m23 
    #   @param [float] m24 
    #   @param [float] m31 
    #   @param [float] m32 
    #   @param [float] m33 
    #   @param [float] m34 
    #   @param [float] m41 
    #   @param [float] m42 
    #   @param [float] m43 
    #   @param [float] m44 
    def initialize
    end

    # 
    # @overload m11=(v)
    #   @param [float] v 
    def m11=(v)
    end

    # 
    # @overload m11
    #   @return [float]
    def m11
    end

    # 
    # @overload m12=(v)
    #   @param [float] v 
    def m12=(v)
    end

    # 
    # @overload m12
    #   @return [float]
    def m12
    end

    # 
    # @overload m13=(v)
    #   @param [float] v 
    def m13=(v)
    end

    # 
    # @overload m13
    #   @return [float]
    def m13
    end

    # 
    # @overload m14=(v)
    #   @param [float] v 
    def m14=(v)
    end

    # 
    # @overload m14
    #   @return [float]
    def m14
    end

    # 
    # @overload m21=(v)
    #   @param [float] v 
    def m21=(v)
    end

    # 
    # @overload m21
    #   @return [float]
    def m21
    end

    # 
    # @overload m22=(v)
    #   @param [float] v 
    def m22=(v)
    end

    # 
    # @overload m22
    #   @return [float]
    def m22
    end

    # 
    # @overload m23=(v)
    #   @param [float] v 
    def m23=(v)
    end

    # 
    # @overload m23
    #   @return [float]
    def m23
    end

    # 
    # @overload m24=(v)
    #   @param [float] v 
    def m24=(v)
    end

    # 
    # @overload m24
    #   @return [float]
    def m24
    end

    # 
    # @overload m31=(v)
    #   @param [float] v 
    def m31=(v)
    end

    # 
    # @overload m31
    #   @return [float]
    def m31
    end

    # 
    # @overload m32=(v)
    #   @param [float] v 
    def m32=(v)
    end

    # 
    # @overload m32
    #   @return [float]
    def m32
    end

    # 
    # @overload m33=(v)
    #   @param [float] v 
    def m33=(v)
    end

    # 
    # @overload m33
    #   @return [float]
    def m33
    end

    # 
    # @overload m34=(v)
    #   @param [float] v 
    def m34=(v)
    end

    # 
    # @overload m34
    #   @return [float]
    def m34
    end

    # 
    # @overload m41=(v)
    #   @param [float] v 
    def m41=(v)
    end

    # 
    # @overload m41
    #   @return [float]
    def m41
    end

    # 
    # @overload m42=(v)
    #   @param [float] v 
    def m42=(v)
    end

    # 
    # @overload m42
    #   @return [float]
    def m42
    end

    # 
    # @overload m43=(v)
    #   @param [float] v 
    def m43=(v)
    end

    # 
    # @overload m43
    #   @return [float]
    def m43
    end

    # 
    # @overload m44=(v)
    #   @param [float] v 
    def m44=(v)
    end

    # 
    # @overload m44
    #   @return [float]
    def m44
    end

    # 単位行列を作成する
    # @overload tity()
    #   @return [LN::Matrix] 結果を格納する行列
    def tity
    end
    # 行列を平行移動する
    # @overload translate(x, y, z)
    #   行列を平行移動する
    #     与えられた引数から平行移動行列を作り、現在の行列に乗算します。
    #   @param [Float] x X 方向の移動量
    #   @param [Float] y Y 方向の移動量
    #   @param [Float] z Z 方向の移動量
    # @overload translate(vec)
    #   行列を平行移動する
    #     与えられた引数から平行移動行列を作り、現在の行列に乗算します。
    #   @param [LN::Vector3] vec 移動量
    def translate
    end
    # X 軸を回転軸にして行列を回転する ( ラジアン単位 )
    # @overload rotate_x(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度 ( 軸方向に対して反時計回り )
    def rotate_x
    end
    # Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
    # @overload rotate_y(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度
    def rotate_y
    end
    # Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
    # @overload rotate_z(radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] radian 回転角度
    def rotate_z
    end
    # 行列を回転する ( ラジアン単位 )
    # @overload rotate(xRad, yRad, zRad, rotOrder=XYZ)
    #   行列を回転する ( ラジアン単位 )
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [Float] xRad X 軸を回転軸とした角度
    #   @param [Float] yRad Y 軸を回転軸とした角度
    #   @param [Float] zRad Z 軸を回転軸とした角度
    #   @param [LN::RotationOrder] rotOrder 回転順序の指定
    # @overload rotate(vec, rotOrder=XYZ)
    #   行列を回転する ( ラジアン単位 )
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。
    #   @param [LN::Vector3] vec 各軸の回転角度
    #   @param [LN::RotationOrder] rotOrder 回転順序の指定
    def rotate
    end
    # 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
    # @overload rotate_axis(axis, radian)
    #   　
    #     与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
    #     与えられた軸が正規化されていない場合は正規化したものを使って計算します。
    #   @param [LN::Vector3] axis 回転軸を示す Vector3
    #   @param [Float] radian 回転角度
    def rotate_axis
    end
    # クォータニオンを使って行列を回転する
    # @overload rotate_quaternion(qua)
    #   　
    #     クォータニオンから回転行列を作り、現在の行列に乗算します。
    #   @param [LN::Quaternion] qua 処理の基になる Quaternion
    def rotate_quaternion
    end
    # 行列をスケーリングする
    # @overload scale(xyz)
    #   行列をスケーリングする
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
    #   @param [Float] xyz X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)
    # @overload scale(x, y, z)
    #   行列をスケーリングする
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。
    #   @param [Float] x X 方向の拡大率
    #   @param [Float] y Y 方向の拡大率
    #   @param [Float] z Z 方向の拡大率
    # @overload scale(scale)
    #   行列をスケーリングする
    #     与えられた引数からスケーリング行列を作り、現在の行列に乗算します。<br>
    #   @param [LN::Vector3] scale 各方向の拡大率
    def scale
    end
    # 行列を乗算する
    # @overload multiply(mat1, mat2)
    #   @param [LN::Matrix] mat1 処理の基になる行列
    #   @param [LN::Matrix] mat2 処理の基になる行列
    #   @return [LN::Matrix] 結果を格納する行列
    def multiply
    end
    # 逆行列を求める
    # @overload inverse(mat)
    #   逆行列を求める
    #   @param [LN::Matrix] mat 処理の基になる行列
    #   @return [LN::Matrix] 演算結果を格納する行列
    # @overload inverse()
    #   逆行列を求める
    #   @return [LN::Matrix] 演算結果を格納する行列
    def inverse
    end
    # 転置行列を求める
    # @overload transpose(mat)
    #   転置行列を求める
    #   @param [LN::Matrix] mat 処理の基になる行列
    #   @return [LN::Matrix] 演算結果を格納する行列
    # @overload transpose()
    #   転置行列を求める
    #   @return [LN::Matrix] 演算結果を格納する行列
    def transpose
    end
    # 左手座標系ビュー行列を作成する
    # @overload view_transform_lh(pos, lookAt, upDir)
    #   @param [LN::Vector3] pos 視点の位置を示す Vector3
    #   @param [LN::Vector3] lookAt 注視点を示す Vector3
    #   @param [LN::Vector3] upDir 上方向を示す Vector3
    #   @return [LN::Matrix] 演算結果を格納する行列
    def view_transform_lh
    end
    # 右手座標系ビュー行列を作成する
    # @overload view_transform_rh(pos, lookAt, upDir)
    #   @param [LN::Vector3] pos 視点の位置を示す Vector3
    #   @param [LN::Vector3] lookAt 注視点を示す Vector3
    #   @param [LN::Vector3] upDir 上方向を示す Vector3
    #   @return [LN::Matrix] 演算結果を格納する行列
    def view_transform_rh
    end
    # 左手座標系射影行列の作成
    # @overload perspective_fov_lh(fovY, aspect, nearZ, farZ)
    #   @param [Float] fovY Y 方向への視野角 (ラジアン単位)
    #   @param [Float] aspect アスペクト比
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    #   @return [LN::Matrix] 演算結果を格納する行列
    def perspective_fov_lh
    end
    # 右手座標系射影行列の作成
    # @overload perspective_fov_rh(fovY, aspect, nearZ, farZ)
    #   @param [Float] fovY Y 方向への視野角 (ラジアン単位)
    #   @param [Float] aspect アスペクト比
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    #   @return [LN::Matrix] 演算結果を格納する行列
    def perspective_fov_rh
    end
    # 左手座標系正射影行列の作成
    # @overload ortho_lh(width, height, nearZ, farZ)
    #   @param [Float] width ビューの幅
    #   @param [Float] height ビューの高さ
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    #   @return [LN::Matrix] 演算結果を格納する行列
    def ortho_lh
    end
    # 右手座標系正射影行列の作成
    # @overload ortho_rh(width, height, nearZ, farZ)
    #   @param [Float] width ビューの幅
    #   @param [Float] height ビューの高さ
    #   @param [Float] nearZ 近くのビュー平面の Z 値
    #   @param [Float] farZ 遠くのビュー平面の Z 値
    #   @return [LN::Matrix] 演算結果を格納する行列
    def ortho_rh
    end
    # 右方向を示す 3D ベクトルの取得
    # @overload right()
    #   @return [LN::Vector3] 結果を格納する Vector3 変数
    def right
    end
    # 上方向を示す 3D ベクトルの取得
    # @overload up()
    #   @return [LN::Vector3] 結果を格納する Vector3 変数
    def up
    end
    # 正面方向を示す 3D ベクトルの取得
    # @overload front()
    #   @return [LN::Vector3] 結果を格納する Vector3 変数
    def front
    end
    # 位置を示す 3D ベクトルの取得
    # @overload position()
    #   @return [LN::Vector3] 結果を格納する Vector3 変数
    def position
    end
    # 回転行列からオイラー角を計算する
    # @overload to_euler(mat)
    #   回転行列からオイラー角を計算する
    #   @param [LN::Matrix] mat 処理の基になる行列
    #   @return [LN::Vector3] 各軸の回転角度(ラジアン)を格納する Vector3 変数
    # @overload to_euler()
    #   回転行列からオイラー角を計算する
    #   @return [LN::Vector3] 各軸の回転角度(ラジアン)を格納する Vector3 変数
    def to_euler
    end
    # 右手系⇔左手系の変換
    # @overload transform_basis(mat)
    #   右手系⇔左手系の変換
    #   @param [LN::Matrix] mat 処理の元になる行列
    #   @return [LN::Matrix] 演算結果を格納する行列
    # @overload transform_basis()
    #   右手系⇔左手系の変換
    #   @return [LN::Matrix] 演算結果を格納する行列
    def transform_basis
    end
end
# クォータニオン
class LN::Quaternion
    # 初期化
    # @overload initialize(x, y, z, w)
    #   @param [float] x 
    #   @param [float] y 
    #   @param [float] z 
    #   @param [float] w 
    def initialize
    end

    # 
    # @overload x=(v)
    #   @param [float] v 
    def x=(v)
    end

    # 
    # @overload x
    #   @return [float]
    def x
    end

    # 
    # @overload y=(v)
    #   @param [float] v 
    def y=(v)
    end

    # 
    # @overload y
    #   @return [float]
    def y
    end

    # 
    # @overload z=(v)
    #   @param [float] v 
    def z=(v)
    end

    # 
    # @overload z
    #   @return [float]
    def z
    end

    # 
    # @overload w=(v)
    #   @param [float] v 
    def w=(v)
    end

    # 
    # @overload w
    #   @return [float]
    def w
    end

    # クォータニオンに値を設定する
    # @overload set(x, y, z, w)
    #   @param [Float] x X 値
    #   @param [Float] y Y 値
    #   @param [Float] z Z 値
    #   @param [Float] w W 値
    def set
    end
    # 単位クォータニオンを作成する
    # @overload tity()
    #   @return [LN::Quaternion] 結果を格納する Quaternion 変数
    def tity
    end
    # 任意の軸を回転軸にして回転するクォータニオンを作成する
    # @overload rotation_axis(axis, r)
    #   　
    #     与えられた軸が正規化されていない場合は正規化された軸を使用します。
    #   @param [LN::Vector3] axis 回転軸を示す Vector3 変数
    #   @param [Float] r 回転角度 (ラジアン)
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_axis
    end
    # 回転行列からクォータニオンを作成する
    # @overload rotation_matrix(mat)
    #   @param [LN::Matrix] mat 処理の基になる行列
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_matrix
    end
    # ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
    # @overload rotation_yaw_pitch_roll(yaw, pitch, roll)
    #   @param [Float] yaw y 軸を中心とするヨー (ラジアン単位)
    #   @param [Float] pitch x 軸を中心とするピッチ (ラジアン単位)
    #   @param [Float] roll z 軸を中心とするロール (ラジアン単位)
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def rotation_yaw_pitch_roll
    end
    # クォータニオンを正規化する
    # @overload normalize(qua)
    #   クォータニオンを正規化する
    #   @param [LN::Quaternion] qua 処理の基になるクォータニオン
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    # @overload normalize()
    #   クォータニオンを正規化する
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def normalize
    end
    # クォータニオンの共役を計算する
    # @overload conjugate(qua)
    #   クォータニオンの共役を計算する
    #   @param [LN::Quaternion] qua 処理の基になるクォータニオン
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    # @overload conjugate()
    #   クォータニオンの共役を計算する
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def conjugate
    end
    # 2 つのクォータニオンの積を計算する
    # @overload multiply(qua1, qua2)
    #   @param [LN::Quaternion] qua1 処理の基になるクォータニオン
    #   @param [LN::Quaternion] qua2 処理の基になるクォータニオン
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def multiply
    end
    # 2 つのクォータニオンを球面線形補間する
    # @overload slerp(qua1, qua2, t)
    #   @param [LN::Quaternion] qua1 処理の基になるクォータニオン
    #   @param [LN::Quaternion] qua2 処理の基になるクォータニオン
    #   @param [Float] t 補間値
    #   @return [LN::Quaternion] 演算結果を格納する Quaternion 変数
    def slerp
    end
end
# 長周期乱数を高速で生成する乱数クラス
class LN::Random < ReferenceObject
    # 現在の時間を乱数シードとして、乱数オブジェクトを作成する
    # @overload initialize()
    #   現在の時間を乱数シードとして、乱数オブジェクトを作成する
    # @overload initialize(seed)
    #   乱数シードを指定し、乱数オブジェクトを作成する
    #   @param [Integer] seed シード値
    def initialize
    end
    # 乱数オブジェクトに乱数シードを設定する
    # @overload seed=(seed)
    #   　
    #     random が NULL の場合、グローバル乱数オブジェクトに設定します。
    #   @param [Integer] seed シード値
    def seed=
    end
    # 整数値乱数の取得
    # @overload get_int()
    #   　
    #     random が NULL の場合、グローバル乱数オブジェクトから取得します。
    #   @return [Integer] 整数値を格納する変数
    def get_int
    end
    # 範囲を指定して、整数値乱数を取得する ( minValue <= x < maxValue )
    # @overload get_int_range(minValue, maxValue)
    #   　
    #     random が NULL の場合、グローバル乱数オブジェクトから取得します。
    #   @param [Integer] minValue 生成される乱数の下限値
    #   @param [Integer] maxValue 生成される乱数の上限値
    #   @return [Integer] 整数値を格納する変数
    def get_int_range
    end
    # 範囲を指定して、整数値乱数を取得する (center ± width/2)
    # @overload get_int_deflect(center, width)
    #   　
    #     random が NULL の場合、グローバル乱数オブジェクトから取得します。
    #   @param [Integer] center 生成される乱数の中央値
    #   @param [Integer] width 生成される乱数の振れ幅
    #   @return [Integer] 整数値を格納する変数
    def get_int_deflect
    end
end
# ディレクトリ上のファイルまたは暗号化アーカイブ内のファイルにアクセスするためのモジュールです
class LN::FileStream < ReferenceObject
    # ファイルを開く
    # @overload initialize(filePath, mode=OPEN, access=READ)
    #   　
    #     ファイルの読み込みが終了したら LNFileStream_Close または LNObject_Release でファイルを閉じる必要があります。
    #   @param [String] filePath ファイル名
    #   @param [LN::FileMode] mode ファイルを開く方法または作成する方法
    #   @param [LN::FileAccess] access ファイルにアクセスする方法
    def initialize
    end
    # ファイルを閉じる
    # @overload close()
    def close
    end
    # ファイルサイズを取得する
    # @overload size()
    #   @return [Integer] ファイルサイズ (バイト単位) を格納する変数
    def size
    end
    # ファイルポインタの位置を取得する
    # @overload position()
    #   @return [Integer] ファイルポインタの位置 (バイト単位) を格納する変数
    def position
    end
    # ファイルポインタの位置を設定する
    # @overload seek(offset, origin)
    #   @param [Integer] offset 読み込んだデータを格納するバッファ
    #   @param [LN::SeekOrigin] origin 起点
    def seek
    end
    # ファイルの存在を調べる
    # @overload exists(filePath)
    #   　
    #     ディレクトリと全ての暗号化アーカイブを対象として確認します。
    #   @param [String] filePath ファイルパス
    #   @return [Bool] 結果を格納する変数 (LN_TRUE = 存在する / LN_FALSE = 存在しない)
    def exists
    end
end
# 入力処理
class LN::Input
    # ボタンが現在押されているかを判定する
    # @overload press?(button)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def press?
    end
    # 仮想ゲームコントローラのボタンが新しく押されたかを判定する
    # @overload trigger?(button)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def trigger?
    end
    # 仮想ゲームコントローラのボタンが離されたかを判定する
    # @overload off_trigger?(button)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def off_trigger?
    end
    # 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
    # @overload repeat?(button)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def repeat?
    end
    # 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
    # @overload get_power(button)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Float] アナログ値を格納する変数
    def get_power
    end
    # 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
    # @overload get_axis_state(axis)
    #   　
    #     0 番の仮想ゲームコントローラ(=1Pコントローラ)が返す状態と同じ状態を返すユーティリティです。
    #   @param [Integer] axis 軸番号 (0～1)
    #   @return [LN::Vector2] 軸の状態を格納する2Dベクトル
    def get_axis_state
    end
end
# 仮想ゲームコントローラ
class LN::VirtualPad < ReferenceObject
    # 仮想ゲームコントローラハンドルを取得する
    # @overload get_virtual_pad(index)
    #   @param [Integer] index 取得する仮想ゲームコントローラの番号 (0～3)
    #   @return [LN::VirtualPad] 仮想ゲームコントローラハンドルを格納する変数
    def get_virtual_pad
    end
    # 仮想ゲームコントローラのボタンが現在押されているかを判定する
    # @overload press?(button)
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def press?
    end
    # 仮想ゲームコントローラのボタンが新しく押されたかを判定する
    # @overload trigger?(button)
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def trigger?
    end
    # 仮想ゲームコントローラのボタンが離されたかを判定する
    # @overload off_trigger?(button)
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def off_trigger?
    end
    # 仮想ゲームコントローラのボタンが新しく押された瞬間とリピート状態を判定する
    # @overload repeat?(button)
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Bool] 状態を格納する変数
    def repeat?
    end
    # 仮想ゲームコントローラの指定ボタンの入力をアナログ値(0.0～1.0)で取得する
    # @overload get_power(button)
    #   @param [Integer] button 仮想ボタン番号 (0～19)
    #   @return [Float] アナログ値を格納する変数
    def get_power
    end
    # 仮想ゲームコントローラの軸の状態(スティックの左右と上下)の取得
    # @overload get_axis_state(virtualPad, axis)
    #   @param [LN::VirtualPad] virtualPad 仮想ゲームコントローラハンドル
    #   @param [Integer] axis 軸番号 (0～1)
    #   @return [LN::Vector2] 軸の状態を格納する2Dベクトル
    def get_axis_state
    end
    # 仮想ゲームコントローラの新しく押されたデバイスボタン番号の取得
    # @overload pressed_device_button()
    #   　
    #     キーボード、マウス、ジョイパッド(スティックを含む)の
    #     何らかのボタンが押された場合、そのボタンを識別するための
    #     番号を返します。(デバイスボタン)<br>
    #     この番号は主にキーコンフィグで使用します。<br>
    #     この関数は isTrigger() 同様に動作し、ボタンが押された瞬間だけ
    #     ボタン番号を返します。<br>
    #     それ以外は 0 を返します。
    #   @return [Integer] デバイスボタン番号を格納する構造体
    def pressed_device_button
    end
    # デバイスボタン番号に対応するデバイスの種類を取得する
    # @overload get_device_button_type(deviceButton)
    #   @param [Integer] deviceButton デバイスボタン番号
    #   @return [LN::DeviceButtonType] デバイスボタンの種類を格納する構造体
    def get_device_button_type
    end
    # 仮想ゲームコントローラのボタンにデバイスボタンを割り当てる
    # @overload attach_virtual_button(virtualButton, deviceButton)
    #   　
    #     この関数はキーコンフィグ用の関数です。<br>
    #     デバイスボタン(キーボードのキーや、ジョイパッドのボタン)を仮想ボタンに割り当てます。<br>
    #     <br>
    #     基本的に LNVirtualPad_GetPressedDeviceButton() とセットで使い、LNVirtualPad_GetDeviceButtonType()から
    #     受け取ったデバイスボタン番号をこの関数の deviceButton に渡します。<br>
    #   @param [Integer] virtualButton 割り当て先の仮想ボタン番号
    #   @param [Integer] deviceButton 割り当てるデバイスボタン番号
    def attach_virtual_button
    end
    # 仮想ゲームコントローラのデバイスボタンの割り当てを解除する
    # @overload detach_virtual_button(virtualButton, deviceButton)
    #   　
    #     解除対象の仮想ボタンに deviceButton で示されるデバイスボタンが割り当てられている場合、解除します。<br>
    #     割り当てられていない場合は何もしません。<br>
    #     deviceButton に 0 を渡すと、virtualButton の全ての割り当てを解除します。
    #   @param [Integer] virtualButton 解除対象の仮想ボタン番号
    #   @param [Integer] deviceButton 解除するデバイスボタン番号
    def detach_virtual_button
    end
end
# 音声管理及びゲームオーディオ機能
class LN::Audio
    # BGM を演奏する
    # @overload play_bgm(filePath, volume=100, pitch=100, fadeTime=0)
    #   @param [String] filePath ファイル名
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    #   @param [Integer] fadeTime フェードインにかける時間 (ミリ秒)
    def play_bgm
    end
    # BGM の演奏を停止する
    # @overload stop_bgm(fadeTime=0)
    #   @param [Integer] fadeTime フェードアウトにかける時間 (ミリ秒)
    def stop_bgm
    end
    # BGS を演奏する
    # @overload play_bgs(filePath, volume=100, pitch=100, fadeTime=0)
    #   @param [String] filePath ファイル名
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    #   @param [Integer] fadeTime フェードインにかける時間 (ミリ秒)
    def play_bgs
    end
    # BGS の演奏を停止する
    # @overload stop_bgs(fadeTime=0)
    #   @param [Integer] fadeTime フェードアウトにかける時間 (ミリ秒)
    def stop_bgs
    end
    # ME を演奏する
    # @overload play_me(filePath, volume=100, pitch=100)
    #   @param [String] filePath ファイル名
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    def play_me
    end
    # ME の演奏を停止する
    # @overload stop_me()
    def stop_me
    end
    # SE を演奏する
    # @overload play_se(filePath, volume=100, pitch=100)
    #   @param [String] filePath ファイル名
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    def play_se
    end
    # SE を演奏する (3D サウンド)
    # @overload play_se3d(filePath, position, distance, volume=100, pitch=100)
    #   SE を演奏する (3D サウンド)
    #   @param [String] filePath ファイル名
    #   @param [LN::Vector3] position 3D 空間上の座標
    #   @param [Float] distance 減衰距離
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    # @overload play_se3d(filePath, x, y, z, distance, volume=100, pitch=100)
    #   SE を演奏する (3D サウンド)
    #   @param [String] filePath ファイル名
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    #   @param [Float] distance 減衰距離
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] pitch ピッチ (50 ～ 200)
    def play_se3d
    end
    # すべての SE の演奏を停止する
    # @overload stop_se()
    def stop_se
    end
    # 3D 空間の1メートル相当の距離の設定
    # @overload set_metre_unit_distance(distance)
    #   @param [Float] distance 距離
    def set_metre_unit_distance
    end
    # 再生中のBGMの音量を設定する (フェードアウト中は無効)
    # @overload set_bgm_volume(volume, fadeTime=0)
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] fadeTime フェードアウトにかける時間 (ミリ秒)
    def set_bgm_volume
    end
    # 再生中のBGSの音量を設定する (フェードアウト中は無効)
    # @overload set_bgs_volume(volume, fadeTime=0)
    #   @param [Integer] volume ボリューム (0 ～ 100)
    #   @param [Integer] fadeTime フェードアウトにかける時間 (ミリ秒)
    def set_bgs_volume
    end
    # [使用非推奨] BGM、BGS、ME として再生されているサウンドオブジェクトの取得
    # @overload get_internal_game_sound(type)
    #   　
    #     この関数は、BGM、BGS、ME の音量、ピッチの変更、及び
    #     再生位置の取得のために用意されています。
    #     この関数から取得したサウンドハンドルに対して
    #     Release、Play、Stop、Pause を行わないでください。
    #   @param [LN::InternalGameSound] type 取得するサウンドの種類
    def get_internal_game_sound
    end
end
# 3D音声リスナー
class LN::SoundListener
    # リスナーの位置の設定
    # @overload set_position(position)
    #   リスナーの位置の設定
    #   @param [LN::Vector3] position 3D 空間上の座標
    # @overload set_position(x, y, z)
    #   リスナーの位置の設定
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    def set_position
    end
    # リスナーの正面方向の設定
    # @overload set_direction(direction)
    #   リスナーの正面方向の設定
    #   @param [LN::Vector3] direction 向き
    # @overload set_direction(x, y, z)
    #   リスナーの正面方向の設定
    #   @param [Float] x 向きの X 成分
    #   @param [Float] y 向きの Y 成分
    #   @param [Float] z 向きの Z 成分
    def set_direction
    end
    # リスナーの上方向の設定 (正面方向とは直交であること)
    # @overload set_up_direction(direction)
    #   リスナーの上方向の設定 (正面方向とは直交であること)
    #   @param [LN::Vector3] direction 上方向
    # @overload set_up_direction(x, y, z)
    #   リスナーの上方向の設定 (正面方向とは直交であること)
    #   @param [Float] x 向きの X 成分
    #   @param [Float] y 向きの Y 成分
    #   @param [Float] z 向きの Z 成分
    def set_up_direction
    end
    # リスナーの速度の設定
    # @overload velocity(velocity)
    #   リスナーの速度の設定
    #   @param [LN::Vector3] velocity 速度
    # @overload velocity(x, y, z)
    #   リスナーの速度の設定
    #   @param [Float] x 速度の X 成分
    #   @param [Float] y 速度の Y 成分
    #   @param [Float] z 速度の Z 成分
    def velocity
    end
end
# 音声機能
class LN::Sound < ReferenceObject
    # ファイルから音声を作成する
    # @overload initialize(filePath, enable3d=false, playType=AUTO)
    #   @param [String] filePath ファイル名
    #   @param [Bool] enable3d true の場合、3D 音源として作成する
    #   @param [LN::SoundPlayingType] playType 音声の再生方法
    def initialize
    end
    # ボリュームの設定
    # @overload volume=(volume)
    #   @param [Integer] volume ボリューム (0～100)
    def volume=
    end
    # ボリュームの取得
    # @overload volume()
    #   @return [Integer] ボリュームを格納する変数
    def volume
    end
    # ピッチの設定
    # @overload pitch=(pitch)
    #   @param [Integer] pitch ピッチ (50～200)
    def pitch=
    end
    # ピッチの取得
    # @overload pitch()
    #   @return [Integer] ピッチを格納する変数
    def pitch
    end
    # ループ再生の有効設定
    # @overload set_loop_state(loopEnable, begin=0, length=0)
    #   　
    #     begin と length に 0 を指定すると、全体をループ領域として設定します。
    #   @param [Bool] loopEnable LN_TRUE = ループ再生する / LN_FALSE = しない
    #   @param [Integer] begin ループ領域の先頭位置 (サンプル数単位)
    #   @param [Integer] length ループ領域長さ (サンプル数単位)
    def set_loop_state
    end
    # ループ再生が有効かを調べる
    # @overload loop?()
    #   @return [Bool] ループ再生の有無状態を格納する変数
    def loop?
    end
    # サウンドの再生状態を取得する
    # @overload play_state()
    #   @return [LN::SoundPlayingState] 状態を格納する変数
    def play_state
    end
    # 音声を再生する
    # @overload play()
    def play
    end
    # 再生を停止する
    # @overload stop()
    def stop
    end
    # 一時停止
    # @overload pause(pause)
    #   @param [Bool] pause LN_TRUE = 一時停止 / LN_FALSE = 一時停止解除
    def pause
    end
    # 音量のフェード
    # @overload fade_volume(targetVolume, time, fadeState)
    #   @param [Integer] targetVolume 変更先の音量
    #   @param [Integer] time フェードにかける時間 (ミリ秒)
    #   @param [LN::SoundFadeState] fadeState フェード完了後の動作の指定
    def fade_volume
    end
    # 再生したサンプル数の取得
    # @overload units_played()
    #   @return [Integer] 再生したサンプル数を格納する変数
    def units_played
    end
    # 音声データ全体のサンプル数の取得
    # @overload total_units()
    #   @return [Integer] 音声データ全体のサンプル数を格納する変数
    def total_units
    end
    # サンプリング周波数の取得
    # @overload units_par_second()
    #   @return [Integer] サンプリング周波数を格納する変数
    def units_par_second
    end
    # 3D 音源かを調べる
    # @overload is_3d_sound?()
    #   @return [Bool] 3D 音源かを示す値を格納する変数
    def is_3d_sound?
    end
    # 3D 音源としての位置を設定する
    # @overload emitter_position=(position)
    #   3D 音源としての位置を設定する
    #   @param [LN::Vector3] position 3D 空間上の座標
    # @overload emitter_position=(x, y, z)
    #   3D 音源としての位置を設定する
    #   @param [Float] x 3D 空間上の X 座標
    #   @param [Float] y 3D 空間上の Y 座標
    #   @param [Float] z 3D 空間上の Z 座標
    def emitter_position=
    end
    # 3D 音源としての速度を設定する
    # @overload emitter_velocity=(velocity)
    #   3D 音源としての速度を設定する
    #   @param [LN::Vector3] velocity 速度
    # @overload emitter_velocity=(x, y, z)
    #   3D 音源としての速度を設定する
    #   @param [Float] x 速度の X 成分
    #   @param [Float] y 速度の Y 成分
    #   @param [Float] z 速度の Z 成分
    def emitter_velocity=
    end
    # 3D 音源の減衰距離 (聴こえなくなる距離) を設定する
    # @overload emitter_distance=(distance)
    #   @param [Float] distance 距離
    def emitter_distance=
    end
end
# 色
class LN::Color
    # 初期化
    # @overload initialize(r, g, b, a)
    #   @param [lnU8] r 赤成分 (0～255)
    #   @param [lnU8] g 緑成分 (0～255)
    #   @param [lnU8] b 青成分 (0～255)
    #   @param [lnU8] a アルファ成分 (0～255)
    def initialize
    end

    # 赤成分 (0～255)
    # @overload r=(v)
    #   @param [lnU8] v 
    def r=(v)
    end

    # 赤成分 (0～255)
    # @overload r
    #   @return [lnU8]
    def r
    end

    # 緑成分 (0～255)
    # @overload g=(v)
    #   @param [lnU8] v 
    def g=(v)
    end

    # 緑成分 (0～255)
    # @overload g
    #   @return [lnU8]
    def g
    end

    # 青成分 (0～255)
    # @overload b=(v)
    #   @param [lnU8] v 
    def b=(v)
    end

    # 青成分 (0～255)
    # @overload b
    #   @return [lnU8]
    def b
    end

    # アルファ成分 (0～255)
    # @overload a=(v)
    #   @param [lnU8] v 
    def a=(v)
    end

    # アルファ成分 (0～255)
    # @overload a
    #   @return [lnU8]
    def a
    end

end
# //
class LN::ColorF
    # 初期化
    # @overload initialize(r, g, b, a)
    #   @param [float] r 赤成分 (0.0～1.0)
    #   @param [float] g 緑成分 (0.0～1.0)
    #   @param [float] b 青成分 (0.0～1.0)
    #   @param [float] a アルファ成分 (0.0～1.0)
    def initialize
    end

    # 赤成分 (0.0～1.0)
    # @overload r=(v)
    #   @param [float] v 
    def r=(v)
    end

    # 赤成分 (0.0～1.0)
    # @overload r
    #   @return [float]
    def r
    end

    # 緑成分 (0.0～1.0)
    # @overload g=(v)
    #   @param [float] v 
    def g=(v)
    end

    # 緑成分 (0.0～1.0)
    # @overload g
    #   @return [float]
    def g
    end

    # 青成分 (0.0～1.0)
    # @overload b=(v)
    #   @param [float] v 
    def b=(v)
    end

    # 青成分 (0.0～1.0)
    # @overload b
    #   @return [float]
    def b
    end

    # アルファ成分 (0.0～1.0)
    # @overload a=(v)
    #   @param [float] v 
    def a=(v)
    end

    # アルファ成分 (0.0～1.0)
    # @overload a
    #   @return [float]
    def a
    end

end
# //
class LN::Tone
    # 初期化
    # @overload initialize(r, g, b, mono)
    #   @param [float] r 赤成分 (-1.0～1.0)
    #   @param [float] g 緑成分 (-1.0～1.0)
    #   @param [float] b 青成分 (-1.0～1.0)
    #   @param [float] mono モノクロ成分 (0.0～1.0)
    def initialize
    end

    # 赤成分 (-1.0～1.0)
    # @overload r=(v)
    #   @param [float] v 
    def r=(v)
    end

    # 赤成分 (-1.0～1.0)
    # @overload r
    #   @return [float]
    def r
    end

    # 緑成分 (-1.0～1.0)
    # @overload g=(v)
    #   @param [float] v 
    def g=(v)
    end

    # 緑成分 (-1.0～1.0)
    # @overload g
    #   @return [float]
    def g
    end

    # 青成分 (-1.0～1.0)
    # @overload b=(v)
    #   @param [float] v 
    def b=(v)
    end

    # 青成分 (-1.0～1.0)
    # @overload b
    #   @return [float]
    def b
    end

    # モノクロ成分 (0.0～1.0)
    # @overload mono=(v)
    #   @param [float] v 
    def mono=(v)
    end

    # モノクロ成分 (0.0～1.0)
    # @overload mono
    #   @return [float]
    def mono
    end

end
# テクスチャ
class LN::Texture < ReferenceObject
    # テクスチャの作成
    # @overload initialize(width, height)
    #   テクスチャの作成
    #     ミップマップレベル 1、フォーマット A8R8B8G8(透明色付フォーマット) で作成します。
    #   @param [Integer] width 幅 (ピクセル数)
    #   @param [Integer] height 高さ (ピクセル数)
    # @overload initialize(filePath)
    #   テクスチャの作成
    #     サポートしているフォーマットは 
    #     .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。<br>
    #     ミップマップレベル 1 で作成します。
    #   @param [String] filePath テクスチャファイル名
    # @overload initialize(width, height, levels, format)
    #   テクスチャの作成
    #   @param [Integer] width 幅 (ピクセル数)
    #   @param [Integer] height 高さ (ピクセル数)
    #   @param [Integer] levels ミップマップレベル (0で最大レベルまで作成)
    #   @param [LN::TextureFormat] format サーフェイスフォーマット
    # @overload initialize(filePath, colorKey, levels, format)
    #   テクスチャの作成
    #     サポートしているフォーマットは 
    #     .bmp、.dds、.dib、.jpg、.png、.tga の6種類です。
    #   @param [String] filePath テクスチャファイル名
    #   @param [Integer] colorKey 透明色として扱う色
    #   @param [Integer] levels ミップマップレベル (0で最大レベルまで作成)
    #   @param [LN::TextureFormat] format サーフェイスフォーマット
    def initialize
    end
    # テクスチャの幅の取得 (ピクセル単位)
    # @overload width()
    #   @return [Integer] テクスチャの幅を格納する変数
    def width
    end
    # テクスチャの高さの取得 (ピクセル単位)
    # @overload height()
    #   @return [Integer] テクスチャの高さを格納する変数
    def height
    end
    # テクスチャの実際の幅の取得 (ピクセル単位)
    # @overload real_width()
    #   @return [Integer] テクスチャの実際の幅を格納する変数
    def real_width
    end
    # テクスチャの実際の高さの取得 (ピクセル単位)
    # @overload real_height()
    #   @return [Integer] テクスチャの実際の高さを格納する変数
    def real_height
    end
    # テクスチャをクリアする
    # @overload clear(color)
    #   @param [LN::Color] color 塗りつぶす色
    def clear
    end
    # テクスチャをクリアする
    # @overload clear_rgba(r, g, b, a)
    #   @param [Integer] r 塗りつぶす色の R 値
    #   @param [Integer] g 塗りつぶす色の G 値
    #   @param [Integer] b 塗りつぶす色の B 値
    #   @param [Integer] a 塗りつぶす色の A 値
    def clear_rgba
    end
    # テクスチャへ文字列を描画する
    # @overload draw_text(text, rect, font=nil, length=-1, align=LEFT)
    #   テクスチャへ文字列を描画する
    #     レンダリングターゲットと深度バッファテクスチャには描画できません。
    #   @param [String] text 描画する文字列
    #   @param [LN::Rect] rect 描画領域 (はみだした場合にクリッピングされます)
    #   @param [LN::Font] font 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
    #   @param [Integer] length 描画文字数 (-1 の場合、文字列全体を描画する)
    #   @param [LN::TextAlign] align 描画基準位置の指定
    # @overload draw_text(text, x, y, font=nil, length=-1, align=LEFT)
    #   テクスチャへ文字列を描画する
    #     レンダリングターゲットと深度バッファテクスチャには描画できません。
    #   @param [String] text 描画する文字列
    #   @param [Integer] x 描画先の左上 X 座標
    #   @param [Integer] y 描画先の左上 Y 座標
    #   @param [LN::Font] font 描画に使用するフォント (0 の場合、デフォルトのフォントを使用する)
    #   @param [Integer] length 描画文字数 (-1 の場合、文字列全体を描画する)
    #   @param [LN::TextAlign] align 描画基準位置の指定
    def draw_text
    end
    # テクスチャのブロック転送
    # @overload bit_blt(x, y, srcTexture, srcRect)
    #   　
    #     srcTexture で指定されたテクスチャから texture へブロック転送を行います。<br>
    #     レンダリングターゲットと深度バッファテクスチャは
    #     転送元・先のどちらにも指定できません。
    #   @param [Integer] x 転送先 X 座標
    #   @param [Integer] y 転送先 Y 座標
    #   @param [LN::Texture] srcTexture 転送元テクスチャハンドル
    #   @param [LN::Rect] srcRect 転送元領域
    def bit_blt
    end
end
# フォントオブジェクト
class LN::Font < ReferenceObject
    # フォントの作成
    # @overload initialize()
    #   フォントの作成
    #     デフォルトのフォントを使用してフォントを作成します。
    # @overload initialize(filePath)
    #   ビットマップフォントの作成
    #   @param [String] filePath 画像ファイル名
    # @overload initialize(srcFont)
    #   フォントのコピーの作成
    #   @param [LN::Font] srcFont コピー元のフォントハンドル
    def initialize
    end
    # 指定フォントをデフォルトフォントに設定する
    # @overload set_default_font(font)
    #   　
    #     テクスチャの DrawText 等でフォントハンドルを NULL にした場合、
    #     このフォントが使われるようになります。
    #   @param [LN::Font] font フォントハンドル
    def set_default_font
    end
    # フォント名の設定
    # @overload name=(fontName)
    #   @param [String] fontName フォント名
    def name=
    end
    # フォント名の取得
    # @overload name()
    #   @return [String] フォント名を格納する変数
    def name
    end
    # フォントサイズの設定
    # @overload size=(size)
    #   @param [Integer] size 設定するサイズ
    def size=
    end
    # フォントサイズの取得
    # @overload size()
    #   @return [Integer] フォントサイズを格納する変数
    def size
    end
    # フォントカラーの設定
    # @overload color=(color)
    #   @param [LN::Color] color 設定する色
    def color=
    end
    # フォントカラーの取得
    # @overload color()
    #   @return [LN::Color] 色を格納する変数
    def color
    end
    # エッジカラーの設定
    # @overload edge_color=(color)
    #   @param [LN::Color] color 設定する色
    def edge_color=
    end
    # エッジカラーの取得
    # @overload edge_color()
    #   @return [LN::Color] 色を格納する変数
    def edge_color
    end
    # エッジの幅の設定
    # @overload edge_size=(size)
    #   @param [Integer] size エッジの幅 (0 でエッジ無効)
    def edge_size=
    end
    # エッジの幅の取得
    # @overload edge_size()
    #   @return [Integer] エッジの幅を格納する変数
    def edge_size
    end
    # 太文字有無の設定
    # @overload bold=(enabled)
    #   @param [Bool] enabled 太文字の有無
    def bold=
    end
    # 太文字有無の取得
    # @overload bold?()
    #   @return [Bool] 太文字の有無を格納する変数
    def bold?
    end
    # イタリック体有無の設定
    # @overload italic=(enabled)
    #   @param [Bool] enabled イタリック体の有無
    def italic=
    end
    # イタリック体有無の取得
    # @overload italic?()
    #   @return [Bool] イタリック体の有無を格納する変数
    def italic?
    end
    # アンチエイリアス有無の設定
    # @overload anti_alias=(enabled)
    #   @param [Bool] enabled アンチエイリアスの有無
    def anti_alias=
    end
    # アンチエイリアス有無の取得
    # @overload anti_alias?()
    #   @return [Bool] アンチエイリアス有無を格納する変数
    def anti_alias?
    end
    # 文字列を描画したときのサイズ (ピクセル単位) の取得
    # @overload get_text_size(font, text, length)
    #   @param [LN::Font] font フォントハンドル
    #   @param [String] text サイズ計算に使用する文字列
    #   @param [Integer] length 文字数 (-1 で文字列全体)
    #   @return [LN::Size] 結果を格納する変数
    def get_text_size
    end
end
# シーングラフ
class LN::SceneGraph
end
# シーングラフノードのベースクラス
class LN::SceneNode < ReferenceObject
    # ノード名の取得
    # @overload name()
    #   @return [String] ノードの名前を格納する変数
    def name
    end
    # ノード名の設定
    # @overload name=(name)
    #   @param [String] name 新しい名前
    def name=
    end
    # 座標の設定
    # @overload position=(vec)
    #   座標の設定
    #   @param [LN::Vector3] vec 座標
    # @overload position=(x, y, z)
    #   座標の設定
    #   @param [Float] x X 座標
    #   @param [Float] y Y 座標
    #   @param [Float] z Z 座標
    def position=
    end
    # 座標の取得
    # @overload position()
    #   @return [LN::Vector3] 座標を格納するベクトル
    def position
    end
    # 回転角度の設定 (ラジアン単位)
    # @overload angle=(vec)
    #   回転角度の設定 (ラジアン単位)
    #   @param [LN::Vector3] vec 各軸の回転角度
    # @overload angle=(x, y, z)
    #   回転角度の設定 (ラジアン単位)
    #   @param [Float] x X 軸の回転角度
    #   @param [Float] y Y 軸の回転角度
    #   @param [Float] z Z 軸の回転角度
    def angle=
    end
    # 回転角度の取得 (ラジアン単位)
    # @overload angle()
    #   @return [LN::Vector3] 回転角度を格納するベクトル
    def angle
    end
    # 拡大率の設定 (ラジアン単位)
    # @overload scale=(vec)
    #   拡大率の設定 (ラジアン単位)
    #   @param [LN::Vector3] vec 拡大率
    # @overload scale=(x, y, z)
    #   拡大率の設定 (ラジアン単位)
    #   @param [Float] x X 方向の拡大率
    #   @param [Float] y Y 方向の拡大率
    #   @param [Float] z Z 方向の拡大率
    def scale=
    end
    # 拡大率の取得 (ラジアン単位)
    # @overload scale()
    #   @return [LN::Vector3] 拡大率を格納するベクトル
    def scale
    end
    # 移動・回転・拡大の原点の設定
    # @overload center=(vec)
    #   移動・回転・拡大の原点の設定
    #   @param [LN::Vector3] vec 各軸のオフセット
    # @overload center=(x, y, z)
    #   移動・回転・拡大の原点の設定
    #   @param [Float] x X 方向のオフセット
    #   @param [Float] y Y 方向のオフセット
    #   @param [Float] z Z 方向のオフセット
    def center=
    end
    # 移動・回転・拡大の原点の取得
    # @overload center()
    #   @return [LN::Vector3] 各軸のオフセットを格納するベクトル
    def center
    end
    # 座標変換行列の設定
    # @overload matrix=(mat)
    #   　
    #     単位行列ではない行列が設定されてる場合、
    #     SetPosition()、SetAngle()、SetScale() で設定された値は無効です。
    #   @param [LN::Matrix] mat 座標変換行列
    def matrix=
    end
    # 座標変換行列の取得
    # @overload matrix()
    #   @return [LN::Matrix] 座標変換行列を格納する変数
    def matrix
    end
    # 回転順序の設定
    # @overload rotate_order=(rotOrder)
    #   @param [LN::RotationOrder] rotOrder 回転順序
    def rotate_order=
    end
    # 回転順序の取得
    # @overload rotate_order()
    #   @return [LN::RotationOrder] 回転順序を示す値を格納する変数
    def rotate_order
    end
    # 配置方法の設定
    # @overload placement_type=(type)
    #   @param [LN::PlacementType] type 配置方法
    def placement_type=
    end
    # 配置方法の取得
    # @overload placement_type()
    #   @return [LN::PlacementType] 配置方法を格納する変数
    def placement_type
    end
    # 子ノードを追加する
    # @overload add_child(childSceneNode)
    #   @param [LN::SceneNode] childSceneNode 子ノードとして追加するノード
    def add_child
    end
    # 子ノードを取り除く
    # @overload remove_child(childSceneNode)
    #   @param [LN::SceneNode] childSceneNode 取り外す子ノード
    def remove_child
    end
    # フレーム更新
    # @overload update(elapsedTime)
    #   　
    #     エフェクトやアニメーションの状態を elapsedTime 分進めます。
    #     通常、1フレームに1度呼び出します。<br>
    #     自動更新が有効になっている場合はライブラリの
    #     フレーム更新を行った時に自動的に呼ばれるため、
    #     この関数を直接呼び出す必要はありません。<br>
    #     自動更新はデフォルトで有効です。
    #   @param [Float] elapsedTime 経過させる時間
    def update
    end
    # 自動更新有無の設定
    # @overload enable_auto_update=(enabled)
    #   　
    #     デフォルト値は LN_TRUE です。
    #   @param [Bool] enabled LN_TRUE=有効 / LN_FALSE=無効
    def enable_auto_update=
    end
    # 自動更新有無の取得
    # @overload enable_auto_update?()
    #   @return [Bool] 自動更新の有無を格納する変数
    def enable_auto_update?
    end
end
# 描画機能を持つシーンノードのスーパークラス
class LN::VisualNode < SceneNode
    # 可視状態の設定
    # @overload visible=(enabled)
    #   @param [Bool] enabled LN_TRUE=表示する / LN_FALSE=表示しない
    def visible=
    end
    # 不透明度の設定
    # @overload opacity=(opacity)
    #   @param [Float] opacity 不透明度 (0.0～1.0)
    def opacity=
    end
    # 乗算色の設定
    # @overload set_color_scale(materialIndex, color)
    #   @param [Integer] materialIndex 設定先のマテリアル番号
    #   @param [LN::ColorF] color 設定する色
    def set_color_scale
    end
    # ブレンドカラーの設定
    # @overload set_blend_color(materialIndex, color)
    #   @param [Integer] materialIndex 設定先のマテリアル番号
    #   @param [LN::ColorF] color 設定する色
    def set_blend_color
    end
    # 色調の設定
    # @overload set_tone(materialIndex, tone)
    #   @param [Integer] materialIndex 設定先のマテリアル番号
    #   @param [LN::Tone] tone 設定する色調
    def set_tone
    end
    # UV 変換行列の設定
    # @overload set_uv_transform(materialIndex, matrix)
    #   @param [Integer] materialIndex 設定先のマテリアル番号
    #   @param [LN::Matrix] matrix 設定する行列
    def set_uv_transform
    end
    # 合成方法の設定
    # @overload blend_mode=(mode)
    #   @param [LN::BlendMode] mode 合成方法
    def blend_mode=
    end
    # カリング方法の設定
    # @overload culling_mode=(mode)
    #   @param [LN::CullingMode] mode カリング方法
    def culling_mode=
    end
    # 深度テストの有効設定
    # @overload enable_depth_test=(enabled)
    #   @param [Bool] enabled 深度テストの有無
    def enable_depth_test=
    end
    # 深度書き込みの有効設定
    # @overload enable_depth_write=(enabled)
    #   @param [Bool] enabled 深度書き込みの有無
    def enable_depth_write=
    end
    # シェーダの設定
    # @overload shader=(shader)
    #   @param [LN::Shader] shader シェーダハンドル
    def shader=
    end
end
# スプライトノード
class LN::Sprite < VisualNode
    # スプライトオブジェクトの作成 (2D空間へ配置する)
    # @overload initialize(texture)
    #   @param [LN::Texture] texture スプライトが表示するテクスチャのハンドル
    def initialize
    end
    # スプライトが表示するテクスチャの設定
    # @overload texture=(texture)
    #   @param [LN::Texture] texture テクスチャハンドル
    def texture=
    end
    # スプライトのサイズの設定
    # @overload set_size(width, height)
    #   @param [Float] width 設定する幅
    #   @param [Float] height 設定する高さ
    def set_size
    end
    # テクスチャの転送矩形の設定
    # @overload src_rect=(rect)
    #   テクスチャの転送矩形の設定
    #   @param [LN::Rect] rect 転送元矩形
    # @overload src_rect=(x, y, w, h)
    #   テクスチャの転送矩形の設定
    #   @param [Integer] x 転送元の左上の X 座標
    #   @param [Integer] y 転送元の左上の Y 座標
    #   @param [Integer] w 転送元の幅
    #   @param [Integer] h 転送元の高さ
    def src_rect=
    end
    # テクスチャの転送矩形の取得
    # @overload src_rect()
    #   @return [LN::Rect] 矩形を格納する変数
    def src_rect
    end
    # 左右反転有無の設定
    # @overload flip_h=(enabled)
    #   @param [Bool] enabled LN_TRUE=反転する / LN_FALSE=反転しない
    def flip_h=
    end
    # 左右反転有無の取得
    # @overload flip_h?()
    #   @return [Bool] 左右反転の有無を示す値を格納する変数 (LN_TRUE=反転する / LN_FALSE=反転しない)
    def flip_h?
    end
    # 上下反転有無の設定
    # @overload flip_v=(enabled)
    #   @param [Bool] enabled LN_TRUE=反転する / LN_FALSE=反転しない
    def flip_v=
    end
    # 上下反転有無の取得
    # @overload flip_v?()
    #   @return [Bool] 左右反転の有無を示す値を格納する変数
    def flip_v?
    end
end
# スプライトパスノード
class LN::SpritePath < VisualNode
    # スプライトパスオブジェクトの作成
    # @overload initialize(maxSpriteCount, coord=SPACE_2D)
    #   @param [Integer] maxSpriteCount このパスが一度の画面更新で描画できるスプライトの数
    #   @param [LN::Coord] coord 描画先の座標空間
    def initialize
    end
    # 以降のスプライトの描画で使用するブレンドモードの設定
    # @overload path_blend_mode=(blendMode)
    #   @param [LN::BlendMode] blendMode 合成方法
    def path_blend_mode=
    end
    # 以降のスプライトの描画で使用する座標変換行列の設定
    # @overload path_transform=(matrix)
    #   @param [LN::Matrix] matrix 座標変換行列
    def path_transform=
    end
    # スプライトの描画
    # @overload draw_sprite(position, center, size, texture, srcRect, vertexColors, frontAxis=RZ)
    #   　
    #     center は 2D/3D で基準位置が異なります。
    #     2D空間へ描画するときはスプライトの左上、
    #     3D空間へ描画するときはスプライトの中央となります。<br>
    #     front は、2Dの時は通常 LN_FRONTAXIS_RZ を指定します。
    #   @param [LN::Vector3] position 描画先座標
    #   @param [LN::Vector3] center 位置や回転の中心となるオフセット
    #   @param [LN::Vector2] size 描画先スプライトのサイズ
    #   @param [LN::Texture] texture 描画するテクスチャハンドル
    #   @param [LN::Rect] srcRect テクスチャからの転送矩形
    #   @param [LN::ColorF] vertexColors 四角形の各頂点の乗算色 (要素数 4 の配列。NULL の場合は考慮しない)
    #   @param [LN::FrontAxis] frontAxis スプライトの正面方法
    def draw_sprite
    end
end
# モデルノード
class LN::Model < VisualNode
    # モデルオブジェクトの作成
    # @overload initialize(filePath, meshCreateFlags)
    #   @param [String] filePath メッシュファイル名
    #   @param [LN::MeshCreateFlags] meshCreateFlags 
    def initialize
    end
end
# エフェクトオブジェクト
class LN::Effect < VisualNode
    # エフェクトオブジェクトの作成 (3D空間へ配置する)
    # @overload initialize(filePath)
    #   @param [String] filePath エフェクトファイル名 (Effekseer 形式)
    def initialize
    end
    # エフェクトの再生を開始する
    # @overload play(overlap)
    #   @param [Bool] overlap LN_TRUE = 再生中のエフェクトを停止しない / LN_FALSE = 再生中のエフェクトを停止する
    def play
    end
    # エフェクトを停止する
    # @overload stop()
    def stop
    end
    # エフェクトが再生中かを確認する
    # @overload playing?()
    #   @return [Bool] 再生中かを示す値を格納する変数 (LN_TRUE=再生中 / LN_FALSE=停止中)
    def playing?
    end
end
# 大量のスプライトを格子状に表示するためのクラスです。主に 2D ゲームのマップ表示に使用します。
class LN::Tilemap < VisualNode
    # タイルマップの作成
    # @overload initialize()
    def initialize
    end
    # タイルマップのマップデータを設定する
    # @overload map_data=(intTable)
    #   @param [LN::IntTable] intTable マップデータとする整数テーブルハンドル
    def map_data=
    end
    # タイルマップのタイルセットとして使用するテクスチャを設定する
    # @overload tileset_texture=(texture)
    #   @param [LN::Texture] texture 設定するテクスチャハンドル
    def tileset_texture=
    end
    # タイルセットテクスチャの横方向のタイル数を設定する
    # @overload tileset_horizontal_tile_count=(count)
    #   @param [Integer] count タイル数
    def tileset_horizontal_tile_count=
    end
    # タイルマップのタイルひとつ分のピクセルサイズを設定する
    # @overload tile_pixel_size=(size)
    #   @param [Integer] size ピクセル数 (正方形。縦横同一サイズ)
    def tile_pixel_size=
    end
end
# シェーダ
class LN::Shader < ReferenceObject
    # シェーダの作成
    # @overload initialize(filePath)
    #   @param [String] filePath シェーダファイル名
    def initialize
    end
end
# ビューペイン
class LN::ViewPane < ReferenceObject
    # デフォルトで生成されるビューペインの取得
    # @overload default_view_pane()
    #   @return [LN::ViewPane] ビューペインハンドルを格納する変数
    def default_view_pane
    end
    # ViewPane の背景色を設定する
    # @overload background_color=(color)
    #   @param [LN::ColorF] color 背景色に設定する色
    def background_color=
    end
    # ViewPane に設定されている Layer のリストを取得する
    # @overload layers()
    #   @return [LN::ObjectList] オブジェクトリストハンドル(Layer)を格納する変数
    def layers
    end
end
# レイヤー
class LN::Layer < ReferenceObject
    # 【この機能は現在使用できません】
    # @overload priority=(priority)
    #   @param [Integer] priority 優先度 (大きいほど手前(後から)に描画されます)
    def priority=
    end
    # 【この機能は現在使用できません】
    # @overload priority()
    #   @return [Integer] 優先度を格納する変数
    def priority
    end
end
# ビューフィルタ
class LN::ViewFilter < Layer
    # フレーム更新
    # @overload update(elapsedTime)
    #   　
    #     エフェクトやアニメーションの状態を elapsedTime 分進めます。
    #     通常、1フレームに1度呼び出します。<br>
    #     自動更新が有効になっている場合はライブラリの
    #     フレーム更新を行った時に自動的に呼ばれるため、
    #     この関数を直接呼び出す必要はありません。<br>
    #     自動更新はデフォルトで有効です。
    #   @param [Float] elapsedTime 経過させる時間
    def update
    end
    # 自動更新有無の設定
    # @overload enable_auto_update=(enabled)
    #   　
    #     デフォルト値は LN_TRUE です。
    #   @param [Bool] enabled LN_TRUE=有効 / LN_FALSE=無効
    def enable_auto_update=
    end
    # 自動更新有無の取得
    # @overload enable_auto_update?()
    #   @return [Bool] 有無を示す値を格納する変数 (LN_TRUE=有効 / LN_FALSE=無効)
    def enable_auto_update?
    end
end
# 色調フィルタ
class LN::ToneFilter < ViewFilter
    # 色調を変更するビューフィルタを作成する
    # @overload initialize()
    def initialize
    end
    # 色調を設定する
    # @overload set_tone(tone, duration)
    #   @param [LN::Tone] tone 設定する色調
    #   @param [Float] duration 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
    def set_tone
    end
end
# トランジションフィルタ
class LN::TransitionFilter < ViewFilter
    # トランジションを行うビューフィルタを作成する
    # @overload initialize()
    def initialize
    end
    # トランジションの準備として、現在の画面を固定する
    # @overload freeze()
    #   　
    #     これ以降、LNTransitionFilter_Transition によるトランジション実行まで
    #     フィルタされた画面は更新されなくなります。
    def freeze
    end
    # 固定された画面から現在の画面へのトランジションを実行する
    # @overload transition(duration, maskTexture, vague)
    #   　
    #     maskTexture が NULL の場合は通常のフェードになります。
    #   @param [Float] duration 変化にかける時間 (デフォルトではフレーム単位。タイムスケール依存)
    #   @param [LN::Texture] maskTexture トランジションに使用するマスク画像
    #   @param [Float] vague マスク画像の境界のあいまいさ
    def transition
    end
end
# ブラーフィルタ
class LN::BlurFilter < ViewFilter
    # ブラ―(残像)を適用するビューフィルタを作成する
    # @overload initialize()
    def initialize
    end
    # 色調を設定する
    # @overload blur(duration, power, scale, center, baseCamera)
    #   　
    #     duration が 0 より大きい場合、その時間かけて徐々に強度を 0.0 に戻します。
    #     duration が 0 の場合は強度を自動で変化させません。<br>
    #     scale は元のスクリーンイメージに対して、1フレーム当たりどの程度拡大するかを指定します。<br>
    #     1.0 でスクリーンと等倍 (変化なし)、2.0 でスクリーンの2倍の大きさとなります。
    #     通常は 1.0～1.1 の範囲で指定するのが無難です。
    #     1.0 より小さい値を指定すると内側向きのブラーになります。<br>
    #     center は baseCamera が 2Dカメラであれば 2D 空間上の点、3Dカメラであれば3D空間上の点を指定します。
    #     baseCamera が NULL の場合、center は -1.0 ～ 1.0 で指定します。(0.0 が画面中央です)
    #   @param [Float] duration 適用時間 (デフォルトではフレーム単位。タイムスケール依存)
    #   @param [Float] power ブラーの強度 (ブラーイメージを描画する時のアルファ値。0.0～1.0 で指定)
    #   @param [Float] scale ブラーの大きさ (長さ)
    #   @param [LN::Vector3] center scale を使用して放射ブラーを作るときの中心座標
    #   @param [LN::Camera] baseCamera center の座標空間の基準となるカメラのハンドル
    def blur
    end
end
# 3D または 2D シーンのカメラ
class LN::Camera < SceneNode
    # デフォルトで作成される3Dカメラを取得する
    # @overload default_3d_camera()
    #   @return [LN::Camera] カメラハンドルを格納する変数
    def default_3d_camera
    end
    # デフォルトで作成される3Dカメラを取得する
    # @overload default_2d_camera()
    #   @return [LN::Camera] カメラハンドルを格納する変数
    def default_2d_camera
    end
    # カメラ姿勢の設定方法
    # @overload camera_type=(type)
    #   @param [LN::CameraType] type 姿勢の設定方法
    def camera_type=
    end
    # カメラの位置を設定する
    # @overload position=(position)
    #   カメラの位置を設定する
    #   @param [LN::Vector3] position 座標
    # @overload position=(x, y, z)
    #   カメラの位置を設定する
    #   @param [Float] x X 座標
    #   @param [Float] y Y 座標
    #   @param [Float] z Z 座標
    def position=
    end
    # カメラの注視点を設定する
    # @overload look_at=(at)
    #   カメラの注視点を設定する
    #   @param [LN::Vector3] at 注視点座標
    # @overload look_at=(x, y, z)
    #   カメラの注視点を設定する
    #   @param [Float] x 注視点 X 座標
    #   @param [Float] y 注視点 Y 座標
    #   @param [Float] z 注視点 Z 座標
    def look_at=
    end
    # カメラの上方向を設定する
    # @overload up_direction=(up)
    #   カメラの上方向を設定する
    #   @param [LN::Vector3] up 上方向ベクトル
    # @overload up_direction=(x, y, z)
    #   カメラの上方向を設定する
    #   @param [Float] x 上方向ベクトルの X 成分
    #   @param [Float] y 上方向ベクトルの Y 成分
    #   @param [Float] z 上方向ベクトルの Z 成分
    def up_direction=
    end
    # カメラの回転角度を設定する
    # @overload angle=(angle)
    #   カメラの回転角度を設定する
    #   @param [LN::Vector3] angle 各軸の回転角度 (ラジアン単位)
    # @overload angle=(x, y, z)
    #   カメラの回転角度を設定する
    #   @param [Float] x X 軸の回転角度 (ラジアン単位)
    #   @param [Float] y Y 軸の回転角度 (ラジアン単位)
    #   @param [Float] z Z 軸の回転角度 (ラジアン単位)
    def angle=
    end
    # カメラの姿勢行列を設定する
    # @overload matrix=(matrix)
    #   　
    #     行列を使用し、位置(0,0,0)、正面(0,0,1)、上方向(0,1,0) を
    #     変換して姿勢を算出します。
    #   @param [LN::Matrix] matrix 変換行列
    def matrix=
    end
    # カメラの視野角を設定する
    # @overload fov_y=(fov)
    #   @param [Float] fov 視野角
    def fov_y=
    end
    # カメラの最手前クリップ距離と最奥クリップ距離を設定する
    # @overload set_near_far_clip(nearClip, farClip)
    #   @param [Float] nearClip 手前クリップ距離
    #   @param [Float] farClip 奥クリップ距離
    def set_near_far_clip
    end
    # カメラのビュー行列を取得する
    # @overload view_matrix()
    #   @return [LN::Matrix] ビュー行列を格納する変数
    def view_matrix
    end
    # カメラのプロジェクション行列を取得する
    # @overload projection_matrix()
    #   @return [LN::Matrix] プロジェクション行列を格納する変数
    def projection_matrix
    end
    # ビュー行列とプロジェクション行列の積を取得する
    # @overload view_projection_matrix()
    #   @return [LN::Matrix] ビュー行列とプロジェクション行列の積を格納する変数
    def view_projection_matrix
    end
end
# 初期設定
class LN::Config
    # ウィンドウタイトルの設定
    # @overload set_window_title(title)
    #   @param [String] title ウィンドウタイトル文字列
    def set_window_title
    end
    # ウィンドウサイズの設定
    # @overload set_window_size(width, height)
    #   @param [Integer] width クライアント領域の幅 (default:640)
    #   @param [Integer] height クライアント領域の高さ (default:480)
    def set_window_size
    end
    # バックバッファサイズの設定
    # @overload set_backbuffer_size(width, height)
    #   　
    #     ウィンドウサイズとは異なるサイズでバックバッファを作成する時、
    #     バックバッファのサイズを指定します。
    #     サイズが 0 の場合、ウィンドウサイズと同じサイズで作成します。
    #   @param [Integer] width バックバッファの幅 (default:0)
    #   @param [Integer] height バックバッファの高さ (default:0)
    def set_backbuffer_size
    end
    # ウィンドウリサイズの有効設定
    # @overload set_window_resize_enabled(enabled)
    #   @param [Bool] enabled LN_TRUE の場合、ウィンドウ枠をD&Dすることでサイズ変更が可能 (default:LN_TRUE)
    def set_window_resize_enabled
    end
    # アクセラレータキーとして割り当てるキーの設定
    # @overload set_accelerator_key(acceleratorKey, keyCode, isShift, isCtrl, isAlt)
    #   　
    #     初期状態では以下のように割り当てられています。<br>
    #     - LN_ACCELERATORKEY_TOGGLE_FULLSCREEN<br>
    #     Alt + Enter キー
    #     - LN_ACCELERATORKEY_SHOW_DEBUG_STATISTICS<br>
    #     F3 キー
    #   @param [LN::AcceleratorKey] acceleratorKey アクセラレータの種別
    #   @param [LN::KeyCode] keyCode 割り当てるキーコード
    #   @param [Bool] isShift LN_TRUE を指定すると、Shift キーを修飾子とする
    #   @param [Bool] isCtrl LN_TRUE を指定すると、Ctrl キーを修飾子とする
    #   @param [Bool] isAlt LN_TRUE を指定すると、Alt キーを修飾子とする
    def set_accelerator_key
    end
    # ログファイルの出力設定
    # @overload set_log_file_enabled(flag)
    #   @param [Bool] flag LN_TRUE=ログファイルを出力する / LN_FALSE=しない (default:LN_FALSE)
    def set_log_file_enabled
    end
    # コンソールの割り当て設定
    # @overload set_console_enabled(flag)
    #   @param [Bool] flag LN_TRUE=割り当てる / LN_FALSE=割り当てない (default:LN_FALSE)
    def set_console_enabled
    end
    # ファイルを開くときにアクセスする暗号化アーカイブの登録
    # @overload register_archive(filePath, password)
    #   @param [String] filePath アーカイブファイル名
    #   @param [String] password アーカイブファイルを開くためのパスワード
    def register_archive
    end
    # ファイルへのアクセス制限の設定
    # @overload set_file_access_priority(priority)
    #   @param [LN::FileAccessPriority] priority 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
    def set_file_access_priority
    end
    # DirectMusic の使用設定
    # @overload set_direct_music_init_mode(mode)
    #   @param [LN::DirectMusicInitMode] mode DirectMusic の使用方法 (default:LN_DMINITMODE_NOT_USE)
    def set_direct_music_init_mode
    end
    # 音声データの最大キャッシュ数の設定
    # @overload set_sound_max_cache_count(count)
    #   @param [Integer] count 最大数 (default:32)
    def set_sound_max_cache_count
    end
    # 音声再生方法の自動選択の音声データバイト数閾値
    # @overload sound_play_type_threshold(bytes)
    #   　
    #     再生方法(オンメモリorストリーミング)の自動選択時、
    #     音声データのバイト数がこの閾値よりも大きい場合ストリーミング、
    #     以下の場合はオンメモリで再生されます。
    #     MP3等の圧縮フォーマットではデコード後のサイズでチェックします。
    #   @param [Integer] bytes 最大数 (default:100000)
    def sound_play_type_threshold
    end
    # フレームレートの設定
    # @overload set_frame_rate(frameRate)
    #   @param [Integer] frameRate フレームレート (default:60)
    def set_frame_rate
    end
    # ライブラリ更新時時のフレーム待ち処理の有効設定
    # @overload set_enable_frame_wait(enabled)
    #   @param [Bool] enabled LN_TRUE=有効 / LN_FALSE=無効 (default:LN_TRUE)
    def set_enable_frame_wait
    end
    # 自動更新時のタイムスケールの設定
    # @overload set_auto_update_time_scale(timeScale)
    #   　
    #     SceneNode や ViewFilter の自動フレーム更新の際、
    #     前回フレームからの経過時間に乗算される値を設定します。
    #     この値を 1.0 にすると、エフェクト等の遷移時間を時間単位、
    #     フレームレートと同じ値にするとフレーム単位で指定できるようになります。
    #     0.0 の場合、フレームレートの値を使用します。
    #   @param [Float] timeScale タイムスケール (default:0.0)
    def set_auto_update_time_scale
    end
    # ライブラリで使用するフォントファイルを登録する
    # @overload register_font_file(filePath)
    #   　
    #     登録したフォントファイルはフォントオブジェクト作成時に
    #     ファミリ名を指定することで使用できます。
    #   @param [String] filePath フォントファイル名
    def register_font_file
    end
    # テクスチャの最大キャッシュ数の設定
    # @overload set_texture_cache_size(count)
    #   @param [Integer] count 最大数 (default:32)
    def set_texture_cache_size
    end
    # シェーダの最大キャッシュ数の設定
    # @overload set_shader_cache_size(count)
    #   @param [Integer] count 最大数 (default:32)
    def set_shader_cache_size
    end
    # 例外エラー発生時のメッセージボックス表示設定
    # @overload set_exception_message_box_enabled(enabled)
    #   @param [Bool] enabled LN_TRUE の場合、表示する (default:LN_TRUE)
    def set_exception_message_box_enabled
    end
    # ユーザー定義のウィンドウハンドルの設定
    # @overload set_win_32_user_window_handle(hWnd)
    #   @param [] hWnd ユーザー定義のウィンドウハンドル (default:NULL)
    def set_win_32_user_window_handle
    end
end
# ライブラリ管理
class LN::Core
    # ライブラリの初期化
    # @overload initialize()
    #   　
    #     ライブラリのすべての機能を使用できるように初期化を行います。
    def initialize
    end
    # ファイル入出力機能の初期化
    # @overload initialize_file_io()
    def initialize_file_io
    end
    # 音声機能の初期化
    # @overload initialize_audio()
    #   　
    #     ファイル入出力機能も同時に初期化されます。
    def initialize_audio
    end
    # 入力機能の初期化
    # @overload initialize_input()
    def initialize_input
    end
    # ライブラリのフレーム更新
    # @overload update()
    #   　
    #     ライブラリの状態を更新し、時間を1フレーム分進めます。
    #     この関数は必ず1フレームに1度だけ呼び出す必要があります。
    def update
    end
    # 画面の更新タイミングのリセット
    # @overload reset_frame_delay()
    #   　
    #     時間のかかる処理の後にこのメソッドを呼ぶことで、
    #     極端なフレームスキップが発生しないようにすることができます。
    def reset_frame_delay
    end
    # アプリケーションを終了するべきかを確認する
    # @overload end_requested?()
    #   　
    #     ウィンドウのクローズボタンが押された場合等、
    #     アプリケーションを終了するべき時には LN_FALSE を返します。
    #   @return [Bool] 終了要求の有無を格納する変数
    def end_requested?
    end
    # ライブラリの終了処理
    # @overload terminate()
    def terminate
    end
end
