implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	re, RE, StrPos: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(15);

	match: array of string;

	match = re2->match("abc", re("(A)"));
	ok(match == nil, "match == nil");

	match = re2->match("abc", re("(?-i:(A))"));
	ok(match == nil, "match == nil");

	match = re2->match("abc", re("(?i:(A))"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, "len match == 1");
	eq(match[0], "a", "match[0] == a");

	match = re2->match("a\nc", re("(a.c)"));
	ok(match == nil, "match == nil");

	match = re2->match("a\nc", re("(?-s:(a.c))"));
	ok(match == nil, "match == nil");

	match = re2->match("a\nc", re("(?s:(a.c))"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, "len match == 1");
	eq(match[0], "a\nc", "match[0] == a\\nc");

	match = re2->match("a\nc", re("(a$)"));
	ok(match == nil, "match == nil");

	match = re2->match("a\nc", re("(?-m:(a$))"));
	ok(match == nil, "match == nil");

	match = re2->match("a\nc", re("(?m:(a$))"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, "len match == 1");
	eq(match[0], "a", "match[0] == a");

}

