#pragma once
#include "Shader.h"//pimplにできる
typedef unsigned int ALuint;
#define Points float*
enum EnemyKind{
  Squid,Dragon,Len,
};
namespace Path{
  void setPass(std::string);
}
//インスタンス作るより、AnimAssets[Squid]で取り出せるほうが
class AnimAsset{
  Points vertice;
  uint* elem;
  int vertLen;
  int count;
  Shader* shader;
public:
  void UpdateAnim(bool);
  AnimAsset(EnemyKind); //

  void SetPoints(Points,int);
  void DamageColor(float HP);
  void NoUse();
  ~AnimAsset();
};

class SoundAsset
{
public:
  SoundAsset(EnemyKind);
  ~SoundAsset();
  void PlayDamageSound();
  void PlayMovingSound();
private:
  ALuint* sources;
  ALuint* bufs;
};

