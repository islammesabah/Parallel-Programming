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
		// printf("%s\n",buffer);
		// buffer = "this is a text to test the code and to test other stuff to dude I hate c because it's stupid w ady shwayet kalam kaman w shwayet 7rooof tttt ";
		// length=strlen(buffer);
	    long mLength = (length+1) / p;
	    if(mLength == 0) mLength=1;
	    for(int i = 1;i<p;++i){
			char  out[mLength];
			// memcpy( out, &buffer[mLength*i], mLength +1);
   			//  strcpy(a->array[a->used++], element);	
			// printf("STRing To send %s length is  %ld\n",out,mLength);
			MPI_Send (&input, 1, MPI_CHAR, i, 0,MPI_COMM_WORLD);
			MPI_Send (&mLength, 1, MPI_LONG, i, 1,MPI_COMM_WORLD);
			MPI_Send (&buffer[i*mLength],  mLength-1, MPI_CHAR, i, 2,MPI_COMM_WORLD);
		}
		// printf("%s \n",buffer);
			char out[mLength];
			memcpy( out, &buffer[0], mLength );
	    for (int i = 0; i < mLength; i++){
			if (out[i] == input)
			{
				out[i] =(char)toupper(input);	
			}
	    }
	    long l;
	    for(int i = 1;i<p;i++){
		// MPI_Recv (&l, 1, MPI_LONG, i, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printf("p1 mlength %ld",mLength);
		// char bufferW[mLength];
		char recieved[mLength];
		// printf("%s \n",bufferW);
		MPI_Recv (&recieved, mLength, MPI_CHAR, i, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			strcat(out,recieved);

	    }
					printf("%s \n",(out));
		fp = fopen("test.txt", "w+");
	    fputs(out, fp);
	    // printf("%s \n",out);
	    fclose(fp);
	}
	else{
		long l;
		char input;
		MPI_Recv (&input, 1, MPI_CHAR, 0, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printf("%d %c\n",my_rank,input);
		MPI_Recv (&l, 1, MPI_LONG, 0, 1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printf("%d %ld %c \n",my_rank,l,input);
		char bufferToW[l];
		MPI_Recv (&bufferToW, l, MPI_CHAR, 0, 2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// printf("%d %ld %c %s\n",my_rank,l,input,bufferToW);
		// printf("end oF pRINT");
		for (int i = 0; i < l; i++)
		{
			if (bufferToW[i] == input)
			{
				bufferToW[i] = (char)toupper(input);
			}
			
		}
		// printf("p1 new l : %ld new buffer %s \n",l,bufferToW);

		// MPI_Send (&l, 1, MPI_LONG, 0, 0,MPI_COMM_WORLD);
		MPI_Send (&bufferToW, l, MPI_CHAR, 0, 0,MPI_COMM_WORLD);
	}

	MPI_Finalize();

	return 0;
}
