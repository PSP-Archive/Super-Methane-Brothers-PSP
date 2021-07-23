#include <stdio.h>


int main( int argc, char *argv[] ) {

        FILE * fp = fopen(argv[1],"r");
        char ch[1];
	int result = fread(ch, 1, 1, fp); 
	int cnt=0;
	printf(" unsigned char %s[] = { \n",argv[2]);

        while (result>0) {
		printf("0x%02x,",(unsigned char)ch[0]);
		if (cnt++>10) {
			printf("\n");
			cnt=0;
		}	
		result = fread(ch, 1, 1, fp); 
        }
	printf(" }; \n");
        fclose(fp);
}

