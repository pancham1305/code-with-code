#include <stdio.h>
// declare fibonacci recursive function
int fibonacci(int n, int a, int b) {
    if (n == 0) 
        return a; // if n is 0, return a
    if (n == 1)
        return b; // if n is 1, return b
    return fibonacci(n - 1, b, a + b); // if n is greater than 1, call fibonacci recursively
}

int main() {
    int n = 10; // Change this to any desired value
    int result = fibonacci(n, 0, 1); // call fibonacci function
    printf("Fibonacci series value at position %d is %d\n", n, result); // print the result
    return 0;
}

