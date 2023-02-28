// Author: Christoph Born (53034, 22/041/62)
// unit test for module list
// see runtest.sh (cmd: unit_tests/runtest.sh listtest)
//  and listtest-expected.log for expected output

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../list.h"
#include "unit_test.h"

int compare(void* str1, void* str2) {
    // wrapper for strcmp(str1, str2): to return int <0 if str1 is alphabetically before str2, ...
    return strcmp((char*) str1, (char*) str2);
}
int compare_rev(void* str1, void* str2) {
    // wrapper for strcmp(str1, str2) with reversed sign
    return compare(str1, str2) * -1;
}

bool print_entry(tList* pList, void* entry, int i) {
    // prints given entry and i (index) as string & returns true (pList unused)
    printf("(%d) %s, ", i, (char*) entry);
    return true;
}
void print_list(tList* pList) {
    // prints all entries of pList into one line
    list_foreach(pList, print_entry);
    puts("");
}

int main() {
    char* textA = "Richard";
    char* textB = "Olaf";
    char* textC = "Anna";
    char* textD = "Ulrike";
    char* textE = "Perry";
    char* tmp;

    tList* pList;

    // test list_create()
    EVAL_POINTER_EXPR(pList = list_create());
    if(!pList)  return 1;

    // test list_insert_first() & list_insert_last()
    puts("=== insert1 ===");
    EVAL_INT_EXPR(list_insert_first(pList, textA));
    EVAL_INT_EXPR(list_insert_first(pList, textB));
    EVAL_INT_EXPR(list_insert_last(pList, textC));
    EVAL_INT_EXPR(list_insert_last(pList, textD));
    EVAL_INT_EXPR(list_insert_last(pList, textE));

    // test list_get_curr(), list_get_prev(), list_get_next(), list_get_first(), list_get_last() & list_get_at()
    puts("=== get single ===");
    EVAL_STRING_EXPR((char*) list_get_curr(pList));
    EVAL_STRING_EXPR((char*) list_get_prev(pList));
    EVAL_STRING_EXPR((char*) list_get_next(pList));

    EVAL_STRING_EXPR((char*) list_get_first(pList));
    EVAL_STRING_EXPR((char*) list_get_last(pList));
    
    EVAL_STRING_EXPR((char*) list_get_at(pList, 0));
    EVAL_STRING_EXPR((char*) list_get_at(pList, 2));
    EVAL_STRING_EXPR((char*) list_get_at(pList, 4));

    // test iterating using list_get_first(), list_get_next() / list_get_last(), list_get_prev()
    puts("=== get loop forwards ===");
    for(tmp = list_get_first(pList); tmp; tmp = list_get_next(pList)) {
        printf("%s\n", tmp);
    }

    puts("=== get loop backwards ===");
    for(tmp = list_get_last(pList); tmp; tmp = list_get_prev(pList)) {
        printf("%s\n", tmp);
    }

    // test list_delete(), list_remove_curr(), list_remove_prev(), list_remove_next(), list_remove_first() & list_remove_last()
    puts("=== remove1 ===");
    EVAL_INT_EXPR(list_delete(pList));

    EVAL_STRING_EXPR((char*) list_get_at(pList, 1));
    EVAL_INT_EXPR(list_remove_next(pList)); print_list(pList);
    EVAL_INT_EXPR(list_remove_prev(pList)); print_list(pList);
    EVAL_INT_EXPR(list_remove_curr(pList)); print_list(pList);

    EVAL_INT_EXPR(list_remove_next(pList));
    EVAL_INT_EXPR(list_remove_prev(pList));
    EVAL_INT_EXPR(list_remove_curr(pList));

    EVAL_INT_EXPR(list_remove_first(pList)); print_list(pList);
    EVAL_INT_EXPR(list_remove_last(pList));

    EVAL_INT_EXPR(list_remove_first(pList));
    EVAL_INT_EXPR(list_remove_last(pList));

    EVAL_INT_EXPR(list_delete(pList));

    // test list_insert_before(), list_insert_behind() & removal loop
    puts("=== insert2 ===");
    EVAL_POINTER_EXPR(pList = list_create());
    if(!pList)  return 1;

    EVAL_INT_EXPR(list_insert_before(pList, textA));
    EVAL_INT_EXPR(list_insert_behind(pList, textB));

    EVAL_STRING_EXPR((char*) list_get_first(pList));
    EVAL_INT_EXPR(list_insert_before(pList, textA));
    EVAL_INT_EXPR(list_insert_behind(pList, textB));

    EVAL_STRING_EXPR((char*) list_get_last(pList));
    EVAL_INT_EXPR(list_insert_before(pList, textC));
    EVAL_INT_EXPR(list_insert_behind(pList, textD));

    for(tmp = list_get_first(pList); tmp; tmp = list_get_next(pList)) {
        EVAL_INT_EXPR(list_remove_prev(pList));
        printf("%s\n", tmp);
    }
    EVAL_INT_EXPR(list_remove_last(pList));
    EVAL_INT_EXPR(list_remove_first(pList));

    EVAL_INT_EXPR(list_delete(pList));

    // test list_insert_sorted() using compare()
    puts("=== insert sorted ===");
    EVAL_POINTER_EXPR(pList = list_create());
    if(!pList)  return 1;

    EVAL_INT_EXPR(list_insert_sorted(pList, textA, compare));
    EVAL_INT_EXPR(list_insert_sorted(pList, textB, compare));
    EVAL_STRING_EXPR((char*) list_get_first(pList));
    EVAL_INT_EXPR(list_insert_sorted(pList, textC, compare));
    EVAL_STRING_EXPR((char*) list_get_curr(pList));
    EVAL_INT_EXPR(list_insert_sorted(pList, textD, compare));

    for(tmp = list_get_first(pList); tmp; tmp = list_get_next(pList)) {
        EVAL_INT_EXPR(list_remove_prev(pList));
        printf("%s\n", tmp);
    }
    EVAL_INT_EXPR(list_remove_last(pList));
    EVAL_INT_EXPR(list_remove_first(pList));

    EVAL_INT_EXPR(list_delete(pList));

    // test list_insert_sorted() using compare_rev()
    puts("=== insert sorted reverse ===");
    EVAL_POINTER_EXPR(pList = list_create());
    if(!pList)  return 1;

    EVAL_INT_EXPR(list_insert_sorted(pList, textA, compare_rev));
    EVAL_INT_EXPR(list_insert_sorted(pList, textB, compare_rev));
    EVAL_STRING_EXPR((char*) list_get_first(pList));
    EVAL_INT_EXPR(list_insert_sorted(pList, textC, compare_rev));
    EVAL_STRING_EXPR((char*) list_get_curr(pList));
    EVAL_INT_EXPR(list_insert_sorted(pList, textD, compare_rev));

    for(tmp = list_get_first(pList); tmp; tmp = list_get_next(pList)) {
        EVAL_INT_EXPR(list_remove_prev(pList));
        printf("%s\n", tmp);
    }
    EVAL_INT_EXPR(list_remove_last(pList));
    EVAL_INT_EXPR(list_remove_first(pList));

    EVAL_INT_EXPR(list_delete(pList));

    return 0;
}