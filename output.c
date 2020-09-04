#include <stdio.h>
#include <string.h>

#include "utbe.h"
#include "output.h"


enum {NONE, ID_VIDEO, ID_CHANNEL, ID_PLAYLISTITEM};

void
output(){
	for(int i=0;i<vcnt;i++){
		for(int j=0;j<(&itms[i])->valcnt;j++){
			char *itmi = (&itms[i])->values[j];
			if(!j) printf("%d", getknd(itmi));
			else printf("%s", itmi);
		}
		printf("\n");
	}
}

int
getknd(char *kind){
	char *k = kind + strlen(KIND_PRE);
	if((strcmp("video", k)) == 0) return ID_VIDEO;
	if((strcmp("channel", k)) == 0) return ID_CHANNEL;
	if((strcmp("playlistItem", k)) == 0) return ID_PLAYLISTITEM;
	return 0;
}
