/* Scanner
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "reader.h"
#include "charcode.h"
#include "token.h"
#include "error.h"


extern int lineNo;
extern int colNo;
extern int currentChar;

extern CharCode charCodes[];
int state;
/*uninitialized state is 0, property of global int*/
int ln, cn;
char str[MAX_IDENT_LEN];
char c;
/***************************************************************/


/*
int readChar(void) {
  currentChar = getc(inputStream);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;
}

int openInputStream() {
  inputStream = fopen("c:\example1.kpl", "rt");
    if (inputStream == NULL)
    {printf("Khong thay");return IO_ERROR;}
    else printf("Da mo xong");

  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
}

*/

void printToken(Token *token);

Token* getToken(void) 
{
  /*Reset the string*/
  memset(str, 0, sizeof(str));
  /*Its already reading currentChar from the file at the start, its implemented down there*/
  Token *token;
  /*debug prints*/
  /*debug prints end*/
  switch(state)
  {
  /*This case is the case to read the first character, and thats about it*/
  case 0:
  	if (currentChar == EOF) state =1;
  	else
  	switch (charCodes[currentChar]) 
    /*charCodes is an array of 255 ASCII character types, currentChar is an ASCII code of a character*/
  	{
  		case CHAR_SPACE: 
  			state =2;break;
  		case CHAR_LETTER: 
  		 ln=lineNo;
			 cn=colNo;
			 state =3;
			break;
  		case CHAR_DIGIT: 
		  	state =7; 
			break;
  		case CHAR_PLUS:
  			state =12;
			break;
  		case CHAR_MINUS:
  			state =10;
			break;
  		case CHAR_TIMES:
    		state =11;
			break;
  		case CHAR_SLASH:
    		state =12;
			break;
  		case CHAR_LT:
  			state =13;
			break;
  		case CHAR_GT:
  			state= 16; 
			break;
  		case CHAR_EQ:
    		state =19; 
			break;
  		case CHAR_EXCLAIMATION:
  			state = 20;
			break;
  		case CHAR_COMMA:
  			state =23; 
			break;
  		case CHAR_PERIOD:
  			state =24; 
			break;
  		case CHAR_SEMICOLON:
    		state=27;
			break;
  		case CHAR_COLON:
  			state =28;
			break;
   		case CHAR_SINGLEQUOTE: 
		    state =31;
			break;
		  case CHAR_LPAR:
			state = 35;
			break;
  		case CHAR_RPAR:
  			state= 42;
			break;
  		default: 
  			state=43;
    } return getToken(); 
   /*case 0 ends here*/


  /*calling getToken() again will make the program go back to the start of the function, and it
  will read the next character from the file, and then it will go to the switch case again, and
  it will go to the case that is the same as the state that was set in the previous switch case*/
  /*The state is set in the switch case, and the state is used to determine which case to go to in the switch case*/
  /*Also, calling getToken() again means reading a new token, and the state is reset to 0, so the program will read the first character of the new token*/

  
  case 1:
  	return makeToken(TK_EOF, lineNo, colNo);
  case 2:/*Blank met case*/
    while (charCodes[currentChar] == CHAR_SPACE){
      readChar();
    }
    state =0;
    return getToken();
  case 3:/*Met CHAR_LETTER*/
 	// TODO Recognize Identifiers and keywords
  ln = lineNo;
  cn = colNo;
  /*debug prints*/
  printf("Line number: %d\n", ln);
  printf("Column number: %d\n", cn);
  /*debug prints end*/
  
  int identifier_too_long = 0;
    while (charCodes[currentChar] == CHAR_LETTER || charCodes[currentChar] == CHAR_DIGIT){
      str[strlen(str)] = currentChar;
      readChar();
      if(strlen(str) > MAX_IDENT_LEN){
        identifier_too_long = 1;
      }
      
  }
  if(identifier_too_long){
    error(ERR_IDENTTOOLONG, ln, cn);
    token = makeToken(TK_NONE, ln, cn);
    state = 0;
    strncpy(token->string, str, strlen(str));
    return token;
  }
  printf("String: %s\n", str);
  token = makeToken(TK_NONE, ln, cn);
  /*Get the token's string content*/
  strncpy(token->string, str, MAX_IDENT_LEN);
  printToken(token);
  printf("Token type: %d\n", token->tokenType);
  state = 4;
  return getToken();

  case 4:
  		token->tokenType = checkKeyword(str);
  		if (token->tokenType == TK_NONE) state=5; else state =6;
    	return getToken();
  case 5:
  		token->tokenType = TK_IDENT;
      return token;
  case 6:
  		return token;
  case 7: /*Met CHAR_DIGIT*/
  /*keep reading the digits until it meets a non-digit character*/
  ln = lineNo;
  cn = colNo;
  /*Parsing characters*/
  while(charCodes[currentChar] == CHAR_DIGIT)
  {
    while(strlen(str)==0 && currentChar == '0'){
      readChar();
      /*Ignore leading zeros*/
    }
    str[strlen(str)] = currentChar;
    readChar();
    }
  /*Maximum number length case*/
  if(strlen(str)== MAX_NUMBER_LEN){
    if(atoi(str) > INT_MAX){
      error(ERR_NUMBERTOOLONG, ln, cn);
      token = makeToken(TK_NONE, ln, cn);
      state = 0;
      strncpy(token->string, str, strlen(str));
      return token;
    }
  }
  
  /*Error: Number too long case*/
  if(strlen(str) > MAX_NUMBER_LEN)
  {
    error(ERR_NUMBERTOOLONG, ln, cn);
    token = makeToken(TK_NONE, ln, cn);
    state = 0;
    strncpy(token->string, str, strlen(str));
    return token;
  }
  
  /*Normal case*/
  state = 8;
  return getToken();
  
  case 8:/*Token is a number*/
  /*Normal case*/
  token = makeToken(TK_NUMBER, ln, cn);
  token->value = atoi(str);
  token->tokenType = TK_NUMBER;
  strncpy(token->string, str, MAX_NUMBER_LEN);
  state = 0;
  return token;

  case 9:/*Met CHAR_PLUS*/
    token =  makeToken(SB_PLUS, lineNo, colNo-1);
    str[strlen(str)] = currentChar;
    strncpy(token->string, str, strlen(str));
    state = 0;
    readChar();
    /*read the next character to prepare for the next token*/
    return token;
  case 10:
	//TODO 
  case 11:
	//TODO 
  case 12:
	//TODO 
  case 13:
    readChar();
   	if (charCodes[currentChar] == CHAR_EQ) state = 14; else state =15; 
	return getToken();
  case 14:
	readChar();
	return makeToken(SB_LE, lineNo, colNo-1);
  case 15:
	return makeToken(SB_LT, lineNo, colNo-1);
  case 16:
	//TODO 
  case 17:
	//TODO 
  case 18:
	//TODO 
  case 19:
	//TODO 
  case 20:
	//TODO 
  case 21:
    readChar();
    return makeToken(SB_NEQ, lineNo, colNo-1);
  case 22:
    token = makeToken(TK_NONE, lineNo, colNo-1);
    error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
    return token;
  case 23:
	//TODO 
  case 24: 
    //TODO 
  case 25:
    //TODO 
  case 26:
    //TODO 
  case 27:
    //TODO 
  case 28:
    //TODO 
  case 29:
	//TODO 
  case 30:
    //TODO 
  case 31:
    readChar();
  	if (currentChar == EOF) 
  		state=34;
  		else 
			if(isprint(currentChar))
		  		state =32;
		  	else state =34;
  		return getToken();
   case 32:
	c= currentChar;
  	readChar();
  	if (charCodes[currentChar] == CHAR_SINGLEQUOTE) 
		  	state=33; 
		else 
			state =34;
  		return getToken();
  case 33:
  	token = makeToken(TK_CHAR, lineNo, colNo-1);
  	token->string[0] =c;
  	token->string[1] ='\0';
  	readChar();
  	return token;
  case 34:
  	error(ERR_INVALIDCHARCONSTANT, lineNo, colNo-2);
  case 35: // tokens begin with lpar, skip comments
  	ln = lineNo;
    cn = colNo;
    readChar();
    if (currentChar == EOF)
		state=41;
    else 
    	switch (charCodes[currentChar]) 
		{
    		case CHAR_PERIOD:
      			state =36;
				break;
    		case CHAR_TIMES:
    			state =38;
				break;
          	default:state =41;
        }
    return getToken();
    
  case 36: 
  	//TODO 
  case 37:
  	//TODO 
  case 38:
  	//TODO 
  case 39: 
  	//TODO 
  case 40:
    error(ERR_ENDOFCOMMENT, lineNo, colNo);
  case 41:return makeToken(SB_LPAR, ln, cn);
  case 42:
    readChar();
    return makeToken(SB_RPAR, lineNo, colNo);
 case 43:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, lineNo, colNo);
    readChar();
    return token;
}
}

