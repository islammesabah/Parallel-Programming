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
	char * buffer = 0;
	if(my_rank==0){
   		char input='5';
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
	    long mLength = (length+1) / p;
	    if(mLength == 0) mLength=1;
	    for(int i = 1;i<p;++i){
			MPI_Send (&input, 1, MPI_CHAR, i, 0,MPI_COMM_WORLD);
			MPI_Send (&mLength, 1, MPI_LONG, i, 1,MPI_COMM_WORLD);
			MPI_Send (&buffer[i*mLength],  mLength-1, MPI_CHAR, i, 2,MPI_COMM_WORLD);
		}
		char out[length];
		memcpy( out, buffer, mLength);
		for (int i = 0; i < mLength; i++){
			if (out[i] == input)
			{
				out[i] =(char)toupper(input);	
			}
	    }
	    for(int i = 1;i<p;i++){
			char recieved[mLength];
			MPI_Recv (&recieved, mLength, MPI_CHAR, i, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			long subleng = mLength;

			if(mLength*i+subleng>length){
				subleng--;
			}
			memcpy(&out[mLength*i],recieved,subleng);
			printf("%s\n", recieved);
	    }
		out[length]='\0';
	 	fp = fopen("test.txt", "w+");
	    fputs(out, fp);
	    fclose(fp);
	 }
	else{
		long l;
		char input;
		MPI_Recv (&input, 1, MPI_CHAR, 0, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv (&l, 1, MPI_LONG, 0, 1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		char bufferToW[l];
		MPI_Recv (&bufferToW, l, MPI_CHAR, 0, 2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		for (int i = 0; i < l; i++)
		{
			if (bufferToW[i] == input)
			{
				bufferToW[i] = (char)toupper(input);
			}
			
		}
		MPI_Send (&bufferToW, l, MPI_CHAR, 0, 0,MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
