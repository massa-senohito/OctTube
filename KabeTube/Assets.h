#pragma once
#include "Shader.h"//pimplにできる
typedef unsigned int ALuint;

//typedefが無効
#define Points float*
//typedef float* Points;
enum EnemyKind{
  Squid,Dragon,Len,
};
namespace Path{
  void setPass(std::string&);
  std::string& getPath();
}
//インスタンス作るより、AnimAssets[Squid]で取り出せるほうが
class AnimAsset{
  Points vertice;
  uint* elem;
  int vertLen;
  int count;
  Shader* shader;
public:
  void UpdateAnim(int);
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
  void SetAsset(EnemyKind);
  void PlayDamageSound();
  void PlayMovingSound();
private:
  ALuint* sources;
  ALuint* bufs;
  typedef std::map<std::string*, ALuint*> LoadedMap;
  LoadedMap& loaded;
};

