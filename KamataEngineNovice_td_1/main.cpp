#include <Novice.h>

#include <Vector2.h>

#include <stdlib.h>
#include <time.h>

const char kWindowTitle[] = "LC1C_20_ヒガ_ワタル_タイトル";


typedef struct EdgePosition //矩形の当たり判定用変数
{
	float right;
	float left;
	float up;
	float down;
}EdgePosition;

typedef struct Player//プレイヤーの変数
{
	Vector2 worldPosition;
	float newPosY;
	Vector2 velocity;
	Vector2 accelecion;
	float radius;
	EdgePosition edgePosition;
}Player;

typedef struct Trap//トラップの変数
{
	Vector2 position;
	float newPosY;
	float radius;
	EdgePosition edgePosition;
	int trapNum;
}Trap;

typedef struct Goal//ゴールの変数
{
	Vector2 position;
	float newPosY;
	float radius;
}Goal;

typedef struct Item//アイテムの変数
{
	Vector2 position;
	float newPosY;
	float radius;
	EdgePosition edgePosition;
}Item;

typedef struct BackPage
{
	Vector2 pos1;//一個減らした
}BackPage;

typedef struct DogSpeed
{
	float normal;
	float slow;
	float fast;
}DogSpeed;

typedef struct JumpPower
{
	float low;
	float high;
}JumpPower;

const int world = 500;

