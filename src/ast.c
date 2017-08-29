#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"

node_t* mkNode(ast_type_t type) {
    node_t* node = malloc(sizeof(node_t));
    node->type = type;
    return node;
}

node_t* mkNodeString(char* str) {
    node_t* node = mkNode(AST_STRING);

    ast_node_string_t* nodeStr = malloc(sizeof(ast_node_string_t));
    nodeStr->value = malloc(strlen(str)+1);
    strcpy(nodeStr->value, str);

    node->value.string = nodeStr;
    return node;
}

node_t* mkNodeSymbol(char* str) {
    node_t* node = mkNode(AST_SYMBOL);

    ast_node_symbol_t* nodeStr = malloc(sizeof(ast_node_symbol_t));
    nodeStr->value = malloc(strlen(str)+1);
    strcpy(nodeStr->value, str);

    node->value.symbol = nodeStr;
    return node;
}

node_t* mkNodeNumber(int64_t num) {
    node_t* node = mkNode(AST_NUMBER);

    ast_node_number_t* nodeNum = malloc(sizeof(ast_node_number_t));
    nodeNum->value = num;

    node->value.number = nodeNum;
    return node;
}

node_t* mkQuoted(node_t* child) {
    node_t* node = mkNode(AST_QUOTED);

    ast_node_quoted_t* quoted = malloc(sizeof(ast_node_quoted_t));
    quoted->inner = child;

    node->value.quoted = quoted;
    return node;
}

ast_node_list_t* cons(node_t* car, ast_node_list_t* cdr) {
    ast_node_list_t* list = malloc(sizeof(ast_node_list_t));
    list->car = car;
    list->cdr = cdr;
    return list;
}

ast_node_list_t* append(node_t* value, ast_node_list_t* list) {
    if (list == NULL) return cons(value, &nil);

    ast_node_list_t* cell = malloc(sizeof(ast_node_list_t));
    cell->car = value;
    cell->cdr = &nil;

    if (IS_NIL(list)) {
        return cell;
    }

    ast_node_list_t* node = NULL;
    ast_node_list_t* parent = list;
    while ((node = parent->cdr)) {
        if (IS_NIL(node)) {
            parent->cdr = cell;
            return list;
        }
        parent = node;
    }
    // We didn't find Nil at the end of a list.
    free(cell);
    fprintf(stderr, "list append failed.");

    return &nil;
}

ast_node_list_t* tail(ast_node_list_t* list) {
    return list->cdr;
}

node_t* wrapNodeList(ast_node_list_t* list) {
    node_t* node = mkNode(AST_LIST);
    node->value.list = list;
    return node;
}

void _nodePP(node_t* node) {
    if (node == NULL) return;

    switch(node->type) {
        case AST_STRING: 
            printf("<STRING \"%s\">", node->value.string->value);
            break;
        case AST_NUMBER: 
            printf("<NUMBER %ld>", node->value.number->value);
            break;
        case AST_LIST: 
            if(IS_NIL(node->value.list)) {
                printf("nil");
            } else {
                printf("(");
                listPP(node->value.list);
                printf(")");
            }
            break;
        case AST_SYMBOL:
            printf("<SYMBOL %s>", node->value.symbol->value);
            break;
        case AST_QUOTED:
            printf("'");
            _nodePP(node->value.quoted->inner);
            break;
        default:
            puts("<UNKNOWN>"); 
    }
}

void nodePP(node_t* node) {
    printf("=> ");
    _nodePP(node);
}

void listPP(ast_node_list_t* list) {
    if (list == NULL) return;

    _nodePP(list->car);

    if (!IS_NIL(list->cdr)) {
        printf(" ");
        listPP(list->cdr);
    } 
} 

const char* strType(node_t* node) {
    switch(node->type) {
        case AST_SYMBOL: return "Symbol";
        case AST_LIST: return "List";
        case AST_NUMBER: return "Number";
        case AST_STRING: return "String";
        case AST_QUOTED: return "Quote";
        default: return "Unknown";
    }
}