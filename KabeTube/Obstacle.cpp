#include "stdafx.h"
#include "Obstacle.h"
#include "GameAlgolyzm.h"
#include "Renderer.h"
#include "PhysicCoefficient.h"
SoundAsset* squidAsset;
void playSquidDamageSound()
{
  squidAsset->PlayDamageSound();
}

const auto renderWid = 320.0f / 6.f;
const auto renderHei = 240.0f / 6.f;
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
CircleSensor::CircleSensor(World w,V2 pos,V2 dir,float rad){
  s    =  new b2CircleShape;
	s    -> m_radius = rad;
  bdef =  new b2BodyDef;
	bdef -> position=pos;
	bod  =  w->CreateBody( bdef );
  fix  =  bod->CreateFixture( s , 0 );
  e    =  new EnemyData( 0 , "typ" );
  e    -> browDir = V2 ( dir );
  fix  -> SetSensor(true);
  fix  -> SetUserData  ( e );
}
CircleSensor::~CircleSensor(){
  auto w=bod->GetWorld();
  for (auto j=w->GetJointList(); j!=nullptr; j=j->GetNext())
  {
    w->DestroyJoint(j);
  }
  SAFE_DELETE( e->Damage);
  w->DestroyBody(bod);
  SAFE_DELETE( s);
  SAFE_DELETE( e);
  SAFE_DELETE( bdef);
}
//高速化するならsizeはV2にしないでおく
Obstacle::Obstacle(World w,b2Vec2 pos,b2Vec2 size)
  :x(0),y(0),xx(0),yy(0),
  inverceMove(false),movable(false),
  inverceCount(0),
  isOuter(false)
{
  def=new b2BodyDef();
  s  =new b2PolygonShape;
  s  ->SetAsBox(size.x/2,size.y/2);
  def->position = pos;
  def->userData = nullptr;
  def->type     = b2BodyType::b2_kinematicBody; //staticとかkinematicとか
  body    = w->CreateBody(def);
  //二つの物体間の距離や、内側に特定の点があるかを fixture から
  //body->
  //fixture->
  fixture = body->CreateFixture(s,0);
}
Body Obstacle::GetBody(){
  return body;
}
Obstacle::~Obstacle()
{
  //呼び出し側のworldにさせる?
  auto w=body->GetWorld();
  w->DestroyBody(body);
  
  SAFE_DELETE(s);
  SAFE_DELETE(def);
  
  //body->DestroyFixture で子の一部をパージできる
  //fixture ,jointはdeleteすんなって　おそらくボディがゾンビになったりしたときに回収されるはず
}
b2Joint& joint(World w,Body b,Body bb){
  b2PrismaticJointDef joi;
  auto anc = b->GetWorldCenter() - bb->GetWorldCenter();
  //auto joi = b2DistanceJointDef();
  joi.Initialize(b, bb
    , anc / 2 , anc / 2);
  joi.enableLimit = true;
  joi.upperTranslation = 1;
  joi.lowerTranslation= -1;
  return *(w->CreateJoint(&joi));
}
PShape sqTentShape(V2& pos,fl ang){
  PShape s=new b2PolygonShape;
  s->SetAsBox(1, 6, pos, ang);
  return s;
}
  //enemydata,body利用してるので順番に注意

