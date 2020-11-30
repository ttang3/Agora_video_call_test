package main

import (
    "encoding/json"
    "os"
    "fmt"
    "io"
	"io/ioutil"
    "log"
    "net/http"
    "path/filepath"

    "github.com/gorilla/mux"
)

type KVEndPointOP interface {
	put(key []byte, value []byte) error 
	delete(key []byte) error
	get(key []byte, w http.ResponseWriter) 
	forEach(filename string, w http.ResponseWriter) 
	prefixGet(prefix []byte, w http.ResponseWriter) 
	containGet(containStr []byte, w http.ResponseWriter) 
	rangeGet(min, max []byte, w http.ResponseWriter) 
}

type Person struct {
    ID        string   `json:"id,omitempty"`
    Firstname string   `json:"firstname,omitempty"`
    Lastname  string   `json:"lastname,omitempty"`
    Address   *Address `json:"address,omitempty"`
}

type Address struct {
    City  string `json:"city,omitempty"`
    State string `json:"state,omitempty"`
}

var people []Person

func Search(w http.ResponseWriter, req *http.Request) {
    ex, err := os.Executable()
    if err != nil {
        panic(err)
    }
    exPath := filepath.Dir(ex)
	b,_ := ioutil.ReadFile(exPath+"/search.html")
	w.Write(b)
}

func Ping(w http.ResponseWriter, req *http.Request) {
    json.NewEncoder(w).Encode(&Person{"1", "Tom", "Tang", &Address{"RedWood", "CA"}})
}

func Insert(w http.ResponseWriter, req *http.Request) {
	io.WriteString(w, "I got your Insert request, but not implemented yet.\n")
}

func Query(w http.ResponseWriter, req *http.Request) {
	io.WriteString(w, "I got your Query request, Following is just a fake data.\n")
    json.NewEncoder(w).Encode(&Person{"1", "Tom", "Tang", &Address{"RedWood", "CA"}})
    json.NewEncoder(w).Encode(&Person{"2", "Tom", "Tang", &Address{"RedWood", "CA"}})
}

func Update(w http.ResponseWriter, req *http.Request) {
	io.WriteString(w, "I got your Update request, will be implemented soon.\n")
}

func Delete(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	fmt.Printf("Get.params = %v", params["key"])

	myBoltDB.delete([]byte(params["key"])) 
}

func Get(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	fmt.Printf("Get.params = %v", params["key"])

	myBoltDB.get([]byte(params["key"]), w) 
}

func Put(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	body, _ := ioutil.ReadAll(req.Body)
	myBoltDB.put([]byte(params["key"]), body) 
}

func ForEach(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	myBoltDB.forEach(params["filename"], w)
}

func PrefixGet(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	myBoltDB.prefixGet([]byte(params["prefixkey"]), w) 
}

func ContainGet(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	myBoltDB.containGet([]byte(params["containkey"]), w) 
}

func RangeGet(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	myBoltDB.rangeGet([]byte(params["min"]), []byte(params["max"]), w) 
}

func Backup(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
	err := myBoltDB.backup(params["filename"], w)
	if err != nil {
		http.Error(w, err.Error(), http.StatusInternalServerError)
	}
}

func setupResponseHeader(w http.ResponseWriter) {
	w.Header().Set("Access-Control-Allow-Origin", "*")
	w.Header().Set("Content-Type", "application/json")
}

func GetPersonEndpoint(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
    for _, item := range people {
        if item.ID == params["id"] {
            json.NewEncoder(w).Encode(item)
            return
        }
    }
    json.NewEncoder(w).Encode(&Person{})
}

func GetPeopleEndpoint(w http.ResponseWriter, req *http.Request) {
    json.NewEncoder(w).Encode(people)
}

func CreatePersonEndpoint(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
    var person Person
    _ = json.NewDecoder(req.Body).Decode(&person)
    person.ID = params["id"]
    people = append(people, person)
    json.NewEncoder(w).Encode(people)
}

func DeletePersonEndpoint(w http.ResponseWriter, req *http.Request) {
    params := mux.Vars(req)
    for index, item := range people {
        if item.ID == params["id"] {
            people = append(people[:index], people[index+1:]...)
            break
        }
    }
    json.NewEncoder(w).Encode(people)
}

func main() {
	if len(os.Args) < 2 {
                log.Fatalf("Usage: %s <port> <certificate> <private key>\n", os.Args[0])
                return
        }
                
    port := ":" + os.Args[1]
	var certfile,privatekeyfile string;
	secure := false
	if len(os.Args) == 4 {
		certfile = os.Args[2]
		privatekeyfile = os.Args[3]
		secure = true
	}

	if initkv() != nil {
		return	
	}

    router := mux.NewRouter()
    people = append(people, Person{ID: "1", Firstname: "Nic", Lastname: "Raboy", Address: &Address{City: "Dublin", State: "CA"}})
    people = append(people, Person{ID: "2", Firstname: "Maria", Lastname: "Raboy"})

    router.HandleFunc("/", Search).Methods("GET")

    router.HandleFunc("/test/v1/ping", Ping).Methods("GET")

    router.HandleFunc("/kv/v1/get/{key}", Get).Methods("GET")
    router.HandleFunc("/kv/v1/put/{key}", Put).Methods("POST")
    router.HandleFunc("/kv/v1/foreach/{filename}", ForEach).Methods("GET")
    router.HandleFunc("/kv/v1/prefixget/{prefixkey}", PrefixGet).Methods("GET")
    router.HandleFunc("/kv/v1/containget/{containkey}", ContainGet).Methods("GET")
    router.HandleFunc("/kv/v1/rangeget/{min}/{max}", RangeGet).Methods("GET")
    router.HandleFunc("/kv/v1/delete/{key}", Delete).Methods("DELETE")
    router.HandleFunc("/kv/v1/backup/{filename}", Backup).Methods("GET")

    if secure {
        if err := http.ListenAndServeTLS(port, certfile, privatekeyfile, router); err != nil {
            log.Fatal("ListenAndServeTLS: %v", err)
            os.Exit(1)
        }
    } else {
        if err := http.ListenAndServe(port, router); err != nil {
            log.Fatal("ListenAndServe: %v", err)
            os.Exit(1)
        }
    }

	//myOracleDB.deleteTable(0)
}