const int worldEnd = 1280 * 8;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	srand((unsigned int)time(NULL));

	//========================================
	//プレイヤーに関する変数
	//========================================
	Player player
	{
		{100.0f,300.0f},
		0.0f,
		{10.0f,0.0f},
		{0.0f,-0.8f},
		30.0f,
	};

	JumpPower jumpPower =
	{
		15.0f,//低いとき
		20.0f//高いとき
	};

	const int jumpBottanPower = 15;

	int jumpBottanCount = 0;

	DogSpeed dogSpeed//速さ
	{
		10.0f,//通常時
		5.0f,//遅くなった時
		15.0f//速くなった時
	};

	int playerHitCount = 0;

	int dogColor = 0;
	int dogColorLottery = 0;
	//========================================
	//========================================

	//========================================
	// 箱に関する変数
	//========================================
	const int maxTrap = 10;

	Trap trap[maxTrap];

	for (int i = 0; i < maxTrap; i++)
	{
		trap[i].radius = 90.0f;
	}

	//========================================
	//========================================

	//========================================
	// スクロール用の変数
	//========================================
	int scrollPosX = 0;

	BackPage backPage
	{
		{0.0f, 0.0f}//一個減らした
	};
	//========================================
	//========================================

	//========================================
	// ゴール用の変数
	//========================================
	Goal goal;
	goal.radius = 128;

	goal.position = { 11000, goal.radius };

	goal.newPosY = (goal.position.y - world) * -1.0f;

	//========================================
	//========================================

	//========================================
	// アイテム用の変数
	//========================================
	const int maxItem = 3;

	Item item[maxItem];
	for (int i = 0; i < maxItem; i++)
	{
		item[i].radius = 20;

		item[i].position = { (float)((worldEnd / maxItem) * (i + 1)) , item[i].radius };

		item[i].newPosY = (item[i].position.y - world) * -1.0f;

		item[i].edgePosition.right = item[i].position.x + item[i].radius;
		item[i].edgePosition.left = item[i].position.x - item[i].radius;
		item[i].edgePosition.up = item[i].position.y + item[i].radius;
		item[i].edgePosition.down = item[i].position.y - item[i].radius;
	}
	//========================================
	//========================================

	//========================================
	// ゲームシーン用の変数
	//========================================
	enum Scene
	{
		TITLE,
		RULE,
		GAME,
		GAME_CLEAR,
		GAME_OVER
	};

	//シーン
	int scene = TITLE;

	//アニメーションのカウント
	const int maxAnimationCount = 120;
	int titleAnimationCount = 120;

	int ghCount = 0;
	//========================================
	//=======================================

	//========================================
	// その他の変数
	//========================================
	int gameCount = 0;
	//========================================
	//========================================

	//========================================
	// 画像ファイルの読み込み
	//========================================

	//====================
	// バックグラウンド用
	//====================
	int titleDogGH = Novice::LoadTexture("./Resources/BackGround/TitleDog.png");
	int gameMessageGH = Novice::LoadTexture("./Resources/BackGround/GameMassage.png");
	int backGroundGH = Novice::LoadTexture("./Resources/BackGround/BackGround.png");
	int gameRuleGH = Novice::LoadTexture("./Resources/BackGround/Game_Rule.png");
	//====================
	//====================

	//====================
	// バックグラウンド用
	//====================

	/*int blackDog = Novice::LoadTexture("./Resources/object/BlackDog.png");
	int retDog = Novice::LoadTexture("./Resources/object/RetDog.png");
	int whiteDog = Novice::LoadTexture("./Resources/object/WhiteDog.png");*/
	int bone = Novice::LoadTexture("./Resources/object/Bone.png");
	int HuntingDogGauge = Novice::LoadTexture("./Resources/object/Hunting_Dog_Gauge.png");
	int petOwner = Novice::LoadTexture("./Resources/object/petOwner.png");
	int MuddyWater = Novice::LoadTexture("./Resources/object/Muddy_Water.png");

	//====================
	//====================
	
	//========================================
	//========================================
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		//========================================
		// 背景の確認に関する処理
		//========================================
		if (keys[DIK_0])
		{
			scene = TITLE;
		}
		if (keys[DIK_1])
		{
			scene = RULE;
		}
		if (keys[DIK_2])
		{
			scene = GAME;
		}
		if (keys[DIK_3])
		{
			scene = GAME_CLEAR;
		}
		if (keys[DIK_4])
		{
			scene = GAME_OVER;
		}
		//========================================
		//========================================
		
		switch (scene)
		{
		case TITLE:

			//ゲームに進む
			if (keys[DIK_RETURN] != 0 && preKeys[DIK_RETURN] == 0)
			{
				//======================
				// ゲーム内の初期化
				//======================

				//====================
				// 初期化処理
				//====================
				for (int i = 0; i < maxTrap; i++)//トラップの位置の再設定
				{
					if (i > 0)
					{
						trap[i].trapNum = rand() % 10;
					}
					else
					{
						trap[i].trapNum = 1;
					}
					
					if (trap[i].trapNum != 0)
					{
						trap[i].trapNum = 1;
					}

					if (trap[i].trapNum == 0)
					{
						trap[i].position = { (float)((worldEnd / maxTrap + rand() % 150 + 90) * (i + 1)),(float)(trap[i].radius) };
					}
					else
					{
						trap[i].position = { (float)((worldEnd / maxTrap + rand() % 150 + 90) * (i + 1)),(float)(trap[i].radius * -1 + 1) };
					}
					trap[i].newPosY = (trap[i].position.y - world) * -1.0f;

					trap[i].edgePosition.right = trap[i].position.x + trap[i].radius;
					trap[i].edgePosition.left = trap[i].position.x - trap[i].radius;
					trap[i].edgePosition.up = trap[i].position.y + trap[i].radius;
					trap[i].edgePosition.down = trap[i].position.y - trap[i].radius;
				}

				player.worldPosition = { 100.0f,player.radius };

				backPage = { 0.0f, 0.0f };

				playerHitCount = 0;

				gameCount = 0;

				titleAnimationCount--;

				dogColorLottery = rand() % 100;

				//====================
				//====================

				if (dogColorLottery >= 94)//赤柴
				{
					dogColor = 3;
				}
				else if (dogColorLottery < 50)
				{
					dogColor = 1;
				}
				else
				{
					dogColor = 2;
				}

				//======================
				//======================

				scene = RULE;

				titleAnimationCount = maxAnimationCount;
			}

			if (titleAnimationCount <= 0)
			{
				titleAnimationCount = maxAnimationCount;
			}

			break;


		case RULE:

			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0)
			{
				ghCount++;

				if (ghCount > 1)
				{
					scene = GAME;
					ghCount = 0;
				}
			}



			break;


		case GAME:


			//========================================
			// フレームの最初の処理
			//========================================

			if (playerHitCount > 0)
			{
				playerHitCount--;
			}
			if (playerHitCount < 0)
			{
				playerHitCount++;
			}

			if (gameCount > 0)
			{
				gameCount++;
			}
			if (gameCount > 1200)
			{
				scene = GAME_OVER;
			}

			//========================================
			//========================================

			//========================================
			// プレイヤーの移動に関する処理
			//========================================
			if (playerHitCount == 0)//移動速度低下時の判定の仕方を変更
			{
				player.velocity.x = dogSpeed.normal;
			}
			else if (playerHitCount > 0)
			{
				player.velocity.x = dogSpeed.slow;
			}
			else if (playerHitCount < 0)
			{
				player.velocity.x = dogSpeed.fast;
			}

			if (keys[DIK_A] != 0 || keys[DIK_LEFT])
			{
				player.worldPosition.x -= player.velocity.x;
			}
			else if (keys[DIK_D] != 0 || keys[DIK_RIGHT])
			{
				player.worldPosition.x += player.velocity.x;
			}
			else
			{
				player.velocity.x = 0;
			}

			/*player.worldPosition.x += player.velocity.x;*/

			//==============
			//背景用
			//==============
			if (player.worldPosition.x >= 640 && player.worldPosition.x <= worldEnd + 640)
			{
				backPage.pos1.x = player.worldPosition.x - 640;
			}



			//==============
			//==============

			scrollPosX = (int)player.worldPosition.x - 640;
			if (scrollPosX < 0)
			{
				scrollPosX = 0;
			}
			if (scrollPosX > worldEnd)
			{
				scrollPosX = worldEnd;
			}


			if (player.worldPosition.y > player.radius)
			{
				player.velocity.y += player.accelecion.y;

			}
			else if (player.worldPosition.y < player.radius)
			{
				player.worldPosition.y = player.radius;
				player.velocity.y = 0.0f;
				jumpBottanCount = 0;
			}
			else if (player.worldPosition.y == player.radius)
			{
				if ((keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] != 0))//スペースキーを押している間
				{
					jumpBottanCount++;
				}
				else if ((keys[DIK_SPACE] == 0 && preKeys[DIK_SPACE] != 0))//スペースキーを離したとき
				{
					if (jumpBottanCount < jumpBottanPower)//ジャンプ力が低いとき
					{
						player.velocity.y = jumpPower.low;

					}
					else if (jumpBottanCount >= jumpBottanPower)//ジャンプ力が高いとき
					{
						player.velocity.y = jumpPower.high;
					}
				}
			}

			player.worldPosition.y += player.velocity.y;


			if (player.worldPosition.x < player.radius)
			{
				player.worldPosition.x = player.radius;
			}
			if (player.worldPosition.x > worldEnd + 1280 - player.radius)
			{
				player.worldPosition.x = worldEnd + 1280 - player.radius;
			}


			//========================================
			//========================================


			//========================================
			// 当たり判定の計算
			//========================================
			player.edgePosition.right = player.worldPosition.x + player.radius;
			player.edgePosition.left = player.worldPosition.x - player.radius;
			player.edgePosition.up = player.worldPosition.y + player.radius;
			player.edgePosition.down = player.worldPosition.y - player.radius;


			for (int i = 0; i < maxTrap; i++)//トラップ用当たり判定
			{
				if (player.edgePosition.right >= trap[i].edgePosition.left && player.edgePosition.left <= trap[i].edgePosition.right)//boxより右にいたら
				{
					if (player.edgePosition.down <= trap[i].edgePosition.up)//boxより下に来たら
					{
						if (trap[i].trapNum == 0)
						{
							scene = GAME_OVER;
						}
						else
						{
							playerHitCount = 120;
						}
					}
				}
			}

			for (int i = 0; i < maxItem; i++)//アイテム用当たり判定
			{
				if (player.edgePosition.right >= item[i].edgePosition.left && player.edgePosition.left <= item[i].edgePosition.right)//アイテムより右にいたら
				{
					if (player.edgePosition.down <= item[i].edgePosition.up)//boxより下に来たら
					{
						playerHitCount = -120;
					}
				}
			}
			//========================================
			//========================================

			//========================================
			// ゴール用の処理
			//========================================
			if (player.worldPosition.x >= goal.position.x)
			{
				scene = GAME_CLEAR;
			}
			//========================================
			//========================================

			player.newPosY = (player.worldPosition.y - (world - player.radius)) * -1.0f;


			break;


		case GAME_CLEAR:

			//タイトルに戻る
			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0)
			{
				scene = TITLE;
			}

			break;


		case GAME_OVER:

			//タイトルに戻る
			if (preKeys[DIK_RETURN] == 0 && keys[DIK_RETURN] != 0)
			{
				scene = TITLE;
			}

			break;
		}
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		switch (scene)
		{
		case TITLE:

			Novice::DrawSprite(0, 0, backGroundGH, 1, 1, 0.0f, WHITE);

			if (titleAnimationCount >= maxAnimationCount / 2 && titleAnimationCount <= maxAnimationCount)
			{
				Novice::DrawSpriteRect(320, 150, 0, 0, 640, 360, titleDogGH, static_cast<float>(1) / 2, 1, 0.0f, WHITE);
			}
			if (titleAnimationCount >= 0 && titleAnimationCount <= maxAnimationCount / 2)
			{
				Novice::DrawSpriteRect(320, 150, 640, 0, 640, 360, titleDogGH, static_cast<float>(1) / 2, 1, 0.0f, WHITE);
			}

			break;


		case RULE:

			if (ghCount == 0)
			{
				Novice::DrawSpriteRect(0, 0, 0, 0, 1280, 720, gameRuleGH, static_cast<float>(1) / 2, 1, 0.0f, WHITE);
			}
			else if (ghCount == 1)
			{
				Novice::DrawSpriteRect(0, 0, 1280, 0, 1280, 720, gameRuleGH, static_cast<float>(1) / 2, 1, 0.0f, WHITE);

			}


			break;


		case GAME:


			//========================================
			// 背景
			//========================================
			Novice::DrawSprite(0 - (int)backPage.pos1.x, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 2, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 3, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 4, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 5, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 6, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 7, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);;
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 8, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 9, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 10, (int)backPage.pos1.y, backGroundGH, 1, 1, 0.0f, WHITE);
			//========================================
			//========================================

			//========================================
			//自機
			//========================================
			Novice::DrawBox
			(
				(int)player.worldPosition.x - ((int)backPage.pos1.x + (int)player.radius), (int)player.newPosY,
				(int)player.radius * 2, (int)player.radius * 2, 0.0f, RED, kFillModeSolid
			);

			if (dogColor == 1)
			{

			}

			Novice::DrawLine
			(
				(int)(player.edgePosition.left - backPage.pos1.x), (int)((player.edgePosition.up - 500.0f) * -1),
				(int)(player.edgePosition.left - backPage.pos1.x), 720,GREEN
			);
			Novice::DrawLine
			(
				(int)(player.edgePosition.right - backPage.pos1.x), (int)((player.edgePosition.up - 500.0f) * -1),
				(int)(player.edgePosition.right - backPage.pos1.x), 720, GREEN
			);
			//========================================
			//========================================

			//========================================
			// アイテム
			//========================================
			for (int i = 0; i < maxItem; i++)
			{
				Novice::DrawQuad
				(
					(int)(item[i].edgePosition.left - scrollPosX), (int)((item[i].edgePosition.up - 500.0f) * -1),
					(int)(item[i].edgePosition.right - scrollPosX), (int)((item[i].edgePosition.up - 500.0f) * -1),
					(int)(item[i].edgePosition.left - scrollPosX), (int)((item[i].edgePosition.down - 500.0f) * -1),
					(int)(item[i].edgePosition.right - scrollPosX), (int)((item[i].edgePosition.down - 500.0f) * -1),
					1, 1, 64, 64, bone, WHITE
				);
			}
			//========================================
			//========================================

			//========================================
			// トラップ
			//========================================
			for (int i = 0; i < maxTrap; i++)
			{
				if ((trap[i].position.x - (trap[i].radius + scrollPosX)) >= (goal.position.x - (goal.radius + scrollPosX)) == 0)
				{
					if (trap[i].trapNum == 0)
					{
						Novice::DrawQuad
						(
							(int)(trap[i].edgePosition.left - scrollPosX), (int)((trap[i].edgePosition.up - 500.0f) * -1),
							(int)(trap[i].edgePosition.right - scrollPosX), (int)((trap[i].edgePosition.up - 500.0f) * -1),
							(int)(trap[i].edgePosition.left - scrollPosX), (int)((trap[i].edgePosition.down - 500.0f) * -1),
							(int)(trap[i].edgePosition.right - scrollPosX), (int)((trap[i].edgePosition.down - 500.0f) * -1),
							1, 1, 90, 90, HuntingDogGauge, WHITE
						);
					}
					else {
						Novice::DrawQuad
						(
							(int)(trap[i].edgePosition.left - scrollPosX), (int)((trap[i].edgePosition.up - 500.0f) * -1),
							(int)(trap[i].edgePosition.right - scrollPosX), (int)((trap[i].edgePosition.up - 500.0f) * -1),
							(int)(trap[i].edgePosition.left - scrollPosX), (int)((trap[i].edgePosition.down - 500.0f) * -1),
							(int)(trap[i].edgePosition.right - scrollPosX), (int)((trap[i].edgePosition.down - 500.0f) * -1),
							1, 1, 90, 90, MuddyWater, WHITE
						);
					}
				}
			}
			//========================================
			//========================================

			//========================================
			// ゴール
			//========================================
			Novice::DrawSprite
			(
				(int)(goal.position.x - (goal.radius + scrollPosX)), (int)(goal.newPosY - goal.radius),
				petOwner,2.0f,2.0f,0.0f,WHITE
			);
			//========================================
			//========================================


			Novice::DrawLine(0, 500, 20000, 500, RED);


			Novice::ScreenPrintf(0, 0, "(%d)", (int)player.worldPosition.x);

			Novice::ScreenPrintf(0, 20, "%d", playerHitCount);

			for (int i = 0; i < maxTrap; i++)
			{
				Novice::ScreenPrintf(i * 40, 40, "%.0f", trap[i].position.x - (trap[i].radius));
			}

			Novice::ScreenPrintf(30, 60, "%d", jumpBottanCount);

			Novice::ScreenPrintf(0, 80, "%d",gameCount);

			break;


		case GAME_CLEAR:

			Novice::DrawSprite(0, 0, backGroundGH, 1, 1, 0.0f, WHITE);

			Novice::DrawSpriteRect(400, 200, 0, 160, 1055, 160, gameMessageGH, 0.75, 0.5, 0.0f, WHITE);

			break;


		case GAME_OVER:

			Novice::DrawSprite(0, 0, backGroundGH, 1, 1, 0.0f, WHITE);

			Novice::DrawSpriteRect(100, 360, 0, 0, 1055, 160, gameMessageGH, 0.75, 0.5, 0.0f, WHITE);

			break;
		}
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
