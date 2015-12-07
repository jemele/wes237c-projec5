#include"wbfm.h"

/* FIR */
static double taps[155] = { 0.00004801760588	, -0.00007127169374	, -0.00015557580628	, -0.00016055021842	, -0.00008158741548	, 0.00004278294000	, 0.00014927044685	, 0.00018144934438	, 0.00011914555944	, -0.00000945342163	, -0.00014000367082	, -0.00020357579342	, -0.00016244665312	, -0.00003183816443	, 0.00012463537860	, 0.00022525878740	, 0.00021222006762	, 0.00008408562280	, -0.00009915599367	, -0.00024333830515	, -0.00026778053143	, -0.00014963364811	, 0.00005908817911	, 0.00025324634043	, 0.00032673988608	, 0.00022965090466	, 0.00000000000000	, -0.00024925271282	, -0.00038485584082	, -0.00032364428625	, -0.00008190503286	, 0.00022487013484	, 0.00043604517123	, 0.00042906127055	, 0.00018916388217	, -0.00017340117483	, -0.00047257813276	, -0.00054102297872	, -0.00032244450995	, 0.00008859649097	, 0.00048545747995	, 0.00065222498961	, 0.00048000202514	, 0.00003461547385	, -0.00046497289441	, -0.00075303140329	, -0.00065725046443	, -0.00019937696925	, 0.00040140660712	, 0.00083177670604	, 0.00084649486234	, 0.00040618091589	, -0.00028585540713	, -0.00087527534924	, -0.00103686051443	, -0.00065223750426	, 0.00011112079665	, 0.00086952449055	, 0.00121444417164	, 0.00093098089565	, 0.00012738478836	, -0.00080057117157	, -0.00136269780342	, -0.00123176013585	, -0.00043077606824	, 0.00065550167346	, 0.00146304338705	, 0.00153975340072	, 0.00079593411647	, -0.00042350223521	, -0.00149569357745	, -0.00183612213004	, -0.00121490121819	, 0.00009693008178	, 0.00144064868800	, 0.00209841830656	, 0.00167447549757	, 0.00032766532968	, -0.00127881590743	, -0.00230122450739	, -0.00215603737161	, -0.00084863294614	, 0.00099319149740	, 0.00241700396873	, 0.00263562053442	, 0.00145847408567	, -0.00057004101109	, -0.00241709826514	, -0.00308421836235	, -0.00214330153540	, 0.00000000000000	, 0.00227281544358	, 0.00346829369664	, 0.00288252183236	, 0.00072097236989	, -0.00195650593378	, -0.00375041994266	, -0.00364873977378	, -0.00159085332416	, 0.00144251843449	, 0.00388995208777	, 0.00440783658996	, 0.00260108732618	, -0.00070789724123	, -0.00384356360883	, -0.00511913513765	, -0.00373642914928	, -0.00026736696600	, 0.00356540596113	, 0.00573547463864	, 0.00497512612492	, 0.00150078407023	, -0.00300650694408	, -0.00620290776715	, -0.00628944765776	, -0.00300890789367	, 0.00211277091876	, 0.00645947689191	, 0.00764654530212	, 0.00481066806242	, -0.00082040886628	, -0.00643209880218	, -0.00900961644948	, -0.00693382974714	, -0.00095351040363	, 0.00602953461930	, 0.01033931691200	, 0.00942746177316	, 0.00333045888692	, -0.00512698898092	, -0.01159535255283	, -0.01238726079464	, -0.00651211757213	, 0.00353117636405	, 0.01273818872869	, 0.01601212844253	, 0.01087844930589	, -0.00089391093934	, -0.01373075507581	, -0.02074968814850	, -0.01725423894823	, -0.00353625230491	, 0.01454010419548	, 0.02775634452701	, 0.02781471237540	, 0.01182719785720	, -0.01513889338821	, -0.04092928767204	, -0.05061084032059	, -0.03266235440969	, 0.01550663728267	, 0.08518035709858	, 0.15838518738747	, 0.21382068097591	, 0.23445974290371};
static float shift[309];
static int d_ntaps = sizeof(shift)/sizeof(*shift);
static float result_holder = 0;

void fir(float * result, float input, unsigned short do_filter)
{
#pragma HLS pipeline
	//write your code here
	float acc = 0;
	for (int i=d_ntaps-1;i>=1;i--) {
#pragma HLS unroll
		shift[i]=shift[i-1];
		acc+=shift[i]*taps[i];
	}

	shift[0]=input;
	acc+=input*taps[0];


	if (do_filter == 0) {
		result_holder = acc;
	}

	*result = result_holder;
}
/* **** */

/* VOLK */
void volk(float outputVector[MYCOUNT], float inputVector[MYCOUNT])
{
#pragma HLS pipeline II=1024
	for(int i = 0; i <MYCOUNT-4; i+=2){
		const float r1 = inputVector[i];
		const float i1 = inputVector[i+1];
		const float r2 = inputVector[i+2];
		const float i2 = inputVector[i+3];

		outputVector[i] = (r1*r2)+(i1*i2);
		outputVector[i+1] = (r1*i2) - (i1*r2);
	}
}
/* **** */

