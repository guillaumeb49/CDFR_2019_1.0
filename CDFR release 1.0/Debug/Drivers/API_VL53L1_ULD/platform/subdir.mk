################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/API_VL53L1_ULD/platform/vl53l1_platform.c 

OBJS += \
./Drivers/API_VL53L1_ULD/platform/vl53l1_platform.o 

C_DEPS += \
./Drivers/API_VL53L1_ULD/platform/vl53l1_platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/API_VL53L1_ULD/platform/vl53l1_platform.o: ../Drivers/API_VL53L1_ULD/platform/vl53l1_platform.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F429xx -c -I../Middlewares/Third_Party/LwIP/src/include/lwip/priv -I../Middlewares/Third_Party/LwIP/src/include/netif/ppp -I../Middlewares/Third_Party/LwIP/src/include/posix -I../Middlewares/Third_Party/LwIP/src/include/posix/sys -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Middlewares/Third_Party/LwIP/src/include/lwip/prot -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip/apps -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/LwIP/system/arch -I../Middlewares/Third_Party/LwIP/src/include -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/Third_Party/LwIP/src/include/lwip -I../Middlewares/Third_Party/LwIP/system -I../Middlewares/Third_Party/LwIP/src/include/netif -I"/home/guillaume/Documents/CDFR_2019/CDFR_2019_1.0/CDFR release 1.0/Drivers/API_VL53L1_ULD/core" -I"/home/guillaume/Documents/CDFR_2019/CDFR_2019_1.0/CDFR release 1.0/Drivers/API_VL53L1_ULD/platform" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/API_VL53L1_ULD/platform/vl53l1_platform.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

