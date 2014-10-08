#include "stdafx.h"
#include "Renderer.h"
#include "GUIUtil.h"
#include "Texture.h"
#include "GameAlgolyzm.h"
namespace Key{

  bool pushed[Key::Length];
  bool* GetPushed(){
    return pushed;
  }
  int lastPushed[Key::Length];
  bool isAPushed(){
      return pushed[Key::A];
  }
  bool isLeftPushed(){
      return pushed[Key::Left];
  }
  bool isRightPushed(){
      return pushed[Key::Right];
  }
  bool isUpPushed(){
      return pushed[Key::Up];
  }
  bool isDownPushed(){
      return pushed[Key::Down];
  }
  int timeOfAPushed(){
      return lastPushed[Key::A];
  }
  int timeOfLeftPushed(){
      return lastPushed[Key::Left];
  }
  int timeOfRightPushed(){
      return lastPushed[Key::Right];
  }
}
typedef float float32;
typedef const char* string;
float32 radius=0;//2
float32 pradius=0.1f;
const float32 pi=3.141592f;

void setMask(GLint ref);
void setPoly(GLint ref);
//typedef void (APIENTRY *GLDEBUGPROC)(GLenum source, GLenum type,
//GLuint id, GLenum severity, GLsizei length, const GLchar* message, GLvoid* userParam);
void debugCall(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
               const GLchar* message, GLvoid* userParam){
                   std::cout << message << std::endl;
}

void setConsoleColorYellow(){
#ifdef _MANAGED
   auto ye=System::ConsoleColor::Yellow;
   System::Console::ForegroundColor=ye;
#else
   printf("\x1b[33m"); 
#endif
}
 //printf("\x1b[41m");//red
void  setConsoleColorDef(){
#ifdef _MANAGED
  System::Console::ResetColor();
#else
  printf("\x1b[0m");
#endif
}
void loopEnd(){
    //setConsoleColorYellow();
    //std::cout << radius //"loopEnd"
    //    << std::endl;
    //setConsoleColorDef();
}
bool isErr(const GLchar* mes,int line){
  GLenum er=glGetError();
  int inva=GL_INVALID_OPERATION;
  int val=GL_INVALID_VALUE;
  bool res=GL_NO_ERROR==er;
  if (!res) {
    std::cout << mes <<std::endl;
    std::cout << "err number" << er << std::endl;
    if(er==inva){
        std::cout << "inva oper" << std::endl;
    }
    if(er==val){
        std::cout << "inva value" << std::endl;
    }//��ԕ\���ƍ��킹�ă}�N���ɂł�����
    std::cout << " in " << line << std::endl;
  }
  return res;
}
void getOnce();
void getStenc();
void oneCall(std::function<void(void)> f);
void display();
void stencilInit();
//GUIUtil* ui;
std::map<string,int> callCount;
std::map<string,int> callTime;
static int count=0;
void pushCommand(const char* s){
  callCount[s]++;
}
void createBParticle(float32 x,float32 y,float32 pr){
  //�Ƃ肠����6�p�`
  glBegin(GL_POLYGON);
  for (int i = 0; i < 6; i++)
  {
    float32 radius=pr;
    float32 radc=i*pi/3;
    glVertex2f(x+radius*cosf(radc),y+radius*sinf(radc));
  }
  glEnd();
}
void createBallon(float32 radius,float32 pradius,float32 x,float32 y){
  if (pradius<0.3f){pradius=0.31f;}
  float32 hpr=pradius/2;
  float32 rad=pi-2*acos(hpr/radius);
  float32 radc=rad;
  float32 dpi=pi*2;
  while (radc <dpi){
    createBParticle(radius*cosf(radc),radius*sinf(radc),pradius);
    radc+=rad;
  }
}
void gluiInit(){
  //GLUI�̐ݒ�
  //ui = new GUIUtil;
  //ui->addSpinner("radius",&radius);
  //ui->addSpinner("pradius",&pradius);
}
static const GLuint ignoreId[]={0};
RenderCallback drawCall;
Texture* testTex;
Callback onClose;
Renderer::Renderer(RenderCallback c,Callback close)
{
  drawCall=c;
  onClose = close;
  float one=1.0;
  float zero=0.0;
  glClearColor(zero, zero, zero,one);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  GLenum tex2d=GL_TEXTURE_2D;
#define gltex(t) GL_TEXTURE ## t
  glTexParameterf(tex2d,gltex(_WRAP_S),GL_CLAMP);
  glTexParameterf(tex2d,gltex(_WRAP_T),GL_CLAMP);
  glTexParameteri(tex2d,gltex(_MAG_FILTER),GL_NEAREST);
  glTexParameteri(tex2d,gltex(_MIN_FILTER),GL_NEAREST);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  glEnable(tex2d);

  glPixelTransferi(GL_RED_SCALE,1);
  glPixelStorei(GL_UNPACK_SWAP_BYTES,GL_TRUE);

  stencilInit();
  //�ގ���glColor�Őݒ肷��
#ifdef COLMAT
  glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_COLOR_MATERIAL);
#endif
  testTex=new Texture;
  //glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);
  //glutIgnoreKeyRepeat(GL_TRUE); //���s�[�g�𖳎�
  for (int i = 0; i < Key::Length; i++)
  {
      Key::pushed[i]=false;
  }
}
void push(){glPushMatrix();}
void pop(){glPopMatrix();}
void rotationTeapod(){
  int hund= 100;//�͂₳
  push();
    glColor3f(1,1,1);
    auto move=(count%hund)-hund/2;
    for (int i = 0; i < 3; i++)
    {
      push();
  	auto ipi=i*pi/2;
        glTranslatef(move/(float)hund*cosf(i*pi)*1.3f,cosf(ipi)*0.5f,0);
        glutSolidCube(0.3f);
      pop();
    }
  pop();
}
void teapotTest(bool mask){
  push();
  glDepthMask(GL_FALSE);
  {
    //if(toggleWindow){
    //  WindowUI(0.8f,0.8f);}
    //glRotatef(360 - CurrentPos.HeadAngle,0,1.0,0);
    //glTranslatef(-CurrentPos.PosX, -CurrentPos.PosY - 0.25, -CurrentPos.PosZ);
    //gluLookAt(CurrentPos.PosX,CurrentPos.PosY+6+ey,CurrentPos.PosZ+2+ez,
    //  CurrentPos.PosX,CurrentPos.PosY,CurrentPos.PosZ,
    //  0,0,-1);
    glClearStencil(5);
    if(mask) setMask(1);
    {
      glClear(GL_STENCIL_BUFFER_BIT);
      rotationTeapod();
     
      //createBParticle(radius,pradius,radius);
    }
    if(mask) setPoly(3);
    push();
      glColor3f(1,0,0);//���Ԃ�v���`�i�̉e�Ȃ񂩂̓A�L�������[�V�����o�b�t�@�ŉe�}�b�v�ǂݍ���ŕ`��Ƃ��������̂ł�
      glRotatef(count*1.3f,0,1,0);
      glutSolidTeapot(0.7f);
    pop();
  }
  glDepthMask(GL_TRUE);
  pop();
}
//------------- ��������e��R�[���o�b�N --------------//

