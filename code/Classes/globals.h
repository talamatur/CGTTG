#pragma once

#include "base/CCEventKeyboard.h"

namespace globals
{
	// KEYBOARD
	const cocos2d::EventKeyboard::KeyCode ENTER = cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER;
	const cocos2d::EventKeyboard::KeyCode UP_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW;
	const cocos2d::EventKeyboard::KeyCode DOWN_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW;
	const cocos2d::EventKeyboard::KeyCode LEFT_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW;
	const cocos2d::EventKeyboard::KeyCode RIGHT_ARROW = cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW;

	const cocos2d::EventKeyboard::KeyCode KEY_W = cocos2d::EventKeyboard::KeyCode::KEY_W;
	const cocos2d::EventKeyboard::KeyCode KEY_A = cocos2d::EventKeyboard::KeyCode::KEY_A;
	const cocos2d::EventKeyboard::KeyCode KEY_S = cocos2d::EventKeyboard::KeyCode::KEY_S;
	const cocos2d::EventKeyboard::KeyCode KEY_D = cocos2d::EventKeyboard::KeyCode::KEY_D;

	const cocos2d::EventKeyboard::KeyCode KEY_SPACE = cocos2d::EventKeyboard::KeyCode::KEY_SPACE;

	// Constants
	const float PLAYER_MAX_VELOCITY = 2500.0f;
	const float PLAYER_JUMP_VELOCITY = 530.0f;
	const float PLAYER_GRAVITY = -450.0f;
	const float DAMPING = 0.87f;
}