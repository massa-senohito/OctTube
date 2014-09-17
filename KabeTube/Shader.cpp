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
}
void sendVert(float* p,int size){
    //vector渡す案
    vbo(p,sizeof(float)* size);

}
void sendFace(float *p){
}
///頂点バッファからシェーダへ贈るのに必要、ポインタはバッファの先頭から最初の属性までのオフセット
//http://www57.atwiki.jp/gametips/pages/29.html
//によればfloatぽいんた一つにまとめるようなことができる
void vertAttPointer(GLuint p,GLsizei stride,const void* pointer){
    //正規化なし、浮動少数、頂点の要素3
    glVertexAttribPointer(p,3,GL_FLOAT,false,stride,pointer);
    //glVertexAttribPointer( テクスチャ情報を渡すならsizeは2らしい strideは隙間という理解でいい模様
}
void Render(){
  //glBindVertexArray(vaid);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  //glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}
void enableVerAtt(int p){
    glEnableVertexAttribArray(p);
}
void Shader::attloc(string attname){
    GLuint l=glGetAttribLocation(pid,attname);
    enableVerAtt(l);
    //フラグメント値のZをとればデプスバッファが取れる
    //デプス値専用のテクスチャを作り、フレームバッファオブジェクトに割り当てる
    //デプステクスチャに対してレンダリング、グレースケールになる
    //視点近いところのデプス値（テクスチャのr値）のラプラシアンフィルタでエッジを作り黒い点を描画
    //
    //vertAttPointer(l,3,
}