#define URL "https://www.googleapis.com/youtube/v3/"
#define usgstr "utbe: --\n"
#define usg() pdie(usgstr)

#define KIND_PRE "youtube#"
#define KIND_VIDEO "video"
#define KIND_SLR "searchListResponse"
#define KIND_CLR "channelListResponse"
#define KIND_PILR "playlistItemListResponse"

#define MAX_URL 2038

typedef struct{
	int valcnt;
	char **values;
} item;

#ifndef UTBE
#define UTBE extern
#endif

UTBE int vcnt;
UTBE int sjsn;
UTBE char qurl[MAX_URL]; 
UTBE char *jstr;
UTBE item *itms;

void clnup();
