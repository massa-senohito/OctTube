#pragma once
void splitTest();
void step();//外からステップできるように泣く泣く
void makeParticle(float,float);
void makeSinglePar(float x,float y,float vx,float vy);

#include "Obstable.h"
#define List System::Collections::Generic::List
typedef List<Obstable^>^ Obss;
#define Obs List<Obstable^>
ref class PhysicSystem:System::IDisposable
{
  Obss obs;
public:
  PhysicSystem(void);
  ~PhysicSystem();
  int MakeParticle(float,float);
void Step();
  void addFence(float,float,float,float);
  void delFences();
};

