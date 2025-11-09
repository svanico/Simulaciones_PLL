clear; close all

%%Parametros PLL
Ts=50e-6; %para la simulacion

%Señal de entrada
fi=50;
wi=2*pi*50;
VDD=10;

%Divisor
N=32;

%LPF
R1=10e3;  
R2=200e3;
Cf=1e-6;

%VCO
Kvco=(2*N*fi)/VDD;
Rvco=100e3;
Cvco=5e-9;