#define at0 0,0,0
#define up 0,1,0
double rx,ry;
void quads(){
  glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_QUADS);
    glVertex2i(25, 25);
    glVertex2i(25, 75);
    glVertex2i(75, 75);
    glVertex2i(75, 25);
    glEnd();
}
void LightUpdate(){
  static float LightPos[]= {0,0,radius};

  glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
}
void display()
{
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glLoadIdentity();
  //glMatrixMode��MODELVIEW�s��ɂ��Ă���
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  static float wid=320.0/6.0;
  static float hei=240.0/6.0;
  //wid = 180;
  //hei= 180;
  if (Key::isLeftPushed()){
    //wid -= 1;
  }
  if (Key::isDownPushed()){
    //hei -= 1;
  }
  glOrtho(-wid,wid,-hei,hei,-50,50);
  
  glMatrixMode(GL_MODELVIEW);
  static GLfloat back[]={-wid,-hei,-wid,hei,wid,hei,wid,-hei};
  //gluLookAt(0,0,pradius,at0,up);
  push();
  glRotated(rx,0,1,0);
  glRotated(ry,1,0,0);
  glClearStencil(5);
  //display�I���Ȃ������ɍĊJ���Ă�
  //glTranslatef(radius,0,pradius);
  setMask(1);//glClearStencil��ref�Ɠ���ւ����invert����
  glClear(GL_STENCIL_BUFFER_BIT);
  drawCall(count);
  //glPushAttrib(GL_CURRENT_BIT);
  //������������X�e���V��disable�ɂ��Ȃ��ƁH
  setPoly(3);
  testTex->BindVert(back);
//  static auto uc = new float[]{ 0.3f, 0.3f, 0.3f };
//  static auto dc = new float[]{0.8f, 0.5f, 0.2f};
//  testTex->SetVertexColor2(uc, dc);

    //glPopAttrib();
  //teapotTest(false);
      //glClear(GL_STENCIL_BUFFER_BIT);
    //testTex->BindVert(back);
  pop();
  glutSwapBuffers();
  //CheckKeyStatus();
  loopEnd();
}
void timer(int value)
{
  //oneCall(getStenc);
  count++;
  glutPostRedisplay();
  //update(pushed);

  glutTimerFunc(value,timer,17); //�^�C�}�[�֐�
}

void onDrag(int x,int y){
  //�}�E�X��zx��]�Ƀ}�b�v
  //std::cout << x << std::endl;
  //std::cout << y << std::endl;
  //320-> 90 160->0
  //0  ->-90  180/wid*
  //rx=180.0/640.0*x-90.0;
  //ry=180.0/480.0*y-90.0;

}
void specialkey(int key,int x,int y){

  float32 del=0.01f;//todo GLCTest�Ɠ����g�p�ɂ���
  if(key==GLUT_KEY_UP){ //radius+=del;
    Key::pushed[Key::Up]=true;
    Key::lastPushed[Key::Up]=count;
  }
  if(key==GLUT_KEY_DOWN){
    Key::pushed[Key::Down]=true;
    Key::lastPushed[Key::Down]=count;
  }
  if(key==GLUT_KEY_LEFT){ //pradius+=del;
    Key::pushed[Key::Left]=true;
    Key::lastPushed[Key::Left]=count;
  }
  if(key==GLUT_KEY_RIGHT){ //pradius-=del;
    Key::pushed[Key::Right]=true;
    Key::lastPushed[Key::Right]=count;
  }
  if(key==GLUT_KEY_ALT_L) glutExit();
  if(key==GLUT_KEY_CTRL_L){ 
    Key::pushed[Key::A]=true;
    Key::lastPushed[Key::A]=count;
  }
}
void SpecialKeyUp(int key,int,int){
    //todo �����ōŏI�����t���[���X�V
  if(key==GLUT_KEY_UP) //radius+=del;
      Key::pushed[Key::Up]=false;
  if(key==GLUT_KEY_DOWN)// radius-=del;
      Key::pushed[Key::Down]=false;
  if(key==GLUT_KEY_LEFT) //pradius+=del;
      Key::pushed[Key::Left]=false;
  if(key==GLUT_KEY_RIGHT) //pradius-=del;
      Key::pushed[Key::Right]=false;
  if(key==GLUT_KEY_CTRL_L) 
      Key::pushed[Key::A]=false;
}
void renderVertice(Points ps,int len,int anim){
#ifdef _MANAGED
  int pl=ps->Length;
#else
  int pl=anim*len+len;
  //auto verts=Vector<float>(len);
  
#endif
  for (int i = anim*len; i < pl; i+=2)
  {
#ifdef _MANAGED
    int psi = ps[i];
#else
    auto psi = ps[i];
    auto psi1 = ps[i+1];
    //verts[i]=(psi);
    //verts[i+1]=(psi1);
#endif
    glVertex2f( psi-59,psi1-90);
    //glVertex2f( psi,psi1);
  }
}
void idle(){
}
void cl(){
  onClose();
}
void GLUT_CALL_FUNCs()
{
  //glui�Ǝ�荇���ɂȂ�炵���̂�
  //GLUI_Master.set_glutIdleFunc( idle );
  //lib�����Ԃ񖭂Ȃ��Ƃ��Ă�  gluiInit();
  glutDisplayFunc(display);
  //glutIdleFunc
  //glutReshapeFunc(reshape);
  glutMotionFunc(onDrag);
  glutTimerFunc(0,timer,17);
  glutSpecialFunc(specialkey);
  glutSpecialUpFunc(SpecialKeyUp); //�L�[�𗣂������̏���
  //glutJoystickFunc(joyStick,10);
  glutCloseFunc(cl);
}
void GLUT_INITs(int argcp,char**argvp)
{
  glutInit(&argcp,argvp);
  glutInitDisplayMode(GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH |GLUT_STENCIL);
  glutInitWindowSize(640,480);
  glutCreateWindow("oct tube");
  //  std::cout << "Vendor :"<<glGetString(GL_VENDOR)<<'\n';
  //std::cout << "GPU : "<<glGetString(GL_RENDERER) <<'\n';
  //std::cout << "OpenGL ver. " <<glGetString(GL_VERSION)<<'\n';
  //std::cout << "�y�g���@�\�ꗗ�z"<<std::endl;
  //std::cout << glGetString(GL_EXTENSIONS) <<std::endl;

#if defined(WIN32)
  /* GLEW �̏����� */
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    exit(1);
  }
  if(GLEW_ARB_point_parameters){
    ;
  }
#endif
  glutInitContextFlags(GLUT_FORWARD_COMPATIBLE
#if _DEBUG
        | GLUT_DEBUG
#endif
  );
  isErr("",__LINE__);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  isErr("",__LINE__);
  glDebugMessageCallback(
    //reinterpret_cast<
     (GLDEBUGPROC)
    //>
    ( debugCall),nullptr);
  glDebugMessageControl(GL_DONT_CARE,
    GL_DONT_CARE,
    GL_DONT_CARE,
    0,
    ignoreId,
    true);
  //MakeFloor(vec3(0,3,0),10,floormat);
  GLUT_CALL_FUNCs();
}
void oneCall(std::function<void(void)> f){
  static bool fl=false;
  if(!fl) {f();fl=true;}

}
void setSphereMap(){
  //http://marina.sys.wakayama-u.ac.jp/~tokoi/?date=20050121
  //�X�t�B�A�}�b�v�̓J��������̊p�x�Ń}�b�s���O����邽�߁A�J�������ړ�����ƁA�}�b�v�������
  //�L���[�u�}�b�v������
}
void drawLines(std::function<void(void)> f)
{
  glBegin(GL_LINES);
  f();
  glEnd();
}
//b2Draw�̂��Q�l�ɂ���
float range[2];
void getOnce(){
    glGetFloatv(GL_VIEWPORT_BOUNDS_RANGE,range);
    std::cout << range[0] << std::endl;

    glGetFloatv( GL_CLIENT_VERTEX_ARRAY_BIT ,range  );
std::cout << "GL_CLIENT_VERTEX_ARRAY_BIT" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT_BIT ,range  );
std::cout << "GL_VIEWPORT_BIT" << range[0] << std::endl;

