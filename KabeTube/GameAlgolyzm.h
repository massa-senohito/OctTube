#pragma once
void drawClothHair(float);
float toRad(float);
using namespace System;
using namespace System::IO;
typedef array<float>^ Points;

Points svgRead();
class GameAlgolyzm
{
public:
  GameAlgolyzm(array<System::String ^> ^arg);
  void Render();
  ~GameAlgolyzm(void);
};

