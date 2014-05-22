#include "GameMenu.h"
#include "GameScene.h"
#include "ChessSprite.h"
#include "Config.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

#define TAG_ACTION_KINGBECHECKMATE  10000 //and 10001

GameScene* GameScene::pGameScene = NULL;

GameScene::GameScene()
:pChessGame(NULL), uiLastMoveColor(CHESSCOCLOR_MIN), pInfoGround(NULL), uiTrashNum(0), bGameStop(false)
{
}

Rect arrayChessImageText[CHESSID_MAX] = {
	Rect(0, 0, 0, 0),			//CHESSID_MIN = 0,

	Rect(0, 0, 50, 50),		    //CHESSID_KING_BLACK,
	Rect(350, 0, 50, 50),		//CHESSID_KING_RED,

	Rect(50, 0, 50, 50),		//CHESSID_GUARD_BLACK_LEFT,
	Rect(50, 0, 50, 50),		//CHESSID_GUARD_BLACK_RIGNT,
	Rect(400, 0, 50, 50),		//CHESSID_GUARD_RED_LEFT,
	Rect(400, 0, 50, 50),		//CHESSID_GUARD_RED_RIGHT,

	Rect(100, 0, 50, 50),		//CHESSID_BISHOP_BLACK_LEFT,
	Rect(100, 0, 50, 50),		//CHESSID_BISHOP_BLACK_RIGNT,
	Rect(450, 0, 50, 50),		//CHESSID_BISHOP_RED_LEFT,
	Rect(450, 0, 50, 50),		//CHESSID_BISHOP_RED_RIGHT,

	Rect(150, 0, 50, 50),		//CHESSID_HORSE_BLACK_LEFT,
	Rect(150, 0, 50, 50),		//CHESSID_HORSE_BLACK_RIGNT,
	Rect(500, 0, 50, 50),		//CHESSID_HORSE_RED_LEFT,
	Rect(500, 0, 50, 50),		//CHESSID_HORSE_RED_RIGHT,

	Rect(200, 0, 50, 50),		//CHESSID_ROOK_BLACK_LEFT,
	Rect(200, 0, 50, 50),		//CHESSID_ROOK_BLACK_RIGNT,
	Rect(550, 0, 50, 50),		//CHESSID_ROOK_RED_LEFT,
	Rect(550, 0, 50, 50),		//CHESSID_ROOK_RED_RIGHT,

	Rect(250, 0, 50, 50),		//CHESSID_CANNON_BLACK_LEFT,
	Rect(250, 0, 50, 50),		//CHESSID_CANNON_BLACK_RIGNT,
	Rect(600, 0, 50, 50),		//CHESSID_CANNON_RED_LEFT,
	Rect(600, 0, 50, 50),		//CHESSID_CANNON_RED_RIGHT,

	Rect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_1,
	Rect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_2,
	Rect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_3,
	Rect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_4,
	Rect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_5,
	Rect(650, 0, 50, 50),		//CHESSID_PAWN_RED_1,
	Rect(650, 0, 50, 50),		//CHESSID_PAWN_RED_2,
	Rect(650, 0, 50, 50),		//CHESSID_PAWN_RED_3,
	Rect(650, 0, 50, 50),		//CHESSID_PAWN_RED_4,
	Rect(650, 0, 50, 50)		//CHESSID_PAWN_RED_5,
};

GameScene::~GameScene()
{
    if (NULL != this->pChessGame)
    {
        delete this->pChessGame;
    }
}

bool GameScene::init(void)
{
    Size size = Director::getInstance()->getWinSize();

    Sprite *GameGround = Sprite::create(CF_F("image_backgroud"));
    GameGround->setPosition(GameGround->getContentSize().width * 0.5, size.height * 0.5);
    this->addChild(GameGround, 0);

    this->pChessGame = new ChessGame();
    CreateChesses();

    Menu* pItemMenu = Menu::create();
    auto labelClose = Label::createWithTTF(CF_S("label_exit_game"), CF_F("fonts_cn"), CF_FT("font_menu_small"));
    MenuItemLabel* pMenuClose = MenuItemLabel::create(labelClose, CC_CALLBACK_1(GameScene::menuCloseGame, this));
    pItemMenu->addChild(pMenuClose);
    pItemMenu->setPosition(size.width * CF_P("pos_game_close_w"), size.height * CF_P("pos_game_close_h"));
    this->addChild(pItemMenu);

    this->pInfoGround = Sprite::create(CF_F("image_InfoBack"));
    this->pInfoGround->setPosition(GameGround->getContentSize().width + this->pInfoGround->getContentSize().width * 0.5, size.height * 0.5);
    this->addChild(pInfoGround, 0);

    this->pTurnLabel = Label::createWithTTF(CF_S("info_start_both_turn"), CF_F("fonts_cn"), CF_FT("font_info"));
    this->pTurnLabel->setColor(Color3B::BLACK);
    this->pInfoGround->addChild(this->pTurnLabel);
    this->pTurnLabel->setPosition(this->pInfoGround->getContentSize().width * 0.5,
                                    this->pInfoGround->getContentSize().height * CF_P("pos_game_info"));

    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(CF_F("music_back").c_str());
    SimpleAudioEngine::getInstance()->playBackgroundMusic(CF_F("music_back").c_str(), true);

    return true;
}

