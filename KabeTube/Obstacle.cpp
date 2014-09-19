#include "stdafx.h"
#include "Obstacle.h"
#include "GameAlgolyzm.h"
#include "Renderer.h"

const auto renderWid = 320.0 / 6.;
const auto renderHei = 240.0 / 6.;
const auto epsilon= 0.003;
bool nearBy(const b2Vec2& pos,float32 x,float32 y){
    auto px=pos.x-x;
    auto py=pos.y-y;
    return (px*px+py*py)<epsilon;//epsilon
}
bool nearFloat(float t, float min, float max){
  return abs(t - max) < epsilon || abs(t - min) < epsilon;
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
  def->userData = nullptr;
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
void Enemy::squidProfile(World w,V2 pos){
  actBox = b2AABB();
  actBox.upperBound = V2(renderWid,renderHei);
  actBox.lowerBound = V2(-renderWid,-renderHei);

  def=new b2BodyDef();
  s=new b2PolygonShape;
  //s->m_radius=size;
  s->SetAsBox(10, 20);
  auto m=b2MassData();
  s->ComputeMass(&m,1);
  
  def->position=pos;
  //def->angle = 90;
  def->gravityScale = 0.01f;
  def->angularDamping = 0.1f;
  
  //
  def->type=b2BodyType::b2_dynamicBody;
  e=  new EnemyData;
  e->Name="squid";
  def->userData=e;
  //"enemy";
  body=w->CreateBody(def);
  body->SetMassData(&m);
  fixture=body->CreateFixture(s,0);
  auto mass=body->GetMass();
}
Enemy::Enemy(b2World* w,b2Vec2 pos,float32 size)
:Age(0), points(nullptr)
{
  //switch
  squidProfile(w, pos);
}
void Enemy::Impulse(V2 v){
  body->ApplyLinearImpulse(v,body->GetLocalCenter(),false);
}
void Enemy::SetPoints(Points p,int len){
  points=p;
  pointsLength=len;
  //Assetがelem 
}
float squidRotation(float v){
  //glRotatef(v, 0, 1, 0);
  return v *2;
}

void Enemy::motion(){
//todo アニメーションステートマシン内で管理させたほうが
  static float rotationAnim = 0.0f;
  static int animationstateWait = 20;
  auto p=body->GetPosition();
  float32 ang=body->GetAngle();
  //-59,-37
  glTranslatef(p.x,p.y,0);
  glRotatef(toDeg( ang)+180.0f,0,0,1);
  float32 scale=1.0f/4.0f;
  glScalef(scale,scale,0);
  static float32 one = 0.5f;
  static float32 zero= 0.0f;
  if (animationstateWait>0){
    if (p.x > actBox.upperBound.x){
      //左に回転しつつちょっと移動
      //rotationAnim = 0.0f;
      squidRotation(bej(rotationAnim)*180.0f);
      rotationAnim += 0.1f;
      --animationstateWait;
      Impulse(V2(-one, zero));
    }
    if (p.y > actBox.upperBound.y){
      Impulse(V2(zero, -one));
    }
    if (p.x < actBox.lowerBound.x){
      squidRotation(bej(rotationAnim)*180.0f);
      rotationAnim += 0.1f;
      --animationstateWait;
      Impulse(V2(one, zero));
    }
    if (p.y < actBox.lowerBound.y){
      Impulse(V2(zero, one));
    }
  }
  else{ animationstateWait = 20; }
}
void Enemy::Update(){
  Age++;
  body->ApplyLinearImpulse(V2(1,0),V2(120,0), false);
  motion();
  auto ang=def->angle;
  auto ab=body->GetAngle();
  //ダメージ分をシェーダに渡して頂点色をいじりたい
  //燻製なので白方面に
  glBegin(GL_LINES);
  //anim+=pointsLength;points[anim];
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