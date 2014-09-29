#pragma once
void GLUT_INITs(int,char**);
//typedef void(* Callback)() ;
typedef std::function<void(int)> RenderCallback;
typedef std::function<void(void)> Callback;

#define Vector std::vector
#define Points float*
void renderVertice(Points ps,int len,int);
class Renderer
{
public:

    Renderer(RenderCallback,Callback);
    GLenum renderPolygon(GLvoid* vert,int length);
    // V2 pos,float scale,float angle
    ~Renderer(void);
};
//https://github.com/assimp/assimp/blob/master/samples/SimpleOpenGL/Sample_SimpleOpenGL.c
namespace Key{
enum  Key{
  Up,Down,Left,Right,A,Length
};
  int timeOfAPushed();
  int timeOfLeftPushed();
  int timeOfRightPushed();
  bool isAPushed();
  bool isLeftPushed();
  bool isRightPushed();
  bool isUpPushed();
  bool isDownPushed();
}