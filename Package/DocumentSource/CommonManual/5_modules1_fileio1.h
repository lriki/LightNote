//=============================================================================
//【 FileIO 】
//-----------------------------------------------------------------------------
/**
    @page modules1_fileio1 ファイル入出力

    - @ref modules1_fileio1_anchor1   "ファイル入出力機能の初期化"
    - @ref modules1_fileio1_anchor2   "ファイルからデータを読み込む"
    - @ref modules1_fileio1_anchor3   "ファイルにデータを書き込む"
    - @ref modules1_fileio1_anchor5   "カレントディレクトリの扱い"
    - @ref modules1_fileio1_anchor4   "ファイルを非同期で読み書きする"
    - @ref modules1_fileio1_anchor5   "暗号化アーカイブを使用する"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor1
    @par ファイル入出力機能の初期化
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    ファイル入出力機能の初期化には LFileIO_Init を使用します。<br>
	機能を使用する前に、1度だけ呼び出してください。<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
LFileIO_Init();
    @ln_endcode
    @ln_code{tab_hsp,cpp}
LFileIO_Init
    @ln_endcode
@ln_endcodeblock
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor2
    @par ファイルからデータを読み込む
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    読み込みには LInFile_～ を使用します。<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// ファイルを開く
lnHandle file = LInFile_Create( "test.dat" );

// ファイルサイズを取得し、データを読み込むバッファを確保する
int size = LInFile_GetSize( file );
lnByte buf = new lnByte[size];

// データをすべて読み込む
LInFile_Read( file, buf, size );

// ファイルを閉じる
LInFile_Close( file );
    @ln_endcode
    @ln_code{tab_hsp,cpp}

    @ln_endcode
@ln_endcodeblock
    <br>
    暗号化アーカイブファイルが登録されている場合は、<br>
    アーカイブ内のファイルも自動で検索して開きます。<br>
    <br>
    <br>

    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor3
    @par ファイルにデータを書き込む
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    書き込みには LOutFile_～ を使用します。<br>
	<br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(関数)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
lnByte buf[] = "test data.";
int size = 10;

// ファイルを開く
lnHandle file = LOutFile_Create( "test.dat" );

// データを書き込む
LOutFile_Save( file, buf, size );

// ファイルを閉じる
LOutFile_Close( file );

    @ln_endcode
    @ln_code{tab_hsp,cpp}

    @ln_endcode
@ln_endcodeblock
	<br>
	上記は同期読み込みの例です。<br>
	この場合、各言語ごとの標準のファイル出力機能 (C/C++のfopen() や、.NET の FileStream) を使用しても<br>
	同様の結果が得られます。<br>
	<br>
	<br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor4
    @par ファイルを非同期で読み書きする
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor5
    @par 暗号化アーカイブを使用する
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <br>
    <br>

    <br>
    <br>
*/
//=============================================================================
