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
#define fun std::function
typedef cpptoml::toml_group Group;
typedef fun<void(cpptoml::toml_base&,std::string&,std::string&)> TomlFun;
void GetPhys(cpptoml::toml_base& t,std::string& name,std::string& parentname){
  if (auto v = t.as<int64>()){

  }
  if (auto v = t.as<std::string>()){

  }
  
}
void recursiveGroup(Group& g,std::string& parent,TomlFun f){
  for (auto& i : g){
    
    auto type = i.second;
    
    auto ename = i.first;
    if (type->is_group()){
      recursiveGroup(*type->as_group(),parent+"."+ename,f);
    }
    else{
      f(*type,ename,parent);
    }
  }
}
void origFun(string path){
  auto pars = cpptoml::parse_file(path);
  //親子構造、データ全部読んでからjointでつなげるなりするのに
  std::map<string, float> parantStruct;
  for (auto i = pars.begin(); i !=pars.end(); i++)
  {
    auto& type = *i->second;
    auto ename = i->first;
    if (type.is_group()){
      auto& group = *type.as_group();
      for (auto i = group.begin(); i != group.end(); i++)
      {
        auto grName=i->first;//Bodyなどグループ名
        auto elem = i->second;
        if (auto& ar = (elem->as_group())){
          if (grName == "Body"){
            auto rect = ar->get_array("rect");
            auto x=asint64(rect->at(0));
            auto pyOp=ar->get_as<std::string>("type");
	    //optional
            auto pyTy = *pyOp;
            //ar->contains("");で存在するエンティティのみ
          }
          if (grName == "Tentacle"){//その内側のグループ
            auto tentBody=ar->as_group()->get_group("Body");
	    
            auto mass = asint64(tentBody->get("mass"));
            auto name = asT(tentBody->get("name"), std::string);
          }
        }
      }
    }
  }

}
PhysicCoefficient* readFromToml(string path){
  auto pars = cpptoml::parse_file(path);
  recursiveGroup(pars,std::string(""), GetPhys);
  auto tmp = new PhysicCoefficient();
  delete tmp;
  return tmp;
}