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
		//buffer = "hello world this iss kgfa test it is realy interistings but whlat is the he hshe r  kgnf kr fjrjeb bcas hhj d knhskf";
		//length = strlen(buffer);
		//printf("%s %ld \n\n\n",buffer, length);
	    long mLength = (length+1) / p;
	    if(mLength == 0) mLength=1;
	    for(int i = 1;i<p;++i){
			char  out[mLength];
			//////////////////////////////////////////////////////////////////////////////
			/*MPI_Send (&input, 1, MPI_CHAR, i, 0,MPI_COMM_WORLD);
			MPI_Send (&mLength, 1, MPI_LONG, i, 1,MPI_COMM_WORLD);
			MPI_Send (&buffer[i*mLength],  mLength-1, MPI_CHAR, i, 2,MPI_COMM_WORLD);
			*/
			MPI_Bcast(&input,1,MPI_CHAR,0,MPI_COMM_WORLD);
			printf("%ld \n\n\n",mLength);
			MPI_Bcast(&mLength,1,MPI_LONG,0,MPI_COMM_WORLD);
			MPI_Bcast(&length,1,MPI_LONG,0,MPI_COMM_WORLD);

			MPI_Bcast(&buffer[0],length,MPI_CHAR,0,MPI_COMM_WORLD);

			///////////////////////////////////////////////////////////////////////////////
		}
		//printf("%s \n\n\n",buffer);
		char out[length];
		memcpy( out, buffer, mLength);
	    //printf("%s \n\n\n",out);
		for (int i = 0; i < mLength; i++){
			if (out[i] == input)
			{
				out[i] =(char)toupper(input);	
			}
	    }
		//printf("%s \n\n\n",out);
		//printf("%s \n\n\n",buffer);
	    long l;
	    for(int i = 1;i<p;i++){
			char recieved[length];
			//MPI_Recv (&recieved, length, MPI_CHAR, i, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Bcast(&recieved[0],length,MPI_CHAR,i,MPI_COMM_WORLD);
			char sub[mLength];
			long subleng = mLength;
			if(mLength*i+subleng>length){
				subleng--;
			}
			memcpy(&out[mLength*i],&recieved[mLength*i],subleng);
			//printf("%s %s \n\n\n",recieved,sub);
			//strcat(out,sub);

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
		/*MPI_Recv (&input, 1, MPI_CHAR, 0, 0,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv (&l, 1, MPI_LONG, 0, 1,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Recv (&bufferToW, l, MPI_CHAR, 0, 2,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		*/
		MPI_Bcast(&input,1,MPI_CHAR,0,MPI_COMM_WORLD);
		MPI_Bcast(&mLength,1,MPI_LONG,0,MPI_COMM_WORLD);
		MPI_Bcast(&l,1,MPI_LONG,0,MPI_COMM_WORLD);
		char bufferToW[l];
		MPI_Bcast(&bufferToW[0],l,MPI_CHAR,0,MPI_COMM_WORLD);
		//printf("%s \n\n\n",bufferToW);
		long start =  my_rank * mLength;
		for (int i = start; i < (start + mLength); i++)
		{
			if (bufferToW[i] == input)
			{
				bufferToW[i] = (char)toupper(input);
			}
			
		}
		//printf("%s \n\n\n",bufferToW);
		//MPI_Send (&bufferToW[0],l, MPI_CHAR, 0,0,MPI_COMM_WORLD);
		MPI_Bcast(&bufferToW[0],l,MPI_CHAR,my_rank,MPI_COMM_WORLD);	
	}

	MPI_Finalize();

	return 0;
}
