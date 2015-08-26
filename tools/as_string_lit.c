#include <ctype.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	if (argc != 1) {
		fprintf(stderr, "usage: %s < input-file > output-string-literal", argv[0]);
	}

	puts("/* generated by ccan/tools/as_string_lit */");
	putchar('"');
	char buf[1024];
	for (;;) {
		size_t r = fread(buf, 1, sizeof(buf), stdin);
		if (r == 0) {
			puts("\"");
			return 0;
		}

		size_t i;
		for (i = 0; i < r; i++) {
			char c = buf[i];
			if (c == '\n')
				/* special case newlines to emit another newline */
				fputs("\\n\"\n\"", stdout);
			/* handle required escapes */
			else if (c == '\\')
				fputs("\\\\", stdout);
			else if (c == '\"')
				fputs("\\\"", stdout);
			else if (isalnum(c) || isblank(c) || ispunct(c))
				/* things we can probably print directly */
				putchar(c);
			else
				/* everything else is encoded as octal */
				printf("\\%03o", c);
		}
	}
}