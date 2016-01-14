#include "PlayerScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "Global.h"
USING_NS_CC;

using namespace cocostudio::timeline;

Scene* PlayerScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = PlayerScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool PlayerScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("PlayerScene.csb");

	auto listenerForKeyBoard = EventListenerKeyboard::create();
	listenerForKeyBoard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			auto director = Director::getInstance();
			director->popScene();
			log("Turn back ...");
			break;
		}
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerForKeyBoard, this);
	addChild(rootNode);

	return true;
}
