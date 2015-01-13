//=============================================================================
//
//-----------------------------------------------------------------------------
/**
	@page modules1_intro1 �͂��߂� (LightNote �̓����E���ӓ_)

	<br>�����ł́ALightNote ���g�p���ĊJ�����s���ɓ������āA���ӂ��Ă����ׂ����Ƃɂ��Ă܂Ƃ߂Ă��܂��B
	
    - @ref modules1_intro1_anchor1   "�}���`�R�A�œK��"
    - @ref modules1_intro1_anchor2   "�Í����A�[�J�C�u"
    - @ref modules1_intro1_anchor3   "����o�C���f�B���O�ƃh�L�������g��̕\�L�ɂ���"
    <br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_intro1_anchor1
    @par �}���`�R�A�œK��
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote �̓}���`�R�A CPU �ɑ΂��čœK������Ă��܂��B<br>
    <br>
    �Q�[���ɂ͕`�揈���╨�����Z���A���Ԃ̂����鏈���̂܂Ƃ܂肪���������݂��܂��B<br>
    ���ɕ`�揈���̓V���O���X���b�h���f���̏ꍇ�ɑS�̂̏�����8�����߂�ƌ����Ă��܂��B<br>
    <br>
    LightNote �͂����̏������Q�[���{�̂̏������s�����C���X���b�h�Ƃ͐؂藣���A��p�̃X���b�h��p�ӂ��ĕ��񏈗����s���܂��B<br>
    ����ɂ�胁�C���X���b�h��CPU���\�[�X�͑傫���팸����A�����p�t�H�[�}���X���Q�[�����W�b�N�ɏW�����邱�Ƃ��ł��܂��B<br>
    <br>
    ����ŁA�e�X���b�h�̓��������̂悤�ɃV���O���X���b�h���f���ł͕K�v�̂Ȃ��I�[�o�[�w�b�h�����݂��܂��B<br>
    ���̂��߁A�V���O���R�A CPU �ł͋t�Ƀp�t�H�[�}���X���ቺ����\��������܂��̂ł����ӂ��������B<br>
    (�����̃f�X�N�g�b�v�E�m�[�gPC�͂ق�2�R�A�ȏ㎝���Ă��܂���)<br>
    <br>
    ���݂� LightNote �́A�ڈ��Ƃ��� 4 �R�A�ȏ�� CPU �ōł������p�t�H�[�}���X�𔭊��ł��܂��B<br>
    <br>
    <br>
    �y�ʏ�̕��@�ł̓p�t�H�[�}���X������Ȃ����Ɓz<br>
    �v���t�@�C�����g���B<br>
    �V�X�e���L�[�̏ڍׂ� @ref modules1_config1_anchor2 ���Q�Ƃ��Ă��������B<br>
    
    @if DOXY_COMMENT --------------------------------------------------- @endif
    <hr>
    @anchor modules1_intro1_anchor3
    @par	����o�C���f�B���O�ƃh�L�������g��̕\�L�ɂ���
    <br>
    @if DOXY_COMMENT --------------------------------------------------- @endif
    LightNote �͈ȉ��̃v���O��������ɑΉ����Ă��܂��B<br>
    - C����<br>
    - C++<br>
    - .NET �Ή�����(C���AVisualBasic ��)<br>
    - Ruby<br>
    - HSP (Hot Soup Processor)<br>
    
    @par
    LightNote �̋@�\�̓I�u�W�F�N�g�w���̃X�^�C���ɏ]���č\��������Ă��܂��B<br>
    ���̂��߁A���̃v���O���~���O�K�C�h�̂悤�ȃh�L�������g�ł͕֋X��A���̂悤�ȊT�O�̖�������ɑ΂��Ă�<br>
    �u�N���X�v��u�p���v�Ƃ������L�[���[�h��p���邱�Ƃ�����܂��̂ŁA�����ӂ��������B<br>
    <br>
@htmlonly<div class="ln_lang_tabs"><ul class="ln_tab_menu"><li name="tab1"><a>C
</a></li><li name="HSP_tab"><a>HSP
</a></li></ul><div class="ln_tab_content" name="tab1">
<pre class="brush: cpp">lnHandle sprite

// LSprite�` �̓X�v���C�g�N���X�̊֐��ł��B
LSprite_Create(&sprite);					// �X�v���C�g���쐬���A�X�v���C�g�n���h���𓾂�
LSprite_SetSize(sprite, 10, 10);			// �X�v���C�g�̃T�C�Y��ݒ肷��

// Sprite �� SceneNode ���p�����Ă���A�X�v���C�g�n���h���ɑ΂��� LSceneNode�` �̊֐����g�p�ł��܂��B
LSceneNode_SetPosition(sprite, 10, 20, 0);	// �X�v���C�g�̈ʒu��ݒ肷��
</pre></div><div class="ln_tab_content" name="HSP_tab">
<pre class="brush: cpp">// LSprite�` �̓X�v���C�g�N���X�̖��߂ł��B
LSprite_Create sprite						// �X�v���C�g���쐬���A�X�v���C�g�n���h���𓾂�
LSprite_SetSize sprite, 10, 10				// �X�v���C�g�̃T�C�Y��ݒ肷��

// Sprite �� SceneNode ���p�����Ă���A�X�v���C�g�n���h���ɑ΂��� LSceneNode�` �̖��߂��g�p�ł��܂��B
LSceneNode_SetPosition sprite, 10, 20, 0	// �X�v���C�g�̈ʒu��ݒ肷��
</pre></div></div>@endhtmlonly
    <br>
    <br>


*/
//=============================================================================
