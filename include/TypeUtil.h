#pragma once
#include <functional>
namespace Types{
  typedef const char * str;
  typedef std::function<void(void)> action;
  template <typename T,typename RT>
  struct funcs{
      typedef std::function<RT(T)> func;
  };
  template <typename T>
  struct funcs<T,bool>{
      typedef T pred;
      //���ɂ���ėv�f���ς��
  };

  struct Vec2{
      int X;int Y;
  };
  //template<typename T>
  //struct action{
  //    typedef std::function<void(T)> act;
  //};
}
  
 // typedef Types::funcs<int,bool>::func pred;
class TypeUtil
{
public:
    TypeUtil(void);
    ~TypeUtil(void);
};

