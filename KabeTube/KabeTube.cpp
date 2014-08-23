// KabeTube.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "PhysicSystem.h"
#include "Renderer.h"
#include "GameAlgolyzm.h"
//#include "Box2D/Box2D.h"
using namespace System;
using namespace System::IO;
//using namespace System::Linq;
//createRice //粘着する
//createLiquid //水
//csvで係数指定


//とりあえずの設計
//class Particle{
//  shape
//};
//class StreamParticle:Particle{
//  shape=Box
//};
//class KomeParticle:Particle{
//  shape=poly //半円形
//};
//class Baloon{
//  Baloon(float32 radius,float32 pradius,float32 x,float32 y)
//  
//};

//void update(){  
//  switch(moveflag)
//    case VMove:
//      updatePos(start,end)

//
//Baloon::Baloon(
//fence->Rotate(barAxis)
//
//auto kome=new KomeParticle();
////kome->Appearか
////part->Appear() 多態にできるし、下のも簡単にできる
////system->AppearKomeか
////system->komeCoffi->moti
//
////objを指定したらパスとobjをVSにとうろくするよーな
//<AdditionalDependencies>solid.lib;liquidfun.lib;
//<AdditionalLibraryDirectories>C:\Users\massa_senohito\Downloads\solid-3.5.7\VisualC6\solid\Debug;C:\Users\massa_senohito\Documents\Visual Studio 11\Projects\KabeTube\Debug;%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>
//let bxml s= "<" + s + ">"
//let cxml s= "<" + s + "/>"
//
//let ad=bxml "AdditionalDependencies"
//let cd=cxml "AdditionalDependencies"
//let al=bxml "AdditionalLibraryDirectories"
//let cl=cxml "AdditionalLibraryDirectories"
//let buildPath ss=
//  ss|>Seq.map (fun (d,l)-> d + ";" 
//

//invalid
//using Ast=array<String^>^;
char* toCp(String^ i){
    char* tmp=new char[32];
    sprintf(tmp,"%s",i);
  return tmp;
}//あまどいのなかをぬけていく
char** strMap(array<String^> ^item){
    int len=item->Length; //マクロでもムリだわ
    char** tmp=new char*[3];
    
    tmp[0]=new char[32];
    tmp[1]=new char[32];
    tmp[2]=new char[32];
    
    ;//(char*)malloc(len*sizeof(char*)*32);
    for (int i = 0; i < len; i++)
    {
        //tmp[i]=item[i]->ToCharArray()->;
        sprintf(tmp[i],"%s",item[i]);
	//tmp[i]=
    }
    return tmp;
}
#define FenceArg float x,float y,float z,float sx,float sy,float sz,float res
void fence(FenceArg){
    
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

}
void fileRead(System::String^ filename,PhysicSystem^ sys){
  auto fi=File::ReadAllLines(filename);
  int c=fi->Length;
  for (int i = 0; i < c; i++)
  {
   auto coefar=fi[i]->Split(',');
   String^ name=coefar[0]; 
   if(name=="Cube"){
    float x=strTofloat(coefar[1]);
    auto y=strTofloat(coefar[2]);
    auto z=strTofloat(coefar[3]);
    auto sx=strTofloat(coefar[4]);
    auto sy=strTofloat(coefar[5]);
    auto sz=strTofloat(coefar[6]);
	//fence(x,y,z,sx,sy,sz,0.0f);
    sys->addFence(x,y,sx,sy);
   }
   else if(name=="dragonbody"){
	   makeDragon(sys,coefar);
   }
  }
}
#define TEXSIZE 64
GLuint texName;

void disp( void ) {
	glClear(GL_COLOR_BUFFER_BIT);
float nine =0.9;
	glBegin(GL_QUADS);
		glColor3ub(0xFF , 0 , 0);
		glVertex2f(-nine , -nine);
		glColor3f(0 , 0 , 1);
		glVertex2f(-nine , nine);
		glVertex2f(nine , nine);
		glColor3b(127 , 0 , 0);
		glVertex2f(nine , -nine);
	glEnd();
    glFlush();
}

void timert(int value) {
	//glRotatef(1 , 0.5 , 1 , 0.25);
	glutPostRedisplay();
	glutTimerFunc(50 , timert , 0);
}
int texTest(int argc , char ** argv){
  GLubyte bits[TEXSIZE][TEXSIZE][3];
      unsigned int i , j;

  for (i = 0 ; i < TEXSIZE ; i++) {
    int r = (i * 0xFF) / TEXSIZE;
    for (j = 0 ; j < TEXSIZE ; j++) {
      bits[i][j][0] = (GLubyte)r;
      bits[i][j][1] = (GLubyte)(( j * 0xFF ) / TEXSIZE);
      bits[i][j][2] = (GLubyte)~r;
    }
  }

  glutInit(&argc , argv);
  glutInitWindowSize(400 , 300);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  
  glutCreateWindow("Kitty on your lap");
  glutDisplayFunc(disp);
  glutTimerFunc(100 , timert , 0);

  glEnable(GL_TEXTURE_2D);
  glGenTextures(1 , &texName);
  glBindTexture(GL_TEXTURE_2D , texName);

  glTexImage2D(
    GL_TEXTURE_2D , 0 , 3 , TEXSIZE , TEXSIZE ,
    0 , GL_RGB , GL_UNSIGNED_BYTE , bits
  );

  glutMainLoop();
  return 0;
}

///最後に撃った時のフレーム
int lastFiredFrame=int(0);
float angle=float(270);
#define pos 0.5,0.4
//timeは17ミリ秒ごとにカウント
void onRenderFrame(int time){
  int movableF=2;
  float power=18;
  //if(c%50==0){makeParticle(pos);}
  step();

  drawClothHair(angle);
  std::cout << angle << std::endl;
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
void kabeMain(array<System::String ^> ^args){
    array<String^> ^ar={"av","as",""};
    auto sys=gcnew PhysicSystem();
    sys->MakeParticle(pos);
    char ** arg= strMap(args);//opentkだと？
    GLUT_INITs((args->Length) ,arg);
    ///drawable,steppable作るべき
    fileRead("coe.csv",sys);

    Renderer rend(onRenderFrame
    );
    //char* c=toCp(ar[0]);
    //auto g=new GUIUtil();
    //splitTest();
    Console::WriteLine(arg[0]);
    std::cout << (arg[0])<< std::endl;;
    rend.renderPolygon(nullptr,0);
    Console::WriteLine(arg[2]);
    glutMainLoop();
    delete[] arg;
    Console::Read();
}
int main(array<System::String ^> ^args)
{
    //auto i=args[0];
    //auto c=args->Count;
    //texTest(0,nullptr);
    kabeMain(args);
    return 0;

}
