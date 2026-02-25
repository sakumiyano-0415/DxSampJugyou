#pragma once
#include "Base.h"
#include <vector>

class ExplosionEffect :
    public Base
{
public:
	ExplosionEffect(const Vector2D& pos, int particleCount = 20);
	void Update() override;
	void Draw() override;
	bool IsFinished() const { return isFinished_; }
	bool isPlayerEffect_; // プレイヤーの爆発かどうか
	// プレイヤー用エフェクトかどうか設定する関数
	void SetPlayerEffect(bool flag);

	struct Particle
	{
		Vector2D Offset;//位置
		Vector2D vel;//速度
		float life;//寿命
		float radius;//半径
		float alpha;//不透明度
	};
private:
	bool isFinished_;//エフェクト終了フラグ
	std::vector<Particle> particles_;//パーティクル群
};

