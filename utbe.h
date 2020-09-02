#define URL "https://www.googleapis.com/youtube/v3/"

typedef struct{
	char publishedAt[22];
	char channelId[30];
	char title[100];
	char description[1000];
	char channelTitle[30];
} video;

int utbftch(char *);
int utbprse();
