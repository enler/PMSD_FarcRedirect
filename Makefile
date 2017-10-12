ifeq ($(strip $(DEVKITARM)),)
$(error "请将DEVKITARM添加到环境变量")
endif

PREFIX = $(DEVKITARM)/bin/arm-none-eabi-

CC = $(PREFIX)gcc
LD = $(PREFIX)gcc
OBJCOPY = $(PREFIX)objcopy

# define options for compilation
CFLAGS = -Wall -Os -fno-builtin -ffunction-sections -march=armv6k -mthumb -fshort-wchar -I$(DEVKITPRO)/libctru/include

# define options for linkage. Prevent the inclusion of standard start
# code and libraries.
LDFLAGS = -fno-builtin -nostartfiles -Wl,--use-blx -nodefaultlibs

# use variables to refer to init code in case it changes
OBJS = main.o head.o

#
# define build targets
#
all: payload.bin

clean:
	rm -f *.o *.elf *.bin *.dump


# build s-record with init code and c files linked together
payload.bin: $(OBJS)
	$(LD) $(LDFLAGS) -T ld.S -T symbol.S -o $(@:.bin=.elf) $(OBJS) $(LIBS)
	$(OBJCOPY) -O binary $(@:.bin=.elf) $@

# handle compilation of C files

%.o:%.S
	$(CC) -D__ASSEMBLY__ $(CFLAGS) -c $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


