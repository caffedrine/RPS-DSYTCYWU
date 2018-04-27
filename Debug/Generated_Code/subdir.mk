################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Generated_Code/ADC.c \
../Generated_Code/AdcLdd1.c \
../Generated_Code/BitIoLdd1.c \
../Generated_Code/BitIoLdd2.c \
../Generated_Code/BitIoLdd3.c \
../Generated_Code/Cpu.c \
../Generated_Code/DebugConsole.c \
../Generated_Code/ErrorsSignaling.c \
../Generated_Code/IO1.c \
../Generated_Code/LedGreen.c \
../Generated_Code/LedRed.c \
../Generated_Code/LedYellow.c \
../Generated_Code/PE_LDD.c \
../Generated_Code/TU1.c \
../Generated_Code/TimerIntLdd1.c \
../Generated_Code/Vectors.c 

OBJS += \
./Generated_Code/ADC.o \
./Generated_Code/AdcLdd1.o \
./Generated_Code/BitIoLdd1.o \
./Generated_Code/BitIoLdd2.o \
./Generated_Code/BitIoLdd3.o \
./Generated_Code/Cpu.o \
./Generated_Code/DebugConsole.o \
./Generated_Code/ErrorsSignaling.o \
./Generated_Code/IO1.o \
./Generated_Code/LedGreen.o \
./Generated_Code/LedRed.o \
./Generated_Code/LedYellow.o \
./Generated_Code/PE_LDD.o \
./Generated_Code/TU1.o \
./Generated_Code/TimerIntLdd1.o \
./Generated_Code/Vectors.o 

C_DEPS += \
./Generated_Code/ADC.d \
./Generated_Code/AdcLdd1.d \
./Generated_Code/BitIoLdd1.d \
./Generated_Code/BitIoLdd2.d \
./Generated_Code/BitIoLdd3.d \
./Generated_Code/Cpu.d \
./Generated_Code/DebugConsole.d \
./Generated_Code/ErrorsSignaling.d \
./Generated_Code/IO1.d \
./Generated_Code/LedGreen.d \
./Generated_Code/LedRed.d \
./Generated_Code/LedYellow.d \
./Generated_Code/PE_LDD.d \
./Generated_Code/TU1.d \
./Generated_Code/TimerIntLdd1.d \
./Generated_Code/Vectors.d 


# Each subdirectory must supply rules for building sources it contributes
Generated_Code/%.o: ../Generated_Code/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -I"/home/curiosul/Workspace/KDS/battery-charger/Static_Code/PDD" -I"/home/curiosul/Workspace/KDS/battery-charger/Static_Code/IO_Map" -I"/home/curiosul/Workspace/KDS/battery-charger/Sources" -I"/home/curiosul/Workspace/KDS/battery-charger/Generated_Code" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


