#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/drivers/layouts.h"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/CHeaders/std.h"

char * pressed_keys[1000] = {};
uint32_t index = 0;
int layer = 0;
int caps = 0;
int printing = 0;
int full = 0;
int reading = 0;

#define layout qwerty_en_us

void queue_push(char * key) {
	if (full == 0) {
		//pressed_keys[index] = key;
		pressed_keys[index] = key;
		index++;
		if (index == 1000) {
			full = 1;
		}
	}
}
void queue_read(char * x) {
	size_t i;
	reading = 1;
	strcpy(x, pressed_keys[0]);
	x[strlen(pressed_keys[0])] = '\0';
	if (pressed_keys[0][0] != '\1') {
		for (i=0; i < 999; i++) {
			pressed_keys[i] = pressed_keys[i+1];
		}
		index--;
	}
	reading = 0;
}
void queue_pop() {
	index--;
	pressed_keys[index] = "\1";
}
void ps2_callback(registers_t regs) {
	uint32_t keycode = inb(0x60);
	if (abs(keycode) == keycode) {
		if (layout[keycode][layer][1] == '\0') {
			if (printing == 1) {
				terminal_printf(layout[keycode][layer]);
			}
			queue_push(layout[keycode][layer]);
		} else {
			char * key = layout[keycode][layer];
			if (strcmp(key, "RSHIFT") == 0) {
				layer = 1;
			} else if (strcmp(key, "LSHIFT") == 0) {
				layer = 1;
			} else if (strcmp(key, "CAPS") == 0) {
				caps = 1 - caps;
				layer = caps;
			} else if (strcmp(key, "BACKSPACE") == 0) {
				if (printing == 1) {
					terminal_move_cursor(cursorx - 1, cursory);
					terminal_cprintf(" ",0);
					terminal_move_cursor(cursorx - 1, cursory);
				}
				queue_pop();
			} else {
				queue_push(layout[keycode][layer]);
			}
		}
	} else {
		keycode = abs(keycode) - 2;
		switch (keycode) {
			case 84 :
				layer = 0;
			case 72 :
				layer = 0;
		}
	}
}
void ps2_init() {
	register_interrupt_handler(IRQ1, &ps2_callback);
	for (int i=0; i < 999; i++) {
		pressed_keys[i] = "\1";
	}
}
void enable_printing() {
	printing = 1;
}
void disable_printing() {
	printing = 0;
}