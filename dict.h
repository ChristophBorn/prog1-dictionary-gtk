// Author: Christoph Born (53034, 22/041/62)
// Module dict: for managing dictionary & entry data
// requires #include <stdbool.h> !

// --------- data structures & constants ---------

typedef struct {
    char* wordDe;
    char* wordEn;
} tDEntry;

#define DICT_MAX_WORD_LEN 127 // determines buffer size where needed
#define DICT_DE 0
#define DICT_EN 1

// --------- API ---------

/* In all functions below, tList** dicts is expected to be tList *dicts[2],
   consisting of dicts[DICT_DE] and dicts[DICT_EN] (lists of tDEntry).
   Example initialization (#include "list.h"): tList *dicts[2] = {list_create(), list_create()};
     Dont forget to check whether one of then is NULL (= error)!
   Alphabetical order of dicts[DICT_DE] and dicts[DICT_EN] (in respective language)
     is established and preserved by the functions below.
   Iterate through the respective list and access the tDEntry's attributes
     to get the contents in german or english alphabetical order.
*/

// ====== entry manipulation ======

bool dict_insert(tList** dicts, char* wordDe, char* wordEn);
/* inserts entry into dicts
   - wordDe & wordEn are text to insert, strlen of both <= DICT_MAX_WORD_LEN respectively!
   - returns true on success, false otherwise */

bool dict_remove(tList** dicts, char* wordDe, char* wordEn);
/* finds & removes entry from dicts
   - wordDe & wordEn are text of the entry to remove
   - returns false if no such entry was found, true otherwise */

// ====== dict maintenance & search ======

tList* dict_search(tList** dicts, int lang, char* query);
/* returns dict (single list) of all entries that contain query in given language
   - lang is DICT_DE or DICT_EN
   - query is text to search for, comparison is performed case-insensitive
   - returns NULL on error */

void dict_free_search(tList* resDict);
/* deletes a list 'resDict' obtained with dict_search()
   - doesn't free corresponding tDEntrys since they may still be used by original dicts */

void dict_clear(tList** dicts);
/* removes all entries from dicts & frees corresponding tDEntrys
   - don't access search results generated from these entries afterwards! */

void dict_free(tList** dicts);
/* releases memory occupied by dicts (and corresponding tDEntrys)
   - execute on application exit!
   - don't access search results generated from these entries afterwards! */

// ====== file operations ======

bool dict_read_file(tList** dicts, char* fname);
/* reads entries from file into dicts
   - fname is name or path of the file
   - doesn't override existing entries, dict_clear() beforehand!
   - if an error occurs, dicts contain all entries read until then!
   - returns false on error, true otherwise */

bool dict_write_file(tList* dict, char* fname);
/* writes entries from dict to file (overriding existing content)
   - dict contains entries to be written, its order is used
     (in most cases that's dicts[DICT_DE] or dicts[DICT_EN])
   - fname is name or path of the file
   - if an error occurs, file contains all entries written until then!
   - returns false on error, true otherwise */