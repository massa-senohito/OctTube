#include "stdafx.h"
#include "Texture.h"

#include "Shader.h"

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

///テストのためのグラフィックパターンテクスチャを生成する
Texture::Texture(void)
{
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
  //Shader texShader("textured.vert","textured.frag");
}
void inline bind2d(GLuint n){
    glBindTexture(GL_TEXTURE_2D,n);
}
#define bai 0.9
void ver2(float x,float y){
    glVertex3f(x*bai,y*bai,0);
}
void setBlue(){
    glColor3f(0.2,0.1,0.6);
}
void setWhite (){
    glColor3f(0.9,0.9,1.0);
}
void regacyGLTex(float* vs,GLuint texName){
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
    if (obits)
    {
	delete[] obits;
    }
    glDeleteTextures(1,&texName);
}