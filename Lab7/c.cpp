#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <unordered_map>

using namespace std;

unordered_map<string, int> symbolTable;
int registerCounter = 0;

string getRegister()
{
	string reg = "r" + to_string(registerCounter);
	registerCounter++;
	return reg;
}

void parseExpression(const string &expr, vector<string> &assembly)
{
	stringstream ss(expr);
	string token;
	string op;
	string lhs, rhs;

	ss >> lhs;
	ss >> op;
	if (op == "=")
	{
		ss >> rhs;
		string reg = getRegister();
		string rhsReg = getRegister();
		parseExpression(rhs, assembly);
		assembly.push_back("mov " + reg + ", " + rhsReg);
		assembly.push_back("str " + reg + ", [sp, #" + to_string(symbolTable[lhs]) + "]");
	}
	else
	{
		ss >> rhs;
		string reg1 = getRegister();
		string reg2 = getRegister();
		parseExpression(lhs, assembly);
		stringstream ss;
		ss << "[sp, #" << symbolTable[lhs] << "]";
		assembly.push_back("mov " + reg1 + ", " + ss.str());
		ss.str(string()); // Clear the stringstream
		parseExpression(rhs, assembly);
		ss << "[sp, #" << symbolTable[rhs] << "]";
		assembly.push_back("mov " + reg2 + ", " + ss.str());
		string result = getRegister();
		if (op == "+")
		{
			assembly.push_back("add " + result + ", " + reg1 + ", " + reg2);
		}
		else if (op == "-")
		{
			assembly.push_back("sub " + result + ", " + reg1 + ", " + reg2);
		}
		else if (op == "*")
		{
			assembly.push_back("mul " + result + ", " + reg1 + ", " + reg2);
		}
		else if (op == "/")
		{
			assembly.push_back("udiv " + result + ", " + reg1 + ", " + reg2);
		}
		assembly.push_back("str " + result + ", [sp, #" + to_string(symbolTable[lhs]) + "]");
	}
}

vector<string> convertToAssembly(const string &input)
{
	vector<string> assembly;
	stringstream ss(input);
	string line;
	int offset = 0;

	while (getline(ss, line))
	{
		if (line.find("=") != string::npos)
		{
			string lhs = line.substr(0, line.find("="));
			symbolTable[lhs] = offset;
			offset += 4;
		}
	}

	ss.clear();
	ss.str(input);
	while (getline(ss, line))
	{
		parseExpression(line, assembly);
	}

	return assembly;
}

int main()
{
	string input = "C = 1 + 3;\nC = C + 2;";
	vector<string> assembly = convertToAssembly(input);

	for (const string &line : assembly)
	{
		cout << line << endl;
	}

	return 0;
}