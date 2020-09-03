#include <stdio.h>

#include "utbe.h"
#include "output.h"

void
output(){
	for(int i=0;i<vcnt;i++){
		printf("%d: \n", i);
		for(int j=0;j<(&itms[i])->valcnt;j++){
			printf("-%s\n", (&itms[i])->values[j]);
		}
	}
}
