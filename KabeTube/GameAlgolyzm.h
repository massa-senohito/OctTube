#pragma once
void drawClothHair(float);
float toRad(float);
//http://manski.net/2011/04/cpp-cli-cheat-sheet/
//CLI���ǂ����m��ɂ́A���ɂ��`�[�g�V�[�g��
#ifdef _MANAGED
using namespace System;
using namespace System::IO;
typedef array<float>^ Points;
typedef System::String^ string;
typedef array<string>^ stringArray;
#define PPhysicSystem PhysicSystem^
#else
#define Vector std::vector
#define Points float*
#define PPhysicSystem PhysicSystem*
#define String const char*
#define stringArray Vector<std::string>
#define gcnew new
//typedef const Points CPoints;
//Obstacle,renderer�ɂ�����A�X�N���v�g�Ώ���
#define ref /**/
#endif
namespace Assets{
  Points getSquidPoints();
  int squidPLen();
}
Points svgRead(String);
class GameAlgolyzm
{
public:
  GameAlgolyzm(stringArray arg);
  void Render();
  ~GameAlgolyzm(void);
};

