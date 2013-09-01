################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
RF_Toggle_LED_Demo.obj: ../RF_Toggle_LED_Demo.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.5/bin/cl430" -vmspx --abi=eabi -O2 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Chris/workspace_v5_3/Fixed_LT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.5/include" --define=MHZ_915 --define=__CC430F6147__ --diag_warning=225 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="RF_Toggle_LED_Demo.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

debug.obj: ../debug.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.5/bin/cl430" -vmspx --abi=eabi -O2 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Chris/workspace_v5_3/Fixed_LT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.5/include" --define=MHZ_915 --define=__CC430F6147__ --diag_warning=225 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="debug.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


