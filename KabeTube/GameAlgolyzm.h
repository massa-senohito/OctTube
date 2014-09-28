#pragma once
void drawClothHair(float);
float toRad(float);
float toDeg(float);
//stepを外に放り出しても解決にはならない,静的保存期間があるので
//#include "PhysicSystem.h"
//http://manski.net/2011/04/cpp-cli-cheat-sheet/
//CLIかどうか知るには、他にもチートシートが
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
typedef unsigned int uint;
namespace Assets{
  Points getSquidPoints();
  int squidPLen();
  uint* getSquidElem();
}
Points svgRead(String);
float bej(float t);
class GameAlgolyzm
{
public:
  GameAlgolyzm(stringArray arg);
  void Render();
  ~GameAlgolyzm(void);
};