Scene* GameScene::scene(void)
{
    Scene *scene = Scene::create();

    GameScene *layer = GameScene::create();
    pGameScene = layer;

    scene->addChild(layer);

    return scene;
}

void GameScene::CreateChesses(void)
{
    log("*********CreateChesses****************");
    std::vector<std::vector<UINT> > vecMatrix = this->pChessGame->GetMatrix();
    auto pSpriteText = Director::getInstance()->getTextureCache()->addImage(CF_F("image_Chesses"));
    float fHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;
    Size& sizeBox = Config::getBoxSize();
    Size& sizeEdge = Config::getEdgeSize();
    for (UINT uiLine = 1; uiLine <= CHESS_DATA_LINES; uiLine++)
    {
        for (UINT uiIndex = 1; uiIndex <= CHESS_DATA_COLUMN; uiIndex++)
        {
            if (0 == vecMatrix[uiLine][uiIndex])
            {
                continue;
            }
            UINT uiId = this->pChessGame->GetChessID(uiLine, uiIndex);
			ChessSprite *pChess = ChessSprite::createWithTexture(pSpriteText, arrayChessImageText[uiId]);
			Point point = Point(sizeBox.width * uiIndex - sizeBox.width * 0.5 + sizeEdge.width,
                                sizeBox.height * uiLine - sizeBox.height * 0.5 + sizeEdge.height);
            point.y = fHeight - point.y;
            pChess->setPosition(point);
            pChess->setChessId(uiId);
            this->addChild(pChess, uiLine * 9 + uiIndex, CHESS_TAG_BASE + uiId);
        }
    }

    return;
}

GameScene* GameScene::getGameScene()
{
    return pGameScene;
}

void GameScene::resetChessGame()
{
    if (NULL != this->pChessGame)
    {
        delete this->pChessGame;
    }

    this->pChessGame = new ChessGame();
}

bool GameScene::checkChessMoveIsValid(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    if (true == this->bGameStop)
    {
        return false;
    }
    if (this->uiLastMoveColor == this->pChessGame->GetChessColor(uiChessId))
    {
        log("chess move invalid, chess color: %d, last color: %d", this->pChessGame->GetChessColor(uiChessId), this->uiLastMoveColor);
        return false;
    }
    return this->pChessGame->CheckChessMoveValid(uiChessId, uiPosX, uiPosY);
}

bool GameScene::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    log("chess moving, chess id :%d, move (%d, %d)", uiChessId, uiPosX, uiPosY);

    UINT uiTargetId = this->pChessGame->GetChessID(uiPosX, uiPosY);
    if (0 != uiTargetId)
    {
        this->moveChessToTrash(uiTargetId);

        this->pChessGame->SetChessPos(uiTargetId, 0, 0);
        this->pChessGame->SetChessAlive(uiTargetId, false);
    }

    this->pChessGame->SetChessPos(uiChessId, uiPosX, uiPosY);
    this->uiLastMoveColor = this->pChessGame->GetChessColor(uiChessId);

    if (CHESSCOCLOR_BLACK == uiLastMoveColor)
    {
        this->setTurnLabel(CF_S("info_red_turn").c_str(), Color3B::RED);
    }
    else
    {
        this->setTurnLabel(CF_S("info_black_turn").c_str(), Color3B::BLACK);
    }


    if (CHESSTYPE_KING == this->pChessGame->GetChessType(uiTargetId))
    {
        this->setGameWin();
        return true;
    }
    else
    {
        //将军判断和处理
        chessCheckmate();
        return false;
    }
}

void GameScene::chessCheckmate(void)
{
    auto redKing = this->pChessGame->GetChessPos(CHESSID_KING_RED);
    auto blackKing = this->pChessGame->GetChessPos(CHESSID_KING_BLACK);
    bool redCheck = false;
    bool blackCheck = false;
    for (UINT uiIndex = CHESSID_MIN + 1; uiIndex < CHESSID_MAX; uiIndex++)
    {
        if (true == this->pChessGame->CheckChessMoveValid(uiIndex, redKing.first, redKing.second))
        {
            checkmateKing(uiIndex);
            redCheck = true;
        }
        else if (true == this->pChessGame->CheckChessMoveValid(uiIndex, blackKing.first, blackKing.second))
        {
            checkmateKing(uiIndex);
            blackCheck = true;
        }
        else
        {
            unCheckmateKing(uiIndex);
        }
    }

    if (true == redCheck)
    {
        this->kingBeCheckmate(CHESSID_KING_RED);
    }
    else
    {
        this->kingUnBeCheckmate(CHESSID_KING_RED);
    }

    if (true == blackCheck)
    {
        this->kingBeCheckmate(CHESSID_KING_BLACK);
    }
    else
    {
        this->kingUnBeCheckmate(CHESSID_KING_BLACK);
    }
}

