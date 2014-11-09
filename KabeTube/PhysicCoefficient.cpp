#include "stdafx.h"
#include "PhysicCoefficient.h"



PhysicCoefficient::PhysicCoefficient()
{
}
void PhysicCoefficient::setShape(b2Shape*)
{

}
void PhysicCoefficient::setBodyDef(b2BodyDef*)
{

}
void PhysicCoefficient::setFixDef(b2FixtureDef){

}
PhysicCoefficient::~PhysicCoefficient()
{
  //SAFE_DELETE(def);
  //SAFE_DELETE(shape);
}
PhysicCoefficient* readFromToml(string path){
#define dcas dynamic_cast
  auto pars = cpptoml::parse_file(path);
  //親子構造、データ全部読んでからjointでつなげるなりするのに
  std::map<string, float> parantStruct;
  for (auto i = pars.begin(); i !=pars.end(); i++)
  {
    auto& type = *i->second;
    auto ename = i->first;
    if ( type.is_array()){
      type.as_array();
    }
    if (type.is_value()){
      float v;
      if (auto x = type.as<int64>()) v = x->value();
      if (auto x = type.as<double>()) v = x->value();
      if (ename == "rect"){ 
        auto a = type.as_array();
        auto x = a->at(1)->as<int64>();
      }
    }
    if (type.is_group()){
      auto& group = *type.as_group();
      for (auto i = group.begin(); i != group.end(); i++)
      {
        auto grName=i->first;//Bodyなどグループ名
        auto elem = i->second;
        if (auto& ar = (elem->as_group())){
          if (grName == "Body"){
            auto recz = ar->get("rect");
            //ar->contains("");で存在するエンティティのみ
          }
          if (grName == "Tentacle"){//その内側のグループ
            auto mass = ar->get("mass");
          }
        }
        //if (auto gro=)
      }
    }
  }
  auto tmp = new PhysicCoefficient();
  delete tmp;
  return tmp;
}