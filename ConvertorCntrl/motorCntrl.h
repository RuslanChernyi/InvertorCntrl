/* ******************************************************************
 * ��������� ���������� ������������ ������� � ����������
 * ******************************************************************/

#ifndef ___MOTOR_CNTRL_H___
#define ___MOTOR_CNTRL_H___

UINT16 controlPwm(float *taskF, float *taskV,
		          float *currentF, float *currentV);
void getMotorFrequency(float task, float *frequency);
void getMotorVoltag(float frequencyInv, float frequencyMotor,
		            float *voltagInv, float *voltagMotor);
void Accelaration(float task, float *freqTask, float *freqMotor);
void Decelaration(float task, float *freqTask, float *freqMotor);

int getAccelTime(void);
int getDecelTime(void);

void rstDecelReg(void);

float getStepDecelCheng(void);
float getStepAccelCheng(void);
void presetAccelDecel(void);
void setTimeAccel(int time);
void setTimeDecel(int time);

void rstCntrlPwm(void);

void presetFreqToVoltag(void);
float getVoltagFromFrequency(float frequencyInv);
float scaleVoltagPwm(float voltag);

void DecelarationStop(float *freqTask, float *freqMotor);

void cntrlMotorCntrlPause(float *taskF, float *taskV, float *currentF, float *currentV);
void AccelarationPause(float task, float *freqTask, float *freqMotor);
void DecelarationPause(float task, float *freqTask, float *freqMotor);
//#define getCorrectionVoltag(frequencyMotor) 0

void initEconomParam(void);
char cntrlEconomPwr(float *voltag);

#endif
