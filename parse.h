#include <jansson.h>

#define KIND_PRE "youtube#"
#define KIND_SLR "searchListResponse"
#define KIND_CLR "channelListResponse"
#define KIND_PILR "playlistItemListResponse"

int itmvadd(item *itm, const char *str);
int utbprse();
int slrprse(json_t *);
int clrprse(json_t *);
int pilrprse(json_t *);
