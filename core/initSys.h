/* ********************************************************
 * инициализация ядра системы
 * ********************************************************/

#ifndef ___INIT_SYS_H___
#define ___INIT_SYS_H___

void InitSysCtrl(void);
void DisableDog(void);
void InitFlash(void);
void InitPieCtrl(void);

extern const struct PIE_VECT_TABLE PieVectTableInit;	// Pie vector table (non-BIOS only)

extern Uint16 secureRamFuncs_loadstart;
extern Uint16 secureRamFuncs_loadsize;
extern Uint16 secureRamFuncs_runstart;
extern Uint16 RamfuncsLoadStart;
extern Uint16 RamfuncsLoadSize;
extern Uint16 RamfuncsRunStart;

extern void usDelay(Uint16);

#define ADC_cal_func_ptr (void (*)(void))0x380080

#define CPU_FRQ_100MHZ /* частота CPU */ 1

#endif
