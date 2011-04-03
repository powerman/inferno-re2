#include <re2/re2.h>
#include <stdio.h>
#include <malloc.h>

using namespace re2;

int main(void) {
	StringPiece word[10];
	RE2::Arg    argv[10];
	RE2::Arg*   args[10];
	int	    off[10], end[10];
	int i;
	int argc = 5;
	StringPiece s = "abcdef";
	RE2 pattern("(.*c)(.*z)?(Z*)(.*e)");
        for(i = 0; i < argc; i++){
            argv[i] = &word[i];
            args[i] = &argv[i];
        }
	if(pattern.Match(s, 0, s.size(), RE2::UNANCHORED, word, argc)) {
		printf("PASS\n");
		for(i = 0; i < argc; i++){
		    if(word[i] == NULL)
			printf("word[%d] == NULL\n", i);
		    if(word[i].data() == NULL)
			printf("word[%d].data() == NULL\n", i);
		    printf("word[%d].size() == %d\n", i, word[i].size());
		    off[i] = word[i].data() - s.data();
		    end[i] = off[i] + word[i].size();
		    printf("off = %d end = %d\n", off[i], end[i]);
		    char* str = (char*)malloc(word[i].size()+1);
		    std::copy(word[i].begin(), word[i].end(), str);
		    str[word[i].size()] = '\0';
		    printf("str = '%s'\n", str);
		}
		return 0;
	}
	printf("FAIL\n");
	return 2;
}
