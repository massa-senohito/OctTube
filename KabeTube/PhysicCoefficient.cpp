#include "stdafx.h"
#include "PhysicCoefficient.h"



PhysicCoefficient::PhysicCoefficient()
  :shapec{}
{
}

void PhysicCoefficient::SetFloat(float v, string name){
  if (name == "denc"){
    //denc=v;
  }
  if (name == "gravScale"){
    
  }
  if (name == "angDamp"){
  }
}
void PhysicCoefficient::SetInt(int64 v, string name){

}
void PhysicCoefficient::SetString(string v, string name){

}
void PhysicCoefficient::SetBool(bool v, string name){
  if (name == ""){
  }
}
void PhysicCoefficient::setShape(b2Shape*)
{

}
void PhysicCoefficient::setBodyDef(b2BodyDef*)
{

}
void PhysicCoefficient::MakeBody(){
  shape->SetAsBox(
    shapec[0],
    shapec[1],
    b2Vec2(
      shapec[2],
      shapec[3]),
    shapec[4]
    );
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
typedef cpptoml::toml_base Base;
//typedef std::list<std::string*> StrList;
typedef std::string StrList;
typedef fun<void(cpptoml::toml_base&,std::string&,StrList&)> TomlFun;
void makeRect(cpptoml::toml_array& a,PhysicCoefficient* c)
{
  auto s=new b2PolygonShape();
  auto w = asint64(a.at(0));
  auto h = asint64(a.at(1));
  auto pos = b2Vec2(asint64(a.at(2)), asint64(a.at(3)));
  s->SetAsBox(w, h, pos, 0);
  c->setShape(s);
}
void setBody(PhysicCoefficient* c){
  b2BodyDef* b=new b2BodyDef();

}

void physParamFrom(Base& t,std::string& name,StrList& parentname,PhysicCoefficient* c){
  //再帰が深くなるとlistにpush_backされすぎる
  //ここはロード時だし、後でバイナリに変更できるのでsplitで
  //終了時にshape追加するなどして
  if (auto v = t.as<int64>()){
    c->SetInt(v->value(), name);
  }
  if (auto v = t.as<std::string>()){
    c->SetString(v->value(), name);
  }
  if (auto v = t.as<double>()){
    c->SetFloat(v->value(), name);
  }
  if (auto v = t.as<bool>()){
    c->SetBool(v->value(), name);
  }
  if (auto v = t.as_array()){
    if (name == "rect"){
      //map[name]=v;けっきょくまた順序の問題になるからこれで
      makeRect(*v, c);
      //decoratorはinterfaceを共有するクラスを受け取って追加していく
      //monoid
    }
    if (name == "pos");
    if (name == "relPos");
  }
  
}
void recursiveGroup(Group& g,StrList& parent,TomlFun f,PhysicCoefficient* c){
  for (auto& i : g){
    
    auto type = i.second;
    auto ename = i.first;
    if (type->is_group()){
      auto paren = parent +"."+ ename;
      recursiveGroup(*type->as_group(),paren,f,c);
      //parent.push_back(&ename);
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
  StrList l;
  auto tmp = new PhysicCoefficient();
  recursiveGroup(pars,l, physParamFrom,tmp);
  delete tmp;
  return tmp;
}