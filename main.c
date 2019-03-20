#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <values.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

#include "history.h"
#include "utils.h"

#define DECLARE "DECLARE "
#define REMOVE "REMOVE "
#define VALID "VALID "
#define ENERGY "ENERGY "
#define EQUAL "EQUAL "
#define DECLARE_FLAG 1
#define REMOVE_FLAG 2
#define VALID_FLAG 3

/* Wczytuje energie z zadanego ciągu znaków od zadanego miejsca, zwraca 0 w przypadku sukcesu,
 * 1 w przypadku błędu (zbyt duża liczba)
 */
bool read_energy(size_t * idx, uint64_t * result, const char * line, const size_t length){
    *result = 0;
    while(*idx <= length && is_number(line[*idx])){
        if(*result <= UINT64_MAX/10) (*result) *= 10;
        else return 1;
        if(*result <= UINT64_MAX - chtoi(line[*idx])) (*result) += chtoi(line[*idx]);
        else return 1;
        (*idx)++;
    }
    if(*result == 0) return 1;
    return 0;
}

/* Wczytuje historie z zadanego ciągu znaków od zadanego miejsca, zwraca 0 w przypadku sukcesu,
 * 1 w przypadku błędu (historia długości zerowej)
 */
bool read_history(size_t * idx, char ** result, size_t * history_length, const char * line, const size_t length){
    size_t end = *idx;
    while(end <= length && is_alphabet_element(line[end])) end++;
    if(end == *idx) return 0;
    *result = calloc(end - *idx, sizeof(char));
    *history_length = end - *idx;
    if((*result) == NULL) return 1;
    size_t i = 0;
    while (i + *idx < end) {
        (*result)[i] = line[*idx + i];
        i++;
    }
    (*idx) += i;
    return 0;
}

/* Sprawdza, czy w zadanym ciągu znaków od pewnego indeksu występuje inny ciąg znaków
 */
bool compare_with(const char * line, const char * test, size_t * s_idx, const size_t line_length){
    *s_idx = 0;
    size_t test_length = strlen(test);
    if(test_length > line_length) return 0;
    while(*s_idx < test_length){
        if(line[*s_idx] != test[*s_idx]) return 0;
        (*s_idx)++;
    }
    return 1;
}

/* Obsługuje polecenia DECLARE, REMOVE i VALID, wczytuje argumenty i wywołuje funkcje na drzewie
 */
bool manage_one_argument(const char * line, const size_t length, node * tree, int type, char * test){
    size_t s_idx = 0;
    if(!compare_with(line, test, &s_idx, length)){
        manage_input_error();
        return 0;
    }
    char * history = NULL;
    size_t history_length = 0;
    if(read_history(&s_idx, &history, &history_length, line, length) == 1) return 1;
    if(history == NULL || history_length == 0 || line[s_idx] != '\n'){
        manage_input_error();
        if(history != NULL) free(history);
        return 0;
    }
    switch (type){
        case DECLARE_FLAG: add_valid(history, history_length, tree); break;
        case REMOVE_FLAG: remove_valid(history, history_length, tree); break;
        case VALID_FLAG: check_valid(history, history_length, tree); break;
        default: break;
    }
    free(history);
    return 0;
}

/* Obsługuje polecenia ENERGY (jedno i dwuargumentowe) wczytuje argumenty i wywołuje funkcje na drzewie
 */
bool manage_energy(const char * line, const size_t length, node * tree){
    size_t s_idx = 0;
    if(!compare_with(line, ENERGY, &s_idx, length)){
        manage_input_error();
        return 0;
    }
    char * history = NULL;
    size_t history_length = 0;
    if(read_history(&s_idx, &history, &history_length, line, length) == 1) return 1;
    if(history == NULL || history_length == 0){
        manage_input_error();
        if(history != NULL) free(history);
        return 0;
    }
    if(line[s_idx] == '\n'){
        get_energy(history, history_length, tree);
    } else if(line[s_idx] == ' '){
        s_idx++;
        uint64_t energy;
        if(read_energy(&s_idx, &energy, line, length) == 1 || line[s_idx] != '\n'){
            manage_input_error();
            if(history != NULL) free(history);
            return 0;
        }
        set_energy(history, history_length, energy, tree);
    } else{
        manage_input_error();
    }
    free(history);
    return 0;
}

/* Obsługuje polecenia EQUAL wczytuje argumenty i wywołuje funkcje na drzewie
 */
bool manage_equal(const char * line, const size_t length, node * tree){
    size_t s_idx = 0;
    if(!compare_with(line, EQUAL, &s_idx, length)){
        manage_input_error();
        return 0;
    }
    char * history1 = NULL;
    size_t history_length1 = 0;
    if(read_history(&s_idx, &history1, &history_length1, line, length) == 1) return 1;
    if(history1 == NULL || history_length1 == 0 || line[s_idx] != ' '){
        manage_input_error();
        if(history1 != NULL) free(history1);
        return 0;
    }
    s_idx++;
    char * history2 = NULL;
    size_t history_length2 = 0;
    if(read_history(&s_idx, &history2, &history_length2, line, length) == 1) {
        free(history1);
        return 1;
    }
    if(history2 == NULL || history_length2 == 0 || line[s_idx] != '\n'){
        manage_input_error();
        if(history1 != NULL) free(history1);
        if(history2 != NULL) free(history2);
        return 0;
    }
    equal_energy(history1, history_length1, history2, history_length2, tree);
    free(history1);
    free(history2);
    return 0;
}

/* Wczytuje wejście linia po linii, obsługuje komentarze i linie puste a następnie
 * wywołuje odopowiednie funkcje obsługujące polecenia
 */
bool read_line(node * tree){
    char * line = NULL;
    size_t length;

    if(getline(&line, &length, stdin) == -1) {
        free(line);
        return 1;
    }
    if(strcmp(line, "\n") == 0 || line[0] == '#'){
        free(line);
        return 0;
    }
    bool flag = false;
    if(length < 2){
        manage_input_error();
    } else if(line[0] == 'D' && line[1] == 'E'){
        flag = manage_one_argument(line, length, tree, DECLARE_FLAG, DECLARE);
    } else if(line[0] == 'R' && line[1] == 'E') {
        flag = manage_one_argument(line, length, tree, REMOVE_FLAG, REMOVE);
    } else if(line[0] == 'V' && line[1] == 'A'){
        flag = manage_one_argument(line, length, tree, VALID_FLAG, VALID);
    } else if(line[0] == 'E' && line[1] == 'N'){
        flag = manage_energy(line, length, tree);
    } else if(line[0] == 'E' && line[1] == 'Q'){
        flag = manage_equal(line, length, tree);
    } else {
        manage_input_error();
    }
    free(line);
    if(flag) return 1;
    return 0;
}

/* Funkcja główna programu, inicjalizuje drzewo znaków i obsługuje pętlę
 * z kolejnymi liniami wejścia
 */
int main() {
    node * a;
    init_tree(&a);
    while(read_line(a) != 1);
    clear_tree(&a);
    return 0;
}