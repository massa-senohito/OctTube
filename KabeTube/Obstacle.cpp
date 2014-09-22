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
typedef float fl;
fl max(fl x, fl y){
  return x>y ? x : y;
}
fl min(fl x, fl y){
  return x<y ? x : y;
}
fl clamp(fl t, fl mn, fl mx){
  return min(mx, max(t, mn));
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
b2Joint& joint(World w,Body b,Body bb){

  auto disj = b2DistanceJointDef();
  disj.Initialize(b, bb, b->GetWorldCenter(), bb->GetWorldCenter());
  return *(w->CreateJoint(&disj));
}
  //squid継承安定
void Enemy::squidProfile(World w,V2 pos){
  actBox = b2AABB();
  actBox.upperBound = V2(renderWid-10,renderHei);
  actBox.lowerBound = V2(-renderWid+10,-renderHei);

  s=new b2PolygonShape;
  //s->m_radius=size;
  s->SetAsBox(10, 15,V2(0,5),0);
  auto m=b2MassData();
  s->ComputeMass(&m,0.1f);
  
  def=new b2BodyDef();
  def->position=pos;
  //def->angle = 90;
  def->gravityScale = 0.01f;
  def->angularDamping = 0.1f;
  
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
void Enemy::Impulse(V2 v)
{
  body->ApplyLinearImpulse(v,body->GetLocalCenter(),false);
}
void Enemy::Force(V2 v)
{
  body->ApplyForceToCenter(v,false);
}
void Enemy::Veloc(V2 v){
  body->SetLinearVelocity(v);
}
void Enemy::SetPoints(Points p,int len){
  points=p;
  pointsLength=len;
  //Assetがelem 
}
enum Moving{
  R,
  L,
  TL,
  TR,
};
void Enemy::motion(){
  static float32 one = 1.0f;
  static float32 zero= 0.0f;
//todo アニメーションステートマシン内で管理させたほうが
  auto p=body->GetPosition();
  float32 ang=body->GetAngle();
  //-59,-37
  glTranslatef(p.x,p.y,0);
  glRotatef(toDeg( ang)+180.0f,0,0,1);
//キャッチボールみたいな動き＝外に壁があれば
  //数フレに1度Impulse
  //Unityで実験化
  static float rotationAnim = 0.0f;
  static auto animationstateWait = 20.0f;
  static const auto stateWait = 20.0f;
  static auto mov = R;
  static auto speed = one*800;
  static const auto slowspeed = 0.1f;
  glRotatef(rotationAnim,0,1,0);
  if (animationstateWait>0){
    switch (mov)
    {
    case R:
      Force(V2(speed, zero));
      break;
    case L:
      Force(V2(-speed, zero));
      break;
    case TL:
      Force(V2(-speed, zero));
      break;
    case TR:
      Force(V2(speed, zero));
      break;
    default:
      break;
    }
    if (p.x > actBox.upperBound.x){
      mov = TL;
      //speed = slowspeed;
      rotationAnim= (1.0f-bej((stateWait-animationstateWait)/stateWait))*180.0f; 
      rotationAnim = clamp(rotationAnim, 0.0f, 180.0f);

      --animationstateWait; //roty<-(1.0f-roty)* 180.0f
      //Force(V2(-one, zero));
      Veloc(V2(-one, zero));
    }
    if (p.y > actBox.upperBound.y){
      Impulse(V2(zero, -one));
    }
    if (p.x < actBox.lowerBound.x){
      mov = TR;
      //speed = slowspeed;
      rotationAnim= bej((stateWait-animationstateWait)/stateWait)*180.0f; 
      rotationAnim = clamp(rotationAnim, 0.0f, 180.0f);
      --animationstateWait;
      Veloc(V2(one, zero));
    }
    if (p.y < actBox.lowerBound.y){
      Impulse(V2(zero, one));
    }
  }
  else{
    //rotCount<-rotWait
    //  speed<-1.0f
    //if mov = TurnLeft then mov<-Left else mov<-Right

    animationstateWait = stateWait; 
    speed = one*800;
    if (mov == TL)mov = L; else mov = R;
  }
}
void Enemy::Update(){
  Age++;
  //body->ApplyLinearImpulse(V2(1,0),V2(120,0), false);
  motion();
  float32 scale=1.0f/4.0f;
  glScalef(scale,scale,0);
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