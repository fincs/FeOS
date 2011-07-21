#include <stdio.h>
#include <time.h>

int main()
{
	time_t t = time(NULL);
	printf("%s\n", ctime(&t));
	return 0;
}
