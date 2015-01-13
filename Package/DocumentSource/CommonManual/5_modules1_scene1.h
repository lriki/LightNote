//=============================================================================
// Scene
//-----------------------------------------------------------------------------
/**
	@page modules1_scene1 

	- @ref modules1_scene1_anchor1   "SceneGraph �ɂ���"
	<br>

	@if DOXY_COMMENT --------------------------------------------------- @endif
	<hr>
	@anchor modules1_graphics1_anchor1
	@par SceneGraph �ɂ���
	<br>
	@if DOXY_COMMENT --------------------------------------------------- @endif
	SceneGraph (�V�[���O���t) �́A�X�v���C�g��3D���f�����̕`��I�u�W�F�N�g�̊֌W��A<br>
	�n�C�N�H���e�B�ȃO���t�B�b�N���������邽�߂̕��G�ȕ`��p�X�𐮗�����d�g�݂ł��B<br>
	<br>
	LightNote �����݌��J���Ă���X�v���C�g�Ȃǂ̕`��@�\�́A���� SceneGraph �Ɋ܂܂��@�\�ł��B<br>
	<br>
	LightNote �́A�ȒP�ȃA�v���ł���� SceneGraph �̑��݂�����قǈӎ������Ƃ��v���O������g�ނ��Ƃ��ł��܂��B<br>
	������ SceneGraph �ɂ��ė�������΁A�Ⴆ��3D���f����G�t�F�N�g��2D��Ԃɔz�u���ĕ`�悷�铙�A�\���̕����L���邱�Ƃ��ł��܂��B<br>
	<br>
	
	
	SceneGraph �̃g�b�v���x���\���v�f
	
	- Node 
	Node �͕`���� (2D�܂���3D���) �ɔz�u�����I�u�W�F�N�g�ł��B<br>
	�X�v���C�g�A���f���A�J�����A���C�g��������܂��B<br>
	<br>
	Node �͊K�w�\���ŊǗ�����܂��B<br>
	���� Node �̃v���p�e�B���ς��ƁA�q Node �̃v���p�e�B�ɂ��ύX���`�d���܂��B<br>
	�Ⴆ�΁A�uA����������B���ꏏ�ɓ����v�̂悤�� Node �Ԃ̈ʒu�̘A�����ȒP�Ɏ������邱�Ƃ��ł��܂��B
	
	
	
	- Layer
	Layer �� SceneGraph �̉��z�I�ȕ`���ł��B<br>
	�`�悷�� Node �ƃJ�������w�肷�邱�Ƃŕ`����s���܂��B<br>
	<br>
	3D���C���[��2D���C���[ ��2��ނ��A�`���Ƃ��ėp�ӂ���Ă��܂��B<br>
	�ŏI�I�ɂ� ViewPane ��ʂ��ăy�C���g�\�t�g�̃��C���[�@�\�̂悤�ɏd�ˍ��킹���A�����_�����O�^�[�Q�b�g (�E�B���h�E��) �ɕ`����s���܂��B<br>
	<br>
	�܂��A��ʑS�̂Ƀu���[��F���ύX���̃G�t�F�N�g���|����A������u�|�X�g�G�t�F�N�g�v�̂��߂̃��C���[���p�ӂ���Ă��܂��B<br>
	
	- ViewPane
	Node �� Layer�A���̑��̗l�X�ȗv�f�����܂Ƃ߂āASceneGraph �̍ŏI�I�ȕ`�挋�ʂ𐶐����A�����_�����O�^�[�Q�b�g (�E�B���h�E��) �ɕ`����s���܂��B<br>
	
	SceneGraph �̃g�b�v���x���T�O�}
	@image html img/5_modules1_scene1_1.png
	
	
	
	
	
	�f�t�H���g�ō쐬�����I�u�W�F�N�g
	
	@image html img/5_modules1_scene1_2.png
	
	- Layer
		3D �p�� 2D �p�̃��C���[���쐬����܂��B<br>
		2D ���C���[�� 3D ���C���[�̎�O�ɔz�u����܂��B<br>
		���ꂼ��̃��C���[�͕`�悷��m�[�h�ƁA���̕`���Ԃ�\�����邽�߂̎��_�ƂȂ� Camera ���Q�Ƃ��܂��B<br>
	
	- 3D/2D RootNode
		���ꂼ��̕`���Ԃ̃��[�g�m�[�h�ł��B<br>
		�`��I�u�W�F�N�g�͍쐬���ɂ��ꂼ�ꌈ�߂�ꂽ���[�g�m�[�h�̎q�m�[�h�Ƃ��Ēǉ�����܂��B<br>
		Model �I�u�W�F�N�g�� Effect �I�u�W�F�N�g���� 3DRootNode�A<br>
		Sprite ���� 2DRootNode �̎q�m�[�h�ƂȂ�܂��B

	- Camera
		3D �܂��� 2D ��Ԃ�\������ۂ̎��_�ł��B3D �܂��� 2D ��ԓ��ɔz�u�����I�u�W�F�N�g�Ƃ��Ĉ����܂��B<br>
		2D �Ƃ��Ă͌����ɂ͕K�v�̂Ȃ��T�O�ł����ALightNote �����ł� 3D �Ɠ����`��p�X���g�p���邽�ߗp�ӂ���Ă��܂��B<br>
		2D �J�����𑀍삷�邱�ƂŁA��ʑS�̂𕽍s�ړ�������A��]�E�g�k�����邱�Ƃ��ł��܂��B<br>
	
	Node �̕`��ɂ���
	
	�ꊇ�`��
	
	
	
	
	
	
	
	
	
	
	
	

	<br>
	<br>
*/
//=============================================================================