//
//  chess.h
//  chinese chess
//
//  Created by woodpile on 13-12-25.
//  Copyright (c) 2013年 woodpile. All rights reserved.
//

#ifndef __chinese_chess__chess__
#define __chinese_chess__chess__

#include "prefix.h"

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <stack>
#include <map>

enum emChessID {
    CHESSID_MIN = 0,

    CHESSID_KING_BLACK,
    CHESSID_KING_RED,

    CHESSID_GUARD_BLACK_LEFT,
    CHESSID_GUARD_BLACK_RIGNT,
    CHESSID_GUARD_RED_LEFT,
    CHESSID_GUARD_RED_RIGHT,

    CHESSID_BISHOP_BLACK_LEFT,
    CHESSID_BISHOP_BLACK_RIGNT,
    CHESSID_BISHOP_RED_LEFT,
    CHESSID_BISHOP_RED_RIGHT,

    CHESSID_HORSE_BLACK_LEFT,
    CHESSID_HORSE_BLACK_RIGNT,
    CHESSID_HORSE_RED_LEFT,
    CHESSID_HORSE_RED_RIGHT,

    CHESSID_ROOK_BLACK_LEFT,
    CHESSID_ROOK_BLACK_RIGNT,
    CHESSID_ROOK_RED_LEFT,
    CHESSID_ROOK_RED_RIGHT,

    CHESSID_CANNON_BLACK_LEFT,
    CHESSID_CANNON_BLACK_RIGNT,
    CHESSID_CANNON_RED_LEFT,
    CHESSID_CANNON_RED_RIGHT,

    CHESSID_PAWN_BLACK_1,
    CHESSID_PAWN_BLACK_2,
    CHESSID_PAWN_BLACK_3,
    CHESSID_PAWN_BLACK_4,
    CHESSID_PAWN_BLACK_5,
    CHESSID_PAWN_RED_1,
    CHESSID_PAWN_RED_2,
    CHESSID_PAWN_RED_3,
    CHESSID_PAWN_RED_4,
    CHESSID_PAWN_RED_5,

    CHESSID_MAX
};

enum emChessType {
    CHESSTYPE_MIN = 0,

    CHESSTYPE_KING,
    CHESSTYPE_GUARD,
    CHESSTYPE_BISHOP,
    CHESSTYPE_HORSE,
    CHESSTYPE_ROOK,
    CHESSTYPE_CANNON,
    CHESSTYPE_PAWN,

    CHESSTYPE_MAX
};

enum emChessColor {
    CHESSCOCLOR_MIN = 0,

    CHESSCOCLOR_BLACK,
    CHESSCOCLOR_RED,

    CHESSCOCLOR_MAX
};

#define CHESS_DATA_LINES    10
#define CHESS_DATA_COLUMN   9

struct ChessRecord {
    uint uiChessID;
    uint uiPosX;
    uint uiPosY;
};

class ChessPieces {
public:
    ChessPieces(void);
    ChessPieces(std::string strName, uint uiID, uint uiType, uint uiPosX, uint uiPosY, uint uiColor);
    //~ChessPieces(void);

    std::string GetName(void) const;
    uint GetID(void) const;
    uint GetType(void) const;
    uint GetColor(void) const;
    std::pair<uint, uint> GetNowPos(void) const;
    bool IsAlive(void) const;

    std::vector<std::pair<uint, uint> > GetProbPos(const std::vector<std::vector<uint> >& vecMatrix) const;

    void SetPos(uint uiPosX, uint uiPosY);
    void SetAlive(bool bIsAlive);

private:
    std::vector<std::pair<uint, uint> > GetProbPosKing(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosGuard(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosBishop(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosHorse(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosRook(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosCannon(const std::vector<std::vector<uint> >& vecMatrix) const;
    std::vector<std::pair<uint, uint> > GetProbPosPawn(const std::vector<std::vector<uint> >& vecMatrix) const;

    uint uiChessID;
    uint uiChessType;
    uint uiChessColor;
    bool bIsAlive;
    std::string strChessName;
    std::pair<uint, uint> prPos;
};

class ChessGame {
public:
    ChessGame(void);
    //~ChessGame(void);

    std::vector<std::vector<uint> >& GetMatrix(void);
    uint GetChessID(uint uiPosX, uint uiPosY) const;
    std::pair<uint, uint> GetChessPos(uint uiChessID) const;
    uint GetChessType(uint uiChessID) const;
    uint GetChessColor(uint uiChessID) const;
    std::string GetChessName(uint uiChessID) const;
    std::vector<std::pair<uint, uint> > GetChessProbPos(uint uiChessID) const;
    bool CheckChessMoveValid(uint uiChessID, uint uiPosX, uint uiPosY) const;

    bool SetChessPos(uint uiChessId, uint uiPosX, uint uiPosY);
    void SetChessAlive(uint uiChessId, bool bIsAlive);

    std::stack<ChessRecord> GetAllRecord(void);

private:
    void InitialAllChesses(void);
    void InitialGameMatrix(void);

    void AddRecord(uint uiChessId, uint uiPosX, uint uiPosY);

    std::map<uint, ChessPieces> mapPieces;
    std::vector<std::vector<uint> > vecMatrix;
    std::stack<ChessRecord> stkRecord;
};

#endif /* defined(__chinese_chess__chess__) */
