#include <stdio.h>

int a[8] = {0, 3, 5};
int count = 3;

void parr(void)
{
	for (size_t i = 0; i < 8; i++) {
		printf("%d\t", a[i]);
	}
	printf("\n");
}

void compress(int i)
{
	count--;
	a[i] = a[count];
	a[count] = -18;
	parr();
}

void close_index(int i)
{
	if (i >= 8)
		return;
	a[i] = -1;
	parr();
}

int main(int argc, const char *argv[])
{

	printf("");
	parr();

	close_index(1);
	compress(1);

	(void)argc;
	(void)argv;
	return 0;
}
