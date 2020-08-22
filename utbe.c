#include <json.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

char *mkprms(char *url, int nargs, ...);
char *mkcmd(char *cmd, char *arg);

typedef struct{
	const char* publishedAt;
	const char* channelId;
	const char *title;
	const char* description;
	const char* channelTitle;
} video;

int
main(int arc, char **argv){
	FILE *fp;
	int status;
	size_t len;
	ssize_t nread;
	char *line = NULL;
	char *lines;
	char *qurl, *cmd;

	video videos[50];

	json_object *jobj = NULL;


	char *url = "https://www.googleapis.com/youtube/v3/search?";

	qurl = mkprms(url, 3, "part", "snippet", "q", argv[1], "key", argv[2]);

	cmd = mkcmd("curl -s ", qurl);

	printf("cmd: %s\n", cmd);

	//fp = popen(cmd, "r");
	fp = fopen("test", "r");
	if (fp == NULL) printf("popen-error\n");

	int count = 0;
	while ((nread = getline(&line, &len, fp)) != -1){
		if(count) lines = realloc(lines, ((count+nread) * sizeof(char)));	
		else  lines = malloc(sizeof(nread));	

		strcat(lines, line);
		count+=nread;
	}

	status = pclose(fp);

	jobj = json_tokener_parse(lines);
	json_object_iter iter;
	
	enum json_type type;	
	json_object_object_foreachC(jobj, iter) {
		json_object *val = iter.val;
		char *key = iter.key;
		type = json_object_get_type(val);
		if(type == json_type_array && (strcmp(key,"items") == 0)){
			int len;
			len = json_object_array_length(val);
			for(int i=0; i<len; i++){
				json_object_iter aiter;
				json_object *aobj = NULL;
				enum json_type atype;	

				aobj = json_object_array_get_idx(val,i);

				json_object_object_foreachC(aobj, aiter) {
					json_object_iter siter;
					json_object *sobj = NULL;
					enum json_type stype;	
					char *akey = aiter.key;

					sobj = aiter.val;
					stype = json_object_get_type(sobj);

					if(stype == json_type_object && (strcmp(akey,"snippet") == 0)){
						json_object_object_foreachC(sobj, siter) {
							json_object_iter csiter;
							json_object *csobj = NULL;
							enum json_type cstype;	
							char *skey = siter.key;
						
							csobj = siter.val;	
							cstype = json_object_get_type(csobj);

							if(cstype == json_type_string){
								const char *str = json_object_get_string(csobj);
								if((strcmp(skey, "publishedAt") == 0)) videos[i].publishedAt = str;
								if((strcmp(skey, "channelId") == 0)) videos[i].channelId = str;
								if((strcmp(skey, "title") == 0)) videos[i].title = str;
								if((strcmp(skey, "description") == 0)) videos[i].description = str;
								if((strcmp(skey, "channelTitle") == 0)) videos[i].channelTitle = str;
							}


						}
					}
				}
			}
		}
	}

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