glGetFloatv( GL_LIGHT_MODEL_LOCAL_VIEWER ,range  );
std::cout << "GL_LIGHT_MODEL_LOCAL_VIEWER" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT ,range  );
std::cout << "GL_VIEWPORT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW_STACK_DEPTH ,range  );
std::cout << "GL_MODELVIEW_STACK_DEPTH" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW_MATRIX ,range  );
std::cout << "GL_MODELVIEW_MATRIX" << range[0] << std::endl;

glGetFloatv( GL_MAX_MODELVIEW_STACK_DEPTH ,range  );
std::cout << "GL_MAX_MODELVIEW_STACK_DEPTH" << range[0] << std::endl;

glGetFloatv( GL_MAX_VIEWPORT_DIMS ,range  );
std::cout << "GL_MAX_VIEWPORT_DIMS" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_3 ,range  );
std::cout << "GL_MAP1_VERTEX_3" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_4 ,range  );
std::cout << "GL_MAP1_VERTEX_4" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_3 ,range  );
std::cout << "GL_MAP2_VERTEX_3" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_4 ,range  );
std::cout << "GL_MAP2_VERTEX_4" << range[0] << std::endl;

glGetFloatv( GL_AND_INVERTED ,range  );
std::cout << "GL_AND_INVERTED" << range[0] << std::endl;

glGetFloatv( GL_INVERT ,range  );
std::cout << "GL_INVERT" << range[0] << std::endl;

glGetFloatv( GL_COPY_INVERTED ,range  );
std::cout << "GL_COPY_INVERTED" << range[0] << std::endl;

glGetFloatv( GL_OR_INVERTED ,range  );
std::cout << "GL_OR_INVERTED" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW ,range  );
std::cout << "GL_MODELVIEW" << range[0] << std::endl;
/*  ���_�z��n
glGetFloatv( GL_VERTEX_ARRAY ,range  );
std::cout << "GL_VERTEX_ARRAY" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_SIZE ,range  );
std::cout << "GL_VERTEX_ARRAY_SIZE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_TYPE ,range  );
std::cout << "GL_VERTEX_ARRAY_TYPE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_STRIDE ,range  );
std::cout << "GL_VERTEX_ARRAY_STRIDE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_POINTER ,range  );
std::cout << "GL_VERTEX_ARRAY_POINTER" << range[0] << std::endl;
*/
glGetFloatv( GL_MAX_ELEMENTS_VERTICES ,range  );
std::cout << "GL_MAX_ELEMENTS_VERTICES" << range[0] << std::endl;

glGetFloatv( GL_SAMPLE_COVERAGE_INVERT ,range  );
std::cout << "GL_SAMPLE_COVERAGE_INVERT" << range[0] << std::endl;

glGetFloatv( GL_TRANSPOSE_MODELVIEW_MATRIX ,range  );
std::cout << "GL_TRANSPOSE_MODELVIEW_MATRIX" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_BUFFER_BINDING ,range  );
std::cout << "GL_VERTEX_ARRAY_BUFFER_BINDING" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_ENABLED ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_ENABLED" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_SIZE ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_SIZE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_STRIDE ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_STRIDE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_TYPE ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_TYPE" << range[0] << std::endl;

glGetFloatv( GL_CURRENT_VERTEX_ATTRIB ,range  );
std::cout << "GL_CURRENT_VERTEX_ATTRIB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_POINT_SIZE ,range  );
std::cout << "GL_VERTEX_PROGRAM_POINT_SIZE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_TWO_SIDE ,range  );
std::cout << "GL_VERTEX_PROGRAM_TWO_SIDE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_POINTER ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_POINTER" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATTRIBS ,range  );
std::cout << "GL_MAX_VERTEX_ATTRIBS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER ,range  );
std::cout << "GL_VERTEX_SHADER" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_UNIFORM_COMPONENTS ,range  );
std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS ,range  );
std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_INTEGER ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_INTEGER" << range[0] << std::endl;

glGetFloatv( GL_CLAMP_VERTEX_COLOR ,range  );
std::cout << "GL_CLAMP_VERTEX_COLOR" << range[0] << std::endl;

glGetFloatv( GL_GEOMETRY_VERTICES_OUT ,range  );
std::cout << "GL_GEOMETRY_VERTICES_OUT" << range[0] << std::endl;

glGetFloatv( GL_MAX_GEOMETRY_OUTPUT_VERTICES ,range  );
std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_OUTPUT_COMPONENTS ,range  );
std::cout << "GL_MAX_VERTEX_OUTPUT_COMPONENTS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_DIVISOR ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_DIVISOR" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_LONG ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_LONG" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATTRIB_STRIDE ,range  );
std::cout << "GL_MAX_VERTEX_ATTRIB_STRIDE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ELEMENT_SWIZZLE_AMD ,range  );
std::cout << "GL_VERTEX_ELEMENT_SWIZZLE_AMD" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ID_SWIZZLE_AMD ,range  );
std::cout << "GL_VERTEX_ID_SWIZZLE_AMD" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_OBJECT_AMD ,range  );
std::cout << "GL_VERTEX_ARRAY_OBJECT_AMD" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ANGLE" << range[0] << std::endl;

glGetFloatv( GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE ,range  );
std::cout << "GL_MIN_PBUFFER_VIEWPORT_DIMS_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_BINDING_APPLE ,range  );
std::cout << "GL_VERTEX_ARRAY_BINDING_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_APPLE ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_LENGTH_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_STORAGE_HINT_APPLE ,range  );
std::cout << "GL_VERTEX_ARRAY_STORAGE_HINT_APPLE" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE ,range  );
std::cout << "GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_POINTER_APPLE ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_POINTER_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP1_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP1_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP2_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP2_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP1_SIZE_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP1_COEFF_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP1_ORDER_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP1_DOMAIN_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP2_SIZE_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP2_COEFF_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP2_ORDER_APPLE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE ,range  );
std::cout << "GL_VERTEX_ATTRIB_MAP2_DOMAIN_APPLE" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_UNIFORM_VECTORS ,range  );
std::cout << "GL_MAX_VERTEX_UNIFORM_VECTORS" << range[0] << std::endl;

glGetFloatv( GL_CLAMP_VERTEX_COLOR_ARB ,range  );
std::cout << "GL_CLAMP_VERTEX_COLOR_ARB" << range[0] << std::endl;

glGetFloatv( GL_GEOMETRY_VERTICES_OUT_ARB ,range  );
std::cout << "GL_GEOMETRY_VERTICES_OUT_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_VARYING_COMPONENTS_ARB ,range  );
std::cout << "GL_MAX_VERTEX_VARYING_COMPONENTS_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB ,range  );
std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_STREAMS ,range  );
std::cout << "GL_MAX_VERTEX_STREAMS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_TEXTURE ,range  );
std::cout << "GL_VERTEX_TEXTURE" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_VIEW ,range  );
std::cout << "GL_TEXTURE_VIEW" << range[0] << std::endl;

glGetFloatv( GL_VIEW_COMPATIBILITY_CLASS ,range  );
std::cout << "GL_VIEW_COMPATIBILITY_CLASS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_128_BITS ,range  );
std::cout << "GL_VIEW_CLASS_128_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_96_BITS ,range  );
std::cout << "GL_VIEW_CLASS_96_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_64_BITS ,range  );
std::cout << "GL_VIEW_CLASS_64_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_48_BITS ,range  );
std::cout << "GL_VIEW_CLASS_48_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_32_BITS ,range  );
std::cout << "GL_VIEW_CLASS_32_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_24_BITS ,range  );
std::cout << "GL_VIEW_CLASS_24_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_16_BITS ,range  );
std::cout << "GL_VIEW_CLASS_16_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_8_BITS ,range  );
std::cout << "GL_VIEW_CLASS_8_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_S3TC_DXT1_RGB ,range  );
std::cout << "GL_VIEW_CLASS_S3TC_DXT1_RGB" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_S3TC_DXT1_RGBA ,range  );
std::cout << "GL_VIEW_CLASS_S3TC_DXT1_RGBA" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_S3TC_DXT3_RGBA ,range  );
std::cout << "GL_VIEW_CLASS_S3TC_DXT3_RGBA" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_S3TC_DXT5_RGBA ,range  );
std::cout << "GL_VIEW_CLASS_S3TC_DXT5_RGBA" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_RGTC1_RED ,range  );
std::cout << "GL_VIEW_CLASS_RGTC1_RED" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_RGTC2_RG ,range  );
std::cout << "GL_VIEW_CLASS_RGTC2_RG" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_BPTC_UNORM ,range  );
std::cout << "GL_VIEW_CLASS_BPTC_UNORM" << range[0] << std::endl;

