#include "stdafx.h"
#include "MyContactFilter.h"
#include "Obstacle.h"
void collideAndDamage(b2Fixture* f, PSys sys, int32 ind){
  //�ђʑ����Ȃ���ł܂ł̎��Ԃ�ݒ肷�邾���A�݂����Ȃ̂��ł���
  auto udata = reinterpret_cast<EnemyData*>(f->GetUserData());
  auto p     = udata->Damage;
  sys->DestroyParticle(ind);
  *p         = *p + 1;
  if (udata->onhit)udata->onhit();
  
}
MyContactFilter::MyContactFilter(void) :StoppingFlame(0)
{
  
}
bool MyContactFilter::ShouldCollide(b2Fixture* f,b2Fixture* ff){
    //body�⃆�[�U�[�f�[�^�AAABB,�|�C���g�e�X�g���ł���
    //f->
    return false;
}
///���炭���̏����̃A�g�R���^�N�g�R�[���o�b�N���Ăяo��
////contactCallback�g���ق�������炵���̂ŕ���p�I�ȓ���͊֐��ɓ������Ă������ق�������
bool MyContactFilter::ShouldCollide(b2Fixture* f, PSys sys, int32 a){
  //sys->ParticleApplyForce(a,;
	//return true;
  if ( f->GetUserData() != nullptr ){
    auto pos = sys->GetPositionBuffer()[a];
    //sys->SetParticleFlags(a,0);
    if (StoppingFlame == 0)StoppingFlame = 2;
    else StoppingFlame--;
    collideAndDamage( f , sys , a );
    //�Ȃ���true�Ԃ��ƃV�~�����[�V���������肷��,false���Ƃ߂荞�񂾂�
    return true;//�ǂƂ͏Փ˂���
  }
  else return true;
}
bool MyContactFilter::ShouldCollide(PSys sys,int32 a,int32 b){
    //f->
    return true;
}

MyContactFilter::~MyContactFilter(void)
{
}
void MyContactListener::BeginContact(PSys sys, b2ParticleBodyContact* con)
{
  auto f = con->fixture;
  auto ind = con->index;
  auto ud = f->GetUserData();
  if (ud){
    auto udata = reinterpret_cast<EnemyData*>(ud);
    auto bdir = udata->browDir;
    if (udata->Name != "typ");
    else{ sys->ParticleApplyLinearImpulse(ind, bdir); }//�������傢�傫���Ȃ�����enamydata�p��
    sys->ParticleApplyForce(ind, bdir);
  }
}
