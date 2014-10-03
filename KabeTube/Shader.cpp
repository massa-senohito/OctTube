#include "stdafx.h"
#include "Shader.h"
//���_�f�[�^�A�f�[�^���i3�̔{���j
GLuint vbo(float* p,GLsizeiptr size){
  GLuint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(GL_ARRAY_BUFFER,buf);
  glBufferData(GL_ARRAY_BUFFER,size,p,GL_STATIC_DRAW);
  return buf;
}
GLuint* vertexAnimes(float* p, GLsizeiptr size, GLuint flame){
  GLuint* bufs=new GLuint[flame];
  glGenBuffers(flame, bufs);
  for (size_t i = 0; i < flame; i++)
  {
    glBindBuffer(GL_ARRAY_BUFFER, bufs[i]);
  }
  glBufferData(GL_ARRAY_BUFFER,size,p,GL_STATIC_DRAW);
  return bufs;
}
//���_�̃C���f�b�N�X�l��ݒ�i�ǂ̒��_�Ŗʂ��\�����邩�j
GLuint ebo(GLuint* is,GLsizeiptr size){
  GLuint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf);
  glBufferData(GL_ARRAY_BUFFER,size,is,GL_STATIC_DRAW);
  return buf;
}
//���_�ƃV�F�[�_�[�̊֘A�t��
GLuint vao(GLuint vbid){
  GLuint vao;
  //glutBitmapString
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, vbid);
  return vao;
}
GLuint* vaAnimes(GLuint* vbis,GLuint flame){
  GLuint* bufs=new GLuint[flame];
  glGenVertexArrays(flame, bufs);
  for (size_t i = 0; i < flame; i++)
  {
    glBindVertexArray(bufs[i]);
    glBindBuffer(GL_ARRAY_BUFFER, vbis[i] );
  }
  return bufs;
}
//https://github.com/progschj/OpenGL-Examples/blob/master/03texture.cpp
//void uniform1(){
//  glUniform1f(gl
int createVerShader(){
  return glCreateShader(GL_VERTEX_SHADER);
}
int createFragShader(){
  return glCreateShader(GL_FRAGMENT_SHADER);
}
void loadProg(const GLchar* file,GLuint id){
  //{100����,120����} {void..,void..}
  //stage1 - enemy - vs
  //               - fs
  //               - //modelinfo�݂����ȃt�@�C���Ƀp�b�N����Ε������ÓI�Ɍ���ł��邯��
  //               - ene.fbx
  //       - car   - vs
  //               - fs 
  //  �S�t�H���_�̒��_�V�F�[�_�̒����A�V�F�[�_��z���
  //              �t���O�����g�V�F�[�_�ł�����
  //              ���f�����[�h���ʕ��̃o�b�t�@�I�u�W�F�N�g���쐬
  FILE *fp;
  const GLchar *source;
  GLsizei length;
  int ret;
  
  /* �t�@�C�����J�� */
  errno_t e = fopen_s(&fp,file, "rb");
  if (e != NOERROR) {
  perror(file);
  return ;
  }
  
  /* �t�@�C���̖����Ɉړ������݈ʒu (�܂�t�@�C���T�C�Y) �𓾂� */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);
  
  /* �t�@�C���T�C�Y�̃��������m�� */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
  fprintf(stderr, "Could not allocate read buffer.\n");
  return ;
  }
  
  /* �t�@�C����擪����ǂݍ��� */
  fseek(fp, 0L, SEEK_SET);
  ret = fread((void *)source, 1, length, fp) != (size_t)length;
  fclose(fp);
  
  /* �V�F�[�_�̃\�[�X�v���O�����̃V�F�[�_�I�u�W�F�N�g�ւ̓ǂݍ��� */
  if (ret)
  fprintf(stderr, "Could not read file: %s.\n", file);
  else
  glShaderSource(id, 1, &source, &length);
  
  /* �m�ۂ����������̊J�� */
  free((void *)source);
  
  //glShaderSource(id,1,&str,&length);
  //gl
  //glLinkProgram(
}
//������̏ꍇ
Shader::Shader(string vs,string fs)
{
  vsid=createVerShader();
  loadProg(vs,vsid);
  glCompileShader(vsid);
  //�V�F�[�_���O��gDebugger�������Ƃ��Ă����͂�

  fsid=createFragShader();
  loadProg(fs,fsid);
  glCompileShader(fsid);

  pid=glCreateProgram();
  glAttachShader(pid,vsid);
  glAttachShader(pid,fsid);
  glLinkProgram(pid);
  //loadProg();
  //�A�^�b�`���ꂽ�v���O�������ׂĂ���f�^�b�`�����Ə��������A
  glDeleteShader(fsid);
  glDeleteShader(vsid);
  glDetachShader(pid, fsid);
  glDetachShader(pid, vsid);
}

