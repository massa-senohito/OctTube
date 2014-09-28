#pragma once
typedef unsigned int ALuint;
#include "Shader.h"//pimpl‚É‚Å‚«‚é
enum EnemyKind{
  Squid,Dragon
};

class AnimAsset{
  //Points vertice;
  //uint* elem;
  int vertLen;
  int count;
  Shader* shader;
public:
  void UpdateAnim(bool);
  AnimAsset(EnemyKind);
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

