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
	//Terminal
	TS_L_PARENS,  // ( 0
	TS_R_PARENS, // ) 1
	TS_ID, // id 2
	TS_PLUS, // + 3
	TS_STAR, // * 4
	TS_EOS, // end of stack, $ 5
	TS_MINUS, // -  6
	TS_DIV, // /  7
	TS_INVALID, // invalid Token 8
	//non-terminal
	NTS_E, // E  9
	NTS_Q, // Q  10
	NTS_T, // T  11
	NTS_R, // R  12
	NTS_F // F  13


};

Symbols lexer(char a){
	std:: string b;
	b.push_back(a);
	if(a!='$' && isIdentifier(b)){
		return TS_ID;
	}
	switch(a) {
	case '(': return TS_L_PARENS;
	case ')':	return TS_R_PARENS;
	case '+':	return TS_PLUS;
	case '*':return TS_STAR;
	case '/':return TS_DIV;
	case '-':return TS_MINUS;
	case '$':	return TS_EOS;
	default:
		std::cout<< "found invalid for " << a << " in lexer" << std::endl;
		return TS_INVALID;

}
}

void syntax(std::string lineString)
{
	if (lineString.size() == 0)
		return;

	std::cout<<lineString<< std::endl;
	std::stack<Symbols> ss; //symbol stack
	int table[15][15]; // table for rules
	std::string line;
	for(int f = 0; f < lineString.size(); f++){
		if(lineString[f] != ' ' || lineString[f] != '='){
			line+= lineString[f];
		}
	}


	std::cout << "NTS_E= " << NTS_E << std::endl;
	std::cout << "NTS_EN= " << NTS_Q << std::endl;
	std::cout << "NTS_T= " << NTS_T << std::endl;
	std::cout << "NTS_TN= " << NTS_R << std::endl;
	std::cout << "NTS_F= " << NTS_F << std::endl;
	std::cout << "TS_ID= " << TS_ID << std::endl;
	std::cout << "TS_L_PARENS= " << TS_L_PARENS << std::endl;
	std::cout << "TS_R_PARENS= " << TS_R_PARENS << std::endl;
	std::cout << "TS_PLUS= " << TS_PLUS << std::endl;
	std::cout << "TS_STAR= " << TS_STAR << std::endl;
	std::cout << "TS_EOS= " << TS_EOS << std::endl;
	std::cout <<"TS_INVALID= " << TS_INVALID << std::endl;

	//initialize table
	table[NTS_E][TS_ID] = 1;
	table[NTS_E][TS_L_PARENS]=1;
	table[NTS_Q][TS_PLUS]=2;
	table[NTS_Q][TS_MINUS]=3;
	table[NTS_Q][TS_R_PARENS]=4;
	table[NTS_Q][TS_EOS]=4;
	table[NTS_T][TS_ID] =5;
	table[NTS_T][TS_L_PARENS] =5;
	table[NTS_R][TS_PLUS]=6;
	table[NTS_R][TS_MINUS]=6;
	table[NTS_R][TS_STAR]=7;
	table[NTS_R][TS_DIV]=8;
	table[NTS_R][TS_R_PARENS]=6;
	table[NTS_R][TS_EOS]=6;
	table[NTS_F][TS_ID]=9;
	table[NTS_F][TS_L_PARENS]=10;


	line+="$";// append EOL symbol($) to end of line


	ss.push(TS_EOS);// initialize stack witb EOL
	ss.push(NTS_E); // initialize stack with non terminal

	int i = 0; // strng iterator

	while (!ss.empty() && i<line.size()){
		if(line[i] == '='){
			i++;
		}

		if(ss.top() == TS_ID || ss.top() == TS_MINUS || ss.top() == TS_PLUS || ss.top() == TS_STAR || ss.top() == TS_DIV ||
				ss.top() == TS_R_PARENS || ss.top() == TS_L_PARENS || ss.top() == TS_EOS){

			if (lexer(line[i]) == ss.top()){
				std::cout<<"Matched Symbols: " << lexer(line[i]) << ", " << line[i] << std::endl;
				if(ss.top() == TS_EOS && lexer(line[i])== TS_EOS){
					//maybe return true
					std::cout << "Line Done";
					std::cout << "SS and LINE == TS_EOS" << std::endl;
					break;
				}
				i++;
				if(!ss.empty()){
					ss.pop();
				}
				std::cout << ss.size() << std::endl;
				}else{
					std::cout<< "SYNTAX ERROR" << std::endl;
					//maybe return false
				}
		}else{

				std::cout << "Switch table Starts: " << std::endl;
				std::cout << "Stack: " << ss.top() << " String: " << lexer(line[i]) << ", " << line[i] <<std::endl;

				if(table[ss.top()][lexer(line[i])] != 0){   // check table position is not blank
					switch (table[ss.top()][lexer(line[i])]){  // check table position in cases
						case 1: // E-> TQ
						std::cout<<"<Expression>--> <Term><Expression>"<< std::endl;
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						break;

						case 2: //Q -> +TQ
						std::cout<<"<ExpressionPrime>--> <PLUS><TERM><Q>"<< std::endl;
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						ss.push(TS_PLUS);
						break;

						case 3: // Q-->-TQ
						std::cout<<"<Term>--><MINUS><TERM><Q>"<< std::endl;
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						ss.push(TS_MINUS);
						break;

						case 4: // T-> e
						std::cout<<"<Term>--><EPSILON>"<< std::endl;
						ss.pop();
						break;

						case 5: // T-> FR
						std::cout<<"<Term>--><F><R>"<< std::endl;
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						break;

						case 6: //R--> e
						std::cout<<"<R>--><EPSILON>"<< std::endl;
						ss.pop();
						break;

						case 7: // R--*FR
						std::cout<<"<R>--><MULTIPLY><F><R>"<< std::endl;
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						ss.push(TS_STAR);
						break;

						case 8: // R --> /FR
						std::cout << "<R>--><DIV><F><R>" << std::endl;
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						ss.push(TS_DIV);
						break;

						case 9: // F--> id
						std::cout << "<F>--><id>" << std::endl;
						ss.pop();
						ss.push(TS_ID);
						break;

						case 10: // F--> (E)
						std::cout << "<F>---> <id>" << std::endl;
						ss.push(TS_R_PARENS);
						ss.push(NTS_E);
						ss.push(TS_L_PARENS);
						break;

						default: std::cout << "error in cases" << std::endl;
				}
			}else{
				std::cout << "Syntax ERROR" << std::endl;
				break;
			}

	}

	}
	return;
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