//���W�Ȃǂ͒��_�ɉe������̂ł���ŃV�F�[�_�ɑ����Ƃ��Ȃ��Ƃ����Ȃ��ƌ������Ƃ��낤��
void enableVerAtt(GLuint p){
  glEnableVertexAttribArray(p);
}
void vertAttPointer(GLuint p,GLsizei stride,const void* pointer){
  //���K���Ȃ��A���������A���_�̗v�f2
  enableVerAtt(p);
  glVertexAttribPointer(p,2,GL_FLOAT,false,stride,pointer);
  //glVertexAttribPointer( �e�N�X�`������n���Ȃ�size��2�炵�� stride�͌��ԂƂ��������ł����͗l
}
//��x�����Ă��Β��_�o�b�t�@�I�u�W�F�N�g�ɋL�������̂ł���ȍ~��render��vbo��Ԃ����ł���
void Shader::SendVert(float* p,int size,GLuint* inds){
  points = p;
  indexs = inds;
  esize = size / 2;
  vbs=vertexAnimes(p,sizeof(float)* size,20);
  eb=ebo(inds, esize);
  vas=vaAnimes(vbs,20);
}
void sendFace(float *p){
}
void Shader::Use(bool isuse){
  if (isuse)
  glUseProgram(pid);
  else
  glUseProgram(0);
}
///���_�o�b�t�@����V�F�[�_�֑���̂ɕK�v�A�|�C���^�̓o�b�t�@�̐擪����ŏ��̑����܂ł̃I�t�Z�b�g
//http://www57.atwiki.jp/gametips/pages/29.html
//�ɂ���float�ۂ��񂽈�ɂ܂Ƃ߂�悤�Ȃ��Ƃ��ł���
#define Vector std::vector

void Shader::Render(int anim){
  //�������������ǒ��_�J���[�ŕ`�悵�悤
  int len = esize * 2;
  int pl=anim*len+len;
  glPushMatrix();
  //glTranslatef( -59 , -90 , 0 );
  //glEnableClientState(GL_VERTEX_ARRAY);
  //glVertexPointer(2, GL_FLOAT, 0, &points[anim*len]);
  //glDrawElements(GL_LINES, esize, GL_UNSIGNED_INT, indexs);
  auto verts=Vector<float>(len);
  for (int i = anim*len; i < pl; i+=2)
  {
    auto ai = i - anim*len;
    auto psi  = points[i];
    auto psi1 = points[i+1];
    verts[ai]=(psi);//�Ƃ肠����260
    verts[ai+1]=(psi1);
    glVertex2f( psi-59,psi1-90);//vao�͈ړ������ĂȂ����炾
    //glVertex2f( psi,psi1);
  }
  glPopMatrix();
}
void Shader::VaoRender(int anim){
  glBindVertexArray(vas[anim]);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glDrawElements(GL_LINES, esize, GL_UNSIGNED_INT, 0);

}
void Shader::Attloc(string attname){
  GLuint l=glGetAttribLocation(pid,attname);
  enableVerAtt(l);
  //�t���O�����g�l��Z���Ƃ�΃f�v�X�o�b�t�@������
  //�f�v�X�l��p�̃e�N�X�`�������A�t���[���o�b�t�@�I�u�W�F�N�g�Ɋ��蓖�Ă�
  //�f�v�X�e�N�X�`���ɑ΂��ă����_�����O�A�O���[�X�P�[���ɂȂ�
  //���_�߂��Ƃ���̃f�v�X�l�i�e�N�X�`����r�l�j�̃��v���V�A���t�B���^�ŃG�b�W����荕���_��`��
  //
  //vertAttPointer(l,3,
}
void Shader::Uniform(string uni,float v){
  auto uid=glGetUniformLocation(pid, uni);
  glUniform1f(uid, v);
  enableVerAtt(uid);
  //2�����ڂ�count,�����炭3�v�f*count��������
  //glUniform3fv()
}
Shader::~Shader(){
  //�񃌃��_�����O���ɏ��������A�����ɂ����A�^�b�`����Ă���V�F�[�_������΃f�^�b�`�����
  glDeleteProgram(pid);
}