#pragma once
#include "Box2D\Box2D.h"
#include "Enemy.h"
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
//error LNK2038 : '_ITERATOR_DEBUG_LEVEL' の不一致が検出されました。

class EnemyData{
public:
  const char* Name=nullptr;
  int* Damage=nullptr;
  (void)(* PlayDamagedSound)();
  EnemyData(int d, const char* n){
    Damage = new int(d);
    Name = n;
  }
  EnemyData(){
  
  }
};
#ifdef _MANAGED
typedef array<float>^ Points;
#else
#define ref /**/
#define Vector std::vector
#define Points float*
typedef EnemyData* PEnemyData;
//typedef const Points CPoints;
#endif
//PhysicSystemと分けた理由
//  アルゴリズムが込み入ってきた
//  コンストラクタが多く必要になった
//    >新しい機能を追加するごとにコンストラクタが
///typedef const char* string;
ref class Obstacle
#ifdef _MANAGED
: IDisposable
#endif
{
  void invMove(const b2Vec2&);
  Body body;
  Fixture fixture;
  BodyDef def;
  PShape s;
  float32 x,xx,y,yy;
  bool movable; //fenceと区別するのはこれ
  bool inverceMove;
  int inverceCount;
public:
  Obstacle(World,b2Vec2 pos,b2Vec2 size);
  ~Obstacle();
  Body GetBody();
  void Update();
  void Movable();//x,y,xx,yy指定する、初期00
};

ref class Enemy
#ifdef _MANAGED
: IDisposable
#endif
{
  Body body;
  Fixture fixture;
  BodyDef def;
  PShape s;
  b2AABB actBox;
  Points points; //draw時に名前で区別したほうが設計変更が少ないので、とりあえずおいておく
  EnemyData* e;
  int pointsLength;
  void squidProfile(World,V2);
  void motion();
  AnimAsset* anim;
  SoundAsset* sound;
  Body* tents;
  Body* sqTentacle(V2 parentPos);
  PEnemyData* tentData;
public:
  Enemy(b2World* w,b2Vec2 pos,float32 siz);
  //void SetPoints(Points,int);
  void SetAssets(int kind);
  bool IsAllMeatFired();
  int* GainPoints();
  ~Enemy();
  //EnemyData GetEnemyData();
  int Age;
  void Impulse(V2);
  void Force(V2 v);
  void Veloc(V2 v);

  Body GetBody();
  void Update(bool);
};