##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.6.0] date: [Mon Mar 02 23:53:15 CST 2020]
##########################################################################################################################

TARGET = ATParse
CC=gcc


DEBUG = 1
OPT = -O0
TEST_USE=1
BUILD_DIR = build

# C sources
C_SOURCES =  \
src/test.c \
src/main.c \
platform/msp_linux.c	\
platform/serial/serial.c \
src/wrappers.c \

#src/atparse.c \
# src/common.c \
# module/air724.c \

# C includes
C_INCLUDES =  \
-Iplatform \
-Iplatform/Serial \
-Isrc \
-Imodule \

# C defines
C_DEFS =  \

#-ffunction-sections, -fdata-sections会使compiler为每个function和data item分配独立的section。
#-Wall 开启所有警告
CFLAGS = $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections  

ifeq ($(DEBUG), 1)
CFLAGS += -g 
endif

ifdef TEST_USE
TEST=-DTEST
endif

# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

#-Wl,--gc-sections 之链接已使用段
# libraries
LIBS =   -lpthread
LIBDIR = $(LIB_DIR)
LDFLAGS = $(LIBDIR) $(LIBS) -Wl,--gc-sections 

# default action: build all
all: $(TARGET) 

# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c  $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(TEST) $(CFLAGS) $< -o $@

$(TARGET): $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(BUILD_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)

env:
	@echo  "C_SOURCES": $(C_SOURCES)
	@echo  "OBJECTS": $(OBJECTS)
	@echo  "CFLAGS":  $(CFLAGS)
	@echo  "LDFLAGS"  $(LDFLAGS)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***