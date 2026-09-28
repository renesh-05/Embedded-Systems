MCU = atmega328p
F_CPU = 16000000UL
TARGET = firmware

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

CFLAGS = -Wall -Os -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I./inc

PORT=/dev/ttyUSB0
PROGRAMMER=arduino
BAUDE=115200

SRC=src/main.c src/soft_uart.c
OBJ=$(SRC:.c=.o)

all: $(TARGET).hex

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET).elf:$(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

$(TARGET).hex:$(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

flash: $(TARGET).hex
	$(AVRDUDE) -v -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUDE) -U flash:w:$<:i

clean:
	rm -f src/*.o $(TARGET).elf $(TARGET).hex