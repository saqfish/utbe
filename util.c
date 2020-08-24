#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *
mkcmd(char *cmd, char *arg){
	char *str;	
	int len;

	len = strlen(cmd) + strlen(arg) + 3;
	str = malloc(len * sizeof(str));

	sprintf(str, "%s \'%s\'", cmd, arg);
	return str;
}

int
mkprms(char *url, int nargs, ...){
	int slen;
	char *arg, *arg2;
	char str[1993] = "";
	va_list ap;

	va_start(ap, nargs);

	for(int i=0; i<(nargs*2); i++){
		int len = 0;
		char *sym;
		arg = va_arg(ap, char *);

		printf("%d(+%d) ", len, strlen(arg));

		len += strlen(arg);

		if(i % 2) sym = "=";
		else  sym = "&";

		strcat(str,sym);
		strcat(str,arg);

		printf("%s%s:  %s\n", sym, arg, str);
	}


	printf("total: %d\n", strlen(url) + strlen(str));

	strcat(url, str);	

	return 1;	
}
