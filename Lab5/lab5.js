// read input and do lexical analysis
const fs = require("fs");

// Define token types
const TokenType = {
  OPERATOR: "Operator",
  KEYWORD: "Keyword",
  IDENTIFIER: "Identifier",
  TEXT_LITERAL: "TextLiteral",
  INTEGER_LITERAL: "IntegerLiteral",
  FLOAT_LITERAL: "FloatLiteral",
  COMMENT: "Comment",
  ERROR: "Error",
};

// Define keywords
const keywords = new Set([
  "if",
  "else",
  "while",
  "for",
  "function",
  "return",
  "var",
  "let",
  "const",
]);

// Define operators
const operators = new Set([
  "+",
  "-",
  "*",
  "/",
  "=",
  "==",
  "===",
  "!=",
  "!==",
  ">",
  "<",
  ">=",
  "<=",
  "&&",
  "||",
  "!",
  "++",
  "--",
]);

// Function to tokenize input string
function tokenize(input) {
  const tokens = [];
  const lines = input.split("\n");

  for (let i = 0; i < lines.length; i++) {
    let line = lines[i];
    let position = 0;

    while (position < line.length) {
      let char = line[position];

      // Skip whitespace
      if (char.match(/\s/)) {
        position++;
        continue;
      }

      // Comments
      if (char === "/" && line[position + 1] === "/") {
        tokens.push({
          type: TokenType.COMMENT,
          value: line.slice(position),
          line: i + 1,
        });
        break;
      }

      // Operators
      if (operators.has(char)) {
        tokens.push({ type: TokenType.OPERATOR, value: char, line: i + 1 });
        position++;
        continue;
      }

      // Identifiers or Keywords
      if (char.match(/[a-zA-Z_]/)) {
        let identifier = "";
        while (char && (char.match(/[a-zA-Z0-9_]/) || char === ".")) {
          identifier += char;
          position++;
          char = line[position];
        }
        if (keywords.has(identifier)) {
          tokens.push({
            type: TokenType.KEYWORD,
            value: identifier,
            line: i + 1,
          });
        } else {
          tokens.push({
            type: TokenType.IDENTIFIER,
            value: identifier,
            line: i + 1,
          });
        }
        continue;
      }

      // Text Literals
      if (char === '"' || char === "'") {
        let endQuoteIndex = line.indexOf(char, position + 1);
        if (endQuoteIndex === -1) {
          tokens.push({
            type: TokenType.ERROR,
            value: "Unmatched quote",
            line: i + 1,
          });
          break;
        }
        tokens.push({
          type: TokenType.TEXT_LITERAL,
          value: line.slice(position, endQuoteIndex + 1),
          line: i + 1,
        });
        position = endQuoteIndex + 1;
        continue;
      }

      // Numbers
      if (char.match(/[0-9]/)) {
        let num = "";
        while (char && char.match(/[0-9\.]/)) {
          num += char;
          position++;
          char = line[position];
        }
        if (num.includes(".")) {
          tokens.push({
            type: TokenType.FLOAT_LITERAL,
            value: parseFloat(num),
            line: i + 1,
          });
        } else {
          tokens.push({
            type: TokenType.INTEGER_LITERAL,
            value: parseInt(num),
            line: i + 1,
          });
        }
        continue;
      }

      // Unrecognized character
      tokens.push({
        type: TokenType.ERROR,
        value: "Unrecognized character",
        line: i + 1,
      });
      position++;
    }
  }

  return tokens;
}

// Test input strings
const testStrings = [
  "var x = 5;",
  'if (x === 5) { return "Hello"; } // This is a comment',
  "function add(a, b) { return a + b; }",
  "1.23 + 4 * 5",
  '"This is a text literal"',
  "invalid !@# characters",
];

// Test the lexical analyzer
testStrings.forEach((str, index) => {
  console.log(`\nInput String ${index + 1}:`);
  console.log(str);
  const tokens = tokenize(str);
  console.log("Tokens:");
  console.log(tokens);
});

// reading txt for lexical analyzer
const textFile = "./hello.txt";
const content = fs.readFileSync(textFile, "utf8");

const tokens = tokenize(content);
console.log("File Tokens:");
console.log(tokens);

/*
───────┬───────────────────────────────────────────────────────────────────────────────────────────────
       │ File: hello.txt
───────┼───────────────────────────────────────────────────────────────────────────────────────────────
   1   │ let x = 1;
   2   │ x = 2;
   3   │ data = 2;
───────┴───────────────────────────────────────────────────────────────────────────────────────────────
*/

/*
 Output:

Input String 1:
var x = 5;
Tokens:
[
  { type: 'Keyword', value: 'var', line: 1 },
  { type: 'Identifier', value: 'x', line: 1 },
  { type: 'Operator', value: '=', line: 1 },
  { type: 'IntegerLiteral', value: 5, line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 }
]

Input String 2:
if (x === 5) { return "Hello"; } // This is a comment
Tokens:
[
  { type: 'Keyword', value: 'if', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Identifier', value: 'x', line: 1 },
  { type: 'Operator', value: '=', line: 1 },
  { type: 'Operator', value: '=', line: 1 },
  { type: 'Operator', value: '=', line: 1 },
  { type: 'IntegerLiteral', value: 5, line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Keyword', value: 'return', line: 1 },
  { type: 'TextLiteral', value: '"Hello"', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Comment', value: '// This is a comment', line: 1 }
]

Input String 3:
function add(a, b) { return a + b; }
Tokens:
[
  { type: 'Keyword', value: 'function', line: 1 },
  { type: 'Identifier', value: 'add', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Identifier', value: 'a', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Identifier', value: 'b', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Keyword', value: 'return', line: 1 },
  { type: 'Identifier', value: 'a', line: 1 },
  { type: 'Operator', value: '+', line: 1 },
  { type: 'Identifier', value: 'b', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 }
]

Input String 4:
1.23 + 4 * 5
Tokens:
[
  { type: 'FloatLiteral', value: 1.23, line: 1 },
  { type: 'Operator', value: '+', line: 1 },
  { type: 'IntegerLiteral', value: 4, line: 1 },
  { type: 'Operator', value: '*', line: 1 },
  { type: 'IntegerLiteral', value: 5, line: 1 }
]

Input String 5:
"This is a text literal"
Tokens:
[ { type: 'TextLiteral', value: '"This is a text literal"', line: 1 } ]

Input String 6:
invalid !@# characters
Tokens:
[
  { type: 'Identifier', value: 'invalid', line: 1 },
  { type: 'Operator', value: '!', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Identifier', value: 'characters', line: 1 }
]
File Tokens:
[
  { type: 'Keyword', value: 'let', line: 1 },
  { type: 'Identifier', value: 'x', line: 1 },
  { type: 'Operator', value: '=', line: 1 },
  { type: 'IntegerLiteral', value: 1, line: 1 },
  { type: 'Error', value: 'Unrecognized character', line: 1 },
  { type: 'Identifier', value: 'x', line: 2 },
  { type: 'Operator', value: '=', line: 2 },
  { type: 'IntegerLiteral', value: 2, line: 2 },
  { type: 'Error', value: 'Unrecognized character', line: 2 },
  { type: 'Identifier', value: 'data', line: 3 },
  { type: 'Operator', value: '=', line: 3 },
  { type: 'IntegerLiteral', value: 2, line: 3 },
  { type: 'Error', value: 'Unrecognized character', line: 3 }
]
 */
