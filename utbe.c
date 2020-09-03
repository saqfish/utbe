#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define UTBE
#include "utbe.h"
#include "util.h"
#include "fetch.h"
#include "parse.h"
#include "output.h"

int
main(int argc, char **argv){
	int opt;
	char *cmd;
	int kflag;

	itms = NULL;

	mkurl(qurl);

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
				spltprms(qurl, optarg);
				break;
			default: pdie("Bad input\n");
		}

	}


	if(argc == 1) exit(EXIT_FAILURE);

	char *utkey = getenv("UTKEY");
	if(utkey != NULL) mkprms(qurl, 1, "key", utkey);

	cmd = mkcmd("curl -s ", qurl);
	
	if(!utbftch(cmd)) pdie("Couldn't get json\n");
	if(!utbprse()) pdie("Couldn't parse json\n");

	output();

	clnup();
	free(cmd);

	return EXIT_SUCCESS;
}

void
clnup(){
	free(jstr);
	for(int i=0;i<vcnt;i++){
		for(int j=0;j<(&itms[i])->valcnt;j++){
			free((&itms[i])->values[j]);
			(&itms[i])->values[j] = NULL;
		}
		free((&itms[i])->values);
	}
	free(itms);
}
