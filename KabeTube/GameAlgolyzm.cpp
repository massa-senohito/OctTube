#include "stdafx.h"
#include "GameAlgolyzm.h"

float toRad(float ang){return ang/180*3.141592f;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}
GameAlgolyzm::GameAlgolyzm(void)
{
}
bool 
  isAllMeatFired ()//ms=
  {
    int length=0;//
    
    for (int i = 0; i < length; i++)
    {
      return false;
    }
    return true;
}
///ƒNƒŠƒA•X‰Ù
float howAllMeatFired()// ms
{
  return 1;
}
void GameAlgolyzm::
  Render()
{
  int length=0;//w.ParCount
  //auto pb=w.PartiPosBuf()
  for (int i = 0; i < length; i++)
  {
    //setMask(i%3*2+1

  }
}
GameAlgolyzm::~GameAlgolyzm(void)
{
}
