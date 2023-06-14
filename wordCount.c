#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int wc(char* file_path, char* word){
	FILE *fp;
	int count = 0;
	int ch, len;
	if(NULL==(fp=fopen(file_path, "r")))
		return -1;
	len = strlen(word);
    int x = 1;
    int y = 1;
	while(x){
        y=1;
		int i;
		if(EOF==(ch=fgetc(fp))) break;
		if((char)ch != *word) continue;
		
		for(i=1;i < len;++i){
			if(EOF==(ch = fgetc(fp))){
                x=0;
                break;
            } 
			if((char)ch != word[i]){
				fseek(fp, 1-i, SEEK_CUR);
                y=0;
				break;
			}
		}
        if(x && y)
		++count;
	}
		fclose(fp);
		return count;
}
//TEST
/*int main(int argc, char const *argv[])
{
    printf("wc: %d\n",wc("applicants.txt","hétfő"));
    return 0;
}*/
