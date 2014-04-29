package main

import (
	"fmt"
	"log"
	"net/http"
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
	if false != ok {
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
		iLive = 0
		log.Println("userid ", iNetId, " opponent leave game")
	}

	if false == mUser[iNetId].bNewMove {
		log.Println("userid ", iNetId, " have no new moveinfo")
		fmt.Fprintf(w, "%d-0-0-0", iLive)
	} else {
		stMoveInfo := mUser[iNetId].slMoveRecord[len(mUser[iNetId].slMoveRecord) - 1]
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
	iUserNetId     int
	iOpponentNetId int
	bNewMove       bool
	slMoveRecord   []UserMoveRecord
}

func (pstData *UserData) init(iNetId int) {
	pstData.iUserNetId = iNetId
	pstData.iOpponentNetId = 0
	pstData.bNewMove = false

	var tmp []UserMoveRecord
	pstData.slMoveRecord = tmp
}

var mUser map[int]*UserData

func netNewIdGet(w http.ResponseWriter, r *http.Request) {
	log.Println("request:netNewIdGet")
	giNetIdIndex += 1
	fmt.Fprintf(w, "%d", giNetIdIndex)

	pstData := new(UserData)
	pstData.init(giNetIdIndex)
	mUser[giNetIdIndex] = pstData

	log.Println("create new id : ", giNetIdIndex)
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
	if 0 == mUser[iNetId].iOpponentNetId {
		for iUser, pstData := range mUser {
			if iUser != iNetId && 0 == pstData.iOpponentNetId {
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
		delete(mUser, iNetId)
		log.Printf("userid %d leave game\n", iNetId)
	}
}

func main() {
	mUser = make(map[int]*UserData)
	http.HandleFunc("/", pretestPrintRequest)
	http.HandleFunc("/chessMove", netChessMove)
	http.HandleFunc("/chessMoveGet", netChessMoveGet)
	http.HandleFunc("/newIdGet", netNewIdGet)
	http.HandleFunc("/findOpponent", netFindOpponent)
	http.HandleFunc("/leaveGame", netLeaveGame)
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
