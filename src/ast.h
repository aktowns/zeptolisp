#ifndef __AST_H
#define __AST_H

#include <stdint.h>
#include <stdlib.h>

struct node;

typedef enum {
    AST_STRING, AST_NUMBER, AST_LIST, AST_SYMBOL
} ast_type_t;

typedef struct {
    char* value;
} ast_node_string_t;

typedef struct {
    int64_t value; 
} ast_node_number_t;

typedef struct ll {
    struct node* car; 
    struct ll* cdr; 
} ast_node_list_t;

typedef struct {
    char* value;
} ast_node_symbol_t;

typedef struct node {
    ast_type_t type; 
    union {
        ast_node_string_t* string;
        ast_node_symbol_t* symbol;
        ast_node_number_t* number;
        ast_node_list_t* list;
    } value;
} node_t;

static ast_node_list_t nil = { .car = NULL, .cdr = NULL };
#define IS_NIL(x) (x != NULL && x->car == NULL && x->cdr == NULL)
#define HAS_VALUE(x) (x != NULL && x->car != NULL)

node_t* mkNode(ast_type_t type);
node_t* mkNodeString(char* str);
node_t* mkNodeSymbol(char* str);
node_t* mkNodeNumber(int64_t num);
ast_node_list_t* cons(node_t* car, ast_node_list_t* cdr);
ast_node_list_t* append(node_t* value, ast_node_list_t* list);
ast_node_list_t* tail(ast_node_list_t* list);
node_t* wrapNodeList(ast_node_list_t* list);
void nodePP(node_t* node);
void listPP(ast_node_list_t* list);
const char* strType(node_t* node);

#endif