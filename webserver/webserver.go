package main

import (
	"fmt"
	"log"
	"net/http"
	"toolsql"
)

func PrintRequest(r *http.Request) {
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

	fmt.Println("**********************************************************\n\n\n")
}

func pretestPrintRequest(w http.ResponseWriter, r *http.Request) {
	PrintRequest(r)
}

var giNetIdIndex int = 100

func netChessMove(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	var sNetId string = r.Form["uiNetId"][0]

	var iNetId int = 0
	fmt.Sscanf(sNetId, "%d", &iNetId)

	log.Println("request:netChessMove userid:", iNetId)

	_, ok := mUser[iNetId]
	if false == ok {
		log.Println("userid ", iNetId, " is not exist")
		return
	}

	sMoveInfo := r.Form["moveinfo"][0]
	var iChessId, iPosX, iPosY int32 = 0, 0, 0
	fmt.Sscanf(sMoveInfo, "%d-%d-%d", &iChessId, &iPosX, &iPosY)

	mUser[iNetId].slMoveRecord = append(mUser[iNetId].slMoveRecord, UserMoveRecord{iChessId, iPosX, iPosY})
	iOpponent := mUser[iNetId].iOpponentNetId
	_, ok = mUser[iOpponent]
	if false != ok && true == mUser[iOpponent].bAlive && mUser[iOpponent].iOpponentNetId == iNetId {
		mUser[iOpponent].slMoveRecord = append(mUser[iOpponent].slMoveRecord, UserMoveRecord{iChessId, iPosX, iPosY})
		mUser[iOpponent].bNewMove = true
	}

	log.Printf("user %d move chess %d to (%d, %d)\n", iNetId, iChessId, iPosX, iPosY)
}

func netChessMoveGet(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	var sNetId string = r.Form["uiNetId"][0]

	var iNetId int = 0
	fmt.Sscanf(sNetId, "%d", &iNetId)

	log.Println("request:netChessMoveGet userid:", iNetId)
	_, ok := mUser[iNetId]
	if false == ok {
		log.Println("userid ", iNetId, " is not exist")
		return
	}

	iOpponent := mUser[iNetId].iOpponentNetId
	if 0 == iOpponent {
		log.Println("userid ", iNetId, " have no opponent")
		return
	}

	iLive := 1
	_, ok = mUser[iOpponent]
	if false == ok {
		log.Println("error for manage userdata")
		return
	}
	if false == mUser[iOpponent].bAlive || mUser[iOpponent].iOpponentNetId != iNetId {
		iLive = 0
		log.Println("userid ", iNetId, " opponent leave game")
	}

	if false == mUser[iNetId].bNewMove {
		log.Println("userid ", iNetId, " have no new moveinfo")
		fmt.Fprintf(w, "%d-0-0-0", iLive)
	} else {
		stMoveInfo := mUser[iNetId].slMoveRecord[len(mUser[iNetId].slMoveRecord)-1]
		fmt.Fprintf(w, "%d-%d-%d-%d", iLive, stMoveInfo.iChessId, stMoveInfo.iPosX, stMoveInfo.iPosY)
		mUser[iNetId].bNewMove = false
		log.Println("userid ", iNetId, " is not get opponent move ", stMoveInfo)
	}
}

type UserMoveRecord struct {
	iChessId int32
	iPosX    int32
	iPosY    int32
}

type UserData struct {
	iUserNetId  int
	strUserName string
	strPasswd   string

	bAlive bool

	iOpponentNetId int
	bNewMove       bool
	slMoveRecord   []UserMoveRecord
}

func (pstData *UserData) init(iNetId int, strUser string, strPwd string) {
	pstData.iUserNetId = iNetId
	pstData.iOpponentNetId = 0
	pstData.bNewMove = false
	pstData.strUserName = strUser
	pstData.strPasswd = strPwd
	pstData.bAlive = false

	var tmp []UserMoveRecord
	pstData.slMoveRecord = tmp
}

var mUser map[int]*UserData

