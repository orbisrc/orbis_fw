# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# version
######################################


FW_VER_MAJOR := $(shell grep " FW_VERSION_MAJOR" src/version.h | awk '{print $$3}' )
FW_VER_MINOR := $(shell grep " FW_VERSION_MINOR" src/version.h | awk '{print $$3}' )
FW_VER_PATCH  := $(shell grep " FW_VERSION_PATCH" src/version.h | awk '{print $$3}' )

FW_VER := $(FW_VER_MAJOR).$(FW_VER_MINOR).$(FW_VER_PATCH)

VERSION := $(FW_VER)

REVISION := $(shell git log -1 --format="%h")

######################################
# target
######################################

TARGET_PCB = air

TARGET = _orbis_$(TARGET_PCB)_$(VERSION)

######################################
# building variables
######################################
# debug build?
DEBUG = 0
# optimization
OPT = -Ofast -Wall
#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
FW_DIR = 	FW
TOOLS_DIR = tools
DFUSE-PACK  := utils/dfuse-pack.py

######################################
# source
######################################
# C sources

C_SOURCES =  $(shell find $(ROOT) -name *.c)

# ASM sources
ASM_SOURCES =  \
startup_stm32f407xx.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

SF=ST-LINK_CLI
DFU=HEX2DFU.exe
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4

# fpu
FPU = -mfpu=fpv4-sp-d16

# float-abi
FLOAT-ABI = -mfloat-abi=hard  -u _printf_float

# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F407xx \
-DDEBUG

# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-Ilib \
-Isrc \
-Ilib/NimaLTD/Inc \
-Ilib/ST/STM32_USB_Device_Library/Core/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/CDC/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/CustomHID/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/DFU/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/HID/Inc \
-Ilib/ST/STM32_USB_Device_Library/Class/MSC/Inc \
-Ilib/Third_Party/FatFs/src \
-Ilib/STM32F4xx_HAL_Driver/Inc \
-Ilib/STM32F4xx_HAL_Driver/Inc/Legacy \
-Ilib/CMSIS/Device/ST/STM32F4xx/Include \
-Ilib/CMSIS/Include \
-Isrc/cli \
-Isrc/core \
-Isrc/fatio \
-Isrc/gui \
-Isrc/gui/dispdriver \
-Isrc/gui/fonts \
-Isrc/gui/screens \
-Isrc/usb \


# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F407VETx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys 
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

.PHONY: all

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo $<
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo $<
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@

#######################################
# make DFU
#######################################
.PHONY: dfu
dfu:

	$(TOOLS_DIR)/$(DFU)  $(FW_DIR)/$(TARGET).hex -out $(FW_DIR)/$(TARGET).dfu


#######################################
# copy to FW folder
#######################################
copy:

	-cp $(BUILD_DIR)/$(TARGET).hex $(FW_DIR)/$(TARGET).hex

#######################################
# clean up
#######################################
.PHONY: clean
clean:
	-rm -fR $(BUILD_DIR)


#######################################
# remake
#######################################
.PHONY: remake
remake:
	clean all
  
 #######################################
# remake
#######################################m
.PHONY: flash
flash:
	$(SF) -P $(BUILD_DIR)/$(TARGET).bin 0x8000000 -Rst
   
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
