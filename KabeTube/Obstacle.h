#pragma once
#include "Box2D\Box2D.h"
typedef b2ParticleSystemDef* PDef ;
typedef b2ParticleSystem* PSys;
typedef b2World* World;
typedef b2Body* Body;
typedef b2BodyDef* BodyDef;
typedef b2Fixture* Fixture;
typedef b2PolygonShape* PShape;
typedef b2CircleShape* CShape;
typedef b2Vec2 V2;
//typedef System::Tuple<float,float>^ V;
typedef array<float>^ Points;
//PhysicSystemと分けた理由
//  アルゴリズムが込み入ってきた
//  コンストラクタが多く必要になった
//    >新しい機能を追加するごとにコンストラクタが
///typedef const char* string;
struct EnemyData{
  const char* Name;

};
ref class Obstacle:System::IDisposable
{
public:
  Obstacle(World,b2Vec2 pos,b2Vec2 size);
  ~Obstacle();
  Body GetBody();
  void Update();
  void Movable();//x,y,xx,yy指定する、初期00
private:
  void invMove(const b2Vec2&);
  Body body;
  Fixture fixture;
  BodyDef def;
  PShape s;
  float32 x,xx,y,yy;
  bool movable; //fenceと区別するのはこれ
  bool inverceMove;
  int inverceCount;
};

ref class Enemy:System::IDisposable
  //:Obstacle
{
  Body body;
  Fixture fixture;
  BodyDef def;
  CShape s;
  Points points; //draw時に名前で区別したほうが設計変更が少ないので、とりあえずおいておく
  EnemyData* e;
public:
  Enemy(b2World* w,b2Vec2 pos,float32 siz);
  void SetPoints(Points);
  ~Enemy();
  //EnemyData GetEnemyData();
  int Age;
  void Impulse(V2);
  Body GetBody();
  void Update();
};
//listに管理させてflyweight作れるけどめんどい