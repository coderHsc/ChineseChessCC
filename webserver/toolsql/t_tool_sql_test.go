package toolsql

import (
	"fmt"
	"testing"
)

func TestToolSql(t *testing.T) {
	InitDatabase("root:123456@tcp(localhost:3306)/chinesechess_test_01?&charset=utf8")

	id, ret := GetUserId("aaa", "sss")
	fmt.Println("id ", id, " ret", ret)

	id, ret = CreateUser("aaa", "sss")
	fmt.Println("id ", id, " ret", ret)

	id, ret = GetUserId("aaa", "sss")
	fmt.Println("id ", id, " ret", ret)

	tmpid, ret := CreateUser("aaa", "sss")
	fmt.Println("id ", tmpid, " ret", ret)

	fmt.Println("refresh last login", RefreshLastLogin(id))

	fmt.Println("last login", GetUserLastLogin(id))

	fmt.Println("general score", GetUserGeneralScore(id))

	fmt.Println("genral times", GetUserGeneralTimes(id))

	DeInitDatabase()
}
