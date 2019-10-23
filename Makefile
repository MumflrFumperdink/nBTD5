DEBUG = FALSE

GCC = nspire-gcc
AS  = nspire-as
GXX = nspire-g++
LD  = nspire-ld
GENZEHN = genzehn

GCCFLAGS = -Wall -Wno-narrowing -Wno-sign-compare -Wno-unused-variable
LDFLAGS = 
ZEHNFLAGS = --name "BloonsTD5"

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
endif

OBJS = $(patsubst %.c, %.o, $(shell find . -name \*.c))
OBJS += $(patsubst %.cpp, %.o, $(shell find . -name \*.cpp))
OBJS += $(patsubst %.S, %.o, $(shell find . -name \*.S))
EXE = BloonsTD5
DISTDIR = .
vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

bold := $(shell tput bold)
sgr0 := $(shell tput sgr0)

all: $(EXE).tns

%.o: %.c
	$(info $(bold)Compiling $< $(sgr0))
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.cpp
	$(info $(bold)Compiling $< $(sgr0))
	$(GXX) $(GCCFLAGS) -c $< -o $@

%.o: %.S
	$(AS) -c $< -o $@

$(EXE).elf: $(OBJS)
	$(info $(bold)Linking $(sgr0))
	mkdir -p $(DISTDIR)
	$(LD) $^ $(LDFLAGS) -o $@

$(EXE).tns: $(EXE).elf
	$(info $(bold)Making Executable $@ $(sgr0))
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $@
	rm $@.zehn

clean:
	rm -f $(OBJS) $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn
