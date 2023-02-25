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

    if(cmp == 0) {
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

    if(cmp == 0) {
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

    if(!list_insert_sorted(dicts[DICT_DE], tmp, cmp_de)
    || !list_insert_sorted(dicts[DICT_EN], tmp, cmp_en)) {
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
            if(i == 0 && (strcmp(tmp->wordDe, wordDe) == 0
                       || strcmp(tmp->wordEn, wordEn) == 0)) {
                target = tmp;
            }
            if(tmp == target) {                
                list_remove_curr(dicts[i]);
                break; // TODO does it break both loops?
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
    //char* word;
    char buf[DICT_MAX_WORD_LEN+1];
    //int i;

    //for(i=0; query[i] != 0; i++)  query[i] = tolower(query[i]);
    strcpytolower(lquery, query);

    for(tmp = list_get_first(dicts[lang]); tmp; tmp = list_get_next(dicts[lang])) {
        strcpytolower(buf, (lang == DICT_DE ? tmp->wordDe : tmp->wordEn));
        /*word = (lang == DICT_DE ? tmp->wordDe : tmp->wordEn);
        for(i=0; word[i] != 0; i++)  buf[i] = tolower(word[i]);
        buf[i+1] = 0;*/

        // word contains query (case insensitive)
        if(strstr(buf, lquery))  list_insert_last(res, tmp);
    }

    free(lquery);
    return res;
}

void dict_clear(tList* dict) {
    tDEntry* tmp;
    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        free_entry(tmp);

        list_remove_curr(dict);
    }
}
void dict_free(tList* dict) {
    dict_clear(dict);
    list_delete(dict);
}

bool dict_read_file(tList** dicts, char* fname) {
    char bufDe[DICT_MAX_WORD_LEN+1];
    char bufEn[DICT_MAX_WORD_LEN+1];
    bool ret = true;

    FILE* pf = fopen(fname, "rt");
    if(!pf)  return false;

    while(!feof(pf)) {
        if(!fgets(bufDe, DICT_MAX_WORD_LEN+1, pf)
        || !fgets(bufEn, DICT_MAX_WORD_LEN+1, pf))  {
            ret = false;
            break;
        }
        
        bufDe[strlen(bufDe) - 1] = 0;
        bufEn[strlen(bufEn) - 1] = 0;

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
    bool first = true;
    tDEntry* tmp;
    FILE* pf = fopen(fname, "wt");
    if(!pf)  return false;

    for(tmp = list_get_first(dict); tmp; tmp = list_get_next(dict)) {
        if(first) {
            first = false;
        }else{
            if(fprintf(pf, "\n") < 1) {
                ret = false;
                break;
            }
        }

        if(fprintf(pf, "%s\n%s", tmp->wordDe, tmp->wordEn) < 1){
            ret = false;
            break;
        }
    }

    fclose(pf);
    return ret;
}