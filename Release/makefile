################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := ${ComSpec}

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include system/pinmux/GeneratedSources/subdir.mk
-include system/subdir.mk
-include src/LibIIO_Redux/subdir.mk
-include src/BLE\ Source/subdir.mk
-include src/ADuCM3029\ Pwr/subdir.mk
-include src/ADuCM3029\ Comms/subdir.mk
-include src/subdir.mk
-include RTE/Device/ADuCM3029/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
OBJS := $(SRC_OBJS)
# Quote makefile variable : OBJS 
OBJS := $(subst \ ,@, $(OBJS))
OBJS := $(foreach var, $(OBJS), "$(subst @, ,$(var))")
# Quote makefile variable : C_DEPS 
C_DEPS := $(subst \ ,@, $(C_DEPS))
C_DEPS := $(foreach var, $(C_DEPS), "$(subst @, ,$(var))")

# All Target
all: BLE_Test

# Tool invocations
BLE_Test: $(USER_OBJS) $(SRC_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: CrossCore GCC ARM Embedded C Linker'
	arm-none-eabi-gcc -TE:\users\jake\Documents\GitHub\CrossCore\BLE_Test/RTE/Device/ADuCM3029/ADuCM3029.ld -Wl,--gc-sections -mcpu=cortex-m3 -mthumb -specs=nosys.specs -o  "BLE_Test" @input-file.txt $(USER_OBJS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) BLE_Test
	-@echo ' '

post-build:
	-arm-none-eabi-objcopy "E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\Release\BLE_Test" -O ihex E:\users\jake\Documents\GitHub\CrossCore\BLE_Test\Release\BLE_Test.hex
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