void GameScene::kingBeCheckmate(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    auto actionKing = pChess->getActionByTag(TAG_ACTION_KINGBECHECKMATE);
    if (nullptr != actionKing)
    {
        return;
    }

    float fWidth = Config::getChessJitter().fWidth;
    float fAngle = Config::getChessJitter().fAngle;
    float fLoop = Config::getChessJitter().fLoop;
    auto actionTo = RotateTo::create(fLoop, fAngle);
    auto actionBack = RotateTo::create(fLoop, -fAngle);
    auto actionRep = RepeatForever::create(Sequence::create(actionTo, actionBack, NULL));
    actionRep->setTag(TAG_ACTION_KINGBECHECKMATE);
    pChess->runAction(actionRep);

    auto actionMoveTo1 = MoveBy::create(fLoop, Point(fWidth, fWidth));
    auto actionMoveTo2 = MoveBy::create(fLoop, Point(fWidth, -fWidth));
    auto actionMoveTo3 = MoveBy::create(fLoop, Point(-fWidth, -fWidth));
    auto actionMoveTo4 = MoveBy::create(fLoop, Point(-fWidth, fWidth));
    auto actionRep2 = RepeatForever::create(Sequence::create(actionMoveTo1, actionMoveTo2, actionMoveTo3, actionMoveTo4, NULL));
    actionRep2->setTag(TAG_ACTION_KINGBECHECKMATE + 1);
    pChess->runAction(actionRep2);

    return;
}

void GameScene::kingUnBeCheckmate(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    auto actionKing = pChess->getActionByTag(TAG_ACTION_KINGBECHECKMATE);
    if (nullptr == actionKing)
    {
        return;
    }

    pChess->stopActionByTag(TAG_ACTION_KINGBECHECKMATE);
    pChess->stopActionByTag(TAG_ACTION_KINGBECHECKMATE + 1);

    return;
}

void GameScene::checkmateKing(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    if (nullptr == pChess)
    {
        return;
    }

    pChess->runAction(ScaleTo::create(Config::getChessJitter().fScaleTo, Config::getChessJitter().fScaleExtent));

    return;
}

void GameScene::unCheckmateKing(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    if (nullptr == pChess)
    {
        return;
    }

    if (1.0f != pChess->getScale())
    {
        pChess->stopAllActions();
        pChess->runAction(ScaleTo::create(Config::getChessJitter().fScaleTo, 1.0f));
    }
}

void GameScene::moveChessToTrash(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->removeFromParent();

    auto pSpriteText = Director::getInstance()->getTextureCache()->getTextureForKey(CF_F("image_Chesses"));
    auto pNewChess = ChessSprite::createWithTexture(pSpriteText, arrayChessImageText[uiChessId]);

    Size& sizeChess = Config::getChessSize();

    UINT uiIndex = this->uiTrashNum / 4;
    UINT uiJndex = this->uiTrashNum % 4;
    pNewChess->setPosition((sizeChess.width + 2) * uiJndex + sizeChess.width * 0.7,
                            (sizeChess.height + 2) * uiIndex + sizeChess.height * 0.9);

    pNewChess->setRotation(Config::getChessJitter().fAngle * 2 * CCRANDOM_MINUS1_1());
    this->pInfoGround->addChild(pNewChess);

    this->uiTrashNum += 1;
}

void GameScene::setGameWin(void)
{
    if (CHESSCOCLOR_BLACK == this->pChessGame->GetChessColor(uiLastMoveColor))
    {
        this->setTurnLabel(CF_S("info_black_win").c_str(), Color3B::YELLOW);
    }
    else
    {
        this->setTurnLabel(CF_S("info_red_win").c_str(), Color3B::YELLOW);
    }

    this->bGameStop = true;
}

void GameScene::menuCloseGame(Ref* pSender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    Director::getInstance()->replaceScene(GameMenu::scene());
}

UINT GameScene::getChessColor(UINT uiChessId)
{
    return this->pChessGame->GetChessColor(uiChessId);
}

void GameScene::setTurnLabel(const char* pstr, const Color3B& color)
{
    this->pTurnLabel->setString(pstr);
    this->pTurnLabel->setColor(color);
}

void GameScene::setLastMoveColor(UINT uiChessColor)
{
    this->uiLastMoveColor = uiChessColor;
}

UINT GameScene::getLastMoveColor(void)
{
    return this->uiLastMoveColor;
}
