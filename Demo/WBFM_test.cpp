#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "wbfm.h"

int main()
{
	float inVec[MYCOUNT] = {0};
	float outVec[MYCOUNT>>3] = {0};

	FILE *fp_r;
	char string[64];
	char I_val[32], Q_val[32];
	fp_r = fopen("myinput.txt", "r");
	if (fp_r == NULL)
	{
		printf("cannot open myinput.txt");
		return 1;
	}

	int k, indx;
	for (k=0, indx=0; k<MYCOUNT/2; k++, indx=indx+2)
	{
		fscanf(fp_r, "%s", string);
		sscanf(string,"%[^,],%[^,]", I_val, Q_val);
		inVec[indx] = atof(I_val);
		inVec[indx+1] = atof(Q_val);
	}
	fclose(fp_r);

	xillybus_wrapper(inVec, outVec);

	fp_r = fopen("golden.txt", "r");
	if (fp_r == NULL)
	{
		printf("cannot open golden.txt");
		return 1;
	}
	float output;
	int fail = 0;
	for (k=0; k<(MYCOUNT>>3); k++)
	{
		fscanf(fp_r, "%f", &output);

		if ( fabs(outVec[k]-output) > 0.000001 )
		{
			printf("%d, output=%.10f\tgolden=%.10f\n",k, outVec[k], output);
			fail = 1;
		}
	}
	fclose(fp_r);

	if (fail)
		printf("Failed!!\n");
	else
		printf("Success!!\n");

	return 0;
}
