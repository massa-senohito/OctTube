#pragma once
#include <Box2D\Dynamics\b2WorldCallbacks.h>
#include <Box2D\Dynamics\b2Fixture.h>
#include <Box2D\Particle\b2ParticleSystem.h>
typedef b2Fixture* Fixture;
typedef b2ParticleSystem* PSys;
class MyContactFilter :
    public b2ContactFilter
{
public:
  int StoppingFlame;
    MyContactFilter(void);
    bool ShouldCollide(b2Fixture*,b2Fixture*);
    bool ShouldCollide(b2Fixture*,PSys,int32);
    bool ShouldCollide(PSys,int32,int32);
    ~MyContactFilter(void);
};
class MyContactListener :public b2ContactListener{
  void BeginContact(PSys , b2ParticleBodyContact* );
};
//  �O���[�v�C���f�b�N�X��������a��0�łȂ��Ȃ�Aa�������ƐڐG
//if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
//	{
//		return filterA.groupIndex > 0;
//	}
//�@a�̃}�X�N��B�̃J�e�S����and a�̃J�e�S����b�̃}�X�N and��0�łȂ��Ȃ�ڐG
//	bool collide = (filterA.maskBits & filterB.categoryBits) != 0 
//        && (filterA.categoryBits & filterB.maskBits) != 0;
//	return collide;