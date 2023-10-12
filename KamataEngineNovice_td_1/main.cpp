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

typedef struct BackPage
{
	Vector2 pos1;
	Vector2 pos2;
}BackPage;

typedef struct JumpPower
{
	float first;
	float second;
}jumpPower;

typedef struct DogSpeed
{
	float normal;
	float slow;
}DogSpeed;

typedef struct Scene
{
	int title;
	int game;
	int score;
}Score;

const int world = 500;

const int worldEnd = 8000;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	srand((unsigned int)time(NULL));//乱数の初期化

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

	JumpPower jumpPower//ジャンプ力
	{
		20.0f,15.0f
	};

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
	/*Trap trap1;
	
	trap1.radius = 90.0f;

	trap1.position = { 1000.0f,trap1.radius};
	trap1.newPosY = (trap1.position.y - world) * -1.0f;
	
	trap1.edgePosition.right = trap1.position.x + trap1.radius;
	trap1.edgePosition.left = trap1.position.x - trap1.radius;
	trap1.edgePosition.up = trap1.position.y + trap1.radius;
	trap1.edgePosition.down = trap1.position.y - trap1.radius;

	
	
	Trap trap2;

	trap2.radius = 90.0f;

	trap2.position = { 1500.0f,trap2.radius * -1 + 1};
	trap2.newPosY = (trap2.position.y - world) * -1.0f;

	trap2.edgePosition.right = trap2.position.x + trap2.radius;
	trap2.edgePosition.left = trap2.position.x - trap2.radius;
	trap2.edgePosition.up = trap2.position.y + trap2.radius;
	trap2.edgePosition.down = trap2.position.y - trap2.radius;*/

	const int maxTrap = 10;

	Trap trap[maxTrap];

	for (int i = 0; i < maxTrap; i++)
	{
		trap[i].radius = 90.0f;

		trap[i].position = {(float)((worldEnd / maxTrap + rand() % 400 + 100) * (i + 1)),(float)(trap[i].radius * -1 + 1)}; 
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
		{    0.0f, 0.0f,},
		{-1280.0f, 0.0f,},
	};
	//========================================
	//========================================

	//========================================
	// その他の変数
	//========================================

	

	//========================================
	//========================================

	//========================================
	// 画像ファイルの読み込み
	//========================================
	int gameBackGround = Novice::LoadTexture("./resources/BackGround/BackGround.png");

	
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
		if (keys[DIK_A] != 0)
		{
			if (playerHitCount == 0)
			{
				player.velocity.x = dogSpeed.normal * -1;
			}
			if (playerHitCount != 0)
			{
				player.velocity.x = dogSpeed.slow * -1;
			}
		}
		else if (keys[DIK_D] != 0)
		{
			if (playerHitCount == 0)
			{
				player.velocity.x = dogSpeed.normal;
			}
			if (playerHitCount != 0)
			{
				player.velocity.x = dogSpeed.slow;
			}
		}
		else
		{
			player.velocity.x = 0;
		}

		player.worldPosition.x += player.velocity.x;
		//==============
		//背景用
		//==============
		backPage.pos1.x -= player.velocity.x;
		backPage.pos2.x -= player.velocity.x;

		if (backPage.pos1.x >= 1280)
		{
			backPage.pos1.x = 1280 * 2;
		}
		if (backPage.pos2.x >= 1280)
		{
			backPage.pos2.x = 1280 * 2;
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
		}
		else if (player.worldPosition.y == player.radius)
		{
			if ((keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0))
			{
				player.velocity.y = jumpPower.first;
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

		//if ((player.edgePosition.right <= trap1.edgePosition.left) == 0)//boxより右にいたら
		//{
		//	if ((player.edgePosition.down >= trap1.edgePosition.up) == 0)//boxより上に来たら
		//	{
		//		if ((player.edgePosition.left >= trap1.edgePosition.right) == 0)
		//		{
		//			playerHitCount = 120;
		//		}
		//	}
		//}

		//if ((player.edgePosition.right <= trap2.edgePosition.left) == 0)//boxより右にいたら
		//{
		//	if ((player.edgePosition.down >= trap2.edgePosition.up) == 0)//boxより上に来たら
		//	{
		//		if ((player.edgePosition.left >= trap2.edgePosition.right) == 0)
		//		{
		//			playerHitCount = 120;
		//		}
		//	}
		//}
		for (int i = 0; i < maxTrap; i++)
		{
			if ((player.edgePosition.right <= trap[i].edgePosition.left) == 0)//boxより右にいたら
			{
				if ((player.edgePosition.down >= trap[i].edgePosition.up) == 0)//boxより上に来たら
				{
					if ((player.edgePosition.left >= trap[i].edgePosition.right) == 0)
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
		Novice::DrawSprite
		(
			(int)backPage.pos1.x, (int)backPage.pos1.y,
			gameBackGround, 1, 1, 0.0f, WHITE
		);

		Novice::DrawSprite
		(
			(int)backPage.pos2.x, (int)backPage.pos2.y,
			gameBackGround, 1, 1, 0.0f, WHITE
		);
		
		Novice::DrawBox
		(
			(int)(player.worldPosition.x - (player.radius + scrollPosX)), (int)(player.newPosY - player.radius),
			(int)player.radius * 2, (int)player.radius * 2,
			0.0f, RED, kFillModeSolid
		);

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

		Novice::ScreenPrintf(0, 0, "x1 %.0f, x2 %.0f",backPage.pos1.x, backPage.pos2.x);
		Novice::ScreenPrintf(0, 20, "%d", playerHitCount);
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
