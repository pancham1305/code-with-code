// validate for loop syntax
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// array to copy first three characters of string str
char arr[3];

void isCorrect(char *str)
{

	// semicolon, bracket1, bracket2 are used
	// to count frequencies of
	//';', '(', and ')' respectively
	// flag is set to 1 when an error is found, else no error
	int semicolon = 0, bracket1 = 0, bracket2 = 0, flag = 0;

	int i;
	for (i = 0; i < 3; i++)
		arr[i] = str[i];

	// first 3 characters of the for loop statement is copied
	if (strcmp(arr, "for") != 0)
	{
		printf("Invalid");
		return;
	}

	// Proper usage of "for" keyword checked
	while (i != strlen(str))
	{
		char ch = str[i++];
		if (ch == '(')
		{
			// opening parenthesis count
			bracket1++;
		}
		else if (ch == ')')
		{
			// closing parenthesis count
			bracket2++;
		}
		else if (ch == ';')
		{
			// semicolon count
			semicolon++;
		}
		else
			continue;
	}

	// check number of semicolons
	if (semicolon != 2)
	{
		printf("\nInvalid");
		flag++;
	}

	// check closing Parenthesis
	else if (str[strlen(str) - 1] != ')')
	{
		printf("\nInvalid");
		flag++;
	}

	// check opening parenthesis
	else if (str[3] == ' ' && str[4] != '(')
	{
		printf("\nInvalid");
		flag++;
	}

	// check parentheses count
	else if (bracket1 != 1 || bracket2 != 1 || bracket1 != bracket2)
	{
		printf("\nInValid");
		flag++;
	}

	// no error
	if (flag == 0)
		printf("\nValid");
}

int main(void)
{

	char str1[100] = "for (i = 10; i < 20; i++)";
	isCorrect(str1);

	char str2[100] = "for i = 10; i < 20; i++)";
	isCorrect(str2);

	return 0;
}
