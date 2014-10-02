// KabeTube.cpp : メイン プロジェクト ファイルです。

#include "stdafx.h"
#include "GameAlgolyzm.h"
#include "Renderer.h"
//#include "Box2D/Box2D.h"
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

//copy $(MSBuildProjectDirectory)/../assets/ $(OutDir)
//copy $(MSBuildProjectDirectory) / .. / glew32.dll / $(OutDir)
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
char* toCp(String i){
    char* tmp=new char[32];
    //sprintf(tmp,"%s",i);
  return tmp;
}//あまどいのなかをぬけていく
char** strMap(stringArray item){
	int len = 0;
#ifdef _MANAGED
        len=item->Length; //マクロでもムリだわ
#endif
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
  static auto squid 
    = "C:\\Users\\massa_senohito\\Documents\\Visual Studio 11\\Projects\\SvgFs\\SvgTest\\bin\\Debug\\squid3allFlame";
  static auto ps=svgRead(squid);
  float nine =0.9f;
  float scale=1.0f/150.0f;
  /*
   	glColor3ub(0xFF , 0 , 0);
   	glVertex2f(-nine , -nine);
   	glColor3f(0 , 0 , 1);
   	glVertex2f(-nine , nine);
   	glVertex2f(nine , nine);
	glColor3b(127 , 0 , 0);
        glVertex2f(nine , -nine);
  */
  glScalef(scale,scale,0);
  glBegin(es[ei]);
 // renderVertice(ps,Assets::squidPLen());
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
GameAlgolyzm* ga;
void kabeMain(const char* args){
  auto ar = Vector<std::string>();
  ar.push_back(std::string( args ));
  ga = new GameAlgolyzm( ar );
}

void quitF(){
  delete ga;  //なぜかよばれない,delete2回入って異常終了可能性
}//glutExitFuncでブレーク追いかけよう
int main(int argc,char* argv[])
{
  try{
    atexit(quitF);
    //texTest(0,nullptr);
    kabeMain(argv[0]);
  }catch
#ifdef _MANAGED
    (System::IO::FileLoadException^ e){
    auto ex=e;
  }
  finally{
    
  }
#else
  (std::exception e){
  }
#endif
  return 0;
}

int _tmain(int argc, char* argv[])
{
  kabeMain("");
	return 0;
}