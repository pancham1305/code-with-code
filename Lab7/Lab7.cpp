// create a symbol table and do lexical analysis 

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>

using namespace std;

// Token types
enum TokenType {
    IDENTIFIER,
    ASSIGNMENT,
    NUMBER,
    OPERATOR,
    UNKNOWN
};

// Symbol table
unordered_map<string, TokenType> symbolTable;

// Helper function to determine token type
TokenType getTokenType(const string& token) {
    if (isdigit(token[0])) {
        return NUMBER;
    } else if (isalpha(token[0])) {
        return IDENTIFIER;
    } else if (token == "=") {
        return ASSIGNMENT;
    } else if (token.length() == 1 && (token == "+" || token == "-" || token == "*" || token == "/" )){
        return OPERATOR;
    }
    return UNKNOWN;
}

// Lexical parser function
void lexicalParser(const string& inputFilePath, const string& outputFilePath) {
    ifstream inputFile(inputFilePath);
    ofstream outputFile(outputFilePath);

    if (!inputFile.is_open()) {
        cerr << "Error opening input file" << endl;
        return;
    }

    string token;
    while (inputFile >> token) {
        TokenType tokenType = getTokenType(token);
        symbolTable[token] = tokenType;
        outputFile << "{ type : " << tokenType << ", value: " << token << " }" << endl;
    }

    inputFile.close();
    outputFile.close();

    cout << "Symbol Table:" << endl;
    for (const auto& entry : symbolTable) {
        cout << entry.first << " : " << entry.second << endl;
    }

    // Print the enum values
    cout << "\nEnum Values:" << endl;
    cout << "IDENTIFIER: " << IDENTIFIER << endl;
    cout << "ASSIGNMENT: " << ASSIGNMENT << endl;
    cout << "NUMBER: " << NUMBER << endl;
    cout << "OPERATOR: " << OPERATOR << endl;
    cout << "UNKNOWN: " << UNKNOWN << endl;
}

int main() {
    const string inputFilePath = "input.txt";
    const string outputFilePath = "output.txt";

    lexicalParser(inputFilePath, outputFilePath);

    return 0;
}