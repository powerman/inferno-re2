implement T;

include "opt/powerman/tap/module/t.m";
include "re2.m";
	re2: Re2;
	re, RE: import re2;

test()
{
	re2 = load Re2 Re2->PATH;
	if(re2 == nil)
		bail_out("fail to load Re2");
	plan(24);

	s: string;
	n: int;

	(s,n) = re2->replace("abc", re(""), "B");
	eq_int(n, 1, nil);
	eq(s, "Babc", nil);

	(s,n) = re2->replace("", re("b"), "B");
	eq_int(n, 0, nil);
	eq(s, "", nil);

	(s,n) = re2->replace("", re(""), "B");
	eq_int(n, 1, nil);
	eq(s, "B", nil);

	{
	(s,n) = re2->replace("abc", ref RE(1,1), "B");
	} exception e { "*" => eq(e, "invalid argument", "exInval (fake RE)"); }

	(s,n) = re2->replace("abc", re("b"), "B");
	eq_int(n, 1, nil);
	eq(s, "aBc", nil);

	(s,n) = re2->replace("abc 123 def", re("\\s*(\\d+)\\s*"), "[\\1]");
	eq_int(n, 1, nil);
	eq(s, "abc[123]def", nil);

	(s,n) = re2->replace("abc 123 def", re("\\s*(\\d+)\\s*"), "\\0[\\1]");
	eq_int(n, 1, nil);
	eq(s, "abc 123 [123]def", nil);

	{
	(s,n) = re2->replace("abc 123 def", re("\\s*(\\d+)\\s*"), "\\3[\\1]\\2");
	} exception e { "*" => eq(e, "value out of range", "exRange (\\2)"); }
	{
	(s,n) = re2->replace("abc 123 def", re("\\s*\\d+\\s*"), "[\\a]");
	} exception e { "*" => eq(e, "value out of range", "exRange (\\a)"); }

	s = "  some &nbsp;spaces\t\r\n  &nbsp   WAS here!\n";
	(s,n) = re2->replace(s, re("&nbsp;?"), " ");
	eq_int(n, 1, nil);
	eq(s, "  some  spaces\t\r\n  &nbsp   WAS here!\n", nil);
	(s,n) = re2->replace(s, re("&nbsp;?"), " ");
	eq_int(n, 1, nil);
	eq(s, "  some  spaces\t\r\n      WAS here!\n", nil);
	(s,n) = re2->replace(s, re("&nbsp;?"), " ");
	eq_int(n, 0, nil);
	eq(s, "  some  spaces\t\r\n      WAS here!\n", nil);

	do{ (s,n) = re2->replace(s, re("[\\t\\r\\n]"), " "); }while(n);
	eq(s, "  some  spaces         WAS here! ", nil);
	do{ (s,n) = re2->replace(s, re("\\s\\s+"), " "); }while(n);
	eq(s, " some spaces WAS here! ", nil);
	s = re2->replace(s, re("^\\s+"), "").t0;
	s = re2->replace(s, re("\\s+$"), "").t0;
	eq(s, "some spaces WAS here!", nil);
}

