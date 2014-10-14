#pragma once
typedef const char* cstring; 
typedef unsigned int uint;
#define ref /**/

ref class Shader
{
    int vsid;
    int fsid;
    int pid;
    int esize;

    float* points;
    GLuint* indexs;

    uint* vbs,* vas;
    uint eb;
    //map<string,atr>
public:
    Shader(cstring vs,cstring fs);
    void SendVert(float* p, int size, GLuint* inds);
    ~Shader();
    void Attloc(cstring);
    void Use(bool);
    void Render(int anim);
    void VaoRender(int anim);
    void Uniform(cstring uniName,float v);
    //void Render
};
//���肵����F#�ŏ������o�C���h�����Ă����ق�����������
