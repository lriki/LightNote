//=============================================================================
//�y FileIO �z
//-----------------------------------------------------------------------------
/**
    @page modules1_fileio1 �t�@�C�����o��

    - @ref modules1_fileio1_anchor1   "�t�@�C�����o�͋@�\�̏�����"
    - @ref modules1_fileio1_anchor2   "�t�@�C������f�[�^��ǂݍ���"
    - @ref modules1_fileio1_anchor3   "�t�@�C���Ƀf�[�^����������"
    - @ref modules1_fileio1_anchor5   "�J�����g�f�B���N�g���̈���"
    - @ref modules1_fileio1_anchor4   "�t�@�C����񓯊��œǂݏ�������"
    - @ref modules1_fileio1_anchor5   "�Í����A�[�J�C�u���g�p����"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor1
    @par �t�@�C�����o�͋@�\�̏�����
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �t�@�C�����o�͋@�\�̏������ɂ� LFileIO_Init ���g�p���܂��B<br>
	�@�\���g�p����O�ɁA1�x�����Ăяo���Ă��������B<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
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
    @par �t�@�C������f�[�^��ǂݍ���
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �ǂݍ��݂ɂ� LInFile_�` ���g�p���܂��B<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// �t�@�C�����J��
lnHandle file = LInFile_Create( "test.dat" );

// �t�@�C���T�C�Y���擾���A�f�[�^��ǂݍ��ރo�b�t�@���m�ۂ���
int size = LInFile_GetSize( file );
lnByte buf = new lnByte[size];

// �f�[�^�����ׂēǂݍ���
LInFile_Read( file, buf, size );

// �t�@�C�������
LInFile_Close( file );
    @ln_endcode
    @ln_code{tab_hsp,cpp}

    @ln_endcode
@ln_endcodeblock
    <br>
    �Í����A�[�J�C�u�t�@�C�����o�^����Ă���ꍇ�́A<br>
    �A�[�J�C�u���̃t�@�C���������Ō������ĊJ���܂��B<br>
    <br>
    <br>

    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor3
    @par �t�@�C���Ƀf�[�^����������
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �������݂ɂ� LOutFile_�` ���g�p���܂��B<br>
	<br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
lnByte buf[] = "test data.";
int size = 10;

// �t�@�C�����J��
lnHandle file = LOutFile_Create( "test.dat" );

// �f�[�^����������
LOutFile_Save( file, buf, size );

// �t�@�C�������
LOutFile_Close( file );

    @ln_endcode
    @ln_code{tab_hsp,cpp}

    @ln_endcode
@ln_endcodeblock
	<br>
	��L�͓����ǂݍ��݂̗�ł��B<br>
	���̏ꍇ�A�e���ꂲ�Ƃ̕W���̃t�@�C���o�͋@�\ (C/C++��fopen() ��A.NET �� FileStream) ���g�p���Ă�<br>
	���l�̌��ʂ������܂��B<br>
	<br>
	<br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor4
    @par �t�@�C����񓯊��œǂݏ�������
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_fileio1_anchor5
    @par �Í����A�[�J�C�u���g�p����
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <br>
    <br>

    <br>
    <br>
*/
//=============================================================================
