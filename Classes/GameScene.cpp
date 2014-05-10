#include "GameMenu.h"
#include "GameScene.h"
#include "ChessSprite.h"

USING_NS_CC;

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

    Sprite *GameGround = Sprite::create("ChessBackground2.png");
    GameGround->setPosition(GameGround->getContentSize().width * 0.5, size.height * 0.5);
    this->addChild(GameGround, 0);

    this->pChessGame = new ChessGame();
    CreateChesses();

    Menu* pItemMenu = Menu::create();
    auto labelClose = Label::createWithTTF("close game", "fonts/arial.ttf", 20);
    MenuItemLabel* pMenuClose = MenuItemLabel::create(labelClose, CC_CALLBACK_1(GameScene::menuCloseGame, this));
    pItemMenu->addChild(pMenuClose);
    pItemMenu->setPosition(60, 15);
    this->addChild(pItemMenu);

    this->pInfoGround = Sprite::create("InfoBackground.png");
    this->pInfoGround->setPosition(GameGround->getContentSize().width + this->pInfoGround->getContentSize().width * 0.5, size.height * 0.5);
    this->addChild(pInfoGround, 0);

    this->pTurnLabel = Label::createWithTTF("Now turn BLACK", "fonts/GILSANUB.ttf", 20);
    this->pTurnLabel->setColor(Color3B::BLACK);
    this->pInfoGround->addChild(this->pTurnLabel);
    this->pTurnLabel->setPosition(this->pInfoGround->getContentSize().width * 0.5, this->pInfoGround->getContentSize().height - 30);

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
    auto pSpriteText = Director::getInstance()->getTextureCache()->addImage("ChessesM.png");
    float fHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;
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
			Point point = Point(-12 + 60 * uiIndex, -10 + 60 * uiLine);
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

void GameScene::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
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
        this->setTurnLabel("Now turn RED", Color3B::RED);
    }
    else
    {
        this->setTurnLabel("Now turn BLACK", Color3B::BLACK);
    }


    if (CHESSTYPE_KING == this->pChessGame->GetChessType(uiTargetId))
    {
        this->setGameWin();
    }

    return;
}

void GameScene::moveChessToTrash(UINT uiChessId)
{
    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->removeFromParent();

    auto pSpriteText = Director::getInstance()->getTextureCache()->getTextureForKey("ChessesM.png");
    auto pNewChess = ChessSprite::createWithTexture(pSpriteText, arrayChessImageText[uiChessId]);

    UINT uiIndex = this->uiTrashNum / 4;
    UINT uiJndex = this->uiTrashNum % 4;
    pNewChess->setPosition(52 * uiJndex + 35, 52 * uiIndex + 45);

    pNewChess->setRotation(20 * CCRANDOM_MINUS1_1());
    this->pInfoGround->addChild(pNewChess);

    this->uiTrashNum += 1;
}

void GameScene::setGameWin(void)
{
    char* pstr = NULL;
    if (CHESSCOCLOR_BLACK == this->pChessGame->GetChessColor(uiLastMoveColor))
    {
        pstr = (char*)"Black Win !!!";
    }
    else
    {
        pstr = (char*)"Red Win !!!";
    }
    this->setTurnLabel(pstr, Color3B::YELLOW);

    this->bGameStop = true;
}

void GameScene::menuCloseGame(Ref* pSender)
{
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
