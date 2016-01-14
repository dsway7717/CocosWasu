#ifndef __Main_SCENE_H__
#define __Main_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	void update(float dt) override;

	int currentIndex=0;
	int currentOffset=0;

	//2016-01-12Ìí¼Ó
	int indexRow01 = 0;


	
	int rowIndex = 1;


	Node* rootNode;

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // __HELLOWORLD_SCENE_H__
