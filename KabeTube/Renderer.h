#pragma once
void GLUT_INITs(int,char**);
//typedef void(* Callback)( int) ;
typedef std::function<void(int)> Callback;

#define Vector std::vector
#define Points float*
void renderVertice(Points ps,int len,int);
class Renderer
{
public:

    Renderer(Callback);
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