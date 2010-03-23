#include <re2/re2.h>
#include <stdio.h>
#include <malloc.h>
#include "re2wrap.h"

using namespace re2;

RE2* NewRE(const char* re, int* parens)
{
	RE2* pattern = new RE2(re);
	if(!pattern->error().empty()){
		delete pattern;
		return NULL;
	}
	*parens = pattern->NumberOfCapturingGroups();
	return pattern;
}

void DeleteRE(RE2* pattern)
{
	delete pattern;
}

int Match(const char* text, int pos, const RE2* re, Range r[], int n)
{
	StringPiece s = text;
	StringPiece parens[n];
	int match, i;

	match = re->Match(s, pos, RE2::UNANCHORED, parens, n);

	if(match){
		for(i = 0; i < n; i++){
			if(parens[i].data() == NULL){ // no match
				r[i].t0 = -1;
				r[i].t1 = -1;
			}else{ // match empty string if t0==t1
				r[i].t0 = parens[i].data() - s.data();
				r[i].t1 = r[i].t0 + parens[i].size();
			}
		}
	}

	return match;
}

int CheckRewriteString(const RE2* re, const char* rewrite)
{
	string s;
	return re->CheckRewriteString(rewrite, &s);
}

int Replace(const char* str, const RE2* re, const char* rewrite, char** res)
{
	string s = str;
	int replace;
	char *newstr;

	replace = RE2::Replace(&s, *re, rewrite);

	if(replace > 0){
		newstr = (char*)malloc(s.size()+1);
		if(newstr == NULL)
			return -1;
		std::copy(s.begin(), s.end(), newstr);
		newstr[s.size()] = '\0';
		*res = newstr;
	}
	return replace;
}

int GlobalReplace(const char* str, const RE2* re, const char* rewrite, char** res)
{
	string s = str;
	int replace;
	char *newstr;

	replace = RE2::GlobalReplace(&s, *re, rewrite);

	if(replace > 0){
		newstr = (char*)malloc(s.size()+1);
		if(newstr == NULL)
			return -1;
		std::copy(s.begin(), s.end(), newstr);
		newstr[s.size()] = '\0';
		*res = newstr;
	}
	return replace;
}

char* QuoteMeta(const char* unquoted)
{
	string s;
	char* quoted;
	
	s = RE2::QuoteMeta(unquoted);

	quoted = (char*)malloc(s.size()+1);
	if(quoted == NULL)
		return NULL;
	std::copy(s.begin(), s.end(), quoted);
	quoted[s.size()] = '\0';
	return quoted;
}

