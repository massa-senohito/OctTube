#include "stdafx.h"
#include "MyContactFilter.h"

void collideAndDamage(b2Fixture* f,PSys sys,int32 a){
    auto pdata=sys->GetUserDataBuffer()[a];
    //�ђʑ����Ȃ���ł܂ł̎��Ԃ�ݒ肷�邾���A�݂����Ȃ̂��ł���
    sys->DestroyParticle(a);
    auto udata=f->GetUserData();
    //udata->AddDamage
}
MyContactFilter::MyContactFilter(void)
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
  //f->
  //sys->ParticleApplyForce(a,;
  if (f != nullptr){
    auto pos = sys->GetPositionBuffer()[a];//[sizeof(float32)*2*a];
    //sys->SetParticleFlags(a,0);
    collideAndDamage(f, sys, a);

    return true;//�ǂƂ͏Փ˂���
  }
  else return false;
}
bool MyContactFilter::ShouldCollide(PSys sys,int32 a,int32 b){
    //f->
    return true;
}

MyContactFilter::~MyContactFilter(void)
{
}
