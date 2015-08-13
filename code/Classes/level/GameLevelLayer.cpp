#include "level/GameLevelLayer.h"
#include "gameobjects/PlayerClass.h"
#include "SimpleAudioEngine.h"
#include "globals.h"

USING_NS_CC;

namespace detail
{

	cocos2d::Rect getIntersectRect(const cocos2d::Rect& a, const cocos2d::Rect& b)
	{
		// Find topleft point
		bool loopEnd = false;
		cocos2d::Vec2 topLeftPoint;
		int endx = -1;
		int endy = -1;

		int width = -1;
		int height = -1;

		for (int y = 0; y <= b.size.height; ++y)
		{
			for (int x = 0; x <= b.size.width; ++x)
			{
				cocos2d::Vec2 point(b.origin.x + x, b.origin.y + y);
				if (a.containsPoint(point))
				{
					// Found topleft point, yeah
					loopEnd = true;
					topLeftPoint = point;
					endx = x;
					endy = y;

					width = 0;
					height = 0/*b.size.height - y*/;
					break;
				}
			}
			if (loopEnd)
			{
				break;
			}
		}


		for (int y = endy; y <= b.size.height; ++y)
		{
			cocos2d::Vec2 point(topLeftPoint.x, topLeftPoint.y + y);
			if (!a.containsPoint(point))
			{
				// Found botomleft point, yeah
				endy = y;
				break;
			}
			else
			{
				++height;
			}
		}
		
		for (int x = endx; x <= b.size.width; ++x)
		{
			cocos2d::Vec2 point(topLeftPoint.x + x, topLeftPoint.y);
			if (!a.containsPoint(point))
			{
				// Found topright point, yeah
				endx = x;
				break;
			}
			else
			{
				++width;
			}
		}


		return cocos2d::Rect(topLeftPoint.x, topLeftPoint.y, width, height);
	}
}

Scene* GameLevelLayer::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = GameLevelLayer::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameLevelLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	mGameOver = false;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(GameLevelLayer::menuCloseCallback, this));

	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width / 2,
		origin.y + closeItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	// Debug label
	mDebugLabel = ui::Text::create("Player Pos", "fonts/Marker Felt.ttf", 24);
	mDebugLabel->setPosition(Vec2(64,
		origin.y + visibleSize.height - mDebugLabel->getContentSize().height));
	this->addChild(mDebugLabel, 1);

	// add splash screen"
	auto sprite = Sprite::create("background/backdrop_m01.png");
	// position the sprite on the center of the screen
	sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);
	
	// Load the map
	mMap = new TMXTiledMap();
	mMap->initWithTMXFile("level/m01.tmx");
	addChild(mMap);
	createEnemies();

	mPlayer = PlayerClass::create("char/hero_idle.png");
	mPlayer->setPosition(Vec2(64 * 5, (64 * 1)+1));
	mMap->addChild(mPlayer, 15);	// Adding the player to the map!

	// load the animations
	AnimationCache::getInstance()->addAnimationsWithFile("char/hero_move.plist");

	// adding a update function
	this->schedule(schedule_selector(GameLevelLayer::update));
	this->setKeyboardEnabled(true);

	// Play audio
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/level1.mp3");

	// Adding debug gfx for the player
	mDebug_TopLeft = Sprite::create("debug/dot_red.png");
	mDebug_TopLeft->setPosition(cocos2d::Vec2(0, 0));
	mMap->addChild(mDebug_TopLeft, 20);
	
	mDebug_TopRight = Sprite::create("debug/dot_blue.png");
	mDebug_TopRight->setPosition(cocos2d::Vec2(0, 0));
	mMap->addChild(mDebug_TopRight, 20);

	mDebug_BotomLeft = Sprite::create("debug/dot_orange.png");
	mDebug_BotomLeft->setPosition(cocos2d::Vec2(0, 0));
	mMap->addChild(mDebug_BotomLeft, 20);

	mDebug_BotomRight = Sprite::create("debug/dot_green.png");
	mDebug_BotomRight->setPosition(cocos2d::Vec2(0, 0));
	mMap->addChild(mDebug_BotomRight, 20);
	return true;
}


