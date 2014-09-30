#include "stdafx.h"
#include "MyContactFilter.h"
#include "Obstacle.h"
void collideAndDamage(b2Fixture* f, PSys sys, int32 a){
  auto pdata = sys->GetUserDataBuffer()[a];
  //貫通属性なら消滅までの時間を設定するだけ、みたいなのができる
  sys->DestroyParticle(a);
  auto udata = reinterpret_cast<EnemyData*>(f->GetUserData());
  auto p     = udata->Damage;
  *p         = *p + 1;
  if (udata->PlayDamagedSound)udata->PlayDamagedSound();
}
MyContactFilter::MyContactFilter(void) :StoppingFlame(0)
{
  
}
bool MyContactFilter::ShouldCollide(b2Fixture* f,b2Fixture* ff){
    //bodyやユーザーデータ、AABB,ポイントテストができる
    //f->
    return false;
}
///恐らくこの処理のアトコンタクトコールバックを呼び出す
////contactCallback使うほうがそれらしいので副作用的な動作は関数に逃がしておいたほうが無難
bool MyContactFilter::ShouldCollide(b2Fixture* f, PSys sys, int32 a){
  //sys->ParticleApplyForce(a,;

  if ( f->GetUserData() != nullptr ){
    auto pos = sys->GetPositionBuffer()[a];
    //sys->SetParticleFlags(a,0);
    if (StoppingFlame == 0)StoppingFlame = 12;
    else StoppingFlame--;
    collideAndDamage( f , sys , a );
    //なぜかtrue返すとシミュレーションが安定する,falseだとめり込んだり
    return true;//壁とは衝突する
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
