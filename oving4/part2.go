package main 

import "fmt"
import "rand"
import "time"

func main () {
     ops, read := StartServer(0);
     for i := 0; i < 5; i++ {
     	 go Changer(ops,i,-5,5);
     }
     go Reader(read);

     never := make(chan int);
     <- never;
}

func Wait(sec int64){
     time.Sleep(1000000000 * sec);
}

func Changer(ops chan<- int, id int,min int, max int){
	for {
		Wait(10 + rand.Int63n(30));
		op := min + rand.Intn(max - min);
		fmt.Printf("Client%d: %d\n",id,op);
		ops <- op;
	}  
}

func Reader(read <-chan int) {
	value := <-read;
    for {
		select {
		case value = <-read:
		default:
		}
		fmt.Printf("Read: %d\n",value);
		Wait(1);
    }
}

func StartServer(start int) (chan<- int, <-chan int) {
     fmt.Printf("Start server: %d\n",start);
     ops := make(chan int);
     read := make(chan int);
     go Server(ops,read,0);
     return ops, read;
}

func Server(ops chan int, out chan int,start int){ 
    var item int = start;
    for {
		out <- item;
     	op := <- ops;
		item += op;
    }
}