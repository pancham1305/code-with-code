// Generate three address code
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <cctype>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
using namespace std;

// Token types
enum TokenType
{
	IDENTIFIER,
	ASSIGNMENT,
	NUMBER,
	OPERATOR,
	NEWLINE,
	UNKNOWN
};

// Node types for AST
enum NodeType
{
	ASSIGNMENT_NODE,
	EXPRESSION_NODE,
	IDENTIFIER_NODE,
	NUMBER_NODE,
	OPERATOR_NODE
};

// Symbol table
unordered_map<string, pair<TokenType, bool>> symbolTable;

// Helper function to determine token type
TokenType getTokenType(const string &token)
{
	if (isdigit(token[0]))
	{
		return NUMBER;
	}
	else if (isalpha(token[0]))
	{
		return IDENTIFIER;
	}
	else if (token == "=")
	{
		return ASSIGNMENT;
	}
	else if (token.length() == 1 && (token == "+" || token == "-" || token == "*" || token == "/"))
	{
		return OPERATOR;
	}
	return UNKNOWN;
}

// AST node structure
struct ASTNode
{
	NodeType type;
	string value;
	vector<ASTNode *> children;
};

// Lexical parser function
vector<pair<TokenType, string>> lexicalParser(const string &inputFilePath)
{
	ifstream inputFile(inputFilePath);
	vector<pair<TokenType, string>> tokens;

	if (!inputFile.is_open())
	{
		cout << "Error opening input file" << endl;
		return tokens;
	}

	string token;
	// add new line into account
	string tokenss;
	while (getline(inputFile, tokenss))
	{
		// split the string into tokens
		string token;
		cout << "tokenss: " << tokenss << endl;
		for (char c : tokenss)
		{
			if (isspace(c))
			{
				if (!token.empty())
				{
					cout << "tt: " << token << "\n";
					TokenType tokenType = getTokenType(token);
					if (tokenType == IDENTIFIER)
						symbolTable[token] = {tokenType, false};
					tokens.emplace_back(tokenType, token);
					token.clear();
				}
			}
			else
			{
				token += c;
			}
		}
		cout << "token: " << token << endl;
		if (!token.empty())
		{
			TokenType tokenType = getTokenType(token);
			if (tokenType == IDENTIFIER)
				symbolTable[token] = {tokenType, false};
			tokens.emplace_back(tokenType, token);
		}
		tokens.emplace_back(NEWLINE, "\n");
	}

	inputFile.close();

	cout << "Symbol Table:" << endl;

	ofstream outputfile;
	outputfile.open("output.txt");

	ofstream symbolTablefile;
	symbolTablefile.open("symbolTable.txt");

	// first write enum TokenType to file

	outputfile << "IDENTIFIER: " << IDENTIFIER << endl;
	outputfile << "ASSIGNMENT: " << ASSIGNMENT << endl;
	outputfile << "NUMBER: " << NUMBER << endl;
	outputfile << "OPERATOR: " << OPERATOR << endl;
	outputfile << "NEWLINE: " << NEWLINE << endl;
	outputfile << "UNKNOWN: " << UNKNOWN << endl
			   << endl;

	symbolTablefile << "IDENTIFIER: " << IDENTIFIER << endl;
	symbolTablefile << "ASSIGNMENT: " << ASSIGNMENT << endl;
	symbolTablefile << "NUMBER: " << NUMBER << endl;
	symbolTablefile << "OPERATOR: " << OPERATOR << endl;
	symbolTablefile << "NEWLINE: " << NEWLINE << endl;
	symbolTablefile << "UNKNOWN: " << UNKNOWN << endl
					<< endl;

	// write the symbol table to file

	for (const auto &entry : symbolTable)
	{
		cout << entry.first << " : " << entry.second.first << endl;
		symbolTablefile << "{ " << entry.first << " : " << entry.second.first << " }" << endl;
	}

	for (const auto &entry : tokens)
	{
		outputfile << "{ type : " << entry.first << ", value: " << entry.second << " }" << endl;
		cout << "token: " << entry.first << " : " << entry.second << endl;
	}

	return tokens;
}
// Syntax parser function
vector<ASTNode> syntaxParser(const vector<pair<TokenType, string>> &tokens)
{
	vector<ASTNode> nodes;
	int i = 0;

	while (i < tokens.size())
	{
		cout << "token: value " << tokens[i].first << " : " << tokens[i].second << endl;
		if (tokens[i].first == IDENTIFIER)
		{
			ASTNode *assignmentNode = new ASTNode{ASSIGNMENT_NODE, ""};
			assignmentNode->children.push_back(new ASTNode{IDENTIFIER_NODE, tokens[i].second});
			symbolTable[tokens[i].second].second = true;
			i++;

			if (tokens[i].first != ASSIGNMENT)
			{
				cout << "Error: Expected '=' after identifier" << endl;
				return {};
			}
			i++;

			ASTNode *expressionNode = new ASTNode{EXPRESSION_NODE, ""};
			assignmentNode->children.push_back(expressionNode);

			while (i < tokens.size())
			{
				cout << "token: " << tokens[i].first << " : " << tokens[i].second << endl;
				if (tokens[i].first == NEWLINE)
				{
					i++;
					break;
				}
				if (tokens[i].first == NUMBER)
				{
					expressionNode->children.push_back(new ASTNode{NUMBER_NODE, tokens[i].second});
					i++;
				}
				else if (tokens[i].first == IDENTIFIER)
				{
					expressionNode->children.push_back(new ASTNode{IDENTIFIER_NODE, tokens[i].second});
					i++;
				}
				else if (tokens[i].first == OPERATOR)
				{
					expressionNode->children.push_back(new ASTNode{OPERATOR_NODE, tokens[i].second});
					i++;
				}
				else
				{
					break; // Exit the expression loop on encountering an unknown token
				}
			}

			nodes.push_back(*assignmentNode);
		}
		else
		{
			cout << "Error: Expected identifier at the start of the line" << endl;
			return {};
		}
	}

	return nodes;
}

