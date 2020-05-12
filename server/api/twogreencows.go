package main

import (
	"fmt"
	"io"
	"net/http"
)

func main() {
	fmt.Println("Starting 2 green cows server...")
	http.HandleFunc("/", root)
	http.HandleFunc("/session", sessions)
	http.HandleFunc("/greenhouses", sessions)
	http.HandleFunc("/devices", devices)
	http.ListenAndServe(":8000", nil)
}

func contentTypeForRequest(r *http.Request) string {
	var contentType = "html"
	contentType = r.Header.Get("Accept")

	switch contentType {
	case "application/json":
		contentType = "json"
	default:
		c := r.URL.Query().Get("format")
		switch c {
		case "html", "json", "xml":
			contentType = c
		default:
			contentType = "json"
		}
	}
	return contentType
}

// Will get a list of repositories with simple information
func sessions(w http.ResponseWriter, r *http.Request) {
	if r.URL.Path != "/session" {
		http.Error(w, "404 not found.", http.StatusNotFound)
		return
	}

	var httpResponse string = "<html><head></head><body><table>"
	httpResponse = httpResponse + "</body>"
	io.WriteString(w, httpResponse)
}

func devices(w http.ResponseWriter, r *http.Request) {

}

func root(w http.ResponseWriter, r *http.Request) {

	var httpResponse string = "<html><head></head><body><table>"
	httpResponse = httpResponse + "</body>"
	io.WriteString(w, httpResponse)
}
