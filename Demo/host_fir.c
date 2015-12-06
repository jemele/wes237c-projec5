#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>

int main(int argc, char *argv[]) {

  int fdr, fdw;

  int tologic[600];
  int fromlogic[600];
  FILE * fout;
  FILE * fin;
  FILE * f_ref;

  int i;


  for (i=0;i<600;i++)
  {
	tologic[i]=0;
  }

  fin=fopen("input.dat","r");
  f_ref=fopen("chirp.txt","w");
  for (i=0;i<600;i++)
  {
        fscanf(fin,"%d",&tologic[i]);
	fprintf(f_ref,"%d ",i);
	fprintf(f_ref,"%d ",tologic[i]);
	fprintf(f_ref,"\n");
  }
  fclose(fin);
  fclose(f_ref);

  //Add your code here:
  //1. open read and write channels using fdr, fdw
  //2. write data into and read data from the FPGA
  //3. when it is done reading, close the channels
  //read the tutorial for details


  fout=fopen("output.txt","w");
  for (i=0;i<600;i++)
  {
	fprintf(fout,"%d ",i);
	fprintf(fout,"%d ",fromlogic[i]);
	fprintf(fout,"\n");
  }
  fclose(fout);


  return 0;
}
