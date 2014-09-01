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
//PhysicSystem�ƕ��������R
//  �A���S���Y�������ݓ����Ă���
//  �R���X�g���N�^�������K�v�ɂȂ���
//    >�V�����@�\��ǉ����邲�ƂɃR���X�g���N�^��
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
  void Movable();//x,y,xx,yy�w�肷��A����00
private:
  void invMove(const b2Vec2&);
  Body body;
  Fixture fixture;
  BodyDef def;
  PShape s;
  float32 x,xx,y,yy;
  bool movable; //fence�Ƌ�ʂ���̂͂���
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
  Points points; //draw���ɖ��O�ŋ�ʂ����ق����݌v�ύX�����Ȃ��̂ŁA�Ƃ肠���������Ă���
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
//list�ɊǗ�������flyweight���邯�ǂ߂�ǂ