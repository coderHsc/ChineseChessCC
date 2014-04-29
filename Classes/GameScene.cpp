#include "GameMenu.h"
#include "GameScene.h"
#include "ChessSprite.h"

USING_NS_CC;

GameScene* GameScene::pGameScene = NULL;

GameScene::GameScene()
:pChessGame(NULL), uiLastMoveColor(CHESSCOCLOR_MIN), pInfoGround(NULL), uiTrashNum(0)
{
}

CCRect arrayChessImageText[CHESSID_MAX] = {
	CCRect(0, 0, 0, 0),			//CHESSID_MIN = 0,

	CCRect(0, 0, 50, 50),		//CHESSID_KING_BLACK,
	CCRect(350, 0, 50, 50),		//CHESSID_KING_RED,

	CCRect(50, 0, 50, 50),		//CHESSID_GUARD_BLACK_LEFT,
	CCRect(50, 0, 50, 50),		//CHESSID_GUARD_BLACK_RIGNT,
	CCRect(400, 0, 50, 50),		//CHESSID_GUARD_RED_LEFT,
	CCRect(400, 0, 50, 50),		//CHESSID_GUARD_RED_RIGHT,

	CCRect(100, 0, 50, 50),		//CHESSID_BISHOP_BLACK_LEFT,
	CCRect(100, 0, 50, 50),		//CHESSID_BISHOP_BLACK_RIGNT,
	CCRect(450, 0, 50, 50),		//CHESSID_BISHOP_RED_LEFT,
	CCRect(450, 0, 50, 50),		//CHESSID_BISHOP_RED_RIGHT,

	CCRect(150, 0, 50, 50),		//CHESSID_HORSE_BLACK_LEFT,
	CCRect(150, 0, 50, 50),		//CHESSID_HORSE_BLACK_RIGNT,
	CCRect(500, 0, 50, 50),		//CHESSID_HORSE_RED_LEFT,
	CCRect(500, 0, 50, 50),		//CHESSID_HORSE_RED_RIGHT,

	CCRect(200, 0, 50, 50),		//CHESSID_ROOK_BLACK_LEFT,
	CCRect(200, 0, 50, 50),		//CHESSID_ROOK_BLACK_RIGNT,
	CCRect(550, 0, 50, 50),		//CHESSID_ROOK_RED_LEFT,
	CCRect(550, 0, 50, 50),		//CHESSID_ROOK_RED_RIGHT,

	CCRect(250, 0, 50, 50),		//CHESSID_CANNON_BLACK_LEFT,
	CCRect(250, 0, 50, 50),		//CHESSID_CANNON_BLACK_RIGNT,
	CCRect(600, 0, 50, 50),		//CHESSID_CANNON_RED_LEFT,
	CCRect(600, 0, 50, 50),		//CHESSID_CANNON_RED_RIGHT,

	CCRect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_1,
	CCRect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_2,
	CCRect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_3,
	CCRect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_4,
	CCRect(300, 0, 50, 50),		//CHESSID_PAWN_BLACK_5,
	CCRect(650, 0, 50, 50),		//CHESSID_PAWN_RED_1,
	CCRect(650, 0, 50, 50),		//CHESSID_PAWN_RED_2,
	CCRect(650, 0, 50, 50),		//CHESSID_PAWN_RED_3,
	CCRect(650, 0, 50, 50),		//CHESSID_PAWN_RED_4,
	CCRect(650, 0, 50, 50)		//CHESSID_PAWN_RED_5,
};


GameScene::~GameScene()
{
    if (NULL != pChessGame)
    {
        delete pChessGame;
    }
}

bool GameScene::init(void)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCSprite *GameGround = CCSprite::create("ChessBackground2.png");
    GameGround->setPosition(ccp(GameGround->getContentSize().width * 0.5, size.height * 0.5));
    this->addChild(GameGround, 0);

    pChessGame = new ChessGame();
    CreateChesses();

    CCMenu* pItemMenu = CCMenu::create();
    CCLabelTTF* labelClose = CCLabelTTF::create("close game", "Arial", 20);
    CCMenuItemLabel* pMenuClose = CCMenuItemLabel::create(labelClose, this, menu_selector(GameScene::menuCloseGame));
    pItemMenu->addChild(pMenuClose);
    pItemMenu->setPosition(60, 15);
    this->addChild(pItemMenu);

    pInfoGround = CCSprite::create("InfoBackground.png");
    pInfoGround->setPosition(ccp(GameGround->getContentSize().width + pInfoGround->getContentSize().width * 0.5, size.height * 0.5));
    this->addChild(pInfoGround, 0);

    pTurnLabel = CCLabelTTF::create("Now turn BLACK", "Gill Sans Ultra Bold", 20);
    pTurnLabel->setColor(ccBLACK);
    pInfoGround->addChild(pTurnLabel);
    pTurnLabel->setPosition(ccp(pInfoGround->getContentSize().width * 0.5, pInfoGround->getContentSize().height - 30));
    
    return true;
}

