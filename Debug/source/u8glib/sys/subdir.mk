################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/u8glib/sys/u8g_kinetis.c 

OBJS += \
./source/u8glib/sys/u8g_kinetis.o 

C_DEPS += \
./source/u8glib/sys/u8g_kinetis.d 


# Each subdirectory must supply rules for building sources it contributes
source/u8glib/sys/%.o: ../source/u8glib/sys/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -I../startup -I../board -I../utilities -I../CMSIS -I../drivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


