//=============================================================================
//�y �������@�Ǝg���� �z
//-----------------------------------------------------------------------------
/**
    @page install1 �������@�Ǝg����

    - @ref install1_anchor1 "Visual C++"<br>
    - @ref install1_anchor3 "HSP (Hot Soup Processor)"<br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor install1_anchor1
    <hr>
    @par Visual C++
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    1. "Include" �t�H���_���C���N���[�h�p�X�Ƃ��Ēǉ����܂��B<br>
    <br>
    2. "Library/Win32-msvcXXXX" ���̃��C�u�����t�@�C���������N���܂��B<br>
    <br>
	<table>
    <tr>
		<td rowspan="4">DLL(���I�����N)</td>	<td>MT</td>		<td>LightNote.lib<br>LightNote.dll(��1)</td>
	</tr>
	<tr>
												<td>MTd</td>	<td>LightNote.Debug.lib<br>LightNote.Debug.dll(��1)</td>
    </tr>
	<tr>
												<td>MD</td>		<td>LightNote.MD.lib<br>LightNote.MD.dll(��1)</td>
    </tr>
	<tr>
												<td>MDd</td>	<td>LightNote.Debug.MDd.lib<br>LightNote.Debug.MDd.dll(��1)</td>
    </tr>
    <tr>
		<td rowspan="4">LIB(�ÓI�����N)</td>	<td>MT</td>		<td>LightNote.Dependencies.Release.lib<br>LightNote.Static.Release.lib</td>
	</tr>
	<tr>
												<td>MTd</td>	<td>LightNote.Dependencies.Debug.lib<br>LightNote.Static.Debug.lib</td>
    </tr>
	<tr>
												<td>MD</td>		<td>LightNote.Dependencies.Release.MD.lib<br>LightNote.Static.Release.MD.lib</td>
    </tr>
	<tr>
												<td>MDd</td>	<td>LightNote.Dependencies.Debug.MDd.lib<br>LightNote.Static.Debug.MDd.lib</td>
    </tr>
    </table>
    (��1)���s�t�@�C��(.exe)�Ɠ����f�B���N�g�����A���ϐ�PATH�̒ʂ����f�B���N�g���ɃR�s�[���Ă��������B<br>
    <br>
    ���T���v���͓��ɐݒ���s��Ȃ��Ă��r���h�ł��܂��B<br>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    @anchor install1_anchor3
    <hr>
    @par HSP (Hot Soup Processor)
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    1. "LNote.dll" �� HSP �̃G�f�B�^�Ɠ����t�H���_�ɃR�s�[���܂��B<br>
    <br>
    2. "lnote.as" �� �P�D�̃t�H���_�ɂ��� "common" �t�H���_�ɃR�s�[���܂��B<br>
    <br>
    3. �v���O�����̐擪�� <br>
        #include "lnote.as"<br>
    �Ə����ƁA�v���O�C���̊e���߂��g����悤�ɂȂ�܂��B<br>
    <br>
    "lnote.hs" �� 1. �̃t�H���_�ɂ��� "hsphelp" �t�H���_�ɃR�s�[����ƁA<br>
    F1 �w���v�ł��̃v���O�C���̖��߂̃w���v�����邱�Ƃ��ł���悤�ɂȂ�܂��B<br>
    <br>
    <br>

*/
//=============================================================================

