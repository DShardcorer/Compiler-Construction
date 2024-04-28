#include <stdio.h>

// Define the TokenType enumeration
typedef enum {
  TK_NONE, TK_IDENT, TK_NUMBER, TK_CHAR, TK_EOF,

  KW_PROGRAM, KW_CONST, KW_TYPE, KW_VAR,
  KW_INTEGER, KW_CHAR, KW_ARRAY, KW_OF, 
  KW_FUNCTION, KW_PROCEDURE,
  KW_BEGIN, KW_END, KW_CALL,
  KW_IF, KW_THEN, KW_ELSE,
  KW_WHILE, KW_DO, KW_FOR, KW_TO,

  SB_SEMICOLON, SB_COLON, SB_PERIOD, SB_COMMA,
  SB_ASSIGN, SB_EQ, SB_NEQ, SB_LT, SB_LE, SB_GT, SB_GE,
  SB_PLUS, SB_MINUS, SB_TIMES, SB_SLASH,
  SB_LPAR, SB_RPAR, SB_LSEL, SB_RSEL
} TokenType;

// Function to print the name of a token type
void printTokenType(TokenType type) {
  switch (type) {
    case TK_NONE: printf("None\n"); break;
    case TK_IDENT: printf("Identifier\n"); break;
    case TK_NUMBER: printf("Number\n"); break;
    case TK_CHAR: printf("Character\n"); break;
    case TK_EOF: printf("End of File\n"); break;
    // Handle keyword token types
    case KW_PROGRAM: printf("Keyword: PROGRAM\n"); break;
    case KW_CONST: printf("Keyword: CONST\n"); break;
    // Handle special symbol token types
    case SB_SEMICOLON: printf("Special Symbol: Semicolon\n"); break;
    case SB_COLON: printf("Special Symbol: Colon\n"); break;
    // Add cases for other token types as needed
    default: printf("Unknown Token Type\n"); break;
  }
}

int main() {
  // Example usage of the TokenType enumeration
  TokenType token1 = TK_IDENT;
  TokenType token2 = KW_PROGRAM;
  TokenType token3 = SB_SEMICOLON;

  // Print the name of each token type
  printf("Token 1: ");
  printTokenType(token1);

  printf("Token 2: ");
  printTokenType(token2);

  printf("Token 3: ");
  printTokenType(token3);

  return 0;
}
