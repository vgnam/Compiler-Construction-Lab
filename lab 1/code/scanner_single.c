
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

/******************************************************************/
/* Reader */

#define IO_ERROR 0
#define IO_SUCCESS 1

FILE *inputStream;
int lineNo, colNo;
int currentChar;

int readChar(void) {
  currentChar = getc(inputStream);
  colNo ++;
  if (currentChar == '\n') {
    lineNo ++;
    colNo = 0;
  }
  return currentChar;
}

int openInputStream(void) {
  inputStream = stdin;
  lineNo = 1;
  colNo = 0;
  readChar();
  return IO_SUCCESS;
}

void closeInputStream(void) {
  if (inputStream != stdin)
    fclose(inputStream);
}

/******************************************************************/
/* CharCode */

typedef enum {
  CHAR_SPACE,
  CHAR_LETTER,
  CHAR_DIGIT,
  CHAR_PLUS,
  CHAR_MINUS,
  CHAR_TIMES,
  CHAR_SLASH,
  CHAR_LT,
  CHAR_GT,
  CHAR_EXCLAMATION,
  CHAR_EQ,
  CHAR_COMMA,
  CHAR_PERIOD,
  CHAR_COLON,
  CHAR_SEMICOLON,
  CHAR_SINGLEQUOTE,
  CHAR_LPAR,
  CHAR_RPAR,
  CHAR_UNKNOWN
} CharCode;

CharCode charCodes[256] = {
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_SPACE, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_SPACE, CHAR_EXCLAMATION, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_SINGLEQUOTE,
  CHAR_LPAR, CHAR_RPAR, CHAR_TIMES, CHAR_PLUS, CHAR_COMMA, CHAR_MINUS, CHAR_PERIOD, CHAR_SLASH,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT, CHAR_DIGIT,
  CHAR_DIGIT, CHAR_DIGIT, CHAR_COLON, CHAR_SEMICOLON, CHAR_LT, CHAR_EQ, CHAR_GT, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_LETTER,
  CHAR_LETTER, CHAR_LETTER, CHAR_LETTER, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,

  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN,
  CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN, CHAR_UNKNOWN
};

/******************************************************************/
/* Error */

typedef enum {
  ERR_ENDOFCOMMENT,
  ERR_IDENTTOOLONG,
  ERR_INVALIDCHARCONSTANT,
  ERR_INVALIDSYMBOL,
  ERR_NUMBERTOOLONG
} ErrorCode;

#define ERM_ENDOFCOMMENT "End of comment expected!"
#define ERM_IDENTTOOLONG "Identification too long!"
#define ERM_INVALIDCHARCONSTANT "Invalid const char!"
#define ERM_INVALIDSYMBOL "Invalid symbol!"
#define ERM_NUMBERTOOLONG "Value of integer number exceeds the range!"

void error(ErrorCode err, int lineNo, int colNo) {
  switch (err) {
  case ERR_ENDOFCOMMENT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_ENDOFCOMMENT);
    break;
  case ERR_IDENTTOOLONG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_IDENTTOOLONG);
    break;
  case ERR_NUMBERTOOLONG:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_NUMBERTOOLONG);
    break;
  case ERR_INVALIDCHARCONSTANT:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDCHARCONSTANT);
    break;
  case ERR_INVALIDSYMBOL:
    printf("%d-%d:%s\n", lineNo, colNo, ERM_INVALIDSYMBOL);
    break;
  }
  exit(0);
}

/******************************************************************/
/* Token */

#define MAX_IDENT_LEN 15
#define KEYWORDS_COUNT 21

typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_REAL, TK_CHAR, TK_EOF,

  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_INTEGER, KW_REAL, KW_CHAR, KW_ARRAY, KW_OF,
  KW_FUNCTION, KW_PROCEDURE,
  KW_BEGIN, KW_END, KW_CALL,
  KW_IF, KW_THEN, KW_ELSE,
  KW_WHILE, KW_DO, KW_FOR, KW_TO,

  SB_SEMICOLON, SB_COLON, SB_PERIOD, SB_COMMA,
  SB_ASSIGN, SB_EQ, SB_NEQ, SB_LT, SB_LE, SB_GT, SB_GE,
  SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH,
  SB_LPAR, SB_RPAR, SB_LSEL, SB_RSEL
} TokenType;

typedef struct {
  char string[MAX_IDENT_LEN + 1];
  int lineNo, colNo;
  TokenType tokenType;
  int value;
} Token;

