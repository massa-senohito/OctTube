#pragma once
typedef const char* string; 
#define ref /**/
ref class Shader
{
    int vsid;
    int fsid;
    int pid;
    //map<string,atr>
public:
    Shader(string,string);
    void attloc(string);
    //void Render
};
//下手したらF#で書いたバインドもってきたほうがいいかも
