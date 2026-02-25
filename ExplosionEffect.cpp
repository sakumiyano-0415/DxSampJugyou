#include "ExplosionEffect.h"
#include "DxLib.h"
#include <algorithm>
#include "easefunction.h"


namespace
{
	const float PARTICLE_LIFE = 1.0f; //パーティクルの寿命(秒)
	const float PARTICLE_RADIUS = 1.5f; //パーティクルの半径
	const float PARTICLE_SPEED[3] = { 50.0f, 80.0f, 120.0f }; //パーティクルの速度
	const float PARTICLE_DECAY = 0.95f; //パーティクルの速さの減衰率
	const float PARTICLE_LIFE_FADE_START = PARTICLE_LIFE/2.0f; //寿命の半分から色を薄くする
}


ExplosionEffect::ExplosionEffect(const Vector2D& pos, int particleCount)
	:Base(pos, { 0.0f, 0.0f }, GetColor(255, 255, 255))
{
	SetObjType(OBJ_TYPE::EFFECT);
	particles_.clear();
	isFinished_ = false;



	//パーティクル1個の初期化
	for (int i = 0; i < particleCount; i++)
	{
		Particle particle;
		particle.Offset = { 0.0f, 0.0f };
		particle.vel = { 0.0f, 0.0f };
		particle.life = PARTICLE_LIFE;//寿命3秒
		particle.radius = PARTICLE_RADIUS;//半径5.0f
		particle.alpha = 1.0f;//不透明度1.0f

		float angle_rad = (float)GetRand(360) * (Math2D::PI / 180.0f);
		Vector2D direction = Math2D::FromAngle(angle_rad);
		particle.vel = Math2D::Mul(direction, PARTICLE_SPEED[GetRand(2)]);

		//リストに追加
		//particles_[0] = particle;
		particles_.push_back(particle);
	}
}





void ExplosionEffect::Update()
{
	float dt = GetDeltaTime();
	bool allDead = true;

	for (auto& particle : particles_)
	{
		//寿命が残ってたら更新
		if (particle.life > 0.0f)
		{
			allDead = false;//まだ生きてるパーティクルがある

			//位置更新
			particle.Offset = Math2D::Add(particle.Offset, Math2D::Mul(particle.vel, dt));

			//速度減衰
			//particle.vel = Math2D::Mul(particle.vel, PARTICLE_DECAY);
			//イージング使って、早さを変更してみて！


			//寿命減少
			particle.life -= dt;
			if (particle.life < 0.0f)
				particle.life = 0.0f;
		}

		//不透明度の更新
		//if (particle.life < PARTICLE_LIFE_FADE_START)
		//{
		//	particle.alpha = particle.life / PARTICLE_LIFE_FADE_START;
		//}
		//else
		//{
		//	particle.alpha = 1.0f;
		//}

		float lifeRatio = std::clamp(1.0f - particle.life / PARTICLE_LIFE, 0.0f, 1.0f);
		particle.alpha = 1.0f - Direct3D::EaseFunc["InOutExpo"](lifeRatio);
		

	}

	//全パーティクルが寿命切れならエフェクト終了
	if (allDead) //allDead == true と同じ
	{
		isFinished_ = true;
	}

}

void ExplosionEffect::Draw()
{
	int col[3] = { 161, 129, 42 }; // 爆発の色を変えるときはここをいじるといいよ
	if (isFinished_) return;//終了してたら描画しない

	for(auto& particle : particles_)
	{
		if (particle.life > 0.0f)
		{
			//パーティクルの描画
			Vector2D drawPos = Math2D::Add(GetPos(), particle.Offset);
			Vector2D screenPos = Math2D::World2Screen(drawPos);
			int particleColor = GetColor(
				(int)(255 * particle.alpha),
				(int)(255 * particle.alpha),
				(int)(255 * particle.alpha)
			);
			DrawCircle((int)screenPos.x,
						(int)screenPos.y,
						particle.radius,
						particleColor);
		}
	}

}
//エフェクトがじんわり消えるところを実装
//あるlifeからだんだん色を薄くして、さいご黒になればいいよ。