/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_CHAR: printf("KW_CHAR\n"); break;
  case KW_ARRAY: printf("KW_ARRAY\n"); break;
  case KW_OF: printf("KW_OF\n"); break;
  case KW_FUNCTION: printf("KW_FUNCTION\n"); break;
  case KW_PROCEDURE: printf("KW_PROCEDURE\n"); break;
  case KW_BEGIN: printf("KW_BEGIN\n"); break;
  case KW_END: printf("KW_END\n"); break;
  case KW_CALL: printf("KW_CALL\n"); break;
  case KW_IF: printf("KW_IF\n"); break;
  case KW_THEN: printf("KW_THEN\n"); break;
  case KW_ELSE: printf("KW_ELSE\n"); break;
  case KW_WHILE: printf("KW_WHILE\n"); break;
  case KW_DO: printf("KW_DO\n"); break;
  case KW_FOR: printf("KW_FOR\n"); break;
  case KW_TO: printf("KW_TO\n"); break;
  case SB_SEMICOLON: printf("SB_SEMICOLON\n"); break;
  case SB_COLON: printf("SB_COLON\n"); break;
  case SB_PERIOD: printf("SB_PERIOD\n"); break;
  case SB_COMMA: printf("SB_COMMA\n"); break;
  case SB_ASSIGN: printf("SB_ASSIGN\n"); break;
  case SB_EQ: printf("SB_EQ\n"); break;
  case SB_NEQ: printf("SB_NEQ\n"); break;
  case SB_LT: printf("SB_LT\n"); break;
  case SB_LE: printf("SB_LE\n"); break;
  case SB_GT: printf("SB_GT\n"); break;
  case SB_GE: printf("SB_GE\n"); break;
  case SB_PLUS: printf("SB_PLUS\n"); break;
  case SB_MINUS: printf("SB_MINUS\n"); break;
  case SB_TIMES: printf("SB_TIMES\n"); break;
  case SB_SLASH: printf("SB_SLASH\n"); break;
  case SB_LPAR: printf("SB_LPAR\n"); break;
  case SB_RPAR: printf("SB_RPAR\n"); break;
  case SB_LSEL: printf("SB_LSEL\n"); break;
  case SB_RSEL: printf("SB_RSEL\n"); break;
  }
}

int scan(char *fileName) {
  Token *token;

  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

/*int main(int argc, char *argv[]) {
  if (argc <= 1) {
    printf("scanner: no input file.\n");
    return -1;
  }

  if (scan(argv[1]) == IO_ERROR) {
    printf("Can\'t read input file!\n");
    return -1;
  }

  return 0;
}*/

int main()
{

scan("test.kpl");
return 0;

}


/*gcc -o scanner scanner.c reader.c token.c error.c charcode.c*/