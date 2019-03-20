//
// Created by adam on 07.03.19.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include "history.h"
#include "utils.h"

#define ALPHABET_SIZE 4

/* Struktura pomocnicza, służąca do zwracania wyników funkcji find_history
 * przechowuje adres elementu node oraz jedną zmienną typu size_t
 */
typedef struct pair_adress_number{
    node * first;
    size_t second;
} PAN;


/* Funkcja przechodzi podane drzewo po zadanej ścieżce, aż do momentu zakończenia ścieżki
 * lub braku odpowiedniego odgałęzienia w drzewie. Zwraca wskaźnik do ostatniego odwiedzonego
 * wierzchołka drzewa oraz odpowiadającą mu pozycję w tablicy opisującej ścieżkę.
 */
static PAN find_history(const char * history, node * tree, const size_t length){
    size_t i = 0;
    node * iterator = tree;
    while(i < length && iterator->child[chtoi(history[i])] != NULL){
        iterator = iterator->child[chtoi(history[i])];
        i++;
    }
    PAN result;
    result.first = iterator;
    result.second = i;
    return result;
}

/* Funkcja usuwa rekurencyjnie wszystkie wierzchołki w poddrzewie zadanego wierzchołka
 */
static void remove_subtree(node * tree){
    for(int i = 0; i < ALPHABET_SIZE; ++i){
        if(tree->child[i] != NULL) {
            remove_subtree(tree->child[i]);
            free(tree->child[i]);
            tree->child[i] = NULL;
        }
    }
    if(tree->right != tree){
        tree->left->right = tree->right;
        tree->right->left = tree->left;
    }
}

bool add_valid(const char * history, const size_t length, node * tree) {
    PAN prefix = find_history(history, tree, length);
    while(prefix.second < length){
        prefix.first->child[chtoi(history[prefix.second])] = malloc(sizeof(node));
        if(prefix.first->child[chtoi(history[prefix.second])] == NULL) return 1;
        prefix.first->child[chtoi(history[prefix.second])]->parent = prefix.first;
        prefix.first = prefix.first->child[chtoi(history[prefix.second])];
        prefix.first->has_e = false;
        prefix.first->sign = history[prefix.second];
        prefix.first->right = prefix.first;
        prefix.first->left = prefix.first;
        for(uint8_t i = 0; i < ALPHABET_SIZE; ++i) {
            prefix.first->child[i] = NULL;
        }
        prefix.second++;
    }
    printf("OK\n");
    return 0;
}

void remove_valid(const char * history, const size_t length, node * tree) {
    PAN prefix = find_history(history, tree, length);
    if(length == prefix.second) {
        remove_subtree(prefix.first);
        prefix.first->parent->child[chtoi(prefix.first->sign)] = NULL;
        free(prefix.first);
    }
    printf("OK\n");
}

void check_valid(const char * history, const size_t length, node * tree) {
    PAN prefix = find_history(history, tree, length);
    printf("%s\n", get_bool_message(length == prefix.second));
}

void get_energy(const char * history, const size_t length, node * tree) {
    PAN prefix = find_history(history, tree, length);
    if(length == prefix.second && prefix.first->has_e) printf("%"PRIu64"\n", prefix.first->e);
    else manage_input_error();
}

void set_energy(const char * history, const size_t length, uint64_t energy, node * tree) {
    PAN prefix = find_history(history, tree, length);
    if(length == prefix.second){
        prefix.first->e = energy;
        prefix.first->has_e = true;
        node * iterator = prefix.first->right;
        while(iterator != prefix.first){
            iterator->e = energy;
            iterator = iterator->right;
        }
        printf("OK\n");
    } else manage_input_error();
}


void equal_energy(const char * history1, const size_t length1, char * history2, const size_t length2, node * tree) {
    PAN prefix1 = find_history(history1, tree, length1);
    PAN prefix2 = find_history(history2, tree, length2);
    if(prefix1.second != length1 || prefix2.second != length2){
        manage_input_error();
        return;
    }
    if(prefix1.first != prefix2.first) {
        if (prefix1.first->has_e && prefix2.first->has_e) {
            size_t new_energy = prefix1.first->e / 2 + prefix2.first->e / 2;
            if (prefix1.first->e % 2 == 1 && prefix2.first->e % 2 == 1) new_energy++;
            prefix1.first->left->right = prefix2.first->right;
            prefix2.first->right->left = prefix1.first->left;
            prefix1.first->left = prefix2.first;
            prefix2.first->right = prefix1.first;
            node *iterator = prefix1.first;
            do {
                iterator->e = new_energy;
                iterator = iterator->right;
            } while (iterator != prefix1.first);
        } else if (prefix1.first->has_e) {
            prefix2.first->e = prefix1.first->e;
            prefix2.first->has_e = true;
            prefix2.first->right = prefix1.first->right;
            prefix1.first->right->left = prefix2.first;
            prefix1.first->right = prefix2.first;
            prefix2.first->left = prefix1.first;
        } else if (prefix2.first->has_e) {
            prefix1.first->e = prefix2.first->e;
            prefix1.first->has_e = true;
            prefix1.first->right = prefix2.first->right;
            prefix2.first->right->left = prefix1.first;
            prefix2.first->right = prefix1.first;
            prefix1.first->left = prefix2.first;
        } else {
            manage_input_error();
            return;
        }
    }
    printf("OK\n");
}

bool init_tree(node ** tree) {
    (*tree) = malloc(sizeof(node));
    if((*tree) == NULL) return 1;
    (*tree)->has_e = false;
    (*tree)->left = (*tree);
    (*tree)->right = (*tree);
    for(uint8_t i = 0; i < ALPHABET_SIZE; ++i) {
        (*tree)->child[i] = NULL;
    }
    (*tree)->parent= NULL;
    return 0;
}
