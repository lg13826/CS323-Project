//Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez
//Assigment 1

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
#include <map>
#include <stack>

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
enum Symbols{
	//non-terminal
	TS_L_PARENS,  // (
	TS_R_PARENS, // )
	TS_ID, // id
	TS_PLUS, // +
	TS_STAR, // *
	TS_EOS, // end of stack, $
	TS_INVALID, // invaled Token
	//non terminal
	NTS_E, // E
	NTS_EN, // E'
	NTS_T, // T
	NTS_TN, // T'
	NTS_F // F


};

Symbols lexer(char a){
	std:: string b;
	b.push_back(a);
	if(isIdentifier(b)){
		return TS_ID;
	}
	switch(a) {
	case '(': return TS_L_PARENS;
	case ')': return TS_R_PARENS;
	case '+': return TS_PLUS;
	case '*': return TS_STAR;
	case '$': return TS_EOS;
	default: return TS_INVALID;

}
}

void syntax(std::string lineString)
{
	std::cout<<lineString<< std::endl;
	std::stack<Symbols> ss; //symbol stack
	std::map<Symbols, std::map<Symbols, int> > table; // table for rules

	//initialize table
	table[NTS_E][TS_ID] = 1;
	table[NTS_E][TS_L_PARENS]=1;
	table[NTS_EN][TS_PLUS]=1;
	table[NTS_EN][TS_R_PARENS]=2;
	table[NTS_EN][TS_EOS]=2;
	table[NTS_T][TS_ID] =3;
	table[NTS_T][TS_L_PARENS] =3;
	table[NTS_TN][TS_PLUS]=5;
	table[NTS_TN][TS_STAR]=4;
	table[NTS_TN][TS_R_PARENS]=4;
	table[NTS_TN][TS_EOS]=4;
	table[NTS_F][TS_ID]=6;
	table[NTS_F][TS_L_PARENS]=7;


	lineString+='$';// append EOL symbol($) to end of line


	ss.push(TS_EOS);// initialize stack witb EOL
	ss.push(NTS_E); // initialize stack with non terminal

	int i = 0; // strng iterator

	while (ss.size()>0 && i<ss.size()-1){
		if (lexer(lineString[i]) == ss.top()){
			std::cout<<"Matched Symbols: " << lexer(lineString.at(i)) << std::endl;
			i++;
			ss.pop();
		}else{
			std::cout << "Rule: " << table[ss.top()][lexer(lineString.at(i))];
			switch (table[ss.top()][lexer(lineString.at(i))]){
				case 1: // E-> TE
				std::cout<<"<Expression>--> <Term><Expression>"<< std::endl;
				ss.pop();
				ss.push(NTS_EN);
				ss.push(NTS_T);
				break;

				case 2: //E' -> e
				std::cout<<"<ExpressionPrime>--> <Epsilon>"<< std::endl;
				ss.pop();
				break;

				case 3: // T>-FT'
				std::cout<<"<Term>--><Factor><TermPrime>"<< std::endl;
				ss.pop();
				ss.push(NTS_TN);
				ss.push(NTS_F);
				break;

				case 4: // T->FT'
				std::cout<<"<Term>--><Factor><TermPrime>"<< std::endl;
				ss.pop();
				ss.push(NTS_TN);
				ss.push(NTS_F);
				break;

				case 5: // T-> e
				std::cout<<"<Term>--><Epsilon>"<< std::endl;
				ss.pop();
				break;

				case 6: //F-> id
				std::cout<<"<Factor>--><ID>"<< std::endl;
				ss.pop();
				ss.push(TS_ID);
				break;

				case 7: // F->(E)
				std::cout<<"<Factor>--><(Expression)>"<< std::endl;
				ss.pop();
				ss.push(TS_R_PARENS);
				ss.push(NTS_F);
				ss.push(TS_L_PARENS);
				break;
				default: ss.pop();
			}
		}

	}
}


int main()
{
	char entity;
	std::string a_line;
	a_line.clear(); //cleans string

	std::string filetoread;
	std::ifstream inputf;


	// basic user interface
	do {
		std::cout << std::string(50, '\n');
		//std::cout << "Input File Name (EX: input.txt): "; Temporarily disabled for debugging
		//getline(std::cin, filetoread);
		inputf.open("input(1).txt"); // open file of name (Temporarily set to input(1).txt for debugging)
	} while (inputf.fail()); // loop if file name doesn't exist
	// basic user interface

	std::ofstream outputf("output.txt");
	outputf << "Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez" << std::endl << std::endl;

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


		outputf << "Token: ";
		if (isOperator(entity))
		{
			a_line.push_back(entity);
			outputf << "OPERATOR" << '\t' << '\t' << "LEXEME: " << '\t' << entity << std::endl;
		}
		else if (isSeperator(entity))
		{
			if (entity = ';')
			{
				syntax(a_line);
				a_line.clear();
			}
			else
				a_line.push_back(entity);
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

			a_line.append(wordString);

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
				a_line.push_back(entity);
				outputf << "Token: ";
				outputf << "OPERATOR" << '\t' << '\t' << "LEXEME: " << '\t' << entity << std::endl;
			}
			else if (isSeperator(entity))
			{
				if (entity = ';')
				{
					syntax(a_line);
					a_line.clear();
				} else
					a_line.push_back(entity);
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
