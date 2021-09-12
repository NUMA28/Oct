#include "DxLib.h"
#include <time.h>
#include <math.h>

const char TITLE[] = "Oct";

const int WIN_WIDTH = 1440; //�E�B���h�E����
const int WIN_HEIGHT = 960;//�E�B���h�E�c��


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//�E�B���h�E���[�h�ɐݒ�
	//�E�B���h�E�T�C�Y���蓮�ł͕ύX�ł����A���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// �^�C�g����ύX
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�i���j�^�[�̉𑜓x�ɍ��킹��j
	SetWindowSizeExtendRate(1.0);				//��ʃT�C�Y��ݒ�i�𑜓x�Ƃ̔䗦�Őݒ�j
	SetBackgroundColor(0, 0, 0);		// ��ʂ̔w�i�F��ݒ肷��

	//Dx���C�u�����̏�����
	if (DxLib_Init() == -1) { return -1; }

	//�i�_�u���o�b�t�@�j�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	//�摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���
	int playerX = 672; //�v���C���[�����ʒuX���W
	int playerY = 576; //�v���C���[�����ʒuY���W

	int right = LoadGraph("Image\\Yumihei teisi migimuki.png");
	int left = LoadGraph("Yumihei teisi hidarimuki.png");

	int rightstanding[5];//�v���C���[�E�����������[�V�����G
	LoadDivGraph("Image\\Yumihei migi standing.png", 4, 4, 1, 72, 96, rightstanding);

	int leftstanding[5];//�v���C���[�������������[�V�����G
	LoadDivGraph("Image\\Yumihei hidari standing.png", 4, 4, 1, 72, 96, leftstanding);

	int rightmoving[5]; //�v���C���[�E�����������[�V�����G
	LoadDivGraph("Image\\Yumihei migi moving.png", 4, 4, 1, 72, 96, rightmoving);

	int leftmoving[5]; //�v���C���[�E�����������[�V�����G
	LoadDivGraph("Image\\Yumihei hidari moving.png", 4, 4, 1, 72, 96, leftmoving);

	int hasigo1 = LoadGraph("Image\\Yumihei hasigo 1.png");

	int hasigo2 = LoadGraph("Image\\Yumihei hasigo 2.png");

	//�v���C���[�̏�ԕϐ�
	int player = 0;

	//�|��[�|�C���g�̏�ԕϐ�
	int arrowbox1 = 0;
	int arrowbox2 = 0;

	//�����|��ϐ�
	int getarrow = 5;

	//////////////�|��ϐ�//////////////////
	//////�E����//////
	int rightarrowX[5];//X���W�錾
	for (int i = 0; i < 5; i++)
	{
		rightarrowX[i] = 500;
	}

	int rightarrowY[5];//Y���W�錾
	for (int i = 0; i < 5; i++)
	{
		rightarrowY[i] = -100;
	}

	int rightarrowflag[5];//�t���O�錾
	for (int i = 0; i < 5; i++)
	{
		rightarrowflag[i] = 0;//������
	}

	///////������////////
	int leftarrowX[5];//X���W�錾
	for (int i = 0; i < 5; i++)
	{
		leftarrowX[i] = 500;
	}

	int leftarrowY[5];//Y���W�錾
	for (int i = 0; i < 5; i++)
	{
		leftarrowY[i] = -100;
	}

	int leftarrowflag[5];//�t���O�錾
	for (int i = 0; i < 5; i++)
	{
		leftarrowflag[i] = 0;//������
	}

	int arrowspeed = 7;

	int rightarrow = LoadGraph("Image\\Yumiya1.png");
	int leftarrow = LoadGraph("Image\\Yumiya2.png");

	//�ԑ̗�
	int fortlife = 10;

	//�V�[��
	int scene = 0;

	//�A�j���[�V�����^�C�}�[
	int playertime = 0;
	int arrowbox1time = 0;
	int arrowbox2time = 0;
	int scrollX = 0;

	//�ˌ��N�[���^�C��
	int rightshotBflag = 0;
	int leftshotBflag = 0;

	int block[14];
	LoadDivGraph("Image\\block.png", 14, 14, 1, 96, 96, block);
	//���ꓙ�̉摜��96*96��5���ɕ������ēǂݍ���

	int life[11];
	LoadDivGraph("Image\\Fort life.png", 11, 3, 4, 348, 42, life);
	int lifeX = 546;
	int lifeY = 45;

	int stage = LoadGraph("Image\\Stage.png");//�w�i
	int cloud = LoadGraph("Image\\Cloud.png");
	int backsky = LoadGraph("Image\\back sky.png");

	int HUDarrow[6];
	LoadDivGraph("Image\\HUD arrow.png", 6, 6, 1, 180, 84, HUDarrow);

	int title = LoadGraph("Image\\Title.png");

	int Howto = LoadGraph("Image\\HowTo.png");

	/////////////�T�E���h�t�@�C��//////////////////
	int shotSE = LoadSoundMem("Sound\\shot sound.wav");

	int arrowgetSE = LoadSoundMem("Sound\\Arrow Get.wav");
	ChangeVolumeSoundMem(255 * 38 / 100, arrowgetSE);

	int opening = LoadSoundMem("Sound\\Opening.mp3");
	ChangeVolumeSoundMem(255 * 35 / 100, opening);

	int SA = LoadSoundMem("Sound\\Sulime_Attack.mp3");
	ChangeVolumeSoundMem(255 * 35 / 100, SA);

	int SkyA = LoadSoundMem("Sound\\Sky_Attack.mp3");
	ChangeVolumeSoundMem(255 * 30 / 100, SkyA);

	int DA = LoadSoundMem("Sound\\Dragon_Attack.mp3");
	ChangeVolumeSoundMem(255 * 10 / 100, DA);

	int explo = LoadSoundMem("Sound\\Hinotama Hit.wav");
	ChangeVolumeSoundMem(255 * 38 / 100, explo);

	int maintheme = LoadSoundMem("Sound\\main theme.mp3");
	ChangeVolumeSoundMem(255 * 35 / 100, maintheme);

	int gameoverBGM = LoadSoundMem("Sound\\Game Over.mp3");
	ChangeVolumeSoundMem(255 * 35 / 100, gameoverBGM);

	int hitSE = LoadSoundMem("Sound\\Hit.wav");
	ChangeVolumeSoundMem(255 * 35 / 100, hitSE);

	//�Q�[�����[�v�Ŏg���ϐ��̐錾
	char keys[256] = { 0 }; //�ŐV�̃L�[�{�[�h���p
	char oldkeys[256] = { 0 };//1���[�v�i�t���[���j�O�̃L�[�{�[�h���

	///////�}�b�v�`�b�v///////
	int mapdata[11][16] =
	{
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,0,0,0,1,0,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,2,3,13, 13,13,13,13},// 0�F����
		{13,13,13,13, 13,0,1,0,0,0,13, 13,13,13,13},// 1�F���ꁕ��q
		{13,13,13,13, 13,4,2,4,4,4,13, 13,13,13,13},// 2�F��q
		{13,13,13,13, 13,0,0,0,1,0,13, 13,13,13,13},// 3�F�|�����
		{13,13,13,13, 13,3,4,4,2,4,13, 13,13,13,13},// 4�F��
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},// 5�F���
	};

	int GameOver = LoadGraph("Image\\Game Over.png");//�Q�[���I�[�o�[���

	//----------------------------�G�̕ϐ�-------------------------------------------
	int Spoon = 0; //�ǂ̃��[���ŃX�|�[�����邩���߂�ϐ�
	int Monster = 0;

	int timer = 0;
	int EnemyTimer = 0;
	int Attack_Timer[8] = { 0,0,0,0,0,0,0,0 };

	int Dragon_HP[6]{ 0,0,0,0,0,0 };//�h���S����HP

	int Alive[8][3] = {                 //�G�l�~�[�̐����t���O
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
	};
	//----------------��--------------�E------------------------------
	int EnemyX[8] = { -192,-192,-192,-192,1440,1440,1440,1440 };       //�G�l�~�[�̍��[

	//-------------1�i-2�i-3�i-4�i---------------------
	int EnemyY[4] = { 768,576,384,192 };

	int an = 0;//----------------------------------------�A�j���[�V�����p�ϐ�--------------------------------------------------------------------------------------
	int an4 = 0;
	int an6 = 0;
	int an10 = 0;
	int anD = 0;

	int lane[8] = { 0,0,0,0,0,0,0,0 }; //------------���[���Ƀ����X�^�[���o�Ă��邩�ǂ���

	int score = 0;

	//----------------------------------------------------�����X�^�[�̉摜�ǂݍ���----------------------------------------------------------------------------------------------------

	int Slime_L_G[2];
	LoadDivGraph("Image\\Slime1.png", 2, 2, 1, 96, 96, Slime_L_G);//�X���C���΁u���v

	int Slime_A_L[6];
	LoadDivGraph("Image\\Slime_A.png", 6, 6, 1, 96, 96, Slime_A_L);//�X���C���΁u���v�U��

	int Slime_L_R[2];
	LoadDivGraph("Image\\Slime_Red1.png", 2, 2, 1, 96, 96, Slime_L_R);//�X���C���ԁu���v

	int Slime_R_A_L[6];
	LoadDivGraph("Image\\Slime_Red_A.png", 6, 6, 1, 96, 96, Slime_R_A_L);//�X���C���ԁu���v�U��

	int Slime_L_B[2];
	LoadDivGraph("Image\\Slime_Blue1.png", 2, 2, 1, 96, 96, Slime_L_B);//�X���C���u���v

	int Slime_B_A_L[6];
	LoadDivGraph("Image\\Slime_Blue_A.png", 6, 6, 1, 96, 96, Slime_B_A_L);//�X���C���u���v�U��

	//---------------------------�u�E�v--------------------------------------------------------------------------------------------------------------------------

	int Slime_R_G[2];
	LoadDivGraph("Image\\Slime_R.png", 2, 2, 1, 96, 96, Slime_R_G);  //�X���C���΁u�E�v

	int Slime_A_R[6];
	LoadDivGraph("Image\\Slime_A_R.png", 6, 6, 1, 96, 96, Slime_A_R);  //�X���C���΁u���v�U��

	int Slime_R_R[2];
	LoadDivGraph("Image\\Slime_Red_R.png", 2, 2, 1, 96, 96, Slime_R_R);  //�X���C���ԁu�E�v

	int Slime_R_A_R[6];
	LoadDivGraph("Image\\Slime_Red_A_R.png", 6, 6, 1, 96, 96, Slime_R_A_R);  //�X���C���ԁu���v�U��

	int Slime_R_B[2];
	LoadDivGraph("Image\\Slime_Blue_R.png", 2, 2, 1, 96, 96, Slime_R_B);  //�X���C���u�E�v

	int Slime_B_A_R[6];
	LoadDivGraph("Image\\Slime_Blue_A_R.png", 6, 6, 1, 96, 96, Slime_B_A_R);  //�X���C���u���v�U��

		//---------------------------�u�󒆁v--------------------------------------------------------------------------------------------------------------------------

	int Slime_Sky[4];
	LoadDivGraph("Image\\Slime_Sky1.png", 4, 4, 1, 192, 96, Slime_Sky);  //�X�J�C�X���C��

	int Slime_Sky_R[4];
	LoadDivGraph("Image\\Slime_Sky_R.png", 4, 4, 1, 192, 96, Slime_Sky_R);  //�X�J�C�X���C���E

	int Slime_Sky_A[6];
	LoadDivGraph("Image\\Slime_Sky_A.png", 6, 6, 1, 192, 96, Slime_Sky_A);  //�X�J�C�X���C�����U��

	int Slime_Sky_A_R[6];
	LoadDivGraph("Image\\Slime_Sky_A_R.png", 6, 6, 1, 192, 96, Slime_Sky_A_R);  //�X�J�C�X���C���E�U��

	int Fire[2];
	LoadDivGraph("Image\\Fire.png", 2, 2, 1, 96, 96, Fire);  //�q�m�^�}

	int Fire_R[2];
	LoadDivGraph("Image\\Fire_R.png", 2, 2, 1, 96, 96, Fire_R);  //�q�m�^�}�E

	int exp[10];
	LoadDivGraph("Image\\explosion.png", 10, 10, 1, 72, 72, exp);  //����

	int Dragon[4];
	LoadDivGraph("Image\\Dragon1.png", 4, 4, 1, 288, 96, Dragon);  //�h���S��

	int Dragon_A[6];
	LoadDivGraph("Image\\Dragon_A1.png", 6, 6, 1, 288, 96, Dragon_A);  //�h���S���U��

	int Dragon_D[4];
	LoadDivGraph("Image\\Dragon_D1.png", 4, 4, 1, 288, 96, Dragon_D);  //�h���S�� �_���[�W�L��

	int Dragon_A_D[6];
	LoadDivGraph("Image\\Dragon_A_D1.png", 6, 6, 1, 288, 96, Dragon_A_D);  //�h���S�� �_���[�W�L�� �U��

	int Dragon_R[4];
	LoadDivGraph("Image\\Dragon_R.png", 4, 4, 1, 288, 96, Dragon_R);  //�h���S�� �E

	int Dragon_A_R[6];
	LoadDivGraph("Image\\Dragon_A_R.png", 6, 6, 1, 288, 96, Dragon_A_R);  //�h���S�� �E�@�U��

	int Dragon_R_D[4];
	LoadDivGraph("Image\\Dragon_D_R.png", 4, 4, 1, 288, 96, Dragon_R_D);  //�h���S�� �E �_���[�W�L��

	int Dragon_A_D_R[6];
	LoadDivGraph("Image\\Dragon_A_D_R.png", 6, 6, 1, 288, 96, Dragon_A_D_R);  //�h���S�� �E�@�U��

	int Flag1 = 1;
	int Flag2 = 1;
	int Flag3 = 1;

	// �Q�[�����[�v
	while (1)
	{
		//�ŐV�̃L�[�{�[�h��񂾂������̂͂P�t���[���O�̃L�[�{�[�h���Ƃ��ĕۑ�

		//�ŐV�̃L�[�{�[�h�����擾
		GetHitKeyStateAll(keys);

		//��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		//�X�V����
		switch (scene)
		{
		case 0://�^�C�g����ʁi�X�g�[���[���[�r�[�j

			if (Flag1 == 1)
			{
				StopSoundMem(gameoverBGM);

				if (CheckSoundMem(opening) == 0)
				{
					PlaySoundMem(opening, DX_PLAYTYPE_BACK);
					PlayMovie("Movie\\Opening.mp4", 1, DX_MOVIEPLAYTYPE_BCANCEL);
				}
			}

			DrawGraph(0, 0, backsky, TRUE);
			DrawGraph(scrollX, 0, cloud, TRUE);
			DrawGraph(scrollX - WIN_WIDTH * 2, 0, cloud, TRUE);

			DrawGraph(0, 0, title, TRUE);

			if (scene == 0)
			{
				scrollX = scrollX + 1;

				if (scrollX >= WIN_WIDTH * 2)
				{
					scrollX = 0;
				}
			}

			if (keys[KEY_INPUT_Z] == 1)
			{
				scene = 1;

				//�����l�ݒ�
				playerX = 672;
				playerY = 576;

				player = 0;
				arrowbox1 = 0;
				arrowbox2 = 0;
				getarrow = 5;
				fortlife = 10;
				playertime = 0;
				arrowbox1time = 0;
				arrowbox2time = 0;
				scrollX = 0;
				rightshotBflag = 0;
				leftshotBflag = 0;
				timer = 0;

				an = 0;
				an4 = 0;
				an6 = 0;
				an10 = 0;

				for (int i = 0; i < 8; i++)
				{
					Attack_Timer[i] = 0;
				}

				for (int i = 0; i < 6; i++)
				{
					Dragon_HP[i] = 0;
				}
				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						Alive[i][j] = 0;
					}
				}
				for (int i = 0; i < 8; i++)
				{
					lane[i] = 0;
				}

				for (int i = 0; i < 4; i++)
				{
					EnemyX[i] = -192;
				}
				for (int i = 0; i < 4; i++)
				{
					EnemyX[i + 4] = 1440;
				}

				EnemyY[0] = 768;
				EnemyY[1] = 576;
				EnemyY[2] = 384;
				EnemyY[3] = 192;

				score = 0;
			}

			if (keys[KEY_INPUT_X] == 1)
			{
				scene = 2;
			}

			break;

		case 1: //�Q�[���v���C

			if (Flag2 == 1)
			{
				StopSoundMem(opening);

				if (CheckSoundMem(maintheme) == 0)
				{
					PlaySoundMem(maintheme, DX_PLAYTYPE_BACK);
				}
			}

			DrawGraph(0, 0, backsky, TRUE);
			DrawGraph(scrollX, 0, cloud, TRUE);
			DrawGraph(scrollX - WIN_WIDTH * 2, 0, cloud, TRUE);

			//�`�揈��
			for (int x = 0; x < 15; x++)
			{
				for (int y = 0; y < 10; y++)
				{
					DrawGraph((x * 96), (y * 96), block[mapdata[y][x]], TRUE);
				}
			}

			////////////�v���C���[�ړ�///////////

			////////////////��ړ�////////////////////
			if ((keys[KEY_INPUT_UP] == 1) &&
				(mapdata[(playerY - 6) / 96][playerX / 96] >= 1) &&
				(mapdata[(playerY - 6) / 96][(playerX + 72) / 96] >= 1))//��ړ�
			{
				//�v���C���[��4�K�̏��Ŏ~�܂鏈��
				if (playerY > 192)
				{
					playerY -= 6;
				}

				if ((mapdata[(playerY + 90) / 96][playerX / 96] <= 2) &&
					(mapdata[(playerY + 90) / 96][(playerX + 72) / 96] <= 2))
				{
					player = 3;

					if (playertime < 10)
					{
						DrawGraph(playerX, playerY, hasigo2, TRUE);
					}

					if ((playertime >= 10) && (playertime < 20))
					{
						DrawGraph(playerX, playerY, hasigo1, TRUE);
					}

					if ((playertime >= 20) && (playertime < 30))
					{
						DrawGraph(playerX, playerY, hasigo2, TRUE);
					}

					if ((playertime >= 30) && (playertime < 40))
					{
						DrawGraph(playerX, playerY, hasigo1, TRUE);
					}
				}

				else
				{
					player = 1;
				}
			}

			////////////////���ړ�////////////////////
			if (keys[KEY_INPUT_DOWN] == 1)//���������ꂽ�Ƃ�
			{
				//��q���v���C���[�̉��ɂ������ꍇ�̂ݍ쓮�i�����������ꍇ������Ȃ������j
				if ((mapdata[(playerY + 96) / 96][playerX / 96] >= 1) &&
					(mapdata[(playerY + 96) / 96][(playerX + 72) / 96] >= 1))
				{
					//�v���C���[���n�ʂŎ~�܂鏈��
					if (playerY < 768)
					{
						playerY += 6;
					}
				}

				if ((mapdata[(playerY + 90) / 96][playerX / 96] <= 2) &&
					(mapdata[(playerY + 90) / 96][(playerX + 72) / 96] <= 2))
				{
					player = 4;

					if (playertime < 10)
					{
						DrawGraph(playerX, playerY, hasigo2, TRUE);
					}

					if ((playertime >= 10) && (playertime < 20))
					{
						DrawGraph(playerX, playerY, hasigo1, TRUE);
					}

					if ((playertime >= 20) && (playertime < 30))
					{
						DrawGraph(playerX, playerY, hasigo2, TRUE);
					}

					if ((playertime >= 30) && (playertime < 40))
					{
						DrawGraph(playerX, playerY, hasigo1, TRUE);
					}
				}
			}


			////////////////�E�ړ�////////////////////
			if (keys[KEY_INPUT_RIGHT] == 1)
			{
				//��q���牡�ړ��ł��Ȃ��悤�ɂ��鏈��
				if ((mapdata[playerY / 96][(playerX + 72) / 96] >= 2) &&
					(mapdata[(playerY + 90) / 96][(playerX + 72) / 96] >= 2))
				{
					//�E���̏�ǂɂԂ��鏈��
					if (playerX + 71 < 959)
					{
						playerX += 6;
					}
				}

				if ((playerY == 192) || (playerY == 384) ||
					(playerY == 576) || (playerY == 768))
				{
					player = 0;

					if (playertime < 10)
					{
						DrawGraph(playerX, playerY, rightmoving[0], TRUE);
					}

					if ((playertime >= 10) && (playertime < 20))
					{
						DrawGraph(playerX, playerY, rightmoving[1], TRUE);
					}

					if ((playertime >= 20) && (playertime < 30))
					{
						DrawGraph(playerX, playerY, rightmoving[2], TRUE);
					}

					if ((playertime >= 30) && (playertime < 40))
					{
						DrawGraph(playerX, playerY, rightmoving[3], TRUE);
					}
				}
			}

			if (keys[KEY_INPUT_LEFT] == 1)//���ړ�
			{
				//��q���牡�ړ��ł��Ȃ��悤�ɂ��鏈��
				if ((mapdata[playerY / 96][playerX / 96] >= 2) &&
					(mapdata[(playerY + 90) / 96][playerX / 96] >= 2))
				{
					//�����̏�ǂɂԂ��鏈��
					if (playerX > 480)
					{
						playerX -= 6;
					}
				}

				if ((playerY == 192) || (playerY == 384) ||
					(playerY == 576) || (playerY == 768))
				{
					player = 1;

					if (playertime < 10)
					{
						DrawGraph(playerX, playerY, leftmoving[0], TRUE);
					}

					if ((playertime >= 10) && (playertime < 20))
					{
						DrawGraph(playerX, playerY, leftmoving[1], TRUE);
					}

					if ((playertime >= 20) && (playertime < 30))
					{
						DrawGraph(playerX, playerY, leftmoving[2], TRUE);
					}

					if ((playertime >= 30) && (playertime < 40))
					{
						DrawGraph(playerX, playerY, leftmoving[3], TRUE);
					}
				}
			}

			////////////�E�����e����//////////////
			if (keys[KEY_INPUT_SPACE] == 1 && player == 0 && getarrow > 0)
			{
				if (rightshotBflag == 0)
				{
					for (int i = 0; i < 5; i++)
					{
						if (rightarrowflag[i] == 0)
						{
							rightarrowflag[i] = 1;
							rightarrowX[i] = playerX + 24;
							rightarrowY[i] = playerY + 48;
							getarrow -= 1;

							PlaySoundMem(shotSE, DX_PLAYTYPE_BACK, TRUE);
							break;
						}
					}
				}
				rightshotBflag = 1;
			}

			else
			{
				rightshotBflag = 0;
			}

			for (int i = 0; i < 5; i++)
			{
				if (rightarrowflag[i] == 1)
				{
					rightarrowX[i] += 12;

					if (rightarrowX[i] > WIN_WIDTH)
					{
						rightarrowX[i] = 500;
						rightarrowflag[i] = 0;
					}
				}
			}

			if ((playerY + 48) / 96 == 4 && (playerX + 36) / 96 == 9
				&& arrowbox1 == 0 && getarrow < 5)
			{
				getarrow = 5;
				arrowbox1 = 1;
			}

			if ((playerY + 48) / 96 == 8 && (playerX + 36) / 96 == 5
				&& arrowbox2 == 0 && getarrow < 5)
			{
				getarrow = 5;
				arrowbox2 = 1;
			}

			////////////�������e����//////////////
			if (keys[KEY_INPUT_SPACE] == 1 && player == 1 && getarrow > 0)
			{
				if (leftshotBflag == 0)
				{
					for (int i = 0; i < 5; i++)
					{
						if (leftarrowflag[i] == 0)
						{
							leftarrowflag[i] = 1;
							leftarrowX[i] = playerX + 24;
							leftarrowY[i] = playerY + 48;
							getarrow -= 1;

							PlaySoundMem(shotSE, DX_PLAYTYPE_BACK, TRUE);
							break;
						}
					}
				}
				leftshotBflag = 1;
			}

			else
			{
				leftshotBflag = 0;
			}

			for (int i = 0; i < 5; i++)
			{
				if (leftarrowflag[i] == 1)
				{
					leftarrowX[i] -= 12;

					if (leftarrowX[i] < -60)//��ʊO�ɍs�������̏���
					{
						leftarrowX[i] = 500;
						leftarrowflag[i] = 0;
					}
				}
			}

			///////�v���C���[�̏�ԑJ�ڂɂ��`��////////
			if ((player == 0) && (keys[KEY_INPUT_RIGHT] == 0))//�E�����Î~��
			{
				if (playertime < 10)
				{
					DrawGraph(playerX, playerY, rightstanding[0], TRUE);
				}

				if ((playertime >= 10) && (playertime < 20))
				{
					DrawGraph(playerX, playerY, rightstanding[1], TRUE);
				}

				if ((playertime >= 20) && (playertime < 30))
				{
					DrawGraph(playerX, playerY, rightstanding[2], TRUE);
				}

				if ((playertime >= 30) && (playertime < 40))
				{
					DrawGraph(playerX, playerY, rightstanding[3], TRUE);
				}
			}

			if ((player == 1) && (keys[KEY_INPUT_LEFT] == 0))//�������Î~��
			{
				if (playertime < 10)
				{
					DrawGraph(playerX, playerY, leftstanding[0], TRUE);
				}

				if ((playertime >= 10) && (playertime < 20))
				{
					DrawGraph(playerX, playerY, leftstanding[1], TRUE);
				}

				if ((playertime >= 20) && (playertime < 30))
				{
					DrawGraph(playerX, playerY, leftstanding[2], TRUE);
				}

				if ((playertime >= 30) && (playertime < 40))
				{
					DrawGraph(playerX, playerY, leftstanding[3], TRUE);
				}
			}

			if ((player == 3) && (keys[KEY_INPUT_UP] == 0))//����͎�
			{
				DrawGraph(playerX, playerY, hasigo1, TRUE);
			}

			if ((player == 4) && (keys[KEY_INPUT_DOWN] == 0))//�����͎�
			{
				DrawGraph(playerX, playerY, hasigo2, TRUE);
			}


			if (arrowbox1 == 1)//���W[4][9]
			{
				arrowbox1time += 1;

				if (arrowbox1time == 1)
				{
					PlaySoundMem(arrowgetSE, DX_PLAYTYPE_BACK, TRUE);
				}

				if ((arrowbox1time >= 0) && (arrowbox1time < 25))
				{
					mapdata[4][9] = 5;
				}

				if ((arrowbox1time >= 25) && (arrowbox1time < 50))
				{
					mapdata[4][9] = 6;
				}

				if ((arrowbox1time >= 50) && (arrowbox1time < 75))
				{
					mapdata[4][9] = 7;
				}

				if ((arrowbox1time >= 75) && (arrowbox1time < 100))
				{
					mapdata[4][9] = 8;
				}

				if ((arrowbox1time >= 100) && (arrowbox1time < 125))
				{
					mapdata[4][9] = 9;
				}

				if ((arrowbox1time >= 125) && (arrowbox1time < 150))
				{
					mapdata[4][9] = 10;
				}

				if ((arrowbox1time >= 150) && (arrowbox1time < 175))
				{
					mapdata[4][9] = 11;
				}

				if ((arrowbox1time >= 175) && (arrowbox1time < 200))
				{
					mapdata[4][9] = 12;
				}

				if (arrowbox1time == 200)
				{
					arrowbox1time = 0;
					arrowbox1 = 0;
					mapdata[4][9] = 3;
				}
			}

			if (arrowbox2 == 1)//���W[4][9]
			{
				arrowbox2time += 1;

				if (arrowbox2time == 1)
				{
					PlaySoundMem(arrowgetSE, DX_PLAYTYPE_BACK, TRUE);
				}

				if ((arrowbox2time >= 0) && (arrowbox2time < 25))
				{
					mapdata[8][5] = 5;
				}

				if ((arrowbox2time >= 25) && (arrowbox2time < 50))
				{
					mapdata[8][5] = 6;
				}

				if ((arrowbox2time >= 50) && (arrowbox2time < 75))
				{
					mapdata[8][5] = 7;
				}

				if ((arrowbox2time >= 75) && (arrowbox2time < 100))
				{
					mapdata[8][5] = 8;
				}

				if ((arrowbox2time >= 100) && (arrowbox2time < 125))
				{
					mapdata[8][5] = 9;
				}

				if ((arrowbox2time >= 125) && (arrowbox2time < 150))
				{
					mapdata[8][5] = 10;
				}

				if ((arrowbox2time >= 150) && (arrowbox2time < 175))
				{
					mapdata[8][5] = 11;
				}

				if ((arrowbox2time >= 175) && (arrowbox2time < 200))
				{
					mapdata[8][5] = 12;
				}

				if (arrowbox2time == 200)
				{
					arrowbox2time = 0;
					arrowbox2 = 0;
					mapdata[8][5] = 3;
				}
			}

			playertime += 1;

			if (playertime == 40)
			{
				playertime = 0;
			}

			if (scene == 1)
			{
				scrollX = scrollX + 1;

				if (scrollX >= WIN_WIDTH * 2)
				{
					scrollX = 0;
				}
			}

			//////////�w�i/////////
			DrawGraph(0, 0, stage, TRUE);//�w�i�摜�\��

			for (int i = 0; i < 7; ++i)
			{
				if (getarrow == i)
				{
					DrawGraph(1221, 18, HUDarrow[i], TRUE);
				}
			}

			for (int i = 0; i < 11; ++i)
			{
				if (fortlife == i)
				{
					DrawGraph(lifeX, lifeY, life[i], TRUE);
				}
			}

			///////�|��/////////
			for (int i = 0; i < 5; i++)
			{
				if (rightarrowflag[i] == 1)
				{
					DrawGraph(rightarrowX[i], rightarrowY[i], rightarrow, TRUE);//�\��
				}

				if (leftarrowflag[i] == 1)
				{
					DrawGraph(leftarrowX[i], leftarrowY[i], leftarrow, TRUE);//�\��
				}
			}

			//---------------------�G�X�|�[���̏���-----------------------------------------------------------------------------------------------------------------------------------------

			timer += 1;

			EnemyTimer += 1; //--------------------------------�G�l�~�[�̃A�j���[�V�����p�^�C�}�[----------------------------------------------------------
			if (EnemyTimer == 40)EnemyTimer = 0;

			if (EnemyTimer % 8 == 0)	//----------------------�G�l�~�[�̃A�j���[�V�����p�ϐ�----------------------------------------------------------------
			{
				an++;
				if (an == 2)
				{
					an = 0;
				}
			}

			if (EnemyTimer % 8 == 0)
			{
				an4++;
				if (an4 == 4)
				{
					an4 = 0;
				}
			}

			if (EnemyTimer % 8 == 0)
			{
				an6++;
				if (an6 == 6)
				{
					an6 = 0;
				}
			}

			if (EnemyTimer % 8 == 0)
			{
				an10++;
				if (an10 == 10)
				{
					an10 = 0;
				}
			}

			if (EnemyTimer % 8 == 0)
			{
				anD++;
				if (anD == 6)
				{
					anD = 2;
				}
			}

			if (timer == 100) //---------------------------�^�C�}�[�����܂�ƃ����X�^�[���X�|�[������--------------------------------------------------------------
			{
				srand((unsigned)time(NULL));
				Spoon = rand() % 8;	//-------------------------�ǂ̃��[���ŃX�|�[�����邩���߂�-------------------------------------------------------------


				if (Spoon == 0 && lane[0] == 0)//------------------------�����ŃX�|�[������------------------------------------------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //------------------------�����X�^�[�̎�ނ�I��----------------------------------------------------

					Alive[0][Monster] = 1;

					lane[0] = 1;//-------------���̃��[���ɂ̓����X�^�[���X�|�[�����Ă��܂�
				}

				if (Spoon == 1 && lane[1] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[1][Monster] = 1;

					lane[1] = 1;//-------------���̃��[���ɂ̓����X�^�[���X�|�[�����Ă��܂�
				}

				if (Spoon == 2 && lane[2] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[2][Monster] = 1;

					lane[2] = 1;//-------------���̃��[���ɂ̓����X�^�[���X�|�[�����Ă��܂�
				}

				if (Spoon == 3 && lane[3] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[3][Monster] = 1;

					lane[3] = 1;//-------------���̃��[���ɂ̓����X�^�[���X�|�[�����Ă��܂�
				}

				if (Spoon == 4 && lane[4] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[4][Monster] = 1;

					lane[4] = 1;
				}

				if (Spoon == 5 && lane[5] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[5][Monster] = 1;

					lane[5] = 1;
				}

				if (Spoon == 6 && lane[6] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[6][Monster] = 1;

					lane[6] = 1;
				}

				if (Spoon == 7 && lane[7] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //�����X�^�[�̎�ނ�I��

					Alive[7][Monster] = 1;

					lane[7] = 1;
				}

				timer = 0;
			}

			//���[��0[��K�@��]

			if (Alive[0][0] == 1)    //�΃X���C��
			{
				if (EnemyX[0] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_G[an], TRUE);        //�΃X���C���A�j���[�V����
					EnemyX[0] += 1;
				}

				else
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_A_L[an6], TRUE);

					Attack_Timer[0] += 1;

					if (Attack_Timer[0] == 100)
					{
						fortlife -= 1;
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						Attack_Timer[0] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[0] + 96 && leftarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[0][0] = 0;
					}
				}

				if (Alive[0][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;

				}
			}

			if (Alive[0][1] == 1) //�ԃX���C��
			{
				if (EnemyX[0] <= 335) //���̕ǂɂ��܂�
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_R[an], TRUE);//�ԃX���C���A�j���[�V����
					EnemyX[0] += 1;
				}

				else
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_R_A_L[an6], TRUE);

					Attack_Timer[0] += 1;

					if (Attack_Timer[0] == 100)
					{
						fortlife -= 1;
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						Attack_Timer[0] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[0] + 96 && leftarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[0][1] = 0;
					}
				}

				if (Alive[0][1] == 0)//�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;

				}
			}

			if (Alive[0][2] == 1) //�X���C��
			{
				if (EnemyX[0] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_B[an], TRUE);        //�X���C���A�j���[�V����
					EnemyX[0] += 1;
				}

				else
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_B_A_L[an6], TRUE);

					Attack_Timer[0] += 1;

					if (Attack_Timer[0] == 100)
					{
						fortlife -= 1;
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						Attack_Timer[0] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[0] + 96 && leftarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[0][2] = 0;
					}
				}

				if (Alive[0][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;
				}
			}

			//���[��1[��K�@�E]
			if (Alive[1][0] == 1)//�΃X���C���E
			{
				if (EnemyX[4] >= 1009) //���̕ǂɂ��܂�	
				{
					EnemyX[4] -= 1;
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_G[an], TRUE);       //�΃X���C���A�j���[�V����
				}

				else
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_A_R[an6], TRUE);

					Attack_Timer[1] += 1;

					if (Attack_Timer[1] == 100)
					{
						fortlife -= 1;
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						Attack_Timer[1] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[4] && rightarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[1][0] = 0;
					}
				}

				if (Alive[1][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			if (Alive[1][1] == 1)    //�ԃX���C���E
			{
				if (EnemyX[4] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_R[an], TRUE);       //�ԃX���C���A�j���[�V����
					EnemyX[4] -= 1;
				}

				else
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_A_R[an6], TRUE);

					Attack_Timer[1] += 1;

					if (Attack_Timer[1] == 100)
					{
						fortlife -= 1;
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						Attack_Timer[1] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[4] && rightarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[1][1] = 0;
					}
				}

				if (Alive[1][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			if (Alive[1][2] == 1)//�X���C���E
			{
				if (EnemyX[4] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_B[an], TRUE);//�X���C���A�j���[�V����
					EnemyX[4] -= 1;
				}

				else
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_B_A_R[an6], TRUE);

					Attack_Timer[1] += 1;

					if (Attack_Timer[1] == 100)
					{
						PlaySoundMem(SA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[1] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[4] && rightarrowY[i] == 816)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[1][2] = 0;
					}
				}

				if (Alive[1][2] == 0)//�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			//���[��2[��K�@��]

			if (Alive[2][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[1] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[1] - 96, EnemyY[1], Slime_Sky[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[1] += 2;
				}

				else
				{
					DrawGraph(EnemyX[1] - 96, EnemyY[1], Slime_Sky_A[an6], TRUE);//�X�J�C�X���C���A�j���[�V����

					Attack_Timer[2] += 1;

					if (Attack_Timer[2] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[2] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[1] + 96 && leftarrowY[i] == 624)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[2][0] = 0;
					}
				}

				if (Alive[2][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[2] = 0;
					EnemyX[1] = -192;
					lane[2] = 0;
				}
			}

			if (Alive[2][1] == 1)//�q�m�^�}
			{
				if (EnemyX[1] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[1], EnemyY[1], Fire[an], TRUE);//�q�m�^�}
					EnemyX[1] += 1;
				}

				else
				{
					fortlife -= 2;
					Alive[2][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[1] + 96 && leftarrowY[i] == 624)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[2][1] = 0;
					}
				}

				if (Alive[2][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{

					EnemyX[1] = -96;
					lane[2] = 0;
				}
			}

			if (Alive[2][2] == 1)//�h���S��
			{
				if (EnemyX[1] <= 335) //���̕ǂɂ��܂�	
				{
					if (Dragon_HP[0] == 1)
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon[an4], TRUE); //�h���S��
					}
					EnemyX[1] += 2;
				}

				else
				{
					if (Dragon_HP[0] == 1)
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_A_D[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_A[anD], TRUE);//�h���S��
					}

					Attack_Timer[2] += 1;

					if (Attack_Timer[2] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_LOOP, TRUE);
						fortlife -= 1;
						Attack_Timer[2] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[1] + 192 && leftarrowY[i] == 624)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[2][2] = 0;
					}
				}

				if (Alive[2][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[2] = 0;
					EnemyX[1] = -192;
					lane[2] = 0;
				}
			}

			//���[��3[��K�@�E]
			if (Alive[3][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[5] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[5], EnemyY[1], Slime_Sky_R[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[5] -= 2;
				}

				else
				{
					DrawGraph(EnemyX[5], EnemyY[1], Slime_Sky_A_R[an6], TRUE);//�X�J�C�X���C���A�j���[�V����

					Attack_Timer[3] += 1;

					if (Attack_Timer[3] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[3] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[5] && rightarrowY[i] == 624)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[3][0] = 0;
					}
				}

				if (Alive[3][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[3] = 0;
					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			if (Alive[3][1] == 1)//�q�m�^�}
			{
				if (EnemyX[5] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[5], EnemyY[1], Fire_R[an], TRUE);//�q�m�^�}�A�j���[�V����
					EnemyX[5] -= 1;
				}

				else
				{
					PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
					fortlife -= 2;
					Alive[3][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[5] && rightarrowY[i] == 624)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[3][1] = 0;
					}
				}

				if (Alive[3][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{

					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			if (Alive[3][2] == 1)//�h���S��
			{
				if (EnemyX[5] >= 1009) //���̕ǂɂ��܂�	
				{
					if (Dragon_HP[1] == 1)
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_R_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_R[an4], TRUE);//�h���S��
					}
					EnemyX[5] -= 2;
				}

				else
				{
					if (Dragon_HP[1] == 1)
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_A_D_R[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_A_R[anD], TRUE);//�h���S��
					}

					Attack_Timer[3] += 1;

					if (Attack_Timer[3] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_LOOP, TRUE);
						fortlife -= 1;
						Attack_Timer[3] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[5] && rightarrowY[i] == 624)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[3][2] = 0;
					}
				}

				if (Alive[3][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[3] = 0;
					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			//���[��4[�O�K�@��]
			if (Alive[4][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[2] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[2] - 96, EnemyY[2], Slime_Sky[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[2] += 2;
				}

				else
				{
					DrawGraph(EnemyX[2] - 96, EnemyY[2], Slime_Sky_A[an6], TRUE);//�X�J�C�X���C���A�j���[�V����

					Attack_Timer[4] += 1;

					if (Attack_Timer[4] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_LOOP, TRUE);
						fortlife -= 1;
						Attack_Timer[4] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[2] + 192 && leftarrowY[i] == 432)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[4][0] = 0;
					}
				}

				if (Alive[4][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[4] = 0;
					EnemyX[2] = -192;
					lane[4] = 0;
				}
			}

			if (Alive[4][1] == 1)    //�q�m�^�}
			{
				if (EnemyX[2] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[2], EnemyY[2], Fire[an], TRUE);//�q�m�^�}�A�j���[�V����
					EnemyX[2] += 1;
				}

				else
				{
					PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
					fortlife -= 2;
					Alive[4][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[2] + 96 && leftarrowY[i] == 432)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[4][1] = 0;
					}
				}

				if (Alive[4][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{

					EnemyX[2] = -96;
					lane[4] = 0;
				}
			}

			if (Alive[4][2] == 1)//�h���S��
			{
				if (EnemyX[2] <= 335)//���̕ǂɂ��܂�	
				{
					if (Dragon_HP[2] == 1)
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon[an4], TRUE);//�h���S��
					}

					EnemyX[2] += 2;
				}

				else
				{
					if (Dragon_HP[2] == 1)
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_A_D[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_A[anD], TRUE);//�h���S��
					}

					Attack_Timer[4] += 1;

					if (Attack_Timer[4] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[4] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[2] + 192 && leftarrowY[i] == 432)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[4][2] = 0;
					}
				}

				if (Alive[4][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[4] = 0;
					EnemyX[2] = -192;
					lane[4] = 0;
				}

			}

			//���[��5[�O�K�@�E]

			if (Alive[5][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[6] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[6], EnemyY[2], Slime_Sky_R[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[6] -= 2;
				}

				else
				{
					DrawGraph(EnemyX[6], EnemyY[2], Slime_Sky_A_R[an6], TRUE);

					Attack_Timer[5] += 1;

					if (Attack_Timer[5] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[5] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[6] && rightarrowY[i] == 432)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[5][0] = 0;
					}
				}

				if (Alive[5][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[5] = 0;
					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			if (Alive[5][1] == 1)//�q�m�^�}
			{
				if (EnemyX[6] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[6], EnemyY[2], Fire_R[an], TRUE);//�q�m�^�}�A�j���[�V����
					EnemyX[6] -= 1;
				}

				else
				{
					PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
					fortlife -= 2;
					Alive[5][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[6] && rightarrowY[i] == 432)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[5][1] = 0;
					}
				}

				if (Alive[5][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{

					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			if (Alive[5][2] == 1)    //�h���S��-----------------------------------------------
			{
				if (EnemyX[6] >= 1009) //���̕ǂɂ��܂�	
				{
					if (Dragon_HP[3] == 1)
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_R_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_R[an4], TRUE);//�h���S��
					}

					EnemyX[6] -= 2;
				}

				else
				{
					if (Dragon_HP[3] == 1)
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_A_D_R[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_A_R[anD], TRUE);//�h���S��
					}

					Attack_Timer[5] += 1;

					if (Attack_Timer[5] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[5] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[6] && rightarrowY[i] == 432)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[5][2] = 0;
					}
				}

				if (Alive[5][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[5] = 0;
					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			//���[��6[�l�K�@��]

			if (Alive[6][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[3] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[3] - 96, EnemyY[3], Slime_Sky[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[3] += 2;
				}

				else
				{
					DrawGraph(EnemyX[3] - 96, EnemyY[3], Slime_Sky_A[an6], TRUE);

					Attack_Timer[6] += 1;

					if (Attack_Timer[6] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[6] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[3] + 96 && leftarrowY[i] == 240)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[6][0] = 0;
					}
				}

				if (Alive[6][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[6] = 0;
					EnemyX[3] = -192;
					lane[6] = 0;
				}
			}

			if (Alive[6][1] == 1)//�q�m�^�}
			{
				if (EnemyX[3] <= 335) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[3], EnemyY[3], Fire[an], TRUE);//�q�m�^�}�A�j���[�V����
					EnemyX[3] += 1;
				}

				else
				{
					PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
					fortlife -= 2;
					Alive[6][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[3] + 96 && leftarrowY[i] == 240)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[6][1] = 0;
					}
				}

				if (Alive[6][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{

					EnemyX[3] = -96;
					lane[6] = 0;
				}
			}

			if (Alive[6][2] == 1)//�h���S��
			{
				if (EnemyX[3] <= 335)//���̕ǂɂ��܂�	
				{
					if (Dragon_HP[4] == 1)
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon[an4], TRUE);//�h���S��
					}

					EnemyX[3] += 2;
				}

				else
				{
					if (Dragon_HP[4] == 1)
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_A_D[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_A[anD], TRUE);//�h���S��
					}

					Attack_Timer[6] += 1;

					if (Attack_Timer[6] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[6] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (leftarrowX[i] <= EnemyX[3] + 288 && leftarrowY[i] == 240)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[6][2] = 0;
					}
				}

				if (Alive[6][2] == 0)//�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[6] = 0;
					EnemyX[3] = -96;
					lane[6] = 0;
				}
			}

			//���[��7[�l�K�@�E]

			if (Alive[7][0] == 1)//�X�J�C�X���C��
			{
				if (EnemyX[7] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[7], EnemyY[3], Slime_Sky_R[an4], TRUE);//�X�J�C�X���C���A�j���[�V����
					EnemyX[7] -= 2;
				}

				else
				{
					DrawGraph(EnemyX[7], EnemyY[3], Slime_Sky_A_R[an6], TRUE);

					Attack_Timer[7] += 1;

					if (Attack_Timer[7] == 100)
					{
						PlaySoundMem(SkyA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[7] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[7] && rightarrowY[i] == 240)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[7][0] = 0;
					}
				}

				if (Alive[7][0] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[7] = 0;
					EnemyX[7] = 1440;
					lane[7] = 0;
				}
			}

			if (Alive[7][1] == 1)    //�q�m�^�}-----------------------------------------------
			{
				if (EnemyX[7] >= 1009) //���̕ǂɂ��܂�	
				{
					DrawGraph(EnemyX[7], EnemyY[3], Fire_R[an], TRUE);//�q�m�^�}�A�j���[�V����
					EnemyX[7] -= 1;
				}

				else
				{
					PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
					fortlife -= 2;
					Alive[7][1] = 0;
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[7] && rightarrowY[i] == 240)
					{
						PlaySoundMem(explo, DX_PLAYTYPE_BACK, TRUE);
						Alive[7][1] = 0;
					}
				}

				if (Alive[7][1] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					EnemyX[7] = 1440;
					lane[7] = 0;
				}
			}

			if (Alive[7][2] == 1)    //�h���S��-----------------------------------------------
			{
				if (EnemyX[7] >= 1008) //���̕ǂɂ��܂�	
				{
					if (Dragon_HP[5] == 1)
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_R_D[an4], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_R[an4], TRUE);//�h���S��
					}

					EnemyX[7] -= 2;
				}

				else
				{
					if (Dragon_HP[5] == 1)
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_A_D_R[anD], TRUE);//�h���S���_���[�W
					}

					else
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_A_R[anD], TRUE);//�h���S��
					}

					Attack_Timer[7] += 1;

					if (Attack_Timer[7] == 100)
					{
						PlaySoundMem(DA, DX_PLAYTYPE_BACK, TRUE);
						fortlife -= 1;
						Attack_Timer[7] = 0;
					}
				}

				for (int i = 0; i < 5; ++i)
				{
					if (rightarrowX[i] + 60 >= EnemyX[7] && rightarrowY[i] == 240)
					{
						PlaySoundMem(hitSE, DX_PLAYTYPE_BACK, TRUE);
						Alive[7][2] = 0;
					}
				}

				if (Alive[7][2] == 0) //�����X�^�[�����񂾂�ϐ�����ʊO�ɖ߂�
				{
					score += 200;
					Attack_Timer[7] = 0;
					EnemyX[7] = 1440;
					lane[7] = 0;
				}
			}

			DrawFormatString(5, 100, GetColor(255, 0, 0), "SCORE=%d", score);

			if (fortlife <= 0)
			{
				StopSoundMem(maintheme);
				StopSoundMem(SA);
				StopSoundMem(SkyA);
				StopSoundMem(DA);
				StopSoundMem(explo);
				StopSoundMem(hitSE);
				StopSoundMem(shotSE);

				if (CheckSoundMem(gameoverBGM) == 0)
				{
					PlaySoundMem(gameoverBGM, DX_PLAYTYPE_BACK);
				}

				DrawGraph(0, 0, GameOver, TRUE);
				DrawGraph(lifeX, lifeY, life[0], TRUE);

				if (CheckHitKey(KEY_INPUT_T) == 1 && oldkeys[KEY_INPUT_T] == 0) //�^�C�g���ɖ߂�
				{
					scene = 0;
				}
			}

			break;

		case 2:

			if (CheckSoundMem(opening) == 0)
			{
				PlaySoundMem(opening, DX_PLAYTYPE_BACK);
			}

			if (keys[KEY_INPUT_TAB] == 1)
			{
				scene = 0;
			}

			DrawGraph(0, 0, backsky, TRUE);
			DrawGraph(scrollX, 0, cloud, TRUE);
			DrawGraph(scrollX - WIN_WIDTH * 2, 0, cloud, TRUE);
			DrawGraph(0, 0, Howto, TRUE);

			scrollX = scrollX + 1;

			if (scrollX >= WIN_WIDTH * 2)
			{
				scrollX = 0;
			}

			break;

		}

		//DrawFormatString(5, 820, GetColor(255, 0, 0), "�V�[��= %d", scene);


		//---------  �����܂łɃv���O�������L�q  ---------//
		ScreenFlip();//�i�_�u���o�b�t�@�j����
		// 20�~���b�ҋ@�i�^��60FPS�j
		WaitTimer(20);
		// Windows �V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}
		// �d�r�b�L�[�������ꂽ�烋�[�v���甲����
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}

	//Dx���C�u�����I������
	DxLib_End();

	return 0;
}