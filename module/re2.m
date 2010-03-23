Re2: module
{
	PATH: con "$Re2";

	# regex(2) compatible interface
#	compile:	fn(e: string, flag: int): (RE, string);
#	execute:	fn(x: RE; s: string): array of (int,int);
#	executese:	fn(x: RE, s: string, se: (int, int), bol: int, eol: int): array of (int, int);

	# user-friendly interface
	re:		fn(re: string): ref RE; # throw on bad regexp
	match:		fn(s: string, re: ref RE): array of string;
#	matchnext:	fn(s: string, re: ref RE, pos: int): array of string;
	replace:	fn(s: string, re: ref RE, rewrite: string): (string, int);
	replaceall:	fn(s: string, re: ref RE, rewrite: string): (string, int);
#	quotemeta:	fn(s: string): string;

	RE: adt
	{
		parens:	int;  # auto-detected amount of capturing parens
		capture:int; # flag: if false make all parens non-capturing
		# there are some hidden state lurking around
	};
};
