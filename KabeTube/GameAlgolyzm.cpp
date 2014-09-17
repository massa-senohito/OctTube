#include "stdafx.h"
#include "GameAlgolyzm.h"
#include "PhysicSystem.h"
#include "Renderer.h"
//#include <sstream>
#ifndef _MANAGED
namespace File{
  stringArray* ReadAllLines(String name){
		FILE* p;
    const int maxCharInLine = 256;
    auto e=fopen_s(&p, name, "r");
    if (e == 0){
      char* line = new char[maxCharInLine];
      auto lines = new stringArray;
      while (fgets(line, maxCharInLine, p))
      {
        lines->push_back(line);
      }
      fclose(p);
      return lines;
    }
    else if (e==ENOENT){
      std::cout << "ファイル:"<<name << " がありません" << std::endl;
      return nullptr;
    }
  }
}
namespace Convert{
	float ToSingle(String str){
    return atof(str);
	}
}
#endif
float toRad(float ang){return ang/180*3.141592f;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}
namespace Assets{
  Points squid;
  int squidLen;
  Points getSquidPoints(){
    return ( squid);
  }
  int squidPLen(){
    return squidLen;
  }
}
//typedef std::basic_istringstream<char> istringstream;
//using vector<T>=std::vector<T>;
//パースしたデータはランダムアクセスで拾うのでvectorでいいや
//http://shnya.jp/blog/?p=195
std::vector<std::string> Split(std::string str, char delim){
  std::istringstream iss(str); std::string tmp; std::vector<std::string> res;
  while(getline(iss, tmp, delim)) res.push_back(tmp);
  return res;
}
float strTofloat(std::string str){
    return Convert::ToSingle(str.c_str());
}
int siz = 0;
void makeDragon(PPhysicSystem sys,stringArray coes ){
  //縦に動く,x10y12に初期値から加速度20で移動,（こげやすさ
  auto name=coes[0];
  auto pat=coes[1];
  float x=strTofloat( coes[2]);
  float y=strTofloat( coes[3]);
  if(pat=="vmove"){
    
  }
  //-16,11でとばす
  auto enemy=sys->addEnemy(-32,0,1);
  
  enemy->Impulse(V2(6,11));
  auto ps = Assets::getSquidPoints();
  enemy->SetPoints(ps,Assets::squidPLen());
}
#define Path SvgFs::Loader::Path 
//using namespace SvgFs;

Points svgRead(String name){
  /*  M始点 C 点 前の点　コントロールポイント 次の点
  M 93.00,37.00
  C 93.00,37.00 80.00,65.00 59.00,72.00
  59.00,72.00 84.00,109.00 71.00,125.00
  71.00,125.00 87.00,152.00 114.00,122.00
  102.00,97.00 130.00,74.00 130.00,74.00
  112.00,71.00 93.00,37.00 93.00,37.00 Z
  M 79.00,143.00
  C 79.00,143.00 66.00,175.00 66.00,175.00M 110.00,141.00
  C 110.00,141.00 118.00,179.00 118.00,179.00M 94.00,146.00
  C 94.00,146.00 94.00,181.00 94.00,181.00
  */
  //  auto fi=Loader::openGimpPath("squid.svg");
  //  auto anim=Loader::anim(fi,0);//たしかにfs側でbyte[][]吐き出せば静的に焼きこんだアニメーション作れる
  //  auto ps=Loader::pathToPointsLis(anim,0.2f);
#ifdef _MANAGED
  auto ps =
    gcnew array<float>(0);
#else
  FILE* p;
  auto e = fopen_s(&p, name, "r");
  if (e == 0){
    int flamenum = 0;
    siz = 0;
    int count = 0;
    size_t t = 1;
    fread(&flamenum, sizeof(int), (size_t)1, p);
    fread(&siz, sizeof(int), (size_t)1, p);
    //freadするために1次元
    auto tmp = new float[(flamenum*siz)];
    while (t != 0){
      tmp; //->push(new float[siz]);
      t = fread(&tmp[count*siz], sizeof(float), (size_t)siz, p);
      ++count;
      //t=0;
    }
  //fclose(p);
  return tmp;
  }

#endif

  int c=1;//ps->Length;
  for (int i = 0; i < c; i++)
  {
    //ps[i]->
  }
  return nullptr;
}
void fileRead(String filename,PPhysicSystem sys){
  auto fi=File::ReadAllLines(filename);
#ifdef _MANAGED
  int c=fi->Length;
#else
  int c = fi->size();
#endif
  for (int i = 0; i < c; i++)
  {
#ifdef _MANAGED
   auto coefar=fi[i]->Split(',');
#else
    auto coefar = Split(fi->at(i), ',');
#endif
   std::string name=coefar[0]; 
   if(name=="Cube"){
     float x = strTofloat(coefar[1]);
     auto  y = strTofloat(coefar[2]);
     auto  z = strTofloat(coefar[3]);
     auto  sx= strTofloat(coefar[4]);
     auto  sy= strTofloat(coefar[5]);
     auto  sz= strTofloat(coefar[6]);
     //fence(x,y,z,sx,sy,sz,0.0f);
     sys->addFence(x,y,sx,sy);
   }
   if(name=="dragonbody"){
     makeDragon(sys,coefar);
   }
   if(name=="fish"){
   }
  }
}

