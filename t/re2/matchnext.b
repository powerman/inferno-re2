implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	re, StrPos: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(34);

	match: array of string;
	sp: ref StrPos;

	sp = ref StrPos("abrakadabra",0);
	match = re2->matchnext(sp, re("notsuch"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	match = re2->matchnext(sp, re("ab"));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, nil);
	eq_int(sp.pos, 2, "pos == 2");
	match = re2->matchnext(sp, re("ab"));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, nil);
	eq_int(sp.pos, 9, "pos == 9");
	match = re2->matchnext(sp, re("ab"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 9, "pos == 9");

	sp.pos = 0;
	match = re2->matchnext(sp, re("(a..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "abr", nil);
	eq_int(sp.pos, 3, "pos == 3");
	match = re2->matchnext(sp, re("(a..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "aka", nil);
	eq_int(sp.pos, 6, "pos == 6");
	match = re2->matchnext(sp, re("(a..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "abr", nil);
	eq_int(sp.pos, 10, "pos == 10");
	match = re2->matchnext(sp, re("(a..)"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 10, "pos == 10");

	sp = ref StrPos("abrakadabra",0);
	match = re2->matchnext(sp, re(""));
	ok(match != nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");
	match = re2->matchnext(sp, re(""));
	ok(match != nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	sp = ref StrPos("",0);
	match = re2->matchnext(sp, re("nosuch"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	sp = ref StrPos("abr",3);
	match = re2->matchnext(sp, re("r"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 3, "pos == 3");

	{
	sp = ref StrPos("abc",10);
	match = re2->matchnext(sp, re("nosuch"));
	} exception e { "*" => eq(e, "value out of range", "exRange"); }
	{
	sp = ref StrPos("abc",-10);
	match = re2->matchnext(sp, re("nosuch"));
	} exception e { "*" => eq(e, "value out of range", "exRange"); }
}

