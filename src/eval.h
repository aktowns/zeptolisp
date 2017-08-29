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
} env_t;

node_t* evaluateList(env_t*[], node_t*);
node_t* evaluate(env_t*[], node_t*);
node_t* lispPrint(ast_node_list_t*);
node_t* lispAdd(ast_node_list_t*);

static env_t print = {.name = "print", .value.function = &lispPrint};
static env_t add = {.name = "+", .value.function = &lispAdd};

static env_t* builtins[] = { &print, &add, NULL };

#endif
