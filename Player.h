#pragma once
#include "Base.h"

class Player :
    public Base
{
public:
	//コンストラクタ
	Player();
	Player(const Vector2D& pos, const Vector2D& vel, unsigned int color,
		   const Vector2D& dir, float r, float omega);
	//デストラクタ
	~Player();
	//OverRide類
	void Update() override; //オーバーライドしそう
	void Draw() override; //オーバーライドしそう
public://セッターゲッターは全部インラインで書いたよ
	void SetDirVec(const Vector2D& dir) { dir_ = dir; }
	Vector2D GetDirVec() { return dir_; }
	void SetRadius(float r) { radius_ = r; }
	float GetRadius() { return radius_; }
	void SetOmega(float omega) { omega_ = omega; };
	float GetOmega() { return omega_; }
	void SetAngle(float angle) { angle_ = angle; }
	float GetAngle() { return angle_; }
	float GetCollisionRadius() const;
	void Dead();
	bool IsAlive() { return isAlive_; }
private:
	//独自のメンバ
	//自分の向き（ベクトル）
	//3角形の3点の座標
	Vector2D dir_; //自分の向きベクトル
	float radius_; //半径
	Vector2D vertex_[3]; //3角形の頂点座標
	float omega_; //回転速度(角速度)
	float angle_; //現在の回転角
	bool isAlive_; //生存フラグ true:生きてる false:死んでる
};

