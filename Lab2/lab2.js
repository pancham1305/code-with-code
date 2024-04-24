/**
 * * Q Write a c program to add two real numbers with three variables including some comment lines in the program. Save the program as Add.c. Now read the program Add.c and display the count of the number of variables as a valid identifier. Count the number of comment lines in the code. and the number of statements in the program Add.c
 **/

/*
───────┬───────────────────────────────────────────────────────────────────────────────────────────────
       │ File: Add.c
───────┼───────────────────────────────────────────────────────────────────────────────────────────────
   1   │ #include <stdio.h>
   2   │ 
   3   │ int main() {
   4   │   // declare 3 variables
   5   │   int num1, num2, sum;
   6   │   
   7   │   printf("Enter two numbers: "); // say user to add input
   8   │   scanf("%d %d", &num1, &num2); // read input from user
   9   │ 
  10   │   sum = num1 + num2; // add two numbers
  11   │   printf("Sum: %d\n", sum); // print the sum
  12   │ 
  13   │   return 0;
  14   │ }
───────┴───────────────────────────────────────────────────────────────────────────────────────────────
*/

const filePath = "./Add.c";
const fs = require("fs");

// read the file
const data = fs.readFileSync(filePath).toString();


// function to get the number of variables as a valid identifier
const getIdentifiers = (code) => {
  // regex to match the variables before int
  let regex = /(\bint\s+(?:\w+\s*,?\s*)+\b);/g;
  let match;
  const variablesBeforeInt = [];
  // loop through the matches and push the variables to the array
  while ((match = regex.exec(code)) !== null) {
    // console.log(match )
    // if there are multiple variables, split them and push to the array
    if (match[1].split(",").length > 1) {
      variablesBeforeInt.push(...match[1].split(","));
    } else variablesBeforeInt.push(match[1]);
  }
  // return the length of the array
  return variablesBeforeInt.length;
};

// function to get the number of comment lines
const getCommentLines = (code) => {
  // regex to match the comments
  const commentRegex = /\/\/\s*\w+/g;
  // match the comments and return the length of the array
  const comments = code.match(commentRegex);
  // return the length of the array
  return comments?.length ?? 0;
};

// function to get the number of statements
const getStatements = (code) => {
  // split the code by the semicolon
  const lines = code.split(";");
  // return the length of the array
  return lines?.length - 1;
};
// get the number of variables, comments and statements
const variableAsIdentifierCount = getIdentifiers(data);
const commentCount = getCommentLines(data);
const statementCount = getStatements(data);

// log the results
console.log(`
Number of Variables:  ${variableAsIdentifierCount}
Number of Comments:  ${commentCount}
Number of Statements: ${statementCount}
`);
