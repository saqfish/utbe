#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
pdie(char *str) {
	fprintf(stderr, "%s",str);
	exit(EXIT_SUCCESS);
}

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
mkrsrc(char *url, char *rsrc){
	strcat(url, rsrc);
	strcat(url, "?");
	return 1;
}

int
mkprms(char *url, int nargs, ...){
	char *arg;
	char str[1993] = "";
	va_list ap;

	va_start(ap, nargs);

	for(int i=0; i<(nargs*2); i++){
		int len = 0;
		char *sym;
		arg = va_arg(ap, char *);

		len += strlen(arg);

		if(i % 2) sym = "=";
		else  sym = "&";

		strcat(str,sym);
		strcat(str,arg);

	}
	strcat(url, str);	
	return 1;	
}
