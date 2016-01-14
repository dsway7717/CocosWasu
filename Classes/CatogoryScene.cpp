#include "CatogoryScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Global.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* CatogoryScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	auto layer = CatogoryScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool CatogoryScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	//添加shadow到缓存
	TextureCache::getInstance()->addImage("shadow_cato.png");
    
	rootNode = CSLoader::createNode("CatogoryScene.csb");

	border = rootNode->getChildByName("Border");
	log("border's tag is %d", border->getTag());

	auto totalView = (ui::ListView*)rootNode->getChildByName("TotalView");

	auto list1 = totalView->getItem(0);

	auto list2 = totalView->getItem(1);

	auto listView = (ui::ListView*)list1->getChildByName("ListView");

	auto listView2 = (ui::ListView*)list2->getChildByName("ListView");

	log("listView's tag is %d", listView->getTag());
	log("listView2's tag is %d", listView2->getTag());

	//定义左右的偏移量
	float offsetHorizontal = listView->getItem(2)->getPosition().x - listView->getItem(1)->getPosition().x;
	log("offsetHorizontal is %f", offsetHorizontal);

	//定义上下的偏移量
	float offsetVertical = list1->getPosition().y - list2->getPosition().y;
	log("offsetVertical is %f",offsetVertical);
	//定义放缩数值
	//float scaleNo = (border->getContentSize().width-2) / listView->getItem(1)->getContentSize().width;
	float scaleNo = 0.925;
	log("scaleNo is %f",scaleNo);
	//初始化全部item的描点
	for (int i = 0; i < totalView->getItems().size(); i++){
		auto itemRow = totalView->getItem(i);
		auto ListViewInRow = (ui::ListView*)itemRow->getChildByName("ListView");
		ListViewInRow->setScrollBarEnabled(false);
		for (int j = 0; j < ListViewInRow->getItems().size();j++){
			auto item = ListViewInRow->getItem(j);
			item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			// set scale
			if (!(i == 0 && j == 0)) {
				item->setScale(scaleNo);
			}
			auto temp = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("shadow_cato.png"));
			temp->setScaleX(item->getContentSize().width);
			temp->setScaleY(0.5);
			temp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			temp->setPosition(Vec2(item->getContentSize().width / 2, 0));
			log("temp's width is ", temp->getContentSize().width);
			item->addChild(temp);
			
		}
	}
	listView->getItem(0)->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	//初始化第一个图片
	listView->getItem(0)->setScale(1.0);

	//设置最外层ListView 的 Scroll bar
	totalView->setScrollBarWidth(4);
	totalView->setScrollBarColor(Color3B(36, 207, 209));
	totalView->setScrollBarOpacity(255);
	totalView->setScrollBarAutoHideEnabled(false);
	totalView->setScrollBarPositionFromCornerForVertical(Vec2(25, 85));
	//log("corner.x: %f, corner.y: %f", totalView->getScrollBarPositionFromCornerForVertical().x, totalView->getScrollBarPositionFromCornerForVertical().y);

	//添加右上角的icons
	if (initIcons()) {
		log("initIcons success.");
	}
	


	auto listenerForKeyboard = EventListenerKeyboard::create();
	listenerForKeyboard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
		//定义动画
		//左动画
		auto moveLeft = MoveBy::create(duration,Point(-offsetHorizontal,0));
		//右动画
		auto moveRight = MoveBy::create(duration,Point(offsetHorizontal,0));
		//上动画
		auto moveUp = MoveBy::create(duration, Point(0,offsetVertical));
		//下动画
		auto moveDown = MoveBy::create(duration,Point(0,-offsetVertical));
		//放大动画
		auto zoomIn = ScaleTo::create(duration, 1);
		//缩小动画
		auto zoomOut = ScaleTo::create(duration, scaleNo);
		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			log("Do the left operation!");
			if (rowIndex != -1 && columnIndex > 0){
				auto listRow = totalView->getItem(rowIndex+rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				auto NextItem = listColumn->getItem(columnIndex - 1);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				border->runAction(moveLeft);
				columnIndex--;
			}
			if (rowIndex == -1) {
				highlightIconLeft();
			}
			
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			log("Do the right operation!");
			if (rowIndex != -1 && columnIndex < 7){
				auto listRow = totalView->getItem(rowIndex+rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				auto NextItem = listColumn->getItem(columnIndex + 1);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				border->runAction(moveRight);
				columnIndex++;
			}
			if (rowIndex == -1) {
				highlightIconRight();
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			log("Do the up operation!");
			if (rowIndex != -1 && (rowIndex + rowOffset == totalView->getItems().size() - 1 || (rowIndex == 1 && rowOffset == 0))){
				auto listRow = totalView->getItem(rowIndex+rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				listRow = totalView->getItem(rowIndex+rowOffset - 1);
				listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto NextItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				border->runAction(moveUp);
				rowIndex--;
			}
			else if (rowIndex != -1 && rowIndex + rowOffset <= totalView->getItems().size() - 2 && rowOffset >= 1){
				//滚动
				log("Now rowIndex is %d,Scrolling", rowIndex + rowOffset);
				//percent -= 40.5;
				percent -= 39.35484;
				totalView->scrollToPercentVertical(percent, duration, false);
				auto listRow = totalView->getItem(rowIndex + rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				listRow = totalView->getItem(rowIndex + rowOffset - 1);
				listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto NextItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				rowOffset--;
			}
			else if (rowIndex==0 && rowOffset==0) {
				auto listRow = totalView->getItem(rowIndex + rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomOut);
				MoveBorderUp();
				rowIndex = -1;
			}
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			log("Do the down operation!");
			if (rowIndex ==0){
				auto listRow = totalView->getItem(rowIndex);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				listRow = totalView->getItem(rowIndex + 1);
				listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto NextItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				border->runAction(moveDown);
				rowIndex++;
			}
			else if (rowIndex >= 1&&rowIndex+rowOffset<totalView->getItems().size()-1){
				//滚动
				log("Now rowIndex is %d,Scrolling", rowIndex+rowOffset);
				//percent += 40.5;
				percent += 39.35484;
				totalView->scrollToPercentVertical(percent,duration,true);
				auto listRow = totalView->getItem(rowIndex+rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				listRow = totalView->getItem(rowIndex+rowOffset + 1);
				listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto NextItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				rowOffset++;
			}
			else if (rowIndex == -1) {
				rowIndex = 0;
				auto listRow = totalView->getItem(rowIndex + rowOffset);
				auto listColumn = (ui::ListView*)listRow->getChildByName("ListView");
				auto CurrentItem = listColumn->getItem(columnIndex);
				CurrentItem->runAction(zoomIn);
				MoveBorderDown();
			}
			break;
			case EventKeyboard::KeyCode::KEY_KP_ENTER:
			log("Do the enter operation");
			if (rowIndex == -1){
				if (_iconIndex == 1){
					auto director = Director::getInstance();
					auto searchScene = SearchScene::createScene();
					auto transition = TransitionFadeTR::create(duration, searchScene);
					director->pushScene(transition);
					break;
				}
			}

			if (rowIndex + rowOffset >= 0 && rowIndex + rowOffset <= totalView->getItems().size() - 1){
				auto director = Director::getInstance();
				//跳转到全部页面，并传递参数
				if (columnIndex == 7){
					/*auto listRow = totalView->getItem(rowIndex + rowOffset);
					auto titleNode = (ui::Text*)listRow->getChildByName("Title");
					log("The catogory title is %s", titleNode->getString().c_str());
					String title = titleNode->getString();
					log("We choose the %d row, and we will go to its all-item page", rowIndex + rowOffset + 1);*/
					auto AllItemScene = AllItemScene::createScene(rowIndex+rowOffset);
					auto transition = TransitionCrossFade::create(duration,AllItemScene);
					director->pushScene(transition);
				}
				else{
					auto DetailScene = DetailScene::createScene();
					auto transition = TransitionProgressVertical::create(duration, DetailScene);
					director->pushScene(transition);
				}
				
			}
			break;
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				auto director = Director::getInstance();
				director->popScene();
				log("Turn back ...");
				break;
		}
		log("Now you choose the %dth row and the %dth column", rowIndex+rowOffset + 1, columnIndex + 1);
		log("RowIndex and RowOffset are <%d,%d>",rowIndex,rowOffset);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerForKeyboard, this);


    addChild(rootNode);

    return true;
}


bool CatogoryScene::initIcons() {

	auto ad = static_cast<ui::Button*>(rootNode->getChildByName("ad"));
	auto search = static_cast<ui::Button*>(rootNode->getChildByName("search"));
	auto collection = static_cast<ui::Button*>(rootNode->getChildByName("collection"));
	auto history = static_cast<ui::Button*>(rootNode->getChildByName("history"));

	_icons.pushBack(ad);
	_icons.pushBack(search);
	_icons.pushBack(collection);
	_icons.pushBack(history);

	_iconIndex = 0;


	// init borderPos
	int delta = 215;
	for (int i = 0; i < 8; i++) {
		auto pos = Vec2(108 + delta*i, 674);
		borderPos[i] = pos;
	}

	return true;
}

void CatogoryScene::highlightIconRight() {
	_icons.at(_iconIndex)->setHighlighted(false);
	_iconIndex = (_iconIndex == _icons.size() - 1) ? _iconIndex : (_iconIndex + 1);
	_icons.at(_iconIndex)->setHighlighted(true);

	log("_icons.at(_iconIndex)->getTag(): %d", _icons.at(_iconIndex)->getTag());
}

void CatogoryScene::highlightIconLeft() {
	_icons.at(_iconIndex)->setHighlighted(false);
	_iconIndex = (_iconIndex == 0) ? _iconIndex : (_iconIndex - 1);
	_icons.at(_iconIndex)->setHighlighted(true);
	log("_iconIndex: %d", _iconIndex);

	log("_icons.at(_iconIndex)->getTag(): %d", _icons.at(_iconIndex)->getTag());
}


void CatogoryScene::MoveBorderUp() {
	_icons.at(_iconIndex)->setHighlighted(true);
	float duration = 0.3;
	auto zoomOut = ScaleTo::create(duration, 0.01);
	auto moveUp = MoveTo::create(duration, _icons.at(_iconIndex)->getPosition());
	auto fadeOut = FadeOut::create(duration);

	//borderPos = border->getPosition();
	border->stopAllActions();
	border->runAction(Spawn::create(zoomOut, moveUp, fadeOut, NULL));
}


void CatogoryScene::MoveBorderDown() {
	_icons.at(_iconIndex)->setHighlighted(false);

	float duration = 0.3;
	auto zoomIn = ScaleTo::create(duration, 1);
	auto moveDown = MoveTo::create(duration, borderPos[columnIndex]);
	auto fadeIn = FadeIn::create(duration);

	border->stopAllActions();
	border->runAction(Spawn::create(zoomIn, moveDown, fadeIn, NULL));
}