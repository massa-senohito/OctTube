#include "stdafx.h"
#include "GameAlgolyzm.h"
//#include "
#include "PhysicSystem.h"
#include "Renderer.h"
float toRad(float ang){return ang/180*3.141592f;}
void drawClothHair(float angle){
  float radius=6;
  
  float vx=radius*cos(toRad(angle));
  float vy=radius*sin(toRad(angle));
  glRectf(vx-2,vy+2,vx+2,vy-2);
}

float strTofloat(String^ str){
    return Convert::ToSingle(str);
}
void makeDragon(PhysicSystem^ sys,array<String^>^ coes ){
  //縦に動く,x10y12に初期値から加速度20で移動,（こげやすさ
  //dragonbody,vmove,10,12,20,
  auto name=coes[0];
  auto pat=coes[1];
  float x=strTofloat( coes[2]);
  float y=strTofloat( coes[3]);
  if(pat=="vmove"){
    
  }
  //-16,11でとばす
  auto enemy=sys->addEnemy(-32,0,1);;
  enemy->Impulse(V2(6,11));
  enemy->SetPoints(svgRead());
}
#define Path SvgFs::Loader::Path 
//using namespace SvgFs;
Points svgRead(){
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
  auto ps=gcnew array<float>(0);
  int c=1;//ps->Length;
  for (int i = 0; i < c; i++)
  {
    //ps[i]->
  }
  return ps;
}
void fileRead(System::String^ filename,PhysicSystem^ sys){
  auto fi=File::ReadAllLines(filename);
  int c=fi->Length;
  for (int i = 0; i < c; i++)
  {
   auto coefar=fi[i]->Split(',');
   String^ name=coefar[0]; 
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
GameAlgolyzm::GameAlgolyzm(array<System::String ^> ^args)
{
    array<String^> ^ar={"av","as",""};
    svgRead();
    auto sys=gcnew PhysicSystem();
    sys->MakeParticle(pos);
    //char ** arg= strMap(args);//opentkだと？
    GLUT_INITs((args->Length) ,nullptr);
    ///drawable,steppable作るべき
    fileRead("coe.csv",sys);
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
    Console::Read();
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
