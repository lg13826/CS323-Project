//Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez
//Assigment 1

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
#include <stack>
#include <map>

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
	for (int i = 0; i < number.length(); i++) // check if everything's an integer
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
	for (int i = 0; i < number.length(); i++)
	{
		if (number[i] == '.') // find the first decimal and keep note of it, move to next integer
		{
			firstDecimalCheck = true;
			continue;
		} else if (number[i] == '.' && firstDecimalCheck) // Check if there's two decimals
			return false;

		if (number[i] >= '0' && number[i] <= '9') // make sure everything is a number
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

void stackParser(std::map<char, char> tableFunctionParameter, std::stack<char> stackFunctionParameter)
{
	while (stackFunctionParameter.empty() != true)
		char x = stackFunctionParameter.top(); // this one access's the first entity in the stack (the top one)
		char a; // incoming token
	// the top command accesses next element in stack
	if (X IS IN T) 
	{
		if (x == a)
		{
			stackFunctionParameter.pop();
		} else return;
	}
	else 
	{
		tableFunctionParameter.insert(x, a);
		stackFunctionParameter.pop();
		stackFunctionParameter.emplace
	}
}

int main()
{
	char entity;

	std::string filetoread;
	std::ifstream inputf;
	std::stack<char> stackProgram;  
	std::map<char, char> tableMap; // Table that Stack pushes into

	// basic user interface
	do { 
		std::cout << std::string(50, '\n');
		std::cout << "Input File Name (EX: input.txt): ";
		getline(std::cin, filetoread);
		inputf.open(filetoread); // open file of name
	} while (inputf.fail()); // loop if file name doesn't exist
	// basic user interface
	
	std::ofstream outputf("output.txt");
	outputf << "Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez" << std::endl << std::endl;

	std::vector<char> word; // this will push characters into a vector for when word is being processed...
	bool wordFormulation = false; // this will be a sentry value to check if word formulation is being done, default false.

	while (inputf.eof() == false) // go through input.txt and read each word.
	{
		inputf >> entity;
		stackProgram.push('$');

		if (entity == '!')
		{
			ignoreComment(entity, inputf);
			continue;
		}
		if (entity != '\n') 
			stackProgram.push(entity);
		else 
		{
			stackProgram.push('$');
			stackParser(&tableMap, &stackProgram);
		}

		outputf << "Token: ";
		if (isOperator(entity))
		{
			outputf << "OPERATOR" << '\t' << '\t' << "LEXEME: " << '\t' << entity << std::endl;
		}
		else if (isSeperator(entity))
		{
			outputf << "SEPERATOR" << '\t' << "LEXEME: " << '\t' << entity << std::endl;
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
				outputf << "INTEGER" << '\t' << "LEXEME: " << '\t' << wordString << std::endl;
			}
			else if (isRealNumber(wordString))
			{
				outputf << "REAL" << '\t' << "LEXEME: " << '\t' << wordString << std::endl;
			}
			else if (isKeyword(wordString))
			{
				outputf << "KEYWORD" << '\t' << "LEXEME: " << '\t' << wordString << std::endl;
			}
			else if (isIdentifier(wordString))
			{
				outputf << "IDENTIFIER" << '\t' << "LEXEME: " << '\t' << wordString << std::endl;
			}
			if (isOperator(entity))
			{
				outputf << "Token: ";
				outputf << "OPERATOR" << '\t' << '\t' << "LEXEME: " << '\t' << entity << std::endl;
			}
			else if (isSeperator(entity))
			{
				outputf << "Token: ";
				outputf << "SEPERATOR" << '\t' << "LEXEME: " << '\t' << entity << std::endl;
			}
			word.clear(); // clear word
			wordFormulation = false;
			inputf >> std::skipws; // re-enable whitespace skipping for next iteration
		}
	}

	inputf.close();
	return 0;
}
