#pragma once
class DebugLogger
{
  
public:
  static bool RenderSimple;
  DebugLogger();
  int GetError();
  int Send(const char*);
  ~DebugLogger();
};

