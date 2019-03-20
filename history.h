//
// Created by adam on 07.03.19.
//
#include <stdint.h>
#define ALPHABET_SIZE 4

#ifndef MZ_HISTORY_H
#define MZ_HISTORY_H

typedef struct node_struct{
    uint64_t e;
    bool has_e;
    char sign;
    struct node_struct * parent;
    struct node_struct * child[ALPHABET_SIZE];
    struct node_struct * left;
    struct node_struct * right;
} node;

/* Funkcja dodaje do zadanego drzewa ścieżkę zadaną tablicą znaków alfabetu.
 * Jeśli dana ścieżka jest już w drzewie, nic nie robi.
 */
bool add_valid(const char * history, size_t length, node * tree);

/* Funkcja usuwa z drzewa poddrzewo ostatniego wierzchołka na ścieżce zadanej tablicą
 * znaków alfabetu, wraz z tym wierzchołkiem. Jeśli taka ścieżka w drzewie nie istnieje,
 * nic nie robi.
 */
void remove_valid(const char * history, size_t length, node * tree);

/* Funkcja sprawdza, czy w drzewie występuje ścieżka zadana tablicą znaków alfabetu,
 * odpowiednio wypisuje 'TAK' lub 'NIE'.
 */
void check_valid(const char * history, size_t length, node * tree);

/* Funkcja wypisuje energię przypisaną ostatniemu wierzchołkowi na ścieżce zadanej
 * tablicą znaków alfabetu. Jeśli takiej ścieżki nie ma w drzewie, lub jej ostatni
 * wierzchołek nie ma przypisanej energii, nic nie robi.
 */
void get_energy(const char * history, size_t length, node * tree);

/* Funkcja ustawi nową wartość energii ostatniemu wierzchołkowi na ścieżce zadanej
 * tablicą znaków alfabetu. Jeśli takiej ścieżki nie ma w drzewie, lub jej ostatni
 * wierzchołek nie ma przypisanej energii, nic nie robi.
 */
void set_energy(const char * history, size_t length, uint64_t energy, node * tree);

/* Funkcja zrównuje energię dwóch historii i łączy ich relacje równoważności. Jeżeli
 * któraś z historii nie jest dopuszczana, bądź są już w jednej relacji równoważności
 * to nic nie robi.
 */
void equal_energy(const char * history1, size_t length1, char * history2, size_t length2, node * tree);

/* Funkcja inicjuje drzewo znaków
 */
bool init_tree(node ** tree);

#endif //MZ_HISTORY_H
