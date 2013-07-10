Utility for generating FNV-1a hashes of arbitrary strings.

If there are any command-line arguments, each argument is processed as a string
to be hashed.  If there are no command-line arguments, STDIN will be read
line-by-line until all input is exhausted (Ctrl-Z on Windows).  Alternatively,
pipes or stream redirection can be used to interface with files and other
programs, i.e:

myprogram --get-ids | idgen > hashed_ids.txt
idgen < ids.txt > hashed_ids.txt

Each string input will output a single line to STDOUT with the hash value of
the text entered.  The format is identical to that used by Id::to_string() with
Id names enabled:

#0000000000000000:Original Text Here

When a blank line is encountered when processing from STDIN, a blank line is
output to STDOUT (rather than hashing the empty string and outputting that).

If a hash collision occurs (the hash generated is the same as a different,
previously processed input string) a warning will be output to STDERR.
