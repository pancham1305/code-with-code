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
	// while (inputFile >> token) {
	//     TokenType tokenType = getTokenType(token);
	//     if(tokenType == IDENTIFIER)
	//         symbolTable[token] = tokenType;
	//     tokens.emplace_back(tokenType, token);
	// }
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

bool semeticAnalysis(ASTNode &node)
{
	// check if the identifier is declared before
	// cout << node.value << " | " << node.type << " | " << node.children.size() << endl;
	if (node.type == ASSIGNMENT_NODE)
	{
		ASTNode idf = *node.children[0];

		ASTNode exp = *node.children[1];

		if (exp.type == IDENTIFIER_NODE)
		{
			// cout << "e : " << exp.value << endl;
			if (symbolTable.find(exp.value) == symbolTable.end())
			{
				cout << "Error: " << exp.value << " is not declared" << endl;
				return false;
			}

			if (symbolTable[exp.value].second == false)
			{
				cout << "Error: " << exp.value << " is not initialized" << endl;
				return false;
			}
		}

		if (symbolTable.find(idf.value) == symbolTable.end())
		{
			cout << "Error: " << idf.value << " is not declared" << endl;
			return false;
		}

		if (node.children.size())
		{
			for (auto child : node.children)
			{
				auto c = *child;
				// cout << "child: " << c.value << " | " << c.type << " | " << c.children.size() << endl;
				bool f = semeticAnalysis(c);
				// cout << "fa: " << f << endl;
				if (!f)
					return false;
			}
		}
	}

	if (node.type == EXPRESSION_NODE)
	{
		for (auto child : node.children)
		{
			bool f = semeticAnalysis(*child);
			// cout << "fe: " << f << endl;
			if (!f)
				return false;
		}
	}

	if (node.type == IDENTIFIER_NODE)
	{
		// cout << "aaaa" << endl;
		if (symbolTable.find(node.value) == symbolTable.end())
		{
			// cout << "bbbb" << endl;
			cout << "Error: " << node.value << " is not declared" << endl;
			return false;
		}

		if (symbolTable[node.value].second == false)
		{
			// cout << "cccc" << endl;
			cout << "Error: " << node.value << " is not initialized" << endl;
			return false;
		}
	}

	if (node.type == NUMBER_NODE)
	{
		return true;
	}

	if (node.type == OPERATOR_NODE)
	{
		return true;
	}

	return true;
}
unordered_map<string, string> constants;
string optimizeCode(const ASTNode &node)
{

	string res = "";
	// what we gonna do is if the idf is not in the constants we add it to the constants and print to the string , if it i sin constant replace it with its  value
	if (node.type == ASSIGNMENT_NODE)
	{
		ASTNode idf = *node.children[0];
		ASTNode exp = *node.children[1];
		string exps = "";

		for (auto child : exp.children)
		{
			if (child->type == NUMBER_NODE)
			{
				constants[idf.value] = child->value;
				// exps += child->value;
			}
			if (child->type == OPERATOR_NODE)
			{
				exps += child->value;
			}
			if (child->type == IDENTIFIER_NODE)
			{
				if (constants.find(child->value) != constants.end())
				{
					exps += constants[child->value];
				}
				else
				{
					exps += child->value;
				}
			}
		}

		if (exps.size())
		{
			constants[idf.value] = exps;
		}

		res += idf.value + " = ";
		for (auto child : exp.children)
		{
			if (child->type == NUMBER_NODE)
			{
				res += child->value;
			}

			if (child->type == IDENTIFIER_NODE)
			{
				res += constants[child->value];
			}

			if (child->type == OPERATOR_NODE)
			{
				res += child->value;
			}
		}
	}

	return res;
}

string fixOPTMIZE(string opt)
{
	string res = "";

	string idf = "";
	int cnt = 0;
	for (int i = 0; i < opt.size(); i++)
	{
		string token = "";
		token += opt[i];
		cout << "token: " << token << "\n";
		if (token == " ")
			continue;
		auto gettype = getTokenType(token);
		if (gettype == IDENTIFIER)
		{
			idf = token;
			res += token;
		}
		else if (token == "=")
		{
			res += " = ";
		}
		else
		{
			auto tokenType = getTokenType(token);
			if (tokenType == OPERATOR)
			{
				cnt++;

				if (cnt == 2)
				{
					res += ";\n" + idf + " = " + idf + " " + token + " ";
					cnt = 0;
				}
				else
				{
					res += " " + token + " ";
				}
			}
			else if (tokenType == NUMBER)
			{
				res += token;
			}
		}
	}

	return res;
}

