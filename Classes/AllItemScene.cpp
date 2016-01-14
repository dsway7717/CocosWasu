#include "AllItemScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Global.h"

USING_NS_CC;

using namespace cocostudio::timeline;

int titleIndex = 0;

Scene* AllItemScene::createScene(int titleNo)
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
	titleIndex = titleNo;
    // 'layer' is an autorelease object
	auto layer = AllItemScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool AllItemScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	//添加shadow图片到缓存
	TextureCache::getInstance()->addImage("shadow_mainScene.png");
    
    rootNode = CSLoader::createNode("AllItems.csb");

	auto titleNode = (ui::Text*)rootNode->getChildByName("Title");

	border = rootNode->getChildByName("Border");
	//border->setAnchorPoint(Vec2(0, 1));
	//border->setPosition(Vec2(99, 990));

	if (initChinese()) {
		log("initChinese Title success");
	}

	//const::std::string rowArr[] = {"热门", "电视剧", "电影", "动画", "综艺" };
	//const::std::string rowArr[] = { "Hot", "TV Show", "Films", "Animations", "Variety Show" };
	//titleNode->setString(rowArr[titleIndex]);
	titleNode->setString(ChineseTitle);


	//设置描点

	auto allList = (ui::ListView*)rootNode->getChildByName("AllList");

	//定义放大比例
	float scaleNoX = 167.0 / 183;
	float scaleNoY = 250.0 / 271;

	for (int i = 0; i < allList->getItems().size(); i++){
		auto rowList = (ui::ListView*)allList->getItem(i);
		rowList->setScrollBarEnabled(false);
		for (int j = 0; j < rowList->getItems().size(); j++){
			auto item = rowList->getItem(j);
			item->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

			auto temp = Sprite::createWithTexture(TextureCache::getInstance()->getTextureForKey("shadow_cato.png"));
			temp->setScaleX(item->getContentSize().width);
			temp->setScaleY(0.5);
			temp->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
			temp->setPosition(Vec2(item->getContentSize().width / 2, 0));
			log("temp's width is ", temp->getContentSize().width);
			item->addChild(temp);

			if (!(i == 0 && j == 0)) {
				item->setScale(scaleNoX, scaleNoY);
			}


		}
	}

	
	//初始化第一行第一个被选中的图片
	auto rowList1 = (ui::ListView*)allList->getItem(rowIndex+rowOffset);
	auto rowList2 = (ui::ListView*)allList->getItem(rowIndex + rowOffset+1);
	auto item11 = rowList1->getItem(columnIndex);
	//item11->setScale(scaleNo);
	//定义左右偏移
	float offsetHorizontal = rowList1->getItem(1)->getPosition().x - rowList1->getItem(0)->getPosition().x;
	//定义上下偏移
	float offsetVertical = rowList1->getPosition().y - rowList2->getPosition().y;



	//定义行数及列数
	int totalRowNo = allList->getItems().size();
	int totalColumnNo = rowList1->getItems().size();

	//定义上下滚动的百分比
	//float increasePercent = 100/(totalRowNo-3);
	float pixelToIncrease = 271 + 103; // row hight + space between rows
	float pixelOutSideView = (271 + 103) * 8 - 103 - 900; // 8 is rowNo, 900 is totalList's hight
	float increasePercent = 100 * pixelToIncrease/pixelOutSideView;
	log("increasePercent is %f", increasePercent);

	//设置最外层ListView 的 Scroll bar
	allList->setScrollBarWidth(4);
	allList->setScrollBarColor(Color3B(36, 207, 209));
	allList->setScrollBarOpacity(255);
	allList->setScrollBarAutoHideEnabled(false);
	allList->setScrollBarPositionFromCornerForVertical(Vec2(16, 33));
	//log("corner.x: %f, corner.y: %f", totalView->getScrollBarPositionFromCornerForVertical().x, totalView->getScrollBarPositionFromCornerForVertical().y);

	//添加右上角的icons
	if (initIcons()) {
		log("initIcons success.");
	}

	//添加监听
	auto listnerForKeyboard = EventListenerKeyboard::create();
	listnerForKeyboard->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
		//定义动画
		//左动画
		auto moveLeft = MoveBy::create(duration, Point(-offsetHorizontal, 0));
		//右动画
		auto moveRight = MoveBy::create(duration, Point(offsetHorizontal, 0));
		//上动画
		auto moveUp = MoveBy::create(duration, Point(0, offsetVertical));
		//下动画
		auto moveDown = MoveBy::create(duration, Point(0, -offsetVertical));
		//放大动画
		auto zoomIn = ScaleTo::create(duration, 1);
		//缩小动画
		auto zoomOut = ScaleTo::create(duration, scaleNoX, scaleNoY);
		//导演类
		auto director = Director::getInstance();

		switch (keyCode)
		{
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			log("Do the left operation!");
			if (rowIndex != -1 && columnIndex > 0){
				auto CurrentItem = getItemByRoot(rowIndex+rowOffset,columnIndex,allList);
				auto NextItem = getItemByRoot(rowIndex + rowOffset, columnIndex - 1, allList);
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
			if (rowIndex != -1 && columnIndex <totalColumnNo-1){
				auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				auto NextItem = getItemByRoot(rowIndex + rowOffset, columnIndex + 1, allList);
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
			
		     if (rowIndex==0&& rowOffset >= 1){
				//滚动
				log("Now rowIndex is %d,Scrolling", rowIndex + rowOffset);
				percent -= increasePercent;
				allList->scrollToPercentVertical(percent, duration, false);
				auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				auto NextItem = getItemByRoot(rowIndex + rowOffset-1, columnIndex, allList);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				rowOffset--;
			 }
			 else if(rowIndex>0){
				 auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				 auto NextItem = getItemByRoot(rowIndex + rowOffset-1, columnIndex, allList);
				 CurrentItem->runAction(zoomOut);
				 NextItem->runAction(zoomIn);
				 border->runAction(moveUp);
				 rowIndex--;
			 }
			 else if(rowIndex != -1){
				 //最上层
				 log("You reached the upstair");
				 auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				 CurrentItem->runAction(zoomOut);
				 MoveBorderUp();
				 rowIndex = -1;
			 }
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			log("Do the down operation!");
			
			if (rowIndex == 1 && rowIndex + rowOffset<totalRowNo - 1){
				//滚动
				log("Now rowIndex is %d,Scrolling", rowIndex + rowOffset);
				percent += increasePercent;
				allList->scrollToPercentVertical(percent, duration, true);
				auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				auto NextItem = getItemByRoot(rowIndex + rowOffset+1, columnIndex, allList);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				rowOffset++;
			}
			else if (rowIndex != -1 && rowIndex + rowOffset<totalRowNo-1){
				auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				auto NextItem = getItemByRoot(rowIndex + rowOffset + 1, columnIndex, allList);
				CurrentItem->runAction(zoomOut);
				NextItem->runAction(zoomIn);
				border->runAction(moveDown);
				rowIndex++;
			}
			else if (rowIndex == -1) {
				rowIndex = 0;
				auto CurrentItem = getItemByRoot(rowIndex + rowOffset, columnIndex, allList);
				CurrentItem->runAction(zoomIn);
				MoveBorderDown();

			}
			else{
				//最底层
				log("You reached the bottom");
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
			if (rowIndex + rowOffset < totalRowNo){
				auto DetailScene = DetailScene::createScene();
				auto transition = TransitionFlipAngular::create(duration, DetailScene);
				director->pushScene(transition);
			}
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			director->popScene();
			log("Turn back ...");
			break;
		}
		log("Now you choose the %dth row and the %dth column", rowIndex + rowOffset + 1, columnIndex + 1);
		log("RowIndex and RowOffset are <%d,%d>", rowIndex, rowOffset);
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listnerForKeyboard, this);

    addChild(rootNode);

    return true;
}


Node* AllItemScene::getItemByRoot(int rowNo, int columnNo, cocos2d::ui::ListView* allList){
	auto listRow = (ui::ListView*)allList->getItem(rowNo);
	auto CurrentItem = listRow->getItem(columnNo);

	return CurrentItem;
}


bool AllItemScene::initChinese() {
	auto xml = FileUtils::getInstance()->getValueMapFromFile("ChineseTitle.xml");

	/*for (int i = 0; i < 5; i++) {

		ui::Text* label = static_cast<ui::Text*>(_mainList->getItem(i)->getChildByName("Text_1"));
		std::string index = Value(i).asString();
		auto s = xml.at(index).asString();
		label->setText(s);

	}*/
	std::string index = Value(titleIndex).asString();
	ChineseTitle = xml.at(index).asString();

	return true;
}

bool AllItemScene::initIcons() {

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
	int delta = 191;
	for (int i = 0; i < 9; i++) {
		auto pos = Vec2(99 + delta*i, 990);
		borderPos[i] = pos;
	}

	return true;
}

void AllItemScene::highlightIconRight() {
	_icons.at(_iconIndex)->setHighlighted(false);
	_iconIndex = (_iconIndex == _icons.size() - 1) ? _iconIndex : (_iconIndex + 1);
	_icons.at(_iconIndex)->setHighlighted(true);

	log("_icons.at(_iconIndex)->getTag(): %d", _icons.at(_iconIndex)->getTag());
}

void AllItemScene::highlightIconLeft() {
	_icons.at(_iconIndex)->setHighlighted(false);
	_iconIndex = (_iconIndex == 0) ? _iconIndex : (_iconIndex - 1);
	_icons.at(_iconIndex)->setHighlighted(true);
	log("_iconIndex: %d", _iconIndex);

	log("_icons.at(_iconIndex)->getTag(): %d", _icons.at(_iconIndex)->getTag());
}


void AllItemScene::MoveBorderUp() {
	_icons.at(_iconIndex)->setHighlighted(true);
	float duration = 0.3;
	auto zoomOut = ScaleTo::create(duration, 0.01);
	auto moveUp = MoveTo::create(duration, _icons.at(_iconIndex)->getPosition());
	auto fadeOut = FadeOut::create(duration);

	//borderPos = border->getPosition();
	border->stopAllActions();
	border->runAction(Spawn::create(zoomOut, moveUp, fadeOut, NULL));
}


void AllItemScene::MoveBorderDown() {
	_icons.at(_iconIndex)->setHighlighted(false);

	float duration = 0.3;
	auto zoomIn = ScaleTo::create(duration, 1);
	auto moveDown = MoveTo::create(duration, borderPos[columnIndex]);
	auto fadeIn = FadeIn::create(duration);

	border->stopAllActions();
	border->runAction(Spawn::create(zoomIn, moveDown, fadeIn, NULL));
}