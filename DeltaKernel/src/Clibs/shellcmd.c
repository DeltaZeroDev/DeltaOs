char prompt[128] = "[DeltaZero@DeltaOs] # ";

void shellstart(){
    terminal_printf("Shell started\n");
    terminal_cprintf(prompt, 5);
}
void ver(){
    terminal_move_cursor(0,cursory + 1);
    terminal_printf("DeltaKernel V=0.2.1\n");
    terminal_cprintf(prompt, 5);
}
void echo(char * barbara){
    terminal_move_cursor(0,cursory + 1);
    terminal_printf(barbara);
    terminal_move_cursor(0,cursory + 1);
    terminal_cprintf(prompt, 5);
}
void clear(){
    terminal_move_cursor(0,0);
    terminal_clear_screen();
    terminal_cprintf(prompt, 5);
}
void add(char * num1, char * num2){
    terminal_printf(atoi(num1) + atoi(num2));
}