struct {
  char string[MAX_IDENT_LEN + 1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
  {"PROGRAM", KW_PROGRAM},
  {"CONST", KW_CONST},
  {"TYPE", KW_TYPE},
  {"VAR", KW_VAR},
  {"INTEGER", KW_INTEGER},
  {"REAL", KW_REAL},
  {"CHAR", KW_CHAR},
  {"ARRAY", KW_ARRAY},
  {"OF", KW_OF},
  {"FUNCTION", KW_FUNCTION},
  {"PROCEDURE", KW_PROCEDURE},
  {"BEGIN", KW_BEGIN},
  {"END", KW_END},
  {"CALL", KW_CALL},
  {"IF", KW_IF},
  {"THEN", KW_THEN},
  {"ELSE", KW_ELSE},
  {"WHILE", KW_WHILE},
  {"DO", KW_DO},
  {"FOR", KW_FOR},
  {"TO", KW_TO}
};

int keywordEq(char *kw, char *string) {
  while ((*kw != '\0') && (*string != '\0')) {
    if (*kw != toupper(*string)) break;
    kw ++; string ++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}

TokenType checkKeyword(char *string) {
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string))
      return keywords[i].tokenType;
  return TK_NONE;
}

Token* makeToken(TokenType tokenType, int lineNo, int colNo) {
  Token *token = (Token*)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

/******************************************************************/
/* Scanner */

int state;
int ln, cn;
int numberAfterTo;
int lastLparLine, lastLparCol;
char str[MAX_IDENT_LEN + 1];
char c;

Token* getToken(void)
{
  Token *token;
  switch(state)
  {
  case 0:
    if (currentChar == EOF) state = 1;
    else {
    CharCode charCode = charCodes[currentChar];
    if (numberAfterTo && charCode != CHAR_SPACE && charCode != CHAR_DIGIT)
      numberAfterTo = 0;
    switch (charCode)
    {
      case CHAR_SPACE:
        state = 2; break;
      case CHAR_LETTER:
        ln = lineNo;
        cn = colNo;
        state = 3;
        break;
      case CHAR_DIGIT:
        state = 7;
        break;
      case CHAR_PLUS:
        state = 12;
        break;
      case CHAR_MINUS:
        state = 10;
        break;
      case CHAR_TIMES:
        state = 11;
        break;
      case CHAR_SLASH:
        state = 12;
        break;
      case CHAR_LT:
        state = 13;
        break;
      case CHAR_GT:
        state = 16;
        break;
      case CHAR_EQ:
        state = 19;
        break;
      case CHAR_EXCLAMATION:
        state = 20;
        break;
      case CHAR_COMMA:
        state = 23;
        break;
      case CHAR_PERIOD:
        state = 24;
        break;
      case CHAR_SEMICOLON:
        state = 27;
        break;
      case CHAR_COLON:
        state = 28;
        break;
      case CHAR_SINGLEQUOTE:
        state = 31;
        break;
      case CHAR_LPAR:
        state = 35;
        break;
      case CHAR_RPAR:
        state = 42;
        break;
      default:
        state = 43;
    }
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
    {
      TokenType tokenType = checkKeyword(str);
      if (tokenType == KW_TO)
        numberAfterTo = 1;
      return makeToken(tokenType, ln, cn);
    }
  case 7:
    {
      int i = 0;
      int tooLong = 0;
      int isReal = 0;
      int afterTo = numberAfterTo;
      long value = 0;

      numberAfterTo = 0;
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
      if (isReal)
        token = makeToken(TK_REAL, ln, cn);
      else if (afterTo)
        token = makeToken(KW_TO, ln, cn);
      else
        token = makeToken(TK_NONE, ln, cn);
      strcpy(token->string, str);
      if (!isReal)
        token->value = (int)value;
      return token;
    }
  case 9:
    readChar();
    return makeToken(SB_PLUS, lineNo, colNo - 1);
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
    if (charCodes[currentChar] == CHAR_EQ) state = 14; else state = 15;
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
    return makeToken(SB_NEQ, lineNo, colNo - 1);
  case 22:
    token = makeToken(TK_NONE, lineNo, colNo - 1);
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
    readChar();
    return makeToken(SB_SEMICOLON, lineNo, colNo - 1);
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
    ln = lineNo;
    cn = colNo;
    readChar();
    if (currentChar == EOF)
      state = 34;
    else
      if (isprint(currentChar))
        state = 32;
      else state = 34;
    return getToken();
  case 32:
    c = currentChar;
    readChar();
    if (charCodes[currentChar] == CHAR_SINGLEQUOTE)
      state = 33;
    else
      state = 34;
    return getToken();
  case 33:
    token = makeToken(TK_CHAR, lastLparLine, lastLparCol);
    token->string[0] = c;
    token->string[1] = '\0';
    readChar();
    return token;
  case 34:
    token = makeToken(TK_NONE, lineNo, colNo - 2);
    error(ERR_INVALIDCHARCONSTANT, token->lineNo, token->colNo);
    return token;
  case 35:
    ln = lineNo;
    cn = colNo;
    lastLparLine = ln;
    lastLparCol = cn;
    readChar();
    if (currentChar == EOF)
      state = 41;
    else
      switch (charCodes[currentChar])
      {
      case CHAR_PERIOD:
        state = 36;
        break;
      case CHAR_TIMES:
        state = 38;
        break;
      default:
        state = 41;
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
    readChar();
    return makeToken(SB_RPAR, lineNo, colNo);
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
  numberAfterTo = 0;
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
    return -1;
  }

  return 0;
}
