#include <iostream>
#include <fstream>
#include <string>
#include <locale>
//include additional function files here

bool isOperator(std::string str)
{
	return false;
}

bool isSeperator(std::string str)
{
	return false;
}

bool isIdentifier(std::string str)
{
	//identifier
	for(int i = 0; i < str.length(); i++)
	{
		if (isalpha(str[i]) || str[i] =='$' || str[i] == '_'
		    || (str[i] >= '0' && str[i] <= '9'))
		{
			// testing where it stops reading
			// cout << str[i];
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool isKeyword(std::string str)
{
	return false;
}

int main()
{
	std::string entity;
	std::ifstream inputf("input.txt");
	std::ofstream outputf("output.txt");
	outputf << "TOKENS" << '\t' << '\t' << "Lexemes" << std::endl << std::endl;

	if (inputf.fail()) // error check, no input file detected and new one is created.
	{
		inputf.close();
		std::ofstream inputf("input.txt");
		return 1;
	}

	while (inputf.eof() == false) // go through input.txt and read each word.
	{
		int type;
		inputf >> entity;

		if (isOperator(entity))
		{
			outputf << "OPERATOR" << '\t' << '=' << '\t';
		}
		else if (isSeperator(entity))
		{
			outputf << "SEPERATOR" << '\t' << '=' << '\t';
		}
		else if (isIdentifier(entity))
		{
			outputf << "IDENTIFIER" << '\t' << '=' << '\t';
		}
		else if (isKeyword(entity))
		{
			outputf << "KEYWORD" << '\t' << '=' << '\t';
		}

		outputf << entity << std::endl;		
	}

	inputf.close();
	return 0;
}