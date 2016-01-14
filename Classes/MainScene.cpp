#include "MainScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <ctime>

#include "Global.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MainScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = MainScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//���shadow������
	TextureCache::getInstance()->addImage("shadow_mainScene.png");


	TextureCache::getInstance()->addImage("free_homepage.png");
    
	//��ӵ�����
	schedule(schedule_selector(MainScene::update), 60.0f, kRepeatForever, 0);

    rootNode = CSLoader::createNode("MainScene.csb");
	auto showCategory = (ui::Button*)rootNode->getChildByName("ShowCategory");
	auto listView = (ui::ListView*)rootNode->getChildByName("ListView");
	auto border= rootNode->getChildByName("Border");
	log("In mainscene, border's tag is %d",border->getTag());




	auto timeBox = (ui::Text*)rootNode->getChildByName("TimeBox");
	log("timeBox's tag is %d", timeBox->getTag());


	//��ȡ ���ҵİ�ť
	auto arrowRight = (ui::Button*)rootNode->getChildByName("ArrowRight");

	//arrowRight->setHighlighted(true);
	arrowRight->setHighlighted(false);

	
	auto arrowLeft = (ui::Button*)rootNode->getChildByName("ArrowLeft");
	arrowLeft->setVisible(false);
	//auto border = rootNode->getChildByName("Border");
	//log("border's tag is %d",border->getTag());


 //   #if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	//log("win 32");
	//time(&timep);
 //   #else   
	//log("others platform");
	//struct  cc_timeval now;
	//CCTime::gettimeofdayCocos2d(&now, NULL);
	//timep = now.tv_sec;
 //   #endif   

	/**************************************ʱ�ӳ���****************************************/
	    struct   tm  * tm;
	    time_t  timep;
		time(&timep);

		tm = localtime(&timep);
		int  year = tm->tm_year + 1900;
		int  month = tm->tm_mon + 1;
		int  day = tm->tm_mday;
		int  hour = tm->tm_hour;
		int  min = tm->tm_min;
		int  second = tm->tm_sec;

		log("%d-%d-%d %d:%d %d", year, month, day, hour, min, second);
		auto timestr = String::createWithFormat("%d:%d", hour, min);
		if (min < 10){
			 timestr = String::createWithFormat("%d:0%d", hour, min);
		}


		timeBox->setString(timestr->getCString());
	

	/************************************ʱ�ӳ������***************************************/



    //����listview�Ĺ��������ɼ�
		//listView->setScrollBarAutoHideEnabled(true);
		listView->setScrollBarWidth(0);

	log("listView's tag is %d", listView->getTag());
	log("listView's item number is %d", listView->getItems().size());
	//����Ŵ�ϵ������Сϵ��������ʱ�䡢��ǰ��Ļ��������ǰ
	//�Ӵ��С
	float numZO =0.926f;
	//ά������
	float numZI = 1.00f;

	currentIndex = 0;
	currentOffset = 0;
	//����ÿҳ���Էŵ�����
	int perPage = 5;
	//��ȡitems����
	int totalNo = listView->getItems().size();
	//��ȡȫ����ҳ��
	int totalPage = totalNo / perPage;
	if (totalNo%perPage != 0){
		totalPage++;
	}

	log("There are %d pages,in which there are %d items", totalPage, perPage);
	float offsetWidth = listView->getItem(1)->getPosition().x-listView->getItem(0)->getPosition().x;

	auto borderInitPos = border->getPosition();

	log("borderInitPos's size is %f,%f", borderInitPos.x, borderInitPos.y);

	float percentX = 100 * (offsetWidth) / (offsetWidth*(listView->getItems().size() - perPage));

	log("percentX is %f", percentX);
	log("the offsetWidth is %f", offsetWidth);
	//��ȫ�����ӽڵ��ê�㶨��Ϊ�ײ�����
	for (int i = 0; i < listView->getItems().size(); i++){
		auto item = listView->getItem(i);
		item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		auto temp = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("shadow_mainScene.png"));
		temp->setScaleX(item->getContentSize().width);
		temp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		temp->setPosition(Vec2(item->getContentSize().width / 2, 0));
		log("temp's width is %f ", temp->getContentSize().width);
		item->addChild(temp);
		//�����˵�һ��֮���ȫ����С
		if (i > 0){
			item->setScale(numZO);
		}
		if (i == 1){
			//���ڶ���������ѵı�ǩ
			auto freeImg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("free_homepage.png"));
			freeImg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			freeImg->setPosition(Vec2(0, item ->getContentSize().height));
			item->addChild(freeImg);
		}
	}




	listView->getItem(0)->setScale(numZI);


    addChild(rootNode);

	//��ʼ����ѡ�е�һ��
	listView->getItem(currentIndex)->setScale(numZI);



	//��ȡ����ʼ��IconList
	auto iconList = (ui::ListView*)rootNode->getChildByName("IconList");

	log("iconList has %d children!", iconList->getItems().size());

	//��Ӽ��̼������������ҵ�ʱ��ִ�в�ͬ�Ĳ���
	auto listenerForKeyBoard = EventListenerKeyboard::create();
	listenerForKeyBoard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){

		//����Ŵ���С����
		//��Ե�Ӱ
		auto zoomIn = ScaleTo::create(duration, numZI);
		auto zoomOut = ScaleTo::create(duration, numZO);

		//��԰�ť
		auto zoomIn1 = ScaleTo::create(duration, numZI*numZI);
		auto zoomOut1 = ScaleTo::create(duration, numZO*numZO);


		auto moveLeft = MoveBy::create(duration, Point(-offsetWidth, 0));

		auto moveRight = MoveBy::create(duration, Point(offsetWidth, 0));

		//border����
		auto moveToMLeft = MoveBy::create(duration, Point(-offsetWidth*(perPage-1),0));

		auto moveToMRight = MoveBy::create(duration, Point(offsetWidth*(perPage - 1), 0));
		
		auto fadeOut = FadeOut::create(duration);

		auto fadeIn = FadeIn::create(duration);


		//�жϼ���

		log("You pressed the button with keycode %d", keyCode);
		auto currentChild = listView->getItem(currentIndex + currentOffset);
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (rowIndex == 0&&indexRow01>0){
				auto button = (ui::Button*)iconList->getItem(indexRow01);
				button->setHighlighted(false);
				indexRow01--;
				button = (ui::Button*)iconList->getItem(indexRow01);
				button->setHighlighted(true);
				break;
			}
			//���ж��Ƿ����ұ߰�ťѡ��״̬
			if (arrowRight->isHighlighted()){
				log("How old are you!!!!!!!!!");
				log("What the fuck you choose,the currentIndex and the currentOffset����%d,%d", currentIndex, currentOffset);
				arrowRight->setHighlighted(false);
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				border->runAction(fadeIn);
			}
			//���ҵ�ʱ�����ж��Ƿ񵽾�ͷ�����û�����ж��Ƿ������ұߣ�������Ļ���ұ������listView�����������ĵط������item
		   else if ((currentIndex + currentOffset)>0&&rowIndex==1){
				if (currentIndex == 0&&currentOffset>0){
					//��ǰ������Ļ�ӽǵ�����ߣ���ҳ
					//listView->runAction(moveLeft);
					//listView->scrollToPercentHorizontal(percentX * (currentOffset - 1), duration, true);
					//listView->scrollToPercentHorizontal(-100, duration, true);

					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);

					//listView->getItem(currentIndex + currentOffset)->runAction(zoomIn);
					arrowLeft->setHighlighted(true);
					border->runAction(fadeOut);
					log("Here is the leftest in the screen.So change the page.Now it is the %dth item", (currentIndex + currentOffset));
				}
				else{
					//��ǰ����С����һ���Ŵ�
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);
					//currentIndex ++;

					currentIndex--;
					log("Here is not the leftest in the screen.So change the page.Now it is the %dth item", (currentIndex + currentOffset));
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
					//border->runAction(moveLeft);
					border->runAction(moveLeft);
				}

			}
			else{
				//�Ѿ������������
				//do nothing
				log("You have reached the leftest item!Do Nothing ........");
			}
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (rowIndex == 0 && indexRow01< iconList->getItems().size()-1){
				auto button = (ui::Button*)iconList->getItem(indexRow01);
				button->setHighlighted(false);
				indexRow01++;
				button = (ui::Button*)iconList->getItem(indexRow01);
				button->setHighlighted(true);
				break;
			}
			//���ж���ߵİ�ť�Ƿ����
			if (arrowLeft->isHighlighted()){
				arrowLeft->setHighlighted(false);
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				border->runAction(fadeIn);

			}
			//���ҵ�ʱ�����ж��Ƿ񵽾�ͷ�����û�����ж��Ƿ������ұߣ�������Ļ���ұ������listView�����������ĵط������item
			else if ((currentIndex + currentOffset*perPage)<9&&rowIndex==1){
				if (currentIndex == 4 && currentOffset<totalPage-1){
					//��ǰ������Ļ�ӽǵ����ұߣ�����һ��
					//listView->runAction(moveLeft);
					//listView->scrollToPercentHorizontal(percentX*(currentOffset + 1), duration, true);
					//listView->scrollToPercentHorizontal(100,duration,true);
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);

					//currentOffset ++ ;
					//listView->getItem(currentIndex + currentOffset)->runAction(zoomIn);
					//border->runAction(moveToMLeft);

					//��ת����ť
					arrowRight->setHighlighted(true);
					border->runAction(fadeOut);
					log("Here is the rightest in the screen.So change the page.Now it is the %dth item",(currentIndex+currentOffset));
				}
				else{
					//��ǰ����С����һ���Ŵ�
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);
					//currentIndex ++;
					
					log("here is the duan point $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					currentIndex++;
					log("Here is not the rightest in the screen.So change the page.Now it is the %dth item", (currentIndex + currentOffset));
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
					//border->runAction(fadeOut);
					border->runAction(moveRight);
				}

			}
			else{
				//�Ѿ��������ұ���
				//do nothing
				log("You have reached the rightest item!Do Nothing ........");
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (rowIndex == 0){
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				border->runAction(fadeIn);
				auto button1 = (ui::Button*)iconList->getItem(indexRow01);
				button1->setHighlighted(false);
				rowIndex++;
				log("000000000000000000000000000000000000000000000000000000000000000000");
				break;
			}
			else if (rowIndex == 1){
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);
				border->runAction(fadeOut);
				showCategory->setHighlighted(true);
				log("showCategory's tag is %d", showCategory->getTag());
				rowIndex++;
				log("11111111111111111111111111111111111111111111111111111111111111111111");
				break;
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (rowIndex == 1){
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);
				border->runAction(fadeOut);
				auto button1 = (ui::Button*)iconList->getItem(indexRow01);
				button1->setHighlighted(true);
				rowIndex--;
				break;
			}
			if (rowIndex == 2){
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				//showCategory->runAction(zoomOut1);
				border->runAction(fadeIn);
				showCategory->setHighlighted(false);
				log("showCategory's tag is %d", showCategory->getTag());
				rowIndex--;
				break;
			}
		case EventKeyboard::KeyCode::KEY_KP_ENTER:
			auto director = Director::getInstance();
			if (rowIndex == 0){
				if (indexRow01 == 1){
					auto searchScene = SearchScene::createScene();
					auto transition = TransitionFadeTR::create(duration, searchScene);
					director->pushScene(transition);
					break;
				}
			}
			if (rowIndex == 1){
				if (arrowRight->isHighlighted()){
					//do something
					log("turn back to right!!!");
					listView->scrollToPercentHorizontal(100, duration, true);
					currentIndex = 0;
					currentOffset++;
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
					log("ZoomIn Page is %d", currentIndex + currentOffset*perPage);
					border->setPosition(borderInitPos);
					border->runAction(fadeIn);
					arrowRight->setHighlighted(false);
					arrowRight->setVisible(false);
					arrowLeft->setVisible(true);
					break;
	
				}
				else if (arrowLeft->isHighlighted()){
					//do something 
					log("turn back to left!!!");
					listView->scrollToPercentHorizontal(0, duration, true);
					currentIndex = 4;
					currentOffset--;
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
					log("ZoomIn Page is %d", currentIndex + currentOffset*perPage);
					border->setPosition(Vec2(borderInitPos.x+4*offsetWidth,borderInitPos.y));
					//log("borderInitPos is %d-%d", borderInitPos.x, borderInitPos.y);
					border->runAction(fadeIn);
					arrowLeft->setHighlighted(false);
					arrowLeft->setVisible(false);
					arrowRight->setVisible(true);
					break;
				} else{
					log("You choose the %dth one", (currentIndex + currentOffset + 1));
					auto detailScene = DetailScene::createScene();
					auto transition = TransitionProgressHorizontal::create(duration, detailScene);
					director->pushScene(transition);
					break;
				}
			}
			else if (rowIndex==2){
				log("You choose the category page!");
				auto catogoryScene = CatogoryScene::createScene();
				auto transition = TransitionProgressInOut::create(duration, catogoryScene);
				director->pushScene(transition);
				break;
			}

		}
		log("Now it is the %d column in the %d row",currentIndex+currentOffset*perPage+1,rowIndex);
		log("CurrentIndex is %d--CurrentOffset is %d", currentIndex, currentOffset);

	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerForKeyBoard, this);

    return true;
}

void MainScene::update(float dt)
{
	auto timeBox = (ui::Text*)rootNode->getChildByName("TimeBox");
	struct   tm  * tm;
	time_t  timep;
	time(&timep);

	tm = localtime(&timep);
	int  year = tm->tm_year + 1900;
	int  month = tm->tm_mon + 1;
	int  day = tm->tm_mday;
	int  hour = tm->tm_hour;
	int  min = tm->tm_min;
	int  second = tm->tm_sec;

	log("%d-%d-%d %d:%d %d", year, month, day, hour, min, second);
	auto timestr = String::createWithFormat("%d:%d", hour, min);
	if (min < 10){
		timestr = String::createWithFormat("%d:0%d", hour, min);
	}

	timeBox->setString(timestr->getCString());

}
