#include "stdafx.h"
#include <Box2D\Particle\b2ParticleSystem.h>
#include <Box2D\Box2D.h>
#include "DebugDraw.h"
#include "PhysicSystem.h"
#include <list>
#include <functional>
#define red 0.9f,0.2f,0.2f
using namespace std;//using string=std::string;

struct DeleteObj {
  template <typename T>
  void operator()(const T* ptr) const {
    if(ptr!=nullptr)
    delete ptr;ptr=nullptr;
  }
};
     //Shapes; 
//typedef std::basic_istringstream<char> istringstream;
//using vector<T>=std::vector<T>;
//�p�[�X�����f�[�^�̓����_���A�N�Z�X�ŏE���̂�vector�ł�����
//http://shnya.jp/blog/?p=195
std::vector<string> split(const string &str, char delim){
  istringstream iss(str); string tmp; std::vector<string> res;
  while(getline(iss, tmp, delim)) res.push_back(tmp);
  return res;
}
void splitTest(){
  auto ls=split("a,s,d,1,2,34,12",',');
  //for(string a=ls.
  std::cout << ls[2] << std::endl;
  std::cout << ls[4] << std::endl;
  std::cout << ls[6] << std::endl;

}
//float parse(System::String^ te){
//  //�߂�ǂ��̂Ńh�g�l�g��
//  return System::Convert::ToSingle(te);
//}

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

void movableFence(float32 x,float32 y,
                  float32 sx,float32 sy,
                  float32 tox,float32){
}
void PhysicSystem::addFence(float32 x,float32 y,float32 sx,float32 sy){
  auto b=gcnew Obstable(w,V2(x,y),V2(sx,sy)); //������new�����Ƃ���list�ɂ���āAdelFences�̎��ɍ폜����̂�
  //obs->push_back(b);
  obs->Add(b);
}

void delExactObstacle(Obstable^ b){
  w->DestroyBody(b->GetBody());
  //b->Dispose();
  delete b;
}
void PhysicSystem::delFences(){
  //std::for_each(obs->begin(),obs->end(),DeleteObj());
  //fence�����������߂ɁAdelExactObstacle�̑O�Ƀf�B�X�p�b�`���K�v
  //std::for_each(obs->begin(),obs->end(),delExactObstacle);
  for each (Obstable^ var in obs)
  {
    delExactObstacle(var);
  }
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

auto downGrav=-9.8f;
b2Vec2 grav(0.0,9.8f);
PhysicSystem::PhysicSystem(void)
{
  string csv("a,b");
  //toi=�^�C���I�u�C���p�N�g����
  w=new b2World(grav);
  
  //�ʒu�ƐڐG�t���O�Ȃǂ��������ĂȂ�
  //pd.position
  //�S���W��,�傫���A�F�̃~�L�V���O�Ȃ�
  particleSysDef=new b2ParticleSystemDef();
  particleSysDef->radius;
  //�\�ʒ���
  particleSysDef->surfaceTensionPressureStrength;
  //�S���W��
  particleSysDef->viscousStrength;
  //�����A�傫������Ɗۂ��ăX���[�X�ȗ��q�ɂȂ�
  particleSysDef->surfaceTensionNormalStrength;
  //�����������̒ǉ����́A�}�C�i�X�ɂ���Ɨ��q�����������^���ɂȂ�i���炭�����݂����ɂ������������Ƃ��ȁj
  particleSysDef->repulsiveStrength;
  
  particleSys=w->CreateParticleSystem(particleSysDef);
  //�p�[�e�B�N���O���[�vdef��shape���w�肵��
  //���Ԃō폜����悤�ɂ���
  particleSys->SetDestructionByAge(true);
  dd=new DebugDraw();
  dd->SetFlags(b2Draw::e_particleBit|b2Draw::e_shapeBit);
  w->SetDebugDraw(dd);
  //w->S
  obs=gcnew Obs;
}
inline PShape createBox(float32 hx,float32 hy,const b2Vec2& cen){
  auto shape=new b2PolygonShape();
  shape->SetAsBox(hx,hy,cen,0);
  return shape;
}
void makeParticle(float x,float y){
  b2ParticleGroupDef d;//d.particleCount=5; �T���v������ׂĂ�����
  uint8 zero=0;uint8 one=1;
  d.userData;
  b2ParticleColor b(zero,one,zero,one);
  d.color=b;
  //auto circle=new b2CircleShape;
  //circle->m_radius=
  d.shape=createBox(3,3,V2(x,y));
  d.flags=b2_tensileParticle;
  d.position=b2Vec2(x,y);
  auto pg=particleSys->CreateParticleGroup(d);
}
void makeFlyBox(float x,float y){
  auto d=new b2BodyDef();
  d->userData;
  d->position=V2(x,y);
  d->type=b2_dynamicBody;
  d->linearVelocity;
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
    b2ParticleDef d;
    b2ParticleColor bc(0,0,1,1);
    d.color=bc;
    d.velocity=V2(vx,vy);
    d.position=V2(x,y);
    d.flags=b2_tensileParticle;
    auto par=particleSys->CreateParticle(d);
}
//�p�[�e�B�N�����p������ KomeParticle , StreamParticle 
//StreamParticle�͐��̃p�[�e�B�N���O���[�v�A
//rigidparticle=����Ȃ��p�[�e�B�N���O���[�v
int PhysicSystem::MakeParticle(float x,float y){
    makeParticle(x,y);
  //pg-> rigidparticle �łȂ���Position�Ƃ��Ă��Ӗ��̂Ȃ����W�ɂȂ�i���Ԃ�d�S���o�邪�AGetCenter����̂ł��������j
  return 0;//pg->GetParticleCount();
}
void step(){
  //�O���[�v���쐬����Ƒ�����A
  //std::cout << particleSys->GetParticleGroupCount() << std::endl;
  //�O���[�v�ň�C�ɑ�����
  //std::cout << particleSys->GetParticleCount() << std::endl;
  //�������p�[�e�B�N���V�~�����[�V�����̃V�~�����[�V�����Ȃ��r�I�傫�ȏd��
  // b2CalculateParticleIterations helps to determine the number.
  float hz=60.f;
  int32 pi=b2CalculateParticleIterations(grav.y,0.06f,1/hz);
  w->Step(1/hz,8,3,pi);
  
  w->DrawDebugData();
}

///���[���h���X�e�b�v�����A�����ɕ`�悵�܂�
void PhysicSystem::Step(){
  step();
}
PhysicSystem::~PhysicSystem(){
  if(dd!=nullptr)
   delete dd;
  dd=nullptr;
  delFences();
  delete obs;
  w->DestroyParticleSystem(particleSys);
  particleSys=nullptr;
  //http://oshiro.bpe.es.osaka-u.ac.jp/people/staff/imura/computer/OpenGL/framebuffer_object/disp_content
}
//float getParticlePos() //ref class�ō��΂ǂƂ˂Ƃɂق��肱�߂邶���