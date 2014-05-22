package game

import (
	"fmt"
	"log"
	"toolsql"
	"user"
)

type MoveRecord struct {
	Chess int
	X     int
	Y     int
}

type gameData struct {
	ub int
	ur int

	last int

	record []MoveRecord
}

type gErr struct {
	s string
}

func (g *gameData) init(id, op int) {
	g.ub = id
	g.ur = op
	g.last = g.ur
	g.record = make([]MoveRecord, 0, 100)
}

func (g *gameData) getOppoId(id int) int {
	if id == g.ub {
		return g.ur
	} else if id == g.ur {
		return g.ub
	} else {
		return 0
	}
}

func (g *gameData) chessMove(id int, r MoveRecord) error {
	if id == g.last {
		return &gErr{"no turn"}
	}

	//check
	g.record = append(g.record, r)
	g.last = id
	return nil
}

func (g *gameData) getOppoMove(id int) *MoveRecord {
	if id == g.last {
		return nil
	}

	return &g.record[len(g.record)-1]
}

func (g *gameData) leaveGame(id int) {
	if id == g.ub {
		g.ub = 0
	} else if id == g.ur {
		g.ur = 0
	}
	return
}

func (e *gErr) Error() string {
	return e.s
}

var mGame map[int]*gameData = make(map[int]*gameData)
var mWaitUser map[int]bool = make(map[int]bool)

var gGameIndex int = 100

func getNewGameId() int {
	gGameIndex += 1
	return gGameIndex
}

func FindGame(id int) string {
	g := user.GetGame(id)
	if 0 == g {
		g = findGame(id)
	}

	return retGame(g)
}

func findGame(id int) int {
	op := selectOpponent(id)
	if 0 == op {
		waitGame(id)
		return 0
	}

	g := createGame(id, op)
	return g
}

func selectOpponent(id int) (op int) {
	op = 0
	for t := selectWaitUser(id); 0 != t; t = selectWaitUser(id) {
		if false == user.LogCheck(t) {
			unWaitGame(t)
			continue
		}

		op = t
		break
	}

	return
}

func selectWaitUser(id int) (op int) {
	op = 0
	for oid, live := range mWaitUser {
		if oid != id && true == live {
			op = oid
			break
		}
	}

	return
}

func waitGame(id int) {
	_, e := mWaitUser[id]
	if false == e {
		mWaitUser[id] = true
		log.Printf("user %d wait a game", id)
	}
}

func unWaitGame(id int) {
	_, e := mWaitUser[id]
	if false != e {
		delete(mWaitUser, id)
		log.Println("id", id, "delete from mWaitUser")
	}
}

func createGame(id, op int) int {
	g := getNewGameId()
	if false == user.JoinGame(id, g) || false == user.JoinGame(op, g) {
		user.LeaveGame(id)
		user.LeaveGame(op)
		return 0
	}

	p := new(gameData)
	mGame[g] = p
	p.init(id, op)

	unWaitGame(id)
	unWaitGame(op)

	log.Printf("create game %d, black:%d <-> red:%d.\n", g, id, op)

	return g
}

func ChessMove(id int, r MoveRecord) string {
	g := user.GetGame(id)
	p := mGame[g]
	if nil == p || (id != p.ub && id != p.ur) {
		return "ret=error"
	}

	ret := p.chessMove(id, r)
	if nil == ret {
		return "ret=success"
	} else {
		return fmt.Sprintf("ret=failed %s", ret.Error())
	}
}

func GetOppoMove(id int) string {
	g := user.GetGame(id)
	p := mGame[g]
	if nil == p {
		return "ret=error"
	}

	ret := p.getOppoMove(id)
	if nil != ret {
		return fmt.Sprintf("ret=success chess=%d posx=%d posy=%d", ret.Chess, ret.X, ret.Y)
	}

	if g != user.GetGame(p.getOppoId(id)) {
		return "ret=oppoleave"
	}

	return "ret=nonew"
}

func LeaveGame(id int) string {
	p := mGame[user.GetGame(id)]
	if nil == p {
		return "ret=error"
	}

	log.Printf("user %d leave game %d", id, user.GetGame(id))
	if 0 == p.getOppoId(id) || 0 == user.GetGame(p.getOppoId(id)) {
		delete(mGame, user.GetGame(id))
	} else {
		p.leaveGame(id)
	}
	user.LeaveGame(id)

	return "ret=success"
}

//return string
func retGame(g int) string {
	if 0 == g || nil == mGame[g] {
		return fmt.Sprintf("game=0")
	}

	return fmt.Sprintf("game=%d uib=%d black=%s red=%s", g, mGame[g].ub, toolsql.GetUserName(mGame[g].ub), toolsql.GetUserName(mGame[g].ur))
}
