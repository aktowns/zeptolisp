#ifndef __EVAL_H
#define __EVAL_H

#include "ast.h"

#define IS_TYPE(node, typ) (node != NULL && node->type == typ)
#define HEAD(node) (node->value.list->car)

#define AS_STRING(node) (node->value.string->value)
#define AS_NUMBER(node) (node->value.number->value)

typedef struct {
    char* name;
    node_t* data;
} context_obj_t;

typedef struct {
  context_obj_t** context;
  size_t size;
  size_t max;
} context_t;

#define DEFAULT_CONTEXT_SIZE 1024

node_t* evaluateList(context_t*, node_t*);
node_t* evaluate(context_t*, node_t*);
node_t* lispPrint(ast_node_list_t*);
node_t* lispAdd(ast_node_list_t*);
context_t* defaultEnv(void);
void freeContext(context_t*);

#endif