int registerCounter = 0;

string getRegister()
{
	return "r" + to_string(registerCounter++);
}

void convertToASM(string optimizeCode)
{
	vector<string> opts = {"+", "-", "*", "/"};

	// split the string into new line
	vector<string> lines;
	string line = "";
	for (int i = 0; i < optimizeCode.size(); i++)
	{
		string token = "";
		token += optimizeCode[i];
		if (token == ";")
		{
			lines.push_back(line);
			line = "";
			continue;
		}
		if(token == "\n")
		{
			continue;
		}
		else
		{
			if(token == ";") continue;
			line += token;
		}
	}

	for(auto l : lines){
		cout << "line: " << l << "\n";
	}

	// for each line we convert it to assembly

	vector<tuple<vector<string>, string, string>> subparts;
	for (auto line : lines)
	{
		stringstream ss(line);
		string token;
		string lhs;
		vector<string> subparttokens;
		string opt;
		string temp;
		bool isEqual = false;
		while (ss >> token)
		{
			cout <<token << "\n";
			if (find(opts.begin(), opts.end(), token) != opts.end())
			{
				opt = token;
			}
			else if (token == "=")
			{
				isEqual = true;
				continue;
			}
			else if (!isEqual)
			{
				lhs = token;
			}
			else
			{
				if (isdigit(token[0]))
				{
					string digits = "";
					while (isdigit(token[0]))
					{
						digits += token;
						ss >> token;
						// check we reach end of ss
						if (ss.eof())
						{
							break;
						}
					}

					subparttokens.push_back(digits);
					if (token.size())
					{
						if (find(opts.begin(), opts.end(), token) != opts.end())
						{
							opt = token;
						}
						else
						{
							subparttokens.push_back(token);
						}
					}
				}
				else
				{
					subparttokens.push_back(token);
				}
			}
		}

		subparts.push_back({subparttokens, opt,lhs});
	}

	// save to asm.as
	ofstream outputfile;
	outputfile.open("output.asm");
		map<string, string> registers;
	for (auto subpart : subparts)
	{
		auto tokens = get<0>(subpart);
		auto opt = get<1>(subpart);
		auto l = get<2>(subpart);
		string ll = l;

		if(registers.find(ll) == registers.end()){
			registers[ll] = getRegister();
		} 

		if (opt == "+")
		{
			auto t1 = tokens[0];
			auto t2 = tokens[1];

			if(!isdigit(t1[0])){
				t1 = registers[t1];
			}
			if(!isdigit(t2[0])){
				t2 = registers[t2];
			}
			outputfile << "add " << registers[ll] << ", " << t1 << ", " << t2 << "\n";
		}
		else if( opt == "-" ){
			auto t1 = tokens[0];
			auto t2 = tokens[1];

			if(!isdigit(t1[0])){
				t1 = registers[t1];
			}
			if(!isdigit(t2[0])){
				t2 = registers[t2];
			}
			outputfile << "sub " << registers[ll] << ", " << t1 << ", " << t2 << "\n";
		}
		else if( opt == "*" ){
			auto t1 = tokens[0];
			auto t2 = tokens[1];

			if(!isdigit(t1[0])){
				t1 = registers[t1];
			}
			if(!isdigit(t2[0])){
				t2 = registers[t2];
			}
			outputfile << "mul " << registers[ll] << ", " << t1 << ", " << t2 << "\n";
		}
		else if( opt == "/" ){
			auto t1 = tokens[0];
			auto t2 = tokens[1];

			if(!isdigit(t1[0])){
				t1 = registers[t1];
			}
			if(!isdigit(t2[0])){
				t2 = registers[t2];
			}
			outputfile << "udiv " << registers[ll] << ", " << t1 << ", " << t2 << "\n";
		}
	}
	outputfile.close();
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

	// semetic analysis

	for (ASTNode &node : nodes)
	{
		bool f = semeticAnalysis(node);
		if (f)
		{
			cout << "Semetic Analysis: Passed" << endl;
		}
		else
		{
			cout << "Semetic Analysis: Failed" << endl;
			return 0;
		}
	}

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

	vector<string> optcode;
	for (const ASTNode &node : nodes)
	{
		optcode.push_back(optimizeCode(node));
	}

	auto opt = optcode.back();

	cout << "semi optimized code: " << opt << "\n\n";

	string fixopt = fixOPTMIZE(opt);

	cout << "Optimized code: " << fixopt << "\n\n";

	// save to ouput.asm

	convertToASM(fixopt+";");

	return 0;
}