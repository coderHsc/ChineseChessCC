package main

import (
	"log"
)

func main() {
	//config init

	log.Println("server start")

	webInit()

	startServer()

	webDeInit()
}
