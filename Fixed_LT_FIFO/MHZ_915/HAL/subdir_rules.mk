################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
HAL/HAL_PMM.obj: ../HAL/HAL_PMM.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.6/bin/cl430" -vmspx --abi=eabi -O2 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Chris/Documents/GitHub/Bravo_Delta_Golf_19er/Fixed_LT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.6/include" --define=MHZ_915 --define=__CC430F6147__ --diag_warning=225 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/HAL_PMM.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/RF1A.obj: ../HAL/RF1A.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.6/bin/cl430" -vmspx --abi=eabi -O2 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Chris/Documents/GitHub/Bravo_Delta_Golf_19er/Fixed_LT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.6/include" --define=MHZ_915 --define=__CC430F6147__ --diag_warning=225 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/RF1A.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

HAL/RfRegSettings.obj: ../HAL/RfRegSettings.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP430 Compiler'
	"C:/ti/ccsv5/tools/compiler/msp430_4.1.6/bin/cl430" -vmspx --abi=eabi -O2 -g --include_path="C:/ti/ccsv5/ccs_base/msp430/include" --include_path="C:/Users/Chris/Documents/GitHub/Bravo_Delta_Golf_19er/Fixed_LT_FIFO/HAL" --include_path="C:/ti/ccsv5/tools/compiler/msp430_4.1.6/include" --define=MHZ_915 --define=__CC430F6147__ --diag_warning=225 --silicon_errata=CPU18 --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU23 --silicon_errata=CPU40 --printf_support=minimal --preproc_with_compile --preproc_dependency="HAL/RfRegSettings.pp" --obj_directory="HAL" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


