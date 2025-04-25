#---------------------------------------------------------------------------------
# Clear the implicit built-in rules
#---------------------------------------------------------------------------------
.SUFFIXES:

#---------------------------------------------------------------------------------
# Check devkitPPC environment
#---------------------------------------------------------------------------------
ifeq ($(strip $(DEVKITPPC)),)
$(error "Please set DEVKITPPC in your environment. export DEVKITPPC=<path to>devkitPPC")
endif

include $(DEVKITPPC)/wii_rules

#---------------------------------------------------------------------------------
# Project settings
#---------------------------------------------------------------------------------
TARGET      := $(notdir $(CURDIR))
BUILD       := build
SOURCES     := source
DATA        := data
INCLUDES    := include

#---------------------------------------------------------------------------------
# Compilation flags
#---------------------------------------------------------------------------------
CFLAGS      := -g -O2 -Wall $(MACHDEP) $(INCLUDE)
CXXFLAGS    := $(CFLAGS)
LDFLAGS     := -g $(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# External libraries
#---------------------------------------------------------------------------------
LIBS := -lgrrlib -lpngu -lpng -ljpeg -lfreetype -lwiiuse -lbte -logc -lm -lfat
LIBDIRS := $(PORTLIBS)

#---------------------------------------------------------------------------------
# Intermediate configuration
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT    := $(CURDIR)/$(TARGET)
export VPATH     := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                    $(foreach dir,$(DATA),$(CURDIR)/$(dir))
export DEPSDIR   := $(CURDIR)/$(BUILD)

# Gather source and binary resources
CFILES     := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES   := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
sFILES     := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
SFILES     := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.S)))
BINFILES := $(foreach dir,$(DATA),$(wildcard $(dir)/*.ttf)) \
            $(foreach dir,$(DATA),$(wildcard $(dir)/*.png))

# Set linker depending on language
ifeq ($(strip $(CPPFILES)),)
  export LD := $(CC)
else
  export LD := $(CXX)
endif

# Object files
export OFILES_BIN     := $(addsuffix .o,$(BINFILES))
export OFILES_SOURCES := $(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(sFILES:.s=.o) $(SFILES:.S=.o)
export OFILES         := $(OFILES_BIN) $(OFILES_SOURCES)
export HFILES         := $(addsuffix .h,$(subst .,_,$(BINFILES)))

# Include and lib paths
export INCLUDE  := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                   $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                   -I$(CURDIR)/$(BUILD) \
                   -I$(LIBOGC_INC)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib) \
                   -L$(LIBOGC_LIB)

.PHONY: $(BUILD) clean

%.ttf.o:
	@echo "Embedding $<..."
	@$(bin2o) $< $@

%.png.o:
	@echo "Embedding $<..."
	@$(bin2o) $< $@

#---------------------------------------------------------------------------------
# Build directory
#---------------------------------------------------------------------------------
$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
# Clean rule
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol

else

DEPENDS := $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# Linking
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif