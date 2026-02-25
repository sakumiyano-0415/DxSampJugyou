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
private:
	//Player* player_;
	//Enemy* enemy_;
	void Enemy_vs_Bullet();
	void DeleteBullet();
	void DeleteEnemy();
	void ShootBullet();
	int long long totalScore_;
};

