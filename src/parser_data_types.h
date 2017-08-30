#ifndef __PARSER_DATA_TYPES_H
#define __PARSER_DATA_TYPES_H

#include "ast.h"

typedef enum {
  PARSER_INIT, PARSER_START_LIST, PARSER_END_LIST, PARSER_STRING, PARSER_NUMBER, PARSER_SYMBOL, PARSER_ERROR
} parser_node_type_t;

typedef struct lexer_state_node {
  int index;
  int size;
  parser_node_type_t type;
  struct lexer_state_node* parent;
} lexer_state_node_t;

typedef struct {
  int error;
  node_t* result;
} parser_result_t;
#endif
