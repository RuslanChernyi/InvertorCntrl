################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
AdcCntrl/AdcCntrl.obj: ../AdcCntrl/AdcCntrl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="AdcCntrl/AdcCntrl.pp" --obj_directory="AdcCntrl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

AdcCntrl/AveragValue.obj: ../AdcCntrl/AveragValue.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="AdcCntrl/AveragValue.pp" --obj_directory="AdcCntrl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

AdcCntrl/FIR_Filtr.obj: ../AdcCntrl/FIR_Filtr.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="AdcCntrl/FIR_Filtr.pp" --obj_directory="AdcCntrl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

AdcCntrl/MessureCntrl.obj: ../AdcCntrl/MessureCntrl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="AdcCntrl/MessureCntrl.pp" --obj_directory="AdcCntrl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

AdcCntrl/OutCurrentCalc.obj: ../AdcCntrl/OutCurrentCalc.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="AdcCntrl/OutCurrentCalc.pp" --obj_directory="AdcCntrl" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


