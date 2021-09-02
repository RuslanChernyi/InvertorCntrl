################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
UART/Flow485Cntrl.obj: ../UART/Flow485Cntrl.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/Flow485Cntrl.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/Rs485hub.obj: ../UART/Rs485hub.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/Rs485hub.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/UartAdded.obj: ../UART/UartAdded.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/UartAdded.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/UartRs485.obj: ../UART/UartRs485.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/UartRs485.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/cntrlTransfUartB.obj: ../UART/cntrlTransfUartB.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/cntrlTransfUartB.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/cntrlTransferUart_485.obj: ../UART/cntrlTransferUart_485.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/cntrlTransferUart_485.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/cntrlUART.obj: ../UART/cntrlUART.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/cntrlUART.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/cntrlUartTransfer.obj: ../UART/cntrlUartTransfer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/cntrlUartTransfer.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '

UART/rs485Timer.obj: ../UART/rs485Timer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --include_path="/opt/ti/ccsv6/tools/compiler/c2000_6.4.11/include" --include_path="/home/test/TiTest/InvertorCntrl" --include_path="${workspace_loc:/InvertorCntrl/DSP2833x_headers/include" -g --diag_warning=225 --display_error_number --diag_wrap=off --asm_listing --preproc_with_compile --preproc_dependency="UART/rs485Timer.pp" --obj_directory="UART" $(GEN_OPTS__FLAG) "$(shell echo $<)"
	@echo 'Finished building: $<'
	@echo ' '


