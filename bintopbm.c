#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void main(int argc, char *argv[])
{
	FILE *fp;
	unsigned char in_array[84];
	unsigned int i, j, x, y;

        if(argc == 1)
		exit(0);

	fp = fopen(argv[1], "r+");
	fputc('b', fp);
	printf("P1\n84 48\n");
	for(y = 0; y < 6; ++y) {
		for(x = 0; x < 84; ++x)
//			in_array[x] = fgetc(stdin);
			in_array[x] = fgetc(fp);
		for(j = 1; j < 129; j <<= 1) {
			for(x = 0; x < 84; ++x)
				printf("%c", (in_array[x] & j)?'1':'0');
			printf("\n");
		}
	}
	fclose(fp);
}
