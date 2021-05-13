//Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez
//Assigment 1

#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <vector>
#include <map>
#include <stack>
#include <queue>


enum TokenType
{
	Operator_token,
	Keyword_token,
	Identifier_token,
	Seperator_token
}; // global token settings

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
	TS_EQUAL, // = 9
	TS_INT, // int 10
	TS_BOOL, // bool 11
	TS_FLOAT, //float 12
	//non-terminal
	NTS_E, // E  13
	NTS_Q, // Q  14
	NTS_T, // T  15
	NTS_R, // R  16
	NTS_F, // F  17
	NTS_S, // S  18
	NTS_A, // A   19
	NTS_D, // D 20
	NTS_TYPE // TYPE 21
};

void appendLineToFile(std::string filePath, std::string line){
	std::ofstream file;
	file.open(filePath, std::ios::out|std::ios::app);
	if(file.fail()){
		std::cout << "File failed to open" << std::endl;
	}
	file << line << std::endl;
}

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

 void outputFunction(std::queue<TokenType> tokenArray, std::vector<std::string>::const_iterator i)
{
	std::string tokenOutput;
	switch (tokenArray.front())
	{
		case 0: tokenOutput = "OPERATOR"; break;//operator
		case 1: tokenOutput = "KEYWORD"; break;//keyword
		case 2: tokenOutput = "IDENTIFIER"; break;//identifier
		case 3: tokenOutput = "SEPERATOR"; break;//seperator
		default: tokenOutput = "OPERATOR"; break; //default to operator for now
	}
	std::string output = "TOKEN: ";
	output+= tokenOutput;
	output += "      LEXEME: ";
	output+= *i;
	appendLineToFile("output.txt", " ");
	appendLineToFile("output.txt", output);


}

Symbols lexer(std::string a){

	std:: string c;
	for(int i = 0; i<a.length(); i++){
		if(a[i]!= ' '){
			c+=a[i];
		}
	}
	if(a.length()>1){
		if (isKeyword(c)){
			if(a[0] == 'i'){
				return TS_INT;
			}else if(a[0]== 'b'){
				return TS_BOOL;
			}else if(a[0] == 'f'){
				return TS_FLOAT;
			}else{
				return TS_INVALID;
			}
		}

	}
	char b = c[0];
	if(b!= '$' && isIdentifier(c)){
	return TS_ID;
}

		switch(b) {
		case '(': return TS_L_PARENS;
		case ')':	return TS_R_PARENS;
		case '+':	return TS_PLUS;
		case '*':return TS_STAR;
		case '/':return TS_DIV;
		case '-':return TS_MINUS;
		case '$':	return TS_EOS;
		case '=': return TS_EQUAL;
		default:
			std::cout<< "found invalid for " << a << " in lexer" << std::endl;
			return TS_INVALID;
	}


};


