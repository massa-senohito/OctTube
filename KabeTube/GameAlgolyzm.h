#pragma once
void drawClothHair(float);
float toRad(float);
float toDeg(float);
//stepを外に放り出しても解決にはならない,静的保存期間があるので
//#include "PhysicSystem.h"
//http://manski.net/2011/04/cpp-cli-cheat-sheet/
//CLIかどうか知るには、他にもチートシートが

//let delAssets
//fade
//let stageLoad i=
//let dragonAI=
  //羽ばたいて喚起してしまうので羽からいぶして
#ifdef _MANAGED
using namespace System;
using namespace System::IO;
typedef array<float>^ Points;
typedef System::String^ string;
typedef array<string>^ stringArray;
#define PPhysicSystem PhysicSystem^
#else
//include使わないための必死の策
#define Vector std::vector
#define Points float*
#define PPhysicSystem PhysicSystem*
#define String const char*
#define stringArray Vector<std::string>
#define gcnew new
//typedef const Points CPoints;
//Obstacle,rendererにもある、スクリプト対処か
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