void GameLevelLayer::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void GameLevelLayer::update(float elapsedTime)
{
	if (mGameOver)
	{
		if (std::find(heldKeys.begin(), heldKeys.end(), globals::UP_ARROW) != heldKeys.end() ||
			std::find(heldKeys.begin(), heldKeys.end(), globals::KEY_SPACE) != heldKeys.end())
		{
			// For easy use
			this->onReplayButtonPressed(nullptr);
		}
		return;
	}

	// Update player input
	if (std::find(heldKeys.begin(), heldKeys.end(), globals::RIGHT_ARROW) != heldKeys.end() ||
		std::find(heldKeys.begin(), heldKeys.end(), globals::KEY_D) != heldKeys.end())
	{
		mPlayer->setPlayerMoveRight();
	}

	if (std::find(heldKeys.begin(), heldKeys.end(), globals::LEFT_ARROW) != heldKeys.end() ||
		std::find(heldKeys.begin(), heldKeys.end(), globals::KEY_A) != heldKeys.end())
	{
		mPlayer->setPlayerMoveLeft();
	}
	
	if (std::find(heldKeys.begin(), heldKeys.end(), globals::UP_ARROW) != heldKeys.end() ||
		std::find(heldKeys.begin(), heldKeys.end(), globals::KEY_SPACE) != heldKeys.end())
	{
		mPlayer->setPlayerJump();
	}
	//// clamp the velocity to the maximum, x-axis only
	//if (std::abs(mPlayer->mVelocity.x) > globals::PLAYER_MAX_VELOCITY) {
	//	mPlayer->mVelocity.x = signum(mPlayer->mVelocity.x) * globals::PLAYER_MAX_VELOCITY;
	//}
	//// clamp the velocity to 0 if it's < 1, and set the state to standing
	//if (std::abs(mPlayer->mVelocity.x) < 1) {
	//	mPlayer->mVelocity.x = 0;
	//	//if (player->grounded) {
	//	//	player->state = Player::State::Standing;
	//	//}
	//}


	mPlayer->update(elapsedTime);
	
	handleHazardCollisions(mPlayer);
	if (checkForWin())
	{
		// Handle the case that the player is fall out of the level
		return;
	}
	
	checkForAndResolveCollision(mPlayer);
	
	// Adjust the camera to the player
	setViewpointCenter(mPlayer->getPosition());
	
	// Show player collision debug
	cocos2d::Rect playerCollision = mPlayer->getCollisionBoundingBox();
	mDebug_BotomLeft->setPosition(playerCollision.origin);
	mDebug_BotomRight->setPosition(playerCollision.origin.x + playerCollision.size.width, playerCollision.origin.y);
	mDebug_TopLeft->setPosition(playerCollision.origin.x, playerCollision.origin.y + playerCollision.size.height);
	mDebug_TopRight->setPosition(playerCollision.origin.x + playerCollision.size.width, playerCollision.origin.y + playerCollision.size.height);

	cocos2d::Vec2 tilePositionPlayer = tileCoordForPosition(mPlayer->getPosition());
	std::stringstream builder;
	builder.precision(2);
	builder << std::fixed;
	builder << "Player Pos: " << tilePositionPlayer.x << "/" << tilePositionPlayer.y << "\n";
	builder << "Player Collision: " << playerCollision.origin.x << "/" << playerCollision.origin.y << "\n";
	builder << "Player Speed: " << mPlayer->mVelocity.x << "/" << mPlayer->mVelocity.y;
	mDebugLabel->setText(builder.str());
}

bool GameLevelLayer::checkForWin()
{
	cocos2d::Vec2 playerPosition = tileCoordForPosition(mPlayer->getPosition());
	if (playerPosition.x > 80)
	{
		gameOver(true);
		return true;
	}
	// Maybe the previosly collision checks detect a falling in the ground
	return mGameOver;
}

void GameLevelLayer::createEnemies()
{
	// Scan the level for information about enemeys
	cocos2d::TMXLayer* enemyLayer = mMap->getLayer("enemy");
	if (enemyLayer)
	{
		for (int x = 0; x < mMap->getMapSize().width; ++x)
		{
			for (int y = 0; y < mMap->getMapSize().height; ++y)
			{
				cocos2d::Vec2 point(x, y);
				int tgid = enemyLayer->getTileGIDAt(point);
				if (tgid != 0)
				{
					Turret* turret = Turret::create("char/enemy_robot.png");
					if (turret)
					{
						turret->setPosition(x * mMap->getTileSize().width, ((mMap->getMapSize().height - y -1) * mMap->getTileSize().height) + 1);
						mMap->addChild(turret, 5);
						mTurrets.push_back(turret);
					}
				}
			}
		}
	}
}

