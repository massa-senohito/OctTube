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
  auto b=gcnew Obstacle(w,V2(x,y),V2(sx,sy)); //������new�����Ƃ���list�ɂ���āAdelFences�̎��ɍ폜����̂�
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
  //fence�����������߂ɁAdelExactObstacle�̑O�Ƀf�B�X�p�b�`���K�v

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
void eachParticle(std::function<void(V2&)> f){
  int length=particleSys->GetParticleCount();
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
MyContactFilter* cfilter;
MyContactListener* mlis;
auto downGrav=-9.8f;
b2Vec2 grav(0.0,9.8f);
void PhysicSystem::makeOuterFence(int x,int y){
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
  particleSys->SetDensity(0.8);
  //�p�[�e�B�N���O���[�vdef��shape���w�肵��
  //���Ԃō폜����悤�ɂ���
  particleSys->SetDestructionByAge(true);
  dd = new DebugDraw();
  dd->SetFlags(
    b2Draw::e_particleBit | b2Draw::e_shapeBit );
  w->SetDebugDraw(dd);
  cfilter = new MyContactFilter;
  mlis = new MyContactListener;
  w->SetContactFilter(cfilter);
  w->SetContactListener(mlis);
  obs = gcnew Obss;
  makeOuterFence(73,53);
  ens = gcnew Enes;
  tys = gcnew Tys;
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
Typhoon* PhysicSystem::Blow(V2 pos, V2 dir, float rad){
  auto typ = new Typhoon(w,pos,dir,rad);
  tys->Add(typ);
  return typ;
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
    dd->DrawCircle( aa , 0.3 , col);
    dd->DrawCircle( ab , 0.3 , col);
    dd->DrawCircle( bb , 0.6 , col);
    dd->DrawCircle( ba , 0.6 , col);
    jl = jl->GetNext();
  }
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
#define deb
#ifndef deb
      //bod->GetFixtureList()->GetShape
#endif
    }
    //���ׂ̕`��@�܂��p����Draw�ĂԂ̂��Ó���
    //bod->GetFixtureList()
  }
#ifdef deb
  w->DrawDebugData();
#else

  eachParticle([](V2& v){dd->DrawCircle(v, 0.5, b2Color()); });
#endif


}
bool PhysicSystem::AllEnemyFired(){
  auto count = ens->Count;
  auto& e = (*ens);
  for (int i = 0; i < count; i++)
  {
    if (!e[i]->IsAllMeatFired())return false;
  }
  return true;
}
std::string tos(int i){ return std::to_string(i)+'\n'; }
std::string leg(int i){ return "leg"+tos(i)+": "; }

///���[���h���X�e�b�v�����A�����ɕ`�悵�܂�
void PhysicSystem::Step(){

#ifdef _MANAGED
  for each (PEnemy var in ens)
  {
    var->Update();
    //�񋓒��ɏ�����Ƃ߂�ǂ������̂�map<,bool>
    //if(var)
  }
#else
  std::for_each(ens->Data->begin(), ens->Data->end(), [](PEnemy i){i->Update(cfilter->StoppingFlame==0); });
#endif
  if( !AllEnemyFired())
    step();
  else{
    //���ʉ�ʕ\��
    auto data = ens->Data;
    auto gain = [](PEnemy e){return e->GainPoints(); };
    auto ps = gain(data->at(0));  //Map(*data, gain);
    glRasterPos2f(10, 10);
    auto clrstr = std::string("squid: ") + tos(ps[0])
      +leg(0)+ tos(ps[1]) + leg(1)+tos(ps[2]) + leg(2)+tos(ps[3]);
    if (ps[1] + ps[2] + ps[3] > 20){ clrstr += "\ntastes good."; }
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_10, reinterpret_cast<const unsigned char*>(clrstr.data()));
  }
  //jointDraw();
}
PhysicSystem::~PhysicSystem(){
  if(dd!=nullptr)
   delete dd;
  dd=nullptr;
  if (cfilter != nullptr)
    delete cfilter;
  cfilter = nullptr;
  delete mlis;
  mlis = nullptr;
  delFences();
  delete obs;
  obs=nullptr;
  ens->Clear();
  delete ens;
  ens=nullptr;
  delete tys;
  tys = nullptr;
  w->DestroyParticleSystem(particleSys);

  particleSys=nullptr;
  delete w;
  w = nullptr;
  //http://oshiro.bpe.es.osaka-u.ac.jp/people/staff/imura/computer/OpenGL/framebuffer_object/disp_content
}