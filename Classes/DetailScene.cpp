#include "DetailScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Global.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* DetailScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = DetailScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool DetailScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

	auto director = Director::getInstance();
    
    auto rootNode = CSLoader::createNode("DeitalPageNew.csb");

	auto border = rootNode->getChildByName("Border");



	//OperationList部分参数设定
	auto opeList = (ui::ListView*)rootNode->getChildByName("OpeList");
	//定义第1排的偏移量
	float widthOffset1 = opeList->getItem(1)->getPosition().x - opeList->getItem(0)->getPosition().x;



	log("The width offset in the 1st row is %f", widthOffset1);

	//RelativeFilms部分的参数设定
	auto relativeFilms = (ui::ListView *)rootNode->getChildByName("RelativeFilms");
	log("relativeFilms's position is %f,%f", relativeFilms->getPosition().x, relativeFilms->getPosition().y);
	log("relativeFilms's tag is %d", relativeFilms->getTag());

	auto list1 = relativeFilms->getItem(0);
	//定义第2排的偏移量
	float widthOffset2 = relativeFilms->getItem(1)->getPosition().x - relativeFilms->getItem(0)->getPosition().x;
	log("The width offset in the 2nd row is %f", widthOffset2);
	//定义拉伸参数
	float border_width = border->getContentSize().width;
	float border_height = border->getContentSize().height;
	float list_width = relativeFilms->getItem(1)->getContentSize().width;
	float list_height = relativeFilms->getItem(1)->getContentSize().height;
	//float btnAll_width = btnAll->getContentSize().width;
	//float btnAll_height = btnAll->getContentSize().height;


	float scaleX1 = list_width / border_width*1.04;
	float scaleY1 = list_height / border_height*1.04;

	//float scaleX2 = btnAll_width / border_width;
	//float scaleY2 = btnAll_height / border_height;

	log("Border's size is %f * %f", border_width, border_height);
	log("List's size is %f * %f", list_width, list_height);


	//对第一行的button进行初始化
	if (rowIndex == 1){
		auto uhd = (ui::Button*)opeList->getItem(columnIndex);
		uhd->setHighlighted(true);
	}

	//定义左右箭头
	auto arrowLeft = (ui::Button*) rootNode->getChildByName("ArrowLeft");

	auto arrowRight = (ui::Button*) rootNode->getChildByName("ArrowRight");

	arrowLeft->setVisible(false);


	//定义右上方的按钮列表
	auto iconList = (ui::ListView*) rootNode->getChildByName("IconList");

	//初始化border
	border->setVisible(false);


	auto listenerForKeyBorad = EventListenerKeyboard::create();

	listenerForKeyBorad->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event){
		log("In detailPage, you pressed the button of keyCode %d", keyCode);
		//定义Border动画
		//帮助参数
		float list5X = relativeFilms->getPosition().x + relativeFilms->getItems().size()*widthOffset2 - widthOffset2;
		float list5Y = relativeFilms->getPosition().y;

		//左动画
		auto moveLeft1 = MoveBy::create(duration,Point(-widthOffset1,0));
		auto moveLeft2 = MoveBy::create(duration, Point(-widthOffset2, 0));
		//右动画
		auto moveRight1 = MoveBy::create(duration, Point(widthOffset1, 0));
		auto moveRight2 = MoveBy::create(duration, Point(widthOffset2, 0));
		//上动画
		auto up2to1 = MoveTo::create(duration, Point(opeList->getItem(0)->getPosition().x, opeList->getItem(0)->getPosition().y));
		auto scale2to1 = ScaleTo::create(duration, 1.0f, 1.0f);
		auto moveup = Spawn::create(up2to1, scale2to1, NULL);
		//下动画
		auto down1to2 = MoveTo::create(duration, Point(relativeFilms->getPosition().x, relativeFilms->getPosition().y));
		auto scale1to2 = ScaleTo::create(duration, scaleX1, scaleY1);
		auto movedown = Spawn::create(down1to2, scale1to2,NULL);

		//向最右动画
		//auto moveRightest = MoveTo::create(duration,Point(btnAll->getPosition().x,btnAll->getPosition().y));
		//auto scaletoRightest = ScaleTo::create(duration,scaleX2,scaleY2);
		//auto moveInRightest = Spawn::create(moveRightest, scaletoRightest, NULL);
		//离开最右动画
		auto moveORightest = MoveTo::create(duration, Point(list5X,list5Y));
		auto moveOutRightest = Spawn::create(moveORightest,scale1to2,NULL);


		//定义淡入淡出动画
		auto fadeIn = FadeIn::create(duration);
		auto fadeOut = FadeOut::create(duration);

		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if (arrowRight->isHighlighted()){
				border->runAction(fadeIn);
				arrowRight->setHighlighted(false);
				break;
			}
			if (rowIndex == 1&&columnIndex>0){
				//border->runAction(moveLeft1);
				auto btn = (ui::Button*) opeList->getItem(columnIndex);
				btn->setHighlighted(false);
				columnIndex--;
				btn = (ui::Button*) opeList->getItem(columnIndex);
				btn->setHighlighted(true);
				break;
			}
			if (rowIndex == 0 && columnIndex>0){
				//border->runAction(moveLeft1);
				auto btn = (ui::Button*) iconList->getItem(columnIndex);
				btn->setHighlighted(false);
				columnIndex--;
				btn = (ui::Button*) iconList->getItem(columnIndex);
				btn->setHighlighted(true);
				break;
			}
			else if (rowIndex == 2){
				if (columnFilm > 0 && columnFilm <= relativeFilms->getItems().size()){
					border->runAction(moveLeft2);
					columnFilm--;
				}
				else if (columnFilm == 0){
					border->runAction(fadeOut);
					arrowLeft->setVisible(true);
					arrowLeft->setHighlighted(true);
				}
				break;
			}
			else{
				//do nothing
				break;
			}
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (arrowLeft->isHighlighted()){
				arrowLeft->setHighlighted(false);
				arrowRight->setVisible(true);
				border->runAction(fadeIn);
				break;
			}
			if (rowIndex == 0 && columnIndex<iconList->getItems().size()-1){
				//border->runAction(moveRight1);
				auto btn = (ui::Button*) iconList->getItem(columnIndex);
				btn->setHighlighted(false);
				columnIndex++;
				btn = (ui::Button*) iconList->getItem(columnIndex);
				btn->setHighlighted(true);
				break;
			}else if (rowIndex == 1 && columnIndex<opeList->getItems().size() - 1){
				//border->runAction(moveRight1);
				auto btn = (ui::Button*) opeList->getItem(columnIndex);
				btn->setHighlighted(false);
				columnIndex++;
				btn = (ui::Button*) opeList->getItem(columnIndex);
				btn->setHighlighted(true);
				break;
			}
			else if (rowIndex == 2){
				if (columnFilm < relativeFilms->getItems().size() - 1)
				{
					border->runAction(moveRight2);
					columnFilm++;
				}
				else if (columnFilm == relativeFilms->getItems().size() - 1){
				//	border->runAction(moveInRightest);
					border->runAction(fadeOut);
					arrowRight->setHighlighted(true);
				}
			} 
			else{
				//do nothing
			}
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (rowIndex ==2){

				border->runAction(fadeOut);
				border->setVisible(true);
				auto btn1 = (ui::Button*)opeList->getItem(columnIndex);
				btn1->setHighlighted(true);
				rowIndex--;
				break;
			}
			else if (rowIndex == 1){
				//向IconList跳转
				auto btn1 = (ui::Button*)opeList->getItem(columnIndex);
				btn1->setHighlighted(false);
				columnIndex = (columnIndex == opeList->getItems().size() - 1) ? (columnIndex - 1) : (columnIndex);
				auto btn0 = (ui::Button*)iconList->getItem(columnIndex);
				btn0->setHighlighted(true);
				rowIndex--;
				break;
			}
			else{
				//do nothing
				break;
			}
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (rowIndex == 1){
				auto btn1 = (ui::Button*)opeList->getItem(columnIndex);
				btn1->setHighlighted(false);
				border->setVisible(true);
				border->runAction(fadeIn);
				rowIndex++;
				break;
			} else if(rowIndex==0){
				//向opeList跳转
				auto btn0 = (ui::Button*)iconList->getItem(columnIndex);
				btn0->setHighlighted(false);
				auto btn1 = (ui::Button*)opeList->getItem(columnIndex);
				btn1->setHighlighted(true);
				rowIndex++;
				break;


			}
			else{
				//do nothing
				break;
			}
		case EventKeyboard::KeyCode::KEY_KP_ENTER:
			//通过rowIndex与columnIndex来判断不同的动作
			if (rowIndex == 0){
				if (columnIndex == 1){
					auto searchScene = SearchScene::createScene();
					auto transition = TransitionFadeTR::create(duration, searchScene);
					director->pushScene(transition);
					break;
				}
			}
			if (rowIndex == 1){
				if (columnIndex == 1){
					
					log("The film with the quality of UHD will be played soon...");
				}
				else if (columnIndex == 2){
					log("The film with the quality of HD will be played soon...");
				}
				else if (columnIndex == 3){
					log("The film with the quality of SD will be played soon...");
				}
				else{
					log("More operations will be done for the film");
				}
				
				auto playerScene = PlayerScene::createScene();
				auto transition = TransitionProgressRadialCW::create(duration, playerScene);
				director->pushScene(transition);
			}
			else if (rowIndex == 2){
				if (columnIndex == 6){
					log("We will go to the allitem scene");
					auto allItemScene = AllItemScene::createScene(2);
					auto transition = TransitionProgressOutIn::create(duration, allItemScene);
					director->replaceScene(transition);
				}
			}
			break;
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			auto director = Director::getInstance();
			director->popScene();
			log("Turn back ...");
			break;

		}
		log("Now it is in the %dth row and the %dth column", rowIndex, columnIndex);
			
		
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listenerForKeyBorad,this);

    addChild(rootNode);

    return true;
}
