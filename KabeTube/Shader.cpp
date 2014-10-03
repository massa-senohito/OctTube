#include "stdafx.h"
#include "Shader.h"
//頂点データ、データ数（3の倍数）
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
//頂点のインデックス値を設定（どの頂点で面を構成するか）
GLuint ebo(GLuint* is,GLsizeiptr size){
  GLuint buf;
  glGenBuffers(1,&buf);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,buf);
  glBufferData(GL_ARRAY_BUFFER,size,is,GL_STATIC_DRAW);
  return buf;
}
//頂点とシェーダーの関連付け
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
  //{100文字,120文字} {void..,void..}
  //stage1 - enemy - vs
  //               - fs
  //               - //modelinfoみたいなファイルにパックすれば文字数静的に決定できるけど
  //               - ene.fbx
  //       - car   - vs
  //               - fs 
  //  全フォルダの頂点シェーダの長さ、シェーダを配列に
  //              フラグメントシェーダでも同じ
  //              モデルロードし面分のバッファオブジェクトを作成
  FILE *fp;
  const GLchar *source;
  GLsizei length;
  int ret;
  
  /* ファイルを開く */
  errno_t e = fopen_s(&fp,file, "rb");
  if (e != NOERROR) {
  perror(file);
  return ;
  }
  
  /* ファイルの末尾に移動し現在位置 (つまりファイルサイズ) を得る */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);
  
  /* ファイルサイズのメモリを確保 */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
  fprintf(stderr, "Could not allocate read buffer.\n");
  return ;
  }
  
  /* ファイルを先頭から読み込む */
  fseek(fp, 0L, SEEK_SET);
  ret = fread((void *)source, 1, length, fp) != (size_t)length;
  fclose(fp);
  
  /* シェーダのソースプログラムのシェーダオブジェクトへの読み込み */
  if (ret)
  fprintf(stderr, "Could not read file: %s.\n", file);
  else
  glShaderSource(id, 1, &source, &length);
  
  /* 確保したメモリの開放 */
  free((void *)source);
  
  //glShaderSource(id,1,&str,&length);
  //gl
  //glLinkProgram(
}
//一つだけの場合
Shader::Shader(string vs,string fs)
{
  vsid=createVerShader();
  loadProg(vs,vsid);
  glCompileShader(vsid);
  //シェーダログはgDebuggerがくいとってくれるはず

  fsid=createFragShader();
  loadProg(fs,fsid);
  glCompileShader(fsid);

  pid=glCreateProgram();
  glAttachShader(pid,vsid);
  glAttachShader(pid,fsid);
  glLinkProgram(pid);
  //loadProg();
  //アタッチされたプログラムすべてからデタッチされると消去される、
  glDeleteShader(fsid);
  glDeleteShader(vsid);
  glDetachShader(pid, fsid);
  glDetachShader(pid, vsid);
}

//座標などは頂点に影響するのでこれでシェーダに送っとかないといけないと言うことだろうか
void enableVerAtt(GLuint p){
  glEnableVertexAttribArray(p);
}
void vertAttPointer(GLuint p,GLsizei stride,const void* pointer){
  //正規化なし、浮動少数、頂点の要素2
  enableVerAtt(p);
  glVertexAttribPointer(p,2,GL_FLOAT,false,stride,pointer);
  //glVertexAttribPointer( テクスチャ情報を渡すならsizeは2らしい strideは隙間という理解でいい模様
}
//一度送ってやれば頂点バッファオブジェクトに記憶されるのでそれ以降はrenderでvboよぶだけでおｋ
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
///頂点バッファからシェーダへ贈るのに必要、ポインタはバッファの先頭から最初の属性までのオフセット
//http://www57.atwiki.jp/gametips/pages/29.html
//によればfloatぽいんた一つにまとめるようなことができる
#define Vector std::vector

void Shader::Render(int anim){
  //せっかくだけど頂点カラーで描画しよう
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
    verts[ai]=(psi);//とりあえず260
    verts[ai+1]=(psi1);
    glVertex2f( psi-59,psi1-90);//vaoは移動させてないからだ
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
  //フラグメント値のZをとればデプスバッファが取れる
  //デプス値専用のテクスチャを作り、フレームバッファオブジェクトに割り当てる
  //デプステクスチャに対してレンダリング、グレースケールになる
  //視点近いところのデプス値（テクスチャのr値）のラプラシアンフィルタでエッジを作り黒い点を描画
  //
  //vertAttPointer(l,3,
}
void Shader::Uniform(string uni,float v){
  auto uid=glGetUniformLocation(pid, uni);
  glUniform1f(uid, v);
  enableVerAtt(uid);
  //2引数目はcount,おそらく3要素*count分送られる
  //glUniform3fv()
}
Shader::~Shader(){
  //非レンダリング時に消去される、同時にもしアタッチされているシェーダがあればデタッチされる
  glDeleteProgram(pid);
}