#pragma once
#include "Obstacle.h"
class Squid
  //:public Enemy
{
  void Motion();
public:
  Squid();
  Body Tentacles();
  //bodyÇÊÇË12â∫ size 1,6 ç∂âEx4.1 äpìx5.7
  void Update();
  ~Squid();
};

