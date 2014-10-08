#pragma once
//#include <initializer_list>
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