void syntax(std::string lineString, std::queue<TokenType> tokenArray )
{
	// Lexer gives us a Line that needs to be cleaned up
	// token Array is the list of every Terminal Symbol
	// and Non-Terminal Symbol the lexer encounters IN ORDER.

	//Bottom-up Table Definitions initialization
	std::string bottomUpTable[11][11];
	bottomUpTable[0][TS_ID] = "S5";
	bottomUpTable[0][TS_L_PARENS] = "S5";
	bottomUpTable[0][NTS_E] = "1";
	bottomUpTable[0][NTS_T] = "2";
	bottomUpTable[0][NTS_F] = "3";

	bottomUpTable[1][TS_PLUS] = "S6";
	bottomUpTable[1][TS_MINUS] = "S6";
	bottomUpTable[1][TS_ID] = "S5";
	bottomUpTable[1][TS_EOS] = "$";

	bottomUpTable[2][TS_PLUS] = "R2";
	bottomUpTable[2][TS_MINUS] = "R2";
	bottomUpTable[2][TS_DIV] = "S7";
	bottomUpTable[2][TS_STAR] = "S7";
	bottomUpTable[2][TS_R_PARENS] = "R2";
	bottomUpTable[2][TS_EOS] = "R2";

	bottomUpTable[3][TS_PLUS] = "R4";
	bottomUpTable[3][TS_MINUS] = "R4";
	bottomUpTable[3][TS_DIV] = "R4";
	bottomUpTable[3][TS_STAR] = "R4";
	bottomUpTable[3][TS_R_PARENS] = "R4";
	bottomUpTable[3][TS_EOS] = "R4";

	bottomUpTable[4][TS_ID] = "S5";
	bottomUpTable[4][TS_L_PARENS] = "S4";
	bottomUpTable[4][NTS_E] = "8";
	bottomUpTable[4][NTS_T] = "2";
	bottomUpTable[4][NTS_F] = "3";

	bottomUpTable[5][TS_PLUS] = "R6";
	bottomUpTable[5][TS_MINUS] = "R6";
	bottomUpTable[5][TS_STAR] = "R6";
	bottomUpTable[5][TS_DIV] = "R6";
	bottomUpTable[5][TS_R_PARENS] = "R6";
	bottomUpTable[5][TS_EOS] = "R6";

	bottomUpTable[6][TS_ID] = "S5";
	bottomUpTable[6][TS_L_PARENS] = "S4";
	bottomUpTable[6][NTS_T] = "9";
	bottomUpTable[6][NTS_F] = "3";

	bottomUpTable[7][TS_ID] = "S5";
	bottomUpTable[7][TS_L_PARENS] = "S4";
	bottomUpTable[7][NTS_F] = "10";

	bottomUpTable[8][TS_PLUS] = "S6";
	bottomUpTable[8][TS_MINUS] = "S6";
	bottomUpTable[8][TS_R_PARENS] = "S11";

	bottomUpTable[9][TS_PLUS] = "R1";
	bottomUpTable[9][TS_MINUS] = "R1";
	bottomUpTable[9][TS_DIV] = "S7";
	bottomUpTable[9][TS_STAR] = "S7";
	bottomUpTable[9][TS_R_PARENS] = "R1";
	bottomUpTable[9][TS_EOS] = "R1";

	bottomUpTable[10][TS_PLUS] = "R3";
	bottomUpTable[10][TS_MINUS] = "R3";
	bottomUpTable[10][TS_DIV] = "R3";
	bottomUpTable[10][TS_STAR] = "R3";
	bottomUpTable[10][TS_R_PARENS] = "R3";
	bottomUpTable[10][TS_EOS] = "R3";

	bottomUpTable[11][TS_PLUS] = "R5";
	bottomUpTable[11][TS_MINUS] = "R5";
	bottomUpTable[11][TS_DIV] = "R5";
	bottomUpTable[11][TS_STAR] = "R5";
	bottomUpTable[11][TS_R_PARENS] = "R5";
	bottomUpTable[11][TS_EOS] = "R5"; //State: 11, Symbol: $, returns R5 Table entry
	// STATE -- SYMBOL -- TABLE ENTRY
	// END OF TABLE DEFINITIONS



	std::vector<std::string> input;
	bool nextWord = false;
	std::string constructedWord;
	constructedWord.clear(); // initialized

	// this goes through the Line given by lexer and cleans it up
	// all words in the line are put into individual vectors
	// EX: I AM HERE, the vector is the line, each index is a word
	// Index 1 contains "I", index 2 contains 'AM', index 3 contains 'HERE'
	// this was done to make it easier for us later on when we needed to compare individual
	// words with other words within the same line.
	for(int f = 0; f < lineString.size(); f++){ // loop through each character
		constructedWord.push_back(lineString[f]); // push lexer word into line

		if( isOperator(lineString[f]) || lineString[f] == ' ')
		{
			input.push_back(constructedWord);
			constructedWord.clear();
			continue;
		}

		if( isOperator(lineString[f+1]) )
		{
			input.push_back(constructedWord);
			nextWord = true;
		}

		if (nextWord == true)
		{
			constructedWord.clear();
			nextWord = false;
			continue;
		}
	}
	input.push_back("$"); // add EOL symbol($) to input stack

	// append EOL symbol($) and '0' to stack.
	std::stack<std::string> characterStack; // initialize Stack
	characterStack.push("$");
	characterStack.push("0");



	if(lineString.size() == 0){ // if the Lexer gave us nothing, exit.
		return;
	}

	// bool to exist loop
	bool acct = false;
	bool error = false;
	// iteratoer for string array of input
	std::vector<std::string >:: const_iterator place = input.begin();


	while(!acct || !error){
		//holds top of nstack for table indexing
		int qm = atoi(characterStack.top().c_str());
		// holds input string for table indexing
		Symbols i = lexer(*place);
		// x hold table index spot
		std::string tableEntry = bottomUpTable[qm][i];

// if string x[0] == S process shift
 		if(tableEntry[0]== 'S'){
			// push input on to Stack then push number from table entry, increment line
			characterStack.push(*place);
			std::string number = atoi(tableEntry.c_str()+1);
			characterStack.push(number);
			place++;
			// if R do reduce process
		}else if (tableEntry[0] == 'R'){
			// holds counter for popping
		 auto g = 2 * atoi(tableEntry.c_str()+1);
			//loop to pop stack specified amount of times
			for(int f = 0; f<g ; f++){
				characterStack.pop();
			}
			// qj holds top of stack
			  int qj =  atoi(characterStack.top().substr(1,2));
				// qk holds table index qj, stack top
				std:: string qk = bottomUpTable[qj][lexer(*place)];
				// push qk onto stack individually
				for(int k = 0; k< qk.length()-1; k++){
					characterStack.push(qk.substr(k,1));
				}

					// if end of line, accept
			}else if (tableEntry[0] == '$'){
				acct = true;
				// if does not work in table, error = true
			}else{
				error =true;
			}
		}




	/*
	int table[25][25]; // table for rules
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
	table[NTS_S][TS_ID]=11;
	table[NTS_S][TS_BOOL]=12;
	table[NTS_S][TS_INT]=12;
	table[NTS_S][TS_FLOAT]=12;
	table[NTS_A][TS_ID]=13;
	table[NTS_D][TS_BOOL]=14;
	table[NTS_D][TS_INT]=14;
	table[NTS_D][TS_FLOAT]=14;
	table[NTS_TYPE][TS_INT]=15;
	table[NTS_TYPE][TS_FLOAT]=16;
	table[NTS_TYPE][TS_BOOL]=17;
	std::vector<std::string> line;

	bool nextWord = false;
	std::string constructedWord;
	constructedWord.clear(); // initialized

	for(int f = 0; f < lineString.size(); f++){
		constructedWord.push_back(lineString[f]);

		if( isOperator(lineString[f]) || lineString[f] == ' ')
		{
			line.push_back(constructedWord);
			constructedWord.clear();
			continue;
		}

		if( isOperator(lineString[f+1]) )
		{
			line.push_back(constructedWord);
			nextWord = true;
		}

		if (nextWord == true)
		{
			constructedWord.clear();
			nextWord = false;
			continue;
		}
	}

	line.push_back("$");
	//line+="$";// append EOL symbol($) to end of line
	ss.push(TS_EOS);
	bool s = false;
	for (int j=0;j<line.size();j++){
		if(line[j] == "="){
			s = true;
			break;
		}
	}

	if (s == true)
		ss.push(NTS_S);
	else
		ss.push(NTS_E);

	// token stuff
	bool firstGoAround = false;



	std::vector<std::string >:: const_iterator i = line.begin(); // strng iterator

	while (!ss.empty() && i!=line.end()){
		while(*i == " "){
			i++;
		}

		if (!firstGoAround)
		{
			outputFunction(tokenArray, i);
			tokenArray.pop();
			firstGoAround = true;
		}

		if(ss.top() == TS_ID || ss.top() == TS_MINUS || ss.top() == TS_PLUS || ss.top() == TS_STAR || ss.top() == TS_DIV ||
				ss.top() == TS_R_PARENS || ss.top() == TS_L_PARENS || ss.top() == TS_EOS || ss.top() == TS_EQUAL || ss.top()==TS_FLOAT ||
			ss.top()==TS_INT || ss.top()==TS_BOOL){

			if (lexer(*i) == ss.top()){
				if(ss.top() == TS_EOS && lexer(*i)== TS_EOS){
					//maybe return true
					appendLineToFile("output.txt", "");
					appendLineToFile("output.txt","-----------------------------------------");
					appendLineToFile("output.txt","LINE DONE");
					appendLineToFile("output.txt", " ");

					break;
				}
				i++;
				ss.pop();
				outputFunction(tokenArray, i);
				tokenArray.pop();

				std:: cout << std::endl;
				}else{
					appendLineToFile("output.txt","SYNTAX ERROR");
					return;
					//maybe return false
				}
		}else{


				if(table[ss.top()][lexer(*i)] != 0){   // check table position is not blank
					switch (table[ss.top()][lexer(*i)]){  // check table position in cases
						case 1: // E-> TQ
						appendLineToFile("output.txt","<Expression> --> <Term> <Expression>");
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						break;

						case 2: //Q -> +TQ
						appendLineToFile("output.txt","<ExpressionPrime> --> <PLUS> <TERM> <Q>");
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						ss.push(TS_PLUS);
						break;

						case 3: // Q-->-TQ
						appendLineToFile("output.txt","<Term>--><MINUS> <TERM> <Q>");
						ss.pop();
						ss.push(NTS_Q);
						ss.push(NTS_T);
						ss.push(TS_MINUS);
						break;

						case 4: // T-> e
						appendLineToFile("output.txt","<Term> --> <EPSILON>");
						ss.pop();
						break;

						case 5: // T-> FR
						appendLineToFile("output.txt","<Term> --> <FACTOR> <R>");
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						break;

						case 6: //R--> e
						appendLineToFile("output.txt","<R> --> <EPSILON>");
						ss.pop();
						break;

						case 7: // R--*FR
						appendLineToFile("output.txt","<R> --> <MULTIPLY> <FACTOR> <R>");
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						ss.push(TS_STAR);
						break;

						case 8: // R --> /FR
						appendLineToFile("output.txt", "<R> --> <DIV> <FACTOR> <R>");
						ss.pop();
						ss.push(NTS_R);
						ss.push(NTS_F);
						ss.push(TS_DIV);
						break;

						case 9: // F--> id
						appendLineToFile("output.txt", "<F> --> <id>");
						ss.pop();
						ss.push(TS_ID);
						break;

						case 10: // F--> (E)
						appendLineToFile("output.txt","<F> ---> <(E)>");
						ss.pop();
						ss.push(TS_R_PARENS);
						ss.push(NTS_E);
						ss.push(TS_L_PARENS);
						break;

						case 11: //S-->A
						appendLineToFile("output.txt","<STATEMENT> --> <ASSIGNMENT>");
						ss.pop();
						ss.push(NTS_A);
						break;

						case 12: // S-->D
						appendLineToFile("output.txt","<STATEMENT> --> <DECLARATIVE>");
						ss.pop();
						ss.push(NTS_D);
						break;

						case 13: // A--> ID=E
						appendLineToFile("output.txt","<ASSIGNMENT> --> <ID> <=> <EXPRESSION>");
						ss.pop();
						ss.push(NTS_E);
						ss.push(TS_EQUAL);
						ss.push(TS_ID);
						break;

						case 14: // D-->NTS_TYPE
						appendLineToFile("output.txt","<DECLARATIVE> --> <TYPE>");
						ss.pop();
						ss.push(NTS_TYPE);
						break;

						case 15: //TYPE-->INT
						appendLineToFile("output.txt","<TYPE> --> <int>");
						ss.pop();
						ss.push(NTS_S);
						ss.push(TS_INT);
						break;

						case 16: //TYPE-- float
						appendLineToFile("output.txt","<TYPE> --> <float>");
						ss.pop();
						ss.push(NTS_S);
						ss.push(TS_FLOAT);
						break;

						case 17: // TYpe--> bool
						appendLineToFile("output.txt","<TYPE> --> <bool>");
						ss.pop();
						ss.push(NTS_S);
						ss.push(TS_BOOL);
						break;



						default: appendLineToFile("output.txt","error in cases");
				}
			}else{
				appendLineToFile("output.txt","Syntax ERROR");
				std::cout << "Stack: " << ss.top() << " Token: " << *i << std::endl;
				return;

			}

	}

	}
	firstGoAround = false;
	*/
	return;
}


