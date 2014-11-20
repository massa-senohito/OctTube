#include "stdafx.h"
#include "Texture.h"
#include "Shader.h"

//#define _TEXTEST
#ifndef _TEXTEST
#define  STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//include対象でないクラスなら大丈夫
static unsigned char tex[] = {
    255, 255, 255, 255,     0,   0,   0, 255,   255, 255, 255 ,255,     0,   0,   0, 255,
    255,   0,   0, 255,     0, 255,   0, 255,     0,   0, 255 ,255,   255, 255, 255, 255,
    128,   0,   0, 255,     0, 128,   0, 255,     0,   0, 128 ,255,   128, 128, 128, 255,
    255, 255,   0, 255,   255,   0, 255, 255,     0, 255, 255 ,255,   255, 255, 255, 255,
};
const int TEXSIZE=64;
GLubyte bits[TEXSIZE][TEXSIZE][3];//rgb
void bitsInit(){
  for (int i = 0 ; i < TEXSIZE ; i++) {
    int r = (i * 0xFF) / TEXSIZE;
    for (int j = 0 ; j < TEXSIZE ; j++) {
      bits[i][j][0] = (GLubyte)r;
      bits[i][j][1] = (GLubyte)(( j * 0xFF ) / TEXSIZE);
      bits[i][j][2] = (GLubyte)~r;
    }
  }
}
GLubyte * obits=nullptr;
void widheiInit(int width,int height){
    obits=new GLubyte[3*width*height];
    int index=0;
  for(int i = 0 ; i < height ; i++) {
    int r = (i * 0xFF) / height;
    for(int j = 0 ; j < width ; j++) {
      obits[index++] = r;
      obits[index++] = (( j * 0xFF ) / width);
      obits[index++] = ~r;
    }
  }
}
void Texture::makeTestTex(){
  bitsInit();
  glGenTextures(1 , &texName);
  glBindTexture(GL_TEXTURE_2D , texName);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,          // mipmap
    GL_RGBA,
    4,          // width
    4,          // height
    0,          // border
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    tex
  );
  //glTexImage2D(
  //  GL_TEXTURE_2D , 0 , 3 , TEXSIZE , TEXSIZE ,
  //  0 , GL_RGB , GL_UNSIGNED_BYTE , bits
  //);
  widheiInit(300,200);
}
///テストのためのグラフィックパターンテクスチャを生成する
Texture::Texture(void)
{
  upc = new float[]{ 0.9f, 0.9f, 1.0f };
  downc = new float[]{0.2f, 0.1f, 0.6f};
  //Shader texShader("textured.vert","textured.frag");
}
void inline bind2d(GLuint n){
    glBindTexture(GL_TEXTURE_2D,n);
}

bool loadPath(std::string s,stbi_uc* data){
  int x,y, req_comp;
  data = stbi_load(s.c_str(), &x, &y, &req_comp, STBI_rgb_alpha);
  if (data){
    auto form = req_comp == 3?GL_RGB: GL_RGBA;
    glTexImage2D(
      GL_TEXTURE_2D, 0, form, x, y,
      0, form, GL_UNSIGNED_BYTE, data);
  }
  return data;
}

