//
// Created by adam on 10.03.19.
//
#include <stdbool.h>
#include <stdint.h>
#include "history.h"

#ifndef MZ_UTILS_H
#define MZ_UTILS_H

/* Konwertuje kod cyfry w formacie ASCII na liczbę
 */
int32_t chtoi(int32_t ch);

/* Sprawdza, czy dany znak zapisany w kodzie ASCII należy do alfabetu
 * nad którym tworzone są historie
 */
bool is_alphabet_element(int32_t ch);

/* Sprawdza, czy dany znak w kodzie ASCII reprezentuje cyfrę
 */
bool is_number(int32_t ch);

/* Obsługuje błędy wykonania, zwalnia pamięć i zatrzymuje program
 */
void clear_tree(node ** tree);

/* Zwraca ciągi znaków odpowiadające wartościom logicznym
 */
const char * get_bool_message(bool b);

/* Obsługuje błędy wejścia, wypisuje komunikat o błędzie
 */
void manage_input_error(void);

#endif //MZ_UTILS_H
