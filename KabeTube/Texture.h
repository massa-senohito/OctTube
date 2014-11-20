#pragma once
#include <memory>

//#define _TEXTEST
typedef std::unique_ptr<std::string> Strptr;
typedef std::vector<Strptr> Paths;
GLuint* reserveTexture
  (int num, Paths& paths);

void spriteTexture
  (GLuint id, int x, int y);
void deleteTex(int num, GLuint* tex);
class Texture
{
  GLuint texName;
  void makeTestTex();
  float* upc;
  float* downc;
  void setBlue();
  void setWhite();
public:
  Texture(void);
  ~Texture(void);
  bool LoadShader();
  void regacyGLTex(float*,GLuint);
  void BindVert(float*);
  //#define ilist _INITIALIZER_LIST_
  void SetVertexColor2(float*,float*);
};
