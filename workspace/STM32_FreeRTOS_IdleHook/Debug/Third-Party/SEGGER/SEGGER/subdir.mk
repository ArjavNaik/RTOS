################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/SEGGER/SEGGER/SEGGER_RTT.c \
../Third-Party/SEGGER/SEGGER/SEGGER_SYSVIEW.c 

OBJS += \
./Third-Party/SEGGER/SEGGER/SEGGER_RTT.o \
./Third-Party/SEGGER/SEGGER/SEGGER_SYSVIEW.o 

C_DEPS += \
./Third-Party/SEGGER/SEGGER/SEGGER_RTT.d \
./Third-Party/SEGGER/SEGGER/SEGGER_SYSVIEW.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/SEGGER/SEGGER/%.o: ../Third-Party/SEGGER/SEGGER/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/StdPeriph_Driver/inc" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Third-Party/SEGGER/Config" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Third-Party/SEGGER/OS" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Third-Party/SEGGER/SEGGER" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Config" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/inc" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/CMSIS/device" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_IdleHook/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


