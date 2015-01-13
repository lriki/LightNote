//=============================================================================
// ツール ArchiveMaker
//-----------------------------------------------------------------------------
/**
    @page tools1_ArchiveMaker ArchiveMaker
    
    ArchiveMaker は LightNote 用の @ref modules1_intro1_anchor2 "暗号化アーカイブ" を作成するツールです。
    実行ファイルは "Tools/ArchiveMaker/ArchiveMaker.exe" です。
    
    暗号化アーカイブの作成は、対象フォルダとパスワードを入力することで行います。
    対象フォルダ以下に含まれるすべてのファイルがひとつの暗号化アーカイブファイルにパッキングされます。
    
    入力されたパスワードは、LightNote の初期化前設定で暗号化アーカイブを取り込むとき、
    セットで指定します。
    <br>
    - @ref tools1_ArchiveMaker_anchor1   "フォルダを D&D して作成する"
    - @ref tools1_ArchiveMaker_anchor2   "コマンドラインから使用する"
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor tools1_ArchiveMaker_anchor1
    @par フォルダを D&D して作成する
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    ① 実行ファイルにフォルダを D&D します。<br>
    @image html img/7_tools1_ArchiveMaker1.png
    <br>
    ② パスワードの入力を求められますので、任意の文字列を入力し、Enter キーを押します。(最大 255 文字です)<br>
    @image html img/7_tools1_ArchiveMaker2.png
    <br>
    ③ 元のフォルダと同じ場所に、"フォルダ名.lna" という名前で暗号化アーカイブファイルが生成されます。<br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor tools1_ArchiveMaker_anchor2
    @par コマンドラインから使用する
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    以下の構文に従い、ArchiveMaker.exe にコマンドライン引数を与えてを実行します。<br>
    @code
    ArchiveMaker 対象フォルダパス パスワード
    @endcode
    <br>
    <br>
*/
//=============================================================================