#include <iostream>
#include <fstream>
//include additional function files here


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

		if (isKeyword(entity))
		{
			outputf << "KEYWORD" << '\t' << '=' << '\t';
		}
		else if (isIdentifier(entity))
		{
			outputf << "IDENTIFIER" << '\t' << '=' << '\t';
		}
		else if (isSeperator(entity))
		{
			outputf << "SEPERATOR" << '\t' << '=' << '\t';
		}
		else if (isOperator(entity))
		{
			outputf << "OPERATOR" << '\t' << '=' << '\t';
		}

		outputf << entity << std::endl;		
	}

	inputf.close();
	return 0;
}