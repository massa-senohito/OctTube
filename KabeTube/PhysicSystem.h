#pragma once
void splitTest();
void step();//外からステップできるように泣く泣く
void makeParticle(float,float);
void makeSinglePar(float x,float y,float vx,float vy);


#include "Obstacle.h"
#include <vector>
#ifdef _MANAGED
#define List System::Collections::Generic::List
typedef List<Obstacle^>^ Obss;
typedef List<Enemy^>^ Enes;
#define Obs List<Obstacle^>
#define Ens List<Enemy^>
#define PEnemy Enemy^
#else
#include "DotNetWrap.h"
#define Vector DotnetList
typedef Obstacle* PObstacle;
typedef Enemy* PEnemy;
typedef Vector<Obstacle*> Obss;
typedef Vector<Enemy*> Enes;
typedef Vector<Obstacle*>* PObss;
typedef Vector<Enemy*>* PEnes;
#define gcnew new
#endif
ref class PhysicSystem
#ifdef _MANAGED
: IDisposable
#endif
{
  PObss obs;
  PEnes ens;
  void makeOuterFence(int,int);
  void jointDraw();
public:
  PhysicSystem(void);
  ~PhysicSystem();
  int MakeParticle(float,float);
  void Step();
  void addFence(float,float,float,float);
  PEnemy addEnemy(float,float,float rad);
  bool AllEnemyFired();
  void delFences();
};

