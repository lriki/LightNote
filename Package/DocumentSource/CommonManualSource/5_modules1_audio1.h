//=============================================================================
//�y �����@�\ �z
//-----------------------------------------------------------------------------
/**
    @page modules1_audio1 �����@�\

    - @ref modules1_audio1_anchor6   "�����@�\�̏�����"
    - @ref modules1_audio1_anchor3   "�T�E���h�̍Đ��Ƒ���"
    - @ref modules1_audio1_anchor1   "�Ή����Ă��鉹���t�@�C���t�H�[�}�b�g"
    - @ref modules1_audio1_anchor2   "MIDI �̍Đ��ɂ���"
    - @ref modules1_audio1_anchor4   "3D �T�E���h"
    - @ref modules1_audio1_anchor5   "�Q�[���I�[�f�B�I"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor6
    @par �����@�\�̏�����
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �����@�\�̏������ɂ� LAudio_Init ���g�p���܂��B<br>
    <br>
    �����@�\���g�p����O�i�ł��邾���v���O�����̐擪�j�ŁA1�x�����Ăяo���Ă��������B<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
LAudio_Init();
    @ln_endcode
    @ln_code{tab_hsp,cpp}
LAudio_Init
    @ln_endcode
@ln_endcodeblock
    <br>
    �Ȃ��A@ref modules1_fileio1 �������ɏ���������܂��B<br>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor3
    @par �T�E���h�̍Đ��Ƒ���
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �T�E���h�̍Đ��Ƒ���́A�����f�[�^�����ɍ쐬�����T�E���h�I�u�W�F�N�g��ʂ��čs���܂��B<br>
    <br>
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// �T�E���h�I�u�W�F�N�g�����ʂ��邽�߂̃T�E���h�n���h�����Ԃ�܂�
lnHandle sound_handle = LSound_Load( "sound.wav" );

// �ǂݍ��񂾉������Đ����܂�
LSound_Play( sound_handle );
    @ln_endcode
    @ln_code{tab_hsp,cpp}
// �T�E���h�I�u�W�F�N�g�����ʂ��邽�߂̃T�E���h�n���h�����A�ϐ� sound_handle �ɑ�����܂�
LSound_Load sound_handle, "sound.wav"

// �ǂݍ��񂾉������Đ����܂�
LSound_Play sound_handle
    @ln_endcode
@ln_endcodeblock
    <br>
    �T�E���h�̉��ʂ�s�b�`�A��q����3D�T�E���h�̈ʒu���́A�Đ����ɂ��ł��ύX���邱�Ƃ��ł��܂��B<br>
    �܂��A���ɓǂݍ��܂�Ă��鉹���t�@�C���Ɠ����̂��̂�ǂݍ������Ƃ����ꍇ�A���\�[�X�ߖ�̂��߂ɉ����f�[�^�������ŋ��L���܂��B�iWave�AOgg �̂݁j<br>
    <br>
    <br>
    �T�E���h�I�u�W�F�N�g�̍쐬��(��L LSound_Load ��)�ɂ́A���L�̃t���O�ōĐ����@���w�肷�邱�Ƃ��ł��܂��B<br>
    �w����@�ɂ��Ă̓��t�@�����X���Q�l�ɂ��Ă��������B<br>
    - LN_SOUNDPLAYTYPE_ONMEMORY<br>
        ������K���I���������Đ����܂��B<br>
        <br>
    - LN_SOUNDPLAYTYPE_STREAMING<br>
        ������K���X�g���[�~���O�Đ����܂��B<br>
        <br>
    - LN_SOUNDPLAYTYPE_AUTO (����, �f�t�H���g)<br>
        �f�R�[�h��̃T�C�Y�� 10000 �o�C�g�ȏ�ɂȂ�ꍇ�̓X�g���[�~���O�A<br>
        �����łȂ��ꍇ�̓I���������Đ��ɂȂ�܂��B<br>
    <br>
    <br>

    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor1
    @par �Ή����Ă��鉹���t�@�C���t�H�[�}�b�g
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    �Ή����Ă��鉹���t�@�C���t�H�[�}�b�g�͈ȉ��̒ʂ�ł��B
    <br>
    - WAVE (RIFF waveform Audio Format)
    - MP3 (MPEG Audio Layer-3)
    - Ogg (Ogg Vorbis)
    - MIDI (Musical Instrument Digital Interface)
    
    @par
    <br>
    MIDI �t�@�C���� DirectMusic�A����ȊO�̃t�@�C���� XAudio2 �ŉ��t����܂��B<br>
    <br>
    �܂��AOgg �� MIDI �͈ȉ��̕��@�Ń��[�v�ʒu�����o���܂��B<br>
    (RPG�c�N�[��VX�̎d�l�Ɠ������̂ł�)<br>
    - Ogg �̃��[�v�ʒu<br>
    <br>
        �t�@�C������ LOOPSTART�ALOOPLENGTH �Ƃ����R�����g������ꍇ�A
        ��������ƂɃ��[�v�ʒu��ݒ肵�܂��B<br>
        LOOPSTART �̓��[�v�̈�̊J�n�T���v�����A<br>
        LOOPLENGTH �̓��[�v�̈�̒����̃T���v�����ł��B<br>
        <br>
    - MIDI �̃��[�v�ʒu<br>
    <br>
        �t�@�C�����ɃR���g���[���`�F���W 111 �Ԃ̃C�x���g������ꍇ�A
        ���������[�v�J�n�ʒu�Ƃ��Đݒ肵�܂��B<br>
        (��������ꍇ�͈�ԍŌ�̂��̂��ΏۂɂȂ�܂�)<br>
        �f�[�^�̍Ō�܂ōĐ����ꂽ�ꍇ�A��������ĊJ���܂��B<br>
        <br>
        <br>
    
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor2
    @par MIDI �̍Đ��ɂ���
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    MIDI �͑O�q�̂Ƃ��� DirectMusic �ŉ��t����܂����ADirectMusic �̏������ɂ͐��b���x�̎��Ԃ�������܂��B<br>
    <br>
    MIDI ���g�p���Ȃ��\�t�g�ł��������Ɏ��Ԃ��������Ă��܂��E�E�E�Ƃ��������Ƃ�h�����߂ɁADirectMusic �̗L���͐؂�ւ��邱�Ƃ��ł��܂��B<br>
    MIDI ���g�p���邽�߂ɂ̓��C�u�����̏������ݒ�ŁAMIDI �̎g�p�iDirectMusic �̏������j�𖾎�����K�v������܂��B<br>
    <br>
    @ln_codeblock
        @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
        @ln_code{tab1,cpp}
LConfig_SetDirectMusicUsingMode( LN_DMINITMODE_NORMAL );
        @ln_endcode
        @ln_code{tab_hsp,cpp}
LConfig_SetDirectMusicUsingMode LN_DMINITMODE_NORMAL
        @ln_endcode
    @ln_endcodeblock
    <br>
    �w��ł���l�͈ȉ��̒ʂ�ł��B<br>
    - LN_DMINITMODE_NOTUSE (�f�t�H���g)<br>
        DirectMusic ���g�p���܂���B(MIDI �����t���邱�Ƃ͂ł��܂���)<br>
        <br>
    - LN_DMINITMODE_NORMAL<br>
        DirectMusic ���g�p���܂��B<br>
        ���ɂ���Ă͏������ɐ��b���x�̎��Ԃ�������܂��B<br>
        <br>
    - LN_DMINITMODE_THREAD_REQUEST<br>
        DirectMusic ��ʃX���b�h�ŏ���������悤�ɂ��܂��B<br>
        ���������̂��߂̒x���͈�؂Ȃ��Ȃ�܂����A<br>
        ����������������O�� MIDI �����t���悤�Ƃ����Ƃ��́A�������Ă��特����n�߂܂��B<br>
        �i���̏�Ԃł��T�E���h�́u�Đ����v�Ƃ��Ĉ����܂��j<br>
        <br>
    
    @par
    �Ȃ��ADirectMusic ���g���� MIDI �̍Đ��ł̓��o�[�u�̃G�t�F�N�g��������܂����A<br>
    ��������y�����邽�߂� DirectMusic ���t���Ă����f�t�H���g�̃��o�[�u�����኱��߂ɐݒ肵�Ă���܂��B<br>
    <br>
    �܂��AMIDI �̓T�E���h�̍Đ��ʒu�⃋�[�v�ʒu�̎w��Ɂu�~���[�W�b�N�^�C���v�Ƃ����l���g�p���܂��B<br>
    �����4�������ЂƂ��� 768 �Ƃ����P�ʂŁALN_MUSICTIMEBASE �Ƃ��Ē�`����Ă��܂��B<br>
    �iLNote �ł́AMIDI �̏ꍇ�̃T���v����=�~���[�W�b�N�^�C�� �Ƃ��Ĉ����Ă��܂��j<br>
    <br>
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_audio1_anchor4
    @par 3D�T�E���h
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    3D�T�E���h�́A������3D��ԏ�ōĐ�����@�\�ł��B<br>
    ���_����̈ʒu�ɂ���āA�T�E���h�����̓I�ɕ������Ă���悤�ȉ��o�����邱�Ƃ��ł��܂��B<br>
    - �P�ʋ����̐ݒ�<br>
        �K�v�ɉ����āA�ŏ��� 3D ��Ԃ� 1 ���[�g���ɑ������鋗����ݒ肵�܂��B<br>
        ���̐ݒ�̓h�b�v���[���ʂ̌v�Z�ɓK�p����܂��B<br>
        �f�t�H���g�� 1.0 �ł��B<br>
        <br>
        �Ⴆ�� MikuMikuDance �̃��f������Ƃ��ă��[���h���\�z����ꍇ�A<br>
        1.58m �̃L�����N�^�[���� 20.0 �̍����̃��f���ƂȂ��Ă��邽�߁A<br>
        20.0 / 1.58 = 12.658... �� 1 ���[�g���Ƃ��邱�ƂɂȂ�܂��B<br>
        <br>
    - ���X�i�[(�����)<br>
        3D��ԏ�̃T�E���h�𕷂��ʒu�Ɋւ�����ł��B�ʒu�A�����A���x��\���܂��B<br>
        ��ʂɁA���X�i�[�̈ʒu�ƕ����̓J�����̈ʒu����ѕ����Ɠ����ł��B<br>
        ���X�i�[�͉����@�\�S�̂ɑ΂���1�������݂��܂��B<br>
        <br>
    - �G�~�b�^�[(�T�E���h�I�u�W�F�N�g)<br>
        �T�E���h�̔������̏��ł��B<br>
        �ЂƂ̃T�E���h�I�u�W�F�N�g���ЂƂ̈ʒu�A���x�A�͈͂������܂��B<br>
        <br>
        
    @par 
@ln_codeblock
    @ln_tabdecl
        @ln_tab{tab1,C/C++(�֐�)}
        @ln_tab{tab_hsp,HSP}
    @ln_endtabdecl
    @ln_code{tab1,cpp}
// 3D ��Ԃ� 1 ���[�g���ɑ������鋗����ݒ肵�܂�
LAudio_SetMetreUnitDistance( 20.0 / 1.58 );

// ���X�i�[�̈ʒu�ƌ�����ݒ肵�܂�
L3DSoundListener_SetPosition( 10.0, 0.0, 5.0 );
L3DSoundListener_SetDirection( 0.0, 0.0, 1.0 );

// �T�E���h�I�u�W�F�N�g�̈ʒu�ƁA�����̕������鋗����ݒ肵�܂�
LSound_SetEmitterPosition( sound_handle, -10.0, 5.0, 20.0 );
LSound_SetEmitterDistance( sound_handle, 100.0 );
    @ln_endcode
    @ln_code{tab_hsp,cpp}
// 3D ��Ԃ� 1 ���[�g���ɑ������鋗����ݒ肵�܂�
LAudio_SetMetreUnitDistance 20.0 / 1.58

// ���X�i�[�̈ʒu�ƌ�����ݒ肵�܂�
L3DSoundListener_SetPosition 10.0, 0.0, 5.0
L3DSoundListener_SetDirection 0.0, 0.0, 1.0

// �T�E���h�I�u�W�F�N�g�̈ʒu�ƁA�����̕������鋗����ݒ肵�܂�
LSound_SetEmitterPosition sound_handle, -10.0, 5.0, 20.0
LSound_SetEmitterDistance sound_handle, 100.0
    @ln_endcode
@ln_endcodeblock
    <br>
    3D�T�E���h�̍��W��Ԃ͍�����W�n�ł��B<br>
    �ix ���̒l�͍�����E�����ɁAy ���̒l�͉����������ɁAz ���̒l�͎�O�����牓����������ɑ傫���Ȃ�܂��j<br>
    <br>
    �Ȃ��AMIDI �� 3D �����Ƃ��čĐ����邱�Ƃ͂ł��܂���B<br>
    <br>
    <br>
    

    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    @anchor modules1_audio1_anchor5
    <hr>
    @par �Q�[���I�[�f�B�I
    <br>
    @if DOXY_COMMENT ----------------------------------------------------------------------- @endif
    �Q�[���I�[�f�B�I�̓Q�[���p�ɓ��������T�E���h�Đ��@�\�ł��B<br>
    BGM�ASE ���̂��炩���ߗp�ӂ���Ă�����@�ŉ��t���܂��B<br>
    ���t���̈ꎞ��~�Ȃǂׂ̍����ݒ�͂ł��܂��񂪁A�S�̓I�ɃV���v���ɃR�[�f�B���O���邱�Ƃ��ł��܂��B<br>
    <br>
    �Q�[���I�[�f�B�I�ł̓T�E���h���ȉ��̎�ނɕ��ނ��A���ꂼ��̗p�r�Ɋ�Â��ĉ��t���܂��B<br>
    <br>
    - <b>BGM</b> (�w�i���y)<br>
        �Q�[������ BGM �ł��B <br>
        BGM �͂ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ����܂��B<br>
        �V���� BGM �����t�悤�Ƃ����ꍇ�A�Â����̂͂��̏�Œ�~���邩�A<br>
        �t�F�[�h�C�����Ԃ��w�肵���ꍇ�̓N���X�t�F�[�h���Ȃ��特����؂�ւ��܂��B<br>
        <br>
    - <b>BGS</b> (����)<br>
        �����̉��Ȃǂ̊����ł��B<br>
        BGM �Ɠ��l�A�ЂƂ������t���邱�Ƃ��ł��A���[�v�Đ����܂��B<br>
        BGS �� BGM �Ɠ����ɉ��t���邱�Ƃ��ł��܂��B<br>
        <br>
        �܂��AME �̉��t�ɂ��e�����󂯂��A��ɉ��t����܂��B<br>
        <br>
    - <b>ME</b>  (���ʉ��y)<br>
        ���x���A�b�v��A�C�e���擾���̃t�@���t�@�[�����̉��t�Ɏg���܂��B<br>
        ME �͂ЂƂ������t����A���[�v�Đ������Ɉ�x�������t����܂��B<br>
        <br>
        ME �̉��t�J�n���� BGM �����t����Ă����ꍇ�́A<br>
        BGM �� 500 �~���b�����Ĉꎞ��~�����A���̌�� ME �̉��t���J�n���܂��B<br>
        ���̏ꍇ�AME �̉��t�I����� BGM �̉��t���ĊJ���A2000 �~���b������ BGM ���t�F�[�h�C�������Ȃ���ĊJ�����܂��B <br>
        (���̎��Ԃ̓f�t�H���g�̐ݒ�ŁA�K�v�ɉ����ĔC�ӂ̒l��ݒ肷�邱�Ƃ��ł��܂�)<br>
        <br>
        BGS �͂��̉e�����󂯂��A��ɍĐ�����܂��B<br>
        <br>
    - <b>SE</b>  (���ʉ�)<br>
        �Q�[�����̌��ʉ��ł��B
        SE �͓����ɕ����Đ����邱�Ƃ��ł��A�������������t�����ꍇ��
        �d�˂�������ĉ��t����܂��B<br>
        (SE �͕K���I���������ōĐ�����܂��B�T�C�Y�̑傫���t�@�C�������t���邱�Ƃ͔����Ă�������)<br>
        <br>
        �܂��A3D �����Ƃ��ĉ��t���邱�Ƃ��\�ł��B<br>
        <br>
    
    @par
    �ȉ��ɂ��ꂼ��̐������܂Ƃ߂܂��B
    <table>
    <tr>
        <td></td><td>���[�v�Đ�</td><td>�d�ˍ��킹</td><td>�������Ǘ�</td>
    </tr>
    <tr>
        <td>BGM</td><td>��</td><td>�~</td><td>�X�g���[�~���O</td>
    </tr>
    <tr>
        <td>BGS</td><td>��</td><td>�~</td><td>�X�g���[�~���O</td>
    </tr>
    <tr>
        <td>ME</td><td>�~</td><td>�~</td><td>�I��������</td>
    </tr>
    <tr>
        <td>SE</td><td>�~</td><td>��</td><td>�I��������</td>
    </tr>
    </table>
    <br>
    @attention
        <b>[HSP]</b><br>
        ���s�t�@�C���Ƀp�b�N���ꂽ�����t�@�C���́A���ׂăI���������ōĐ�����܂��B<br>
        MP3 �� OGG �`���̉����̓f�R�[�h��̃T�C�Y���̃��������g�����߁A<br>
        �傫���T�C�Y�̃t�@�C�����Đ�����Ƃ��͒��ӂ��Ă��������B<br>
        (MP3 �� OGG �ł́A�t�@�C���T�C�Y�̖� 10�`20 �{�̃��������g���A�f�R�[�h�ɂ����̕��̎��Ԃ��g���܂�)<br>
        
    <br>
    <br>

    <br>
    <br>
*/
//=============================================================================