func netCreateNewUser(w http.ResponseWriter, r *http.Request) {
	log.Println("request:createNewUser")

	//检查请求
	if "POST" != r.Method {
		return
	}

	//解析请求中的用户名和密口令
	r.ParseForm()
	_, ok := r.Form["user"]
	if false == ok {
		return
	}
	_, ok = r.Form["passwd"]
	if false == ok {
		return
	}
	strUser := r.Form["user"][0]
	strPwd := r.Form["passwd"][0]
	log.Println("register user", strUser, len(strUser), "passwd", strPwd, len(strPwd), ".")

	//检查用户名和口令的合法性
	if 3 > len(strUser) {
		fmt.Fprintf(w, "ret=failed uid=0 reason %s", "invalid username")
		return
	}
	if 3 > len(strPwd) {
		fmt.Fprintf(w, "ret=failed uid=0 reason %s", "invalid password")
		return
	}

	//尝试向数据库注册用户
	iUid, strRet := toolsql.CreateUser(strUser, strPwd)
	if 0 == iUid {
		fmt.Fprintf(w, "ret=failed uid=0 reason %s", strRet)
		return
	}

	fmt.Fprintf(w, "ret=success uid=%d reason", iUid)
	log.Println("create new user", iUid)
}

func netUserLogin(w http.ResponseWriter, r *http.Request) {
	log.Println("request:netUserLogin")

	if "POST" != r.Method {
		return
	}

	r.ParseForm()
	_, ok := r.Form["user"]
	if false == ok {
		return
	}
	_, ok = r.Form["passwd"]
	if false == ok {
		return
	}
	strUser := r.Form["user"][0]
	strPwd := r.Form["passwd"][0]
	log.Println("login user ", strUser, len(strUser), "passwd", strPwd, len(strPwd), ".")

	iUid, strRet := toolsql.GetUserId(strUser, strPwd)
	if 0 == iUid {
		fmt.Fprintf(w, "ret=failed uid=0 reason %s", strRet)
		return
	}

	_, ok = mUser[iUid]
	if false == ok {
		pstData := new(UserData)
		pstData.init(iUid, strUser, strPwd)
		mUser[iUid] = pstData

		log.Println("create new userdata")
	}

	fmt.Fprintf(w, "ret=success uid=%d reason", iUid)
	log.Println("user", iUid, "login")
}

func netFindOpponent(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	var ss string = r.Form["uiNetId"][0]
	var iNetId int = 0
	fmt.Sscanf(ss, "%d", &iNetId)
	log.Println("request:netFindOpponent userid:", iNetId)

	var iOpponentId int = 1
	var iUserColor int = 0
	_, ok := mUser[iNetId]
	if false == ok {
		log.Println("userid ", iNetId, " is not exist")
		return
	}

	mUser[iUid].bAlive = true

	if 0 == mUser[iNetId].iOpponentNetId {
		for iUser, pstData := range mUser {
			if iUser != iNetId && true == pstData.bAlive && 0 == pstData.iOpponentNetId {
				iOpponentId = iUser
				iUserColor = 1
				mUser[iNetId].iOpponentNetId = iUser
				mUser[iUser].iOpponentNetId = iNetId

				log.Println("userid ", iNetId, " choose a opponent ", iUser, "local color ", iUserColor)
				break
			}
		}
	} else {
		iOpponentId = mUser[iNetId].iOpponentNetId
		iUserColor = 2
		log.Println("userid ", iNetId, " have a opponent ", iOpponentId, "local color ", iUserColor)
	}

	fmt.Fprintf(w, "opponent=%dcolor=%d", iOpponentId, iUserColor)
}

func netLeaveGame(w http.ResponseWriter, r *http.Request) {
	r.ParseForm()
	var ss string = r.Form["uiNetId"][0]

	var iNetId int = 0
	fmt.Sscanf(ss, "%d", &iNetId)
	log.Println("request:netLeaveGame userid:", iNetId)

	_, ok := mUser[iNetId]
	if true == ok {
		mUser[iNetId].bAlive = false
		mUser[iNetId].bNewMove = false
		mUser[iNetId].iOpponentNetId = 0
		log.Printf("userid %d leave game\n", iNetId)
	}
}

func main() {
	webInit()

	http.HandleFunc("/", pretestPrintRequest)
	http.HandleFunc("/regUser", netCreateNewUser)
	http.HandleFunc("/login", netUserLogin)
	http.HandleFunc("/chessMove", netChessMove)
	http.HandleFunc("/chessMoveGet", netChessMoveGet)
	http.HandleFunc("/findOpponent", netFindOpponent)
	http.HandleFunc("/leaveGame", netLeaveGame)
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}

	webDeInit()
}

func webInit() {
	toolsql.InitDatabase("root:123456@tcp(localhost:3306)/chinesechess_test_01?&charset=utf8")

	mUser = make(map[int]*UserData)
}

func webDeInit() {
	toolsql.DeInitDatabase()
}
