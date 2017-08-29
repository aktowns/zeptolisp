#include "parser.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_NUMBER_WIDTH 25 
parser_result_t* parseNumber(char* input) {
    parser_result_t* res = malloc(sizeof(parser_result_t));
    char c = input[0];

    char* bfr = calloc(sizeof(char), MAX_NUMBER_WIDTH);
    int z = 0;
    for (; isdigit(c = input[z]) || c == '-'; z++) {
        bfr[z] = c;
    }
    node_t* node = mkNodeNumber(atoi(bfr));
    free(bfr);

    res->consumed = z;
    res->value = node;
    return res;
}

parser_result_t* parseString(char* input) {
    parser_result_t* res = malloc(sizeof(parser_result_t));
    char c = input[0];

    char* bfr = calloc(sizeof(char), 1024);
    int z = 1;
    for (; (c = input[z]) != '"'; z++) {
        bfr[z-1] = c;
        if (z % 1023 == 0) {
            int sz = strlen(bfr)*sizeof(char);
            bfr = realloc(bfr, sz + 1024);
            memset(bfr+sz, 0, sz);
        }
    }
    node_t* node = mkNodeString(bfr);
    free(bfr);

    res->consumed = z + 1;
    res->value = node;
    return res;
}

parser_result_t* parseSymbol(char* input) {
    parser_result_t* res = malloc(sizeof(parser_result_t));
    char c = input[0];

    char* bfr = calloc(sizeof(char), 1024);
    int z = 0;
    for (; (c = input[z]) != ' ' && (c = input[z]) != '\0'; z++) {
        bfr[z] = c;
        bfr[z+1] = '\0';
        if (z != 0 && z % 1024 == 0) {
            int sz = strlen(bfr)*sizeof(char);
            bfr = realloc(bfr, sz + 1024);
            memset(bfr+sz, 0, sz);
        }
    }
    node_t* node = mkNodeSymbol(bfr);
    free(bfr);

    res->consumed = z;
    res->value = node;
    return res;
}

parser_result_t* parse(char*);

int findClosingParenIndex(char* input) {
    char c;
    int i = 0;
    int balance = 0;
    while ((c = input[i++]) != 0) {
        if (c == '(') {
            balance++;
        } else if (c == ')') {
            balance--;
            if (balance == 0) return i;
        }
    }

    return -1;
}

parser_result_t* parseList(char* input) {
    parser_result_t* res = malloc(sizeof(parser_result_t));

    int end = findClosingParenIndex(input);

    char* bfr = calloc(sizeof(char), end);
    memcpy(bfr, input+1, end-2);
    bfr[end-1] = '\0';

    parser_result_t* currentNode = NULL;
    ast_node_list_t* rootNode = &nil;
    int bfrOffset = 0;
    while ((currentNode = parse(bfr+bfrOffset))->consumed != 0) {
        bfrOffset += currentNode->consumed;
        rootNode = append(currentNode->value, rootNode);
    }

    node_t* node = wrapNodeList(rootNode);

    res->consumed = end;
    res->value = node;
    return res;
}

parser_result_t* parse(char* input) {
//    printf("entering parse '%s' (%lu)\n", input, strlen(input));
    if (input == NULL || strlen(input) == 0) {
        parser_result_t* pr = malloc(sizeof(parser_result_t));
        pr->consumed = 0;
        pr->value = wrapNodeList(&nil);
        return pr;
    }

    char c = input[0];
    int i = 0;
    do {
        parser_result_t* res; 
        if (isdigit(c) || c == '-') {
            res = parseNumber(input);
        } else if (c == '"') {
            res = parseString(input);
        } else if (c == '(') {
            res = parseList(input);
        } else {
            res = parseSymbol(input);
        }

        i += res->consumed;
        c = input[i];

        if (!isspace(c) && c != (int)NULL) {
            printf("Syntax error at %d (%c)\n", i, c);

            parser_result_t* pr = malloc(sizeof(parser_result_t));
            pr->consumed = 0;
            pr->value = wrapNodeList(&nil);
            return pr;
        }

        while(isspace(input[i])) {
            c = input[++i];
        }

        res->consumed = i;

        return res;
    } while ((c = input[i++]) != (int)NULL);
}