#pragma once
#include "Box2D\Box2D.h"
#include "cpptoml.h"
#include "GameAlgolyzm.h"
typedef long long int64;
#define asint64(x) x->as<int64>()->value()
#define asT(x,T) x->as<T>()->value()
typedef std::string string;
typedef std::list<string> StrList;
template<typename T>
bool always(T i) { return true; }
typedef cpptoml::toml_array Array;

#define ChildrenList std::map<string*,PhysicCoefficient*> 
class PhysicCoefficient
{
  b2Shape* shape;
  ChildrenList& children;
  b2BodyDef* def;
  b2MassData* mass;
  b2Fixture* fix;
  b2Body* body;
  EnemyData* edata;
  float dencity;
  //float shapec[5];
  bool isSensor;
  void makeRect(cpptoml::toml_array& a);
  void PhysicCoefficient::makeJoint(b2World&, b2Body&);
public:
  PhysicCoefficient();
  PhysicCoefficient** child;
  PhysicCoefficient* parent;
// EnemyData* data; //SetUserDataで使える
  int childCount;
  int HP;
  void setShape(b2Shape*);
  b2BodyDef* getBodyDef();
  b2Fixture* getFixDef();
  void moveFixDef(b2Fixture* otherfix);
  void PhysicCoefficient::SetFloat(float,string);
  void PhysicCoefficient::SetInt  (int64,string);
  void PhysicCoefficient::SetString(string, string);
  void PhysicCoefficient::SetBool (bool, string);
  void PhysicCoefficient::SetArray(Array&, string);
  PhysicCoefficient& GetChild(string&);
  PhysicCoefficient& GetChild(StrList&);
  b2Body* PhysicCoefficient::MakeBody(b2World&);
  
  ~PhysicCoefficient();
};
//相対パスを指定したりするので
std::shared_ptr<PhysicCoefficient> readFromToml(string);