///最後に撃った時のフレーム
int lastFiredFrame=int(0);
float angle=float(270);
#define pos 0.5f,0.4f
//timeは17ミリ秒ごとにカウント
//void drawFish(

void onRenderFrame(int time){
  int movableF=2;
  float power=18;
  //if(c%50==0){makeParticle(pos);}
  step();

  drawClothHair(angle);
  //std::cout << angle << std::endl;
  if(Key::isAPushed()){//フレームじゃなくてタイマー
    if(time-lastFiredFrame>10){
      float vx=cos(toRad(angle));
      float vy=sin(toRad(angle));
      makeSinglePar(pos,vx*power,vy*power);
      lastFiredFrame=time;
    }
  }
  if(Key::isLeftPushed()){
    if(time%movableF==0){
      angle-=0.5f;
    }
  }
  if(Key::isRightPushed()){
    if(time%movableF==0){
      angle+=0.5f;
    }
  }
}
GameAlgolyzm::GameAlgolyzm(stringArray args)
{
    auto squid = "C:\\Users\\massa_senohito\\Documents\\Visual Studio 11\\Projects\\SvgFs\\SvgTest\\bin\\Debug\\squid3allFlame";
    Assets::squid= svgRead(squid);
    Assets::squidLen = siz;
    auto sys=gcnew PhysicSystem();
    sys->MakeParticle(pos);
    //char ** arg= strMap(args);//opentkだと？
    int len = 0; //(args->Length);
    GLUT_INITs(len ,nullptr);
    ///drawable,steppable作るべき
    auto csv = "coe.csv";
#ifdef _DEBUG
    csv="C:\\Users\\massa_senohito\\Documents\\Visual Studio 2013\\Projects\\OctTube\\Debug\\coe.csv";
#endif
    std::cout << csv<< std::endl;;
    fileRead(csv,sys);

    Renderer rend([](int count){
      onRenderFrame(count);
    }
    );
    //char* c=toCp(ar[0]);
    //auto g=new GUIUtil();
    //splitTest();
    //Trueになる
    //Console::WriteLine(arg[0]);
    //std::cout << (arg[0])<< std::endl;;
    rend.renderPolygon(nullptr,0);
    glutMainLoop();
}
bool 
  isAllMeatFired ()//ms=
  {
    int length=0;//
    
    for (int i = 0; i < length; i++)
    {
      return false;
    }
    return true;
}
///クリア氷菓
float howAllMeatFired()// ms
{
  return 1;
}
void GameAlgolyzm::
  Render()
{
  int length=0;//w.ParCount
  //auto pb=w.PartiPosBuf()
  for (int i = 0; i < length; i++)
  {
    //setMask(i%3*2+1

  }
}
GameAlgolyzm::~GameAlgolyzm(void)
{
}
