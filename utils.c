//
// Created by adam on 10.03.19.
//
#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

#define ALPHABET_SIZE 4

int32_t chtoi(const int32_t ch) {
    return ch - 48;
}

bool is_alphabet_element(const int32_t ch) {
    return ch <= '3' && ch >= '0';
}

void clear_sub(node * tree){
    if(tree == NULL) return;
    for(uint8_t i = 0; i < ALPHABET_SIZE; ++i){
        if(tree->child[i] != NULL){
            clear_sub(tree->child[i]);
            free(tree->child[i]);
        }
    }
}

void clear_tree(node ** tree){
    clear_sub(*tree);
    free(*tree);
}

void manage_input_error(void){
    fprintf(stderr, "ERROR\n");
}

bool is_number(const int32_t ch) {
    return ch <= '9' && ch >= '0';
}

const char * get_bool_message(const bool b) {
    if(b) return "YES";
    return "NO";
}
