#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "eval.h"
#include "ast.h"

node_t* lispPrint(ast_node_list_t* args) {
    while (HAS_VALUE(args)) {
        node_t* node = args->car;

        switch(node->type) {
            case AST_STRING:
                printf("%s", node->value.string->value);
                break;
            case AST_NUMBER:
                printf("%ld", node->value.number->value);
                break;
            case AST_LIST:
                if(IS_NIL(node->value.list)) {
                    printf("nil");
                } else {
                    printf("(");
                    printf(")");
                }
                break;
            case AST_SYMBOL:
                printf("%s", node->value.symbol->value);
                break;
            default:
                puts("");
        }
        args = tail(args);
        if (HAS_VALUE(args)){
            printf(" ");
        }
    }
    printf("\n");
    return wrapNodeList(&nil);
}

node_t* lispAdd(ast_node_list_t* args) {
    int64_t acc = 0;
    while (HAS_VALUE(args)) {
        if (!IS_TYPE(args->car, AST_NUMBER)) {
            fprintf(stderr, "Add only takes numbers\n");
            return wrapNodeList(&nil);
        }
        acc += AS_NUMBER(args->car);
        args = tail(args);
    }

    return mkNodeNumber(acc);
}


env_t* lookupEnv(env_t* env[], char* name) {
    for (int i = 0; builtins[i] != NULL; i++) {
        if (strcmp(builtins[i]->name, name) == 0) {
            return builtins[i];
        }
    }

    return NULL;
}

node_t* evaluateList(env_t* env[], node_t* node) {
    if (!IS_TYPE(HEAD(node), AST_SYMBOL)) {
        fprintf(stderr, "Expected a Symbol but got %s\n", strType(HEAD(node)));
    }

    char* head = HEAD(node)->value.symbol->value;
    env_t* fn = lookupEnv(env, head);
    if (fn == NULL) {
        fprintf(stderr, "%s is not in scope\n", head);
    } else {
        ast_node_list_t* args = tail(node->value.list);
        ast_node_list_t* retArgs = &nil;
        while (HAS_VALUE(args)) {
            retArgs = append(evaluate(env, args->car), retArgs);
            args = tail(args);
        }
        return fn->value.function(retArgs);
    }
    return NULL;
}

node_t* evaluate(env_t* env[], node_t* node) {
    switch (node->type) {
        case AST_LIST:
            return evaluateList(env, node);
        default:
            return node;
    }

    return NULL;
}
