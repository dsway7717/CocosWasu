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
	//添加shadow到缓存
	TextureCache::getInstance()->addImage("shadow_mainScene.png");


	TextureCache::getInstance()->addImage("free_homepage.png");
    
	//添加调度器
	schedule(schedule_selector(MainScene::update), 60.0f, kRepeatForever, 0);

    rootNode = CSLoader::createNode("MainScene.csb");
	auto showCategory = (ui::Button*)rootNode->getChildByName("ShowCategory");
	auto listView = (ui::ListView*)rootNode->getChildByName("ListView");
	auto border= rootNode->getChildByName("Border");
	log("In mainscene, border's tag is %d",border->getTag());




	auto timeBox = (ui::Text*)rootNode->getChildByName("TimeBox");
	log("timeBox's tag is %d", timeBox->getTag());


	//获取 左右的按钮
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

	/**************************************时钟程序****************************************/
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
	

	/************************************时钟程序结束***************************************/



    //设置listview的滚动条不可见
		//listView->setScrollBarAutoHideEnabled(true);
		listView->setScrollBarWidth(0);

	log("listView's tag is %d", listView->getTag());
	log("listView's item number is %d", listView->getItems().size());
	//定义放大系数、缩小系数、持续时间、当前屏幕索引、当前
	//从大变小
	float numZO =0.926f;
	//维持正常
	float numZI = 1.00f;

	currentIndex = 0;
	currentOffset = 0;
	//定义每页可以放的数量
	int perPage = 5;
	//获取items数量
	int totalNo = listView->getItems().size();
	//获取全部的页数
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
	//将全部的子节点的锚点定义为底部居中
	for (int i = 0; i < listView->getItems().size(); i++){
		auto item = listView->getItem(i);
		item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
		auto temp = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("shadow_mainScene.png"));
		temp->setScaleX(item->getContentSize().width);
		temp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
		temp->setPosition(Vec2(item->getContentSize().width / 2, 0));
		log("temp's width is %f ", temp->getContentSize().width);
		item->addChild(temp);
		//将除了第一个之外的全部缩小
		if (i > 0){
			item->setScale(numZO);
		}
		if (i == 1){
			//给第二个加上免费的标签
			auto freeImg = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("free_homepage.png"));
			freeImg->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
			freeImg->setPosition(Vec2(0, item ->getContentSize().height));
			item->addChild(freeImg);
		}
	}




	listView->getItem(0)->setScale(numZI);


    addChild(rootNode);

	//初始化，选中第一个
	listView->getItem(currentIndex)->setScale(numZI);



	//获取并初始化IconList
	auto iconList = (ui::ListView*)rootNode->getChildByName("IconList");

	log("iconList has %d children!", iconList->getItems().size());

	//添加键盘监听，向左向右的时候执行不同的操作
	auto listenerForKeyBoard = EventListenerKeyboard::create();
	listenerForKeyBoard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){

		//定义放大缩小动画
		//针对电影
		auto zoomIn = ScaleTo::create(duration, numZI);
		auto zoomOut = ScaleTo::create(duration, numZO);

		//针对按钮
		auto zoomIn1 = ScaleTo::create(duration, numZI*numZI);
		auto zoomOut1 = ScaleTo::create(duration, numZO*numZO);


		auto moveLeft = MoveBy::create(duration, Point(-offsetWidth, 0));

		auto moveRight = MoveBy::create(duration, Point(offsetWidth, 0));

		//border动画
		auto moveToMLeft = MoveBy::create(duration, Point(-offsetWidth*(perPage-1),0));

		auto moveToMRight = MoveBy::create(duration, Point(offsetWidth*(perPage - 1), 0));
		
		auto fadeOut = FadeOut::create(duration);

		auto fadeIn = FadeIn::create(duration);


		//判断监听

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
			//先判断是否处于右边按钮选中状态
			if (arrowRight->isHighlighted()){
				log("How old are you!!!!!!!!!");
				log("What the fuck you choose,the currentIndex and the currentOffset：：%d,%d", currentIndex, currentOffset);
				arrowRight->setHighlighted(false);
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				border->runAction(fadeIn);
			}
			//向右的时候，先判断是否到尽头，如果没有再判断是否在最右边，处于屏幕最右边则滚动listView，处于其他的地方则滚动item
		   else if ((currentIndex + currentOffset)>0&&rowIndex==1){
				if (currentIndex == 0&&currentOffset>0){
					//当前处于屏幕视角的最左边，翻页
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
					//当前的缩小，下一个放大
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
				//已经处于最左边了
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
			//先判断左边的按钮是否高亮
			if (arrowLeft->isHighlighted()){
				arrowLeft->setHighlighted(false);
				listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomIn);
				border->runAction(fadeIn);

			}
			//向右的时候，先判断是否到尽头，如果没有再判断是否在最右边，处于屏幕最右边则滚动listView，处于其他的地方则滚动item
			else if ((currentIndex + currentOffset*perPage)<9&&rowIndex==1){
				if (currentIndex == 4 && currentOffset<totalPage-1){
					//当前处于屏幕视角的最右边，翻下一个
					//listView->runAction(moveLeft);
					//listView->scrollToPercentHorizontal(percentX*(currentOffset + 1), duration, true);
					//listView->scrollToPercentHorizontal(100,duration,true);
					listView->getItem(currentIndex + currentOffset*perPage)->runAction(zoomOut);

					//currentOffset ++ ;
					//listView->getItem(currentIndex + currentOffset)->runAction(zoomIn);
					//border->runAction(moveToMLeft);

					//跳转到按钮
					arrowRight->setHighlighted(true);
					border->runAction(fadeOut);
					log("Here is the rightest in the screen.So change the page.Now it is the %dth item",(currentIndex+currentOffset));
				}
				else{
					//当前的缩小，下一个放大
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
				//已经处于最右边了
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
