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

	itms = NULL;

	mkurl(qurl);

	while((opt = getopt(argc, argv, "jt:q:c:p:P:x:k:r:")) != -1){
		switch(opt){
			case 't':
				mkrsrc(qurl, optarg);
				break;
			case 'j':
				sjsn=1;
				break;
			case 'q':
				mkrsrc(qurl, "search");
				mkprms(qurl, 2, "part", "snippet", "q", optarg);
				break;
			case 'c':
				mkrsrc(qurl, "channels");
				mkprms(qurl, 2, "part", "snippet,contentDetails", "id", optarg);
				break;
			case 'p':
				mkrsrc(qurl, "playlists");
				mkprms(qurl, 2, "part", "snippet,contentDetails", "id", optarg);
				break;
			case 'P':
				mkrsrc(qurl, "playlistItems");
				mkprms(qurl, 2, "part", "snippet,contentDetails", "playlistId", optarg);
				break;
			case 'x':
				mkprms(qurl, 1, "maxResults", optarg);
				break;
			case 'k':
				mkprms(qurl, 1, "key", optarg);
				break;
			case 'r':
				spltprms(qurl, optarg);
				break;
			default: usg();
		}

	}

	if(argc == 1) usg();

	char *utkey = getenv("UTKEY");
	if(utkey != NULL) mkprms(qurl, 1, "key", utkey);

	cmd = mkcmd("curl -s ", qurl);
	
	if(!ftch(cmd)) pdie("fetch failed \n");
	if(!prse()) pdie("parse failed \n");

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
