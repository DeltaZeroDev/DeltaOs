#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/std.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/shellcmd.c"

extern void init_interrupts(void);
extern void test_interrupts(void);


char osname[16] = "DeltaOs";
char username[16] = "DeltaZero";

int kernel_main() {
	char key[124];
	char typed[1024];
	size_t i = 0;
	terminal_clear_screen();
	terminal_move_cursor(0,0);
	terminal_cprintf("Kernel loaded \x01!\n\nInitializing GDT and IDT\n", 7);
	init_dt();
	terminal_cprintf("Testing interrupts (You should see interrupt warnings)\n",7);
	terminal_move_cursor(0,4);
	test_interrupts();
	terminal_move_cursor(0,5);
	terminal_cprintf("Initializing interrupts and keyboard + RTC drivers\n\n",7);
	disable_interrupts();
	init_interrupts();
	init_timer(50);
	ps2_kbd_init();
	enable_printing();
	terminal_clear_screen();
	terminal_move_cursor(0,0);
	terminal_cprintf("Welcome to DeltaOs\n", 1);
	shellstart();	
	//for (i = 1; i < 256; i++) {
	//	test_string[i-1] = (char) i;
	//}
	//test_string[i] = '\0';
	//terminal_printf(test_string);
	for (;;) {
		if (ticks % 5 == 0) {
			queue_read(key);
			if (key[1] == '\0') {
				if (key[0] != '\1') {
					typed[i] = key[0];
					i++;
				}
			} else {
				if (strcmp(key, "ENTER") == 0) {
					typed[i] = '\0';
					for (int j = 0; j < 1023; j++) {
						typed[j] = '\0';
					}
					i = 0;
				}
			}
			//key[0] = '\1';
			//key[1] = '\1';
			//key[2] = '\0';
		}
	};
}