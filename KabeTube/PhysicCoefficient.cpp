#include "stdafx.h"
#include "PhysicCoefficient.h"



PhysicCoefficient::PhysicCoefficient()
:
//shapec{},
  children(*(new ChildrenList)), mass(new b2MassData)
  , shape(new b2PolygonShape), def(new b2BodyDef), edata(new EnemyData(0,""))
  , child(nullptr)
{

}

PhysicCoefficient::~PhysicCoefficient()
{
  SAFE_DELETE(def);
  SAFE_DELETE(shape);
  SAFE_DELETE(mass);
  //SAFE_DELETE(edata);
  DA(child);
}
void PhysicCoefficient::SetFloat(float v, string name){
  if (name == "denc"){
    dencity=v;
    
  }
  if (name == "gravScale"){
    def->gravityScale = v;
  }
  if (name == "angDamp"){
    def->angularDamping = v;
  }
}
void PhysicCoefficient::SetInt(int64 v, string name){
  if (name == "hp")
  {
    HP = v;
  }
}
void PhysicCoefficient::SetString(string v, string name){
  if (v == "dynamic")
    //type未指定ならkinematicに
    def->type = b2BodyType::b2_dynamicBody;
}
void PhysicCoefficient::SetBool(bool v, string name){
  if (name == "sensor"){
    isSensor = v;
  }
}

void PhysicCoefficient::SetArray(Array& v, string name)
{

    if (name == "rect"){
      //map[name]=v;けっきょくまた順序の問題になるからこれで

      makeRect(v);
    }
    if (name == "pos")
    {
      auto& p = b2Vec2(asT(v.at(0), double), asT(v.at(1), double));
      def->position = p;
      //v->at(;
    }
    if (name == "relPos")
    {
      auto& ppos=parent->def->position;
      auto& p =
        b2Vec2(asT(v.at(0), double) + ppos.x, asT(v.at(1), double) + ppos.y);
      def->position = p;
    }
}
PhysicCoefficient& PhysicCoefficient::GetChild(StrList& cname){
  //newしてくれないとかならfindして手動で何とか
  if (cname.size() == 1)
  {
    return *children[&cname.front()];
  }
  //もとはvecなので
  cname.pop_front();
  GetChild(cname);
}

PhysicCoefficient& PhysicCoefficient::GetChild(string& cname){
  //newしてくれないとかならfindして手動で何とか
  return *children[&cname];
}
void PhysicCoefficient::setShape(b2Shape* s)
{
  shape = s;
}
b2BodyDef* PhysicCoefficient::getBodyDef()
{
  return def;
}

void PhysicCoefficient::makeJoint(b2World& w, b2Body& b)
{
  b2PrismaticJointDef joi;
  auto anc = b.GetWorldCenter() - body->GetWorldCenter();
  //auto joi = b2DistanceJointDef();
  joi.Initialize(&b, body
    , anc / 2 , anc / 2);
  joi.enableLimit = true;
  joi.upperTranslation = 1;
  joi.lowerTranslation= -1;
  w.CreateJoint(&joi);
}
b2Body* PhysicCoefficient::MakeBody(b2World& w)
{
  shape->ComputeMass(mass, dencity);
  body = w.CreateBody(def);
  for (int i = 0; i < childCount; i++)
  {
    auto& cbody= *child[i]->MakeBody(w);
    makeJoint(w, cbody);
  }
  fix = body->CreateFixture(shape,dencity);
  fix ->SetSensor(isSensor);
  def->userData = edata;
  fix ->SetUserData(edata);
  return body;
}
b2Fixture* PhysicCoefficient::getFixDef(){
  return fix;
}
#define fun std::function
typedef cpptoml::toml_group Group;
typedef cpptoml::toml_base Base;
//typedef std::list<std::string*> StrList;
typedef fun<void(Base&,std::string&,string&,PhysicCoefficient*)> TomlFun;
void PhysicCoefficient::makeRect(cpptoml::toml_array& a)
{
  auto s=new b2PolygonShape();
  auto w = asint64(a.at(0));
  auto& h = asint64(a.at(1));
  auto& pos = b2Vec2(asint64(a.at(2)), asint64(a.at(3)));
  s->SetAsBox(w, h, pos, 0);
  setShape(s);
}
void setBody(PhysicCoefficient* c){
  b2BodyDef* b=new b2BodyDef();

}

void physParamFrom(Base& t,string& name,string& parentname,PhysicCoefficient* c){
  //再帰が深くなるとlistにpush_backされすぎる
  //ここはロード時だし、後でバイナリに変更できるのでsplitで
  //終了時にshape追加するなどして
  auto& parent=Split(parentname, '.');
  auto parentls =std::unique_ptr<StrList>( new StrList(parent.begin(),parent.end()));
  if (parentls->back() == "Body")
  {
    int p = 0;
  }

  if (auto v = t.as<int64>()){
    c->SetInt(v->value(), name);
  }
  if (auto v = t.as<std::string>()){
    c->SetString(v->value(), name);
  }
  if (auto v = t.as<double>()){
    
    c->SetFloat((float)v->value(), name);
  }
  if (auto v = t.as<bool>()){
    c->SetBool(v->value(), name);
  }
  if (auto v = t.as_array()){
    c->SetArray(*v, name);
  }
  
}
void recursiveGroup(Group& g,string& parent,TomlFun f,PhysicCoefficient* c)
{
  for (auto& i : g){
    
    auto type = i.second;
    auto ename = i.first;
    if (type->is_group())
    {
      auto paren = parent +"."+ ename;
      auto& group=*type->as_group();
      if (ename != "Body")
      {
	//Setnameはあと回し
        int children = group.size();
        if (children > 2)
        {
	  //仕方なくここにロジック書く
          auto subbody = children - 1;
          c->child =
            new PhysicCoefficient*[subbody];
          c->childCount = subbody;
          for (int j = 0; j < subbody; j++)
          {
            auto p = new PhysicCoefficient();
            recursiveGroup(group, parent, f, p);
            c->child[j] = p;
          }
        }
      }
      recursiveGroup(group,paren,f,c);
      //parent.push_back(&ename);
    }
    else{
      f(*type,ename,parent,c);
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
#define sPtr std::shared_ptr
sPtr<PhysicCoefficient> readFromToml(string path){
  auto pars = cpptoml::parse_file(path);
  string l;
  auto tmp = std::make_shared < PhysicCoefficient>();
  recursiveGroup(pars,l, physParamFrom,tmp.get());
  //worldから全ボディをとってきてイテレータで消せば
  return tmp;
}