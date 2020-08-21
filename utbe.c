#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


#define PATH_MAX 50

char *mkprms(char *url, int nargs, ...);
char *mkcmd(char *cmd, char *arg);

int
main(int arc, char **argv){
	FILE *fp;
	int status;
	char line[100];
	char *qurl;
	char *cmd;

	char *url = "https://www.googleapis.com/youtube/v3/search?";

        qurl = mkprms(url, 3, "part", "snippet", "q", argv[1], "key", argv[2]);
	
	cmd = mkcmd("curl -s ", qurl);

	printf("cmd: %s\n", cmd);
	
	char path[PATH_MAX];

	fp = popen(cmd, "r");
	if (fp == NULL) printf("popen-error\n");

	while (fgets(line, 100, fp) != NULL)
		printf("%s", line); 

	status = pclose(fp);
	free(cmd);
	free(qurl);
}
char *
mkcmd(char *cmd, char *arg){
	char *str;	
	int len;

	len = strlen(cmd) + strlen(arg) + 3;
	str = malloc(len * sizeof(char));
	
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
		arg = va_arg(ap, char *);

		if(i%2) slen += strlen(arg) + 1; // +1 for &
		else slen += strlen(arg) + 1;// +1 for equal 

		slen++; // +1 for \0

		if(i) str = realloc(str, (slen * sizeof(char)));
		else str = malloc(slen * sizeof(char));

		if(i % 2) sprintf(str, "%s=%s", str, arg);
		else sprintf(str, "%s&%s", str ,arg);
	}


	slen+=strlen(url);
	slen++; // +1 for \0
	final = malloc(slen * sizeof(char));

	sprintf(final, "%s%s", url ,str);
	free(str);

	return final;	
}
