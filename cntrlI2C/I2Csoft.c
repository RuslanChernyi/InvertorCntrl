/*

** ��������������� ���������� ������ MSSP - I2C

*/

#include "DSP2833x_Device.h"

#include "I2Ccore.h"
#include "added.h"

#define SDA_PIN() GpioDataRegs.GPBDAT.bit.GPIO32
#define SCL_PIN() GpioDataRegs.GPBDAT.bit.GPIO33

#define setI2C_SCL() GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0
#define clrI2C_SCL() GpioCtrlRegs.GPBDIR.bit.GPIO33 = 1
#define setI2C_SDA() GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0
#define clrI2C_SDA() GpioCtrlRegs.GPBDIR.bit.GPIO32 = 1

#define Pause4us() I2CDelay(110)
#define Pause1us() I2CDelay(24)

Uint32 state_i2c = 0;

void initGPIOPinI2C(void){
	EALLOW;
/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBPUD.bit.GPIO32 = 0;    // Enable pull-up for GPIO32 (SDAA)
	GpioCtrlRegs.GPBPUD.bit.GPIO33 = 0;	   // Enable pull-up for GPIO33 (SCLA)

/* Set qualification for selected pins to asynch only */
// This will select asynch (no qualification) for the selected pins.
// Comment out other unwanted lines.

	GpioCtrlRegs.GPBQSEL1.bit.GPIO32 = 2;  // Asynch input GPIO32 (SDAA)
	GpioCtrlRegs.GPBQSEL1.bit.GPIO33 = 2;  // Asynch input GPIO33 (SCLA)

/* Configure SCI pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be I2C functional pins.
// Comment out other unwanted lines.

	GpioDataRegs.GPBCLEAR.bit.GPIO33 = 1;
	GpioDataRegs.GPBCLEAR.bit.GPIO32 = 1;
	GpioCtrlRegs.GPBDIR.bit.GPIO33 = 0;
	GpioCtrlRegs.GPBDIR.bit.GPIO32 = 0;

	GpioCtrlRegs.GPBMUX1.bit.GPIO32 = 0;   // Configure GPIO32 for SDAA operation
	GpioCtrlRegs.GPBMUX1.bit.GPIO33 = 0;   // Configure GPIO33 for SCLA operation

	EDIS;
}


void InitI2CSoft(void){
	initGPIOPinI2C();
}

void I2CDelay(Uint16 dl){
	while(dl)
		dl--;
}

char testIdle_I2C(void)
{
  return 0;
}

//------------------------------

char PulseSCL (void)
{
  setI2C_SCL();
  Pause4us();
  if(SCL_PIN())
  {
    clrI2C_SCL();
    Pause4us();
    return 0;
  }
  return 1;
}

//------------------------------

void presetSCL(void)
{
  int i = 0;
  while(i<9)
  {
    if(!PulseSCL())
      i++;
  }
}

//------------------------------

char StartI2C (void)
{
  setI2C_SCL();
  Pause4us();
  if(!SCL_PIN())
  {
    return 1;
  }
  clrI2C_SDA();
  Pause4us();
  clrI2C_SCL();
  Pause1us();
  return 0;
}

//------------------------------

char StopI2C (void)
{
  clrI2C_SDA();
  Pause1us();
  setI2C_SCL();
  Pause4us();
  if(!SCL_PIN())
  {
    clrI2C_SCL();
    return 1;
  }
  setI2C_SDA();
  Pause4us();
  return 0;
}

//------------------------------

char ReStartI2C(void)
{
  
  setI2C_SDA();
  Pause1us();
  setI2C_SCL();
  Pause4us();
  if((!SDA_PIN()) || (!SCL_PIN()))
    return 1;
    
  Pause4us();
  if(StartI2C())
    return 1;
  else
    return 0;
}

//------------------------------

char i2c_trsmt_byte(UINT8 trsmt)
{
  char cnt;
  for(cnt=0; cnt<8; cnt++)
  {
    if(trsmt&0x080)
      setI2C_SDA();
    else
      clrI2C_SDA();
    Pause1us();
    trsmt <<= 1;
    while(PulseSCL())
      state_i2c |= _SCL_LATCHING;
  }
  setI2C_SDA();
  Pause4us();
  setI2C_SCL();
  Pause1us();
  while(!SCL_PIN())
    state_i2c |= _SCL_LATCHING;
  Pause4us();
  if(SDA_PIN())
  {
    cnt = 1;
    state_i2c |= _ACK_BIT;
  }
  else
  {
    cnt = 0;
    state_i2c &= ~_ACK_BIT;
  }
  clrI2C_SCL();
  Pause1us();
  return cnt;
}

//------------------------------

char i2c_rcv_byte(UINT8 *rcv)
{
  char cnt;
  UINT8 msk = 0x080;
  UINT8 data = 0;
  
  setI2C_SDA();
  for(cnt=0; cnt<8; cnt++)
  {
    Pause4us();
    setI2C_SCL();
    Pause1us();
    while(!SCL_PIN())
      state_i2c |= _SCL_LATCHING;
    Pause4us();
    if(SDA_PIN())
      data |= msk;
    else
      data &= ~msk;
    clrI2C_SCL();
    Pause1us();
    msk >>= 1;
  }
  *rcv = data;
  return 0;
}

//------------------------------

char i2c_set_ack(UINT8 ack)
{
  UINT8 tmp;
  if(ack)
    setI2C_SDA();
  else
    clrI2C_SDA();
  Pause4us();
  setI2C_SCL();
  Pause4us();
  while(!SCL_PIN())
    state_i2c |= _SCL_LATCHING;
  
  if(SDA_PIN() && ack)
    tmp = 0;
  else
  {
    if(!ack)
      tmp = 0;
    else
      tmp = 1;
  }
  clrI2C_SCL();
  Pause1us();
  return tmp;
}
