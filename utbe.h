#define URL "https://www.googleapis.com/youtube/v3/"

#define MAX_URL 2038

typedef struct{
	char publishedAt[22];
	char channelId[30];
	char title[100];
	char description[1000];
	char channelTitle[30];
} video;


#ifndef UTBE
#define EXTERN extern
#endif

EXTERN int vcnt;
EXTERN int sjsn;
EXTERN char qurl[MAX_URL]; 
EXTERN char *jstr;
EXTERN video *vids;


int utbftch(char *);
int utbprse();
