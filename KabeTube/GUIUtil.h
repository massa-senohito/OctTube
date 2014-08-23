#pragma once
//#define GLUI_ROTATION
#include "GL/glui.h"
class GUIUtil
{
    GLUI *glui;
    GLUI_CB *cb;
    int count;
public:
    GUIUtil(void);
    ~GUIUtil(void);
    //GLUI_Rotation* addRot(float*);
    GLUI_EditText* addText();
    GLUI_Spinner* addSpinner(const char* name,float* ref);
};

