
extern void* NewRE(const char*, int*);
extern void DeleteRE(void*);

extern int Match(const char*, const void*, int*, int*, int);
extern int Replace(const char*, const void*, const char*, char**);

