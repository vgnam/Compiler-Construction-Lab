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
int ln, cn;
char str[MAX_IDENT_LEN + 1];
char c;
/***************************************************************/
Token* getToken(void) 
{
  Token *token;
  switch(state)
  {
  case 0:
  	if (currentChar == EOF) state =1;
  	else
  	switch (charCodes[currentChar]) 
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
  		case CHAR_EXCLAMATION:
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
    }
	return getToken();
  case 1:
  	return makeToken(TK_EOF, lineNo, colNo);
  case 2:
    while (currentChar != EOF && charCodes[currentChar] == CHAR_SPACE)
      readChar();
    state = 0;
    return getToken();
  case 3:
 	{
      int i = 0;
      int tooLong = 0;

      while (currentChar != EOF &&
             (charCodes[currentChar] == CHAR_LETTER ||
              charCodes[currentChar] == CHAR_DIGIT)) {
        if (i < MAX_IDENT_LEN)
          str[i++] = currentChar;
        else
          tooLong = 1;
        readChar();
      }
      str[i] = '\0';
      if (tooLong)
        error(ERR_IDENTTOOLONG, ln, cn);
      state = (checkKeyword(str) == TK_NONE) ? 5 : 6;
      return getToken();
    }
  case 4:
    {
      TokenType tokenType = checkKeyword(str);
      state = (tokenType == TK_NONE) ? 5 : 6;
      return getToken();
    }
  case 5:
  		token = makeToken(TK_IDENT, ln, cn);
      strcpy(token->string, str);
      return token;
  case 6:
  		return makeToken(checkKeyword(str), ln, cn);
  case 7: 
	{
    int i = 0;
    int tooLong = 0;
    int isReal = 0;
    long value = 0;

    ln = lineNo;
    cn = colNo;
    while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT) {
      int digit = currentChar - '0';
      if (i < MAX_IDENT_LEN)
        str[i++] = currentChar;
      if (value > (LONG_MAX - digit) / 10)
        tooLong = 1;
      else
        value = value * 10 + digit;
      if (value > INT_MAX)
        tooLong = 1;
      readChar();
    }
    if (currentChar == '.') {
      isReal = 1;
      if (i < MAX_IDENT_LEN)
        str[i++] = currentChar;
      else
        tooLong = 1;
      readChar();
      while (currentChar != EOF && charCodes[currentChar] == CHAR_DIGIT) {
        if (i < MAX_IDENT_LEN)
          str[i++] = currentChar;
        else
          tooLong = 1;
        readChar();
      }
    }
    str[i] = '\0';
    if (tooLong)
      error(ERR_NUMBERTOOLONG, ln, cn);
    token = makeToken(isReal ? TK_REAL : TK_NUMBER, ln, cn);
    strcpy(token->string, str);
    if (!isReal)
      token->value = (int)value;
    return token;
  }
  case 9:
	readChar();
    return makeToken(SB_PLUS, lineNo, colNo-1);
  case 10:
	ln = lineNo;
  cn = colNo;
  readChar();
	return makeToken(SB_MINUS, ln, cn);
  case 11:
	ln = lineNo;
  cn = colNo;
  readChar();
	return makeToken(SB_TIMES, ln, cn);
  case 12:
	ln = lineNo;
  cn = colNo;
  c = currentChar;
  readChar();
	return makeToken(c == '+' ? SB_PLUS : SB_SLASH, ln, cn);
  case 13:
    ln = lineNo;
    cn = colNo;
    readChar();
   	if (charCodes[currentChar] == CHAR_EQ) state = 14; else state =15; 
	return getToken();
  case 14:
	readChar();
	return makeToken(SB_LE, ln, cn);
  case 15:
	return makeToken(SB_LT, ln, cn);
  case 16:
	ln = lineNo;
  cn = colNo;
  readChar();
  if (charCodes[currentChar] == CHAR_EQ) state = 17; else state = 18;
  return getToken();
  case 17:
	readChar();
	return makeToken(SB_GE, ln, cn);
  case 18:
	return makeToken(SB_GT, ln, cn);
  case 19:
	ln = lineNo;
  cn = colNo;
  readChar();
	return makeToken(SB_EQ, ln, cn);
  case 20:
	ln = lineNo;
  cn = colNo;
  readChar();
  if (charCodes[currentChar] == CHAR_EQ) {
    readChar();
    return makeToken(SB_NEQ, ln, cn);
  }
  token = makeToken(TK_NONE, ln, cn);
  error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
  return token;
  case 21:
    readChar();
    return makeToken(SB_NEQ, lineNo, colNo-1);
  case 22:
    token = makeToken(TK_NONE, lineNo, colNo-1);
    error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
    return token;
  case 23:
	ln = lineNo;
  cn = colNo;
  readChar();
	return makeToken(SB_COMMA, ln, cn);
  case 24: 
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_RPAR) {
      readChar();
      return makeToken(SB_RSEL, ln, cn);
    }
    return makeToken(SB_PERIOD, ln, cn);
  case 25:
    readChar();
    return makeToken(SB_RSEL, ln, cn);
  case 26:
    return makeToken(SB_PERIOD, ln, cn);
  case 27:
    ln = lineNo;
    cn = colNo;
    readChar();
    return makeToken(SB_SEMICOLON, ln, cn);
  case 28:
    ln = lineNo;
    cn = colNo;
    readChar();
    if (charCodes[currentChar] == CHAR_EQ) {
      readChar();
      return makeToken(SB_ASSIGN, ln, cn);
    }
    return makeToken(SB_COLON, ln, cn);
  case 29:
	readChar();
	return makeToken(SB_ASSIGN, ln, cn);
  case 30:
    return makeToken(SB_COLON, ln, cn);
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
  	token = makeToken(TK_CHAR, lineNo, colNo-2);
  	token->string[0] =c;
  	token->string[1] ='\0';
  	readChar();
  	return token;
  case 34:
  	token = makeToken(TK_NONE, lineNo, colNo-2);
  	error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
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
  	readChar();
    return makeToken(SB_LSEL, ln, cn);
  case 37:
  	if (currentChar == EOF) {
      token = makeToken(TK_NONE, lineNo, colNo);
      error(ERR_ENDOFCOMMENT, token->lineNo, token->colNo);
      return token;
    }
    if (charCodes[currentChar] == CHAR_TIMES)
      state = 39;
    else {
      readChar();
      state = 37;
    }
    return getToken();
  case 38:
  	readChar();
    state = 37;
    return getToken();
  case 39: 
  	readChar();
    if (currentChar == EOF) {
      token = makeToken(TK_NONE, lineNo, colNo);
      error(ERR_ENDOFCOMMENT, token->lineNo, token->colNo);
      return token;
    }
    if (charCodes[currentChar] == CHAR_RPAR) {
      readChar();
      state = 0;
    } else
      state = 37;
    return getToken();
  case 40:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_ENDOFCOMMENT, token->lineNo, token->colNo);
    return token;
  case 41:
    return makeToken(SB_LPAR, ln, cn);
  case 42:
    ln = lineNo;
    cn = colNo;
    readChar();
    return makeToken(SB_RPAR, ln, cn);
 case 43:
    token = makeToken(TK_NONE, lineNo, colNo);
    error(ERR_INVALIDSYMBOL, token->lineNo, token->colNo);
    readChar();
    return token;
}
  return makeToken(TK_NONE, lineNo, colNo);
}

