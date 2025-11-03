
/*
 * Include Files
 *
 */
#if defined(MATLAB_MEX_FILE)
#include "tmwtypes.h"
#include "simstruc_types.h"
#else
#include "rtwtypes.h"
#endif



/* %%%-SFUNWIZ_wrapper_includes_Changes_BEGIN --- EDIT HERE TO _END */
// #include "myfunc.c"
#include "control.c"
/* %%%-SFUNWIZ_wrapper_includes_Changes_END --- EDIT HERE TO _BEGIN */
#define u_width 1
#define y_width 1

/*
 * Create external references here.  
 *
 */
/* %%%-SFUNWIZ_wrapper_externs_Changes_BEGIN --- EDIT HERE TO _END */
// extern double myfunc(double a);
float PLL_SdC_function(float Input10V, int *UserLED, int *UserGPIO);
/* %%%-SFUNWIZ_wrapper_externs_Changes_END --- EDIT HERE TO _BEGIN */

/*
 * Output function
 *
 */
void sfun_SdC_Outputs_wrapper(const real_T *Input10V,
			real_T *Output3_3V,
			int32_T *UserLED,
			int32_T *UserGPIO)
{
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_BEGIN --- EDIT HERE TO _END */
float Input10Vsat,Output3_3Vsat;

if (Input10V[0]>10) Input10Vsat=10;
else if (Input10V[0]<0) Input10Vsat=0;
else Input10Vsat=Input10V[0];

Output3_3Vsat=PLL_SdC_function(Input10Vsat,UserLED,UserGPIO);

if (Output3_3Vsat>3.3) Output3_3V[0]=3.3;
else if (Output3_3Vsat<0) Output3_3V[0]=0;
else Output3_3V[0]=Output3_3Vsat;
/* %%%-SFUNWIZ_wrapper_Outputs_Changes_END --- EDIT HERE TO _BEGIN */
}


