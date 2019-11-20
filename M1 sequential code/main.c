#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main(){
    FILE *fp;
    long length;
    char * buffer = 0;

    char input;
    printf("%s","please Enter the character : \n"); 
    scanf("%c",&input);


 //open the file to count the number of chars
    fp = fopen ("test.txt", "rb");
    if (fp)
    {
        fseek (fp, 0, SEEK_END);
        length = ftell (fp);
        fseek (fp, 0, SEEK_SET);
        buffer = malloc (length);
        if (buffer)
        {
            fread (buffer, 1, length, fp);
        }
        fclose (fp);
    }



    for (int i = 0; i < length; i++)
    {
        if (buffer[i] == input)
        {
            buffer[i] = (char)toupper(input);
            
        }
        
    }



    fp = fopen("test.txt", "w+");
    fputs(buffer, fp);
    printf("%s",buffer);
    fclose(fp);



   return 0;
}