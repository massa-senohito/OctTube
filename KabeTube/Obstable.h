#pragma once
#include "Box2D\Box2D.h"
typedef b2ParticleSystemDef* PDef ;
typedef b2ParticleSystem* PSys;
typedef b2World* World;
typedef b2Body* Body;
typedef b2BodyDef* BodyDef;
typedef b2Fixture* Fixture;
typedef b2PolygonShape* PShape;
typedef b2Vec2 V2;
//PhysicSystemと分けた理由
//  アルゴリズムが込み入ってきた
//  コンストラクタが多く必要になった
//    >新しい機能を追加するごとにコンストラクタが

ref class Obstable:System::IDisposable
{
public:
  Obstable(World,b2Vec2 pos,b2Vec2 size);
  ~Obstable();
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
  bool movable;
  bool inverceMove;
  int inverceCount;
};

ref class Enemy:System::IDisposable
{
  Body body;
  Fixture fixture;
  BodyDef def;
  PShape s;
  float32 x,xx,y,yy;
public:
  Enemy(b2World* w,b2Vec2 pos,b2Vec2 siz);
  ~Enemy();
  void Impulse(V2);
  void Update();
};
//listに管理させてflyweight作れるけどめんどい