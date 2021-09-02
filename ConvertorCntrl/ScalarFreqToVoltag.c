/*
 * ���������� ��������� ����������
 */

#include "ModBus/modbus.h"

char initLineAproximVoltage(void);
float getLineAproximVoltag(float frequency);
void presetSimpleFreqToVoltag(void);
float getSimpleVoltagFromFrequency(float frequencyInv);

float satFreq, satVoltag;
float UperF;
float nominalVout;
float Fkl[4], Ukl[4], UFkl[4];
float (*UwithFreq)(float);


void presetFreqToVoltag(void){
	if(getRegister(_U_WITH_F_TYPE)){
		if(initLineAproximVoltage()){
			UwithFreq = getLineAproximVoltag;
			return;
		}
	}
	presetSimpleFreqToVoltag();
	UwithFreq = getSimpleVoltagFromFrequency;
}

float getVoltagFromFrequency(float frequencyInv) {
	if (frequencyInv < 0)
		frequencyInv = -frequencyInv;
	return (*UwithFreq)(frequencyInv);
}


void presetSimpleFreqToVoltag(void) {
	float u, f;
	u = getRegister(_SAT_VOLTAG);
	f = getRegister(_SAT_FREQ);
	u = u / 10;
	f = f / 100;
	satFreq = f;
	satVoltag = u;
	UperF = u / f;
	nominalVout = getRegister(_NOMINAL_VOLTAG);
	nominalVout = nominalVout / 10;
}

float getSimpleVoltagFromFrequency(float frequencyInv) {

	if (frequencyInv > satFreq)
		return satVoltag;
	else {
		return frequencyInv * UperF;	//simple u divide by f equally constant
	}
}

float scaleVoltagPwm(float voltag) {
	voltag *= (7400/*0x02000*/);
	voltag = voltag / nominalVout;
	return voltag;
}

float getPieceLine(char piece, float frequency){
	float voltag;
	voltag = Ukl[piece];
	voltag += frequency * UFkl[piece];
	return voltag;
}

float getLineAproximVoltag(float frequency){
	if (frequency > 0){
		if(frequency < Fkl[1]){
			return getPieceLine(0, frequency);
		}else{
			if(frequency < Fkl[2]){
				return getPieceLine(1, frequency);
			}else{
				if(frequency < Fkl[3]){
					return getPieceLine(2, frequency);
				}else{
					if(frequency < satFreq){
						return getPieceLine(3, frequency);
					}else
						return satVoltag;
				}
			}
		}
	}
	return 0;
}

char initUFklPiece(float *U, float *F, float *Uz, float *UFkl){
	float kf;
	if(F[0] == F[1])
		return 1;
	kf = (U[1] - U[0]) / (F[1] - F[0]);
	*Uz = U[1] - (F[1] * kf);
	*UFkl = kf;
	return 0;
}

char initLineAproximVoltage(void){
	float u[2], f[2];
	Fkl[0] = 0; Fkl[1] = 0;
	Fkl[2] = 0; Fkl[3] = 0;
	u[0] = getRegister(_MAGTETIC_VOLTAG);
	u[1] = getRegister(_1_BROCK_VOLTAG);
	f[0] = 0;
	f[1] = getRegister(_1_BROCK_FREQ);
	u[0] /= 10;
	u[1] /= 10;
	f[1] /= 100;
	if(initUFklPiece(u, f, &Ukl[0], &UFkl[0]))
		return 0;
	Fkl[1] = f[1];

	u[0] = getRegister(_1_BROCK_VOLTAG);
	u[1] = getRegister(_2_BROCK_VOLTAG);
	f[0] = getRegister(_1_BROCK_FREQ);
	f[1] = getRegister(_2_BROCK_FREQ);
	u[0] /= 10;
	u[1] /= 10;
	f[0] /= 100;
	f[1] /= 100;
	if(initUFklPiece(u, f, &Ukl[1], &UFkl[1]))
		return 0;
	Fkl[2] = f[1];

	u[0] = getRegister(_2_BROCK_VOLTAG);
	u[1] = getRegister(_3_BROCK_VOLTAG);
	f[0] = getRegister(_2_BROCK_FREQ);
	f[1] = getRegister(_3_BROCK_FREQ);
	u[0] /= 10;
	u[1] /= 10;
	f[0] /= 100;
	f[1] /= 100;
	if(initUFklPiece(u, f, &Ukl[2], &UFkl[2]))
		return 0;
	Fkl[3] = f[1];

	u[0] = getRegister(_3_BROCK_VOLTAG);
	u[1] = getRegister(_SAT_VOLTAG);
	f[0] = getRegister(_3_BROCK_FREQ);
	f[1] = getRegister(_SAT_FREQ);
	u[0] /= 10;
	u[1] /= 10;
	f[0] /= 100;
	f[1] /= 100;
	if(initUFklPiece(u, f, &Ukl[3], &UFkl[3]))
		return 0;

	presetSimpleFreqToVoltag();

	return 1;
}

