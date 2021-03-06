#include "stdafx.h"
#include "MyContactFilter.h"
#include "Obstacle.h"
#include "PhysicSystem.h"
void collideAndDamage(b2Fixture* f, PSys sys, int32 ind){
  //貫通属性なら消滅までの時間を設定するだけ、みたいなのができる
  auto udata = reinterpret_cast<EnemyData*>(f->GetUserData());
  auto p     = udata->Damage;
  //コンタクト処理が終わらないうちにパーティクルが消えるので問題かも
  //sys->DestroyParticle(ind);
  *p         = *p + 1;
  if (udata->onhit)udata->onhit();
  
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
bool MyContactFilter::ShouldCollide(b2Fixture* f, PSys sys, int32 ind){
  //sys->ParticleApplyForce(a,;
	//return true;
  if ( f->GetUserData() != nullptr ){
    auto pos = sys->GetPositionBuffer()[ind];
    //sys->SetParticleFlags(a,0);
    if (StoppingFlame == 0)StoppingFlame = 2;
    else StoppingFlame--;
    auto pudata= reinterpret_cast<ParticleData*>
      ( sys->GetUserDataBuffer()[ind]);
    if (!pudata->IsHitEnemy)return false;
    pudata->IsHitEnemy = false;
    collideAndDamage( f , sys , ind );
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
void MyContactListener::BeginContact(PSys sys, b2ParticleBodyContact* con)
{
  auto f = con->fixture;
  auto ind = con->index;
  auto ud = f->GetUserData();
  if (ud){
    auto udata = reinterpret_cast<EnemyData*>(ud);
    auto bdir = udata->browDir;
    if (udata->Name != "typ");
    else{ sys->ParticleApplyLinearImpulse(ind, bdir); }//もうちょい大きくなったらenamydata継承
    sys->ParticleApplyForce(ind, bdir);
  }
}
