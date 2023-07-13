#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/std.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/shellcmd.c"
extern void init_interrupts(void);
extern void test_interrupts(void);

// thi.s will be us,ed for shell whme keyboard stuff is done

char osname[16] = "DeltaOs";
char username[16] = "DeltaZero";

int kernel_main() {
	char key[124];
	char typed[1024];
	size_t i = 0;
    int g = 0;  
    int b = 2;
	//char test_string[256] = {};
	//int i;
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
	ps2_init();
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
					terminal_printf(key);
					i++;
                    if (b == 1){
                        b--;
                    }  
                // What
				}              
                if (g == 0){
                    if (strcmp(key, " ") == 0){
                        char cmd[16] = typed;
                        g++;
                        typed = "";
						if (cmd != "echo "){
							b--;
						}
                    }
                }
                if (b == 0){
                    if (strcmp(key, " ") == 0){
                        char inp1[16] = typed;
                        g++;
                        typed = "";
                    }
                }
			} else {
            
				if (strcmp(key, "ENTER") == 0) {
                    char inp2[16] = typed;
					typed[i] = '\0';

					if (strcmp (fn, "function1") == 0) function1();
					for (int j = 0; j < 1024; j++) {
						typed[j] = '\0';
					}
					i = 0;
				}
				else if (strcmp(key, "BACKSPACE") == 0) {
				if (typed[0] != '\0') {
					if (cursorx - 1 > 0) {
						terminal_move_cursor(cursorx - 1,cursory);
						video_mem[cursor * 2] = '\0';
						video_mem[cursor * 2 + 1] = '\0';
						i--;
						typed[i] = '\0';
					}
				}

			}
			//key[0] = '\1';
			//key[1] = '\1';
			//key[2] = '\0';
		}
	};
	}
}