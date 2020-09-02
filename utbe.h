#define URL "https://www.googleapis.com/youtube/v3/"

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

int utbftch(char *);
int utbprse();
