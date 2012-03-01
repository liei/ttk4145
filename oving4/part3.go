package main 

import "fmt"
import "rand"
import "time"

const N int = 5;

func main () {


	first := make(chan bool);
	left := make(chan bool);
	go philosopher(0,first,left);
	for i := 1; i < N; i++ {
		right := make(chan bool);
		go fork(left,right);
		left = right;
		right = make(chan bool);
		go philosopher(i,left,right);
		left = right;
	}
	go fork(left,first);
	


	never := make(chan bool);
	<- never;
}

func wait(sec int64){
     time.Sleep(1000000000 * sec);
}

func philosopher(id int, left chan<- bool, right chan<- bool) {
	eaten := 0;
	var gotleft, gotright bool;
	for {		
		for !gotleft || !gotright {
			time.Sleep(1000 * (1 + rand.Int63n(5)));
			gotleft = left <- true;
			if gotleft {
		//		fmt.Printf("p%d picked up left\n",id);
			} else {
		//		fmt.Printf("p%d could not pick up left\n",id);
				continue;
			}
			
			gotright = right <- true;
			if gotright {
		//		fmt.Printf("p%d picked up right\n",id);
			} else {
		//		fmt.Printf("p%d could not pick up right, dropped left\n",id); 
				left <- true;
			}
		}
		eaten++;
		fmt.Printf("p%d eating [%d]\n",id,eaten);
		time.Sleep(1000 * (5 + rand.Int63n(20)));
		gotleft = false;
		left <- true;
		//fmt.Printf("p%d put down left\n",id);
		right <- true;
		gotright = false;
		//fmt.Printf("p%d put down right\n",id);
		//fmt.Printf("p%d thinking\n",id);
		time.Sleep(1000 * (5 + rand.Int63n(20)));
	}
}

func fork(left, right <- chan bool){
	for {
		select {
		case <-left: <-left;
		case <-right: <-right;
		}
	}	
}