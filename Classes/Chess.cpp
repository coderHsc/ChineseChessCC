//
//  chess.cpp
//  chinese chess
//
//  Created by woodpile on 13-12-25.
//  Copyright (c) 2013年 woodpile. All rights reserved.
//

#include "Chess.h"

using std::map;
using std::string;
using std::vector;
using std::pair;
using std::stack;
using std::make_pair;

ChessPieces::ChessPieces(void)
{
    ;
}
ChessPieces::ChessPieces(string strName, uint uiID, uint uiType, uint uiPosX, uint uiPosY, uint uiColor)
            : uiChessID(uiID), uiChessType(uiType), uiChessColor(uiColor),
              bIsAlive(true), strChessName(strName), prPos(make_pair(uiPosX, uiPosY))
{
    ;
}

string ChessPieces::GetName(void) const
{
    return strChessName;
}

uint ChessPieces::GetID(void) const
{
    return uiChessID;
}

uint ChessPieces::GetType(void) const
{
    return uiChessType;
}

uint ChessPieces::GetColor(void) const
{
    return uiChessColor;
}

pair<uint, uint> ChessPieces::GetNowPos(void) const
{
    return prPos;
}

bool ChessPieces::IsAlive(void) const
{
    return bIsAlive;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosKing(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vecMatrix;

    vector<pair<uint, uint> > vecProbPos;
    uint uiUpEdge = 0;

    if (CHESSID_KING_BLACK == uiChessID)
    {
        uiUpEdge = 1;
    }
    else
    {
        uiUpEdge = 8;
    }

    if (prPos.first > uiUpEdge)
    {
        vecProbPos.push_back(make_pair(prPos.first - 1, prPos.second));
    }
    if (prPos.first < (uiUpEdge + 2))
    {
        vecProbPos.push_back(make_pair(prPos.first + 1, prPos.second));
    }
    if (prPos.second > 4)
    {
        vecProbPos.push_back(make_pair(prPos.first, prPos.second - 1));
    }
    if (prPos.second < 6)
    {
        vecProbPos.push_back(make_pair(prPos.first, prPos.second + 1));
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosGuard(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vecMatrix;

    vector<pair<uint, uint> > vecProbPos;
    uint uiUpEdge = 0;

    if ((CHESSID_GUARD_BLACK_LEFT == uiChessID) || (CHESSID_GUARD_BLACK_RIGNT == uiChessID))
    {
        uiUpEdge = 1;
    }
    else
    {
        uiUpEdge = 8;
    }

    if ((uiUpEdge == prPos.first) || ((uiUpEdge + 2) == prPos.first))
    {
        vecProbPos.push_back(make_pair(uiUpEdge + 1, 5));
    }
    else if ((uiUpEdge + 1) == prPos.first)
    {
        vecProbPos.push_back(make_pair(uiUpEdge, 4));
        vecProbPos.push_back(make_pair(uiUpEdge, 6));
        vecProbPos.push_back(make_pair(uiUpEdge + 2, 4));
        vecProbPos.push_back(make_pair(uiUpEdge + 2, 6));
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosBishop(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vector<pair<uint, uint> > vecProbPos;
    uint uiUpEdge = 0;

    if ((CHESSID_BISHOP_BLACK_LEFT == uiChessID) || (CHESSID_BISHOP_BLACK_RIGNT == uiChessID))
    {
        uiUpEdge = 1;
    }
    else
    {
        uiUpEdge = 6;
    }

    if (uiUpEdge == prPos.first)
    {
        if (0 == vecMatrix[uiUpEdge + 1][prPos.second - 1])
        {
            vecProbPos.push_back(make_pair(uiUpEdge + 2, prPos.second - 2));
        }
        if (0 == vecMatrix[uiUpEdge + 1][prPos.second + 1])
        {
            vecProbPos.push_back(make_pair(uiUpEdge + 2, prPos.second + 2));
        }
    }
    else if ((uiUpEdge + 4) == prPos.first)
    {
        if (0 == vecMatrix[uiUpEdge + 3][prPos.second - 1])
        {
            vecProbPos.push_back(make_pair(uiUpEdge + 2, prPos.second - 2));
        }
        if (0 == vecMatrix[uiUpEdge + 3][prPos.second + 1])
        {
            vecProbPos.push_back(make_pair(uiUpEdge + 2, prPos.second + 2));
        }
    }
    else if ((uiUpEdge + 2) == prPos.first)
    {
        if (1 == prPos.second)
        {
            if (0 == vecMatrix[uiUpEdge + 1][2])
            {
                vecProbPos.push_back(make_pair(uiUpEdge, 3));
            }
            if (0 == vecMatrix[uiUpEdge + 3][2])
            {
                vecProbPos.push_back(make_pair(uiUpEdge + 4, 3));
            }
        }
        else if (9 == prPos.second)
        {
            if (0 == vecMatrix[uiUpEdge + 1][8])
            {
                vecProbPos.push_back(make_pair(uiUpEdge, 7));
            }
            if (0 == vecMatrix[uiUpEdge + 3][8])
            {
                vecProbPos.push_back(make_pair(uiUpEdge + 4, 7));
            }
        }
        else
        {
            if (0 == vecMatrix[uiUpEdge + 1][4])
            {
                vecProbPos.push_back(make_pair(uiUpEdge, 3));
            }
            if (0 == vecMatrix[uiUpEdge + 3][4])
            {
                vecProbPos.push_back(make_pair(uiUpEdge + 4, 3));
            }
            if (0 == vecMatrix[uiUpEdge + 1][6])
            {
                vecProbPos.push_back(make_pair(uiUpEdge, 7));
            }
            if (0 == vecMatrix[uiUpEdge + 3][6])
            {
                vecProbPos.push_back(make_pair(uiUpEdge + 4, 7));
            }
        }
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosHorse(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vector<pair<uint, uint> > vecProbPos;

    if ((prPos.first > 1) && (prPos.second > 2))
    {
        if (0 == vecMatrix[prPos.first][prPos.second - 1])
        {
            vecProbPos.push_back(make_pair(prPos.first - 1, prPos.second - 2));
        }
    }
    if ((prPos.first < 10) && (prPos.second > 2))
    {
        if (0 == vecMatrix[prPos.first][prPos.second - 1])
        {
            vecProbPos.push_back(make_pair(prPos.first + 1, prPos.second - 2));
        }
    }
    if ((prPos.first > 1) && (prPos.second < 8))
    {
        if (0 == vecMatrix[prPos.first][prPos.second + 1])
        {
            vecProbPos.push_back(make_pair(prPos.first - 1, prPos.second + 2));
        }
    }
    if ((prPos.first < 10) && (prPos.second < 8))
    {
        if (0 == vecMatrix[prPos.first][prPos.second + 1])
        {
            vecProbPos.push_back(make_pair(prPos.first + 1, prPos.second + 2));
        }
    }
    if ((prPos.first > 2) && (prPos.second > 1))
    {
        if (0 == vecMatrix[prPos.first - 1][prPos.second])
        {
            vecProbPos.push_back(make_pair(prPos.first - 2, prPos.second - 1));
        }
    }
    if ((prPos.first < 9) && (prPos.second > 1))
    {
        if (0 == vecMatrix[prPos.first + 1][prPos.second])
        {
            vecProbPos.push_back(make_pair(prPos.first + 2, prPos.second - 1));
        }
    }
    if ((prPos.first > 2) && (prPos.second < 9))
    {
        if (0 == vecMatrix[prPos.first - 1][prPos.second])
        {
            vecProbPos.push_back(make_pair(prPos.first - 2, prPos.second + 1));
        }
    }
    if ((prPos.first < 9) && (prPos.second < 9))
    {
        if (0 == vecMatrix[prPos.first + 1][prPos.second])
        {
            vecProbPos.push_back(make_pair(prPos.first + 2, prPos.second + 1));
        }
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosRook(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vector<pair<uint, uint> > vecProbPos;

    uint uiUpEdge = 1, uiDownEdge = 10;
    uint uiLeftEdge = 1, uiRightEdge = 9;

    for (uint uiFirst = prPos.first - 1; uiFirst >= 1; uiFirst--)
    {
        if (0 != vecMatrix[uiFirst][prPos.second])
        {
            uiUpEdge = uiFirst;
            break;
        }
    }
    for (uint uiFirst = prPos.first + 1; uiFirst <= 10; uiFirst++)
    {
        if (0 != vecMatrix[uiFirst][prPos.second])
        {
            uiDownEdge = uiFirst;
            break;
        }
    }
    for (uint uiSecond = prPos.second - 1; uiSecond >= 1; uiSecond--)
    {
        if (0 != vecMatrix[prPos.first][uiSecond])
        {
            uiLeftEdge = uiSecond;
            break;
        }
    }
    for (uint uiSecond = prPos.second + 1; uiSecond <= 9; uiSecond++)
    {
        if (0 != vecMatrix[prPos.first][uiSecond])
        {
            uiRightEdge = uiSecond;
            break;
        }
    }

    for (uint uiFirst = uiUpEdge; uiFirst < prPos.first; uiFirst++)
    {
        vecProbPos.push_back(make_pair(uiFirst, prPos.second));
    }
    for (uint uiFirst = uiDownEdge; uiFirst > prPos.first; uiFirst--)
    {
        vecProbPos.push_back(make_pair(uiFirst, prPos.second));
    }

    for (uint uiSecond = uiLeftEdge; uiSecond < prPos.second; uiSecond++)
    {
        vecProbPos.push_back(make_pair(prPos.first, uiSecond));
    }
    for (uint uiSecond = uiRightEdge; uiSecond > prPos.second; uiSecond--)
    {
        vecProbPos.push_back(make_pair(prPos.first, uiSecond));
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosCannon(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vector<pair<uint, uint> > vecProbPos;

    uint uiUpEdge = 0, uiDownEdge = 11;
    uint uiLeftEdge = 0, uiRightEdge = 10;

    for (uint uiFirst = prPos.first - 1, uiCnt = 0; uiFirst >= 1; uiFirst--)
    {
        if (0 != vecMatrix[uiFirst][prPos.second])
        {
            if (0 == uiCnt)
            {
                uiUpEdge = uiFirst;
                uiCnt++;
            }
            else if (1 == uiCnt)
            {
                vecProbPos.push_back(make_pair(uiFirst, prPos.second));
                break;
            }
        }
    }
    for (uint uiFirst = prPos.first + 1, uiCnt = 0; uiFirst <= 10; uiFirst++)
    {
        if (0 != vecMatrix[uiFirst][prPos.second])
        {
            if (0 == uiCnt)
            {
                uiDownEdge = uiFirst;
                uiCnt++;
            }
            else if (1 == uiCnt)
            {
                vecProbPos.push_back(make_pair(uiFirst, prPos.second));
                break;
            }
        }
    }
    for (uint uiSecond = prPos.second - 1, uiCnt = 0; uiSecond >= 1; uiSecond--)
    {
        if (0 != vecMatrix[prPos.first][uiSecond])
        {
            if (0 == uiCnt)
            {
                uiLeftEdge = uiSecond;
                uiCnt++;
            }
            else if (1 == uiCnt)
            {
                vecProbPos.push_back(make_pair(prPos.first, uiSecond));
                break;
            }
        }
    }
    for (uint uiSecond = prPos.second + 1, uiCnt = 0; uiSecond <= 9; uiSecond++)
    {
        if (0 != vecMatrix[prPos.first][uiSecond])
        {
            if (0 == uiCnt)
            {
                uiRightEdge = uiSecond;
                uiCnt++;
            }
            else if (1 == uiCnt)
            {
                vecProbPos.push_back(make_pair(prPos.first, uiSecond));
                break;
            }
        }
    }

    for (uint uiFirst = uiUpEdge + 1; uiFirst < prPos.first; uiFirst++)
    {
        vecProbPos.push_back(make_pair(uiFirst, prPos.second));
    }
    for (uint uiFirst = uiDownEdge - 1; uiFirst > prPos.first; uiFirst--)
    {
        vecProbPos.push_back(make_pair(uiFirst, prPos.second));
    }

    for (uint uiSecond = uiLeftEdge + 1; uiSecond < prPos.second; uiSecond++)
    {
        vecProbPos.push_back(make_pair(prPos.first, uiSecond));
    }
    for (uint uiSecond = uiRightEdge - 1; uiSecond > prPos.second; uiSecond--)
    {
        vecProbPos.push_back(make_pair(prPos.first, uiSecond));
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPosPawn(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vecMatrix;

    vector<pair<uint, uint> > vecProbPos;

    if ((CHESSID_PAWN_RED_1 <= uiChessID) && (CHESSID_PAWN_RED_5 >= uiChessID))
    {
        if (prPos.first > 1)
        {
            vecProbPos.push_back(make_pair(prPos.first - 1, prPos.second));
        }
        if (prPos.first < 6)
        {
            if (prPos.second > 1)
            {
                vecProbPos.push_back(make_pair(prPos.first, prPos.second - 1));
            }
            if (prPos.second < 9)
            {
                vecProbPos.push_back(make_pair(prPos.first, prPos.second + 1));
            }
        }
    }
    else
    {
        if (prPos.first < 10)
        {
            vecProbPos.push_back(make_pair(prPos.first + 1, prPos.second));
        }
        if (prPos.first > 5)
        {
            if (prPos.second > 1)
            {
                vecProbPos.push_back(make_pair(prPos.first, prPos.second - 1));
            }
            if (prPos.second < 9)
            {
                vecProbPos.push_back(make_pair(prPos.first, prPos.second + 1));
            }
        }
    }

    return vecProbPos;
}

vector<pair<uint, uint> > ChessPieces::GetProbPos(const std::vector<std::vector<uint> >& vecMatrix) const
{
    vector<pair<uint, uint> > vecProbPos;

    switch (uiChessType) {
        case CHESSTYPE_KING:
        {
            vecProbPos = GetProbPosKing(vecMatrix);
            break;
        }
        case CHESSTYPE_GUARD:
        {
            vecProbPos = GetProbPosGuard(vecMatrix);
            break;
        }
        case CHESSTYPE_BISHOP:
        {
            vecProbPos = GetProbPosBishop(vecMatrix);
            break;
        }
        case CHESSTYPE_HORSE:
        {
            vecProbPos = GetProbPosHorse(vecMatrix);
            break;
        }
        case CHESSTYPE_ROOK:
        {
            vecProbPos = GetProbPosRook(vecMatrix);
            break;
        }
        case CHESSTYPE_CANNON:
        {
            vecProbPos = GetProbPosCannon(vecMatrix);
            break;
        }
        case CHESSTYPE_PAWN:
        {
            vecProbPos = GetProbPosPawn(vecMatrix);
            break;
        }

        default:
            break;
    }

    return vecProbPos;
}

void ChessPieces::SetPos(uint uiPosX, uint uiPosY)
{
    prPos.first = uiPosX;
    prPos.second = uiPosY;
    return;
}

void ChessPieces::SetAlive(bool bIsAlive)
{
    bIsAlive = bIsAlive;

    if (false == bIsAlive)
    {
        prPos.first = 0;
        prPos.second = 0;
    }
    return;
}


//ChessGame定义部分

ChessGame::ChessGame(void)
{
    InitialAllChesses();

    InitialGameMatrix();
}

void ChessGame::InitialAllChesses(void)
{
    mapPieces[CHESSID_KING_BLACK] = ChessPieces(string("Black King"), CHESSID_KING_BLACK, CHESSTYPE_KING, 1, 5, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_KING_RED] = ChessPieces(string("Red King"), CHESSID_KING_RED, CHESSTYPE_KING, 10, 5, CHESSCOCLOR_RED);

    mapPieces[CHESSID_GUARD_BLACK_LEFT] = ChessPieces(string("Black Guard"), CHESSID_GUARD_BLACK_LEFT, CHESSTYPE_GUARD, 1, 4, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_GUARD_BLACK_RIGNT] = ChessPieces(string("Black Guard"), CHESSID_GUARD_BLACK_RIGNT, CHESSTYPE_GUARD, 1, 6, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_GUARD_RED_LEFT] = ChessPieces(string("Red Guard"), CHESSID_GUARD_RED_LEFT, CHESSTYPE_GUARD, 10, 4, CHESSCOCLOR_RED);
    mapPieces[CHESSID_GUARD_RED_RIGHT] = ChessPieces(string("Red Guard"), CHESSID_GUARD_RED_RIGHT, CHESSTYPE_GUARD, 10, 6, CHESSCOCLOR_RED);

    mapPieces[CHESSID_BISHOP_BLACK_LEFT] = ChessPieces(string("Black Bishop"), CHESSID_BISHOP_BLACK_LEFT, CHESSTYPE_BISHOP, 1, 3, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_BISHOP_BLACK_RIGNT] = ChessPieces(string("Black Bishop"), CHESSID_BISHOP_BLACK_RIGNT, CHESSTYPE_BISHOP, 1, 7, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_BISHOP_RED_LEFT] = ChessPieces(string("Red Bishop"), CHESSID_BISHOP_RED_LEFT, CHESSTYPE_BISHOP, 10, 3, CHESSCOCLOR_RED);
    mapPieces[CHESSID_BISHOP_RED_RIGHT] = ChessPieces(string("Red Bishop"), CHESSID_BISHOP_RED_RIGHT, CHESSTYPE_BISHOP, 10, 7, CHESSCOCLOR_RED);

    mapPieces[CHESSID_HORSE_BLACK_LEFT] = ChessPieces(string("Black Horse"), CHESSID_HORSE_BLACK_LEFT, CHESSTYPE_HORSE, 1, 2, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_HORSE_BLACK_RIGNT] = ChessPieces(string("Black Horse"), CHESSID_HORSE_BLACK_RIGNT, CHESSTYPE_HORSE, 1, 8, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_HORSE_RED_LEFT] = ChessPieces(string("Red Horse"), CHESSID_HORSE_RED_LEFT, CHESSTYPE_HORSE, 10, 2, CHESSCOCLOR_RED);
    mapPieces[CHESSID_HORSE_RED_RIGHT] = ChessPieces(string("Red Horse"), CHESSID_HORSE_RED_RIGHT, CHESSTYPE_HORSE, 10, 8, CHESSCOCLOR_RED);

    mapPieces[CHESSID_ROOK_BLACK_LEFT] = ChessPieces(string("Black Rook"), CHESSID_ROOK_BLACK_LEFT, CHESSTYPE_ROOK, 1, 1, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_ROOK_BLACK_RIGNT] = ChessPieces(string("Black Rook"), CHESSID_ROOK_BLACK_RIGNT, CHESSTYPE_ROOK, 1, 9, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_ROOK_RED_LEFT] = ChessPieces(string("Red Rook"), CHESSID_ROOK_RED_LEFT, CHESSTYPE_ROOK, 10, 1, CHESSCOCLOR_RED);
    mapPieces[CHESSID_ROOK_RED_RIGHT] = ChessPieces(string("Red Rook"), CHESSID_ROOK_RED_RIGHT, CHESSTYPE_ROOK, 10, 9, CHESSCOCLOR_RED);

    mapPieces[CHESSID_CANNON_BLACK_LEFT] = ChessPieces(string("Black Cannon"), CHESSID_CANNON_BLACK_LEFT, CHESSTYPE_CANNON, 3, 2, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_CANNON_BLACK_RIGNT] = ChessPieces(string("Black Cannon"), CHESSID_CANNON_BLACK_RIGNT, CHESSTYPE_CANNON, 3, 8, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_CANNON_RED_LEFT] = ChessPieces(string("Red Cannon"), CHESSID_CANNON_RED_LEFT, CHESSTYPE_CANNON, 8, 2, CHESSCOCLOR_RED);
    mapPieces[CHESSID_CANNON_RED_RIGHT] = ChessPieces(string("Red Cannon"), CHESSID_CANNON_RED_RIGHT, CHESSTYPE_CANNON, 8, 8, CHESSCOCLOR_RED);

    mapPieces[CHESSID_PAWN_BLACK_1] = ChessPieces(string("Black Pawn"), CHESSID_PAWN_BLACK_1, CHESSTYPE_PAWN, 4, 1, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_PAWN_BLACK_2] = ChessPieces(string("Black Pawn"), CHESSID_PAWN_BLACK_2, CHESSTYPE_PAWN, 4, 3, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_PAWN_BLACK_3] = ChessPieces(string("Black Pawn"), CHESSID_PAWN_BLACK_3, CHESSTYPE_PAWN, 4, 5, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_PAWN_BLACK_4] = ChessPieces(string("Black Pawn"), CHESSID_PAWN_BLACK_4, CHESSTYPE_PAWN, 4, 7, CHESSCOCLOR_BLACK);
    mapPieces[CHESSID_PAWN_BLACK_5] = ChessPieces(string("Black Pawn"), CHESSID_PAWN_BLACK_5, CHESSTYPE_PAWN, 4, 9, CHESSCOCLOR_BLACK);

    mapPieces[CHESSID_PAWN_RED_1] = ChessPieces(string("Red Pawn"), CHESSID_PAWN_RED_1, CHESSTYPE_PAWN, 7, 1, CHESSCOCLOR_RED);
    mapPieces[CHESSID_PAWN_RED_2] = ChessPieces(string("Red Pawn"), CHESSID_PAWN_RED_2, CHESSTYPE_PAWN, 7, 3, CHESSCOCLOR_RED);
    mapPieces[CHESSID_PAWN_RED_3] = ChessPieces(string("Red Pawn"), CHESSID_PAWN_RED_3, CHESSTYPE_PAWN, 7, 5, CHESSCOCLOR_RED);
    mapPieces[CHESSID_PAWN_RED_4] = ChessPieces(string("Red Pawn"), CHESSID_PAWN_RED_4, CHESSTYPE_PAWN, 7, 7, CHESSCOCLOR_RED);
    mapPieces[CHESSID_PAWN_RED_5] = ChessPieces(string("Red Pawn"), CHESSID_PAWN_RED_5, CHESSTYPE_PAWN, 7, 9, CHESSCOCLOR_RED);

    return;
}

void ChessGame::InitialGameMatrix(void)
{
    vector<uint> vec(10);
    for (uint uiIndex = 0; uiIndex < 12; uiIndex++)
    {
        vecMatrix.push_back(vec);
    }

    for (map<uint, ChessPieces>::const_iterator it = mapPieces.begin(); it != mapPieces.end(); it++)
    {
        vecMatrix[it->second.GetNowPos().first][it->second.GetNowPos().second] = 1;
    }

    return;
}

pair<uint, uint> ChessGame::GetChessPos(uint uiChessId) const
{
    map<uint, ChessPieces>::const_iterator it = mapPieces.find(uiChessId);

    if (mapPieces.end() == it)
    {
        return make_pair(0, 0);
    }
    else
    {
        return it->second.GetNowPos();
    }
}

vector<vector<uint> >& ChessGame::GetMatrix(void)
{
    return vecMatrix;
}

uint ChessGame::GetChessID(uint uiPosX, uint uiPoxY) const
{
    if (0 == vecMatrix[uiPosX][uiPoxY])
    {
        return 0;
    }

    uint uiRet = 0;

    for (map<uint, ChessPieces>::const_iterator it = mapPieces.begin(); it != mapPieces.end(); it++)
    {
        if ((it->second.GetNowPos().first == uiPosX) && (it->second.GetNowPos().second == uiPoxY))
        {
            uiRet = it->second.GetID();
        }
    }

    return uiRet;
}

uint ChessGame::GetChessColor(uint uiChessId) const
{
    map<uint, ChessPieces>::const_iterator it = mapPieces.find(uiChessId);

    if (mapPieces.end() == it)
    {
        return CHESSCOCLOR_MIN;
    }
    else
    {
        return it->second.GetColor();
    }
}

uint ChessGame::GetChessType(uint uiChessId) const
{
    map<uint, ChessPieces>::const_iterator it = mapPieces.find(uiChessId);

    if (mapPieces.end() == it)
    {
        return CHESSTYPE_MIN;;
    }
    else
    {
        return it->second.GetType();
    }
}

string ChessGame::GetChessName(uint uiChessId) const
{
    map<uint, ChessPieces>::const_iterator it = mapPieces.find(uiChessId);

    if (mapPieces.end() == it)
    {
        return string();
    }
    else
    {
        return it->second.GetName();
    }
}

vector<pair<uint, uint> > ChessGame::GetChessProbPos(uint uiChessId) const
{
    map<uint, ChessPieces>::const_iterator it = mapPieces.find(uiChessId);

    if (mapPieces.end() == it)
    {
        vector<pair<uint, uint> > vecr;
        return vecr;
    }
    else
    {
        return it->second.GetProbPos(vecMatrix);
    }
}

bool ChessGame::CheckChessMoveValid(uint uiChessID, uint uiPosX, uint uiPosY) const
{
    vector<pair<uint, uint> > vecChessPos = GetChessProbPos(uiChessID);
    if (0 == vecChessPos.size())
    {
        return false;
    }

    vector<pair<uint, uint> >::iterator it;
    for (it = vecChessPos.begin(); it != vecChessPos.end(); it++)
    {
        if (*it == make_pair(uiPosX, uiPosY))
        {
            break;
        }
    }
    if (it == vecChessPos.end())
    {
        return false;
    }

    if (0 == vecMatrix[uiPosX][uiPosY])
    {
        return true;
    }

    map<uint, ChessPieces>::const_iterator itTag = mapPieces.find(GetChessID(uiPosX, uiPosY));
    map<uint, ChessPieces>::const_iterator itSelf = mapPieces.find(uiChessID);

    if (itTag->second.GetColor() == itSelf->second.GetColor())
    {
        return false;
    }

    return true;
}

stack<ChessRecord> ChessGame::GetAllRecord(void)
{
    return stkRecord;
}

void ChessGame::AddRecord(uint uiChessId, uint uiPosX, uint uiPosY)
{
    ChessRecord stRecord;
    stRecord.uiChessID = uiChessId;
    stRecord.uiPosX = uiPosX;
    stRecord.uiPosY = uiPosY;

    stkRecord.push(stRecord);

    return;
}

bool ChessGame::SetChessPos(uint uiChessId, uint uiPosX, uint uiPosY)
{
    if ((uiPosX > 10) || (uiPosY > 9))
    {
        return false;
    }

    map<uint, ChessPieces>::iterator it = mapPieces.find(uiChessId);
    if (it == mapPieces.end())
    {
        return false;
    }

    vecMatrix[it->second.GetNowPos().first][it->second.GetNowPos().second] = 0;
    it->second.SetPos(uiPosX, uiPosY);
    vecMatrix[uiPosX][uiPosY] = 1;

    AddRecord(uiChessId, uiPosX, uiPosY);

    return true;
}

void ChessGame::SetChessAlive(uint uiChessId, bool bIsAlive)
{
    map<uint, ChessPieces>::iterator it = mapPieces.find(uiChessId);
    if (it == mapPieces.end())
    {
        return;
    }

    it->second.SetAlive(bIsAlive);

    AddRecord(uiChessId, 0, 0);

    return;
}
