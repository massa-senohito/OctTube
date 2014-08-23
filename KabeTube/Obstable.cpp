#include "stdafx.h"
#include "Obstable.h"
bool nearBy(const b2Vec2& pos,float32 x,float32 y){
    auto px=pos.x-x;
    auto py=pos.y-y;
    return (px*px+py*py)<0.003;//epsilon
}
const int invcount=10;//�Œ�10�t���[���̊Ԕ��]���Ȃ�
void Obstable::invMove(const b2Vec2& pos){
    if(inverceMove){
        if(nearBy(pos,xx,yy)){
            inverceCount=invcount;
            inverceMove=false;
        }
    }else{
	//�ŏ�
        if(nearBy(pos,x,y)){
            inverceCount=invcount;
            inverceMove=true;
        }
    }
}
void Obstable::Update(){
    if(!movable)return;
    auto pos=body->GetPosition();

    if(inverceCount==0)invMove(pos);
    else inverceCount--;
}
void Obstable::Movable()
{

    float32 tox;float32 toy;
    movable=true;
}

Obstable::Obstable(World w,b2Vec2 pos,b2Vec2 size):x(0),y(0),xx(0),yy(0),inverceMove(false),movable(false),inverceCount(0)
{
  def=new b2BodyDef();
  s=new b2PolygonShape;
  s->SetAsBox(size.x/2,size.y/2);
  def->position=pos;
  
  def->type=b2BodyType::b2_kinematicBody; //static�Ƃ�kinematic�Ƃ�
  body=w->CreateBody(def);
  //��̕��̊Ԃ̋�����A�����ɓ���̓_�����邩�� fixture ����
  //body->
  //fixture->
  fixture=body->CreateFixture(s,0);
}
Body Obstable::GetBody(){
  return body;
}
Obstable::~Obstable()
{
  //�Ăяo������world�ɂ�����ׂ�
 // w->DestroyBody(body);
  
  delete s;
  delete def;
  
  //body->DestroyFixture �Ŏq�̈ꕔ���p�[�W�ł���
  //fixture ,joint��delete����Ȃ��ā@�����炭�{�f�B���]���r�ɂȂ����肵���Ƃ��ɉ�������͂�
}


Enemy::Enemy(b2World* w,b2Vec2 pos,b2Vec2 size)
{
  def=new b2BodyDef();
  s=new b2PolygonShape;
  s->SetAsBox(size.x/2,size.y/2);
  def->position=pos;
  //
  def->type=b2BodyType::b2_dynamicBody;
  body=w->CreateBody(def);
  fixture=body->CreateFixture(s,0);

}
void Enemy::Impulse(V2 v){
  body->GetLocalCenter();
  body->ApplyLinearImpulse(v,body->GetLocalCenter(),false);
}
void Enemy::Update(){
  //age++;
  //return pos;
}
Enemy::~Enemy(){
  delete s;
  delete def;
}