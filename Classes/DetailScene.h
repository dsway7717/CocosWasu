#ifndef __Detail_SCENE_H__
#define __Detail_SCENE_H__

#include "cocos2d.h"

class DetailScene : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
	//����ǰ���ţ�ʹ��columnIndex
	int rowIndex = 1;
	int columnIndex = 0;

	//���ڵ����� ��ʹ��columnFilm
	int columnFilm = 0;

    // implement the "static create()" method manually
    CREATE_FUNC(DetailScene);
};

#endif // __HELLOWORLD_SCENE_H__
