#include "GameScene.h"
#include "ChessSprite.h"

USING_NS_CC;

#define CHESS_TAG_BASE  100

GameScene *pGameScene = NULL;

GameScene::GameScene()
:pChessGame(NULL), uiLastMoveColor(CHESSCOCLOR_MIN)
{
}


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

    CCSprite *GameGround = CCSprite::create("ChessBackground.png");
    GameGround->setPosition(ccp(size.width * 0.5, size.height * 0.5));
    this->addChild(GameGround, 0);

    pChessGame = new ChessGame();
    CreateChesses();

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
    CCTexture2D *pSpriteText = CCTextureCache::sharedTextureCache()->addImage("Chesses.png");
    float fHeight = CCEGLView::sharedOpenGLView()->getFrameSize().height;
    for (UINT uiLine = 1; uiLine <= CHESS_DATA_LINES; uiLine++)
    {
        for (UINT uiIndex = 1; uiIndex <= CHESS_DATA_COLUMN; uiIndex++)
        {
            if (0 == vecMatrix[uiLine][uiIndex])
            {
                continue;
            }
            ChessSprite *pChess = ChessSprite::createWithTexture(pSpriteText, CCRectMake(10, 6, 80, 48));
            CCPoint point = ccp(-20 + 100 * uiIndex, -10 + 60 * uiLine);
            point.y = fHeight - point.y;
            pChess->setPosition(point);
            UINT uiId = pChessGame->GetChessID(uiLine, uiIndex);
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
        this->removeChildByTag(CHESS_TAG_BASE + uiTargetId);
        pChessGame->SetChessPos(uiTargetId, 0, 0);
        pChessGame->SetChessAlive(uiTargetId, false);
    }

    pChessGame->SetChessPos(uiChessId, uiPosX, uiPosY);
    uiLastMoveColor = pChessGame->GetChessColor(uiChessId);

    if (CHESSTYPE_KING == pChessGame->GetChessType(uiTargetId))
    {
        CCDirector::sharedDirector()->replaceScene(GameScene::scene());
    }

    return;
}