void saveSymbolTable()
{
	// save it to file
	ofstream symbolTablefile;
	symbolTablefile.open("symbolTable.txt");

	// write the symbol table to file

	for (const auto &entry : symbolTable)
	{
		cout << entry.first << " : " << entry.second.first << " : " << entry.second.second << endl;
		symbolTablefile << "{ " << entry.first << " : " << entry.second.first << " : " << entry.second.second << " }" << endl;
	}
}

void printASCITree(const ASTNode node, int indent = 0)
{

	cout << string(indent, ' ');
	cout << "(" << node.type << ": " << node.value << ")";

	if (!node.children.empty())
	{
		cout << endl;
		for (ASTNode *child : node.children)
		{
			printASCITree(*child, indent + 2);
		}
	}
	else
	{
		cout << " (leaf node)";
	}
	cout << endl;
}

void printASCIITree(const vector<ASTNode> nodes)
{
	for (auto &node : nodes)
	{
		printASCITree(node);
		cout << endl;
	}
}

string generateExpression(const ASTNode &node, vector<string> &code)
{
	string res = "";
	for (auto child : node.children)
	{
		if (child->type == NUMBER_NODE)
		{
			res += child->value;
		}
		else if (child->type == IDENTIFIER_NODE)
		{
			res += child->value;
		}
		else if (child->type == OPERATOR_NODE)
		{
			res += child->value;
		}
	}
	return res;
}

vector<string> tempVars = {"t1", "t2", "t3", "t4", "t5", "t6", "t7", "t8"};
int curridx = 0;
void generateThreeAddressCode(const ASTNode &node, vector<string> &code)
{
	string res = "";
	if (node.type == ASSIGNMENT_NODE)
	{
		ASTNode idf = *node.children[0];
		string tempvar = tempVars[curridx++];
		ASTNode exp = *node.children[1];

		res += tempvar + " = ";
		res += generateExpression(exp, code);
		code.push_back(res);
		res = idf.value + " = " + tempvar;

		code.push_back(res);
	}
}

int main()
{
	const string inputFilePath = "input.txt";

	vector<pair<TokenType, string>> tokens = lexicalParser(inputFilePath);

	if (tokens.empty())
	{
		cout << "Error: No tokens found in input file" << endl;
		return 1;
	}

	vector<ASTNode> nodes = syntaxParser(tokens);

	if (!nodes.size())
	{
		cout << "Error: Failed to create AST" << endl;
		return 1;
	}

	cout << "\nASCII Parse Tree:" << endl;
	printASCIITree(nodes);

	saveSymbolTable();

	cout << "\nThree-Address Code:" << endl;
	vector<string> threeAddressCode;
	for (const ASTNode &node : nodes)
	{
		generateThreeAddressCode(node, threeAddressCode);
	}
	for (const string &line : threeAddressCode)
	{
		cout << line << "\n";
	}
	return 0;
}