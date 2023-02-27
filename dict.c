// Author: Christoph Born (53034, 22/041/62)
// Module dict: for managing dictionary & entry data

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "list.h"

#include "dict.h"

/*private*/ void free_entry(tDEntry* entry) {
    free(entry->wordDe);
    free(entry->wordEn);
    free(entry);
}

/*private*/ void strcpytolower(char* dest, char* src) {
    int i;
    for(i=0; src[i]; i++)  dest[i] = tolower(src[i]);
    dest[i] = 0;
}

/*private*/ int cmp_de(void* entry1, void* entry2) {
    int cmp;
    char buf1[DICT_MAX_WORD_LEN+1];
    char buf2[DICT_MAX_WORD_LEN+1];

    strcpytolower(buf1, ((tDEntry*) entry1)->wordDe);
    strcpytolower(buf2, ((tDEntry*) entry2)->wordDe);
    cmp = strcmp(buf1, buf2);

    if(cmp == 0) { // english order if equal
        strcpytolower(buf1, ((tDEntry*) entry1)->wordEn);
        strcpytolower(buf2, ((tDEntry*) entry2)->wordEn);
        cmp = strcmp(buf1, buf2);
    }

    return cmp;
}
/*private*/ int cmp_en(void* entry1, void* entry2) {
    int cmp;
    char buf1[DICT_MAX_WORD_LEN+1];
    char buf2[DICT_MAX_WORD_LEN+1];

    strcpytolower(buf1, ((tDEntry*) entry1)->wordEn);
    strcpytolower(buf2, ((tDEntry*) entry2)->wordEn);
    cmp = strcmp(buf1, buf2);

    if(cmp == 0) { // german order if equal
        strcpytolower(buf1, ((tDEntry*) entry1)->wordDe);
        strcpytolower(buf2, ((tDEntry*) entry2)->wordDe);
        cmp = strcmp(buf1, buf2);
    }

    return cmp;
}

bool dict_insert(tList** dicts, char* wordDe, char* wordEn) {
    if(strlen(wordDe) > DICT_MAX_WORD_LEN
    || strlen(wordEn) > DICT_MAX_WORD_LEN)  return false;

    tDEntry* tmp = malloc(sizeof(tDEntry));
    if(!tmp)  return false;

    tmp->wordDe = malloc(strlen(wordDe) + 1);
    tmp->wordEn = malloc(strlen(wordEn) + 1);
    if(!tmp->wordDe || !tmp->wordEn) {
        free_entry(tmp);
        return false;
    }
    
    strcpy(tmp->wordDe, wordDe);
    strcpy(tmp->wordEn, wordEn);

    if(!list_insert_sorted(dicts[DICT_DE], tmp, cmp_de)) {
        free_entry(tmp);
        return false;
    }
    if(!list_insert_sorted(dicts[DICT_EN], tmp, cmp_en)) {
        dict_remove(dicts, tmp->wordDe, tmp->wordEn);
        free_entry(tmp);
        return false;
    }

    return true;
}

bool dict_remove(tList** dicts, char* wordDe, char* wordEn) {
    tDEntry* tmp;
    tDEntry* target = NULL;
    int i;
    for(i=0; i<2; i++) {
        for(tmp = list_get_first(dicts[i]); tmp; tmp = list_get_next(dicts[i])) {
            if(i == 0 && strcmp(tmp->wordDe, wordDe) == 0
                      && strcmp(tmp->wordEn, wordEn) == 0) {
                target = tmp;
            }
            if(tmp == target) {                
                list_remove_curr(dicts[i]);
                break;
            }
        }
        
        if(i == 0 && !target)  return false;
    }
    
    free_entry(target);
    return true;
}

tList* dict_search(tList** dicts, int lang, char* query) {
    char* lquery = malloc(strlen(query) + 1);
    if(!lquery)  return NULL;
    
    tList* res = list_create();
    tDEntry* tmp;
    char buf[DICT_MAX_WORD_LEN+1];

    strcpytolower(lquery, query);

    for(tmp = list_get_first(dicts[lang]); tmp; tmp = list_get_next(dicts[lang])) {
        strcpytolower(buf, (lang == DICT_DE ? tmp->wordDe : tmp->wordEn));

        // word contains query (case insensitive)
        if(strstr(buf, lquery))  list_insert_last(res, tmp);
    }

    free(lquery);
    return res;
}

/*private*/ void delete_dict(tList* dict, bool freeEnries) {
    tDEntry* tmp;
    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        list_remove_prev(dict); // list_remove_curr() would make list_get_next() fail
        
        if(freeEnries) free_entry(tmp);
    }
    list_remove_last(dict);
}
void dict_free_search(tList* resDict) {
    delete_dict(resDict, false); // can't free tDEntrys since still used by original dicts
    list_delete(resDict);
}
void dict_clear(tList** dicts) {
    delete_dict(dicts[DICT_DE], false); // tDEntrys can only be freed once for both dicts!
    delete_dict(dicts[DICT_EN], true);
}
void dict_free(tList** dicts) {
    dict_clear(dicts);

    list_delete(dicts[DICT_DE]);
    list_delete(dicts[DICT_EN]);
}

bool dict_read_file(tList** dicts, char* fname) {
    char bufDe[DICT_MAX_WORD_LEN+2];
    char bufEn[DICT_MAX_WORD_LEN+2];
    bool ret = true;

    FILE* pf = fopen(fname, "rt");
    if(!pf)  return false;

    while(true) {
        if(!fgets(bufDe, DICT_MAX_WORD_LEN+2, pf)) {
            ret = feof(pf); // stop at EOF before entry, otherwise error
            break;
        }
        if(!fgets(bufEn, DICT_MAX_WORD_LEN+2, pf)) {
            ret = false;
            break;
        }
        
        if((strlen(bufDe) == DICT_MAX_WORD_LEN+1 && bufDe[DICT_MAX_WORD_LEN] != '\n')
        || (strlen(bufEn) == DICT_MAX_WORD_LEN+1 && bufEn[DICT_MAX_WORD_LEN] != '\n')) {
            ret = false; // line > DICT_MAX_WORD_LEN
            break;
        }

        // Remove line break
        bufDe[strlen(bufDe) - 1] = 0;
        if(bufEn[strlen(bufEn) - 1] == '\n')  bufEn[strlen(bufEn) - 1] = 0;

        if(strlen(bufDe) < 1 || strlen(bufEn) < 1) {
            ret = false; // empty line
            break;
        }

        if(!dict_insert(dicts, bufDe, bufEn)) {
            ret = false;
            break;
        }
    }

    fclose(pf);
    return ret;
}

bool dict_write_file(tList* dict, char* fname) {
    bool ret = true;
    tDEntry* tmp;
    FILE* pf = fopen(fname, "wt");
    if(!pf)  return false;

    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        if(fprintf(pf, "%s\n%s\n", tmp->wordDe, tmp->wordEn) < 4) {
            ret = false;
            break;
        }
    }

    fclose(pf);
    return ret;
}