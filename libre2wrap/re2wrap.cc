#include <re2/re2.h>
#include <stdio.h>
#include <malloc.h>

using namespace re2;

extern "C"
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

extern "C"
void DeleteRE(RE2* pattern)
{
	delete pattern;
}

extern "C"
int Match(const char* text, const RE2* re, int off[], int end[], int argc)
{
	StringPiece s = text;
	StringPiece parens[argc];
	int match, i;

	match = re->Match(s, 0, RE2::UNANCHORED, parens, argc);

	if(match){
		for(i = 0; i < argc; i++){
			if(parens[i].data() == NULL){ // no match
				off[i] = -1;
				end[i] = -1;
			}else{ // match empty string if off[i]==end[i]
				off[i] = parens[i].data() - s.data();
				end[i] = off[i] + parens[i].size();
			}
		}
	}

	return match;
}

extern "C"
int Replace(const char* str, const RE2* re, const char* rewrite, char** res)
{
	string s;
	int replace;
	char *newstr;
	if(!re->CheckRewriteString(rewrite,&s))
		return -2;
	s = str;
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

extern "C"
int GlobalReplace(const char* str, const RE2* re, const char* rewrite, char** res)
{
	string s;
	int replace;
	char *newstr;
	if(!re->CheckRewriteString(rewrite,&s))
		return -2;
	s = str;
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

