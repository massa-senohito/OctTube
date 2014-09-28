#include "stdafx.h"
#include <Box2D\Particle\b2ParticleSystem.h>
#include <Box2D\Box2D.h>
#include "DebugDraw.h"
#include "PhysicSystem.h"
#include "MyContactFilter.h"
#include <list>
#include <functional>
#define red 0.9f,0.2f,0.2f
using namespace std;//using string=std::string;
typedef float32 f32;
struct DeleteObj {
  template <typename T>
  void operator()(const T* ptr) const {
    if(ptr!=nullptr)
    delete ptr;ptr=nullptr;
  }
};
void makeFlyBox(float,float);
     //Shapes; 

//void loadStage(PhysicSystem^ p ){
//  //auto test=split("0.1,2.2,11.0,9.3,2.5,9.9",',');
//  auto lines=System::IO::File::ReadAllLines("");
//  for (int i = 0; i < lines->Length; i++)
//  {
//    auto geos=lines[i]->Split(',');
//	p->addFence(parse(geos[0]),parse(geos[1]),parse(geos[2]),parse(geos[3]));
//  }
//}
PDef particleSysDef;
PSys particleSys;
World w;

void movableFence(f32 x,f32 y,
                  f32 sx,f32 sy,
                  f32 tox,f32){
  
}
void PhysicSystem::addFence(f32 x,f32 y,f32 sx,f32 sy){
  auto b=gcnew Obstacle(w,V2(x,y),V2(sx,sy)); //自分をnewしたときにlistにいれて、delFencesの時に削除するので
  //obs->push_back(b);
  obs->Add(b);
}
//#define EneMap std::map<Enemy^,bool>
//EneMap* enemyLiving;
PEnemy PhysicSystem::addEnemy(f32 x,f32 y,f32 rad){
  auto e=gcnew Enemy(w,V2(x,y),rad);
  ens->Add(e);
  //auto em=enemyLiving->emplace(e,true);
  //em.first
  return e;
}
void delExactObstacle(PObstacle b){
  //w->DestroyBody(b->GetBody());
  //b->Dispose();
  delete b;
}
void PhysicSystem::delFences(){
  //std::for_each(obs->begin(),obs->end(),DeleteObj());
  //fenceだけ消すために、delExactObstacleの前にディスパッチが必要

#ifdef _MANAGED
  for each (PObstacle var in obs)
  {
    delExactObstacle(var);
  }
#else
  std::vector<PObstacle>::iterator d = obs->Data->begin();
  std::for_each(d,obs->Data->end(),delExactObstacle);
#endif
  obs->Clear();
}
void eachParticle(){
  int length=particleSys->GetParticleCount();
  size_t vsize=sizeof V2;
  size_t csize=sizeof b2Color;
  auto poss=particleSys->GetPositionBuffer();
  //w->SetDebugDraw(b2Draw
}
//w->GetParticleFlagsBuffer()[i] |= b2_zombieParticle;
 


DebugDraw* dd;
MyContactFilter* cfilter;
auto downGrav=-9.8f;
b2Vec2 grav(0.0,9.8f);
void PhysicSystem::makeOuterFence(int x,int y){
  //sizeのせいで外にはみ出す可能性がある
  auto f1= gcnew Obstacle
    ( w , V2( x / 2.f , 0.f ) , V2( 1.f , y ) );
  auto f2= gcnew Obstacle
    ( w , V2(-x / 2.f , 0.f ) , V2( 1.f , y ) );
  auto f3= gcnew Obstacle
    ( w , V2( 0.f , y / 2.f ) , V2( x - 1.f , 1.f ) );
  auto f4= gcnew Obstacle
    ( w , V2( 0.f , -y/ 2.f ) , V2( x - 1.f , 1.f ) );
  obs->Add(f1);
  obs->Add(f2);
  obs->Add(f3);
  obs->Add(f4);
}

