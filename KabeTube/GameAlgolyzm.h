#pragma once
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
#define Vector std::vector
#define Points float*
#define PPhysicSystem PhysicSystem*
#define String const char*
#define stringArray Vector<std::string>
#define gcnew new
//typedef const Points CPoints;
//Obstacle,renderer�ɂ�����A�X�N���v�g�Ώ���
#define ref /**/
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
class GameAlgolyzm
{
public:
  GameAlgolyzm(stringArray arg);
  void Render();
  ~GameAlgolyzm(void);
};