CCScene* GameScene::scene(void)
{
    CCScene *scene = CCScene::create();

    GameScene *layer = GameScene::create();
    pGameScene = layer;

    scene->addChild(layer);

    return scene;
}

void GameScene::CreateChesses(void)
{
    CCLog("*********CreateChesses****************");
    std::vector<std::vector<UINT> > vecMatrix = pChessGame->GetMatrix();
    CCTexture2D *pSpriteText = CCTextureCache::sharedTextureCache()->addImage("ChessesM.png");
    float fHeight = CCEGLView::sharedOpenGLView()->getFrameSize().height;
    for (UINT uiLine = 1; uiLine <= CHESS_DATA_LINES; uiLine++)
    {
        for (UINT uiIndex = 1; uiIndex <= CHESS_DATA_COLUMN; uiIndex++)
        {
            if (0 == vecMatrix[uiLine][uiIndex])
            {
                continue;
            }
			UINT uiId = pChessGame->GetChessID(uiLine, uiIndex);
			ChessSprite *pChess = ChessSprite::createWithTexture(pSpriteText, arrayChessImageText[uiId]);
			CCPoint point = ccp(-12 + 60 * uiIndex, -10 + 60 * uiLine);
            point.y = fHeight - point.y;
            pChess->setPosition(point);
            pChess->setChessId(uiId);
            this->addChild(pChess, uiLine * 4 + uiIndex, CHESS_TAG_BASE + uiId);
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
    if (NULL != pChessGame)
    {
        delete pChessGame;
    }

    pChessGame = new ChessGame();
}

bool GameScene::checkChessMoveIsValid(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    if (uiLastMoveColor == pChessGame->GetChessColor(uiChessId))
    {
		CCLog("chess move invalid, chess color: %d, last color: %d", pChessGame->GetChessColor(uiChessId), uiLastMoveColor);
        return false;
    }
    return pChessGame->CheckChessMoveValid(uiChessId, uiPosX, uiPosY);
}

void GameScene::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    CCLog("chess moving, chess id :%d, move (%d, %d)", uiChessId, uiPosX, uiPosY);

    UINT uiTargetId = pChessGame->GetChessID(uiPosX, uiPosY);
    if (0 != uiTargetId)
    {
        this->moveChessToTrash(uiTargetId);

        pChessGame->SetChessPos(uiTargetId, 0, 0);
        pChessGame->SetChessAlive(uiTargetId, false);
    }

    pChessGame->SetChessPos(uiChessId, uiPosX, uiPosY);
    uiLastMoveColor = pChessGame->GetChessColor(uiChessId);

    if (CHESSCOCLOR_BLACK == uiLastMoveColor)
    {
        this->setTurnLabel("Now turn RED", ccRED);
    }
    else
    {
        this->setTurnLabel("Now turn BLACK", ccBLACK);
    }
    

    if (CHESSTYPE_KING == pChessGame->GetChessType(uiTargetId))
    {
        this->setGameWin();
    }

    return;
}

void GameScene::moveChessToTrash(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->removeFromParent();
    pInfoGround->addChild(pChess);

    UINT uiIndex = uiTrashNum / 4;
    UINT uiJndex = uiTrashNum % 4;
    pChess->setPosition(ccp(52 * uiJndex + 35, 52 * uiIndex + 45));

    pChess->setRotation(20 * CCRANDOM_MINUS1_1());

    uiTrashNum += 1;
}

void GameScene::setGameWin(void)
{
    char* pstr = NULL;
    if (CHESSCOCLOR_BLACK == pChessGame->GetChessColor(uiLastMoveColor))
    {
        pstr = "Black Win !!!";
    }
    else
    {
        pstr = "Red Win !!!";
    }
    this->setTurnLabel(pstr, ccYELLOW);
}

void GameScene::menuCloseGame(CCObject* pSender)
{
    CCDirector::sharedDirector()->replaceScene(GameMenu::scene());
}

UINT GameScene::getChessColor(UINT uiChessId)
{
    return pChessGame->GetChessColor(uiChessId);
}

void GameScene::setTurnLabel(const char* pstr, const ccColor3B& color)
{
    pTurnLabel->setString(pstr);
    pTurnLabel->setColor(color);
}

void GameScene::setLastMoveColor(UINT uiChessColor)
{
    uiLastMoveColor = uiChessColor;
}

UINT GameScene::getLastMoveColor(void)
{
    return uiLastMoveColor;
}
