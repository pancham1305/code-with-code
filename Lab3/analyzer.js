const fs = require('fs');
// tdrf where f is fibonaci , return count of token , identifiers and operators 
// Function to check if a character is an operator
function isOperator(c) {
    const operators = "+-*/%=<>&|^";
    return operators.includes(c);
}

// Function to count tokens, identifiers, and operators
function analyzeFile(filename) {
    let tokenCount = 0;
    let identifierCount = 0;
    let operatorCount = 0;

    // Read file contents
    const fileContent = fs.readFileSync(filename, 'utf8');

    // Split file contents into tokens
    const tokens = fileContent.split(" ");
    tokenCount = tokens.length;

    // Iterate through tokens and count identifiers and operators
    for (let token of tokens) {
//        tokenCount++;
        // Check if the token is an identifier
        if (/^[a-zA-Z_]\w*$/.test(token)) {
            identifierCount++;
        }
        // Check if the token is an operator
        if (token.length === 1 && isOperator(token)) {
            operatorCount++;
        }
    }

    // Output results
    console.log("Number of tokens:", tokenCount);
    console.log("Number of identifiers:", identifierCount);
    console.log("Number of operators:", operatorCount);
}

// Analyze the file
analyzeFile('./TDRF.c');

