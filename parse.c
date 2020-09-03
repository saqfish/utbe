#include <string.h>

#include "utbe.h"
#include "parse.h"

int
utbprse(){
	json_t *jobj, *utberr, *items, *kind;
	json_error_t jerr;

	int prsed = 0;

	if(sjsn) printf(jstr);

	itms = calloc(1,sizeof(*itms));
	if(itms == NULL) return 0;

	jobj = json_loads(jstr, 0, &jerr);

	utberr = json_object_get(jobj, "error");
	if(utberr != NULL){
		json_t *errmsg = json_object_get(utberr, "message");
		if(errmsg != NULL) 
			fprintf(stderr, "Error: %s\n", json_string_value(errmsg));
		return 0;
	}

	kind = json_object_get(jobj, "kind");
	if(kind == NULL) return 0;

	items = json_object_get(jobj, "items");
	if(items == NULL) return 0;

	if((strcmp(KIND_SLR, json_string_value(kind) + strlen(KIND_PRE)) == 0)){
		prsed = slrprse(items);
		if(!prsed) fprintf(stderr, "sl parse failed - ");
	}
	else if((strcmp(KIND_CLR, json_string_value(kind) + strlen(KIND_PRE)) == 0)){
		prsed = clrprse(items);
		if(!prsed) fprintf(stderr, "clr parse failed - ");
	}
	else if((strcmp(KIND_PILR, json_string_value(kind) + strlen(KIND_PRE)) == 0)){
		prsed = pilrprse(items);
		if(!prsed) fprintf(stderr, "pilr parse failed - ");
	}
	else fprintf(stderr, "Unknown type - ");

	json_decref(jobj);
	return prsed;
}

int itmvadd(item *itm, const char *str){
	if(itm->valcnt) {
		int len = (itm->valcnt+1)*sizeof(*itm->values);
		itm->values = realloc(itm->values, len);
		if(itms->values == NULL) return 0;
	} else {
		int len = sizeof(*itm->values);
		itm->values = malloc(len);
	}
	int len = (strlen(str)+1);
	itm->values[itm->valcnt] = calloc(len,sizeof(char));
	strcpy(itm->values[itm->valcnt],str);
	
	itm->valcnt++;
	return 1;
}

int slrprse(json_t *items){
	for(int i=0; i<json_array_size(items); i++){
		json_t *item, *snippet;
		json_t *publishedAt, *channelId, *title, *description, *channelTitle;

		if(vcnt) itms = realloc(itms,(vcnt+1)*sizeof(*itms));
		(&itms[i])->valcnt = 0;

		item = json_array_get(items, i);
		snippet = json_object_get(item, "snippet");

		if(snippet == NULL) return 0;

		publishedAt = json_object_get(snippet, "publishedAt");
		channelId = json_object_get(snippet, "channelId");
		title = json_object_get(snippet, "title");
		description = json_object_get(snippet, "description");
		channelTitle = json_object_get(snippet, "channelTitle");

		vcnt++;

		if(publishedAt != NULL && json_is_string(publishedAt)) itmvadd(&itms[i],json_string_value(publishedAt));
		if(channelId != NULL && json_is_string(channelId)) itmvadd(&itms[i], json_string_value(channelId));
		if(title != NULL && json_is_string(title)) itmvadd(&itms[i], json_string_value(title));
		if(description != NULL && json_is_string(description)) itmvadd(&itms[i], json_string_value(description));
		if(channelTitle != NULL && json_is_string(channelTitle)) itmvadd(&itms[i], json_string_value(channelTitle));
	}
	return 1;
}

int 
clrprse(json_t *items){
	for(int i=0; i<json_array_size(items); i++){
		json_t *item, *snippet, *contentDetails, *id;
		json_t *relatedPlaylists;
		json_t *uploads;

		if(vcnt) itms = realloc(itms,(vcnt+1)*sizeof(*itms));
		(&itms[i])->valcnt = 0;

		item = json_array_get(items, i);

		snippet = json_object_get(item, "snippet");
		if(snippet == NULL) return 0;

		contentDetails = json_object_get(item, "contentDetails");
		if(contentDetails == NULL) return 0;

		relatedPlaylists = json_object_get(contentDetails, "relatedPlaylists");
		if(relatedPlaylists == NULL) return 0;

		id = json_object_get(item, "id");
		uploads = json_object_get(relatedPlaylists, "uploads");

		vcnt++;

		if(id != NULL && json_is_string(id)) itmvadd(&itms[i], json_string_value(id));
		if(uploads != NULL && json_is_string(uploads)) itmvadd(&itms[i], json_string_value(uploads));
	}
	return 1;
}

int 
pilrprse(json_t *items){
	for(int i=0; i<json_array_size(items); i++){
		json_t *item, *snippet;
		json_t *publishedAt, *channelId, *title, *description, *channelTitle;

		if(vcnt) itms = realloc(itms,(vcnt+1)*sizeof(*itms));
		(&itms[i])->valcnt = 0;

		item = json_array_get(items, i);
		snippet = json_object_get(item, "snippet");

		if(snippet == NULL) return 0;

		publishedAt = json_object_get(snippet, "publishedAt");
		channelId = json_object_get(snippet, "channelId");
		title = json_object_get(snippet, "title");
		description = json_object_get(snippet, "description");
		channelTitle = json_object_get(snippet, "channelTitle");

		vcnt++;

		if(publishedAt != NULL && json_is_string(publishedAt)) itmvadd(&itms[i],json_string_value(publishedAt));
		if(channelId != NULL && json_is_string(channelId)) itmvadd(&itms[i], json_string_value(channelId));
		if(title != NULL && json_is_string(title)) itmvadd(&itms[i], json_string_value(title));
		if(description != NULL && json_is_string(description)) itmvadd(&itms[i], json_string_value(description));
		if(channelTitle != NULL && json_is_string(channelTitle)) itmvadd(&itms[i], json_string_value(channelTitle));
	}
	return 1;
}
