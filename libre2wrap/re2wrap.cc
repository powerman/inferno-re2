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
int PartialMatchN(const char* text, const RE2* re, char* c_args[], int argc)
{
	std::string word[argc];
	RE2::Arg    argv[argc];
	RE2::Arg*   args[argc];
	int match;
	int i;

	for(i = 0; i < argc; i++){
	    argv[i] = &word[i];
	    args[i] = &argv[i];
	}

	match = RE2::PartialMatchN(text, *re, args, argc);

	if(match)
	    for(i = 0; i < argc; i++){
		c_args[i] = (char*)malloc(word[i].size()+1);
		if(c_args[i] == NULL)
		    return -1;
		std::copy(word[i].begin(), word[i].end(), c_args[i]);
		c_args[i][word[i].size()] = '\0';
	    }

	return match;
}

extern "C"
int Replace(char* str, const RE2* re, const char* rewrite)
{
}