#if 0
//=============================================================================
//�y Visual C++ �z
//-----------------------------------------------------------------------------
/**
    @page install_1_vs2008 Visual C++ 2008 (Visual Studio 2008)

    <br>
    - <b>�p�X�̐ݒ�</b><br>
    <br>
    �K�v�ȃt�@�C���� LNote �̃p�b�P�[�W���́uLNote�v�t�H���_�ɑS�ē����Ă��܂��B<br>
    ���̃t�H���_�ւ̃p�X(�C���N���[�h�t�@�C���ƃ��C�u�����t�@�C��)��ʂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u�c�[���v���u�I�v�V�����v<br>
        �����̃��X�g�́u�v���W�F�N�g����у\�����[�V�����v���uVC++ �f�B���N�g���v��<br>
        �E��́u�f�B���N�g����\������v���W�F�N�g�v��I������ƕ\������郊�X�g�Ƀp�X��ǉ����܂��B<br>
        �ݒ肷�鍀�ڂ͈ȉ��̂ӂ��ł��B<br>
        - �C���N���[�h�t�@�C��
        - ���C�u�����t�@�C��<br>
    <br>
    <br>
    - <b>�����R�[�h�Z�b�g</b><br>
    <br>
    ���݂̓}���`�o�C�g�����R�[�h�ɂ̂ݑΉ����Ă��܂��B<br>
    �v���W�F�N�g�̕����Z�b�g���u�}���`�o�C�g��������g�p����v�ɂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u[�v���W�F�N�g��]�̃v���p�e�B�v��<br>
        �����̃��X�g�́u�\���v���p�e�B�v���u�S�ʁv��<br>
        �E���̃��X�g�́u�����Z�b�g�v<br>
    <br>
    <br>
    - <b>�����^�C�����C�u����</b><br>
    <br>
    LNote �́u�}���`�X���b�h �f�o�b�O(/MTd)�v�A�u�}���`�X���b�h (/MT)�v�Ńr���h���Ă���܂��B<br>
    �v���W�F�N�g�̃����^�C�����C�u�����̐ݒ������Ɠ������̂ɂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u�v���W�F�N�g�v���u[�v���W�F�N�g��]�̃v���p�e�B�v��<br>
        �����̃��X�g�́u�\���v���p�e�B�v���uC/C++�v���u�R�[�h�����v��<br>
        �E���̃��X�g�́u�����^�C�����C�u�����v<br>
        <br>
        Debug �r���h�ł́u�}���`�X���b�h �f�o�b�O(/MTd)�v�A<br>
        Release �r���h�ł́u�}���`�X���b�h (/MT)�v�ɂ��Ă��������B<br>
    <br>
    <br>



*/
//=============================================================================

//=============================================================================
//�y Visual C++ 2010 �z
//-----------------------------------------------------------------------------
/**
    @page install_1_vs2010 Visual C++ 2010 (Visual Studio 2010)

    <br>
    - <b>�p�X�̐ݒ�</b><br>
    <br>
    �K�v�ȃt�@�C���� LNote �̃p�b�P�[�W���́uLNote�v�t�H���_�ɑS�ē����Ă��܂��B<br>
    ���̃t�H���_�ւ̃p�X(�C���N���[�h�t�@�C���ƃ��C�u�����t�@�C��)��ʂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u[�v���W�F�N�g��]�̃v���p�e�B�v��<br>
        �����̃��X�g�́u�\���v���p�e�B�v���uVC++ �f�B���N�g���v��<br>
        �E���̃��X�g�́A�ȉ��̂ӂ��Ƀp�X��ǉ����܂��B<br>
        - �C���N���[�h �f�B���N�g��
        - ���C�u���� �f�B���N�g��<br>
    <br>
    <br>
    - <b>�����R�[�h�Z�b�g</b><br>
    <br>
    ���݂̓}���`�o�C�g�����R�[�h�ɂ̂ݑΉ����Ă��܂��B<br>
    �v���W�F�N�g�̕����Z�b�g���u�}���`�o�C�g��������g�p����v�ɂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u[�v���W�F�N�g��]�̃v���p�e�B�v��<br>
        �����̃��X�g�́u�\���v���p�e�B�v���u�S�ʁv��<br>
        �E���̃��X�g�́u�����Z�b�g�v<br>
    <br>
    <br>
    - <b>�����^�C�����C�u����</b><br>
    <br>
    LNote �́u�}���`�X���b�h �f�o�b�O(/MTd)�v�A�u�}���`�X���b�h (/MT)�v�Ńr���h���Ă���܂��B<br>
    �v���W�F�N�g�̃����^�C�����C�u�����̐ݒ������Ɠ������̂ɂ��Ă��������B<br>
    <br>
    �ݒ肷��ꏊ�E�E�E<br>
    @par 
        ���j���[�́u�v���W�F�N�g�v���u[�v���W�F�N�g��]�̃v���p�e�B�v��<br>
        �����̃��X�g�́u�\���v���p�e�B�v���uC/C++�v���u�R�[�h�����v��<br>
        �E���̃��X�g�́u�����^�C�����C�u�����v<br>
        <br>
        Debug �r���h�ł́u�}���`�X���b�h �f�o�b�O(/MTd)�v�A<br>
        Release �r���h�ł́u�}���`�X���b�h (/MT)�v�ɂ��Ă��������B<br>
    <br>
    <br>



*/
//=============================================================================
#endif