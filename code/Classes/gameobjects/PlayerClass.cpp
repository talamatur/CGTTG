#include "gameobjects\PlayerClass.h"
#include "globals.h"

// Cocos
#include "SimpleAudioEngine.h"

PlayerClass* PlayerClass::create(const std::string& filename)
{
	PlayerClass *playerClass = new (std::nothrow) PlayerClass();
	if (playerClass && playerClass->initWithFile(filename))
	{
		playerClass->autorelease();
		playerClass->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
		return playerClass;
	}
	CC_SAFE_DELETE(playerClass);
	return nullptr;
}

PlayerClass::PlayerClass() : Sprite(),
	mVelocity(0.f,0.f),
	mOnGround(true),
	mAnimationState(AnimationState_Idle),
	mMovedRightPressed(false),
	mMovedLeftPressed(false),
	mMovedJumpPressed(false)
{
	// Nothing here
}

PlayerClass::~PlayerClass()
{
	// Nothing here
}

void PlayerClass::update(float elapsedTime)
{
	cocos2d::Vec2 gravity = cocos2d::Vec2(0.0, globals::PLAYER_GRAVITY);

	// Apply Gravity to player
	cocos2d::Vec2 gravityStep = gravity *  elapsedTime;

	mVelocity += gravityStep;
	//mVelocity.x *= globals::DAMPING;

	cocos2d::Vec2 forwardStep = cocos2d::Vec2(globals::PLAYER_MAX_VELOCITY, 0.0f) * elapsedTime;
	mVelocity.x *= globals::DAMPING;


	float jumpCutoff = 150.0;

	if (mMovedJumpPressed && mOnGround)
	{
		mVelocity.y = + globals::PLAYER_JUMP_VELOCITY;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/jump.wav");
	}
	else if (!mMovedJumpPressed && mVelocity.y > jumpCutoff)
	{
		mVelocity.y = jumpCutoff;
	}
	if (mMovedLeftPressed)
	{
		mVelocity -= forwardStep;
		//if (player->grounded){
		//	player->state = Player::State::Walking;
		//}
		//player->facingRight = false;
	}
	if (mMovedRightPressed)
	{
		mVelocity += forwardStep;
		//if (player->grounded){
		//	player->state = Player::State::Walking;
		//}
		//player->facingRight = true;
	}

	// Clamp the Velocity
	mVelocity.x = cocos2d::clampf(mVelocity.x, -globals::PLAYER_MAX_VELOCITY, globals::PLAYER_MAX_VELOCITY);
	mVelocity.y = cocos2d::clampf(mVelocity.y, globals::PLAYER_GRAVITY, globals::PLAYER_JUMP_VELOCITY);

	cocos2d::Vec2 stepVelocity = mVelocity * elapsedTime;
	mDesiredPosition = getPosition() + stepVelocity;

	mDesiredPosition.x = ceilf(mDesiredPosition.x);
	mDesiredPosition.y = ceilf(mDesiredPosition.y);

	// animation
	if (abs(mVelocity.x) > 1)
	{
		//mAnimationState = AnimationState_Move;

		//if (this->numberOfRunningActions() == 0)
		//{
		//	auto animation = cocos2d::AnimationCache::getInstance()->animationByName("walk");
		//	auto animate = cocos2d::Animate::create(animation);
		//	runAction(animate);
		//}

		//this->setFlippedX((mVelocity.x > 1));
	}
	else
	{
		mAnimationState = AnimationState_Idle;
		stopAllActions();
	}

	mMovedLeftPressed = false;
	mMovedRightPressed = false;
	mMovedJumpPressed = false;
}

cocos2d::Rect PlayerClass::getCollisionBoundingBox()
{
	cocos2d::Rect collisionBox(this->getBoundingBox());
	cocos2d::Vec2 diff(mDesiredPosition - getPosition());
	collisionBox.origin += diff;
	return collisionBox;
}

void PlayerClass::setPlayerMoveRight()
{
	mMovedRightPressed = true;
}

void PlayerClass::setPlayerMoveLeft()
{
	mMovedLeftPressed = true;
}

void PlayerClass::setPlayerJump()
{
	mMovedJumpPressed = true;
}