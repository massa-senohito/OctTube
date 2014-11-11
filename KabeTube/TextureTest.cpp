#include "stdafx.h"
#include "TextureTest.h"
#include "stb_perlin.h"
#include "DebugLogger.h"
#define  STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
GLuint*      texture=nullptr;
DebugLogger& logger = *(new DebugLogger);
typedef unsigned char  PixData;
//typedef unsigned char* PPixData;
typedef std::vector<PixData> PPixData;
PPixData*    pixels=nullptr;
int sizeX =  101;
int sizeY =  101;
void DrawPolys();
int  DrawGLScene(GLvoid);
stbi_uc* data =nullptr;

#define SAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
void testDisplay()
{
  DrawGLScene();
  DrawPolys();
  glutSwapBuffers();
}
void OnExit()
{
  glDeleteTextures(1, texture);
  if (data){ stbi_image_free(data); data = nullptr; }
  SAFE_DELETE( texture);
  SAFE_DELETE_ARRAY( pixels);
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
bool loadPath(string s,stbi_uc* data){
  int x,y, req_comp;
  data = stbi_load(s.c_str(), &x, &y, &req_comp, STBI_rgb_alpha);
  if (data){
    auto form = req_comp == 3?GL_RGB: GL_RGBA;
    glTexImage2D(
      GL_TEXTURE_2D, 0, form, x, y,
      0, form, GL_UNSIGNED_BYTE, data
      );
  }
  return data;
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

  glutInit(&one, &hoge);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize( 400 , 300 );
  
  glutCreateWindow("file tex test");
  glClearColor(0, 0.5, 0, 1);
  glutDisplayFunc ( testDisplay);
  glutTimerFunc   (100 , tt , 0);
  //glutSpecialFunc ( key );
  //glutKeyboardFunc( nokey );
}
int InitGL(GLvoid)                      // All Setup For OpenGL Goes Here
{
  glEnable    (GL_TEXTURE_2D);              // Enable Texture Mapping ( NEW )
  glShadeModel(GL_SMOOTH);              // Enable Smooth Shading
  glClearDepth(1.0f);                   // Depth Buffer Setup
  glEnable    (GL_DEPTH_TEST);              // Enables Depth Testing
  glDepthFunc (GL_LEQUAL);               // The Type Of Depth Testing To Do
  glHint      (GL_PERSPECTIVE_CORRECTION_HINT,
    GL_NICEST);          // Really Nice Perspective Calculations

  glEnable(GL_BLEND);
  return TRUE;                          // Initialization Went OK
}
TextureTest::TextureTest()
{

  InitGlut();
  InitGL  ();
  //LoadGLTextures();
  texture = new GLuint;
  glGenTextures(1, texture);   // Create The Texture

  glBindTexture(GL_TEXTURE_2D, *texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//Šg‘åEk¬‚É‚Â‚¢‚Ä‚ÌÝ’è
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  // Typical Texture Generation Using Data From The Bitmap
  // Generate The Texture
  if (!loadPath("D:\\onD\\OctTWork\\OctTube\\Debug\\tex.png", data)){
    stbi_image_free( data);
    exit(0);
  }
  //glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX , sizeY
  //  , 0, GL_RGB, GL_UNSIGNED_BYTE, pixels->data());
  glutMainLoop();

}
void DrawPolys(){
  
  glEnable(GL_TEXTURE_2D);
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
  glDisable(GL_TEXTURE_2D);
}
int DrawGLScene(GLvoid)
{
  glClear(GL_COLOR_BUFFER_BIT 
    | GL_DEPTH_BUFFER_BIT);       // Clear Screen And Depth Buffer
  glLoadIdentity();               // Reset The Current Matrix
  glScalef(0.5, 0.5, 0.5);
  //glTranslatef( 0.0f , 0.0f, -5.0f );// Move Into The Screen 5 Units
  glBindTexture(GL_TEXTURE_2D , *texture);// Select Our Texture
  return 1;
}
TextureTest::~TextureTest()
{
  OnExit();
}
