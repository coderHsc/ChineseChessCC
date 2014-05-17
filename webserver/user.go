package main

import (
	"log"
	"toolsql"
)

type uErr struct {
	s string
}

func (e *uErr) Error() string {
	return e.s
}

type userData struct {
	id     int
	alive  bool
	gameId int
}

func (u *userData) init(id int) {
	u.id = id
	u.alive = false
	u.gameId = 0
}

var mUser map[int]*userData = make(map[int]*userData)

func RegisterUser(u, p string) (int, string) {
	err := checkUnAndPw(u, p)
	if nil != err {
		return 0, err.Error()
	}

	id, ret := toolsql.CreateUser(u, p)
	log.Printf("register: uid=%d %s", id, ret)

	return id, ret
}

func LoginUser(u, p string) (int, string) {
	err := checkUnAndPw(u, p)
	if nil != err {
		return 0, err.Error()
	}

	id, ret := toolsql.GetUserId(u, p)
	log.Printf("login: uid=%d %s", id, ret)

	if 0 != id {
		procLoginUser(id)
	}

	return id, ret
}

func LogoffUser(id int) {
	procLogoffUser(id)
	log.Printf("logoff: uid=%d", id)
}

func procLoginUser(id int) {
	pU := getUser(id)
	if nil == pU {
		pU = initUser(id)
	}

	pU.alive = true
}

func procLogoffUser(id int) {
	pU := getUser(id)
	if nil == pU {
		return
	}

	pU.alive = false
}

func initUser(id int) *userData {
	pU := new(userData)
	mUser[id] = pU
	pU.init(id)
	return pU
}

func getUser(id int) *userData {
	p, ok := mUser[id]
	if false == ok {
		return nil
	}

	return p
}

func checkUnAndPw(u, p string) error {
	if false == checkUsernameValid(u) {
		return &uErr{"invalid username"}
	}
	if false == checkPasswordValid(p) {
		return &uErr{"invalid password"}
	}
	return nil
}

func checkUsernameValid(u string) bool {
	if 6 > len(u) {
		return false
	}
	return true
}

func checkPasswordValid(p string) bool {
	if 6 > len(p) {
		return false
	}
	return true
}
