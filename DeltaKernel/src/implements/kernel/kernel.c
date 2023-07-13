#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/std.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/shellcmd.c"

extern void init_interrupts(void);
extern void test_interrupts(void);



char osname[16] = "DeltaOs";
char username[16] = "DeltaZero";

int kernel_main() {
	char key[124];
	char typed[1024] = "";
	size_t i = 0;
    int g = 0;  
    int b = 2;
	char cmd[1024] = "";
	char inp1[1024] = "";
	char inp2[1024] = "";
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
	terminal_clear_screen();
	terminal_move_cursor(0,0);
	terminal_cprintf("Welcome to DeltaOs\n", 1);
	shellstart();
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
						strcpy(cmd, typed);
						cmd[i] = '\0';
                        g++;
                        for (int j = 0; j < 1024; j++) {
 							typed[j] = '\0';
						}
						i = 0;
						if (cmd != "echo "){
							b--;
						}
                    }
                }
                if (b == 0){
                    if (strcmp(key, " ") == 0){
						strcpy(inp1, typed);
						inp1[i] = '\0';
                        g++;
                        for (int j = 0; j < 1024; j++) {
 							typed[j] = '\0';
						}
						i = 0;
                    }
                }
			} else {
            
				if (strcmp(key, "ENTER") == 0) {
					strcpy(inp2, typed);
					inp2[i] = '\0';
					typed[i] = '\0';
					if (strcmp (cmd, "ver ") == 0) ver();
					// if (strcmp (cmd, "help ") == 0) ver();
					// if (strcmp (cmd, "clear ") == 0) terminal_clear_screen();
					// iterate through every char in cmd and inp1 and set the location of the space character to null before adding more commands
					else{
						terminal_move_cursor(0,cursory + 1);
						terminal_cprintf("Command not found!\n", 4);
						terminal_cprintf("[DeltaZero@DeltaOs] # ", 5);
						terminal_printf(cmd);
					}
					for (int j = 0; j < 1024; j++) {
						typed[j] = '\0';
						cmd[j] = '\0';
						inp1[j] = '\0';
						inp2[j] = '\0';
					}
					g = 0;
					b = 0;
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