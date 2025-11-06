/*
 *  Title: control.c
 *	Version:
 *  Description:
 *  Created:  16/07/2014 NW
 *  Last Mod: 17/10/2016 NW
 *  Author: C�tedra "Sistemas de Control"
 *  		Departamento de Electr�nica
 *  		Facultad de Ingenier�a
 *  		Universidad Nacional de Mar del Plata
 
 *	En este archivo se implementa la funci�n PLL_SdC_function() en la cual debe ser incluida la implementaci�n del PLL.
 *	Entradas:
 * 		Input10V: 0-10V -- etapa de adecuacion en la placa para sacar offset??
 *	Salidas:
 *		*UserLED / *UserGPIO: 0(0V):OFF-1(3.3V):ON
 *		Output3_3V: 0-3.3V  --> rango DAC 

 */
#include <math.h>
#define PI 3.14159265358979323846
int Output_Selector=9;

float lpf_in = 0;
float lpf_out = 0;
float error = 0;
float comp_out = 0;
float omega = 0;

float PLL_SdC_function(float Input10V, int *UserLED, int *UserGPIO) // return Output3_3V // HAL function que se ejecuta por cada sample
	{
	float InputPM1V;  // input
	float OutputZ0;  // output

	//adecuacion 
	float OutputPM1V;
	float OutputSat;
	float Output3_3V=1.65;

	// aux variables
	static float angle = 0;

	static float f_OutZ1,f_InpZ1= 0; // valores pasados lpf
	static float c_OutZ1,c_InpZ1= 0; // valores pasdos compensador
	static float i_OutZ1= 0; // valores pasdos integrador

	// Input conditioning
	InputPM1V = (Input10V/5.0)-1.0; // normalizo entre 0 y 1 

	// --**--**-- Inicio del c�digo del alumno --**--**--
	// multiplicador
	lpf_in = InputPM1V * sin(angle);

	//LPF
	lpf_out = (0.0419 * f_InpZ1) + (0.979 * f_OutZ1);
	//	update
	f_InpZ1 = lpf_in;
	f_OutZ1 = lpf_out;

	//sumador 1
	error =   0 - lpf_out ;

	// compensador

	comp_out = c_OutZ1 + (29.47 * error) - (29.408 * c_InpZ1);
	// update
	c_OutZ1 = comp_out;
	c_InpZ1 = error;

	//sumador 2

	omega = comp_out + (2*PI*50);

	// integrador

	angle = (0.001 * omega) + i_OutZ1;
	

	// wrap angle
	if (angle >= 2*PI) 
	{
		angle -= 2*PI;
	}
	if (angle < 0 ) 
	{
		angle += 2*PI;
	}

	i_OutZ1 = angle;

	OutputZ0 = sin(angle);

	// --**--**-- Fin del c�digo del alumno    --**--**--

	// --**--**-- Inicio del c�digo de ejemplo 1 --**--**--
	
		// Matlab 50Hz Low-Pass Filter
				// >> s=tf('s');
				// >> c2d(1/(s/(2*pi*50)+1),1e-3)
				// Transfer function:
				//   0.2696
				// ----------
				// z - 0.7304
				// Sampling time: 0.001

			// NumCoefZ1=0.2696;
			// DenCoefZ1=0.7304;
			// OutputZ0= NumCoefZ1 * InpZ1 + DenCoefZ1 * OutZ1;
			// OutZ1=OutputZ0;
			// InpZ1=InputPM1V;

	// puede servir 

		// Saturation and Flag
			
			*UserGPIO=1;
			if      (OutputZ0>0.5)  OutputSat=0.5;
			else if (OutputZ0<-0.5) OutputSat=-0.5;
			else
			{
				OutputSat=OutputZ0;
				*UserGPIO=0;
			}
		// Output Selector
			switch ( Output_Selector ) {
			case 0:
				OutputPM1V=angle;
			  break;//
			case 1:
				OutputPM1V=OutputZ0;
			  break;
			default:
				OutputPM1V=OutputSat;
			  break;
			}
	
	// --**--**-- Fin del c�digo de ejemplo 1 --**--**--
 
	// Output conditioning
	//Output3_3V= (OutputPM1V+1.0)*3.3/2.0; 
	Output3_3V= error;

	return(Output3_3V);
	}