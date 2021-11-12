#ifndef _SPHERE_COLLIDER_H
#define _SPHERE_COLLIDER_H

#include "collider.h"

class SphereCollider final : public Collider
{
public:
	SphereCollider(float _radius, bool trigger) : Collider(trigger), radius(_radius)
	{
	}

	~SphereCollider() override
	{
	};

	float get_radius() const { return radius; };

private:
	float radius;
};

#endif
