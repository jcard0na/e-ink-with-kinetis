################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/u8glib/u8g_bitmap.c \
../source/u8glib/u8g_circle.c \
../source/u8glib/u8g_clip.c \
../source/u8glib/u8g_com_api.c \
../source/u8glib/u8g_com_api_16gr.c \
../source/u8glib/u8g_com_i2c.c \
../source/u8glib/u8g_com_io.c \
../source/u8glib/u8g_com_null.c \
../source/u8glib/u8g_cursor.c \
../source/u8glib/u8g_delay.c \
../source/u8glib/u8g_ellipse.c \
../source/u8glib/u8g_font.c \
../source/u8glib/u8g_font_data.c \
../source/u8glib/u8g_line.c \
../source/u8glib/u8g_ll_api.c \
../source/u8glib/u8g_page.c \
../source/u8glib/u8g_pb.c \
../source/u8glib/u8g_pb8h1.c \
../source/u8glib/u8g_pb200h1.c \
../source/u8glib/u8g_polygon.c \
../source/u8glib/u8g_rect.c \
../source/u8glib/u8g_rot.c \
../source/u8glib/u8g_scale.c \
../source/u8glib/u8g_state.c \
../source/u8glib/u8g_u16toa.c \
../source/u8glib/u8g_u8toa.c \
../source/u8glib/u8g_virtual_screen.c 

OBJS += \
./source/u8glib/u8g_bitmap.o \
./source/u8glib/u8g_circle.o \
./source/u8glib/u8g_clip.o \
./source/u8glib/u8g_com_api.o \
./source/u8glib/u8g_com_api_16gr.o \
./source/u8glib/u8g_com_i2c.o \
./source/u8glib/u8g_com_io.o \
./source/u8glib/u8g_com_null.o \
./source/u8glib/u8g_cursor.o \
./source/u8glib/u8g_delay.o \
./source/u8glib/u8g_ellipse.o \
./source/u8glib/u8g_font.o \
./source/u8glib/u8g_font_data.o \
./source/u8glib/u8g_line.o \
./source/u8glib/u8g_ll_api.o \
./source/u8glib/u8g_page.o \
./source/u8glib/u8g_pb.o \
./source/u8glib/u8g_pb8h1.o \
./source/u8glib/u8g_pb200h1.o \
./source/u8glib/u8g_polygon.o \
./source/u8glib/u8g_rect.o \
./source/u8glib/u8g_rot.o \
./source/u8glib/u8g_scale.o \
./source/u8glib/u8g_state.o \
./source/u8glib/u8g_u16toa.o \
./source/u8glib/u8g_u8toa.o \
./source/u8glib/u8g_virtual_screen.o 

C_DEPS += \
./source/u8glib/u8g_bitmap.d \
./source/u8glib/u8g_circle.d \
./source/u8glib/u8g_clip.d \
./source/u8glib/u8g_com_api.d \
./source/u8glib/u8g_com_api_16gr.d \
./source/u8glib/u8g_com_i2c.d \
./source/u8glib/u8g_com_io.d \
./source/u8glib/u8g_com_null.d \
./source/u8glib/u8g_cursor.d \
./source/u8glib/u8g_delay.d \
./source/u8glib/u8g_ellipse.d \
./source/u8glib/u8g_font.d \
./source/u8glib/u8g_font_data.d \
./source/u8glib/u8g_line.d \
./source/u8glib/u8g_ll_api.d \
./source/u8glib/u8g_page.d \
./source/u8glib/u8g_pb.d \
./source/u8glib/u8g_pb200h1.d \
./source/u8glib/u8g_pb8h1.d \
./source/u8glib/u8g_polygon.d \
./source/u8glib/u8g_rect.d \
./source/u8glib/u8g_rot.d \
./source/u8glib/u8g_scale.d \
./source/u8glib/u8g_state.d \
./source/u8glib/u8g_u16toa.d \
./source/u8glib/u8g_u8toa.d \
./source/u8glib/u8g_virtual_screen.d 


# Each subdirectory must supply rules for building sources it contributes
source/u8glib/%.o: ../source/u8glib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -I../startup -I../board -I../utilities -I../CMSIS -I../drivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


