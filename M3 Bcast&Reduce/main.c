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
		buffer[length]='\0';
	    long mLength = (length+1) / p;
	    if(mLength == 0) mLength=1;
	    for(int i = 1;i<p;++i){
			char  out[mLength];
			MPI_Bcast(&input,1,MPI_CHAR,0,MPI_COMM_WORLD);
			MPI_Bcast(&mLength,1,MPI_LONG,0,MPI_COMM_WORLD);
			MPI_Bcast(&length,1,MPI_LONG,0,MPI_COMM_WORLD);
			MPI_Bcast(&buffer[0],length,MPI_CHAR,0,MPI_COMM_WORLD);
		}
		char out[length];
		memcpy( out, buffer, mLength);
		for (int i = 0; i < mLength; i++){
			if (out[i] == input)
			{
				out[i] =(char)toupper(input);	
			}
	    }
	    long l;
	    for(int i = 1;i<p;i++){
			char recieved[length];
			MPI_Bcast(&recieved[0],length,MPI_CHAR,i,MPI_COMM_WORLD);
			long subleng = mLength;
			if(mLength*i+subleng>length){
				subleng--;
			}
			memcpy(&out[mLength*i],&recieved[mLength*i],subleng);
	    }
		out[length]='\0';
		printf("the out is ---> %s \n",(out));
		fp = fopen("test.txt", "w+");
	    fputs(out, fp);
	    fclose(fp);
	}
	else{
		long l;
		long mLength;
		char input;
		MPI_Bcast(&input,1,MPI_CHAR,0,MPI_COMM_WORLD);
		MPI_Bcast(&mLength,1,MPI_LONG,0,MPI_COMM_WORLD);
		MPI_Bcast(&l,1,MPI_LONG,0,MPI_COMM_WORLD);
		char bufferToW[l];
		MPI_Bcast(&bufferToW[0],l,MPI_CHAR,0,MPI_COMM_WORLD);
		long start =  my_rank * mLength;
		for (int i = start; i < (start + mLength); i++)
		{
			if (bufferToW[i] == input)
			{
				bufferToW[i] = (char)toupper(input);
			}
			
		}
		MPI_Bcast(&bufferToW[0],l,MPI_CHAR,my_rank,MPI_COMM_WORLD);	
	}

	MPI_Finalize();

	return 0;
}
