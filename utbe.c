#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define UTBE
#include "utbe.h"
#include "util.h"


int
main(int argc, char **argv){
	int opt;
	char *cmd;
	int kflag;

	itms = NULL;

	strcpy(qurl,URL);

	while((opt = getopt(argc, argv, "t:jq:c:k:r:")) != -1){
		switch(opt){
			case 't':
				mkrsrc(qurl, optarg);
				break;
			case 'j':
				sjsn=1;
				break;
			case 'q':
				mkprms(qurl, 2, "part", "snippet", "q", optarg);
				break;
			case 'c':
				mkprms(qurl, 1, "maxResults", optarg);
				kflag = 1;
				break;
			case 'k':
				mkprms(qurl, 1, "key", optarg);
				break;
			case 'r':
				{
					char *tok, *itok, *tmp;
					int pcnt = 0;
					while(tok = strtok_r(optarg, "=", &optarg)){
						while(itok = strtok_r(tok, "/", &tok)){
							if(pcnt % 2)mkprms(qurl, 1, tmp, itok);
							else tmp = itok;
							pcnt++;	
						}
					}
					break;
				}
			default: pdie("Bad input\n");
		}

	}


	//printf("qurl: %s\n",qurl);
	char *utkey = getenv("UTKEY");
	if(utkey != NULL) mkprms(qurl, 1, "key", utkey);

	if(argc == 1) exit(EXIT_FAILURE);

	cmd = mkcmd("curl -s ", qurl);
	
	if(!utbftch(cmd)) pdie("Couldn't get json\n");

	if(!utbprse()) pdie("Couldn't parse json\n");

	/*for(int i=0;i<vcnt;i++)
		printf("%s,%s,%s,%s,%s\n", vids[i].channelTitle, vids[i].channelId, vids[i].title, vids[i].description, vids[i].publishedAt);*/

	free(jstr);
	free(cmd);
	for(int i=0;i<vcnt;i++){
		for(int j=0;j<(&itms[i])->valcnt;j++){
			free((&itms[i])->values[j]);
			(&itms[i])->values[j] = NULL;
		}
		free((&itms[i])->values);
	}
	free(itms);
	return EXIT_SUCCESS;
}

int
utbftch(char *cmd){
	FILE *fp;
	int status;
	char *line = NULL;
	size_t len;
	ssize_t nread;

	//fp = popen(cmd, "r");
	fp = fopen("test", "r");
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
	status = pclose(fp);

	return 1;
}
