package main

import (
	"fmt"
	"game"
	"log"
	"net/http"
	"strconv"
	"toolsql"
	"user"
)

type sErr struct {
	s string
}

func (e *sErr) Error() string {
	return e.s
}

func dataInit() {
	toolsql.InitDatabase("root:123456@tcp(localhost:3306)/chinesechess_test_01?&charset=utf8")
}

func dataDeInit() {
	toolsql.DeInitDatabase()
}

func routeInit() {
	http.HandleFunc("/", hUndefinedRequest)
	http.HandleFunc("/regUser", hUserRegister)
	http.HandleFunc("/login", hUserLogin)
	http.HandleFunc("/logoff", hUserLogoff)
	http.HandleFunc("/findGame", hFindGame)
	http.HandleFunc("/chessMove", hChessMove)
	http.HandleFunc("/getOppoMove", hGetOppoMove)
	http.HandleFunc("/leaveGame", hLeaveGame)
}

func routeDeInit() {
}

func webInit() {
	dataInit()
	routeInit()
}

func webDeInit() {
	dataDeInit()
	routeDeInit()
}

func startServer() {
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}

func hUndefinedRequest(w http.ResponseWriter, r *http.Request) {
	printRequest(r)
}

func hUserRegister(w http.ResponseWriter, r *http.Request) {
	log.Println("request: user register")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	u, p := parseUsernameAndPasswd(r)

	id, ret := user.RegisterUser(u, p)
	fmt.Fprintf(w, "uid=%d reason %s", id, ret)
}

func hUserLogin(w http.ResponseWriter, r *http.Request) {
	log.Println("request: user login")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	u, p := parseUsernameAndPasswd(r)

	id, ret := user.LoginUser(u, p)
	fmt.Fprintf(w, "uid=%d reason %s", id, ret)
}

func hUserLogoff(w http.ResponseWriter, r *http.Request) {
	log.Println("request: user logoff")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	id := parseUid(r)

	if 0 != id {
		user.LogoffUser(id)
	}
}

func hFindGame(w http.ResponseWriter, r *http.Request) {
	log.Println("request: find game")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	ret := game.FindGame(parseUid(r))
	fmt.Fprintln(w, ret)
}

func hChessMove(w http.ResponseWriter, r *http.Request) {
	log.Println("request:chess move")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	ret := game.ChessMove(parseUid(r), parseChessMove(r))
	fmt.Fprintln(w, ret)
}

func hGetOppoMove(w http.ResponseWriter, r *http.Request) {
	log.Println("request:get opponent move")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	ret := game.GetOppoMove(parseUid(r))
	fmt.Fprintln(w, ret)
}

func hLeaveGame(w http.ResponseWriter, r *http.Request) {
	log.Println("request:leave game")

	err := checkRequest(r)
	if nil != err {
		log.Println("request:", err)
		return
	}

	game.LeaveGame(parseUid(r))
}

func checkRequest(r *http.Request) error {
	if "POST" != r.Method {
		return &sErr{"invalid method"}
	}

	r.ParseForm()

	return nil
}

func parseUsernameAndPasswd(r *http.Request) (u, p string) {
	u = r.Form.Get("user")
	p = r.Form.Get("passwd")
	log.Printf("parse-> username:%s(%d), passwd:%s(%d)\n", u, len(u), p, len(p))

	return
}

func parseUid(r *http.Request) int {
	id, _ := strconv.Atoi(r.Form.Get("uid"))
	log.Printf("parse-> uid: %d", id)

	user.LogCheck(id)

	return id
}

func parseChessMove(r *http.Request) game.MoveRecord {
	c, _ := strconv.Atoi(r.Form.Get("chess"))
	x, _ := strconv.Atoi(r.Form.Get("posX"))
	y, _ := strconv.Atoi(r.Form.Get("posY"))
	log.Printf("parse-> chess: %d, posx: %d, posy: %d", c, x, y)

	return game.MoveRecord{c, x, y}
}

func printRequest(r *http.Request) {
	fmt.Println("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<")
	r.ParseForm()
	fmt.Println("Method: ", r.Method)
	fmt.Println("URL: ", r.URL)
	fmt.Println("    Scheme: ", r.URL.Scheme)
	fmt.Println("    Opaque: ", r.URL.Opaque)
	fmt.Println("    User: ", r.URL.User)
	fmt.Println("    Host: ", r.URL.Host)
	fmt.Println("    Path: ", r.URL.Path)
	fmt.Println("    RawQuery: ", r.URL.RawQuery)
	fmt.Println("    Fragment: ", r.URL.Fragment)
	fmt.Println("Proto: ", r.Proto)
	fmt.Println("ProtoMajor: ", r.ProtoMajor)
	fmt.Println("ProtoMinor: ", r.ProtoMinor)
	fmt.Println("Header: ", r.Header)
	//Body like a interface
	//fmt.Println("Body: ", r.Body)
	fmt.Println("ContentLength: ", r.ContentLength)
	fmt.Println("TransferEncoding: ", r.TransferEncoding)
	fmt.Println("Close: ", r.Close)
	fmt.Println("Host: ", r.Host)
	fmt.Println("Form: ", r.Form)
	fmt.Println("PostForm: ", r.PostForm)
	r.ParseMultipartForm(10000)
	fmt.Println("MultipartForm: ", r.MultipartForm)
	fmt.Println("Trailer: ", r.Trailer)
	fmt.Println("RemoteAddr: ", r.RemoteAddr)
	fmt.Println("RequestURI: ", r.RequestURI)
	//TLS

	fmt.Println(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n")
}
