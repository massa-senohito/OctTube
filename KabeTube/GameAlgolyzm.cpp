#include "stdafx.h"
#include "GameAlgolyzm.h"
#include "PhysicSystem.h"
#include "Renderer.h"
#include "Enemy.h"
uint svgVLength;
Renderer* rend;

uint getSvgVLen(){
  return svgVLength;
}
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
PPhysicSystem sys;//managedなので外に置けない
#endif

float toRad(float ang){return ang/180*3.141592f;}
float toDeg(float deg){return deg*180/3.141592f;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}
namespace Assets{
  Points squid;
  uint*  squidElem;//テンプレートメタプログラミングで最適化が望めるが、ここではやらんでおく
  int    squidLen;

  Points getSquidPoints(){
    return ( squid);
  }
  uint* getSquidElem(){
    return squidElem;
  }
  int squidPLen(){
    return squidLen;
  }
  Points wall;
  uint*  wallElem;
  int    wallLen;

  Points getWallP(){
    return ( wall);
  }
  uint* getWallElem(){
    return wallElem;
  }
  int wallPLen(){
    return wallLen;
  }

  //後ほどクラスに移動する
  Points tako;
  uint*  takoElem;
  int    takoLen;
  Points getTakoPoints(){
    return tako;
  }
  uint* getTakoElem(){
    return takoElem;
  }
  int getTakoLen(){
    return takoLen;
  }
}
/*
let tt t=
thre * (one-t)* (one-t)* t
let t3 t=
thre*t*t*(one-t)
let ttt t=t*t*t
let bejx xx xxx t=
(tt t) * xx * (t3 t) * xxx+ttt(t)
*/
const float onef = 1.0f;
float bej(float t){
  return 3.0f * (onef - t)*(onef - t)*t *
    onef * 3.0f * t * t *(onef - t)  * onef + t*t*t;
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
void makeDragon(PPhysicSystem sys,stringArray coes ){
  //縦に動く,x10y12に初期値から加速度20で移動,（こげやすさ
  auto name = coes[0];
  auto pat  = coes[1];
  auto x    = strTofloat( coes[2]);
  auto y    = strTofloat( coes[3]);
  if(pat=="vmove"){
    
  }
  //-16,11でとばす
  auto enemy = sys->addEnemy(-32,0,1);
  
  enemy->Impulse(V2(6,11));
  auto ps = Assets::getSquidPoints();
  enemy->SetAssets(Squid); //
  //enemy->SetPoints(ps,Assets::squidPLen());
}
//#define Path SvgFs::Loader::Path 

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
  auto e = fopen_s(&p, name, "rb");
  if (e == 0){
    int flamenum = 0;
    svgVLength= 0;
    int count = 0;
    size_t t = 1;
    fread(&flamenum, sizeof(int), (size_t)1, p);
    fread(&svgVLength, sizeof(int), (size_t)1, p);
    //freadするために1次元
    auto tmp = new float[(flamenum*svgVLength)];
    //auto tes = new float[flamenum][442];
    while (t != 0){
      tmp; //->push(new float[svgVLength]);
      t = fread(
        &tmp[count*svgVLength]
        //tes[count]
        , sizeof(float), (size_t)svgVLength, p);
      ++count;
      //t=0;
    }
  fclose(p);
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
     auto x = strTofloat(coefar[1]);
     auto y = strTofloat(coefar[2]);
     auto z = strTofloat(coefar[3]);
     auto sx= strTofloat(coefar[4]);
     auto sy= strTofloat(coefar[5]);
     auto sz= strTofloat(coefar[6]);
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
float angle=float(90);
#define pos 0.5f,0.4f
float posx = 29; float posy = -18.5f;
//float scalex = 0, scaley = -98.5;
//timeは17ミリ秒ごとにカウント
//void drawFish(
void onRenderFrame(int time){
  int movableF = 2;
  float power  = 18;
  //if(c%50==0){makeParticle(pos);}
  //stepできるようになったので
  sys->Step();

  //クリップ範囲から離れてしまう
  glTranslatef( posx + 30 , posy - 60 , 0.0f );
  glRotatef(180, 1, 0, 0);
  glBegin(GL_LINES);

  static bool state = false;
  const int animLen = 38;
  if (time % animLen == 0) state = !state;
  int anim = state ? time % animLen : animLen - time % animLen;
  renderVertice( Assets::tako , Assets::takoLen , anim/2 );
  glEnd();
  //たこの触手をキーで動かせるが、足はベジエカーブを移動させればそれなりの見栄えになるみたい
  //drawClothHair(angle);
  glLoadIdentity();
  //std::cout << angle << std::endl;
  if(Key::isAPushed()){//フレームじゃなくてタイマー
    if(time-lastFiredFrame>10){
      float vx=cos(toRad(angle));
      float vy=sin(toRad(angle));
      makeSinglePar
        ( posx , posy , vx*power , vy*power );
      lastFiredFrame=time;
    }
  }
  //auto s = 0.5f;
  if(Key::isLeftPushed()){
    if(time%movableF==0){
      angle+=0.5f;
      //posx -= 0.5f;
    }
  }
  if(Key::isRightPushed()){
    if(time%movableF==0){
      angle-=0.5f;
      //posx += 0.5f;
    }
  }
  if(Key::isUpPushed()){
    if(time%movableF==0){
      posy += 0.5f;
    }
  }
  if(Key::isDownPushed()){
    if(time%movableF==0){
      posy -= 0.5f;
    }
  }
  glScalef(0.17f, 0.2f,1.f);
  glRotatef(180, 1, 0, 0);
  glTranslatef(-263.5f,-98.5f, 0);
  glBegin(GL_LINES);
  renderVertice(Assets::wall, Assets::wallLen,0);
  glEnd();
  glLoadIdentity();
}
GameAlgolyzm::GameAlgolyzm(stringArray args)
{
  auto path    = args[0];
  auto lastBel = path.find_last_of('\\');
  path         = path.substr(0,lastBel+1);
  Path::setPass(path);
  //Assets::squid     = svgRead((path+"\\allFlameOld").data());
  //Assets::squidLen  = siz;
  //Assets::squidElem = gcnew uint[siz];
  //for (size_t i = 0; i < siz; i++)
  //{
  //  Assets::squidElem[i] = i;
  //}
  Assets::tako     = svgRead((path+"\\takoallFlame").data());
  Assets::takoLen  = svgVLength;
  Assets::takoElem = gcnew uint[svgVLength];
  for (size_t i = 0; i < svgVLength; i++)
  {
    Assets::takoElem[i] = i;
  }
  Assets::wall = svgRead((path + "\\wallallflame").data());
  Assets::wallLen = svgVLength;

  int len = 0; //(args->Length);
  //glewInit呼ばないといけないので
  GLUT_INITs(len ,nullptr);
  sys = gcnew PhysicSystem();
  sys->MakeParticle(pos);
  //char ** arg= strMap(args);//opentkだと？
  ///drawable,steppable作るべき
  auto csv = path+"\\coe.csv";
  std::cout << csv<< std::endl;;
  fileRead(csv.data(),sys);

  rend=new Renderer([](int count){
    onRenderFrame(count);
  }
  , [&](){delete this; }
  );
  //char* c=toCp(ar[0]);
  //auto g=new GUIUtil();
  //splitTest();
  //Trueになる
  //Console::WriteLine(arg[0]);
  //std::cout << (arg[0])<< std::endl;;
  rend->renderPolygon(nullptr,0);
  glutMainLoop();
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
  SAFE_DELETE(rend);
  DA ( Assets::squid);
  DA ( Assets::squidElem);
  DA ( Assets::tako);
  DA ( Assets::takoElem);
  DA ( Assets::wall);
  DA ( Assets::wallElem);
  SAFE_DELETE( sys );
}
