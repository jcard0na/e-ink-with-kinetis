RM := rm -rf
CC := arm-none-eabi-g++
BUILD_BASE_DIR = build

vpath %.c drivers board startup utilities source

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

SECONDARY_FLASH += \
e-ink-with-kinetis.hex \

SECONDARY_SIZE += \
e-ink-with-kinetis.siz \

ELF_SRCS += \
../e-ink-with-kinetis-test.elf 

MODULES   := drivers utilities board
BUILD_DIR := $(addprefix $(BUILD_BASE_DIR)/,$(MODULES))
C_SRCS    := $(foreach sdir,$(MODULES),$(wildcard $(sdir)/*.c))
OBJS      := $(addprefix $(BUILD_BASE_DIR)/, $(C_SRCS:%.c=%.o))
C_DEPS    := $(addprefix $(BUILD_BASE_DIR)/, $(C_SRCS:%.c=%.d))

C_SRCS += \
source/display.c \
source/main.c \
startup/system_MKL43Z4.c \
source/u8glib/dev/Display_EPD_W21.c \
source/u8glib/dev/Display_EPD_W21_spi.c \
source/u8glib/dev/u8g_dev_gdep015oc1.c 

OBJS += \
build/source/display.o \
build/source/main.o  \
build/startup/startup_MKL43Z4.o \
build/startup/system_MKL43Z4.o  \
build/source/u8glib/dev/Display_EPD_W21.o \
build/source/u8glib/dev/Display_EPD_W21_spi.o \
build/source/u8glib/dev/u8g_dev_gdep015oc1.o \
build/source/u8glib/u8g_bitmap.o \
build/source/u8glib/u8g_circle.o \
build/source/u8glib/u8g_clip.o \
build/source/u8glib/u8g_com_api.o \
build/source/u8glib/u8g_com_api_16gr.o \
build/source/u8glib/u8g_com_i2c.o \
build/source/u8glib/u8g_com_io.o \
build/source/u8glib/u8g_com_null.o \
build/source/u8glib/u8g_cursor.o \
build/source/u8glib/u8g_delay.o \
build/source/u8glib/u8g_ellipse.o \
build/source/u8glib/u8g_font.o \
build/source/u8glib/u8g_font_data.o \
build/source/u8glib/u8g_line.o \
build/source/u8glib/u8g_ll_api.o \
build/source/u8glib/u8g_page.o \
build/source/u8glib/u8g_pb.o \
build/source/u8glib/u8g_pb200h1.o \
build/source/u8glib/u8g_pb32h1.o \
build/source/u8glib/u8g_pb8h1.o \
build/source/u8glib/u8g_polygon.o \
build/source/u8glib/u8g_rect.o \
build/source/u8glib/u8g_rot.o \
build/source/u8glib/u8g_scale.o \
build/source/u8glib/u8g_state.o \
build/source/u8glib/u8g_u16toa.o \
build/source/u8glib/u8g_u8toa.o \
build/source/u8glib/u8g_virtual_screen.o 

C_DEPS += \
build/source/display.d \
build/source/main.d \
build/startup/system_MKL43Z4.d \
build/source/u8glib/dev/Display_EPD_W21.d \
build/source/u8glib/dev/Display_EPD_W21_spi.d \
build/source/u8glib/dev/u8g_dev_gdep015oc1.d 

S_UPPER_SRCS += \
startup/startup_MKL43Z4.S 

S_UPPER_DEPS += \
build/startup/startup_MKL43Z4.d 

O_SRCS += \
source/u8glib/u8g_bitmap.o \
source/u8glib/u8g_circle.o \
source/u8glib/u8g_clip.o \
source/u8glib/u8g_com_api.o \
source/u8glib/u8g_com_api_16gr.o \
source/u8glib/u8g_com_i2c.o \
source/u8glib/u8g_com_io.o \
source/u8glib/u8g_com_null.o \
source/u8glib/u8g_cursor.o \
source/u8glib/u8g_delay.o \
source/u8glib/u8g_ellipse.o \
source/u8glib/u8g_font.o \
source/u8glib/u8g_font_data.o \
source/u8glib/u8g_line.o \
source/u8glib/u8g_ll_api.o \
source/u8glib/u8g_page.o \
source/u8glib/u8g_pb.o \
source/u8glib/u8g_pb200h1.o \
source/u8glib/u8g_pb32h1.o \
source/u8glib/u8g_pb8h1.o \
source/u8glib/u8g_polygon.o \
source/u8glib/u8g_rect.o \
source/u8glib/u8g_rot.o \
source/u8glib/u8g_scale.o \
source/u8glib/u8g_state.o \
source/u8glib/u8g_u16toa.o \
source/u8glib/u8g_u8toa.o \
source/u8glib/u8g_virtual_screen.o 

all: checkdirs e-ink-with-kinetis.elf secondary-outputs

build/source/u8glib/dev/%.o: source/u8glib/dev/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/source/u8glib/%.o: source/u8glib/%.c
	@echo 'Building file:  $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/source/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/utilities/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/board/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/startup/%.o: startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/startup/%.o: startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

build/drivers/%.o: %.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

e-ink-with-kinetis.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -T "MKL43Z256xxx4_flash.ld" -Xlinker --gc-sections -Wl,-Map,"e-ink-with-kinetis.map" -specs=nosys.specs -specs=nano.specs -Xlinker -z -Xlinker muldefs -o "e-ink-with-kinetis.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

e-ink-with-kinetis.hex: e-ink-with-kinetis.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex "e-ink-with-kinetis.elf"  "e-ink-with-kinetis.hex"
	@echo 'Finished building: $@'
	@echo ' '

e-ink-with-kinetis.siz: e-ink-with-kinetis.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "e-ink-with-kinetis.elf"
	@echo 'Finished building: $@'
	@echo ' '

clean:
	-$(RM) $(SECONDARY_SIZE)$(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(SECONDARY_FLASH)$(CPP_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS)$(S_UPPER_DEPS) $(BUILD_BASE_DIR)  e-ink-with-kinetis.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

checkdirs:
	@mkdir -p build/drivers/
	@mkdir -p build/source/
	@mkdir -p build/board/
	@mkdir -p build/startup/
	@mkdir -p build/utilities/
	@mkdir -p build/source/u8glib/dev/

.PHONY: all clean dependents checkdirs
.SECONDARY:

