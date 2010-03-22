Re2: module
{
	PATH: con "$Re2";

	re: fn(re: string): ref RE;				# RE2::RE
	match: fn(s: string, re: ref RE, parens: array of string): int;# RE2::PartialMatchN

# internal
	RE: adt
	{
		parens: int;	
		# there are some hidden state lurking around
	};

};
