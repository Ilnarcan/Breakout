#ifndef _COLLIDER_H
#define _COLLIDER_H


class Collider
{
public:
	Collider(bool trigger) : is_trigger(trigger)
	{
	}

	Collider() : is_trigger(true)
	{
	}

	virtual ~Collider()
	{
	}

private:
	bool is_trigger;
};

#endif
