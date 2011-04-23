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

	sp = ref StrPos("абракадабра",0);
	match = re2->matchnext(sp, re("нету"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	match = re2->matchnext(sp, re("аб"));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, nil);
	eq_int(sp.pos, 4, "pos == 4");
	match = re2->matchnext(sp, re("аб"));
	ok(match != nil, "match != nil");
	eq_int(len match, 0, nil);
	eq_int(sp.pos, 18, "pos == 18");
	match = re2->matchnext(sp, re("аб"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 18, "pos == 18");

	sp.pos = 0;
	match = re2->matchnext(sp, re("(а..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "абр", nil);
	eq_int(sp.pos, 6, "pos == 6");
	match = re2->matchnext(sp, re("(а..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "ака", nil);
	eq_int(sp.pos, 12, "pos == 12");
	match = re2->matchnext(sp, re("(а..)"));
	ok(match != nil, "match != nil");
	eq_int(len match, 1, nil);
	eq(match[0], "абр", nil);
	eq_int(sp.pos, 20, "pos == 20");
	match = re2->matchnext(sp, re("(а..)"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 20, "pos == 20");

	sp = ref StrPos("абракадабра",0);
	match = re2->matchnext(sp, re(""));
	ok(match != nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");
	match = re2->matchnext(sp, re(""));
	ok(match != nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	sp = ref StrPos("",0);
	match = re2->matchnext(sp, re("нету"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 0, "pos == 0");

	sp = ref StrPos("абр",6);
	match = re2->matchnext(sp, re("р"));
	ok(match == nil, "match == nil");
	eq_int(sp.pos, 6, "pos == 6");

	{
	sp = ref StrPos("абв",10);
	match = re2->matchnext(sp, re("нету"));
	} exception e { "*" => eq(e, "value out of range", "exRange"); }
	{
	sp = ref StrPos("абв",-10);
	match = re2->matchnext(sp, re("нету"));
	} exception e { "*" => eq(e, "value out of range", "exRange"); }
}

