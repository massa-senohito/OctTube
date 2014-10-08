#include "stdafx.h"
#include "TextureTest.h"
#include "stb_perlin.h"
#include "DebugLogger.h"
GLuint*      texture;
DebugLogger& logger = *(new DebugLogger);
typedef unsigned char  PixData;
//typedef unsigned char* PPixData;
typedef std::vector<PixData> PPixData;
PPixData*    pixels;
int sizeX =  101;
int sizeY =  101;
void DrawPolys();
int  DrawGLScene(GLvoid);

void testDisplay()
{
  DrawGLScene();
  DrawPolys();
  glutSwapBuffers();
}
void OnExit()
{
  glDeleteTextures(1, texture);
  delete texture;
  delete[] pixels;
}
void perlinTex(){
  auto kiz   = 0.02f;
  auto to    = 1;
  auto pc    = (to + 1) / kiz+1;
  auto count = 0;
  //pixels = new PixData[(int)(pc*pc)];//10201
  pixels = new PPixData;//((int)pc*pc);
  for   (float i = -1; i < to; i += kiz){
    for (float j = -1; j < to; j += kiz)
    {
      auto nois = stb_perlin_noise3
        (i * 10 , j * 10 , 0 ) + 1.0f;
      nois  = nois / 2.0f * 255.0f;
      //pixels[count++] = nois;
      pixels-> push_back(nois);
    }
  }
}
bool LoadGLTextures(){
  perlinTex();
  return 1;
}

void tt(int value) {
  static int count = 0;
  static bool isValid = true;
  if (isValid){
    if ( ++count > 20){
      delete &logger;
      isValid = false;
    }
    else{
      logger.Send("hello");
      logger.Send( std::to_string(count).data() );
    }
  }
  glRotatef(1 , 0.5 , 1 , 0.25);
  glutPostRedisplay();
  glutTimerFunc(50 , tt , 0);
}
void InitGlut(){
  auto one = 1; char* hoge = "";
  glutInit( &one , &hoge );
  glutInitWindowSize( 400 , 300 );
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutDisplayFunc( testDisplay );
  glutTimerFunc(100 , tt , 0);
  glutCloseFunc( OnExit );
  glutCreateWindow("Kitty on your lap");
}
int InitGL(GLvoid)                      // All Setup For OpenGL Goes Here
{
  glEnable    (GL_TEXTURE_2D);              // Enable Texture Mapping ( NEW )
  glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
  glClearColor(0.0f, 0.0f, 0.0f, 0.5f); // Black Background
  glClearDepth(1.0f);                   // Depth Buffer Setup
  glEnable    (GL_DEPTH_TEST);              // Enables Depth Testing
  glDepthFunc (GL_LEQUAL);               // The Type Of Depth Testing To Do
  glHint      (GL_PERSPECTIVE_CORRECTION_HINT,
    GL_NICEST);          // Really Nice Perspective Calculations
  return TRUE;                          // Initialization Went OK
}
TextureTest::TextureTest()
{

  InitGlut();
  InitGL  ();
  LoadGLTextures();
  texture = new GLuint;
  glGenTextures(1, texture);   // Create The Texture

  // Typical Texture Generation Using Data From The Bitmap
  glBindTexture(GL_TEXTURE_2D, *texture);
  // Generate The Texture
  glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX , sizeY
    , 0, GL_RGB, GL_UNSIGNED_BYTE, pixels->data());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Filtering
  glutMainLoop();

}
void DrawPolys(){
  glBegin(GL_QUADS);
  // Front Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);  // Top Left Of The Texture and Quad
  // Back Face
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
  // Top Face
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, 1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
  // Bottom Face
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
  // Right face
  glTexCoord2f(1.0f, 0.0f); glVertex3f(1.0f, -1.0f, -1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(1.0f, 1.0f, -1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(1.0f, 1.0f, 1.0f);  // Top Left Of The Texture and Quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom Left Of The Texture and Quad
  // Left Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);  // Bottom Left Of The Texture and Quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, 1.0f);  // Bottom Right Of The Texture and Quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);  // Top Right Of The Texture and Quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, -1.0f);  // Top Left Of The Texture and Quad
  glEnd();
}
int DrawGLScene(GLvoid)
{
  glClear(GL_COLOR_BUFFER_BIT 
    | GL_DEPTH_BUFFER_BIT);       // Clear Screen And Depth Buffer
  glLoadIdentity();               // Reset The Current Matrix
  glTranslatef( 0.0f , 0.0f, -5.0f );// Move Into The Screen 5 Units
  glBindTexture(GL_TEXTURE_2D , *texture);// Select Our Texture
  return 1;
}
TextureTest::~TextureTest()
{
  OnExit();
}
