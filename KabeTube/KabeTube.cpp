// KabeTube.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "stdafx.h"
#include "PhysicSystem.h"
#include "Renderer.h"
#include "GameAlgolyzm.h"
//#include "Box2D/Box2D.h"
using namespace System;
using namespace System::IO;
//using namespace System::Linq;
//createRice //�S������
//createLiquid //��
//csv�ŌW���w��
Points svgRead();

//�Ƃ肠�����̐݌v
//class Particle{
//  shape
//};
//class StreamParticle:Particle{
//  shape=Box
//};
//class KomeParticle:Particle{
//  shape=poly //���~�`
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
////kome->Appear��
////part->Appear() ���Ԃɂł��邵�A���̂��ȒP�ɂł���
////system->AppearKome��
////system->komeCoffi->moti
//
////obj���w�肵����p�X��obj��VS�ɂƂ��낭�����[��
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
    //sprintf(tmp,"%s",i);
  return tmp;
}//���܂ǂ��̂Ȃ����ʂ��Ă���
char** strMap(array<String^> ^item){
    int len=item->Length; //�}�N���ł���������
    char** tmp=new char*[3];
    
    tmp[0]=new char[32];
    tmp[1]=new char[32];
    tmp[2]=new char[32];
    
    ;//(char*)malloc(len*sizeof(char*)*32);
    for (int i = 0; i < len; i++)
    {
        //sprintf(tmp[i],"%s",item[i]);
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
  //�c�ɓ���,x10y12�ɏ����l��������x20�ňړ�,�i�����₷��
  //dragonbody,vmove,10,12,20,
  auto name=coes[0];
  auto pat=coes[1];
  float x=strTofloat( coes[2]);
  float y=strTofloat( coes[3]);
  if(pat=="vmove"){
    
  }
  //-16,11�łƂ΂�
  auto enemy=sys->addEnemy(-32,0,1);;
  enemy->Impulse(V2(6,11));
  enemy->SetPoints(svgRead());
}
#define Path SvgFs::Loader::Path 
using namespace SvgFs;
Points svgRead(){
  /*  M�n�_ C �_ �O�̓_�@�R���g���[���|�C���g ���̓_
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
  auto fi=Loader::openGimpPath("squid.svg");
  auto anim=Loader::anim(fi,0);//��������fs����byte[][]�f���o���ΐÓI�ɏĂ����񂾃A�j���[�V��������
  auto ps=Loader::pathToPointsLis(anim,0.2f);
  int c=ps->Length;
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
#define TEXSIZE 64
GLuint texName;
GLint es[]={
  GL_LINES,
  GL_LINE_STRIP,
  GL_TRIANGLES,
  GL_TRIANGLE_STRIP,
  GL_TRIANGLE_FAN,
  GL_QUADS,
  GL_QUAD_STRIP,
  GL_POLYGON,
  -1
};
int ei=0;
void disp( void ) {
  glClear(GL_COLOR_BUFFER_BIT);
  static auto ps=svgRead();
  float nine =0.9f;
  glBegin(es[ei]);
  /*
   	glColor3ub(0xFF , 0 , 0);
   	glVertex2f(-nine , -nine);
   	glColor3f(0 , 0 , 1);
   	glVertex2f(-nine , nine);
   	glVertex2f(nine , nine);
	glColor3b(127 , 0 , 0);
        glVertex2f(nine , -nine);
  */
  float32 scale=1.0f/150.0f;
  glScalef(scale,scale,0);
  int pl=ps->Length;
  for (int i = 0; i < pl; i+=2)
  {
    glVertex2f( ps[i]-59,ps[i+1]-37);
  }  
  glEnd();
  glFlush();
}
void key(int key,int,int){
  ei++;
  if(es[ei]==-1)ei=0;
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
  glutSpecialFunc(key);
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

///�Ō�Ɍ��������̃t���[��
int lastFiredFrame=int(0);
float angle=float(270);
#define pos 0.5f,0.4f
//time��17�~���b���ƂɃJ�E���g
//void drawFish(

void onRenderFrame(int time){
  int movableF=2;
  float power=18;
  //if(c%50==0){makeParticle(pos);}
  step();

  drawClothHair(angle);
  //std::cout << angle << std::endl;
  if(Key::isAPushed()){//�t���[������Ȃ��ă^�C�}�[
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
    svgRead();
    auto sys=gcnew PhysicSystem();
    sys->MakeParticle(pos);
    char ** arg= strMap(args);//opentk���ƁH
    GLUT_INITs((args->Length) ,arg);
    ///drawable,steppable���ׂ�
    fileRead("coe.csv",sys);
    Renderer rend([](int count){
      onRenderFrame(count);
    }
    );
    //char* c=toCp(ar[0]);
    //auto g=new GUIUtil();
    //splitTest();
    //True�ɂȂ�
    //Console::WriteLine(arg[0]);
    std::cout << (arg[0])<< std::endl;;
    rend.renderPolygon(nullptr,0);
    glutMainLoop();
    delete[] arg;
    Console::Read();
}
int main(array<System::String ^> ^args)
{
    //auto i=args[0];
    //auto c=args->Count;
    texTest(0,nullptr);
    //kabeMain(args);
    return 0;

}
