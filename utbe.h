#define URL "https://www.googleapis.com/youtube/v3/search?"

typedef struct{
	char publishedAt[20];
	char channelId[20];
	char title[50];
	char description[50];
	char channelTitle[50];
} video;

int utbftch(char *);
int utbprse();
