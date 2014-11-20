#pragma once
#include "Box2D\Box2D.h"
#include "Assets.h"
//#include "DotNetWrap.h"
//include���邾���ŃR���p�C���ʂ�Ȃ��Ȃ�
typedef b2ParticleSystemDef* PDef ;
typedef b2ParticleSystem* PSys;
typedef b2World* World;
typedef b2Body* Body;
typedef b2BodyDef* BodyDef;
typedef b2Fixture* Fixture;
typedef b2PolygonShape* PShape;
typedef b2CircleShape* CShape;
typedef b2Vec2 V2;
using Bodies = std::vector<Body>;
//#define Bodies std::vector<Body>
//typedef PEnemyData* AppendBodyData;//vector��

#define SAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define DA(p) SAFE_DELETE_ARRAY(p)
#define zerov V2(0, 0)
void playSquidDamageSound();
typedef void(* OnHit)() ;
class EnemyData{
public:
  const char* Name=nullptr;
  int* Damage=nullptr;
  OnHit onhit;
  V2 browDir;
  //�����Ńe�N�X�`����T�E���h�f�[�^��o�^���Ă�contact���ɌĂяo����
  //Enemy�|�C���^���������Ⴄ��
  EnemyData(int d, const char* n) 
    :onhit(playSquidDamageSound),
    browDir(zerov)
  {
    Damage = new int(d);
    Name = n;
  }
  EnemyData() 
    :onhit(playSquidDamageSound),
    browDir(zerov)
  {
  
  }
  ~EnemyData()
  {
    SAFE_DELETE(Damage);
  }
  void SetOnHit(OnHit h){
    onhit = h;
  }
};
typedef EnemyData* PEnemyData;
typedef std::vector<PEnemyData> AppendBodyData;
typedef std::shared_ptr<std::vector<int>> Score;
//typedef System::Tuple<float,float>^ V;
//error LNK2038 : '_ITERATOR_DEBUG_LEVEL' �̕s��v�����o����܂����B
//debug�r���h����vector�̃T�C�Y�`�F�b�N������A�����[�X�r���h���Ɠ���Ȃ��Ȃ�A���̕s��v�̂���
#ifdef _MANAGED
typedef array<float>^ Points;
#else
#define ref /**/
#define Points float*
//typedef float* Points;
#endif
//PhysicSystem�ƕ��������R
//  �A���S���Y�������ݓ����Ă���
//  �R���X�g���N�^�������K�v�ɂȂ���
//    >�V�����@�\��ǉ����邲�ƂɃR���X�g���N�^��
///typedef const char* string;

class CircleSensor{
  PEnemyData e;
  CShape s;
  Fixture fix;
  Body bod;
  BodyDef bdef;
public:
  CircleSensor(World,V2 pos,V2 dir,float);
  ~CircleSensor();
};

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
  bool movable; //fence�Ƌ�ʂ���̂͂���
  bool inverceMove;
  int inverceCount;
public:
  Obstacle(World,b2Vec2 pos,b2Vec2 size);
  bool isOuter;
  ~Obstacle();
  Body GetBody();
  void Update();
  void Movable();//x,y,xx,yy�w�肷��A����00
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
  EnemyData* e;
  World world;
  void squidProfile(World,V2);
  void motion();
  AnimAsset* anim;
  //�ǉ��{�f�B,�N���X�ɂ܂Ƃ߂��Disopose�ň�C�ɊJ���ł���
  Bodies* appendBody;
  Bodies* sqTentacle(V2 parentPos);
  AppendBodyData tentData;
  int hp;
  bool selfDelete;
  Enemy(const Enemy&) ;
  Enemy& operator=(const Enemy&);
public:
  Enemy(b2World* w,EnemyKind);
  b2AABB& GetActBox();
  void SetHp(int);
  void SetPos(V2&);
  void SetProfile(EnemyKind);
  bool IsAllMeatFired();
  void Enemy::RefreshSubHP();
  Score GetScore();
  ~Enemy();
  //EnemyData GetEnemyData();
  int Age;
  void Impulse(V2);
  void Force(V2 v);
  void Veloc(V2 v);
  void UnLoad(EnemyKind);

  Body GetBody();
  void Update(bool);
};