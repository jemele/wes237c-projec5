#include "volk.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	float input[MYCOUNT];
	float output[MYCOUNT];
	float gold_r, gold_i;

	FILE *fp_in_r, *fp_in_i, *fp_out_r, *fp_out_i;

	fp_in_r=fopen("volk_in_r.txt", "r");
	if(fp_in_r==NULL)
	{
		printf("cannot open volk_in_r.dat\n");
		return -1;
	}

	fp_in_i=fopen("volk_in_i.txt", "r");
	if(fp_in_i==NULL)
	{
		printf("cannot open volk_in_i.dat\n");
		return -1;
	}

	fp_out_r=fopen("volk_out_r.txt", "r");
	if(fp_out_r==NULL)
	{
		printf("cannot open volk_out_r.dat\n");
		return -1;
	}

	fp_out_i=fopen("volk_out_i.txt", "r");
	if(fp_out_i==NULL)
	{
		printf("cannot open volk_out_i.dat\n");
		return -1;
	}

	int j, fail;
	fail = 0;

	for(j= 0; j<MYCOUNT/2; j+=2){
		fscanf(fp_in_r, "%f", &input[j]);
		fscanf(fp_in_i, "%f", &input[j+1]);
	}

	volk(output, input);

	for(j = 0; j<MYCOUNT/2; j+=2){
		fscanf(fp_out_r, "%f", &gold_r);
		fscanf(fp_out_i, "%f", &gold_i);

		if( (fabs(output[j] - gold_r) > 0.000015) | (fabs(output[j+1] - gold_i) > 0.00001) )
		{
			printf("j=%d: out=%f, %f, golden=%f, %f\n", j, output[j], output[j+1], gold_r, gold_i);
			fail=1;
		}
	}

	fclose(fp_in_r);
	fclose(fp_in_i);
	fclose(fp_out_r);
	fclose(fp_out_i);

	if(fail)
	{
		printf("\n\nFailed!!\n\n");
	}
	else
	{
		printf("\n\nSuccess!!\n\n");
	}

	return 0;

}
