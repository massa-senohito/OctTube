#include "stdafx.h"
#include <Box2D\Particle\b2ParticleSystem.h>
#include <Box2D\Box2D.h>
#include "DebugDraw.h"
#include "PhysicSystem.h"
#include "MyContactFilter.h"
#include <list>
#include <functional>
typedef b2ParticleSystemDef* PDef ;
typedef b2ParticleSystem* PSys;
typedef b2World* World;
typedef b2Body* Body;
typedef b2BodyDef* BodyDef;
typedef b2Fixture* Fixture;
typedef b2PolygonShape* PShape;
typedef b2CircleShape* CShape;
typedef b2Vec2 V2;
#define red 0.9f,0.2f,0.2f

#define Vector DotnetList
typedef Obstacle*          PObstacle;
typedef DotnetList<Obstacle*>  Obss;
typedef DotnetList<Obstacle*>* PObss;
PObss obs;
using namespace std;//using string=std::string;
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
  auto b=gcnew Obstacle(w,V2(x,y),V2(sx,sy));
  obs->Add(b);
}
//#define EneMap std::map<Enemy^,bool>
//EneMap* enemyLiving;
  //std::for_each(obs->begin(),obs->end(),DeleteObj());
  //fence�����������߂ɁAdelExactObstacle�̑O�Ƀf�B�X�p�b�`���K�v

void delExactObstacle(PObstacle b){
  //w->DestroyBody(b->GetBody());
  //b->Dispose();
  delete b;
}
void PhysicSystem::delFences(){

#ifdef _MANAGED
  for each (PObstacle var in obs)
  {
    delExactObstacle(var);
  }
#else
  auto data = obs->Data;
  std::vector<PObstacle>::iterator d = data->begin();
  std::for_each(d,data->end(),delExactObstacle);
#endif
  obs->Clear();
}
void eachParticle(std::function<void(V2&)> f){
  auto length=particleSys->GetParticleCount();
  size_t vsize=sizeof V2;
  size_t csize=sizeof b2Color;
  auto poss=particleSys->GetPositionBuffer();
  for (size_t i = 0; i < length; i++)
  {
    f(poss[i]);
  }
}
//w->GetParticleFlagsBuffer()[i] |= b2_zombieParticle;
 
