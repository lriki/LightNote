���V�������J ReferenceObject �̍���
�@ Core ���̃N���X��`(.h)�� LN_TYPE_INFO_ACCESS_DECL; �}�N�����L�q����B
�A Core ���̃N���X��`(.cpp)�� LN_TYPE_INFO_ACCESS_IMPL(�N���X��); �}�N�����L�q����B
�B C_API ���̃N���X��`(.h)�� LNOTE_TYPE_INFO_DECL(�N���X��); �}�N�����L�q����B
�C C_API ���̃N���X��`(.cpp)�� LNOTE_TYPE_INFO_IMPL(Scene::Camera, LNCamera); �}�N�����L�q����B

���\���̃N���X�̐������[��
�ESet�` (�`�� "Vec2" ���A�I�[�o�[���[�h����������) �́Ainstance �����o�ƂȂ�B
�EGet�` �ŁA�Ō�̈����� out �����̏ꍇ�� getter �v���p�e�B�ƂȂ�B
�EGet�` �ȊO�ŁA�Ō�̈����� out �����̏ꍇ�� static �ȃ��\�b�h�ƂȂ�Aout �����̈������߂�l�ƂȂ�B
�E�O�q�p�^�[�����A�S���� 2 �����Ȃ��ꍇ�� instance ���\�b�h�� static ���\�b�h��2�����������B(slimDX���C�N)
	��) LNVector3_Normalize(vec, outVec)
		�� Vector3 Normalize()		�c this ���g�p
		�� static Vector3 Normalize(vec) 
 

���߂�l�ƃG���[�����ɂ���
	�ECreate�n �� NULL ��Ԃ�����G���[�B�ڍׂ� GetLastError ���g���B
		out �����ɂ͂��Ȃ��́H
			��C++��C#�ŃR���X�g���N�^�������q�ɏ������Ƃ��ł��Ȃ��Ȃ�B
			  ����΂�Β����邪�E�E�E�B
	�E���ےl�� lnBool �ɂ��Ȃ��́H
		�� �o�C���_�����Ƃ��ɁA�u���̖߂�l�����ۂ�����킷�v���Ƃ��킩��Ȃ��Ȃ�B
		   �܂� @return �R�����g����Ƃ���@�����邪�E�E�E�B
	
	���ɂ��uCreate�܂߂Ė߂�n���v			





���̃t�H���_�̃t�@�C�����A���C�u�����̃��[�U�[�Ƀ��C���Ō��J����C���^�[�t�F�C�X�ɂȂ�B

���̂��߁A���[�U�r���e�B����Ɗe�팾��o�C���_���������̊ȈՉ��̂��߁A
�ȉ��̂悤�ȃR�[�f�B���O���[����݂��Ă���B

�� �֐����� �u���W���[����_�@�\���v�ł��邱��
	��)	LSound_Play




��	lnHandle �I�u�W�F�N�g���쐬����֐��� �u���W���[����_Create�`�v�ł��邱��
	lnHandle ��߂�l�Ƃ��邱�̊֐��́A����o�C���_�ŃR���X�g���N�^�Ƃ��ēW�J�����B
	��)	LTexture_CreateFromFile

��	lnHandle �^�̉������́A���������W���[�����Ƃ��邱��
	����o�C���_�͈��������烂�W���[��(�N���X�^)����肷��B
	��) font		�� �S�̂� LFont �ƃ}�b�`	
	��)	sceneNode	�� �S�̂� LSceneNode �ƃ}�b�`
	��)	srcTexture	�� "Texture" �̕����� LTexture �ƃ}�b�`



��	�v���p�e�B getter setter �� @biref �́u�̎擾�v�u�̐ݒ�v�ȊO����v���Ă��邱��

��	Doxygen �R�����g�̒�`���� (@defgroup) �� @brief �� @param �� @return �� @par �� (@{)�ł��邱�ƁB
	����ȊO�͎g���Ȃ��B

��	@param �̌�� [in, out] �͕K���w�肷�邱�ƁB
	C#���̍\���̃N���X�ɕϊ�����Ƃ��A���̎�ނ����ɗl�X�Ȑ��`���s���B
	��1������[in]���܂܂�Ă��� �� ��1������ ref this �Ƃ���C���X�^���X���\�b�h
	��1������[out]�̂�			�� ��1������߂�l�Ƃ��� static �֐�



��	�ϊ��g���u���V���[�e�B���O
	��	[C#] API �̉������� ref �ŁA�N���X���C�u��������̌Ăяo���� out �ɂȂ��Ă��ăR���p�C���G���[
		�� @param ��� [in, out]�̎w�肪�Ԉ���Ă��� 

�� @override
	Doxygen �̐ݒ�ňȉ��̂悤�ɂ��Ă���B
	�EALIASES  = "override=@par Override:\n@endhtmlonly @ref "
	@return ���̓����悤�ɁAOverride: �Z�N�V�����Ń����N�t���ŋ��������B
	C# �ł͂�������� override �L�[���[�h��t�����邽�߁A�x������̂��߂ɕK���w�肷�邱�ƁB
