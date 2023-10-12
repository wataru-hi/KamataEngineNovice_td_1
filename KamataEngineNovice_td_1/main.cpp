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
	int number;
	Vector2 position;
	float newPosY;
	float radius;
	EdgePosition edgePosition;
}Trap;

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
	int jumpCount = 0;

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
	Trap trap;
	
	trap.radius = 90.0f;

	trap.position = { 1000.0f,trap.radius};
	trap.newPosY = (trap.position.y - world) * -1.0f;
	
	trap.edgePosition.right = trap.position.x + trap.radius;
	trap.edgePosition.left = trap.position.x - trap.radius;
	trap.edgePosition.up = trap.position.y + trap.radius;
	trap.edgePosition.down = trap.position.y - trap.radius;
	//========================================
	//========================================

	//========================================
	// スクロール用の変数
	//========================================
	int scrollPosX = 0;
	//========================================
	//========================================

	//========================================
	// その他の変数
	//========================================
	
	int beside = 0;

	srand((unsigned int)time(NULL));//乱数の初期化



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
		beside = 0;

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
			if ((keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) && (jumpCount == 1))
			{
				player.velocity.y = jumpPower.second;
				jumpCount = 2;
			}
		}
		else if (player.worldPosition.y < player.radius)
		{
			player.worldPosition.y = player.radius;
			player.velocity.y = 0.0f;
			jumpCount = 0;
		}
		else if (player.worldPosition.y == player.radius)
		{
			if ((keys[DIK_SPACE] != 0 && preKeys[DIK_SPACE] == 0) && (jumpCount == 0))
			{
				player.velocity.y = jumpPower.first;
				jumpCount = 1;
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

		if (player.edgePosition.right <= trap.edgePosition.left)//boxより右にいたら
		{
			beside = 1;
		}

		if ((player.edgePosition.down >= trap.edgePosition.up) == 0)//boxより上に来たら
		{
			if ((player.edgePosition.left >= trap.edgePosition.right) == 0)
			{
				if (beside != 1)
				{
					playerHitCount = 120;
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
		Novice::DrawBox
		(
			(int)(player.worldPosition.x - (player.radius + scrollPosX)), (int)(player.newPosY - player.radius),
			(int)player.radius * 2, (int)player.radius * 2,
			0.0f, RED, kFillModeSolid
		);

		Novice::DrawBox
		(
			(int)(trap.position.x - (trap.radius + scrollPosX)), (int)(trap.newPosY - trap.radius),
			(int)trap.radius * 2, (int)trap.radius * 2,
			0.0f, BLUE, kFillModeSolid
		);
	
		Novice::ScreenPrintf(0, 0, "testGit");
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
