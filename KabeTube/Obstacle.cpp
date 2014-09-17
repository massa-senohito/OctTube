#include "stdafx.h"
#include "Obstacle.h"
#include "Renderer.h"
bool nearBy(const b2Vec2& pos,float32 x,float32 y){
    auto px=pos.x-x;
    auto py=pos.y-y;
    return (px*px+py*py)<0.003;//epsilon
}
const int invcount=10;//最低10フレームの間反転しない
void Obstacle::invMove(const b2Vec2& pos){
    if(inverceMove){
        if(nearBy(pos,xx,yy)){
            inverceCount=invcount;
            inverceMove=false;
        }
    }else{
	//最初
        if(nearBy(pos,x,y)){
            inverceCount=invcount;
            inverceMove=true;
        }
    }
}
void Obstacle::Update(){
    if(!movable)return;
    auto pos=body->GetPosition();

    if(inverceCount==0)invMove(pos);
    else inverceCount--;
}
void Obstacle::Movable()
{

//    float32 tox;float32 toy;
    movable=true;
}

//高速化するならsizeはV2にしないでおく
Obstacle::Obstacle(World w,b2Vec2 pos,b2Vec2 size)
  :x(0),y(0),xx(0),yy(0),inverceMove(false),movable(false),inverceCount(0)
{
  def=new b2BodyDef();
  s=new b2PolygonShape;
  s->SetAsBox(size.x/2,size.y/2);
  def->position=pos;
  
  def->type=b2BodyType::b2_kinematicBody; //staticとかkinematicとか
  body=w->CreateBody(def);
  //二つの物体間の距離や、内側に特定の点があるかを fixture から
  //body->
  //fixture->
  fixture=body->CreateFixture(s,0);
}
Body Obstacle::GetBody(){
  return body;
}
Obstacle::~Obstacle()
{
  //呼び出し側のworldにさせる?
  auto w=body->GetWorld();
  w->DestroyBody(body);
  
  delete s;
  delete def;
  
  //body->DestroyFixture で子の一部をパージできる
  //fixture ,jointはdeleteすんなって　おそらくボディがゾンビになったりしたときに回収されるはず
}

Enemy::Enemy(b2World* w,b2Vec2 pos,float32 size)
:Age(0), points(nullptr)
{
  def=new b2BodyDef();
  s=new b2CircleShape;
  s->m_radius=size;
  auto m=new b2MassData();
  m->mass=1;
  s->ComputeMass(m,12);
  def->position=pos;
  
  //
  def->type=b2BodyType::b2_dynamicBody;
  e=  new EnemyData;
  e->Name="squid";
  def->userData=e;
  //"enemy";
  body=w->CreateBody(def);
  fixture=body->CreateFixture(s,0);
  
}
void Enemy::Impulse(V2 v){
  body->ApplyLinearImpulse(v,body->GetLocalCenter(),false);
}
void Enemy::SetPoints(Points p,int len){
  points=p;
  pointsLength=len;
}
void Enemy::Update(){
  Age++;
  float32 ang=body->GetAngle();
  auto p=body->GetPosition();
  //-59,-37
  glTranslatef(p.x,p.y,0);
  glRotatef(ang,0,0,1);
  float32 scale=1.0f/150.0f;
  glScalef(scale,scale,0);
  glBegin(GL_LINES);
  renderVertice(points,pointsLength);
  glEnd();
  glLoadIdentity();
  //return pos;
}
Body Enemy::GetBody(){
  return body;
}
//EnemyData
Enemy::~Enemy(){
  auto w=body->GetWorld();
  w->DestroyBody(body);
  delete s;
  delete e;
  delete def;
}