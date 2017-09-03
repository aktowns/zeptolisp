#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser_data_types.h"

lexer_state_node_t* mkEmptyStateNode(lexer_state_node_t* parent, int index) {
  lexer_state_node_t* state = malloc(sizeof(lexer_state_node_t));
  state->type = PARSER_INIT;
  state->parent = parent;
  state->index = index;
  state->balance = (parent) ? parent->balance : 0;
  state->size = 0;

  return state;
}

lexer_state_node_t* mkErrorStateNode(lexer_state_node_t* parent, int index) {
  lexer_state_node_t* state = mkEmptyStateNode(parent, index);
  state->type = PARSER_ERROR;

  return state;
}

lexer_state_node_t* mkEndListStateNode(lexer_state_node_t* parent, int index) {
  lexer_state_node_t* state = mkEmptyStateNode(parent, index);
  state->type = PARSER_END_LIST;
  state->balance -= 1;

  return state;
}

void updateStateNode(lexer_state_node_t* state, char c, int index) {
  state->index = index;

  if (isspace(c)) {
    // noop
  }
  else if(c == '(') {
    state->type = PARSER_START_LIST;
    state->balance += 1;
  }
  else if(c == ')') {
    state->type = PARSER_END_LIST;
    state->balance -= 1;
  }
  else if(c == '"') {
    state->index = index + 1;
    state->size = -1;
    state->type = PARSER_STRING;
  }
  else if(isdigit(c)) {
    state->size = 1;
    state->type = PARSER_NUMBER;
  }
  else if(c == '-' || c == '+') {
    state->size = 1;
    state->type = PARSER_NUMERIC_SYMBOL;
  }
  else {
    state->size = 1;
    state->type = PARSER_SYMBOL;
  }
}

lexer_state_node_t* mkStateNode(lexer_state_node_t* parent, char c, int index) {
  lexer_state_node_t* state = mkEmptyStateNode(parent, index);
  updateStateNode(state, c, index);

  return state;
}

void statePP(lexer_state_node_t* state) {
  puts("--");
  printf("Start: %d | Size %d\n", state->index, state->size);
  printf("Balance: %d\n", state->balance);
  printf("Type: ");

  switch(state->type) {
  case PARSER_INIT:
    printf("init");
    break;
  case PARSER_START_LIST:
    printf("start list");
    break;
  case PARSER_END_LIST:
    printf("end list");
    break;
  case PARSER_STRING:
    printf("string");
    break;
  case PARSER_NUMBER:
    printf("number");
    break;
  case PARSER_SYMBOL:
    printf("symbol");
    break;
  case PARSER_ERROR:
    printf("error");
    break;
  default:
    printf("<unknown>");
  }
  puts("\n--");

}

int handleEndToken(lexer_state_node_t** state, char c, int index) {
  if(isspace(c)) {
    *state = mkEmptyStateNode(*state, index);
  }
  else if(c == ')') {
    *state = mkEndListStateNode(*state, index);
  }
  else {
    return false;
  }

  return true;
}

lexer_state_node_t* lexChar(lexer_state_node_t* state, char c, int index) {
  switch(state->type) {
  case PARSER_INIT:
    updateStateNode(state, c, index);
    break;
  case PARSER_START_LIST:
  case PARSER_END_LIST:
    state = mkStateNode(state, c, index);
    break;
  case PARSER_STRING:
    if(state->size == -1) {
      if(c == '"') {
        state->size = index - state->index;
      }
    } else {
      if(!handleEndToken(&state, c, index)) {
        state = mkErrorStateNode(state, index);
      }
    }

    break;
  case PARSER_NUMERIC_SYMBOL:
    if(isdigit(c)) {
      state->type = PARSER_NUMBER;
      state->size++;
    }
    else {
      state->type = PARSER_SYMBOL;

      if(!handleEndToken(&state, c, index)) {
        state->size++;
      }
    }

    break;
  case PARSER_NUMBER:
    if(!handleEndToken(&state, c, index)) {
      state->size++;

      if (!isdigit(c)) {
        state->type = PARSER_SYMBOL;
      }
    }
    break;
  case PARSER_SYMBOL:
    if(!handleEndToken(&state, c, index)) {
      state->size++;
    }
  case PARSER_ERROR:
    // noop
    break;
  default:
    printf("whoops\n");
    exit(1);
  }

  return state;
}

lexer_state_node_t* lexString(char* input) {
  lexer_state_node_t* state = mkStateNode(NULL, input[0], 0);

  for(int i = 1; i < strlen(input); i++) {
    state = lexChar(state, input[i], i);
  }

  if(state->type == PARSER_NUMERIC_SYMBOL) {
    state->type = PARSER_SYMBOL;
  }

  return state;
}
