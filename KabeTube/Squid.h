#pragma once
#include "Obstacle.h"
class Squid
  //:public Enemy
{
  void Motion();
public:
  Squid();
  Body Tentacles();
  //body���12�� size 1,6 ���Ex4.1 �p�x5.7
  void Update();
  ~Squid();
};