PhysicSystem::PhysicSystem(void)
{
  string csv("a,b");
  //toi=タイムオブインパクトｚｃ
  w=new b2World(grav);
  
  //位置と接触フラグなどしかもってない
  //pd.position
  //粘性係数,大きさ、色のミキシングなど
  particleSysDef = new b2ParticleSystemDef();
  particleSysDef->radius=1.0f;
  //表面張力
  particleSysDef->surfaceTensionPressureStrength;
  //粘性係数
  particleSysDef->viscousStrength;
  //引張、大きくすると丸くてスムースな粒子になる
  particleSysDef->surfaceTensionNormalStrength;
  //反発した時の追加圧力、マイナスにすると粒子が引き合う運動になる（恐らくもちみたいにくっつきあうことかな）
  particleSysDef->repulsiveStrength;
  
  particleSys=w->CreateParticleSystem(particleSysDef);
  //particleSys のdencityとstride(stride*diameter)で質量もとめられている
  //しかし、パーティクル同士の接触の振る舞いには関与しない
  particleSys->SetDensity(0.8);
  //パーティクルグループdefでshapeを指定して
  //時間で削除するようにする
  particleSys->SetDestructionByAge(true);
  dd = new DebugDraw();
  dd->SetFlags(
    b2Draw::e_particleBit | b2Draw::e_shapeBit );
  w->SetDebugDraw(dd);
  cfilter = new MyContactFilter;
  
  w->SetContactFilter(cfilter);
  obs = gcnew Obss;
  makeOuterFence(73,53);
  ens = gcnew Enes;
}
inline PShape createBox(float32 hx,float32 hy,const b2Vec2& cen){
  auto shape = new b2PolygonShape();
  shape->SetAsBox( hx , hy , cen , 0 );
  auto mass  = new b2MassData;
  shape->ComputeMass( mass , 0.1f );
  
  return shape;
}
void makeParticle(float x,float y){
  b2ParticleGroupDef d;//d.particleCount=5; サンプル見比べておこう
  uint8 zero = 0 ; uint8 one = 1;
  d.userData;
  b2ParticleColor    b(zero,one,zero,one);
  d.color    = b;
  auto pos   = V2(x, y);
  //auto circle=new b2CircleShape;
  //circle->m_radius=
  d.shape    = createBox( 3 , 3 , pos );
  
  d.flags    = b2_tensileParticle //;
    | b2_fixtureContactFilterParticle;
  d.position = pos ;
  auto pg    = particleSys->CreateParticleGroup(d);
  //dにもpgにもdencityない,particleSysに
  auto mass  = pg->GetMass();//グループ内の総質量
}
void makeFlyBox(float x,float y){
  auto d=new b2BodyDef();
  d->userData;
  d->position=V2(x,y);
  d->type=b2_dynamicBody;
  d->linearVelocity=V2(-16,11);
  auto body=w->CreateBody(d);
  auto fix=new b2FixtureDef();
  fix->shape;
  fix->friction;
  
  fix->density;
  body->CreateFixture(fix);
  //各々の重力を変更できる
  //body->SetGravityScale
}
void makeFlyFish(){
  //makeFlyBox(-12,12).Joint();
}
void makeSinglePar(float x,float y,float vx,float vy){
  b2ParticleDef   d;
  b2ParticleColor bc( 0 , 0 , 1 , 1 );
  d.color    = bc;
  d.velocity = V2(vx,vy);
  d.position = V2(x,y);
  d.flags    = b2_tensileParticle
     |b2_fixtureContactFilterParticle;
  auto par   = particleSys->CreateParticle(d);
}
//パーティクルを継承した KomeParticle , StreamParticle 
//StreamParticleは水のパーティクルグループ、
//rigidparticle=崩れないパーティクルグループ
int PhysicSystem::MakeParticle(float x,float y){
  makeParticle(x,y);
  //pg-> rigidparticle でないとPositionとっても意味のない座標になる（たぶん重心が出るが、GetCenterあるのでそっちを）
  return 0;//pg->GetParticleCount();
}
void step(){
  //グループを作成すると増える、
  //std::cout << particleSys->GetParticleGroupCount() << std::endl;
  //グループで一気に増える
  //std::cout << particleSys->GetParticleCount() << std::endl;
  //小さいパーティクルシミュレーションのシミュレーションなら比較的大きな重力
  // b2CalculateParticleIterations helps to determine the number.
  float hz = 60.f;
  int32 pi =
    b2CalculateParticleIterations( grav.y , 1 , 1/hz );
  w->Step( 1/hz , 8 , 3 , pi );

  auto fpart = w->GetParticleSystemList()->GetPositionBuffer();

  for (auto bod = w->GetBodyList() ;
    bod != nullptr ; bod=bod->GetNext() ){
    auto ud = bod->GetUserData();
    if(ud){
      //userdataならビット演算で区別するのが
      auto    p= bod->GetPosition();
      auto dist= p-(*fpart);
//      int siz=1;
//      glRecti(-siz,-siz,siz,siz);
    }
    //かべの描画　まあ継承でDraw呼ぶのが妥当か
    //bod->GetFixtureList()
  }
  auto jl  = w->GetJointList();
  auto col = b2Color();
  while (jl){
    auto aa = jl->GetAnchorA();
    auto ab = jl->GetAnchorB();
    auto ba = jl->GetBodyA()->GetPosition();
    auto bb = jl->GetBodyB()->GetPosition();
    dd->DrawCircle( aa , 0.3 , col);
    dd->DrawCircle( ab , 0.3 , col);
    dd->DrawCircle( bb , 0.6 , col);
    dd->DrawCircle( ba , 0.6 , col);
    jl = jl->GetNext();
  }
  w->DrawDebugData();
}

///ワールドをステップさせ、同時に描画します
void PhysicSystem::Step(){

#ifdef _MANAGED
  for each (PEnemy var in ens)
  {
    var->Update();
    //列挙中に消えるとめんどくさいのでmap<,bool>
    //if(var)
  }
#else
  std::for_each(ens->Data->begin(), ens->Data->end(), [](PEnemy i){i->Update(); });
#endif
  step();
}
PhysicSystem::~PhysicSystem(){
  if(dd!=nullptr)
   delete dd;
  dd=nullptr;
  if (cfilter != nullptr)
    delete cfilter;
  cfilter = nullptr;
  delFences();
  delete obs;
  obs=nullptr;
  //vectorにしたかったがマネージ型はlistに放り込めない
  //リークするかもしれないがとりあえず
  //for (int i = 0; i < ens->Count; i++)
  //{
  //  delete ens[i];
  //}
  ens->Clear();
  delete ens;
  ens=nullptr;

  w->DestroyParticleSystem(particleSys);
  particleSys=nullptr;
  //http://oshiro.bpe.es.osaka-u.ac.jp/people/staff/imura/computer/OpenGL/framebuffer_object/disp_content
}