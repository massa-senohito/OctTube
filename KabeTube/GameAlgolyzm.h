#pragma once

#include <vector>
#ifdef _MANAGED
#define List System::Collections::Generic::List
typedef List<Obstacle^>^ Obss;
typedef List<Enemy^>^ Enes;
#define Obs List<Obstacle^>
#define Ens List<Enemy^>
#define PEnemy Enemy^
#else
#include "Obstacle.h"
#include "PhysicSystem.h"

enum Stages;
typedef Enemy* PEnemy;
typedef DotnetList<Enemy*>    Enes;
typedef DotnetList<Enemy*>*   PEnes;
typedef CircleSensor Typhoon;
typedef Typhoon* PTyphoon;
typedef DotnetList<Typhoon*>  Tys;
typedef DotnetList<Typhoon*>* PTys;
#endif

typedef float32 f32;

std::vector<std::string> Split(std::string str, char delim);
void drawClothHair(float);
float toRad(float);
float toDeg(float);
//step���O�ɕ���o���Ă������ɂ͂Ȃ�Ȃ�,�ÓI�ۑ����Ԃ�����̂�
//#include "PhysicSystem.h"
//http://manski.net/2011/04/cpp-cli-cheat-sheet/
//CLI���ǂ����m��ɂ́A���ɂ��`�[�g�V�[�g��

//let delAssets
//fade
//let stageLoad i=
//let dragonAI=
  //�H�΂����Ċ��N���Ă��܂��̂ŉH���炢�Ԃ���
#ifdef _MANAGED
using namespace System;
using namespace System::IO;
typedef array<float>^ Points;
typedef System::String^ string;
typedef array<string>^ stringArray;
#define PPhysicSystem PhysicSystem^
#else
//include�g��Ȃ����߂̕K���̍�
#undef Vector
#define Vector std::vector
#define Points float*
#define PPhysicSystem PhysicSystem*
#define String const char*
#define stringArray Vector<std::string>
#define gcnew new
//typedef const Points CPoints;
//Obstacle,renderer�ɂ�����A�X�N���v�g�Ώ���
#endif

//https://github.com/cocos2d/cocos2d-x/blob/v3/cocos/platform/CCPlatformMacros.h
#define SAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define DA(p) SAFE_DELETE_ARRAY(p)
typedef unsigned int uint;
namespace Assets{
  Points getSquidPoints();
  int squidPLen();
  uint* getSquidElem();
}
Points svgRead(String);
uint getSvgVLen();
float bej(float t);
struct ClearStatus{
  bool IsWellBurned;
  int Score;
  ClearStatus(bool well, int score){
    Score = score;
    IsWellBurned = well;
  }
};
typedef DotnetList<ClearStatus*>& ClearStatusList;
class GameAlgolyzm
{
  ClearStatusList clearStatus;
  PhysicSystem* sys;
  ClearStatus* HowAllMeatFired(Score);
  char* message;
  PEnemy addEnemy(EnemyKind);
  bool AllEnemyFired();
  PTyphoon Blow(V2 pos,V2 dir,f32 rad);

  //StageManager�Ɉړ�������\��̂���
  Stages onClearStage(Stages s);
  Stages stage;
  std::string& currentStagePath;
  bool isClear;
  bool isResultScreen();
  void makeMonster(stringArray coes);
  //void makeMonster(stringArray coes);
  void fileRead(String filename, PPhysicSystem sys);
  void PrintResult();
public:
  GameAlgolyzm(Vector<std::string> arg);
  void Step();
  void SetStage(Stages);
  void Render();
  ~GameAlgolyzm(void);
};

