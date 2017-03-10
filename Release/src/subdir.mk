################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BLE_Test.c 

SRC_OBJS += \
./src/BLE_Test.o 

C_DEPS += \
./src/BLE_Test.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore GCC ARM Embedded C Compiler'
	arm-none-eabi-gcc -O2 -ffunction-sections -fdata-sections -D_RTE_ -D__ADUCM3029__ -D__SILICON_REVISION__=0x100 -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\system" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\src\ADuCM3029 Comms" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\src\BLE Source" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\src\LibIIO_Redux" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\src\ADuCM3029 Pwr" -I"E:/Analog Devices/CrossCore Embedded Studio 2.5.0/ARM/packs/ARM/CMSIS/4.5.0/CMSIS/Include" -I"E:/Analog Devices/CrossCore Embedded Studio 2.5.0/ARM/packs/AnalogDevices/ADuCM302x_DFP/1.0.3/Include" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test/RTE" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test/RTE/Device/ADuCM3029" -Wall -c -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


