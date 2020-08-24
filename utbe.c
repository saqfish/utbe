#include <jansson.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utbe.h"
#include "util.h"

int
main(int arc, char **argv){
	FILE *fp;
	int status;
	char *str, *cmd;
	char qurl[MAX_URL] = URL; 
	char *line = NULL;
	size_t len;
	ssize_t nread;

	video *vids;

	mkprms(qurl, 3, "part", "snippet", "q", argv[1], "key", argv[2]);
	mkprms(qurl, 1, "maxResults", "15");
	cmd = mkcmd("curl -s ", qurl);

	printf("cmd: %s\n", cmd);

	//fp = popen(cmd, "r");
	fp = fopen("test", "r");
	if (fp == NULL) printf("popen-error\n");

	int count = 0;
	while ((nread = getline(&line, &len, fp)) != -1){
		if(count) str = realloc(str, ((count+nread) * sizeof(str)));	
		else  str = calloc(nread, sizeof(str));	


		strcat(str, line);
		count+=nread;
	}
	free(line);

	status = pclose(fp);

	vids = getvids(str);

	printf("%s\n", vids[2].title);
	free(str);
	free(cmd);
	free(vids);
}

video * 
getvids(char *str){
	video *videos = calloc(50,sizeof(*videos));
	json_t *jobj;
	json_t *items;
	json_error_t jerr;

	jobj = json_loads(str, 0, &jerr);

	if(!jobj) printf("Couldn't parse JSON!\n");
	
	items = json_object_get(jobj, "items");

	if(!items) printf("Couldn't get items\n");

	if(!json_is_array(items)) printf("Couldn't get items\n");
	else printf("items is an array\n");

	for(int i=0; i<json_array_size(items); i++){
		json_t *item, *snippet;
		json_t *publishedAt, *channelId, *title, *description, *channelTitle;
		char *pm, cim, tm, dm, ctm;
		item = json_array_get(items, i);
		snippet = json_object_get(item, "snippet");

		if(!snippet) printf("Couldn't get snippet\n");
		if(!json_is_object(snippet)) printf("invalid snnipet\n");

		publishedAt = json_object_get(snippet, "publishedAt");
		channelId = json_object_get(snippet, "channelId");
		title = json_object_get(snippet, "title");
		description = json_object_get(snippet, "description");
		channelTitle = json_object_get(snippet, "channelTitle");

		if(!json_is_string(publishedAt)) printf("Couldn't get publishedAt\n");
		if(!json_is_string(channelId)) printf("Couldn't get channelId\n");
		if(!json_is_string(title)) printf("Couldn't get title\n");
		if(!json_is_string(description)) printf("Couldn't get description\n");
		if(!json_is_string(channelTitle)) printf("Couldn't get channelTitle\n");

		strcpy(videos[i].publishedAt,json_string_value(publishedAt));
		strcpy(videos[i].channelId, json_string_value(channelId));
		strcpy(videos[i].title, json_string_value(title));
		strcpy(videos[i].description, json_string_value(description));
		strcpy(videos[i].channelTitle, json_string_value(channelTitle));
	}
	json_decref(jobj);

	return videos;
}
