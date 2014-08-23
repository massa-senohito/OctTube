#pragma once
void GLUT_INITs(int,char**);
//typedef void(* Callback)( int) ;
typedef std::function<void(int)> Callback;
class Renderer
{
public:

    Renderer(Callback);
    GLenum renderPolygon(GLvoid* vert,int length);
    ~Renderer(void);
};

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
}