void GameLevelLayer::gameOver(bool winTheLevel)
{
	mGameOver = true;
	std::string displayedText = (winTheLevel ? "You Won, far enough. Hero of the universe!" : "You lose, try harder!");

	auto diedLabel = cocos2d::LabelTTF::create(displayedText, "Marker Felt", 50);
	diedLabel->setPosition(cocos2d::Vec2(800, 600));
	
	auto slideIn = cocos2d::CCMoveBy::create(1.0, cocos2d::Vec2(0, 250));
	cocos2d::CCMenuItemImage* replay = cocos2d::CCMenuItemImage::create("replay.png", "replay.png", "replay.png", CC_CALLBACK_1(GameLevelLayer::onReplayButtonPressed, this));

	auto menu = cocos2d::CCMenu::create(replay, nullptr);

	addChild(menu, 20);
	addChild(diedLabel);

	menu->runAction(slideIn);

	// Play audio
	if (!winTheLevel)
	{
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("audio/hurt.wav");
	}
}

cocos2d::Vec2 GameLevelLayer::tileCoordForPosition(cocos2d::Vec2 position)
{
	float x = floor(position.x / mMap->getTileSize().width);
	float levelHeightInPixels = mMap->getMapSize().height * mMap->getTileSize().height;
	float y = floor((levelHeightInPixels - position.y) / mMap->getTileSize().height);
	return cocos2d::Vec2(x, y);
}

cocos2d::Rect GameLevelLayer::tileRectFromTileCoords(cocos2d::Vec2 tileCoords)
{
	float levelHeightInPixels = mMap->getMapSize().height * mMap->getTileSize().height;
	cocos2d::Vec2 origin(tileCoords.x * mMap->getTileSize().width, levelHeightInPixels - ((tileCoords.y + 1) * mMap->getTileSize().height));
	return cocos2d::Rect(origin.x, origin.y, mMap->getTileSize().width, mMap->getTileSize().height);
}

