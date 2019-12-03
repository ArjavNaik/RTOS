################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.c 

OBJS += \
./Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.o 

C_DEPS += \
./Third-Party/FreeRTOS/org/Source/portable/MemMang/heap_4.d 


# Each subdirectory must supply rules for building sources it contributes
Third-Party/FreeRTOS/org/Source/portable/MemMang/%.o: ../Third-Party/FreeRTOS/org/Source/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_STDPERIPH_DRIVER -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/StdPeriph_Driver/inc" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Third-Party/SEGGER/Config" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Third-Party/SEGGER/OS" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Third-Party/SEGGER/SEGGER" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Config" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Third-Party/FreeRTOS/org/Source/portable/GCC/ARM_CM4F" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/Third-Party/FreeRTOS/org/Source/include" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/inc" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/CMSIS/device" -I"C:/Users/Arjav/workspace/STM32_FreeRTOS_binarsema/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


