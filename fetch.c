#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utbe.h"
#include "fetch.h"

int
ftch(char *cmd){
	FILE *fp;
	char *line = NULL;
	size_t len;
	ssize_t nread;

	fp = popen(cmd, "r");
	//fp = fopen("search", "r");
	if (fp == NULL) return 0;

	int count = 0;
	while ((nread = getline(&line, &len, fp)) != -1){
		if(count) jstr = realloc(jstr, ((count+nread) * sizeof(jstr)));	
		else  jstr = calloc(nread, sizeof(jstr));	

		if(jstr == NULL) return 0;

		strcat(jstr, line);
		count+=nread;
	}
	free(line);
	pclose(fp);

	return 1;
}

int
fftch(char *file){
 	FILE *fp;
 	char *line = NULL;
 	size_t len;
 	ssize_t nread;
 
 	fp = fopen(file, "r");
 	if (fp == NULL) return 0;
 
 	int count = 0;
 	while ((nread = getline(&line, &len, fp)) != -1){
 		if(count) jstr = realloc(jstr, ((count+nread) * sizeof(jstr)));	
 		else  jstr = calloc(nread, sizeof(jstr));	
 
 		if(jstr == NULL) return 0;
 
 		strcat(jstr, line);
 		count+=nread;
 	}
 	free(line);
 	fclose(fp);
 	return 1;
 }
