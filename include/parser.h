#ifndef __PARSER_H
#define __PARSER_H

#include "ast.h"

typedef struct {
  int consumed;
  node_t* value;
} parser_result_t;

parser_result_t* parse(const char* input);
#endif
