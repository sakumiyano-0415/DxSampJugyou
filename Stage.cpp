#include "Base.h"
#include "Stage.h"
#include "Math2D.h"
#include "Player.h"
#include "globals.h"
#include <DxLib.h>
#include "Bullet.h"
#include "Input.h"
#include <vector>
#include "Enemy.h"
#include "ExplosionEffect.h"


namespace
{
	//Player Parameters :　プレイヤーの初期値
	const Vector2D START_POS = {WIN_WIDTH/2, WIN_HEIGHT/2 };
	const Vector2D START_VEL = { 0.0f, 0.0f };
	const Vector2D START_DIR = { 0.0f, -1.0f };
	const float START_RADIUS = 30.0f;
	const float START_OMEGA = 2.0f;
	const unsigned int START_COLOR = GetColor(255, 0, 0);
	const unsigned int ENEMY_MAX = 100; //敵の最大数
	const unsigned int ENEMY_NUM = 10; //最初に出現する敵の数
	Player* player = nullptr;
	//std::vector<Bullet*> bullets; //弾丸の保管庫
	//std::vector<Enemy*> enemies; //敵の保管庫
	//std::vector<ExplosionEffect*> effects; //エフェクトの保管庫

	std::vector<Base*> objects; //全てのオブジェクトの保管庫
	//オブジェクトの保管庫にオブジェクトを追加する関数
	void AddObject(Base* obj)
	{
		objects.push_back(obj);
	}

	void UpdateAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Update();
		}
	}
	void DrawAllObjects()
	{
		for (auto& obj : objects)
		{
			obj->Draw();
		}
	}
}


Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
	player = new Player(START_POS, START_VEL, START_COLOR,
		                START_DIR, START_RADIUS, START_OMEGA);

	AddObject(player);

	//enemies.clear();
	//enemies.reserve(ENEMY_NUM);

	//最初の敵を生成
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		Enemy* e = new Enemy(Enemy::Size::LARGE, 8);
		AddObject(e);
	}

	totalScore_ = 0;
}

void Stage::Update()
{
	// 敵と弾の当たり判定
	Enemy_vs_Bullet();
	
	//賞味期限切れの弾を消す
	DeleteBullet();
	//死んでる敵を消す
	DeleteEnemy();
	UpdateAllObjects();

	//Zキーが押されたら弾丸を生成
	if (Input::IsKeyDown(KEY_INPUT_Z))
	{
		ShootBullet();
	}
}

void Stage::Draw()
{
	DrawAllObjects();
	DrawFormatString(10, 10, GetColor(255, 255, 255),
		"SCORE : %020lld", totalScore_); //スコア表示
}

void Stage::Release()
{
	//if (player != nullptr)
	//	delete player;
	//for (int i = 0;i < enemies.size();i++)
	//{
	//	if (enemies[i] != nullptr)
	//		delete enemies[i];
	//}
}

void Stage::Enemy_vs_Bullet()
{
	//敵の位置と、当たり判定の半径
	//弾の位置
	//isAlive_ -> falseにする手段
	std::vector<Enemy*> aliveEnemies;
	std::vector<Bullet*> aliveBullets;

	aliveEnemies.clear();//念のため、毎フレームaliveEnemiesを空にする
	aliveBullets.clear();//念のため、毎フレームaliveBulletsを空にする

	//for(int i=0; i < objects.size(); i++)
	for (auto& obj : objects)
	{
		if (obj->GetType() == OBJ_TYPE::ENEMY)
		{
			//baseクラスのポインタを敵クラスのポインタに変換してる
			Enemy* e = (Enemy*)obj;
			if (e->IsAlive()) {
				aliveEnemies.push_back(e);
			}
		}
		else if (obj->GetType() == OBJ_TYPE::BULLET)
		{
			//baseクラスのポインタを弾クラスのポインタに変換してる
			Bullet* b = (Bullet*)obj;
			if (!b->IsDead()) {
				aliveBullets.push_back(b);
			}
		}
	}
	//for(int i=0;i<aliveBullets.size();i++)
	for (auto& bullet : aliveBullets)
	{
		for (auto& enemy : aliveEnemies)
		{
			float dist = Math2D::Length(Math2D::Sub(bullet->GetPos(), enemy->GetPos()));
			if (dist < enemy->GetCollisionRadius())
			{
				//当たった
				enemy->Dead();//敵を消す(生存フラグをfalseに）
				int sc[3] = { 20, 50, 100 }; // 大、中、小のスコア
				int score = sc[enemy->GetSize()]; // 敵のサイズに応じたスコアを取得
				totalScore_ += score; // スコアを加算
				//分裂の処理をここでやりたい
				//大か中か小かを判定して
				//大なら中を2~4つ、中なら小を2~4つ、小なら消してエフェクト生成
				if (enemy->GetSize() != Enemy::Size::SMALL)
				{
					int num = GetRand(3) + 2; //2~4のランダムな数
					//大きさによって、分裂数変えると素敵です。
					for (int i = 0;i < num;i++)
					{
						Enemy* e = nullptr;
						if (enemy->GetSize() == Enemy::Size::LARGE)
						{
							e = new Enemy(Enemy::Size::MEDIUM, 8);
						}
						else
						{
							e = new Enemy(Enemy::Size::SMALL, 8);
						}
						e->SetPos(enemy->GetPos());
						//速さの設定は必要
						e->SetVel({ (float)(GetRand(200) - 100), (float)(GetRand(200) - 100) });
						AddObject(e);
					}
				}
				else
				{
					ExplosionEffect* effect = new ExplosionEffect(enemy->GetPos());
					AddObject(effect);
				}
				bullet->Dead();//弾も消す
			}
		}
	}
}

void Stage::DeleteBullet()
{
	//賞味期限切れの弾を消す
	//まず、箱の中身を確認して、死んでる弾があったらdeleteする（箱は残るので、nullptrを入れておく）
	//for (int i = 0;i < objects.size();i++)
	for (auto& itr : objects)
	{
		if(itr->GetType() == OBJ_TYPE::BULLET)
		{
			//base->継承クラスの時は、ちゃんと継承クラスのポインタに変換してあげないと、継承クラスのメンバ関数は呼び出せない
			//継承クラス→baseクラスの変換は暗黙的に行われる
			Bullet* b = (Bullet *)(itr);
			if (b->IsDead())
			{
				delete b;
				itr = nullptr; //ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める）
	for(auto it = objects.begin(); it != objects.end(); )
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::DeleteEnemy()
{
	//死んでる敵を消す
	for (auto& itr : objects)
	{
		if (itr->GetType() == OBJ_TYPE::ENEMY)
		{
			Enemy* b = (Enemy*)(itr);
			if (b->IsAlive() == false)
			{
				delete b;
				itr = nullptr; //ポインタをnullptrにしておく
			}
		}
	}
	//次に、箱の中身を確認して、nullptrがあったら箱から消す(箱自体を詰める）
	for (auto it = objects.begin(); it != objects.end(); )
	{
		if (*it == nullptr)
		{
			it = objects.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void Stage::ShootBullet()
{
	Vector2D pos = player->GetPos();
	Vector2D v = Math2D::Mul(player->GetDirVec(), 300.0f);
	unsigned int bcol = GetColor(255, 255, 255);
	float r = 2;
	float life = 2.0f;

	Bullet* b = new Bullet(pos, v, bcol, r, life);
	AddObject(b);
}