int main()
{
	bool debugMode = true; // disables user interface and loads input.txt automatically

	char entity;
	std::string a_line;
	a_line.clear(); //cleans string

	std::string filetoread;
	std::ifstream inputf;


	// basic user interface
	if (debugMode == false)
	{
		do {
			std::cout << std::string(50, '\n');
			std::cout << "Input File Name (EX: input.txt): "; //comment out for debugging
			getline(std::cin, filetoread); //comment out for debugging
			inputf.open(filetoread);
		} while (inputf.fail()); // loop if file name doesn't exist
		// basic user interface
	}
	else
		inputf.open("input.txt");


	std::ofstream outputf("output.txt");
	outputf << "Group Members: Kim Eaton, Luciano Gibertoni, Yanessa Vazquez" << std::endl << std::endl;

	std::vector<char> word; // this will push characters into a vector for when word is being processed...
	bool wordFormulation = false; // this will be a sentry value to check if word formulation is being done, default false.

	std::queue<TokenType> tokenArray; // this will be used to check what to output in the syntax section

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
			tokenArray.push(Operator_token);
			a_line.push_back(entity);
		}
		else if (isSeperator(entity))
		{
			if (entity = ';')
			{
				syntax(a_line, tokenArray); //sends the line to syntax
				while (tokenArray.empty() == false)
					tokenArray.pop(); // clears tokens
				a_line.clear(); // clears line
			}
			else
			{
				tokenArray.push(Seperator_token);
				a_line.push_back(entity);
			}
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
			a_line.push_back(' ');

			if (isInteger(wordString))
			{
				tokenArray.push(Identifier_token);
			}
			else if (isRealNumber(wordString))
			{
				tokenArray.push(Identifier_token);
			}
			else if (isKeyword(wordString))
			{
				tokenArray.push(Keyword_token);
			}
			else if (isIdentifier(wordString))
			{
				tokenArray.push(Identifier_token);
			}
			if (isOperator(entity))
			{
				a_line.push_back(entity);
				tokenArray.push(Operator_token);
			}
			else if (isSeperator(entity))
			{
				if (entity = ';')
				{
					syntax(a_line, tokenArray);
					a_line.clear();
					while (tokenArray.empty() == false)
						tokenArray.pop(); // clears tokens
				} else
				{
					a_line.push_back(entity);
					tokenArray.push(Seperator_token);
				}
			}
			word.clear(); // clear word
			wordFormulation = false;
			inputf >> std::skipws; // re-enable whitespace skipping for next iteration
		}
	}

	inputf.close();
	return 0;
}
