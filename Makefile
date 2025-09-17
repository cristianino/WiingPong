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
SOURCES     := source source/input source/physics source/rendering source/audio source/gamestate source/assets
DATA        := data
INCLUDES    := include

# Enable FAT and device support
HAVE_FAT := 1
LIBSD := 1

#---------------------------------------------------------------------------------
# Compilation flags
#---------------------------------------------------------------------------------
CFLAGS      := -g -O2 -Wall $(MACHDEP) $(INCLUDE)
CXXFLAGS    := $(CFLAGS)
LDFLAGS     := -g $(MACHDEP) -Wl,-Map,$(notdir $@).map

#---------------------------------------------------------------------------------
# External libraries
#---------------------------------------------------------------------------------
LIBS := -lgrrlib -lfreetype -lpngu -lpng -ljpeg -lz -lbz2 -lfat -lwiiuse -lbte -lasnd -logc -lm
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

.PHONY: $(BUILD) clean package release

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
# Package for SD card
#---------------------------------------------------------------------------------
package: $(BUILD)
	@echo "Creating SD package..."
	@mkdir -p apps/$(TARGET)
	@cp $(OUTPUT).dol apps/$(TARGET)/boot.dol
	@echo "Copying data files..."
	@if [ -d "data" ]; then cp -r data apps/$(TARGET)/; fi
	@echo "Copying icon..."
	@if [ -f "icon_banner.png" ]; then cp icon_banner.png apps/$(TARGET)/icon.png; fi
	@echo "Creating meta.xml..."
	@echo '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>' > apps/$(TARGET)/meta.xml
	@echo '<app version="1">' >> apps/$(TARGET)/meta.xml
	@echo '  <name>$(TARGET)</name>' >> apps/$(TARGET)/meta.xml
	@echo '  <coder>cristianino</coder>' >> apps/$(TARGET)/meta.xml
	@echo '  <version>1.0.0</version>' >> apps/$(TARGET)/meta.xml
	@echo '  <release_date>$(shell date +%Y%m%d)</release_date>' >> apps/$(TARGET)/meta.xml
	@echo '  <short_description>Pong game for Nintendo Wii</short_description>' >> apps/$(TARGET)/meta.xml
	@echo '  <long_description>A classic Pong game implementation for Nintendo Wii using Wiimote controls.</long_description>' >> apps/$(TARGET)/meta.xml
	@echo '  <ahb_access/>' >> apps/$(TARGET)/meta.xml
	@echo '</app>' >> apps/$(TARGET)/meta.xml
	@echo "Package created in apps/$(TARGET)/"
	@echo "Copy the 'apps' folder to the root of your SD card"

#---------------------------------------------------------------------------------
# Create release package
#---------------------------------------------------------------------------------
release: package
	@echo "Creating release packages..."
	@mkdir -p release
	@echo "Creating standard release (with apps/ folder)..."
	@tar -czf release/$(TARGET)-$(shell date +%Y%m%d).tar.gz apps/
	@zip -r release/$(TARGET)-$(shell date +%Y%m%d).zip apps/
	@echo "Creating SD-ready release (direct extract to SD root)..."
	@cd apps && tar -czf ../release/$(TARGET)-SD-ready-$(shell date +%Y%m%d).tar.gz *
	@cd apps && zip -r ../release/$(TARGET)-SD-ready-$(shell date +%Y%m%d).zip *
	@echo "Release packages created:"
	@echo "  Standard (with apps/ folder):"
	@echo "    - release/$(TARGET)-$(shell date +%Y%m%d).tar.gz"
	@echo "    - release/$(TARGET)-$(shell date +%Y%m%d).zip"
	@echo "  SD-ready (extract directly to SD root):"
	@echo "    - release/$(TARGET)-SD-ready-$(shell date +%Y%m%d).tar.gz"
	@echo "    - release/$(TARGET)-SD-ready-$(shell date +%Y%m%d).zip"

#---------------------------------------------------------------------------------
# Clean rule
#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@rm -fr $(BUILD) $(OUTPUT).elf $(OUTPUT).dol apps/ release/

else

DEPENDS := $(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# Linking
#---------------------------------------------------------------------------------
$(OUTPUT).dol: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPENDS)

endif