stbi_uc* data = nullptr;
typedef std::unique_ptr<std::string> Strptr;
typedef std::vector<Strptr> Paths;
GLuint* reserveTexture(int num,Paths& paths)
{
  assert(num == paths.size());
  auto texs = new GLuint[num];
  glGenTextures(num, texs);
  for (int i = 0; i < num; i++)
  {
    glBindTexture(GL_TEXTURE_2D, texs[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//拡大・縮小についての設定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    if (!loadPath(*paths.at(i), data))
    {
      exit(2);
    }
    stbi_image_free( data);
  }
  return texs;
}
void deleteTex(int num, GLuint* tex)
{
  glDeleteTextures(num, tex);
}
void spriteTexture(GLuint id, int x, int y)
{
  bind2d(id);
  glEnable(GL_TEXTURE_2D);
  //setMask
  glRasterPos2f(x, y);
  glScalef(30, 30,1);
  glBegin(GL_QUADS);
  // Front Face
  glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex2f( 1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex2f( 1.0f,  1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f,  1.0f);  // Top Left Of The Texture and Quad
  glEnd();
  glDisable(GL_TEXTURE_2D);
}
#define bai 0.9f
void ver2(float x,float y){
    glVertex3f(x*bai,y*bai,0);
}
void Texture::setBlue(){
    glColor3fv(downc);
}
void Texture::setWhite (){
    glColor3fv(upc);
}
void Texture::regacyGLTex(float* vs,GLuint texName){
    float x=vs[0]   ;float y=vs[1];
    float xx=vs[2]  ;float yy=vs[3];
    float xxx=vs[4] ;float yyy=vs[5];
    float xxxx=vs[6];float yyyy=vs[7];
      //glColor4i(1,1,1,1);
    bind2d(texName);
    glBegin(GL_QUADS);
#define oneone  1,1
#define onezero 1,0
#define zeroone 0,1
#define zeroz   0,0
      //zeroz
      //zeroone
      //oneone
      //onezero
    //textureが回転して表示されるはず
    setBlue();
    glTexCoord2f(zeroone); ver2(x   ,   y);
    setWhite();
      glTexCoord2f(oneone); ver2(xx  ,  yy);
      glTexCoord2f(onezero); ver2(xxx , yyy);
    setBlue();
      glTexCoord2f(zeroz); ver2(xxxx,yyyy);
    glEnd();
      //glColor4i(1,1,1,1);
    bind2d(0);
    //glDisable( GL_TEXTURE_2D );
}
void latestGLTex(float* vs,GLuint texName){
    //glEnableClientState(GL_VERTEX_ARRAY);
    bind2d(texName);
    GLuint ind[]={0,1,2,0,2,3};
    //GLfloat texuv[]={0,0,0,1,1,1,1,0};
    GLfloat texuv[]={0,1,1,1,1,0,0,0};
    glTexCoordPointer(2, GL_FLOAT, 0,texuv);
    glRectf(vs[0],vs[1],vs[4],vs[5]);
    //glVertexPointer  (2, GL_FLOAT, 0, vs);
    //glDrawElements(GL_POLYGON, 2, GL_UNSIGNED_INT, ind);
    //glDisableClientState(GL_VERTEX_ARRAY);
}
void Texture::SetVertexColor2(float* upcol,float* downcol)
{
  _ASSERT(upcol);
  _ASSERT(downcol);
  upc   = upcol;
  downc = downcol;
}
void Texture::BindVert(float* vs){
    //色だけじゃなくデプスやステンシル、輝度値も
    //glRasterPos2i(0,0);
    //glDrawPixels(TEXSIZE,TEXSIZE,GL_RGB,GL_UNSIGNED_INT,bits);
    //glDrawPixels(300,100,GL_RGB,GL_UNSIGNED_INT,obits);
    //GLfloat color[]={1,1,1,1};
    //glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
    //glNormal3d(0.0, 0.0, 1.0);
    //vbo(vs,sizeof(float)*8);
    //ebo(ind,sizeof(int)*6);
    regacyGLTex(vs,texName);
    //latestGLTex(vs,texName);
}
///複数のテクスチャファイルを読み込み、それぞれのmap<name,id>を管理する
//Texture::Texture(array<>)

Texture::~Texture(void)
{
  if (data) { stbi_image_free( data); data = nullptr; }
  if (obits)
  {
    delete[] obits;
  }
  glDeleteTextures(1, &texName);
}
#else

GLuint* reserveTexture
(int num, Paths& paths)
{
  return nullptr;
}

void spriteTexture
(GLuint id, int x, int y)
{
}
void deleteTex(int num, GLuint* tex)
{

}
Texture::Texture() {}
Texture::~Texture() {}
bool Texture::LoadShader()
{
  return false;
}

void Texture::regacyGLTex(float*, GLuint)
{
  ;
}
void Texture::BindVert(float*)
{
  ;
}
//#define ilist _INITIALIZER_LIST_
void Texture::SetVertexColor2(float*, float*)
{

}
void setBlue()
{

}
void setWhite()
{

}
#endif