GameLevelLayer::Collisions GameLevelLayer::getSurroundingTilesAtPosition(cocos2d::Vec2 position, TMXLayer* layer)
{
	cocos2d::Vec2 playerPosition = tileCoordForPosition(position);
	Collisions result;

	// If the player is under the ground, end game
	if (playerPosition.y > (mMap->getMapSize().height - 1))
	{
		gameOver(false);
		return result;
	}
	
	// Give the collision info a priority order. Solving the collision for the vertical and horizontal, also solves the collision for the diagonal
	// 1 2 3     5 2 6
	// 4 5 6  => 3 x 4
	// 7 8 9     7 1 8

	// Player is 1 tiles width and 2 tiles height
	const int playerTileSizeX = 1;
	const int playerTileSizeY = 2;

	// Add all tiles below the player
	{
		if ((playerPosition.y + 1) < mMap->getMapSize().height)
		{
			result.push_back(createCollisionInfoForPosition(layer, playerPosition, 0, +1, Direction_Bottom));
		}
		else
		{
			int breakMe = 1;
		}
	}

	// Add tiles over the player
	{
		if (playerPosition.y >= 0)
		{
			result.push_back(createCollisionInfoForPosition(layer, playerPosition, 0, -(playerTileSizeX + 1), Direction_Top));
		}
	}
	
	{
		// Add tiles left to the player
		if (playerPosition.x > 0)
		{
			for (int i = 0; i < playerTileSizeY; ++i)
			{
				result.push_back(createCollisionInfoForPosition(layer, playerPosition, -1, -i, Direction_Left));
			}
		}

		// Add tiles right to the player
		if (playerPosition.x < mMap->getMapSize().width - 1)
		{
			for (int i = 0; i < playerTileSizeY; ++i)
			{
				result.push_back(createCollisionInfoForPosition(layer, playerPosition, 1, -i, Direction_Right));
			}
		}
	}
	// Topleft
	result.push_back(createCollisionInfoForPosition(layer, playerPosition, -1, -(playerTileSizeX + 1), Direction_TopLeft));
	// Topright
	result.push_back(createCollisionInfoForPosition(layer, playerPosition, 1, -(playerTileSizeX + 1), Direction_TopRight));

	// Bottomleft
	result.push_back(createCollisionInfoForPosition(layer, playerPosition, -1, +1, Direction_BottomLeft));
	// Bottomright
	result.push_back(createCollisionInfoForPosition(layer, playerPosition, 1, +1, Direction_BottomRight));

	// Add tiles left of the player
	//result.push_back(createCollisionInfoForPosition(layer, playerPosition, -1, 0, Direction_Left));
	// Add tiles left of the player
	//result.push_back(createCollisionInfoForPosition(layer, playerPosition, +1, 0, Direction_Right));


	// This uses only one tile for the player collision box, the result is ok if everything has the same size
	//Collisions resultMapOrder;
	//// Build up information about all surrinding tiles
	//for (int i = 0; i < 9; i++)
	//{
	//	int column = i % 3;
	//	int row = (int)(i / 3);
	//	cocos2d::Vec2 tilePos = cocos2d::Vec2(playerPosition.x + (column - 1), playerPosition.y + (row - 1));

	//	// Get tile information
	//	int tgid = layer->getTileGIDAt(tilePos);

	//	cocos2d::Rect tileRect = tileRectFromTileCoords(tilePos);

	//	CollisionInfo* collisionInfo = new CollisionInfo();
	//	collisionInfo->collisionOrder = i+1;
	//	collisionInfo->collisionId = tgid;
	//	collisionInfo->tileOrigin = cocos2d::Vec2(tileRect.origin.x, tileRect.origin.y);
	//	collisionInfo->tilePosition = tilePos;
	//	collisionInfo->tileRect = tileRect;
	//	resultMapOrder.push_back(collisionInfo);
	//}

	//// Give the collision info a priority order. Solving the collision for the vertical and horizontal, also solves the collision for the diagonal
	//// 1 2 3     5 2 6
	//// 4 5 6  => 3 x 4
	//// 7 8 9     7 1 8

	//Collisions result;
	//result.push_back(resultMapOrder[7]);
	//result.push_back(resultMapOrder[1]);
	//result.push_back(resultMapOrder[3]);
	//result.push_back(resultMapOrder[5]);
	//result.push_back(resultMapOrder[0]);
	//result.push_back(resultMapOrder[2]);
	//result.push_back(resultMapOrder[6]);
	//result.push_back(resultMapOrder[8]);

	return result;
}

GameLevelLayer::CollisionInfo* GameLevelLayer::createCollisionInfoForPosition(cocos2d::TMXLayer* layer, cocos2d::Vec2 position, int xOffset, int yOffset, Direction direction)
{
	cocos2d::Vec2 tilePos(position.x + xOffset, position.y + yOffset);

	// Get tile information
	int tgid = layer->getTileGIDAt(tilePos);

	cocos2d::Sprite* tilesprite = layer->getTileAt(tilePos);
	if (nullptr != tilesprite)
	{
		//tilesprite->setColor(cocos2d::Color3B::BLUE);
	}

	cocos2d::Rect tileRect = tileRectFromTileCoords(tilePos);

	CollisionInfo* collisionInfo = new CollisionInfo();
	collisionInfo->direction = direction;
	collisionInfo->collisionId = tgid;
	collisionInfo->tileOrigin = cocos2d::Vec2(tileRect.origin.x, tileRect.origin.y);
	collisionInfo->tilePosition = tilePos;
	collisionInfo->tileRect = tileRect;
	return collisionInfo;
}

void GameLevelLayer::handleHazardCollisions(PlayerClass* player)
{
	cocos2d::TMXLayer* walls = mMap->getLayer("spikes");
	Collisions tiles = getSurroundingTilesAtPosition(player->getPosition(), walls);
	cocos2d::Rect pRect = player->getCollisionBoundingBox();

	for (CollisionInfo* collisionInfo : tiles)
	{
		int gid = collisionInfo->collisionId;

		if (gid != 0 && pRect.intersectsRect(collisionInfo->tileRect))
		{
			gameOver(false);
		}
	}

	// Delete the allocated collision memory
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		delete tiles[i];
	}
}

