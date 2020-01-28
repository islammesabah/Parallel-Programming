#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <mpi.h>

int main(int argc, char* argv[]){
	
	MPI_Init (&argc, &argv);

	int my_rank,p;
	MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size (MPI_COMM_WORLD, &p);
	FILE *fp;
	long length;
	long mLength;
	char * buffer = 0;
	char input;
	char inputArray[p];
	long mLengthArray[p];
	if(my_rank==0){
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
	    buffer[length]='\0';
	    mLength = (length+1) / p;
	    if(mLength == 0) mLength=1;
		for(int i=0;i<p;i++){
			inputArray[i]=input;
			mLengthArray[i]=mLength;
		}
	}
	MPI_Scatter(inputArray,1,MPI_CHAR,&input,1,MPI_CHAR,0,MPI_COMM_WORLD);
	MPI_Scatter(mLengthArray,1,MPI_LONG,&mLength,1,MPI_LONG,0,MPI_COMM_WORLD);
	char bufferToW[mLength];
	MPI_Scatter(buffer,mLength,MPI_CHAR,&bufferToW,mLength,MPI_CHAR,0,MPI_COMM_WORLD);
	
	for (int i = 0; i < mLength; i++)
	{
		if (bufferToW[i] == input)
		{
			bufferToW[i] = (char)toupper(input);
		}
	}
	
	char *outBuffer = NULL;
	if(my_rank==0){
		outBuffer = malloc(sizeof(char) * length);
	}
	MPI_Gather(&bufferToW, mLength, MPI_CHAR, outBuffer, mLength, MPI_CHAR, 0,MPI_COMM_WORLD);
	if(my_rank==0){
		fp = fopen("test.txt", "w+");
	    fputs(outBuffer, fp);
	    fclose(fp);
	}
	MPI_Finalize();

	return 0;
}
