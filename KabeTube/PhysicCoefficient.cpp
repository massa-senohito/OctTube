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
//typedef std::list<std::string*> StrList;
typedef std::string StrList;
typedef fun<void(cpptoml::toml_base&,std::string&,StrList&)> TomlFun;
void makeRect(cpptoml::toml_array& a){
  auto s=new b2PolygonShape();
  auto w = asint64(a.at(0));
  auto h = asint64(a.at(1));
  auto pos = b2Vec2(asint64(a.at(2)), asint64(a.at(3)));
  s->SetAsBox(w, h, pos, 0);
}
void GetPhys(cpptoml::toml_base& t,std::string& name,StrList& parentname){
  //�ċA���[���Ȃ��list��push_back���ꂷ����
  //�����̓��[�h�������A��Ńo�C�i���ɕύX�ł���̂�split��
  //�I������shape�ǉ�����Ȃǂ���
  if (auto v = t.as<int64>()){

  }
  if (auto v = t.as<std::string>()){

  }
  if (auto v = t.as_array()){
    if (name == "rect"){
      auto x = asint64(v->at(0));
    }
  }
  
}
void recursiveGroup(Group& g,StrList& parent,TomlFun f){
  for (auto& i : g){
    
    auto type = i.second;
    auto ename = i.first;
    if (type->is_group()){
      auto paren = parent +"."+ ename;
      recursiveGroup(*type->as_group(),paren,f);
      //parent.push_back(&ename);
    }
    else{
      f(*type,ename,parent);
    }
  }
}
void origFun(string path){
  auto pars = cpptoml::parse_file(path);
  //�e�q�\���A�f�[�^�S���ǂ�ł���joint�łȂ���Ȃ肷��̂�
  std::map<string, float> parantStruct;
  for (auto i = pars.begin(); i !=pars.end(); i++)
  {
    auto& type = *i->second;
    auto ename = i->first;
    if (type.is_group()){
      auto& group = *type.as_group();
      for (auto i = group.begin(); i != group.end(); i++)
      {
        auto grName=i->first;//Body�ȂǃO���[�v��
        auto elem = i->second;
        if (auto& ar = (elem->as_group())){
          if (grName == "Body"){
            auto rect = ar->get_array("rect");
            auto x=asint64(rect->at(0));
            auto pyOp=ar->get_as<std::string>("type");
	    //optional
            auto pyTy = *pyOp;
            //ar->contains("");�ő��݂���G���e�B�e�B�̂�
          }
          if (grName == "Tentacle"){//���̓����̃O���[�v
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
  recursiveGroup(pars,l, GetPhys);
  auto tmp = new PhysicCoefficient();
  delete tmp;
  return tmp;
}