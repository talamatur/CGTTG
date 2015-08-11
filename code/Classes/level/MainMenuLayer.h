#ifndef __MAINMENULAYER_SCENE_H__
#define __MAINMENULAYER_SCENE_H__

#include "cocos2d.h"

class MainMenuLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

private:
	void update(float elapsedTime);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	void startGameCallback(cocos2d::Ref* pSender);


	// Input
	std::vector<cocos2d::EventKeyboard::KeyCode> heldKeys;
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // implement the "static create()" method manually
	CREATE_FUNC(MainMenuLayer);
};

#endif // __MAINMENULAYER_SCENE_H__