glGetFloatv( GL_VIEW_CLASS_BPTC_FLOAT ,range  );
std::cout << "GL_VIEW_CLASS_BPTC_FLOAT" << range[0] << std::endl;

glGetFloatv( GL_SAMPLE_COVERAGE_INVERT_ARB ,range  );
std::cout << "GL_SAMPLE_COVERAGE_INVERT_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SUBROUTINE ,range  );
std::cout << "GL_VERTEX_SUBROUTINE" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SUBROUTINE_UNIFORM ,range  );
std::cout << "GL_VERTEX_SUBROUTINE_UNIFORM" << range[0] << std::endl;

glGetFloatv( GL_REFERENCED_BY_VERTEX_SHADER ,range  );
std::cout << "GL_REFERENCED_BY_VERTEX_SHADER" << range[0] << std::endl;

glGetFloatv( GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION ,range  );
std::cout << "GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION" << range[0] << std::endl;

glGetFloatv( GL_FIRST_VERTEX_CONVENTION ,range  );
std::cout << "GL_FIRST_VERTEX_CONVENTION" << range[0] << std::endl;

glGetFloatv( GL_LAST_VERTEX_CONVENTION ,range  );
std::cout << "GL_LAST_VERTEX_CONVENTION" << range[0] << std::endl;

glGetFloatv( GL_PROVOKING_VERTEX ,range  );
std::cout << "GL_PROVOKING_VERTEX" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_BIT ,range  );
std::cout << "GL_VERTEX_SHADER_BIT" << range[0] << std::endl;

glGetFloatv( GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER ,range  );
std::cout << "GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS ,range  );
std::cout << "GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATOMIC_COUNTERS ,range  );
std::cout << "GL_MAX_VERTEX_ATOMIC_COUNTERS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_IMAGE_UNIFORMS ,range  );
std::cout << "GL_MAX_VERTEX_IMAGE_UNIFORMS" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS" << range[0] << std::endl;

glGetFloatv( GL_PATCH_VERTICES ,range  );
std::cout << "GL_PATCH_VERTICES" << range[0] << std::endl;

glGetFloatv( GL_TESS_CONTROL_OUTPUT_VERTICES ,range  );
std::cout << "GL_TESS_CONTROL_OUTPUT_VERTICES" << range[0] << std::endl;

glGetFloatv( GL_TESS_GEN_VERTEX_ORDER ,range  );
std::cout << "GL_TESS_GEN_VERTEX_ORDER" << range[0] << std::endl;

glGetFloatv( GL_MAX_PATCH_VERTICES ,range  );
std::cout << "GL_MAX_PATCH_VERTICES" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_VIEW_MIN_LEVEL ,range  );
std::cout << "GL_TEXTURE_VIEW_MIN_LEVEL" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_VIEW_NUM_LEVELS ,range  );
std::cout << "GL_TEXTURE_VIEW_NUM_LEVELS" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_VIEW_MIN_LAYER ,range  );
std::cout << "GL_TEXTURE_VIEW_MIN_LAYER" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_VIEW_NUM_LAYERS ,range  );
std::cout << "GL_TEXTURE_VIEW_NUM_LAYERS" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_STREAMS ,range  );
std::cout << "GL_MAX_VERTEX_STREAMS" << range[0] << std::endl;

glGetFloatv( GL_TRANSPOSE_MODELVIEW_MATRIX_ARB ,range  );
std::cout << "GL_TRANSPOSE_MODELVIEW_MATRIX_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_UNIFORM_BLOCKS ,range  );
std::cout << "GL_MAX_VERTEX_UNIFORM_BLOCKS" << range[0] << std::endl;

glGetFloatv( GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS ,range  );
std::cout << "GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS" << range[0] << std::endl;

glGetFloatv( GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER ,range  );
std::cout << "GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_BINDING ,range  );
std::cout << "GL_VERTEX_ARRAY_BINDING" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_BINDING ,range  );
std::cout << "GL_VERTEX_ATTRIB_BINDING" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_RELATIVE_OFFSET ,range  );
std::cout << "GL_VERTEX_ATTRIB_RELATIVE_OFFSET" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_BINDING_DIVISOR ,range  );
std::cout << "GL_VERTEX_BINDING_DIVISOR" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_BINDING_OFFSET ,range  );
std::cout << "GL_VERTEX_BINDING_OFFSET" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_BINDING_STRIDE ,range  );
std::cout << "GL_VERTEX_BINDING_STRIDE" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET ,range  );
std::cout << "GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATTRIB_BINDINGS ,range  );
std::cout << "GL_MAX_VERTEX_ATTRIB_BINDINGS" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW0_ARB ,range  );
std::cout << "GL_MODELVIEW0_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW1_ARB ,range  );
std::cout << "GL_MODELVIEW1_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_UNITS_ARB ,range  );
std::cout << "GL_MAX_VERTEX_UNITS_ARB" << range[0] << std::endl;

glGetFloatv( GL_ACTIVE_VERTEX_UNITS_ARB ,range  );
std::cout << "GL_ACTIVE_VERTEX_UNITS_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_BLEND_ARB ,range  );
std::cout << "GL_VERTEX_BLEND_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW2_ARB ,range  );
std::cout << "GL_MODELVIEW2_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW3_ARB ,range  );
std::cout << "GL_MODELVIEW3_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW4_ARB ,range  );
std::cout << "GL_MODELVIEW4_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW5_ARB ,range  );
std::cout << "GL_MODELVIEW5_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW6_ARB ,range  );
std::cout << "GL_MODELVIEW6_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW7_ARB ,range  );
std::cout << "GL_MODELVIEW7_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW8_ARB ,range  );
std::cout << "GL_MODELVIEW8_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW9_ARB ,range  );
std::cout << "GL_MODELVIEW9_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW10_ARB ,range  );
std::cout << "GL_MODELVIEW10_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW11_ARB ,range  );
std::cout << "GL_MODELVIEW11_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW12_ARB ,range  );
std::cout << "GL_MODELVIEW12_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW13_ARB ,range  );
std::cout << "GL_MODELVIEW13_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW14_ARB ,range  );
std::cout << "GL_MODELVIEW14_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW15_ARB ,range  );
std::cout << "GL_MODELVIEW15_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW16_ARB ,range  );
std::cout << "GL_MODELVIEW16_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW17_ARB ,range  );
std::cout << "GL_MODELVIEW17_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW18_ARB ,range  );
std::cout << "GL_MODELVIEW18_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW19_ARB ,range  );
std::cout << "GL_MODELVIEW19_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW20_ARB ,range  );
std::cout << "GL_MODELVIEW20_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW21_ARB ,range  );
std::cout << "GL_MODELVIEW21_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW22_ARB ,range  );
std::cout << "GL_MODELVIEW22_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW23_ARB ,range  );
std::cout << "GL_MODELVIEW23_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW24_ARB ,range  );
std::cout << "GL_MODELVIEW24_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW25_ARB ,range  );
std::cout << "GL_MODELVIEW25_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW26_ARB ,range  );
std::cout << "GL_MODELVIEW26_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW27_ARB ,range  );
std::cout << "GL_MODELVIEW27_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW28_ARB ,range  );
std::cout << "GL_MODELVIEW28_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW29_ARB ,range  );
std::cout << "GL_MODELVIEW29_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW30_ARB ,range  );
std::cout << "GL_MODELVIEW30_ARB" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW31_ARB ,range  );
std::cout << "GL_MODELVIEW31_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_BUFFER_BINDING_ARB ,range  );
std::cout << "GL_VERTEX_ARRAY_BUFFER_BINDING_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_ARB ,range  );
std::cout << "GL_VERTEX_PROGRAM_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB" << range[0] << std::endl;

