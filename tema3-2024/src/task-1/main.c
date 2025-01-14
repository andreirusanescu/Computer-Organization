#include <stdio.h>

extern int check_parantheses(char *str);

int main() {
	char s[1 << 16];
	scanf("%s", s);
	int r = check_parantheses(s);
	printf("%d\n", r);
	return 0;
}
