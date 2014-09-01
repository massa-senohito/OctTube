#pragma once
void splitTest();
void step();//外からステップできるように泣く泣く
void makeParticle(float,float);
void makeSinglePar(float x,float y,float vx,float vy);


#include "Obstacle.h"
#define List System::Collections::Generic::List
typedef List<Obstacle^>^ Obss;
typedef List<Enemy^>^ Enes;
#define Obs List<Obstacle^>
#define Ens List<Enemy^>
ref class PhysicSystem:System::IDisposable
{
  Obss obs;
  Enes ens;
  void makeOuterFence(int,int);
public:
  PhysicSystem(void);
  ~PhysicSystem();
  int MakeParticle(float,float);
  void Step();
  void addFence(float,float,float,float);
  Enemy^ addEnemy(float,float,float rad);
  void delFences();
};

