#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/drivers/layouts.h"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/CHeaders/std.h"

char * pressed_keys[5000] = {};
uint32_t idx = 0;
int layer = 0;
int caps = 0;
int printing = 0;
int full = 0;
int reading = 0;

#define layout qwerty_en_us

// queue_push and queue_read are defined in the keyboard driver
// but are not exclusively used for the keyboard (see mouse driver)

void queue_push(char * key) {
	if (full == 0) {
		pressed_keys[idx] = key;
		idx++;
		if (idx == 5000) {
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
		for (i=0; i < 4999; i++) {
			pressed_keys[i] = pressed_keys[i+1];
		}
		idx--;
	}
	reading = 0;
}
void queue_pop() {
	idx--;
	pressed_keys[idx] = "\1";
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
				//queue_pop();
				queue_push(key);
			} else {
				queue_push(key);
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
	pic_send_eoi(IRQ1);
}
void ps2_kbd_init() {
	register_interrupt_handler(IRQ1, &ps2_callback);
	register_interrupt_handler(1, &ps2_callback);
	for (int i=0; i < 4999; i++) {
		pressed_keys[i] = "\1";
	}
}
void enable_printing() {
	printing = 1;
}
void disable_printing() {
	printing = 0;
}