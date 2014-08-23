#include "stdafx.h"
#include "MyContactFilter.h"

void collideAndDamage(b2Fixture* f,PSys sys,int32 a){
    auto pdata=sys->GetUserDataBuffer()[a];
    //貫通属性なら消滅までの時間を設定するだけ、みたいなのができる
    sys->DestroyParticle(a);
    auto udata=f->GetUserData();
    //udata->AddDamage
}
MyContactFilter::MyContactFilter(void)
{
}
bool MyContactFilter::ShouldCollide(b2Fixture* f,b2Fixture* ff){
    //bodyやユーザーデータ、AABB,ポイントテストができる
    //f->
    return true;
}
///恐らくこの処理のアトコンタクトコールバックを呼び出す
//なので副作用的な動作は関数に逃がしておいたほうが無難
bool MyContactFilter::ShouldCollide(b2Fixture* f,PSys sys,int32 a){
    //f->
    //sys->ParticleApplyForce(a,;
    auto pos=sys->GetPositionBuffer()[a];//[sizeof(float32)*2*a];
    //sys->SetParticleFlags(a,0);
    collideAndDamage(f,sys,a);

    return true;//壁とは衝突する
}
bool MyContactFilter::ShouldCollide(PSys sys,int32 a,int32 b){
    //f->
    return true;
}

MyContactFilter::~MyContactFilter(void)
{
}
