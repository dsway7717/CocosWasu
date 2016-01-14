#ifndef __ALLITEM_SCENE_H__
#define __ALLITEM_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class AllItemScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene(int titleNo);
	Node* getItemByRoot(int rowNo, int columnNo, cocos2d::ui::ListView* allList);

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	int rowIndex = 0;

	int rowOffset = 0;

	int columnIndex = 0;

	//¶¨ÒåÆ«ÒÆÁ¿
	float percent = 0;

    // implement the "static create()" method manually
	CREATE_FUNC(AllItemScene);

	cocos2d::Vector<cocos2d::ui::Button*> _icons;
	Node* border;

private:
	bool initChinese();
	std::string ChineseTitle;

	void highlightIconRight();
	void highlightIconLeft();

	bool initIcons();
	int _iconIndex;
	cocos2d::Node* rootNode;
	void MoveBorderUp();
	void MoveBorderDown();

	cocos2d::Vec2 borderPos[9];

};

#endif // __HELLOWORLD_SCENE_H__
