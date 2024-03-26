// gcc -o test_memmove test_memmove.c
#include <stdio.h>
#include <string.h>

void* memcpy_(void *dst, const void *src, int n) {
	char *tmp = (char*)dst;
	char *s_src = (char*)src;

	while(n--) {
		*tmp++ = *s_src++;
	}

	return dst;
}

void* memcpy__(void *dst, const void *src, int n) {
	char *tmp = (char*)dst;
	char *s_src = (char*)src;
	char *s_dst = (char*)dst;

	if (s_dst > s_src && s_src + n > s_dst) {
		s_dst = s_dst + n - 1;
		s_src = s_src + n - 1;
		while (n--) {
			*s_dst-- = *s_src--;
		}
	} else {
	
	while(n--) {
		*tmp++ = *s_src++;
	}
	}

	return dst;
}

int main() {
	char str[32] = "abcdefg123456";
	int len = strlen(str);

#if 1
	memcpy__(str + 1, str, len);
	printf("%s", str + 1);
#else
	memmove(str + 1, str, len);
	printf("%s", str + 1);
#endif

	return 0;
}

