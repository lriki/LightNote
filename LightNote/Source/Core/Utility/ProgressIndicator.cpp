
#include "stdafx.h"

/*
// �ǂݍ��݂�ʃX���b�h�ōs���p�^�[��

GameScene
	main
		_battlerManager.beginGameScene();				// ModelData(Mesh?) �Ƃ��񓯊��ł���������
		_view = new GameView();							// LNote �̔񓯊��ǂݍ��݂��g��
		_bw = new BackGroundWorker(() = > {���낢��});	// �����ŃX���b�h���N����
		Progress.Show();	// progressEffect = new ProgressEffect();	// �Z���A�j���܂��� Effect
		do
		{
		} while (Manager.Update() && _view.Loaded && _battlerManager.Loaded && _bw.Finished)
		Progress.Hide();	// progressEffect.Dispose

		// ���C�����[�v
		do
		{
			���������񓯊����[�h�������̂����邪�A(PSO2 �̃v���C���[���f���Ƃ�)
				����͂����܂Łu���o�I�v�ɕK�v�Ȃ��́B
				�����蔻��Ƃ��A�T�C�Y�Ƃ��A�Q�[�����W�b�N�I�ɕK�v�Ȃ��̂͂��ׂă��[�h�ς݁B

		} while (Manager.Update())


Indicator
// �ǂݍ��݂����C���X���b�h�ōs���p�^�[��

GameScene
	main
		BackGroundProgress.Begin();

		Tex.Load();
		bload();

		BackGroundProgress.End();;

// ���_���B���C���X���b�h�̃��b�Z�[�W���[�v������Ă��Ȃ��B
// Ruby �Ƃ��Ȃ牽�Ƃ��Ȃ邯�ǁAHSP �͍ŏ�����UI�X���b�h�ɂȂ��Ă���B
	*/
