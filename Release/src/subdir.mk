################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/BLE_Module.c \
../src/BLE_Test.c \
../src/Communications.c \
../src/LibIIO_Interface.c \
../src/LibIIO_Utility.c \
../src/Placeholder_TempInterface.c 

SRC_OBJS += \
./src/BLE_Module.o \
./src/BLE_Test.o \
./src/Communications.o \
./src/LibIIO_Interface.o \
./src/LibIIO_Utility.o \
./src/Placeholder_TempInterface.o 

C_DEPS += \
./src/BLE_Module.d \
./src/BLE_Test.d \
./src/Communications.d \
./src/LibIIO_Interface.d \
./src/LibIIO_Utility.d \
./src/Placeholder_TempInterface.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore GCC ARM Embedded C Compiler'
	arm-none-eabi-gcc -O2 -ffunction-sections -fdata-sections -D_RTE_ -D__ADUCM3029__ -D__SILICON_REVISION__=0x100 -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\system" -I"E:/Analog Devices/CrossCore Embedded Studio 2.5.0/ARM/packs/ARM/CMSIS/4.5.0/CMSIS/Include" -I"E:/Analog Devices/CrossCore Embedded Studio 2.5.0/ARM/packs/AnalogDevices/ADuCM302x_DFP/1.0.3/Include" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test/RTE" -I"E:\users\jake\Documents\GitHub\CrossCore\BLE_Test/RTE/Device/ADuCM3029" -Wall -c -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