glGetFloatv( GL_CURRENT_VERTEX_ATTRIB_ARB ,range  );
std::cout << "GL_CURRENT_VERTEX_ATTRIB_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_POINT_SIZE_ARB ,range  );
std::cout << "GL_VERTEX_PROGRAM_POINT_SIZE_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_TWO_SIDE_ARB ,range  );
std::cout << "GL_VERTEX_PROGRAM_TWO_SIDE_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ATTRIBS_ARB ,range  );
std::cout << "GL_MAX_VERTEX_ATTRIBS_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_ARB ,range  );
std::cout << "GL_VERTEX_SHADER_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB ,range  );
std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB ,range  );
std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT ,range  );
std::cout << "GL_VIEWPORT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VIEWPORTS ,range  );
std::cout << "GL_MAX_VIEWPORTS" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT_SUBPIXEL_BITS ,range  );
std::cout << "GL_VIEWPORT_SUBPIXEL_BITS" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT_BOUNDS_RANGE ,range  );
std::cout << "GL_VIEWPORT_BOUNDS_RANGE" << range[0] << std::endl;

glGetFloatv( GL_LAYER_PROVOKING_VERTEX ,range  );
std::cout << "GL_LAYER_PROVOKING_VERTEX" << range[0] << std::endl;

glGetFloatv( GL_VIEWPORT_INDEX_PROVOKING_VERTEX ,range  );
std::cout << "GL_VIEWPORT_INDEX_PROVOKING_VERTEX" << range[0] << std::endl;

glGetFloatv( GL_UNDEFINED_VERTEX ,range  );
std::cout << "GL_UNDEFINED_VERTEX" << range[0] << std::endl;

glGetFloatv( GL_FIRST_VERTEX_CONVENTION ,range  );
std::cout << "GL_FIRST_VERTEX_CONVENTION" << range[0] << std::endl;

glGetFloatv( GL_LAST_VERTEX_CONVENTION ,range  );
std::cout << "GL_LAST_VERTEX_CONVENTION" << range[0] << std::endl;

glGetFloatv( GL_PROVOKING_VERTEX ,range  );
std::cout << "GL_PROVOKING_VERTEX" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_STREAMS_ATI ,range  );
std::cout << "GL_MAX_VERTEX_STREAMS_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SOURCE_ATI ,range  );
std::cout << "GL_VERTEX_SOURCE_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM0_ATI ,range  );
std::cout << "GL_VERTEX_STREAM0_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM1_ATI ,range  );
std::cout << "GL_VERTEX_STREAM1_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM2_ATI ,range  );
std::cout << "GL_VERTEX_STREAM2_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM3_ATI ,range  );
std::cout << "GL_VERTEX_STREAM3_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM4_ATI ,range  );
std::cout << "GL_VERTEX_STREAM4_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM5_ATI ,range  );
std::cout << "GL_VERTEX_STREAM5_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM6_ATI ,range  );
std::cout << "GL_VERTEX_STREAM6_ATI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STREAM7_ATI ,range  );
std::cout << "GL_VERTEX_STREAM7_ATI" << range[0] << std::endl;

glGetFloatv( GL_CG_VERTEX_SHADER_EXT ,range  );
std::cout << "GL_CG_VERTEX_SHADER_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT" << range[0] << std::endl;

glGetFloatv( GL_CULL_VERTEX_EXT ,range  );
std::cout << "GL_CULL_VERTEX_EXT" << range[0] << std::endl;

glGetFloatv( GL_CULL_VERTEX_EYE_POSITION_EXT ,range  );
std::cout << "GL_CULL_VERTEX_EYE_POSITION_EXT" << range[0] << std::endl;

glGetFloatv( GL_CULL_VERTEX_OBJECT_POSITION_EXT ,range  );
std::cout << "GL_CULL_VERTEX_OBJECT_POSITION_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_ELEMENTS_VERTICES_EXT ,range  );
std::cout << "GL_MAX_ELEMENTS_VERTICES_EXT" << range[0] << std::endl;

glGetFloatv( GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT ,range  );
std::cout << "GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_EXT" << range[0] << std::endl;

glGetFloatv( GL_GEOMETRY_VERTICES_OUT_EXT ,range  );
std::cout << "GL_GEOMETRY_VERTICES_OUT_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_VARYING_COMPONENTS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_VARYING_COMPONENTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT ,range  );
std::cout << "GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT" << range[0] << std::endl;

glGetFloatv( GL_SAMPLE_MASK_INVERT_EXT ,range  );
std::cout << "GL_SAMPLE_MASK_INVERT_EXT" << range[0] << std::endl;

glGetFloatv( GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT ,range  );
std::cout << "GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION_EXT" << range[0] << std::endl;

glGetFloatv( GL_FIRST_VERTEX_CONVENTION_EXT ,range  );
std::cout << "GL_FIRST_VERTEX_CONVENTION_EXT" << range[0] << std::endl;

glGetFloatv( GL_LAST_VERTEX_CONVENTION_EXT ,range  );
std::cout << "GL_LAST_VERTEX_CONVENTION_EXT" << range[0] << std::endl;

glGetFloatv( GL_PROVOKING_VERTEX_EXT ,range  );
std::cout << "GL_PROVOKING_VERTEX_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_SIZE_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_SIZE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_TYPE_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_TYPE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_STRIDE_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_STRIDE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_COUNT_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_COUNT_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_POINTER_EXT ,range  );
std::cout << "GL_VERTEX_ARRAY_POINTER_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_BINDING_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_BINDING_EXT" << range[0] << std::endl;

glGetFloatv( GL_OUTPUT_VERTEX_EXT ,range  );
std::cout << "GL_OUTPUT_VERTEX_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_INSTRUCTIONS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_VARIANTS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_VARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_INVARIANTS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_INVARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_LOCAL_CONSTANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_SHADER_LOCALS_EXT ,range  );
std::cout << "GL_MAX_VERTEX_SHADER_LOCALS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT ,range  );
std::cout << "GL_MAX_OPTIMIZED_VERTEX_SHADER_INSTRUCTIONS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT ,range  );
std::cout << "GL_MAX_OPTIMIZED_VERTEX_SHADER_VARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT ,range  );
std::cout << "GL_MAX_OPTIMIZED_VERTEX_SHADER_INVARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT ,range  );
std::cout << "GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCAL_CONSTANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT ,range  );
std::cout << "GL_MAX_OPTIMIZED_VERTEX_SHADER_LOCALS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_INSTRUCTIONS_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_INSTRUCTIONS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_VARIANTS_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_VARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_INVARIANTS_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_INVARIANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_LOCAL_CONSTANTS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_LOCALS_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_LOCALS_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_SHADER_OPTIMIZED_EXT ,range  );
std::cout << "GL_VERTEX_SHADER_OPTIMIZED_EXT" << range[0] << std::endl;