void GameLevelLayer::checkForAndResolveCollision(PlayerClass* player)
{
	cocos2d::TMXLayer* walls = mMap->getLayer("walls");
	Collisions tiles = getSurroundingTilesAtPosition(player->getPosition(), walls);

	player->mOnGround = false;

	cocos2d::Vec2 playerPosition = tileCoordForPosition(player->getPosition());

	for (CollisionInfo* collisionInfo : tiles)
	{
		cocos2d::Rect pRect = player->getCollisionBoundingBox();

		int gid = collisionInfo->collisionId;

		if (gid != 0) 
		{
			int breakMe = 1;
			//CGRect tileRect = CGRectMake([[dic objectForKey : @"x"] floatValue], [[dic objectForKey : @"y"] floatValue], map.tileSize.width, map.tileSize.height); //4
			if (pRect.intersectsRect(collisionInfo->tileRect))
			{
				cocos2d::Rect intersects = detail::getIntersectRect(pRect, collisionInfo->tileRect);

				Direction direction = collisionInfo->direction;

				if (direction == Direction_Bottom)
				{
					player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x, ceilf(player->mDesiredPosition.y + intersects.size.height));
					player->mVelocity.y = 0.f; // Reset the y velocity
					player->mOnGround = true;
				}
				else if (direction == Direction_Top)
				{
					player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x, player->mDesiredPosition.y - intersects.size.height -1);
					player->mVelocity.y = 0.f; // Reset the y velocity
				}
				else if (direction == Direction_Left)
				{
					player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x + intersects.size.width, player->mDesiredPosition.y);
				}
				else if (direction == Direction_Right)
				{
					player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x - intersects.size.width, player->mDesiredPosition.y);
				}
				else
				{
					if (intersects.size.width > intersects.size.height) { //3
						player->mVelocity.y = 0.f; // Reset the y velocity
						//tile is diagonal, but resolving collision vertically
						float intersectionHeight;
						if (direction == Direction_BottomLeft || direction == Direction_BottomRight) //if (direction > 5) { // TODO(mk) Check this value 5 here
						{
							intersectionHeight = intersects.size.height;
							player->mOnGround = true;
						}
						else
						{
							intersectionHeight = -intersects.size.height;
						}
						player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x, player->mDesiredPosition.y + intersects.size.height);
					}
					else
					{
						//tile is diagonal, but resolving horizontally
						float resolutionWidth;
						if (direction == Direction_BottomLeft || direction == Direction_BottomRight)
						{
							resolutionWidth = intersects.size.width;
						}
						else {
							resolutionWidth = -intersects.size.width;
						}
						player->mDesiredPosition = cocos2d::Vec2(player->mDesiredPosition.x, player->mDesiredPosition.y + resolutionWidth);
					}
				}
			}
		}
	}
	if (player->mDesiredPosition.y < 65)
	{
		int breakMe = 1;
	}

	player->setPosition(player->mDesiredPosition);

	// Delete the allocated collision memory
	for (size_t i = 0; i < tiles.size(); ++i)
	{
		delete tiles[i];
	}
}

void GameLevelLayer::setViewpointCenter(cocos2d::Vec2 position)
{
	cocos2d::Size winSize = Director::getInstance()->getWinSize();

	int x = MAX(position.x, winSize.width / 2);
	int y = MAX(position.y, winSize.height / 2);
	x = MIN(x, (mMap->getMapSize().width * mMap->getTileSize().width)
		- winSize.width / 2);
	y = MIN(y, (mMap->getMapSize().height * mMap->getTileSize().height)
		- winSize.height / 2);
	cocos2d::Vec2 actualPosition(x, y);

	cocos2d::Vec2 centerOfView(winSize.width / 2, winSize.height / 2);
	cocos2d::Vec2 viewPoint(centerOfView - actualPosition);
	mMap->setPosition(viewPoint);
}

void GameLevelLayer::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	if (std::find(heldKeys.begin(), heldKeys.end(), keyCode) == heldKeys.end()){
		heldKeys.push_back(keyCode);
	}
}

void GameLevelLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	heldKeys.erase(std::remove(heldKeys.begin(), heldKeys.end(), keyCode), heldKeys.end());
}

void GameLevelLayer::onReplayButtonPressed(Ref* pSender)
{
	auto scene = GameLevelLayer::createScene();
	Director::getInstance()->replaceScene(scene);
}

int GameLevelLayer::signum(float x)
{
	if (x > 0.0L)
		return 1.0L;
	else if (x < 0.0L)
		return -1.0L;
	else
		return 0.0L;
}