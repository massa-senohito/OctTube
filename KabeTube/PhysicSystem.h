#pragma once
void splitTest();
void step();//外からステップできるように泣く泣く
void makeParticle(float,float);
void makeSinglePar(float x,float y,float vx,float vy);

#include "Obstacle.h"
#include "DotNetWrap.h"

typedef float32 f32;
#define ref /**/
#define gcnew new
ref class PhysicSystem
#ifdef _MANAGED
: IDisposable
#endif
{

  void makeOuterFence(f32,f32);
  void jointDraw();
public:
  int GetStopping();
  World GetWorld();
  PhysicSystem(void);
  ~PhysicSystem();
  int MakeParticle(float,float);
  void Step();
  void addFence(float,float,float,float);
  void delFences();
};

