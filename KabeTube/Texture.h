#pragma once
class Texture
{
  GLuint texName;
public:
  Texture(void);
  ~Texture(void);
  bool LoadShader();
  void BindVert(float*);
};
