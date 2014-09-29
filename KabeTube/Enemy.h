#pragma once
#include "Shader.h"//pimpl�ɂł���
typedef unsigned int ALuint;
#define Points float*
enum EnemyKind{
  Squid,Dragon,Len,
};
namespace Path{
  void setPass(std::string);
}
//�C���X�^���X�����AAnimAssets[Squid]�Ŏ��o����ق���
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

