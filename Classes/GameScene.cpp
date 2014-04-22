#include "GameScene.h"
#include "ChessSprite.h"

USING_NS_CC;

GameScene::GameScene()
:pChessGame(NULL)
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

    scene->addChild(layer);

    return scene;
}

void GameScene::CreateChesses(void)
{
    std::vector<std::vector<UINT> > vecMatrix = pChessGame->GetMatrix();
    CCTexture2D *pSpriteText = CCTextureCache::sharedTextureCache()->addImage("Chesses.png");
    for (UINT uiLine = 1; uiLine <= CHESS_DATA_LINES; uiLine++)
    {
        for (UINT uiIndex = 1; uiIndex <= CHESS_DATA_COLUMN; uiIndex++)
        {
            if (0 == vecMatrix[uiLine][uiIndex])
            {
                continue;
            }
            Chess *Chess = Chess::createWithTexture(pSpriteText, CCRectMake(10, 6, 80, 48));
            Chess->setPosition(ccp(-20 + 100 * uiIndex, -10 + 60 * uiLine));
            this->addChild(Chess, uiLine * 4 + uiIndex);
        }
    }

    return;
}