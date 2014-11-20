#pragma once
enum ScreenName
{
  Title,Main,GameOver,Ending,Length
};
class Screen
{
  ScreenName screenName;
  int cursol;
  GLuint* texHandle;
  void showTitle();
public:
  Screen(std::string&);
  ScreenName GetScreenName();
  ~Screen();
  void Show();
  void Hide();
};

