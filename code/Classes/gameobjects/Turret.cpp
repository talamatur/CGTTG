#include "gameobjects\Turret.h"
#include "globals.h"


Turret* Turret::create(const std::string& filename)
{
	Turret* turret = new (std::nothrow) Turret();
	if (turret && turret->initWithFile(filename))
	{
		turret->autorelease();
		turret->setAnchorPoint(cocos2d::Vec2(0.0f, 0));
		return turret;
	}
	CC_SAFE_DELETE(turret);
	return nullptr;
}

Turret::Turret() : Sprite()
{
	// Nothing here
}

Turret::~Turret()
{
	// Nothing here
}