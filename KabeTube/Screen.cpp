#include "stdafx.h"
#include "Screen.h"
#include "Renderer.h"
#include "Texture.h"
typedef const char* CString;
Strptr makeStr(std::string& e)
{
  return std::make_unique<std::string>(e);
}
Screen::Screen(std::string& path) :screenName(Title)
{
  
  auto pa = std::make_unique< Paths>();
  pa->push_back(makeStr(path+"title.png"));
  texHandle = reserveTexture(1, *pa);
}


Screen::~Screen()
{
  deleteTex(1, texHandle);
  delete[] texHandle;
}
void Screen::showTitle()
{
  _ASSERT(texHandle);
  setPolyAlways(7);
  spriteTexture(texHandle[0], 0, 0);
  setMask(1);
}
ScreenName Screen::GetScreenName()
{
  return screenName;
}
void Screen::Show()
{
  switch (screenName)
  {
  case Title:
    //loadImage
    showTitle();
    break;
  case Main:
    //キャラクター表示ループなどをこっちに持って来たい
    
    break;
  case GameOver:
    break;
  case Ending:
    break;
  case Length:
    break;
  default:
    break;
  }
}
void Screen::Hide()
{
  switch (screenName)
  {
  case Title:
    screenName = Main;
    break;
  case Main:
    break;
  case GameOver:
    screenName = Title;
    break;
  case Ending:
    screenName = Title;
    break;
  case Length:
    break;
  default:
    break;
  }
}