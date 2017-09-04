#ifndef __EVAL_H
#define __EVAL_H

#include "ast.h"

#define IS_TYPE(node, typ) (node != NULL && node->type == typ)
#define HEAD(node) (node->value.list->car)

#define AS_STRING(node) (node->value.string->value)
#define AS_NUMBER(node) (node->value.number->value)

typedef node_t *(*fn)(ast_node_list_t*);

typedef struct {
  char* name;
  union {
    fn function;
    node_t* data;
  } value;
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

static context_obj_t print = {.name = "print", .value.function = &lispPrint};
static context_obj_t add = {.name = "+", .value.function = &lispAdd};

static context_obj_t* builtins[] = { &print, &add, NULL };

#endif