DebugDraw* dd;
void PhysicSystem::DrawAABB(b2AABB& aabb){
  dd->DrawAABB(aabb, false);
}
MyContactFilter*   cfilter;
MyContactListener* mlis;
auto downGrav=-9.8f;
b2Vec2 grav(0.0,9.8f);
void PhysicSystem::makeOuterFence(f32 x,f32 y){
  //size�̂����ŊO�ɂ͂ݏo���\��������
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


int PhysicSystem::GetStopping() {
  return cfilter->StoppingFlame;
}
World PhysicSystem::GetWorld(){
  return w;
}
PhysicSystem::PhysicSystem(void)
  
{
  //toi=�^�C���I�u�C���p�N�g����
  w=new b2World(grav);
  //�ʒu�ƐڐG�t���O�Ȃǂ��������ĂȂ�
  //pd.position
  //�S���W��,�傫���A�F�̃~�L�V���O�Ȃ�
  particleSysDef = new b2ParticleSystemDef();
  particleSysDef->radius=1.0f;
  //�\�ʒ���
  particleSysDef->surfaceTensionPressureStrength;
  //�S���W��
  particleSysDef->viscousStrength;
  //�����A�傫������Ɗۂ��ăX���[�X�ȗ��q�ɂȂ�
  particleSysDef->surfaceTensionNormalStrength;
  //�����������̒ǉ����́A�}�C�i�X�ɂ���Ɨ��q�����������^���ɂȂ�i���炭�����݂����ɂ������������Ƃ��ȁj
  particleSysDef->repulsiveStrength;
  
  particleSys=w->CreateParticleSystem(particleSysDef);
  //particleSys ��dencity��stride(stride*diameter)�Ŏ��ʂ��Ƃ߂��Ă���
  //�������A�p�[�e�B�N�����m�̐ڐG�̐U�镑���ɂ͊֗^���Ȃ�
  particleSys->SetDensity(0.8f);
  //�p�[�e�B�N���O���[�vdef��shape���w�肵��
  //���Ԃō폜����悤�ɂ���
  particleSys->SetDestructionByAge(true);
  dd = new DebugDraw();
  dd->SetFlags(
    b2Draw::e_particleBit | b2Draw::e_shapeBit );
  w->SetDebugDraw(dd);
  cfilter = new MyContactFilter;
  mlis    = new MyContactListener;
  obs     = gcnew Obss;
  w->SetContactFilter  (cfilter);
  w->SetContactListener(mlis);
  makeOuterFence(73,53);
}
inline PShape createBox(float32 hx,float32 hy,const b2Vec2& cen){
  auto shape = new b2PolygonShape();
  shape->SetAsBox( hx , hy , cen , 0 );
  auto mass  = new b2MassData;
  shape->ComputeMass( mass , 0.1f );
  
  return shape;
}
auto pflag=b2_tensileParticle //;
    | b2_fixtureContactFilterParticle
    | b2_fixtureContactListenerParticle
    ;
void makeParticle(float x,float y){
  b2ParticleGroupDef d;//d.particleCount=5; �T���v������ׂĂ�����
  uint8 zero = 0 ; uint8 one = 1;
  d.userData;
  b2ParticleColor    b(zero,one,zero,one);
  d.color    = b;
  auto pos   = V2(x, y);
  //auto circle=new b2CircleShape;
  //circle->m_radius=
  d.shape    = createBox( 3 , 3 , pos );
  
  d.flags = pflag;
  d.position = pos ;
  auto pg    = particleSys->CreateParticleGroup(d);
  //d�ɂ�pg�ɂ�dencity�Ȃ�,particleSys��
  auto mass  = pg->GetMass();//�O���[�v���̑�����
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
  //�e�X�̏d�͂�ύX�ł���
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
  d.flags    = pflag;
  auto par   = particleSys->CreateParticle(d);
}
//�p�[�e�B�N�����p������ KomeParticle , StreamParticle 
//StreamParticle�͐��̃p�[�e�B�N���O���[�v�A
//rigidparticle=����Ȃ��p�[�e�B�N���O���[�v
int PhysicSystem::MakeParticle(float x,float y){
  makeParticle(x,y);
  //pg-> rigidparticle �łȂ���Position�Ƃ��Ă��Ӗ��̂Ȃ����W�ɂȂ�i���Ԃ�d�S���o�邪�AGetCenter����̂ł��������j
  return 0;//pg->GetParticleCount();
}
void PhysicSystem::jointDraw(){

  auto jl  = w->GetJointList();
  auto col = b2Color();
  while (jl){
    auto aa = jl->GetAnchorA();
    auto ab = jl->GetAnchorB();
    auto ba = jl->GetBodyA()->GetPosition();
    auto bb = jl->GetBodyB()->GetPosition();
    dd->DrawCircle( aa , 0.3f , col);
    dd->DrawCircle( ab , 0.3f , col);
    dd->DrawCircle( bb , 0.6f , col);
    dd->DrawCircle( ba , 0.6f , col);
    jl = jl->GetNext();
  }
}
void PhysicSystem::DebugDr()
{
  w->DrawDebugData();
}
void step(){
  //�O���[�v���쐬����Ƒ�����A
  //std::cout << particleSys->GetParticleGroupCount() << std::endl;
  //�O���[�v�ň�C�ɑ�����
  //std::cout << particleSys->GetParticleCount() << std::endl;
  //�������p�[�e�B�N���V�~�����[�V�����̃V�~�����[�V�����Ȃ��r�I�傫�ȏd��
  // b2CalculateParticleIterations helps to determine the number.
  float hz = 60.f;
  int32 pi =
    b2CalculateParticleIterations( grav.y , 1 , 1/hz );

  if ( cfilter->StoppingFlame==0 )
    w->Step( 1/hz , 8 , 3 , pi );
  else (cfilter->StoppingFlame--);
  
  for (auto bod = w->GetBodyList() ;
    bod != nullptr ; bod=bod->GetNext() ){
    auto ud = bod->GetUserData();
    if(ud){
      //userdata�Ȃ�r�b�g���Z�ŋ�ʂ���̂�
      //auto    p= bod->GetPosition();
      //glRecti(-siz,-siz,siz,siz);
    }
    else{
      //bod->GetFixtureList()->GetShape
    }
    //���ׂ̕`��@�܂��p����Draw�ĂԂ̂��Ó���
    //bod->GetFixtureList()
  }

  eachParticle([](V2& v){
    glBegin(GL_POINTS);
    float y = -0.2f,x=0.4f;
    auto p1 = v - V2(-x,  y);
    auto p2 = v - V2( 0, -y);
    auto p3 = v - V2( x,  y);
    glVertex2f( vdec(p1) );
    glVertex2f( vdec(p2) );
    glVertex2f( vdec(p3) );
    glEnd();
    //dd->DrawCircle(v, 0.5, b2Color());
  });


}

#define SAFE_DELETE(p)           do { if(p) { delete (p); (p) = nullptr; } } while(0)
///���[���h���X�e�b�v�����A�����ɕ`�悵�܂�
void PhysicSystem::Step(){
  step();
}
PhysicSystem::~PhysicSystem(){

  delFences();
  SAFE_DELETE(obs);
  SAFE_DELETE(dd);
  SAFE_DELETE(cfilter);
  delete mlis;
  mlis = nullptr;
  w->DestroyParticleSystem(particleSys);

  particleSys=nullptr;
  delete w;
  w = nullptr;
  //http://oshiro.bpe.es.osaka-u.ac.jp/people/staff/imura/computer/OpenGL/framebuffer_object/disp_content
}