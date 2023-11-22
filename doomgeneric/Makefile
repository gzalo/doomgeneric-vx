CC = C:/Users/Gzalo/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc.exe
OBJCOPY = C:/Users/Gzalo/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-objcopy.exe
CFLAGS = -nostdinc -ffreestanding -mbig-endian -mthumb -mno-unaligned-access -mcpu=ARM1136J-S -nodefaultlibs -nostartfiles -nostdlib -fshort-wchar -I include/
LDFLAGS = -mbig-endian -nodefaultlibs -nostartfiles -nostdlib -T link.ld -Wl,-Map=a.map
SRC_DIR = .
BUILD_DIR = build

EXCLUDED_SRCS = doomgeneric_emscripten.c doomgeneric_sdl.c doomgeneric_soso.c doomgeneric_sosox.c doomgeneric_win.c doomgeneric_xlib.c i_sdlmusic.c i_sdlsound.c
SRCS = $(filter-out $(addprefix $(SRC_DIR)/, $(EXCLUDED_SRCS)), $(wildcard $(SRC_DIR)/*.c))
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

all: $(BUILD_DIR)/app.out

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app.elf: $(OBJS)
	$(CC) $(LDFLAGS) lib/vrxcrt_t.o $^ lib/verix11.lib lib/fz_5s.b lib/c_5.b -o $@

$(BUILD_DIR)/app.out: $(BUILD_DIR)/app.elf
	$(OBJCOPY) -O binary $< $@

	