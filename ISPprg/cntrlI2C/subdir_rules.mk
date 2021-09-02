################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
cntrlI2C/EEControl.obj: ../cntrlI2C/EEControl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="cntrlI2C/EEControl.pp" --obj_directory="cntrlI2C" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

cntrlI2C/I2Ccontrol.obj: ../cntrlI2C/I2Ccontrol.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="cntrlI2C/I2Ccontrol.pp" --obj_directory="cntrlI2C" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

cntrlI2C/I2Ccore.obj: ../cntrlI2C/I2Ccore.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="cntrlI2C/I2Ccore.pp" --obj_directory="cntrlI2C" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

cntrlI2C/ResetEprom.obj: ../cntrlI2C/ResetEprom.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="cntrlI2C/ResetEprom.pp" --obj_directory="cntrlI2C" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


