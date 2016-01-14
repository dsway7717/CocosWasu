#ifndef __CATOGORY_SCENE_H__
#define __CATOGORY_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
class CatogoryScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	int rowIndex = 0;

	int columnIndex = 0;

	int rowOffset = 0;

	float percent = 0;
	
	cocos2d::Vector<cocos2d::ui::Button*> _icons;
	Node* border;


    // implement the "static create()" method manually
	CREATE_FUNC(CatogoryScene);
private:
	void highlightIconRight();
	void highlightIconLeft();
	bool initIcons();
	int _iconIndex;
	cocos2d::Node* rootNode;
	void MoveBorderUp();
	void MoveBorderDown();
	cocos2d::Vec2 borderPos[8];
};

#endif // __HELLOWORLD_SCENE_H__
