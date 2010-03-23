
#ifdef __cplusplus
#	define EXTERN	extern "C"
#	define RE2	RE2
#else
#	define EXTERN	extern
#	define RE2	void
#endif

typedef struct Range Range;
struct Range {
	int t0;
	int t1;
};

EXTERN RE2* NewRE(const char*, int*);
EXTERN void DeleteRE(RE2*);
EXTERN int CheckRewriteString(const RE2*, const char*);

EXTERN int Match(const char*, const RE2*, Range*, int);
EXTERN int Replace(const char*, const RE2*, const char*, char**);
EXTERN int GlobalReplace(const char*, const RE2*, const char*, char**);

