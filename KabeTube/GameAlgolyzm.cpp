#include "stdafx.h"
#include "GameAlgolyzm.h"

float toRad(float ang){return ang/180*3.141592;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}
GameAlgolyzm::GameAlgolyzm(void)
{
}


GameAlgolyzm::~GameAlgolyzm(void)
{
}