glGetFloatv( GL_CURRENT_VERTEX_EXT ,range  );
std::cout << "GL_CURRENT_VERTEX_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW0_STACK_DEPTH_EXT ,range  );
std::cout << "GL_MODELVIEW0_STACK_DEPTH_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW0_MATRIX_EXT ,range  );
std::cout << "GL_MODELVIEW0_MATRIX_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW0_EXT ,range  );
std::cout << "GL_MODELVIEW0_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW1_STACK_DEPTH_EXT ,range  );
std::cout << "GL_MODELVIEW1_STACK_DEPTH_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW1_MATRIX_EXT ,range  );
std::cout << "GL_MODELVIEW1_MATRIX_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHTING_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHTING_EXT" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW1_EXT ,range  );
std::cout << "GL_MODELVIEW1_EXT" << range[0] << std::endl;

glGetFloatv( GL_CURRENT_VERTEX_WEIGHT_EXT ,range  );
std::cout << "GL_CURRENT_VERTEX_WEIGHT_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHT_ARRAY_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHT_ARRAY_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHT_ARRAY_SIZE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHT_ARRAY_TYPE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHT_ARRAY_STRIDE_EXT" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT ,range  );
std::cout << "GL_VERTEX_WEIGHT_ARRAY_POINTER_EXT" << range[0] << std::endl;

glGetFloatv( GL_CULL_VERTEX_IBM ,range  );
std::cout << "GL_CULL_VERTEX_IBM" << range[0] << std::endl;

glGetFloatv( GL_STATIC_VERTEX_ARRAY_IBM ,range  );
std::cout << "GL_STATIC_VERTEX_ARRAY_IBM" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_LIST_IBM ,range  );
std::cout << "GL_VERTEX_ARRAY_LIST_IBM" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_LIST_STRIDE_IBM ,range  );
std::cout << "GL_VERTEX_ARRAY_LIST_STRIDE_IBM" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL ,range  );
std::cout << "GL_VERTEX_ARRAY_PARALLEL_POINTERS_INTEL" << range[0] << std::endl;

glGetFloatv( GL_PACK_INVERT_MESA ,range  );
std::cout << "GL_PACK_INVERT_MESA" << range[0] << std::endl;

glGetFloatv( GL_INVERT_RGB_NV ,range  );
std::cout << "GL_INVERT_RGB_NV" << range[0] << std::endl;

glGetFloatv( GL_INVERT_OVG_NV ,range  );
std::cout << "GL_INVERT_OVG_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB0_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB0_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB1_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB1_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB2_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB2_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB3_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB3_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB4_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB4_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB5_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB5_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB6_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB6_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB7_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB7_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB8_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB8_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB9_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB9_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB10_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB10_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB11_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB11_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB12_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB12_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB13_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB13_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB14_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB14_NV" << range[0] << std::endl;

glGetFloatv( GL_EVAL_VERTEX_ATTRIB15_NV ,range  );
std::cout << "GL_EVAL_VERTEX_ATTRIB15_NV" << range[0] << std::endl;

glGetFloatv( GL_MAX_PROGRAM_OUTPUT_VERTICES_NV ,range  );
std::cout << "GL_MAX_PROGRAM_OUTPUT_VERTICES_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV ,range  );
std::cout << "GL_VERTEX_PROGRAM_PARAMETER_BUFFER_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTICAL_LINE_TO_NV ,range  );
std::cout << "GL_VERTICAL_LINE_TO_NV" << range[0] << std::endl;

glGetFloatv( GL_RELATIVE_VERTICAL_LINE_TO_NV ,range  );
std::cout << "GL_RELATIVE_VERTICAL_LINE_TO_NV" << range[0] << std::endl;

glGetFloatv( GL_GLYPH_VERTICAL_BEARING_X_BIT_NV ,range  );
std::cout << "GL_GLYPH_VERTICAL_BEARING_X_BIT_NV" << range[0] << std::endl;

glGetFloatv( GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV ,range  );
std::cout << "GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV" << range[0] << std::endl;

glGetFloatv( GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV ,range  );
std::cout << "GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV" << range[0] << std::endl;

glGetFloatv( GL_MITER_REVERT_NV ,range  );
std::cout << "GL_MITER_REVERT_NV" << range[0] << std::endl;

glGetFloatv( GL_UNSIGNED_INVERT_NV ,range  );
std::cout << "GL_UNSIGNED_INVERT_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ID_NV ,range  );
std::cout << "GL_VERTEX_ID_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_LENGTH_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_LENGTH_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_VALID_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_VALID_NV" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV ,range  );
std::cout << "GL_MAX_VERTEX_ARRAY_RANGE_ELEMENT_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_POINTER_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_POINTER_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_RANGE_WITHOUT_FLUSH_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_UNIFIED_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_ADDRESS_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_ADDRESS_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_ADDRESS_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_LENGTH_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ARRAY_LENGTH_NV ,range  );
std::cout << "GL_VERTEX_ARRAY_LENGTH_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_NV ,range  );
std::cout << "GL_VERTEX_PROGRAM_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_STATE_PROGRAM_NV ,range  );
std::cout << "GL_VERTEX_STATE_PROGRAM_NV" << range[0] << std::endl;

glGetFloatv( GL_MODELVIEW_PROJECTION_NV ,range  );
std::cout << "GL_MODELVIEW_PROJECTION_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_POINT_SIZE_NV ,range  );
std::cout << "GL_VERTEX_PROGRAM_POINT_SIZE_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_TWO_SIDE_NV ,range  );
std::cout << "GL_VERTEX_PROGRAM_TWO_SIDE_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PROGRAM_BINDING_NV ,range  );
std::cout << "GL_VERTEX_PROGRAM_BINDING_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY0_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY0_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY1_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY1_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY2_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY2_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY3_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY3_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY4_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY4_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY5_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY5_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY6_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY6_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY7_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY7_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY8_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY8_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY9_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY9_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY10_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY10_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY11_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY11_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY12_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY12_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY13_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY13_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY14_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY14_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY15_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY15_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB0_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB0_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB1_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB1_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB2_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB2_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB3_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB3_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB4_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB4_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB5_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB5_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB6_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB6_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB7_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB7_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB8_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB8_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB9_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB9_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB10_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB10_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB11_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB11_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB12_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB12_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB13_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB13_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB14_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB14_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP1_VERTEX_ATTRIB15_4_NV ,range  );
std::cout << "GL_MAP1_VERTEX_ATTRIB15_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB0_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB0_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB1_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB1_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB2_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB2_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB3_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB3_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB4_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB4_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB5_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB5_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB6_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB6_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB7_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB7_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB8_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB8_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB9_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB9_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB10_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB10_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB11_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB11_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB12_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB12_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB13_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB13_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB14_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB14_4_NV" << range[0] << std::endl;

glGetFloatv( GL_MAP2_VERTEX_ATTRIB15_4_NV ,range  );
std::cout << "GL_MAP2_VERTEX_ATTRIB15_4_NV" << range[0] << std::endl;

glGetFloatv( MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB ,range  );
std::cout << "MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV ,range  );
std::cout << "GL_VERTEX_ATTRIB_ARRAY_INTEGER_NV" << range[0] << std::endl;

glGetFloatv( GL_VERTEX23_BIT_PGI ,range  );
std::cout << "GL_VERTEX23_BIT_PGI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX4_BIT_PGI ,range  );
std::cout << "GL_VERTEX4_BIT_PGI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_DATA_HINT_PGI ,range  );
std::cout << "GL_VERTEX_DATA_HINT_PGI" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_CONSISTENT_HINT_PGI ,range  );
std::cout << "GL_VERTEX_CONSISTENT_HINT_PGI" << range[0] << std::endl;

glGetFloatv( GL_MAX_VERTEX_HINT_PGI ,range  );
std::cout << "GL_MAX_VERTEX_HINT_PGI" << range[0] << std::endl;

