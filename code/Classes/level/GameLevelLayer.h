#ifndef __GameLevelLayer_H__
#define __GameLevelLayer_H__

#include "gameobjects\Turret.h"

// Cocos
#include "cocos2d.h"
#include "ui/UIText.h"

class PlayerClass;

class GameLevelLayer : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(GameLevelLayer);
private:
	void update(float elapsedTime);

	void gameOver(bool wintheGame);
	bool checkForWin();

	// Populate
	void createEnemies();

	// Collision
	cocos2d::Vec2 tileCoordForPosition(cocos2d::Vec2 position);
	cocos2d::Rect tileRectFromTileCoords(cocos2d::Vec2 tileCoords);

	enum Direction
	{
		Direction_TopLeft = 1,
		Direction_Top = 2,
		Direction_TopRight = 3,
		Direction_Left = 4,
		Direction_Center = 5,
		Direction_Right = 6,
		Direction_BottomLeft = 7,
		Direction_Bottom = 8,
		Direction_BottomRight = 9
	};

	struct CollisionInfo
	{
		Direction direction;
		int collisionId;
		cocos2d::Vec2 tileOrigin;
		cocos2d::Vec2 tilePosition;
		cocos2d::Rect tileRect;
	};
	typedef std::vector<CollisionInfo*> Collisions;
	Collisions GameLevelLayer::getSurroundingTilesAtPosition(cocos2d::Vec2 position, cocos2d::TMXLayer* layer);
	CollisionInfo* createCollisionInfoForPosition(cocos2d::TMXLayer* layer, cocos2d::Vec2 position, int xOffset, int yOffset, Direction direction);
	void GameLevelLayer::handleHazardCollisions(PlayerClass* player);
	void checkForAndResolveCollision(PlayerClass* player);
	void setViewpointCenter(cocos2d::Vec2 position);

	// Input
	std::vector<cocos2d::EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	int signum(float x);

	// Button callbacks
	void onReplayButtonPressed(Ref* pSender);

private:
	// Data
	PlayerClass* mPlayer;
	cocos2d::TMXTiledMap* mMap;

	// Gamestates
	bool mGameOver;

	// Interface
	cocos2d::ui::Text* mDebugLabel;

	cocos2d::Sprite* mDebug_TopLeft;
	cocos2d::Sprite* mDebug_TopRight;
	cocos2d::Sprite* mDebug_BotomLeft;
	cocos2d::Sprite* mDebug_BotomRight;

	std::vector<Turret*> mTurrets;
};

#endif // __GameLevelLayer_H__
