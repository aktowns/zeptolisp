#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "parser_data_types.h"
#include "lexer.h"

parser_result_t* resolveStack(lexer_state_node_t**, char*, char*);

parser_result_t* resolveList(lexer_state_node_t** state, char* input, char* bfr) {
  parser_result_t* result = malloc(sizeof(parser_result_t));
  ast_node_list_t* list = &nil;

  int recur = true;
  int firstRun = true;
  do {
    if (firstRun) {
      firstRun = false;
    } else {
      *state = (*state)->parent;
    }
    lexer_state_node_t* innerState = (*state);

    //statePP(innerState);

    if((*state) == NULL || (*state)->type == PARSER_START_LIST) {
      *state = (*state)->parent;
      recur = false;
    }
    else {
      parser_result_t* result = resolveStack(state, input, bfr);
      list = cons(result->result, list);
      free(result);
    }

    memset(bfr, 0, strlen(bfr));
  } while((*state) != NULL && (*state)->parent != NULL && recur);

  result->result = wrapNodeList(list);

  return result;
}

parser_result_t* resolveStack(lexer_state_node_t** state, char* input, char* bfr) {
  parser_result_t* result = NULL;
  node_t* node = wrapNodeList(&nil);
  lexer_state_node_t* innerState = (*state);

  switch(innerState->type) {
  case PARSER_STRING:
    memcpy(bfr, input + innerState->index, innerState->size);
    node = mkNodeString(bfr);
    break;
  case PARSER_NUMBER:
    memcpy(bfr, input + innerState->index, innerState->size);
    node = mkNodeNumber(atoi(bfr));
    break;
  case PARSER_SYMBOL:
    memcpy(bfr, input + innerState->index, innerState->size);
    node = mkNodeSymbol(bfr);
    break;
  case PARSER_END_LIST:
    *state = innerState->parent;
    result = resolveList(state, input, bfr);
    break;
  default:
    // noop
    break;
  }

  if(!result) {
    result = malloc(sizeof(parser_result_t));
    result->result = node;
  } else {
    free(node);
  }

  return result;
}

parser_result_t* parse(char* input) {
  lexer_state_node_t* state = lexString(input);

  lexer_state_node_t* otherState = state;
  do {
    statePP(otherState);
    otherState = otherState->parent;
  } while(otherState != NULL);

  char* bfr = calloc(sizeof(char), 1024);
  parser_result_t* result = resolveStack(&state, input, bfr);
  free(bfr);

  return result;
}

int main(void) {
  nodePP(parse("((abc) (aa) aa)")->result);
  puts("");
  exit(1);
}
