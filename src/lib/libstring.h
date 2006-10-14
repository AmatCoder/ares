/*
  libstring : version 0.12 ~byuu (10/05/06)
*/

#ifndef __LIBSTRING
#define __LIBSTRING

#include "libvector.h"

class string;
typedef linear_vector<string> stringarray;

char  chrlower(char c);
char  chrupper(char c);

uint  count(stringarray &str);

void  strresize(string &str, uint size);

char* strptr(string &str);

uint  strlen(string &str);

int   strcmp(string &dest, const char *src);
int   strcmp(const char *dest, string &src);
int   strcmp(string &dest, string &src);

//vc6/win32 and gcc/dos only support stricmp, whereas
//gcc/unix only supports strcasecmp. this is an attempt
//to avoid platform-specific defines...
#define stricmp __stricmp
int   __stricmp(const char *dest, const char *src);
int   stricmp(string &dest, const char *src);
int   stricmp(const char *dest, string &src);
int   stricmp(string &dest, string &src);

bool  strmatch(const char *dest, const char *src);
bool  strmatch(string &dest, const char *src);
bool  strmatch(const char *dest, string &src);
bool  strmatch(string &dest, string &src);

bool  strimatch(const char *dest, const char *src);
bool  strimatch(string &dest, const char *src);
bool  strimatch(const char *dest, string &src);
bool  strimatch(string &dest, string &src);

void  strcpy(string &dest, const char *src);
void  strcpy(string &dest, string &src);
void  strncpy(string &dest, const char *src, uint32 length);
void  strncpy(string &dest, string &src, uint32 length);

void  strset(string &dest, uint pos, uint8 c);

void  strcat(string &dest, const char src);
void  strcat(string &dest, const char *src);
void  strcat(string &dest, string &src);

void  strinsert(string &dest, const char *src, uint pos);
void  strinsert(string &dest, string &src, uint pos);

void  strremove(string &dest, uint start, uint length = 0);

char *strlower(char *str);
string &strlower(string &str);

char *strupper(char *str);
string &strupper(string &str);

bool  strpos(const char *str, const char *key, uint &pos);
bool  strpos(string &str, const char *key, uint &pos);
bool  strpos(const char *str, string &key, uint &pos);
bool  strpos(string &str, string &key, uint &pos);

bool  qstrpos(const char *str, const char *key, uint &pos);
bool  qstrpos(string &str, const char *key, uint &pos);
bool  qstrpos(const char *str, string &key, uint &pos);
bool  qstrpos(string &str, string &key, uint &pos);

void  strtr(char *dest, const char *before, const char *after);
void  strtr(string &dest, const char *before, const char *after);

bool  strbegin(const char *str, const char *key);
bool  strbegin(string &str, const char *key);

bool  stribegin(const char *str, const char *key);
bool  stribegin(string &str, const char *key);

bool  strend(const char *str, const char *key);
bool  strend(string &str, const char *key);

bool  striend(const char *str, const char *key);
bool  striend(string &str, const char *key);

void  strltrim(char *str, const char *key);
void  strltrim(string &str, const char *key);

void  striltrim(char *str, const char *key);
void  striltrim(string &str, const char *key);

void  strrtrim(char *str, const char *key);
void  strrtrim(string &str, const char *key);

void  strirtrim(char *str, const char *key);
void  strirtrim(string &str, const char *key);

void  strquote(string &str);

bool  strunquote(char *str);
bool  strunquote(string &str);

uint  strhex(const char *str);
uint  strhex(string &str);

int   sstrhex(const char *str);
int   sstrhex(string &str);

uint  strdec(const char *str);
uint  strdec(string &str);

int   sstrdec(const char *str);
int   sstrdec(string &str);

uint  strbin(const char *str);
uint  strbin(string &str);

int   sstrbin(const char *str);
int   sstrbin(string &str);

char *utoa(char *str, uint num);
string &utoa(string &str, uint num);

char *itoa(char *str, uint num);
string &itoa(string &str, uint num);

char *htoa(char *str, uint num);
string &htoa(string &str, uint num);

char *uhtoa(char *str, uint num);
string &uhtoa(string &str, uint num);

char *btoa(char *str, uint num);
string &btoa(string &str, uint num);

bool  strfread(string &str, const char *filename);

int   strmath(const char *in_str);
int   strmath(string &in_str);

string &replace(string &str, const char *key, const char *token);
string &replace(string &str, const char *key, string &token);

string &qreplace(string &str, const char *key, const char *token);
string &qreplace(string &str, const char *key, string &token);

void  split(stringarray &dest, const char *key, const char *src);
void  split(stringarray &dest, const char *key, string &src);

void  qsplit(stringarray &dest, const char *key, const char *src);
void  qsplit(stringarray &dest, const char *key, string &src);

uint  vsprintf(string &str, const char *s, va_list args);
uint  sprintf(string &str, const char *s, ...);

class string {
public:
char *s;
uint size;
  string() {
    size = 16;
    s = (char*)malloc(size + 1);
    *s = 0;
  }

  ~string() { SafeFree(s); }

  void operator=(const string &p) {
    SafeFree(s);
    size = p.size;
    s = (char*)malloc(size + 1);
    strcpy(s, p.s);
  }
};

#endif //__LIBSTRING