glGetFloatv( GL_INVERTED_SCREEN_W_REND ,range  );
std::cout << "GL_INVERTED_SCREEN_W_REND" << range[0] << std::endl;

glGetFloatv( GL_SAMPLE_MASK_INVERT_SGIS ,range  );
std::cout << "GL_SAMPLE_MASK_INVERT_SGIS" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PRECLIP_SGIX ,range  );
std::cout << "GL_VERTEX_PRECLIP_SGIX" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PRECLIP_HINT_SGIX ,range  );
std::cout << "GL_VERTEX_PRECLIP_HINT_SGIX" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PRECLIP_SGIX ,range  );
std::cout << "GL_VERTEX_PRECLIP_SGIX" << range[0] << std::endl;

glGetFloatv( GL_VERTEX_PRECLIP_HINT_SGIX ,range  );
std::cout << "GL_VERTEX_PRECLIP_HINT_SGIX" << range[0] << std::endl;

}
void getStenc(){
//�X�e���V���o�b�t�@���N���A����̂�
/*    glGetFloatv( GL_STENCIL_BUFFER_BIT ,range  );
std::cout << "GL_STENCIL_BUFFER_BIT :" << range[0] << std::endl*/;
//glReadPixels

glGetFloatv( GL_STENCIL_TEST ,range  );
std::cout << "GL_STENCIL_TEST :" << range[0] << std::endl;
isErr("stest",__LINE__);
glGetFloatv( GL_STENCIL_CLEAR_VALUE ,range  );
std::cout << "GL_STENCIL_CLEAR_VALUE :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_FUNC ,range  );
std::cout << "GL_STENCIL_FUNC :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_VALUE_MASK ,range  );
std::cout << "GL_STENCIL_VALUE_MASK :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_FAIL ,range  );
std::cout << "GL_STENCIL_FAIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_PASS_DEPTH_FAIL ,range  );
std::cout << "GL_STENCIL_PASS_DEPTH_FAIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_PASS_DEPTH_PASS ,range  );
std::cout << "GL_STENCIL_PASS_DEPTH_PASS :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_REF ,range  );
std::cout << "GL_STENCIL_REF :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_WRITEMASK ,range  );
std::cout << "GL_STENCIL_WRITEMASK :" << range[0] << std::endl;

glGetFloatv( GL_MAP_STENCIL ,range  );
std::cout << "GL_MAP_STENCIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BITS ,range  );
std::cout << "GL_STENCIL_BITS :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL ,range  );
std::cout << "GL_STENCIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX ,range  );
std::cout << "GL_STENCIL_INDEX :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_FUNC ,range  );
std::cout << "GL_STENCIL_BACK_FUNC :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_FAIL ,range  );
std::cout << "GL_STENCIL_BACK_FAIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_PASS_DEPTH_FAIL ,range  );
std::cout << "GL_STENCIL_BACK_PASS_DEPTH_FAIL :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_PASS_DEPTH_PASS ,range  );
std::cout << "GL_STENCIL_BACK_PASS_DEPTH_PASS :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_REF ,range  );
std::cout << "GL_STENCIL_BACK_REF :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_VALUE_MASK ,range  );
std::cout << "GL_STENCIL_BACK_VALUE_MASK :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_WRITEMASK ,range  );
std::cout << "GL_STENCIL_BACK_WRITEMASK :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BUFFER ,range  );
std::cout << "GL_STENCIL_BUFFER :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_OP_VALUE_AMD ,range  );
std::cout << "GL_STENCIL_OP_VALUE_AMD :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_OP_VALUE_AMD ,range  );
std::cout << "GL_STENCIL_BACK_OP_VALUE_AMD :" << range[0] << std::endl;

glGetFloatv( GL_AUX_DEPTH_STENCIL_APPLE ,range  );
std::cout << "GL_AUX_DEPTH_STENCIL_APPLE :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH32F_STENCIL8 ,range  );
std::cout << "GL_DEPTH32F_STENCIL8 :" << range[0] << std::endl;

glGetFloatv( GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE ,range  );
std::cout << "GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH_STENCIL_ATTACHMENT ,range  );
std::cout << "GL_DEPTH_STENCIL_ATTACHMENT :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH_STENCIL ,range  );
std::cout << "GL_DEPTH_STENCIL :" << range[0] << std::endl;
isErr("depth_stencil",__LINE__);
glGetFloatv( GL_DEPTH24_STENCIL8 ,range  );
std::cout << "GL_DEPTH24_STENCIL8 :" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_STENCIL_SIZE ,range  );
std::cout << "GL_TEXTURE_STENCIL_SIZE :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_ATTACHMENT ,range  );
std::cout << "GL_STENCIL_ATTACHMENT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX1 ,range  );
std::cout << "GL_STENCIL_INDEX1 :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX4 ,range  );
std::cout << "GL_STENCIL_INDEX4 :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX8 ,range  );
std::cout << "GL_STENCIL_INDEX8 :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX16 ,range  );
std::cout << "GL_STENCIL_INDEX16 :" << range[0] << std::endl;

glGetFloatv( GL_RENDERBUFFER_STENCIL_SIZE ,range  );
std::cout << "GL_RENDERBUFFER_STENCIL_SIZE :" << range[0] << std::endl;

glGetFloatv( GL_INTERNALFORMAT_STENCIL_SIZE ,range  );
std::cout << "GL_INTERNALFORMAT_STENCIL_SIZE :" << range[0] << std::endl;

glGetFloatv( GL_INTERNALFORMAT_STENCIL_TYPE ,range  );
std::cout << "GL_INTERNALFORMAT_STENCIL_TYPE :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_COMPONENTS ,range  );
std::cout << "GL_STENCIL_COMPONENTS :" << range[0] << std::endl;
isErr("stencil comp ",__LINE__);
glGetFloatv( GL_STENCIL_RENDERABLE ,range  );
std::cout << "GL_STENCIL_RENDERABLE :" << range[0] << std::endl;

glGetFloatv( GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST ,range  );
std::cout << "GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST :" << range[0] << std::endl;

glGetFloatv( GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE ,range  );
std::cout << "GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH_STENCIL_TEXTURE_MODE ,range  );
std::cout << "GL_DEPTH_STENCIL_TEXTURE_MODE :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX ,range  );
std::cout << "GL_STENCIL_INDEX :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX8 ,range  );
std::cout << "GL_STENCIL_INDEX8 :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_FUNC_ATI ,range  );
std::cout << "GL_STENCIL_BACK_FUNC_ATI :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_FAIL_ATI ,range  );
std::cout << "GL_STENCIL_BACK_FAIL_ATI :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI ,range  );
std::cout << "GL_STENCIL_BACK_PASS_DEPTH_FAIL_ATI :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI ,range  );
std::cout << "GL_STENCIL_BACK_PASS_DEPTH_PASS_ATI :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_ATTACHMENT_EXT ,range  );
std::cout << "GL_STENCIL_ATTACHMENT_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX1_EXT ,range  );
std::cout << "GL_STENCIL_INDEX1_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX4_EXT ,range  );
std::cout << "GL_STENCIL_INDEX4_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX8_EXT ,range  );
std::cout << "GL_STENCIL_INDEX8_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_INDEX16_EXT ,range  );
std::cout << "GL_STENCIL_INDEX16_EXT :" << range[0] << std::endl;

glGetFloatv( GL_RENDERBUFFER_STENCIL_SIZE_EXT ,range  );
std::cout << "GL_RENDERBUFFER_STENCIL_SIZE_EXT :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH_STENCIL_EXT ,range  );
std::cout << "GL_DEPTH_STENCIL_EXT :" << range[0] << std::endl;

