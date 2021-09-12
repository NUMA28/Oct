#include "DxLib.h"
#include <time.h>
#include <math.h>

const char TITLE[] = "Oct";

const int WIN_WIDTH = 1440; //ウィンドウ横幅
const int WIN_HEIGHT = 960;//ウィンドウ縦幅


int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(TRUE);						//ウィンドウモードに設定
	//ウィンドウサイズを手動では変更できず、かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);
	SetMainWindowText(TITLE);					// タイトルを変更
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);	//画面サイズの最大サイズ、カラービット数を設定（モニターの解像度に合わせる）
	SetWindowSizeExtendRate(1.0);				//画面サイズを設定（解像度との比率で設定）
	SetBackgroundColor(0, 0, 0);		// 画面の背景色を設定する

	//Dxライブラリの初期化
	if (DxLib_Init() == -1) { return -1; }

	//（ダブルバッファ）描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	//画像などのリソースデータの変数宣言と読み込み
	int playerX = 672; //プレイヤー初期位置X座標
	int playerY = 576; //プレイヤー初期位置Y座標

	int right = LoadGraph("Image\\Yumihei teisi migimuki.png");
	int left = LoadGraph("Yumihei teisi hidarimuki.png");

	int rightstanding[5];//プレイヤー右向き立ちモーション絵
	LoadDivGraph("Image\\Yumihei migi standing.png", 4, 4, 1, 72, 96, rightstanding);

	int leftstanding[5];//プレイヤー左向き立ちモーション絵
	LoadDivGraph("Image\\Yumihei hidari standing.png", 4, 4, 1, 72, 96, leftstanding);

	int rightmoving[5]; //プレイヤー右向き歩きモーション絵
	LoadDivGraph("Image\\Yumihei migi moving.png", 4, 4, 1, 72, 96, rightmoving);

	int leftmoving[5]; //プレイヤー右向き歩きモーション絵
	LoadDivGraph("Image\\Yumihei hidari moving.png", 4, 4, 1, 72, 96, leftmoving);

	int hasigo1 = LoadGraph("Image\\Yumihei hasigo 1.png");

	int hasigo2 = LoadGraph("Image\\Yumihei hasigo 2.png");

	//プレイヤーの状態変数
	int player = 0;

	//弓補充ポイントの状態変数
	int arrowbox1 = 0;
	int arrowbox2 = 0;

	//所持弓矢変数
	int getarrow = 5;

	//////////////弓矢変数//////////////////
	//////右向き//////
	int rightarrowX[5];//X座標宣言
	for (int i = 0; i < 5; i++)
	{
		rightarrowX[i] = 500;
	}

	int rightarrowY[5];//Y座標宣言
	for (int i = 0; i < 5; i++)
	{
		rightarrowY[i] = -100;
	}

	int rightarrowflag[5];//フラグ宣言
	for (int i = 0; i < 5; i++)
	{
		rightarrowflag[i] = 0;//初期化
	}

	///////左向き////////
	int leftarrowX[5];//X座標宣言
	for (int i = 0; i < 5; i++)
	{
		leftarrowX[i] = 500;
	}

	int leftarrowY[5];//Y座標宣言
	for (int i = 0; i < 5; i++)
	{
		leftarrowY[i] = -100;
	}

	int leftarrowflag[5];//フラグ宣言
	for (int i = 0; i < 5; i++)
	{
		leftarrowflag[i] = 0;//初期化
	}

	int arrowspeed = 7;

	int rightarrow = LoadGraph("Image\\Yumiya1.png");
	int leftarrow = LoadGraph("Image\\Yumiya2.png");

	//砦体力
	int fortlife = 10;

	//シーン
	int scene = 0;

	//アニメーションタイマー
	int playertime = 0;
	int arrowbox1time = 0;
	int arrowbox2time = 0;
	int scrollX = 0;

	//射撃クールタイム
	int rightshotBflag = 0;
	int leftshotBflag = 0;

	int block[14];
	LoadDivGraph("Image\\block.png", 14, 14, 1, 96, 96, block);
	//足場等の画像を96*96の5枚に分割して読み込み

	int life[11];
	LoadDivGraph("Image\\Fort life.png", 11, 3, 4, 348, 42, life);
	int lifeX = 546;
	int lifeY = 45;

	int stage = LoadGraph("Image\\Stage.png");//背景
	int cloud = LoadGraph("Image\\Cloud.png");
	int backsky = LoadGraph("Image\\back sky.png");

	int HUDarrow[6];
	LoadDivGraph("Image\\HUD arrow.png", 6, 6, 1, 180, 84, HUDarrow);

	int title = LoadGraph("Image\\Title.png");

	int Howto = LoadGraph("Image\\HowTo.png");

	/////////////サウンドファイル//////////////////
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

	//ゲームループで使う変数の宣言
	char keys[256] = { 0 }; //最新のキーボード情報用
	char oldkeys[256] = { 0 };//1ループ（フレーム）前のキーボード情報

	///////マップチップ///////
	int mapdata[11][16] =
	{
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},
		{13,13,13,13, 13,0,0,0,1,0,13, 13,13,13,13},
		{13,13,13,13, 13,4,4,4,2,3,13, 13,13,13,13},// 0：足場
		{13,13,13,13, 13,0,1,0,0,0,13, 13,13,13,13},// 1：足場＆梯子
		{13,13,13,13, 13,4,2,4,4,4,13, 13,13,13,13},// 2：梯子
		{13,13,13,13, 13,0,0,0,1,0,13, 13,13,13,13},// 3：弓矢の籠
		{13,13,13,13, 13,3,4,4,2,4,13, 13,13,13,13},// 4：空白
		{13,13,13,13, 13,4,4,4,4,4,13, 13,13,13,13},// 5：城壁
	};

	int GameOver = LoadGraph("Image\\Game Over.png");//ゲームオーバー画面

	//----------------------------敵の変数-------------------------------------------
	int Spoon = 0; //どのレーンでスポーンするか決める変数
	int Monster = 0;

	int timer = 0;
	int EnemyTimer = 0;
	int Attack_Timer[8] = { 0,0,0,0,0,0,0,0 };

	int Dragon_HP[6]{ 0,0,0,0,0,0 };//ドラゴンのHP

	int Alive[8][3] = {                 //エネミーの生存フラグ
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
		{0,0,0},
	};
	//----------------左--------------右------------------------------
	int EnemyX[8] = { -192,-192,-192,-192,1440,1440,1440,1440 };       //エネミーの座票

	//-------------1段-2段-3段-4段---------------------
	int EnemyY[4] = { 768,576,384,192 };

	int an = 0;//----------------------------------------アニメーション用変数--------------------------------------------------------------------------------------
	int an4 = 0;
	int an6 = 0;
	int an10 = 0;
	int anD = 0;

	int lane[8] = { 0,0,0,0,0,0,0,0 }; //------------レーンにモンスターが出ているかどうか

	int score = 0;

	//----------------------------------------------------モンスターの画像読み込み----------------------------------------------------------------------------------------------------

	int Slime_L_G[2];
	LoadDivGraph("Image\\Slime1.png", 2, 2, 1, 96, 96, Slime_L_G);//スライム緑「左」

	int Slime_A_L[6];
	LoadDivGraph("Image\\Slime_A.png", 6, 6, 1, 96, 96, Slime_A_L);//スライム緑「左」攻撃

	int Slime_L_R[2];
	LoadDivGraph("Image\\Slime_Red1.png", 2, 2, 1, 96, 96, Slime_L_R);//スライム赤「左」

	int Slime_R_A_L[6];
	LoadDivGraph("Image\\Slime_Red_A.png", 6, 6, 1, 96, 96, Slime_R_A_L);//スライム赤「左」攻撃

	int Slime_L_B[2];
	LoadDivGraph("Image\\Slime_Blue1.png", 2, 2, 1, 96, 96, Slime_L_B);//スライム青「左」

	int Slime_B_A_L[6];
	LoadDivGraph("Image\\Slime_Blue_A.png", 6, 6, 1, 96, 96, Slime_B_A_L);//スライム青「左」攻撃

	//---------------------------「右」--------------------------------------------------------------------------------------------------------------------------

	int Slime_R_G[2];
	LoadDivGraph("Image\\Slime_R.png", 2, 2, 1, 96, 96, Slime_R_G);  //スライム緑「右」

	int Slime_A_R[6];
	LoadDivGraph("Image\\Slime_A_R.png", 6, 6, 1, 96, 96, Slime_A_R);  //スライム緑「左」攻撃

	int Slime_R_R[2];
	LoadDivGraph("Image\\Slime_Red_R.png", 2, 2, 1, 96, 96, Slime_R_R);  //スライム赤「右」

	int Slime_R_A_R[6];
	LoadDivGraph("Image\\Slime_Red_A_R.png", 6, 6, 1, 96, 96, Slime_R_A_R);  //スライム赤「左」攻撃

	int Slime_R_B[2];
	LoadDivGraph("Image\\Slime_Blue_R.png", 2, 2, 1, 96, 96, Slime_R_B);  //スライム青「右」

	int Slime_B_A_R[6];
	LoadDivGraph("Image\\Slime_Blue_A_R.png", 6, 6, 1, 96, 96, Slime_B_A_R);  //スライム青「左」攻撃

		//---------------------------「空中」--------------------------------------------------------------------------------------------------------------------------

	int Slime_Sky[4];
	LoadDivGraph("Image\\Slime_Sky1.png", 4, 4, 1, 192, 96, Slime_Sky);  //スカイスライム

	int Slime_Sky_R[4];
	LoadDivGraph("Image\\Slime_Sky_R.png", 4, 4, 1, 192, 96, Slime_Sky_R);  //スカイスライム右

	int Slime_Sky_A[6];
	LoadDivGraph("Image\\Slime_Sky_A.png", 6, 6, 1, 192, 96, Slime_Sky_A);  //スカイスライム左攻撃

	int Slime_Sky_A_R[6];
	LoadDivGraph("Image\\Slime_Sky_A_R.png", 6, 6, 1, 192, 96, Slime_Sky_A_R);  //スカイスライム右攻撃

	int Fire[2];
	LoadDivGraph("Image\\Fire.png", 2, 2, 1, 96, 96, Fire);  //ヒノタマ

	int Fire_R[2];
	LoadDivGraph("Image\\Fire_R.png", 2, 2, 1, 96, 96, Fire_R);  //ヒノタマ右

	int exp[10];
	LoadDivGraph("Image\\explosion.png", 10, 10, 1, 72, 72, exp);  //爆発

	int Dragon[4];
	LoadDivGraph("Image\\Dragon1.png", 4, 4, 1, 288, 96, Dragon);  //ドラゴン

	int Dragon_A[6];
	LoadDivGraph("Image\\Dragon_A1.png", 6, 6, 1, 288, 96, Dragon_A);  //ドラゴン攻撃

	int Dragon_D[4];
	LoadDivGraph("Image\\Dragon_D1.png", 4, 4, 1, 288, 96, Dragon_D);  //ドラゴン ダメージ有り

	int Dragon_A_D[6];
	LoadDivGraph("Image\\Dragon_A_D1.png", 6, 6, 1, 288, 96, Dragon_A_D);  //ドラゴン ダメージ有り 攻撃

	int Dragon_R[4];
	LoadDivGraph("Image\\Dragon_R.png", 4, 4, 1, 288, 96, Dragon_R);  //ドラゴン 右

	int Dragon_A_R[6];
	LoadDivGraph("Image\\Dragon_A_R.png", 6, 6, 1, 288, 96, Dragon_A_R);  //ドラゴン 右　攻撃

	int Dragon_R_D[4];
	LoadDivGraph("Image\\Dragon_D_R.png", 4, 4, 1, 288, 96, Dragon_R_D);  //ドラゴン 右 ダメージ有り

	int Dragon_A_D_R[6];
	LoadDivGraph("Image\\Dragon_A_D_R.png", 6, 6, 1, 288, 96, Dragon_A_D_R);  //ドラゴン 右　攻撃

	int Flag1 = 1;
	int Flag2 = 1;
	int Flag3 = 1;

	// ゲームループ
	while (1)
	{
		//最新のキーボード情報だったものは１フレーム前のキーボード情報として保存

		//最新のキーボード情報を取得
		GetHitKeyStateAll(keys);

		//画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		//更新処理
		switch (scene)
		{
		case 0://タイトル画面（ストーリームービー）

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

				//初期値設定
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

		case 1: //ゲームプレイ

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

			//描画処理
			for (int x = 0; x < 15; x++)
			{
				for (int y = 0; y < 10; y++)
				{
					DrawGraph((x * 96), (y * 96), block[mapdata[y][x]], TRUE);
				}
			}

			////////////プレイヤー移動///////////

			////////////////上移動////////////////////
			if ((keys[KEY_INPUT_UP] == 1) &&
				(mapdata[(playerY - 6) / 96][playerX / 96] >= 1) &&
				(mapdata[(playerY - 6) / 96][(playerX + 72) / 96] >= 1))//上移動
			{
				//プレイヤーが4階の床で止まる処理
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

			////////////////下移動////////////////////
			if (keys[KEY_INPUT_DOWN] == 1)//下が押されたとき
			{
				//梯子がプレイヤーの下にあった場合のみ作動（床があった場合下がらない処理）
				if ((mapdata[(playerY + 96) / 96][playerX / 96] >= 1) &&
					(mapdata[(playerY + 96) / 96][(playerX + 72) / 96] >= 1))
				{
					//プレイヤーが地面で止まる処理
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


			////////////////右移動////////////////////
			if (keys[KEY_INPUT_RIGHT] == 1)
			{
				//梯子から横移動できないようにする処理
				if ((mapdata[playerY / 96][(playerX + 72) / 96] >= 2) &&
					(mapdata[(playerY + 90) / 96][(playerX + 72) / 96] >= 2))
				{
					//右側の城壁にぶつかる処理
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

			if (keys[KEY_INPUT_LEFT] == 1)//左移動
			{
				//梯子から横移動できないようにする処理
				if ((mapdata[playerY / 96][playerX / 96] >= 2) &&
					(mapdata[(playerY + 90) / 96][playerX / 96] >= 2))
				{
					//左側の城壁にぶつかる処理
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

			////////////右向き弾発射//////////////
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

			////////////左向き弾発射//////////////
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

					if (leftarrowX[i] < -60)//画面外に行った時の処理
					{
						leftarrowX[i] = 500;
						leftarrowflag[i] = 0;
					}
				}
			}

			///////プレイヤーの状態遷移による描画////////
			if ((player == 0) && (keys[KEY_INPUT_RIGHT] == 0))//右向き静止時
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

			if ((player == 1) && (keys[KEY_INPUT_LEFT] == 0))//左向き静止時
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

			if ((player == 3) && (keys[KEY_INPUT_UP] == 0))//上入力時
			{
				DrawGraph(playerX, playerY, hasigo1, TRUE);
			}

			if ((player == 4) && (keys[KEY_INPUT_DOWN] == 0))//下入力時
			{
				DrawGraph(playerX, playerY, hasigo2, TRUE);
			}


			if (arrowbox1 == 1)//座標[4][9]
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

			if (arrowbox2 == 1)//座標[4][9]
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

			//////////背景/////////
			DrawGraph(0, 0, stage, TRUE);//背景画像表示

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

			///////弓矢/////////
			for (int i = 0; i < 5; i++)
			{
				if (rightarrowflag[i] == 1)
				{
					DrawGraph(rightarrowX[i], rightarrowY[i], rightarrow, TRUE);//表示
				}

				if (leftarrowflag[i] == 1)
				{
					DrawGraph(leftarrowX[i], leftarrowY[i], leftarrow, TRUE);//表示
				}
			}

			//---------------------敵スポーンの処理-----------------------------------------------------------------------------------------------------------------------------------------

			timer += 1;

			EnemyTimer += 1; //--------------------------------エネミーのアニメーション用タイマー----------------------------------------------------------
			if (EnemyTimer == 40)EnemyTimer = 0;

			if (EnemyTimer % 8 == 0)	//----------------------エネミーのアニメーション用変数----------------------------------------------------------------
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

			if (timer == 100) //---------------------------タイマーが溜まるとモンスターがスポーンする--------------------------------------------------------------
			{
				srand((unsigned)time(NULL));
				Spoon = rand() % 8;	//-------------------------どのレーンでスポーンするか決める-------------------------------------------------------------


				if (Spoon == 0 && lane[0] == 0)//------------------------左下でスポーンする------------------------------------------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //------------------------モンスターの種類を選ぶ----------------------------------------------------

					Alive[0][Monster] = 1;

					lane[0] = 1;//-------------このレーンにはモンスターがスポーンしています
				}

				if (Spoon == 1 && lane[1] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[1][Monster] = 1;

					lane[1] = 1;//-------------このレーンにはモンスターがスポーンしています
				}

				if (Spoon == 2 && lane[2] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[2][Monster] = 1;

					lane[2] = 1;//-------------このレーンにはモンスターがスポーンしています
				}

				if (Spoon == 3 && lane[3] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[3][Monster] = 1;

					lane[3] = 1;//-------------このレーンにはモンスターがスポーンしています
				}

				if (Spoon == 4 && lane[4] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[4][Monster] = 1;

					lane[4] = 1;
				}

				if (Spoon == 5 && lane[5] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[5][Monster] = 1;

					lane[5] = 1;
				}

				if (Spoon == 6 && lane[6] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[6][Monster] = 1;

					lane[6] = 1;
				}

				if (Spoon == 7 && lane[7] == 0)//---------------------------------------------------------
				{
					srand((unsigned)time(NULL));
					Monster = rand() % 3;   //モンスターの種類を選ぶ

					Alive[7][Monster] = 1;

					lane[7] = 1;
				}

				timer = 0;
			}

			//レーン0[一階　左]

			if (Alive[0][0] == 1)    //緑スライム
			{
				if (EnemyX[0] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_G[an], TRUE);        //緑スライムアニメーション
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

				if (Alive[0][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;

				}
			}

			if (Alive[0][1] == 1) //赤スライム
			{
				if (EnemyX[0] <= 335) //塔の壁につくまで
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_R[an], TRUE);//赤スライムアニメーション
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

				if (Alive[0][1] == 0)//モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;

				}
			}

			if (Alive[0][2] == 1) //青スライム
			{
				if (EnemyX[0] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[0], EnemyY[0], Slime_L_B[an], TRUE);        //青スライムアニメーション
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

				if (Alive[0][2] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[0] = 0;
					EnemyX[0] = -96;
					lane[0] = 0;
				}
			}

			//レーン1[一階　右]
			if (Alive[1][0] == 1)//緑スライム右
			{
				if (EnemyX[4] >= 1009) //塔の壁につくまで	
				{
					EnemyX[4] -= 1;
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_G[an], TRUE);       //緑スライムアニメーション
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

				if (Alive[1][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			if (Alive[1][1] == 1)    //赤スライム右
			{
				if (EnemyX[4] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_R[an], TRUE);       //赤スライムアニメーション
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

				if (Alive[1][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			if (Alive[1][2] == 1)//青スライム右
			{
				if (EnemyX[4] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[4], EnemyY[0], Slime_R_B[an], TRUE);//青スライムアニメーション
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

				if (Alive[1][2] == 0)//モンスターが死んだら変数を画面外に戻す
				{
					score += 100;
					Attack_Timer[1] = 0;
					EnemyX[4] = 1440;
					lane[1] = 0;
				}
			}

			//レーン2[二階　左]

			if (Alive[2][0] == 1)//スカイスライム
			{
				if (EnemyX[1] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[1] - 96, EnemyY[1], Slime_Sky[an4], TRUE);//スカイスライムアニメーション
					EnemyX[1] += 2;
				}

				else
				{
					DrawGraph(EnemyX[1] - 96, EnemyY[1], Slime_Sky_A[an6], TRUE);//スカイスライムアニメーション

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

				if (Alive[2][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[2] = 0;
					EnemyX[1] = -192;
					lane[2] = 0;
				}
			}

			if (Alive[2][1] == 1)//ヒノタマ
			{
				if (EnemyX[1] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[1], EnemyY[1], Fire[an], TRUE);//ヒノタマ
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

				if (Alive[2][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{

					EnemyX[1] = -96;
					lane[2] = 0;
				}
			}

			if (Alive[2][2] == 1)//ドラゴン
			{
				if (EnemyX[1] <= 335) //塔の壁につくまで	
				{
					if (Dragon_HP[0] == 1)
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon[an4], TRUE); //ドラゴン
					}
					EnemyX[1] += 2;
				}

				else
				{
					if (Dragon_HP[0] == 1)
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_A_D[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[1] - 192, EnemyY[1], Dragon_A[anD], TRUE);//ドラゴン
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

				if (Alive[2][2] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[2] = 0;
					EnemyX[1] = -192;
					lane[2] = 0;
				}
			}

			//レーン3[二階　右]
			if (Alive[3][0] == 1)//スカイスライム
			{
				if (EnemyX[5] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[5], EnemyY[1], Slime_Sky_R[an4], TRUE);//スカイスライムアニメーション
					EnemyX[5] -= 2;
				}

				else
				{
					DrawGraph(EnemyX[5], EnemyY[1], Slime_Sky_A_R[an6], TRUE);//スカイスライムアニメーション

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

				if (Alive[3][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[3] = 0;
					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			if (Alive[3][1] == 1)//ヒノタマ
			{
				if (EnemyX[5] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[5], EnemyY[1], Fire_R[an], TRUE);//ヒノタマアニメーション
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

				if (Alive[3][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{

					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			if (Alive[3][2] == 1)//ドラゴン
			{
				if (EnemyX[5] >= 1009) //塔の壁につくまで	
				{
					if (Dragon_HP[1] == 1)
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_R_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_R[an4], TRUE);//ドラゴン
					}
					EnemyX[5] -= 2;
				}

				else
				{
					if (Dragon_HP[1] == 1)
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_A_D_R[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[5], EnemyY[1], Dragon_A_R[anD], TRUE);//ドラゴン
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

				if (Alive[3][2] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[3] = 0;
					EnemyX[5] = 1440;
					lane[3] = 0;
				}
			}

			//レーン4[三階　左]
			if (Alive[4][0] == 1)//スカイスライム
			{
				if (EnemyX[2] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[2] - 96, EnemyY[2], Slime_Sky[an4], TRUE);//スカイスライムアニメーション
					EnemyX[2] += 2;
				}

				else
				{
					DrawGraph(EnemyX[2] - 96, EnemyY[2], Slime_Sky_A[an6], TRUE);//スカイスライムアニメーション

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

				if (Alive[4][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[4] = 0;
					EnemyX[2] = -192;
					lane[4] = 0;
				}
			}

			if (Alive[4][1] == 1)    //ヒノタマ
			{
				if (EnemyX[2] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[2], EnemyY[2], Fire[an], TRUE);//ヒノタマアニメーション
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

				if (Alive[4][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{

					EnemyX[2] = -96;
					lane[4] = 0;
				}
			}

			if (Alive[4][2] == 1)//ドラゴン
			{
				if (EnemyX[2] <= 335)//塔の壁につくまで	
				{
					if (Dragon_HP[2] == 1)
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon[an4], TRUE);//ドラゴン
					}

					EnemyX[2] += 2;
				}

				else
				{
					if (Dragon_HP[2] == 1)
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_A_D[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[2] - 192, EnemyY[2], Dragon_A[anD], TRUE);//ドラゴン
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

				if (Alive[4][2] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[4] = 0;
					EnemyX[2] = -192;
					lane[4] = 0;
				}

			}

			//レーン5[三階　右]

			if (Alive[5][0] == 1)//スカイスライム
			{
				if (EnemyX[6] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[6], EnemyY[2], Slime_Sky_R[an4], TRUE);//スカイスライムアニメーション
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

				if (Alive[5][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[5] = 0;
					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			if (Alive[5][1] == 1)//ヒノタマ
			{
				if (EnemyX[6] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[6], EnemyY[2], Fire_R[an], TRUE);//ヒノタマアニメーション
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

				if (Alive[5][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{

					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			if (Alive[5][2] == 1)    //ドラゴン-----------------------------------------------
			{
				if (EnemyX[6] >= 1009) //塔の壁につくまで	
				{
					if (Dragon_HP[3] == 1)
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_R_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_R[an4], TRUE);//ドラゴン
					}

					EnemyX[6] -= 2;
				}

				else
				{
					if (Dragon_HP[3] == 1)
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_A_D_R[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[6], EnemyY[2], Dragon_A_R[anD], TRUE);//ドラゴン
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

				if (Alive[5][2] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[5] = 0;
					EnemyX[6] = 1440;
					lane[5] = 0;
				}
			}

			//レーン6[四階　左]

			if (Alive[6][0] == 1)//スカイスライム
			{
				if (EnemyX[3] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[3] - 96, EnemyY[3], Slime_Sky[an4], TRUE);//スカイスライムアニメーション
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

				if (Alive[6][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[6] = 0;
					EnemyX[3] = -192;
					lane[6] = 0;
				}
			}

			if (Alive[6][1] == 1)//ヒノタマ
			{
				if (EnemyX[3] <= 335) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[3], EnemyY[3], Fire[an], TRUE);//ヒノタマアニメーション
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

				if (Alive[6][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{

					EnemyX[3] = -96;
					lane[6] = 0;
				}
			}

			if (Alive[6][2] == 1)//ドラゴン
			{
				if (EnemyX[3] <= 335)//塔の壁につくまで	
				{
					if (Dragon_HP[4] == 1)
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon[an4], TRUE);//ドラゴン
					}

					EnemyX[3] += 2;
				}

				else
				{
					if (Dragon_HP[4] == 1)
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_A_D[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[3] - 192, EnemyY[3], Dragon_A[anD], TRUE);//ドラゴン
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

				if (Alive[6][2] == 0)//モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[6] = 0;
					EnemyX[3] = -96;
					lane[6] = 0;
				}
			}

			//レーン7[四階　右]

			if (Alive[7][0] == 1)//スカイスライム
			{
				if (EnemyX[7] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[7], EnemyY[3], Slime_Sky_R[an4], TRUE);//スカイスライムアニメーション
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

				if (Alive[7][0] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					score += 200;
					Attack_Timer[7] = 0;
					EnemyX[7] = 1440;
					lane[7] = 0;
				}
			}

			if (Alive[7][1] == 1)    //ヒノタマ-----------------------------------------------
			{
				if (EnemyX[7] >= 1009) //塔の壁につくまで	
				{
					DrawGraph(EnemyX[7], EnemyY[3], Fire_R[an], TRUE);//ヒノタマアニメーション
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

				if (Alive[7][1] == 0) //モンスターが死んだら変数を画面外に戻す
				{
					EnemyX[7] = 1440;
					lane[7] = 0;
				}
			}

			if (Alive[7][2] == 1)    //ドラゴン-----------------------------------------------
			{
				if (EnemyX[7] >= 1008) //塔の壁につくまで	
				{
					if (Dragon_HP[5] == 1)
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_R_D[an4], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_R[an4], TRUE);//ドラゴン
					}

					EnemyX[7] -= 2;
				}

				else
				{
					if (Dragon_HP[5] == 1)
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_A_D_R[anD], TRUE);//ドラゴンダメージ
					}

					else
					{
						DrawGraph(EnemyX[7], EnemyY[3], Dragon_A_R[anD], TRUE);//ドラゴン
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

				if (Alive[7][2] == 0) //モンスターが死んだら変数を画面外に戻す
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

				if (CheckHitKey(KEY_INPUT_T) == 1 && oldkeys[KEY_INPUT_T] == 0) //タイトルに戻る
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

		//DrawFormatString(5, 820, GetColor(255, 0, 0), "シーン= %d", scene);


		//---------  ここまでにプログラムを記述  ---------//
		ScreenFlip();//（ダブルバッファ）裏面
		// 20ミリ秒待機（疑似60FPS）
		WaitTimer(20);
		// Windows システムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}
		// ＥＳＣキーが押されたらループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
	}

	//Dxライブラリ終了処理
	DxLib_End();

	return 0;
}