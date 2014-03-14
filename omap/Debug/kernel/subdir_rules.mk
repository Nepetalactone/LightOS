################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
kernel/interrupt.obj: ../kernel/interrupt.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"E:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me --include_path="E:/ti/ccsv5/tools/compiler/arm_5.1.1/include" -g --define=omap3530 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="kernel/interrupt.pp" --obj_directory="kernel" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

kernel/timer.obj: ../kernel/timer.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"E:/ti/ccsv5/tools/compiler/arm_5.1.1/bin/armcl" -mv7A8 --code_state=32 --abi=eabi -me --include_path="E:/ti/ccsv5/tools/compiler/arm_5.1.1/include" -g --define=omap3530 --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="kernel/timer.pp" --obj_directory="kernel" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


