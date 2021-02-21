#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>

//include additional function files here

void ignoreComment(char character, std::ifstream &inputf)
{
	do // read through the file until it reaches the last !
	{
		inputf >> character;
	} while (character != '!');
}

bool isOperator(char character)
{
	char operators[100] = { '+', '-', '*', '/','%', '>', '<','=' };
	for (int i = 0; i < 10; i++) {
        if (character == operators[i])
        return true;
    }
    return false;
}

bool isSeperator(char character)
{
	char separators[30] = { ';',',',':','(',')','{','}','[',']' };
	for (int j = 0; j < 10; ++j) {
		if (character == separators[j])
		return true;
    }
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

bool isInteger(std::string number)
{
	for (int i = 0; i < number.length(); i++)
	{
		if (number[i] >= '0' && number[i] <= '9')
			continue;
		else return false;
	}
	return true;
}

bool isRealNumber(std::string number)
{
	bool firstDecimalCheck = false;
	for (int i = 0; i < number.length(); i++) // check if the numbers left of the decimal are integers
	{
		if (number[i] == '.')
		{
			firstDecimalCheck = true;
			continue;
		} else if (number[i] == '.' && firstDecimalCheck) 
			return false;

		if (number[i] >= '0' && number[i] <= '9')
			continue;
		else return false;
	}
	return true;
}

bool isKeyword(std::string str)
{
	//vector to hold library of strings
	std::vector <std::string> library {"int" ,"float", "bool", "true", "False",
																"if", "else", "then", "endif", "endelse",
															  "while", "whileend", "do", "enddo", "for",
															  "endfor", "STDinput", "STDoutput", "and",
															   "or", "not"};
	//iterate through library, and compare strings, if the same return true
	for(int i = 0; i< library.size()-1; i++){
		if( str.compare(library[i]) == 0){
			return true;
		}
	}
// if no string is the same as library, return false
	return false;
}

int main()
{
	char entity;
	std::ifstream inputf("input.txt");
	std::ofstream outputf("output.txt");
	outputf << "TOKENS" << '\t' << '\t' << "Lexemes" << std::endl << std::endl;

	if (inputf.fail()) // error check, no input file detected and new one is created.
	{
		inputf.close();
		std::ofstream inputf("input.txt");
		return 1;
	}

	std::vector<char> word; // this will push characters into a vector for when word is being processed...
	bool wordFormulation = false; // this will be a sentry value to check if word formulation is being done, default false.

	while (inputf.eof() == false) // go through input.txt and read each word.
	{
		inputf >> entity;
		if (entity == '!')
		{
			ignoreComment(entity, inputf);
			continue;
		}
		if (isOperator(entity))
		{
			outputf << "OPERATOR" << '\t' << '=' << '\t' << entity << std::endl;
		}
		else if (isSeperator(entity))
		{
			outputf << "SEPERATOR" << '\t' << '=' << '\t' << entity << std::endl;
		}
		else do //most likely a word or number, start building the word using characters
		{
			wordFormulation = true;
			word.push_back(entity);
			inputf >> std::noskipws >> entity;
		} while (!isOperator(entity) && !isSeperator(entity) && entity != ' ' && entity != '\n' && inputf.eof() == false); // keep looping until a seperator, operator or whitespace is encountered....

		if(wordFormulation == true)
		{
			std::string wordString(word.begin(), word.end()); //convert character vector to string for reading in functions
			if (isInteger(wordString))
			{
				outputf << "INTEGER" << '\t' << '=' << '\t' << wordString << std::endl;
			}
			else if (isRealNumber(wordString))
			{
				outputf << "REAL" << '\t' << '=' << '\t' << wordString << std::endl;
			}
			else if (isKeyword(wordString))
			{
				outputf << "KEYWORD" << '\t' << '=' << '\t' << wordString << std::endl;
			}
			else if (isIdentifier(wordString))
			{
				outputf << "IDENTIFIER" << '\t' << '=' << '\t' << wordString << std::endl;
			}
			if (isOperator(entity))
			{
				outputf << "OPERATOR" << '\t' << '=' << '\t' << entity << std::endl;
			}
			else if (isSeperator(entity))
			{
				outputf << "SEPERATOR" << '\t' << '=' << '\t' << entity << std::endl;
			}
			//inputf.unget(); // move pointer backwards for next iteration, it's going to end up skipping a letter if this isn't done.
			word.clear(); // clear word
			wordFormulation = false;
			inputf >> std::skipws; // re-enable whitespace skipping for next iteration
		}
	}

	inputf.close();
	return 0;
}
