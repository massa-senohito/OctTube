#include "stdafx.h"
#include "GameAlgolyzm.h"
#include "AL\al.h"
#include "AL\alc.h"
#include "Enemy.h"
std::string& path = std::string(""); 
void Path::setPass(std::string& p){
  path = p;
}
void AnimAsset::UpdateAnim(int flame){
  //if ( update ) ++count;
  shader->Render(flame);
}
AnimAsset::AnimAsset(EnemyKind ek){
  //if (ek == Squid){ "squid"; }
  auto vs = path + "\\squid.vs";
  auto fs = path + "\\squid.fs";
  shader = (new Shader(vs.data(), fs.data()));
  vertice = svgRead((path + "\\allFlame").data());
  vertLen = getSvgVLen();
  elem = new uint[vertLen];
  for (size_t i = 0; i < vertLen; i++)
  {
    elem[i] = i;
  }
  shader->SendVert(vertice, vertLen, elem);
  //ステンシルテストで落ちたらシェーダーの影響受けないはず
  //頂点シェーダはかかるはずなので、x/2
}
void AnimAsset::DamageColor(float HP){
  shader->Use(true);
  shader->Uniform("Koge", HP/100.f);
}
void AnimAsset::NoUse(){
  shader->Use(false);
}
AnimAsset::~AnimAsset(){
  SAFE_DELETE( shader);
}

//http://www-fps.nifs.ac.jp/ito/memo/openal02.html
int ReadHeaderWav(FILE* fp, int *channel, int* bit, int *size, int* freq){
  short res16;
  int   res32;
  int   dataSize, chunkSize;
  short channelCnt, bitParSample, blockSize;
  int   samplingRate, byteParSec;

  int   dataPos;
  int   flag = 0;

  fread( &res32 , 4 , 1 , fp );
  if ( res32 != 0x46464952){	//"RIFF"
    return 1;	//error 1
  }

  //データサイズ = ファイルサイズ - 8 byte の取得
  fread( &dataSize , 4 , 1 , fp );

  //WAVEヘッダーの読み
  fread( &res32    , 4 , 1 , fp );
  if (res32 != 0x45564157){	//"WAVE"
    return 2;	//error 2
  }

  while (flag != 3){
    //チャンクの読み
    fread(&res32, 4, 1, fp);
    fread(&chunkSize, 4, 1, fp);

    switch (res32){
    case 0x20746d66:	//"fmt "
      //format 読み込み
      //PCM種類の取得
      fread(&res16, 2, 1, fp);
      if (res16 != 1){
        //非対応フォーマット
        return 4;
      }
      //モノラル(1)orステレオ(2)
      fread(&channelCnt, 2, 1, fp);
      if (res16 > 2){
        //チャンネル数間違い
        return 5;
      }
      //サンプリングレート
      fread(&samplingRate, 4, 1, fp);
      //データ速度(byte/sec)=サンプリングレート*ブロックサイズ
      fread(  &byteParSec, 4, 1, fp);
      //ブロックサイズ(byte/sample)=チャンネル数*サンプルあたりのバイト数
      fread(   &blockSize, 2, 1, fp);
      //サンプルあたりのbit数(bit/sample)：8 or 16
      fread(&bitParSample, 2, 1, fp);

      *channel = (int)channelCnt;
      *bit     = (int)bitParSample;
      *freq    = samplingRate;

      flag += 1;

      break;
    case  0x61746164:	//"data"

      *size = chunkSize;
      //現在のpos
      dataPos = ftell(fp);

      flag += 2;
      break;
    }

  }

  //頭出し("fmt "が"data"より後にあった場合のみ動く)
  if (dataPos != ftell(fp)){
    fseek(fp, dataPos, SEEK_SET);
  }

  return 0;
}
void openAndClose(std::string name,ALuint buffer){
  FILE* ip;
  auto er=fopen_s(&ip,name.data() , "rb");
  int freq, channel, size, bit;
  //最初にファイル数置けばバイナリにまとめられる
  ReadHeaderWav(ip,&channel,&bit,&size,&freq);

  auto data = new unsigned char[size];
  fread(data, size, 1, ip);
  if (channel == 1){
    if (bit == 8){
      alBufferData(buffer, AL_FORMAT_MONO8, data, size, freq);
    }
    else{
      alBufferData(buffer, AL_FORMAT_MONO16, data, size, freq);
    }
  }
  else{
    if (bit == 8){
      alBufferData(buffer, AL_FORMAT_STEREO8, data, size, freq);
    }
    else{
      alBufferData(buffer, AL_FORMAT_STEREO16, data, size, freq);
    }
  }
  fclose(ip);
}
bool isInit = false;
void initialize(){

  auto dev=alcOpenDevice(nullptr);
  _ASSERT(dev);
  auto cont=alcCreateContext(dev,nullptr);
  alcMakeContextCurrent(cont);
  isInit = true;
}

SoundAsset::SoundAsset(EnemyKind)
{
  if (!isInit)initialize();
  //ALuint* squidSources;
  bufs = new ALuint[2];
  alGenBuffers(2, bufs);
  openAndClose(path + std::string("\\ido2.wav") , bufs[0]);
  openAndClose(path + std::string("\\yake.wav") , bufs[1]);
  sources = new ALuint[2];
  alGenSources(2, sources);
  alSourcei(sources[0], AL_BUFFER, bufs[0]);
  alSourcei(sources[1], AL_BUFFER, bufs[1]);
}
void SoundAsset::PlayDamageSound(){
  alSourcePlay(sources[1]);
}
void SoundAsset::PlayMovingSound(){
  alSourcePlay(sources[0]);
}
SoundAsset::~SoundAsset()
{
  alDeleteSources(2, sources);
  alDeleteBuffers(2, bufs);
  SAFE_DELETE_ARRAY( bufs);
}
