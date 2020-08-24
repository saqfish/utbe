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

	while((opt = getopt(argc, argv, "q:c:k:r:")) != -1){
		switch(opt){
			case 'q':
				mkprms(qurl, 2, "part", "snippet", "q", optarg);
				break;
			case 'c':
				mkprms(qurl, 1, "maxResults", optarg);
				break;
			case 'k':
				mkprms(qurl, 1, "key", optarg);
				break;
			case 'r':
				{
					char *tok, *tmp;
					int pcnt = 0;
					while(tok = strtok_r(optarg, ":", &optarg)){
						if(pcnt % 2)mkprms(qurl, 1, tmp, tok);
						else tmp = tok;
						pcnt++;	
					}
					break;
				}
			default: pdie("Bad input\n");
		}

	}

	if(argc == 1) exit(EXIT_FAILURE);

	cmd = mkcmd("curl -s ", qurl);
	printf("cmd: %s\n", cmd);
	
	if(!utbftch(cmd)) pdie("Couldn't get json\n");

	if(!utbprse()) pdie("Couldn't parse json\n");

	for(int i=0;i<vcnt;i++)
		printf("%s\n", vids[i].title);

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

	if(!jobj || !json_is_object(jobj)) return 0;

	utberr = json_object_get(jobj, "error");
	if(utberr && json_is_object(utberr)){
		json_t *errmsg = json_object_get(utberr, "message");
		if(errmsg || json_is_object(errmsg)) 
			fprintf(stderr, "Error: %s\n", json_string_value(errmsg));
		return 0;
	}
	
	items = json_object_get(jobj, "items");

	if(!items || !json_is_array(items)) return 0;

	for(int i=0; i<json_array_size(items); i++){
		char *pm, cim, tm, dm, ctm;
		json_t *item, *snippet;
		json_t *publishedAt, *channelId, *title, *description, *channelTitle;

		item = json_array_get(items, i);
		snippet = json_object_get(item, "snippet");

		if(!snippet ||!json_is_object(snippet)) return 0;

		publishedAt = json_object_get(snippet, "publishedAt");
		channelId = json_object_get(snippet, "channelId");
		title = json_object_get(snippet, "title");
		description = json_object_get(snippet, "description");
		channelTitle = json_object_get(snippet, "channelTitle");

		if(!json_is_string(publishedAt) || 
				!json_is_string(channelId) 
				|| !json_is_string(title) 
				|| !json_is_string(description) 
				|| !json_is_string(channelTitle)) 
			return 0;
		vcnt++;
		strcpy(vids[i].publishedAt,json_string_value(publishedAt));
		strcpy(vids[i].channelId, json_string_value(channelId));
		strcpy(vids[i].title, json_string_value(title));
		strcpy(vids[i].description, json_string_value(description));
		strcpy(vids[i].channelTitle, json_string_value(channelTitle));
	}

	json_decref(jobj);
	return 1;
}
