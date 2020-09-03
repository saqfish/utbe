#define _POSIX_C_SOURCE 200809L
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utbe.h"
#include "util.h"

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
mkurl(char *url){
	strcpy(url,URL);
	return 1;
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

int
spltprms(char *url, char *args){
	char *tok, *itok, *tmp;
	int pcnt = 0;
	while(tok = strtok_r(args, "=", &args)){
		while(itok = strtok_r(tok, "/", &tok)){
			if(pcnt % 2)mkprms(url, 1, tmp, itok);
			else tmp = itok;
			pcnt++;	
		}
	}
}
