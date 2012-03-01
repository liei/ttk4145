
package main 

import "fmt"

type pair_t struct {
       a, b int;
};

const N int = 5;

func main() {
    var pi float64 = 3.1415;
    var p pair_t = pair_t{1,2};
    for i := 0; i < N; i++ {
    	fmt.Println("Hello World",foo(p),pi,i);	
    }
    var a [N]float64 = [N]float64{2.0,4.0,0.56,9.4,10.12};
    for i, f := range a {
    	fmt.Printf("a[%d] = %f\n",i,f);
    }
    n := 6;
    for n > 0 {
    	fmt.Printf("n = %d\n",n);
	n--;
    }
/*    for i := 0; i < 1000000; i++ {
    	go countdown(i);   
    }
*/
    channel := make(chan int);  

    go gen(channel,10);
    go double(channel);
    
    never := make(chan int);
    <- never;

}



func countdown(n int) int{
     fmt.Printf("Counting from %d.\n",n);
     sum := 0;
     for i := 0; i < n; i++ {
     	 sum += i;	 
     }  
     fmt.Printf("Done counting from %d\n",n);
     return sum;
}

func foo(p pair_t) int {
     return p.a + p.b;
}

func gen(output chan int, n int){
     for i := 0; i < n; i++ {
     	 fmt.Printf("generate: %d\n",i);
     	 output <- i;
     }
     output <- -1;
}

func double(input chan int){
     i := <- input;
     for i >= 0 {
     	 fmt.Printf("double: %d\n",i*2);
	 i = <- input;
     }
}