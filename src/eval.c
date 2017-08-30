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

context_obj_t* lookupEnv(context_t* env, char* name) {
    for (int i = 0; i < env->size; i++) {
        if (strcmp(env->context[i]->name, name) == 0) {
            return env->context[i];
        }
    }

    return NULL;
}

node_t* evaluateList(context_t* env, node_t* node) {
    if (!IS_TYPE(HEAD(node), AST_SYMBOL)) {
        fprintf(stderr, "Expected a Symbol but got %s\n", strType(HEAD(node)));
        return NULL;
    }

    char* head = HEAD(node)->value.symbol->value;
    context_obj_t* fn = lookupEnv(env, head);
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

node_t* evaluate(context_t* env, node_t* node) {
    switch (node->type) {
        case AST_LIST:
            return evaluateList(env, node);
        default:
            return node;
    }

    return NULL;
}

void addFunctionToContext(context_t* context, char* name, fn function) {
    if(context->size == context->max) {
        context->max += DEFAULT_CONTEXT_SIZE;
        context->context = realloc(context->context, context->max * sizeof(context_obj_t*));
    }

    size_t n = context->size;

    context->context[n] = malloc(sizeof(context_obj_t));
    context->context[n]->name = name;
    context->context[n]->value.function = function;

    context->size = n + 1;
}

void freeContext(context_t* context) {
    for(int i = 0; i < context->size; i++) {
        free(context->context[i]);
    }

    free(context->context);
    free(context);
}

context_t* defaultEnv() {
    context_t* env = malloc(sizeof(context_t));

    env->max = DEFAULT_CONTEXT_SIZE;
    env->context = malloc(env->max * sizeof(context_obj_t*));

    addFunctionToContext(env, "print", &lispPrint);
    addFunctionToContext(env, "+", &lispAdd);

    return env;
}