glGetFloatv( GL_DEPTH24_STENCIL8_EXT ,range  );
std::cout << "GL_DEPTH24_STENCIL8_EXT :" << range[0] << std::endl;

glGetFloatv( GL_TEXTURE_STENCIL_SIZE_EXT ,range  );
std::cout << "GL_TEXTURE_STENCIL_SIZE_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_TAG_BITS_EXT ,range  );
std::cout << "GL_STENCIL_TAG_BITS_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_CLEAR_TAG_VALUE_EXT ,range  );
std::cout << "GL_STENCIL_CLEAR_TAG_VALUE_EXT :" << range[0] << std::endl;

glGetFloatv( GL_STENCIL_TEST_TWO_SIDE_EXT ,range  );
std::cout << "GL_STENCIL_TEST_TWO_SIDE_EXT :" << range[0] << std::endl;

glGetFloatv( GL_ACTIVE_STENCIL_FACE_EXT ,range  );
std::cout << "GL_ACTIVE_STENCIL_FACE_EXT :" << range[0] << std::endl;

glGetFloatv( GL_KTX_STENCIL_REGION ,range  );
std::cout << "GL_KTX_STENCIL_REGION :" << range[0] << std::endl;

}
GLenum Renderer::renderPolygon(GLvoid* vert,int length){
  //setSphereMap(); 
  //ind size type norm? stride poi ind�͒��_�C���f�b�N�X�Asize�͒��_���A���_�̌^������float,false,
  auto ty=GL_FLOAT;
  //glVertexAttribPointer(i,2,ty,false,
  //auto si=sizeof(
  //glColorPointer(i,ty,si,
  //http://www57.atwiki.jp/gametips/m/pages/29.html?guid=on attrivpointer�̘b
  return glGetError();
}

auto stagecolor = new float*[]{
    new float[]{ 0.9f, 0.9f, 1.0f },
    new float[]{0.2f, 0.1f, 0.6f},
    new float[]{ 0.3f, 0.3f, 0.3f },
    new float[]{0.8f, 0.5f, 0.2f},
};
void Renderer::SetStage(Stages s){
  testTex->SetVertexColor2(stagecolor[s*2], stagecolor[s*2 + 1]);
}
//�I�t�X�N���[�������_�@�X�e���V���o�b�t�@�@�L���[�u�}�b�v

void renderCircle(){
  //stride���炵��glPointer����Ύ���
}
void stencilInit(){
  //stencilfunc 519=always

  glEnable(GL_STENCIL_TEST);
  //�K���s���i�ɂȂ�l�p�̋�Ԃ��쐬���āA�s���i�r�b�g�̂Ƃ���ɂ͎w�胂�f���`����s��Ȃ��悤�ɂ���
}
//ref�Ɣ�ׂđ傫���X�e���V���̂ݕ\��
void stencil(GLint refV){
    //�X�e���V�����ʂ�Ȃ������Ƃ�
    GLenum stencilFail=GL_ZERO;
    GLenum depthFail=GL_KEEP; //���Ԃ�depth�ɂ�����炸���삵�������Ƃ�
    GLenum stencilSuccess=GL_KEEP;
    //GL_KEEP : ���݂̒l��ێ�����B
    //GL_ZERO : �l���[���Ƃ���B
    //GL_REPLACE : �l�� glStencilFunc() �̈����� ref �̒l�ɂ���B
    //GL_INCR : ���݂̒l��+1����B
    //GL_INCR_WRAP : ���ݒl+1���邪�A�ő�l�𒴂���ꍇ�̓[���ɂ���B
    //GL_DECR : ���݂̒l��-1����B�[���ȏ�ɕ␳�����B
    //GL_DECR_WRAP : ���ݒl-1���邪�A�[���ȉ��ɂȂ�Ȃ�ő�l�ɂ���B
    //GL_INVERT : ���݂̒l���r�b�g���Z�Ŕ��]�������l�ɂ���B

    //�ʂ������A�ʂ�Ȃ������Ƃ��ɑ��삷�邱�ƂŁA������z�o�b�t�@�݂����Ȃ̂�������
    glStencilOp(stencilFail,depthFail,stencilSuccess); 
    //�f�v�X���ʂ�Ȃ��������A�f�v�X���ʂ������������A���񂴂����Ȃ�
    //�|���S�����Ƃɂǂ������e�X�g���s�����w��ł���
    //GL_NEVER�F���ׂĕs���i
    //GL_LESS�F( ref & mask ) < ( stencil & mask ) �ō��i
    //GL_LEQUAL�F( ref & mask ) <= ( stencil & mask ) �ō��i
    //GL_GREATER�F( ref & mask ) > ( stencil & mask ) �ō��i
    //GL_GEQUAL�F( ref & mask ) >= ( stencil & mask ) �ō��i
    //GL_EQUAL�F( ref & mask ) = ( stencil & mask ) �ō��i
    //GL_NOTEQUAL�F( ref & mask ) != ( stencil & mask ) �ō��i
    //GL_ALWAYS�F���ׂč��i
    //��[&]�̓r�b�g�P�ʂ�AND���Z
    GLuint mask=0x11111111;
    glStencilFunc(GL_GREATER,refV,mask);//�O�p�̂ƐF�X������̂���낤
}
  GLuint alwaysmask=0xFF;



//drawSprite() 
  //�}�X�N���ꂽ�ꏊ����������悤�ɂ���Α�h�̃G�t�F�N�g�A�䂽�Ă��ۂ��ǂŗV�ڂ�

//setMask(1);//�X�e���V���ɑI�т����|���S���͏����h�̂͂�
//drawYuta();
//setPoly(2);//�X�e���V���l
//drawTexture()
GLboolean F=GL_FALSE;

void setMask(GLint refV){
  auto rep=GL_REPLACE;
  //glColorMask(F,F,F,F);
  //glDepthMask(F);
  glStencilOp(rep,rep,rep);
  glStencilFunc(GL_NEVER,refV,alwaysmask);
  //glStencilMask(0);
}

void setPoly(GLint refV){
  auto keep=GL_KEEP;
  auto rep=GL_INCR;
  glStencilOp(keep,keep,keep);
  glStencilFunc(GL_GEQUAL,refV,alwaysmask);
}
const int cubeTexSize=128;
void loadCubeTexture(){
 static const char *textures[] = {          /* �e�N�X�`���t�@�C���� */
  "room2nx.raw",
  "room2ny.raw",
  "room2nz.raw",
  "room2px.raw",
  "room2py.raw",
  "room2pz.raw",
};
 static const GLenum target[] = {       /* �e�N�X�`���̃^�[�Q�b�g�� */
  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
 };
    /* �e�N�X�`���摜�̓��[�h�P�ʂɋl�ߍ��܂�Ă��� */
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  
  for (int i = 0; i < 6; ++i) {
    /* �e�N�X�`���̓ǂݍ��݂Ɏg���z�� */
    GLubyte texture[cubeTexSize][cubeTexSize][4];
    FILE *fp;
    errno_t t=(fopen_s(&fp,textures[i], "rb"));
    /* �e�N�X�`���摜�̓ǂݍ��� */
    if (t == NOERROR) {
      fread(texture, sizeof texture, 1, fp);
      fclose(fp);
    }
    else {
      perror(textures[i]);
    }
  
   /* �e�N�X�`���̊��蓖�� */
    glTexImage2D(target[i], 0, GL_RGBA, cubeTexSize, cubeTexSize, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, texture);
  }
  
  /* �e�N�X�`�����g��E�k��������@�̎w�� */
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
void setUtsurikomi(){
    //�X�t�B�A�}�b�v�̂ق����F�X�y�����A���ʂɎア,�X�N���[���Ɛ����ʂŕ���邻����
}

Renderer::~Renderer(void)
{
    delete testTex;
    //GLUI_Master.close_all();
}