/******************************************************************/

void printToken(Token *token) {

  printf("%d-%d:", token->lineNo, token->colNo);

  switch (token->tokenType) {
  case TK_NONE: printf("TK_NONE\n"); break;
  case TK_IDENT: printf("TK_IDENT(%s)\n", token->string); break;
  case TK_NUMBER: printf("TK_NUMBER(%s)\n", token->string); break;
  case TK_REAL: printf("TK_REAL(%s)\n", token->string); break;
  case TK_CHAR: printf("TK_CHAR(\'%s\')\n", token->string); break;
  case TK_EOF: printf("TK_EOF\n"); break;

  case KW_PROGRAM: printf("KW_PROGRAM\n"); break;
  case KW_CONST: printf("KW_CONST\n"); break;
  case KW_TYPE: printf("KW_TYPE\n"); break;
  case KW_VAR: printf("KW_VAR\n"); break;
  case KW_INTEGER: printf("KW_INTEGER\n"); break;
  case KW_REAL: printf("KW_REAL\n"); break;
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

int scan(void) {
  Token *token;

  if (openInputStream() == IO_ERROR)
    return IO_ERROR;

  state = 0;
  token = getToken();
  while (token->tokenType != TK_EOF) {
    printToken(token);
    free(token);
    state = 0;
    token = getToken();
  }

  free(token);
  closeInputStream();
  return IO_SUCCESS;
}

/******************************************************************/

int main(void) {
  if (scan() == IO_ERROR) {
    printf("Can\'t read input!\n");
    return -1;
  }

  return 0;
}
