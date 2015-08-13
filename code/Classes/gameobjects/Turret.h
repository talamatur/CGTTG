#ifndef __TURRET_H__
#define __TURRET_H__

#include "cocos2d.h"
#include "2d\CCSprite.h"

class Turret : public cocos2d::Sprite
{
public:
	static Turret* create(const std::string& filename);

	Turret();
	virtual ~Turret();

};

#endif // __TURRET_H__