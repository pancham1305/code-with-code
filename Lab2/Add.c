#include <stdio.h>

int main() {
  // declare 3 variables
  int num1, num2, sum;
  
  printf("Enter two numbers: "); // say user to add input
  scanf("%d %d", &num1, &num2); // read input from user

  sum = num1 + num2; // add two numbers
  printf("Sum: %d\n", sum); // print the sum

  return 0;
}
