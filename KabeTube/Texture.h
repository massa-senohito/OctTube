#pragma once
class Texture
{
  GLuint texName;
  void makeTestTex();
public:
  Texture(void);
  ~Texture(void);
  bool LoadShader();
  void BindVert(float*);
};
