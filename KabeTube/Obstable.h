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
//PhysicSystem�ƕ��������R
//  �A���S���Y�������ݓ����Ă���
//  �R���X�g���N�^�������K�v�ɂȂ���
//    >�V�����@�\��ǉ����邲�ƂɃR���X�g���N�^��

ref class Obstable:System::IDisposable
{
public:
  Obstable(World,b2Vec2 pos,b2Vec2 size);
  ~Obstable();
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
//list�ɊǗ�������flyweight���邯�ǂ߂�ǂ