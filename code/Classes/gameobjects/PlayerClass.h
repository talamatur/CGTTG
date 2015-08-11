#ifndef __PLAYERCLASS_H__
#define __PLAYERCLASS_H__

#include "cocos2d.h"
#include "2d\CCSprite.h"

class PlayerClass : public cocos2d::Sprite
{
public:
	static PlayerClass* create(const std::string& filename);

	PlayerClass();
	virtual ~PlayerClass();

	virtual void update(float elapsedTime) override;

	cocos2d::Rect getCollisionBoundingBox();

	void setPlayerMoveRight();
	void setPlayerMoveLeft();
	void setPlayerJump();

public:
	// Data
	cocos2d::Vec2 mDesiredPosition;
	cocos2d::Vec2 mVelocity;
	bool mOnGround;	// "true" if the object is on ground

	bool mMovedRightPressed;
	bool mMovedLeftPressed;
	bool mMovedJumpPressed;

	enum MoveDirection
	{
		MoveDirection_Left,
		MoveDirection_None,
		MoveDirection_Right
	};
	MoveDirection mMovementDirection;

	enum AnimationState
	{
		AnimationState_Idle,
		AnimationState_Move,
		AnimationState_Jumping
	};
	AnimationState mAnimationState;

};

#endif // __PLAYERCLASS_H__