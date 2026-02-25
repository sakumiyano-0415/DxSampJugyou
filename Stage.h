#pragma once
#include <vector>



// 前方宣言
class Player;
class Enemy;

class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void Update();
	void Draw();
	void Release();
	int scene_; // 0=タイトル 1=ゲーム 2=ゲームオーバー
	int gameOverTimer_;    // ゲームオーバー用タイマー
private:
	//Player* player_;
	//Enemy* enemy_;
	void Enemy_vs_Bullet();
	void Player_vs_Enemy();
	void DeleteBullet();
	void DeleteEnemy();
	void ShootBullet();
	void ClearAllObjects();
	int long long totalScore_;
};

