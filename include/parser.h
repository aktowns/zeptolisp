#ifndef __PARSER_H
#define __PARSER_H

#include "parser_data_types.h"

parser_result_t* resolveStack(lexer_state_node_t**, char*, char*);
parser_result_t* parse(char*);

#endif
