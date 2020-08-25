#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utbe.h"
#include "util.h"

int vcnt;
char qurl[MAX_URL] = URL; 
char *jstr;
video *vids;

int
main(int argc, char **argv){
	int opt;
	char *cmd;
	int kflag;

	while((opt = getopt(argc, argv, "q:c:k:r:")) != -1){
		switch(opt){
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
						while(itok = strtok_r(tok, ",", &tok)){
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


	char *utkey = getenv("UTKEY");
	if(utkey != NULL) mkprms(qurl, 1, "key", utkey);

	if(argc == 1) exit(EXIT_FAILURE);

	cmd = mkcmd("curl -s ", qurl);
	
	if(!utbftch(cmd)) pdie("Couldn't get json\n");

	if(!utbprse()) pdie("Couldn't parse json\n");

	for(int i=0;i<vcnt;i++)
		printf("Channel: %s\nID: %s\nTitle: %s\nDesc: %s\nPublished: %s\n-------------\n", vids[i].channelTitle, vids[i].channelId, vids[i].title, vids[i].description, vids[i].publishedAt);

	free(jstr);
	free(cmd);
	free(vids);
	return EXIT_SUCCESS;
}

int
utbftch(char *cmd){
	FILE *fp;
	int status;
	char *line = NULL;
	size_t len;
	ssize_t nread;

	fp = popen(cmd, "r");
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

int
utbprse(){
	vids = calloc(50,sizeof(*vids));
	json_t *jobj, *utberr, *items;
	json_error_t jerr;

	jobj = json_loads(jstr, 0, &jerr);

	utberr = json_object_get(jobj, "error");
	if(utberr != NULL){
		json_t *errmsg = json_object_get(utberr, "message");
		if(errmsg != NULL) 
			fprintf(stderr, "Error: %s\n", json_string_value(errmsg));
		return 0;
	}
	
	items = json_object_get(jobj, "items");
	if(items == NULL) return 0;

	for(int i=0; i<json_array_size(items); i++){
		char *pm, cim, tm, dm, ctm;
		json_t *item, *snippet;
		json_t *publishedAt, *channelId, *title, *description, *channelTitle;

		item = json_array_get(items, i);
		snippet = json_object_get(item, "snippet");

		if(snippet == NULL) return 0;

		publishedAt = json_object_get(snippet, "publishedAt");
		channelId = json_object_get(snippet, "channelId");
		title = json_object_get(snippet, "title");
		description = json_object_get(snippet, "description");
		channelTitle = json_object_get(snippet, "channelTitle");

		vcnt++;

		if(publishedAt != NULL && json_is_string(publishedAt)) strcpy(vids[i].publishedAt,json_string_value(publishedAt));
		if(channelId != NULL && json_is_string(channelId)) strcpy(vids[i].channelId, json_string_value(channelId));
		if(title != NULL && json_is_string(title)) strcpy(vids[i].title, json_string_value(title));
		if(description != NULL && json_is_string(description)) strcpy(vids[i].description, json_string_value(description));
		if(channelTitle != NULL && json_is_string(channelTitle)) strcpy(vids[i].channelTitle, json_string_value(channelTitle));
	}

	json_decref(jobj);
	return 1;
}
