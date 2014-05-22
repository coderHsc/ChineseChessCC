package toolsql

import (
	"database/sql"
	"fmt"
	_ "github.com/Go-SQL-Driver/MySQL"
	"log"
	"time"
)

var g_db *sql.DB = nil

type SqlUserInfo struct {
	iUid          int
	name          string
	last_login    string
	general_score int64
	general_times int
}

var mCacheUserInfo map[int]*SqlUserInfo

func InitDatabase(dsn string) bool {
	mCacheUserInfo = make(map[int]*SqlUserInfo)

	db, err := sql.Open("mysql", dsn)
	if nil != err {
		log.Println("database error: open ->", err)

		return false
	}

	g_db = db

	return true
}

func DeInitDatabase() {
	if nil != g_db {
		g_db.Close()
	}
}

func GetUserId(strUser, passwd string) (int, string) {
	//检查数据库已初始化
	if nil == g_db {
		log.Panicln("database error: no init")
	}

	//按照用户名查询uid
	rows, err := g_db.Query("select uid from t_user where username=?", strUser)
	if nil != err {
		log.Panicln("database error: select ->", err)
	}

	var iUid1 int = 0
	for rows.Next() {
		var tmpUid int
		err = rows.Scan(&tmpUid)
		if nil != err {
			log.Panicln("database error: scan ->", err)
			break
		}

		iUid1 = tmpUid
	}

	//不存在则返回
	if 0 == iUid1 {
		return 0, "user no exist"
	}

	//按照加密后的口令查询uid
	var iUid2 int = 0
	err = g_db.QueryRow("select uid from t_user where username=? and password=password(?)", strUser, passwd).Scan(&iUid2)
	if sql.ErrNoRows == err {
		return 0, "error password"
	} else if nil != err {
		log.Panicln("database error: scan ->", err)
	}

	if iUid2 != iUid1 {
		log.Fatalln("username repetition")
		return 0, "error password"
	}

	return iUid1, "ok"
}

func CreateUser(strUser, strPasswd string) (int, string) {
	//检查数据库已初始化
	if nil == g_db {
		log.Panicln("database error: no init")
	}

	//按照用户名查询uid
	rows, err := g_db.Query("select uid from t_user where username=?", strUser)
	if nil != err {
		log.Panicln("database error: select ->", err)
	}

	var iUid1 int = 0
	for rows.Next() {
		var tmpUid int
		err = rows.Scan(&tmpUid)
		if nil != err {
			log.Panicln("database error: scan ->", err)
			break
		}

		iUid1 = tmpUid
	}

	//已存在则返回
	if 0 != iUid1 {
		return 0, "username exist"
	}

	//插入数据
	stmt, err := g_db.Prepare("insert t_user set username=?, password=password(?), created_date=?, last_login=?")
	if nil != err {
		log.Panicln("database error: insert ->", err)
		return 0, "database error"
	}

	tNow := time.Now()
	strDate := fmt.Sprintf("%.10s", tNow.String())
	strDateTime := fmt.Sprintf("%.19s", tNow.String())
	res, err := stmt.Exec(strUser, strPasswd, strDate, strDateTime)
	if nil != err {
		log.Panicln("database error: insert ->", err)
		return 0, "database error"
	}

	num, err := res.RowsAffected()
	if 1 != num {
		log.Panic("database insert failed")
		return 0, "database error"
	}

	id, err := res.LastInsertId()
	if nil != err {
		log.Panicln("database error: insert ->", err)
		return 0, "database error"
	}

	return int(id), "ok"
}

func RefreshLastLogin(iUid int) bool {
	//检查数据库已初始化
	if nil == g_db {
		log.Panicln("database error: no init")
	}

	//更新新的登录时间
	stmt, err := g_db.Prepare("update t_user set last_login=? where uid=?")
	if nil != err {
		log.Panicln("database error: update ->", err)
		return false
	}

	_, err = stmt.Exec(fmt.Sprintf("%.19s", time.Now().String()), iUid)
	if nil != err {
		log.Panicln("database error: update ->", err)
		return false
	}

	return true
}

func (pstUserInfo *SqlUserInfo) getUserInfo(iUid int) {
	//检查数据库已初始化
	if nil == g_db {
		log.Panicln("database error: no init")
		return
	}

	//按照uid查询用户信息
	rows := g_db.QueryRow("select username, last_login, general_score, general_times from t_user where uid=?", iUid)
	var tmpName string
	var tmpLastLogin string
	var tmpScore int64
	var tmpTimes int
	err := rows.Scan(&tmpName, &tmpLastLogin, &tmpScore, &tmpTimes)
	if nil != err {
		log.Panicln("database error: select row ->", err)
		return
	}

	pstUserInfo.iUid = iUid
	pstUserInfo.name = tmpName
	pstUserInfo.last_login = tmpLastLogin
	pstUserInfo.general_score = tmpScore
	pstUserInfo.general_times = tmpTimes

	return
}

func getUserInfo(iUid int) *SqlUserInfo {
	_, ok := mCacheUserInfo[iUid]
	if false == ok {
		pstUserInfo := new(SqlUserInfo)
		pstUserInfo.getUserInfo(iUid)
		mCacheUserInfo[iUid] = pstUserInfo
	}

	return mCacheUserInfo[iUid]
}

func GetUserLastLogin(iUid int) string {
	return getUserInfo(iUid).last_login
}

func GetUserGeneralScore(iUid int) int64 {
	return getUserInfo(iUid).general_score
}

func GetUserGeneralTimes(iUid int) int {
	return getUserInfo(iUid).general_times
}

func GetUserName(iUid int) string {
	return getUserInfo(iUid).name
}
