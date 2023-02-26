/*
    AUTHOR
    Dictionary utility functions
    requires #include <stdbool.h>
*/

// === data structures ===
typedef struct {
    char* wordDe;
    char* wordEn;
} tDEntry;

#define DICT_MAX_WORD_LEN 128
#define DICT_DE 0
#define DICT_EN 1

// === API ===
// tList** dicts is expected to be tList *dicts[2],
// consisting of dicts[DICT_DE] and dicts[DICT_EN] (lists of tDEntry)
// alphabetical order of which is preserved by all functions below

bool dict_insert(tList** dicts, char* wordDe, char* wordEn); // inserts entry, mind DICT_MAX_WORD_LEN!
bool dict_remove(tList** dicts, char* wordDe, char* wordEn); // removes entry, returns false if not found
tList* dict_search(tList** dicts, int lang, char* query); // returns dict of all entries that contain query in given language, dict_free_search after use!, returns NULL on error
void dict_free_search(tList* resDict);
void dict_clear(tList** dicts); // remove all entries, don't access search results generated from these entries afterwards!
void dict_free(tList** dicts); // releases memory occupied by dicts (and content), execute on program exit! Don't access search results generated from these dicts afterwards!

bool dict_read_file(tList** dicts, char* fname); // doesn't override existing entries, dict_clear beforehand!
bool dict_write_file(tList* dict, char* fname); // uses order of given dict