/* IIR */
static float a1 = 0.928849;
static float b0 = 0.964424;
static float b1 = 0.964424;

static float input_minus_one = 0.0;
static float output_minus_one = 0.0;

void iir(float input, float *output, int size)
{
	*output = input*b0+input_minus_one*b1 + output_minus_one*a1;
	output_minus_one = *output;
	input_minus_one = input;
}
/* **** */

  #define TAN_MAP_RES 0.003921549 /* (smallest non-zero value in table) */

  #define RAD_PER_DEG 0.017453293

  #define TAN_MAP_SIZE 255

  /* arctangents from 0 to pi/4 radians */

  static float
  fast_atan_table[257] = {
		  0.000000000, 0.003921549, 0.007842976, 0.011764160,
		  0.015684990, 0.019605330, 0.023525070, 0.027444090,
		  0.031362260, 0.035279470, 0.039195600, 0.043110530,
		  0.047024130, 0.050936290, 0.054846900, 0.058755820,
		  0.062662950, 0.066568160, 0.070471340, 0.074372380,
		  0.078271140, 0.082167520, 0.086061410, 0.089952670,
		  0.093841210, 0.097726910, 0.101609600, 0.105489300,
		  0.109365800, 0.113239000, 0.117108700, 0.120975000,
		  0.124837600, 0.128696500, 0.132551500, 0.136402600,
		  0.140249600, 0.144092400, 0.147931000, 0.151765200,
		  0.155594800, 0.159419900, 0.163240300, 0.167055900,
		  0.170866500, 0.174672200, 0.178472800, 0.182268100,
		  0.186058200, 0.189842800, 0.193622000, 0.197395600,
		  0.201163400, 0.204925500, 0.208681800, 0.212432000,
		  0.216176200, 0.219914300, 0.223646100, 0.227371600,
		  0.231090700, 0.234803300, 0.238509300, 0.242208600,
		  0.245901200, 0.249586900, 0.253265800, 0.256937600,
		  0.260602400, 0.264260000, 0.267910400, 0.271553500,
		  0.275189200, 0.278817500, 0.282438300, 0.286051400,
		  0.289656900, 0.293254700, 0.296844700, 0.300426800,
		  0.304000900, 0.307567100, 0.311125200, 0.314675200,
		  0.318217000, 0.321750600, 0.325275800, 0.328792700,
		  0.332301200, 0.335801200, 0.339292600, 0.342775500,
		  0.346249700, 0.349715300, 0.353172100, 0.356620100,
		  0.360059300, 0.363489600, 0.366911000, 0.370323400,
		  0.373726800, 0.377121100, 0.380506400, 0.383882500,
		  0.387249400, 0.390607000, 0.393955500, 0.397294600,
		  0.400624400, 0.403944800, 0.407255800, 0.410557400,
		  0.413849600, 0.417132200, 0.420405400, 0.423668900,
		  0.426922900, 0.430167300, 0.433402100, 0.436627200,
		  0.439842600, 0.443048300, 0.446244300, 0.449430600,
		  0.452607000, 0.455773800, 0.458930700, 0.462077800,
		  0.465215000, 0.468342400, 0.471460000, 0.474567600,
		  0.477665400, 0.480753200, 0.483831200, 0.486899200,
		  0.489957300, 0.493005500, 0.496043700, 0.499071900,
		  0.502090200, 0.505098500, 0.508096800, 0.511085200,
		  0.514063600, 0.517032000, 0.519990400, 0.522938800,
		  0.525877200, 0.528805600, 0.531724100, 0.534632500,
		  0.537531000, 0.540419500, 0.543298000, 0.546166600,
		  0.549025100, 0.551873800, 0.554712400, 0.557541100,
		  0.560359900, 0.563168700, 0.565967600, 0.568756600,
		  0.571535700, 0.574304800, 0.577064100, 0.579813500,
		  0.582553100, 0.585282800, 0.588002600, 0.590712600,
		  0.593412800, 0.596103200, 0.598783900, 0.601454700,
		  0.604115800, 0.606767200, 0.609408800, 0.612040700,
		  0.614663000, 0.617275500, 0.619878400, 0.622471700,
		  0.625055400, 0.627629400, 0.630193900, 0.632748800,
		  0.635294200, 0.637830100, 0.640356500, 0.642873400,
		  0.645380800, 0.647878800, 0.650367400, 0.652846600,
		  0.655316500, 0.657777000, 0.660228200, 0.662670100,
		  0.665102700, 0.667526100, 0.669940200, 0.672345200,
		  0.674740900, 0.677127600, 0.679505100, 0.681873500,
		  0.684232800, 0.686583100, 0.688924400, 0.691256700,
		  0.693580000, 0.695894300, 0.698199800, 0.700496400,
		  0.702784100, 0.705063000, 0.707333000, 0.709594300,
		  0.711846900, 0.714090700, 0.716325800, 0.718552300,
		  0.720770100, 0.722979400, 0.725180000, 0.727372100,
		  0.729555700, 0.731730700, 0.733897400, 0.736055500,
		  0.738205300, 0.740346700, 0.742479700, 0.744604500,
		  0.746720900, 0.748829100, 0.750929100, 0.753020800,
		  0.755104400, 0.757179800, 0.759247200, 0.761306400,
		  0.763357600, 0.765400800, 0.767436000, 0.769463300,
		  0.771482600, 0.773494000, 0.775497500, 0.777493200,
		  0.779481100, 0.781461200, 0.783433500, 0.785398200,
		  0.785398200
  };

  /*****************************************************************************

   Function: Arc tangent


   Syntax: angle = fast_atan2(y, x);

   float y y component of input vector

   float x x component of input vector

   float angle angle of vector (x, y) in radians


   Description: This function calculates the angle of the vector (x,y)

   based on a table lookup and linear interpolation. The table uses a

   256 point table covering -45 to +45 degrees and uses symetry to

   determine the final angle value in the range of -180 to 180

   degrees. Note that this function uses the small angle approximation

   for values close to zero. This routine calculates the arc tangent

   with an average error of +/- 3.56e-5 degrees (6.21e-7 radians).

  *****************************************************************************/
  float fast_atan(float y, float x)
  {
#pragma HLS pipeline

	float x_abs, y_abs, z;
	float alpha, angle, base_angle;
	int index;

	/* normalize to +/- 45 degree range */
	y_abs = fabsf(y);
	x_abs = fabsf(x);
	/* don't divide by zero! */
	if(!((y_abs > 0.0f) || (x_abs > 0.0f)))
		return 0.0;

	if(y_abs < x_abs)
		z = y_abs / x_abs;
	else
		z = x_abs / y_abs;

	/* when ratio approaches the table resolution, the angle is */
	/* best approximated with the argument itself... */
	if(z < TAN_MAP_RES)
		base_angle = z;
	else {
		/* find index and interpolation value */
		alpha = z * (float)TAN_MAP_SIZE;
		index = ((int)alpha) & 0xff;
		alpha -= (float)index;
		/* determine base angle based on quadrant and */
		/* add or subtract table value from base angle based on quadrant */
		base_angle  =  fast_atan_table[index];
		base_angle += (fast_atan_table[index + 1] - fast_atan_table[index]) * alpha;
	}

	if(x_abs > y_abs) { /* -45 -> 45 or 135 -> 225 */
		if(x >= 0.0) { /* -45 -> 45 */
		  if(y >= 0.0)
			angle = base_angle; /* 0 -> 45, angle OK */
		  else
			angle = -base_angle; /* -45 -> 0, angle = -angle */
		}
		else { /* 135 -> 180 or 180 -> -135 */
		  angle = 3.14159265358979323846;
		  if(y >= 0.0)
			angle -= base_angle; /* 135 -> 180, angle = 180 - angle */
		  else
			angle = base_angle - angle; /* 180 -> -135, angle = angle - 180 */
		}
	}
	else { /* 45 -> 135 or -135 -> -45 */
		if(y >= 0.0) { /* 45 -> 135 */
		  angle = 1.57079632679489661923;
		  if(x >= 0.0)
			angle -= base_angle; /* 45 -> 90, angle = 90 - angle */
		  else
			angle += base_angle; /* 90 -> 135, angle = 90 + angle */
		}
		else { /* -135 -> -45 */
		  angle = -1.57079632679489661923;
		  if(x >= 0.0)
			angle += base_angle; /* -90 -> -45, angle = -90 + angle */
		  else
			angle -= base_angle; /* -135 -> -90, angle = -90 - angle */
		}
	}

	return (angle);
  }

/* XILLYBUS WRAPPER */
static float d_gain = 0.002122;

void xillybus_wrapper(int *in, int *out)
{
#pragma AP interface ap_fifo port=in
#pragma AP interface ap_fifo port=out
#pragma AP interface ap_ctrl_none port=return

	int x1, y1;
	int i = 0, j = 0;
	float inf[MYCOUNT];
#pragma HLS ARRAY_PARTITION variable=inf complete dim=1

	float outf[MYCOUNT];
#pragma HLS ARRAY_PARTITION variable=outf complete dim=1

	float y11;
	/* arc tan inputs */
	float c, d, result, tempb;

	for( i = 0; i < MYCOUNT; i++) {
		x1 = *in++;
		inf[i] = *((float *)&x1);
	}

	volk(outf, inf);

	for( i = 0, j = 0; i < MYCOUNT-1; i=i+2, j++) {
		c = fast_atan(outf[i+1], outf[i]); //image, real
		d = d_gain * c;
		fir(&tempb, d, j%4);
		if (j%4 == 0) {
		  iir(tempb, &result, 1);
		  y11 = result;
		  y1 = *((int *) &y11);
		  *out++ = y1;
		}
	}
}
