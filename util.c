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

char *
mkprms(char *url, int nargs, ...){
	int slen;
	char *arg, *arg2, *str, *final;
	va_list ap;

	va_start(ap, nargs);

	slen = 0;
	for(int i=0; i<(nargs*2); i++){
		char *sym;
		arg = va_arg(ap, char *);

		if(i%2) slen += strlen(arg) + 1; // +1 for &
		else slen += strlen(arg) + 1;// +1 for equal 

		slen++; // +1 for \0

		if(i) str = realloc(str, (slen * sizeof(str)));
		else str = calloc(slen, sizeof(str));

		if(i % 2) sym = "=";
		else  sym = "&";

		strcat(str, sym);
		strcat(str, arg);
	}


	slen+=strlen(url);
	slen++; // +1 for \0
	final = malloc(slen * sizeof(char));

	sprintf(final, "%s%s", url ,str);
	free(str);

	return final;	
}
