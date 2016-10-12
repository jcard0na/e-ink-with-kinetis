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

MODULES   := drivers utilities board source source/u8glib/dev source/u8glib startup
BUILD_DIR := $(addprefix $(BUILD_BASE_DIR)/,$(MODULES))
C_SRCS    := $(foreach sdir,$(MODULES),$(wildcard $(sdir)/*.c))
OBJS      := $(addprefix $(BUILD_BASE_DIR)/, $(C_SRCS:%.c=%.o))
C_DEPS    := $(addprefix $(BUILD_BASE_DIR)/, $(C_SRCS:%.c=%.d))

OBJS += \
build/startup/startup_MKL43Z4.o \
build/startup/system_MKL43Z4.o  \

S_UPPER_SRCS += \
build/startup/startup_MKL43Z4.S 

S_UPPER_DEPS += \
build/startup/startup_MKL43Z4.d 

all: checkdirs e-ink-with-kinetis.elf secondary-outputs

build/startup/%.o: startup/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
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

.PHONY: all clean dependents checkdirs
.SECONDARY:

define make-goal
$1/%.o: %.c
	@echo '(make goal) Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g3 -D"CPU_MKL43Z256VMP4" -Istartup -Iboard -Iutilities -ICMSIS -Idrivers -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$$@" "$$<"
	@echo 'Finished building: $<'
	@echo ' '
endef

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))
