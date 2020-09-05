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

			if(j < (&itms[i])->valcnt -1 ) printf(",");
		}
		printf("\n");
	}
}

int
getknd(char *kind){
	int vchk = 0;	
	int cchk = 0;	
	int plichk = 0;	

	vchk = (strcmp(KIND_VIDEO, kind) == 0);
	cchk = (strcmp(KIND_CHANNEL, kind) == 0);
	plichk = (strcmp(KIND_PLAYLISTITEM, kind) == 0);

	if(vchk) return ID_VIDEO;
	if(cchk) return ID_CHANNEL;
	if(plichk) return ID_PLAYLISTITEM;

	return 0;
}
