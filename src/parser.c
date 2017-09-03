#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "parser_data_types.h"
#include "lexer.h"

parser_result_t* resolveStack(lexer_state_node_t**, char*, char*);

parser_result_t* mkEmptyResult(void) {
  parser_result_t* result = malloc(sizeof(parser_result_t));
  result->result = NULL;
  result->error = 0;

  return result;
}

parser_result_t* mkResult(node_t* node, int error) {
  parser_result_t* result = malloc(sizeof(parser_result_t));
  result->result = node;
  result->error = error;

  return result;
}

parser_result_t* resolveList(lexer_state_node_t** state, char* input, char* bfr) {
  parser_result_t* result = mkEmptyResult();
  ast_node_list_t* list = &nil;

  bool recur = true;
  int firstRun = true;
  do {
    if (firstRun) {
      firstRun = false;
    } else {
      *state = (*state)->parent;
    }

    if((*state) == NULL || (*state)->type == PARSER_START_LIST) {
      *state = (*state)->parent;
      recur = false;
    }
    else {
      parser_result_t* stackResult = resolveStack(state, input, bfr);

      if(stackResult->error) {
        free(result);
        return stackResult;
      } else {
        list = cons(stackResult->result, list);
        free(stackResult);
      }
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

  if(innerState->balance < 0) {
    return mkResult(node, 1);
  }

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

  if(result) {
    free(node);
  } else {
    result = mkEmptyResult();
    result->result = node;
  }

  return result;
}

parser_result_t* parse(char* input) {
  lexer_state_node_t* state = lexString(input);

  char* bfr = calloc(sizeof(char), 1024);
  parser_result_t* result = resolveStack(&state, input, bfr);
  free(bfr);

  return result;
}
