################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Souln/Desktop/Projects/Board_STM32_Smart_V2.0/Libs/MotorDriver/MotorDriver.c 

OBJS += \
./Libs/MotorDriver/MotorDriver.o 

C_DEPS += \
./Libs/MotorDriver/MotorDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Libs/MotorDriver/MotorDriver.o: C:/Users/Souln/Desktop/Projects/Board_STM32_Smart_V2.0/Libs/MotorDriver/MotorDriver.c Libs/MotorDriver/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../../Core/Inc -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/STM32F1xx_HAL_Driver/Inc -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/include -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Device/ST/STM32F1xx/Include -IC:/STM32Cube/Repository/STM32Cube_FW_F1_V1.8.4/Drivers/CMSIS/Include -I"C:/Users/Souln/Desktop/Projects/Board_STM32_Smart_V2.0/Libs" -I"C:/Users/Souln/Desktop/Projects/Board_STM32_Smart_V2.0/Adapters" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Libs-2f-MotorDriver

clean-Libs-2f-MotorDriver:
	-$(RM) ./Libs/MotorDriver/MotorDriver.d ./Libs/MotorDriver/MotorDriver.o

.PHONY: clean-Libs-2f-MotorDriver

