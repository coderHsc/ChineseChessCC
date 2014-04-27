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

var back int = 0

func netChessMove(w http.ResponseWriter, r *http.Request) {
	PrintRequest(r)
}

type BackData struct {
	ifirst int32
	iSecond int32
	iThird int32
}

func netChessMoveGet(w http.ResponseWriter, r *http.Request) {
	PrintRequest(r)

	stBack := BackData{1, int32(back), 1}
	fmt.Fprintf(w, "%v", stBack)
}

func netNewIdGet(w http.ResponseWriter, r *http.Request) {
	PrintRequest(r)

	back += 1
	fmt.Fprintf(w, "%d",  back)
}

func main() {
	http.HandleFunc("/pretestPrintRequest", pretestPrintRequest)
	http.HandleFunc("/chessMove", netChessMove)
	http.HandleFunc("/chessMoveGet", netChessMoveGet)
	http.HandleFunc("/newIdGet", netNewIdGet)
	err := http.ListenAndServe(":9090", nil)
	if err != nil {
		log.Fatal("ListenAndServe: ", err)
	}
}
