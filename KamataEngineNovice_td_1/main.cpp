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
}Trap;

typedef struct Goal//ゴールの変数
{
	Vector2 position;
	float newPosY;
	float radius;
	EdgePosition edgePosition;
}Goal;

typedef struct BackPage
{
	Vector2 pos1;//一個減らした
}BackPage;

typedef struct DogSpeed
{
	float normal;
	float slow;
}DogSpeed;

typedef struct JumpPower
{
	float low;
	float high;
}JumpPower;

const int world = 500;

const int worldEnd = 1280*7;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };


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

	const int jumpBottanPower = 60;

	int jumpBottanCount = 0;

	DogSpeed dogSpeed//速さ
	{
		10.0f,//通常時
		5.0f//遅くなった時
	};

	int playerHitCount = 0;
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

		trap[i].position = { (float)((worldEnd / maxTrap + rand() % 200) * (i + 1)),(float)(trap[i].radius * -1 + 1) };
		trap[i].newPosY = (trap[i].position.y - world) * -1.0f;

		trap[i].edgePosition.right = trap[i].position.x + trap[i].radius;
		trap[i].edgePosition.left = trap[i].position.x - trap[i].radius;
		trap[i].edgePosition.up = trap[i].position.y + trap[i].radius;
		trap[i].edgePosition.down = trap[i].position.y - trap[i].radius;
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

	goal.position = { worldEnd - 200, goal.radius };
	//========================================
	//========================================

	//========================================
	// その他の変数
	//========================================
	int a = 0;


	//========================================
	//========================================

	//========================================
	// 画像ファイルの読み込み
	//========================================
	int gameBackGround = Novice::LoadTexture("./resources/BackGround/BackGround.png");

	int white1x1 = Novice::LoadTexture("./NoviceResources/white1x1.png");


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
		// フレームの最初の処理
		//========================================

		if (playerHitCount > 0)
		{
			playerHitCount--;
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
		else if (playerHitCount != 0)
		{
			player.velocity.x = dogSpeed.slow;
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
					a = 1;

				}
				else if (jumpBottanCount >= jumpBottanPower)//ジャンプ力が高いとき
				{
					player.velocity.y = jumpPower.high;
					a = 2;
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

		
		for (int i = 0; i < maxTrap; i++)
		{
			if (player.edgePosition.right >= trap[i].edgePosition.left)//boxより右にいたら
			{
				if (player.edgePosition.down <= trap[i].edgePosition.up)//boxより上に来たら
				{
					if (player.edgePosition.left <= trap[i].edgePosition.right)
					{
						playerHitCount = 120;
					}
				}
			}
		}

		//========================================
		//========================================


		player.newPosY = (player.worldPosition.y - world) * -1.0f;


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		/// 背景
		/*Novice::DrawSprite
		(0-(int)backPage.pos1.x, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		(0-(int)backPage.pos1.x+1280, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		(0-(int)backPage.pos1.x+1280*2, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		(0-(int)backPage.pos1.x+1280*3, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		(0-(int)backPage.pos1.x+1280*4, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);*/

		//========================================
		// 背景
		//========================================
		Novice::DrawSprite(0 - (int)backPage.pos1.x, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 2, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 3, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 4, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 5, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 6, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		Novice::DrawSprite(0 - (int)backPage.pos1.x + 1280 * 7, (int)backPage.pos1.y, gameBackGround, 1, 1, 0.0f, WHITE);
		//========================================
		//========================================
		
		//========================================
		//自機
		//========================================
		/*Novice::DrawEllipse((int)player.worldPosition.x - (int)backPage.pos1.x, (int)player.newPosY, 50, 50, 0.0f, GREEN, kFillModeSolid);*/

		

		Novice::DrawQuad
		(
			(int)player.edgePosition.left - (int)backPage.pos1.x, (int)((player.edgePosition.up - world) * -1.0f), (int)player.edgePosition.right - (int)backPage.pos1.x, (int)((player.edgePosition.up - world) * -1.0f),
			(int)player.edgePosition.left - (int)backPage.pos1.x, (int)((player.edgePosition.down - world) * -1.0f), (int)player.edgePosition.right - (int)backPage.pos1.x, (int)((player.edgePosition.down - world) * -1.0f),
			1, 1, 64, 64, white1x1, GREEN
		);

		Novice::DrawBox
		(
			(int)player.worldPosition.x - ((int)backPage.pos1.x + (int)player.radius), (int)player.newPosY,
			(int)player.radius * 2, (int)player.radius * 2, 0.0f, RED, kFillModeSolid
		);
		//========================================
		//========================================


		/*Novice::DrawBox
		(
			(int)(player.worldPosition.x - (player.radius + scrollPosX)), (int)(player.newPosY - player.radius),
			(int)player.radius * 2, (int)player.radius * 2,
			0.0f, RED, kFillModeSolid
		);*/

		/*Novice::DrawBox
		(
			(int)(trap1.position.x - (trap1.radius + scrollPosX)), (int)(trap1.newPosY - trap1.radius),
			(int)trap1.radius * 2, (int)trap1.radius * 2,
			0.0f, BLUE, kFillModeSolid
		);

		Novice::DrawBox
		(
			(int)(trap2.position.x - (trap2.radius + scrollPosX)), (int)(trap2.newPosY - trap2.radius),
			(int)trap2.radius * 2, (int)trap2.radius * 2,
			0.0f, BLUE, kFillModeSolid
		);*/

		for (int i = 0; i < maxTrap; i++)
		{
			Novice::DrawBox
			(
				(int)(trap[i].position.x - (trap[i].radius + scrollPosX)), (int)(trap[i].newPosY - trap[i].radius),
				(int)trap[i].radius * 2, (int)trap[i].radius * 2,
				0.0f, BLUE, kFillModeSolid
			);
		}

		//Novice::ScreenPrintf(0, 0, "x1 %.0f, x2 %.0f",backPage.pos1.x, backPage.pos2.x);
		Novice::ScreenPrintf(0, 20, "%d", playerHitCount);

		for (int i = 0; i < maxTrap; i++)
		{
			Novice::ScreenPrintf(i * 40, 40, "%.0f", trap[i].position.x - (trap[i].radius));
		}

		if (a == 1)
		{
			Novice::ScreenPrintf(0, 60, "low");
		}
		if (a == 2)
		{
			Novice::ScreenPrintf(0, 60, "high");
		}

		Novice::ScreenPrintf(0, 0, "(%d)", (int)player.worldPosition.x);
		Novice::ScreenPrintf(0, 0, "(%d)", (int)player.worldPosition.x);
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
