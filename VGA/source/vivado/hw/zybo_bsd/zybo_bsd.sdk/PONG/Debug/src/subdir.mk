################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/display_ctrl.c \
../src/main.c \
../src/pong.c \
../src/timer_ps.c 

LD_SRCS += \
../src/lscript.ld 

OBJS += \
./src/display_ctrl.o \
./src/main.o \
./src/pong.o \
./src/timer_ps.o 

C_DEPS += \
./src/display_ctrl.d \
./src/main.d \
./src/pong.d \
./src/timer_ps.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM gcc compiler'
	arm-xilinx-eabi-gcc -Wall -O0 -g3 -c -fmessage-length=0 -I../../pong_bsp/ps7_cortexa9_0/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


