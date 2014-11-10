#pragma once
#include "Box2D\Box2D.h"
#include "cpptoml.h"
#define SAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
#define SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)
#define DA(p) SAFE_DELETE_ARRAY(p)
typedef long long int64;
#define asint64(x) x->as<int64>()->value()
#define asT(x,T) x->as<T>()->value()
class PhysicCoefficient
{
  b2Shape* shape;
  b2BodyDef* def;
  b2FixtureDef fixdef;
public:
  PhysicCoefficient();
  void setShape(b2Shape*);
  void setBodyDef(b2BodyDef*);
  void setFixDef(b2FixtureDef);
  ~PhysicCoefficient();
};
typedef std::string string;
//‘Š‘ÎƒpƒX‚ðŽw’è‚µ‚½‚è‚·‚é‚Ì‚Å
PhysicCoefficient* readFromToml(string);
