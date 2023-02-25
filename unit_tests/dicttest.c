#include <stdio.h>
#include <stdbool.h>
#include "unit_test.h"
#include "../list.h"
#include "../dict.h"

void print_dict(tList* dict, int lang) {
    tDEntry* tmp;
    
    printf(
        "%-20s | %-20s\n",
        (lang == DICT_DE ? "deutsch" : "englisch"),
        (lang != DICT_DE ? "deutsch" : "englisch")
    );
    printf("==================== | ====================\n");
    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        printf(
            "%-20s | %-20s\n",
            (lang == DICT_DE ? tmp->wordDe : tmp->wordEn),
            (lang != DICT_DE ? tmp->wordDe : tmp->wordEn)
        );    
    }
}

int main() {
    char* entryBig = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquya";
    char* entry1a = "Mitarbeiter";
    char* entry1b = "employee";
    char* entry2a = "Kartoffelsalat";
    char* entry2b = "potato salad";
    char* entry3a = "schwindend";
    char* entry3b = "dwindling";
    char* entry4 = "aussterbend";
    char* entry5 = "versiegend";
    char* entry6 = "Schwund";

    tList *dicts[2] = {list_create(), list_create()};

    tList* res;

    puts("=== insert ===");
    EVAL_INT_EXPR(dict_insert(dicts, entryBig, entry1b));
    EVAL_INT_EXPR(dict_insert(dicts, entry1a, entryBig));

    EVAL_INT_EXPR(dict_insert(dicts, entry1a, entry1b));
    EVAL_INT_EXPR(dict_insert(dicts, entry1a, entry1b));
    EVAL_INT_EXPR(dict_insert(dicts, entry2a, entry2b));
    EVAL_INT_EXPR(dict_insert(dicts, entry3a, entry3b));
    EVAL_INT_EXPR(dict_insert(dicts, entry4, entry3b));
    EVAL_INT_EXPR(dict_insert(dicts, entry5, entry3b));
    EVAL_INT_EXPR(dict_insert(dicts, entry6, entry3b));


    puts("=== list ===");
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    puts("=== remove ===");
    EVAL_INT_EXPR(dict_remove(dicts, entry1a, entry2b));
    EVAL_INT_EXPR(dict_remove(dicts, entry1a, entry1b));
    EVAL_INT_EXPR(dict_remove(dicts, entry3a, entry3b));

    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    puts("=== search ===");
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, entry1b));
    if(res)  print_dict(res, DICT_DE), dict_free(res);

    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry1a));
    if(res)  print_dict(res, DICT_EN), dict_free(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, entry1a));
    if(res)  print_dict(res, DICT_DE), dict_free(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry1b));
    if(res)  print_dict(res, DICT_EN), dict_free(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, "end"));
    if(res)  print_dict(res, DICT_DE), dict_free(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry3b));
    if(res)  print_dict(res, DICT_EN), dict_free(res);
    

    puts("=== file ===");
    EVAL_INT_EXPR(dict_write_file(dicts[DICT_DE], "/noperm.txt"));
    EVAL_INT_EXPR(dict_read_file(dicts, "/noperm.txt"));

    EVAL_INT_EXPR(dict_write_file(dicts[DICT_DE], "dicttest_assets/dict.txt"));
    dict_clear(dicts[DICT_DE]);
    dict_clear(dicts[DICT_EN]);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/dict.txt"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_clear(dicts[DICT_DE]);
    dict_clear(dicts[DICT_EN]);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/invalid.txt"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_free(dicts[DICT_DE]);
    dict_free(dicts[DICT_EN]);

    return 0;
}