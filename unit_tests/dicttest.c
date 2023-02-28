// Author: Christoph Born (53034, 22/041/62)
// unit test for module dict
// see runtest.sh (cmd: unit_tests/runtest.sh dicttest)
//  and dicttest-expected.log for expected output

#include <stdio.h>
#include <stdbool.h>
#include "../list.h"
#include "../dict.h"
#include "unit_test.h"

void print_dict(tList* dict, int lang) {
    // print table containing all entries from dict, lang (DICT_DE or DICT_EN) determines first column
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
    char* entryBig = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquy"; // > DICT_MAX_WORD_LEN
    char* entry1de = "Mitarbeiter";
    char* entry1en = "employee";
    char* entry2de = "Kartoffelsalat";
    char* entry2en = "potato salad";
    char* entry3de = "schwindend";
    char* entry3en = "dwindling";
    char* entry4de = "aussterbend"; // entry4en = entry3en
    char* entry5de = "versiegend"; // entry5en = entry3en
    char* entry6de = "Schwund"; // entry6en = entry3en

    tList* res;
    tList *dicts[2] = {list_create(), list_create()};
    if(!dicts[DICT_DE] || !dicts[DICT_EN]) {
        puts("list_create() failed!");
        return 1;
    }

    // test dict_insert()
    puts("=== insert ===");
    EVAL_INT_EXPR(dict_insert(dicts, entryBig, entry1en));
    EVAL_INT_EXPR(dict_insert(dicts, entry1de, entryBig));

    EVAL_INT_EXPR(dict_insert(dicts, entry1de, entry1en));
    EVAL_INT_EXPR(dict_insert(dicts, entry1de, entry1en));
    EVAL_INT_EXPR(dict_insert(dicts, entry2de, entry2en));
    EVAL_INT_EXPR(dict_insert(dicts, entry3de, entry3en));
    EVAL_INT_EXPR(dict_insert(dicts, entry4de, entry3en));
    EVAL_INT_EXPR(dict_insert(dicts, entry5de, entry3en));
    EVAL_INT_EXPR(dict_insert(dicts, entry6de, entry3en));

    // test iterating dicts[DICT_DE] & dicts[DICT_EN], accessing each tDEntry's attributes
    puts("=== list ===");
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    // test dict_remove()
    puts("=== remove ===");
    EVAL_INT_EXPR(dict_remove(dicts, entry1de, entry2en));
    EVAL_INT_EXPR(dict_remove(dicts, entry1de, entry1en));
    EVAL_INT_EXPR(dict_remove(dicts, entry3de, entry3en));

    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    // test dict_search()
    puts("=== search ===");
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, entry1en));
    if(res)  print_dict(res, DICT_DE), dict_free_search(res);

    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry1de));
    if(res)  print_dict(res, DICT_EN), dict_free_search(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, entry1de));
    if(res)  print_dict(res, DICT_DE), dict_free_search(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry1en));
    if(res)  print_dict(res, DICT_EN), dict_free_search(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_DE, "end"));
    if(res)  print_dict(res, DICT_DE), dict_free_search(res);
    
    EVAL_POINTER_EXPR(res = dict_search(dicts, DICT_EN, entry3en));
    if(res)  print_dict(res, DICT_EN), dict_free_search(res);
    
    // test dict_read_file() & dict_write_file()
    puts("=== file ===");
    EVAL_INT_EXPR(dict_write_file(dicts[DICT_DE], "/nopermission.dict"));
    EVAL_INT_EXPR(dict_read_file(dicts, "/nopermission.dict"));

    EVAL_INT_EXPR(dict_write_file(dicts[DICT_DE], "dicttest_assets/test.dict"));
    dict_clear(dicts);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/test.dict"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_clear(dicts);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/invalid1.dict"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_clear(dicts);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/invalid2.dict"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_clear(dicts);
    EVAL_INT_EXPR(dict_read_file(dicts, "dicttest_assets/invalid3.dict"));
    print_dict(dicts[DICT_DE], DICT_DE);
    puts("");
    print_dict(dicts[DICT_EN], DICT_EN);

    dict_free(dicts);

    return 0;
}