Bodies* Enemy::sqTentacle(V2 parentPos)
{
  //shapeに追加すればjoint追加しなくてもいいんじゃない
  auto bodsr = (new Bodies());
  bodsr->resize(3);
    //new Body[tentacleCount];
  auto& bods = *bodsr;
  auto underp = parentPos + V2(31.f, -20.f);
  auto street =sqTentShape(underp, 0.f);
  auto le     =sqTentShape(underp+V2( 7.1f , 2.f) ,-5.7f);
  auto ri     =sqTentShape(underp+V2(-7.1f , 2.f) , 5.7f);
  auto def      = new b2BodyDef;
  def->position = parentPos;
  def->type     = b2BodyType::b2_dynamicBody;
  auto m      = b2MassData();
  street->ComputeMass( &m , 1/24.0f );
  auto w      = body->GetWorld();
  bods[0]  =(w->CreateBody(def));
  bods[1]  =(w->CreateBody(def));
  bods[2]  =(w->CreateBody(def));
  //bods[2]     = w->CreateBody(def);
  auto f1     = bods[0]->CreateFixture(street, 0);
  auto f2     = bods[1]->CreateFixture(le, 0);
  auto f3     = bods[2]->CreateFixture(ri, 0);
  //tentData    = new EnemyData*[3];
  tentData.resize(3);
  tentData[0] = new EnemyData(0,"tent0");
  tentData[1] = new EnemyData(0,"tent1");
  tentData[2] = new EnemyData(0,"tent2");
  
  f1->SetUserData(tentData[0]);
  f2->SetUserData(tentData[1]);
  f3->SetUserData(tentData[2]);
  //center inertia狂う
  auto tesm = b2MassData();
  le->ComputeMass( &tesm , 0.01f );
  bods[0]->SetMassData(&m);
  bods[1]->SetMassData(&m);
  bods[2]->SetMassData(&m);
  return (&bods);
}
void Enemy::squidProfile(World w,V2 pos){
  actBox            = b2AABB();
  actBox.upperBound = V2(renderWid-10,renderHei);
  actBox.lowerBound = V2(-renderWid+10,-renderHei);

  s                 = new b2PolygonShape;
  //s->m_radius=size;
  s                 ->SetAsBox(10, 15,V2(0,5),0);
  auto m            = b2MassData();
  s                 ->ComputeMass(&m,0.1f);
  def               = new b2BodyDef();
  def->position     = pos;
  //def->angle = 90;
  def->gravityScale   = 0.01f;
  def->angularDamping = 0.1f;
  def->type         = b2BodyType::b2_dynamicBody;
  e=  new EnemyData(0,"squid");
  body       = w->CreateBody(def);
  body       ->SetMassData(&m);
  fixture           = body->CreateFixture(s,0);
  fixture           ->SetUserData(e);
  fixture           ->SetSensor(false);
  appendBody        = sqTentacle(pos);
  auto& j    = joint(w, body, (*appendBody)[0]);
  auto& jj   = joint(w, body, (*appendBody)[1]);
  auto& jjj  = joint(w, body, (*appendBody)[2]);
}
Enemy::Enemy(World w, EnemyKind ek)
:Age(0), hp(100), world(w),
selfDelete(false), tentData((AppendBodyData()))
{
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
Score Enemy::GetScore(){
  //std::shared_ptrにしてクリア成績に参照させるか
  auto sc = std::shared_ptr<Vector<int>>(new Vector<int>(tentData.size()));
  auto size = tentData.size();
  sc->resize(size+1);
  sc->operator[](0) = *e->Damage;
  for (size_t i = 1; i < size; i++)
  {
    sc->operator[](i) = *tentData[i]->Damage;
  }

  //{ *e->Damage ,*tentData[0]->Damage,
  //*tentData[1]->Damage,*tentData[2]->Damage};
  return sc;
}
void Enemy::SetProfile(EnemyKind kin)
{

  decltype(readFromToml("")) phy;
  switch (kin)
  {
  case Squid:
    //squid.csvでアセットと形状、トランスフォーム
    squidProfile(world, V2(-32,0));
    anim       = new AnimAsset(kin);
    squidAsset = new SoundAsset(kin);
    break;
  case Dragon:
    phy=readFromToml(Path::getPath()+"Dragon.toml");
    SetHp(phy->HP);
    body=phy->MakeBody(*world);
    def =std::move( phy->getBodyDef());
    //moveできないのか、途中で終了した
    fixture =std::move( phy->getFixDef());
    e =reinterpret_cast<EnemyData*> (fixture->GetUserData());
    //phy->moveFixDef(fixture);
    break;
  default:
    break;
  }
  //auto kin  = static_cast<EnemyKind>(kind);
}
enum Moving{
  R,
  L,
  TL,
  TR,
};
static auto   mov = R;
std::string makeString(Moving m)
{
  switch (m)
  {
  case R:
    return "R";
    break;
  case L:
    return "L";
    break;
  case TL:
    return "TL";
    break;
  case TR:
    return "TR";
    break;
  default:
    break;
  }
}
std::string makeString(String name, float v)
{
  auto vs=std::to_string(v);
  return std::string(name) + (": ") + vs + '\n';
}

void Enemy::motion(){
  static float32 one = 1.0f;
  static float32 zero= 0.0f;
//todo アニメーションステートマシン内で管理させたほうが
  auto p             = body->GetPosition();
  float32 ang        = body->GetAngle();
  //-59,-37
//キャッチボールみたいな動き＝外に壁があれば
  //数フレに1度Impulse
  //Unityで実験化
  static float       rotationAnim = 0.0f;
  static auto  animationstateWait = 20.0f;
  static const auto     stateWait = 20.0f;
  static auto               speed = one*800;
  static const auto     slowspeed = 0.1f;
  auto dbs = makeString("rotationAnim", rotationAnim)
    + makeString("animationstateWait", animationstateWait)
    + makeString("speed", speed)
    + makeString( mov);
  assert(DrawDebugString(dbs.c_str()));
  glTranslatef  ( p.x , p.y , 0 );
  glRotatef     ( toDeg( ang) + 180.0f , 0 , 0 , 1 );
  glRotatef(rotationAnim,0,1,0);
  if (animationstateWait>0){
    switch (mov)
    {
    case R:
      Force( V2( speed , zero ) );
      break;
    case L:
      Force( V2(-speed , zero ) );
      break;
    case TL:
      Force( V2(-speed , zero ) );
      break;
    case TR:
      Force( V2( speed , zero ) );
      break;
    default:
      break;
    }
    if ( p.x > actBox.upperBound.x ){
      mov = TL;
      rotationAnim = (1.0f-
        bej( ( stateWait - animationstateWait ) / stateWait ) ) * 180.0f; 
      rotationAnim = clamp( rotationAnim , 0.0f , 180.0f );
      --animationstateWait; //roty<-(1.0f-roty)* 180.0f
      //Force(V2(-one, zero));
      Veloc  ( V2( -one , zero ) );
    }
    if (p.y > actBox.upperBound.y){
      Impulse( V2( zero , -one*5 ) );
    }
    if (p.x < actBox.lowerBound.x){
      mov = TR;
      rotationAnim= 
        bej( ( stateWait - animationstateWait ) / stateWait ) * 180.0f; 
      rotationAnim = clamp( rotationAnim , 0.0f , 180.0f );
      --animationstateWait;
      Veloc  ( V2( one  , zero ) );
    }
    if (p.y < actBox.lowerBound.y){
      Impulse( V2( zero , one*5  ) );
    }
  }
  else{
    //rotCount<-rotWait
    //  speed<-1.0f
    //if mov = TurnLeft then mov<-Left else mov<-Right

    animationstateWait = stateWait; 
    speed              = one * 800;
    if( mov == TL ) mov = L; else mov = R;
  }
}
b2AABB& Enemy::GetActBox(){
  return actBox;
}
bool 
  Enemy::IsAllMeatFired ()//ms=
  {
    //0なら不死
    if (hp == 0){ return false; }
    int length=0;//eがdeleteされていて読み取れない
    auto dam = *e->Damage;
    auto size = appendBody ? appendBody->size() : 0;
    for (int i = 0; i < size; i++)
    {
      dam = dam + *tentData[i]->Damage;
    }
    //それぞれのDamageを0に、nameを修正
    auto defeated=dam>hp;
    return defeated;
}

void Enemy::SetHp(int hp){
  this->hp=hp;
}
void Enemy::RefreshSubHP()
{
  *e->Damage = 0;
  auto size = appendBody ? appendBody->size() : 0;
  for (int i = 0; i < size; i++)
  {
    *tentData[i]->Damage = 0;
  }
}
void Enemy::SetPos(V2& pos){
  body->SetTransform(pos, 0);
}
void Enemy::Update(bool move){
  Age++;
  assert(appendBody->size() < 1000);
  //body->ApplyLinearImpulse(V2(1,0),V2(120,0), false);
  if (!IsAllMeatFired())
    motion();
  //std::cout << *(e->Damage) << std::endl;
  float32  scale = 1.0f / 4.0f ;
  glScalef(scale , scale , 0 );
  //ダメージ分をシェーダに渡して頂点色をいじりたい
  //燻製なので白方面に
  //パーティクルが多くなると画面に白のもこもこが現れ、みづらくなる
  static bool state = false;
  const int animLen = 38;
  if (Age % animLen == 0) {
    state = !state;
    auto vel = body->GetLinearVelocity().x > 0 ? V2(2,0) : V2(-2,0);
    if (state){
      Impulse(vel);
      if (mov==L || mov==R) //motionがつねにTL,TRになってる
      squidAsset->PlayMovingSound();
    }
  }
  int animFl = state ? Age % animLen : animLen - Age % animLen;
  //anim->DamageColor(*e->Damage);//足などに個別には表示できない
  //renderVertice( points , pointsLength , animFl/2 ); //遅くしてる
  drawLines([&](){
    anim->UpdateAnim(animFl / 2);
  });
  //anim->NoUse();
  glLoadIdentity();
  //return pos;
}
Body Enemy::GetBody(){
  return body;
}
//EnemyData
void Enemy::UnLoad(EnemyKind ek)
{

  auto w=body->GetWorld();
  auto count = w->GetJointCount();
  int i = 0;
  auto p = new b2Joint*[count];
  for (auto j=w->GetJointList();i<count ; j=j->GetNext(),++i)
  {
    p[i] = j;
  }
  for (size_t i = 0; i < count; ++i)
  {
    w->DestroyJoint(p[i]);
  }
  SAFE_DELETE_ARRAY(p);
  tentData.clear();
  if (appendBody)
  {
    std::for_each(appendBody->begin(), appendBody->end(),
      [&w](Body b){w->DestroyBody(b); });
    appendBody->clear();
  }
  SAFE_DELETE( e);
  w->DestroyBody(body);
  if (selfDelete){
    SAFE_DELETE(appendBody);
    SAFE_DELETE( s);
    SAFE_DELETE( (squidAsset));
    SAFE_DELETE( anim);
    SAFE_DELETE( def);
  }
  else{
    //物理データとアセットをアンロード
    //if(kind==Next){}
    SAFE_DELETE(anim);
    anim = new AnimAsset(ek);
    SetProfile(ek);
    int k = 0;
  }
}
Enemy::~Enemy(){
  selfDelete = true;

  UnLoad(EnemyKind::Len);
  //DA( tentData);
  tentData.clear();
}