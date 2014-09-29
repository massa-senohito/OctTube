#pragma once
typedef const char* string; 
typedef unsigned int uint;
#define ref /**/

ref class Shader
{
    int vsid;
    int fsid;
    int pid;
    int esize;
    uint* vbs,* vas;
    uint eb;
    //map<string,atr>
public:
    Shader(string vs,string fs);
    void SendVert(float* p, int size, GLuint* inds);
    ~Shader();
    void Attloc(string);
    void Use(bool);
    void Render(int anim);
    void VaoRender(int anim);
    void Uniform(string uniName,float v);
    //void Render
};
//‰ºè‚µ‚½‚çF#‚Å‘‚¢‚½ƒoƒCƒ“ƒh‚à‚Á‚Ä‚«‚½‚Ù‚¤‚ª‚¢‚¢‚©‚à
