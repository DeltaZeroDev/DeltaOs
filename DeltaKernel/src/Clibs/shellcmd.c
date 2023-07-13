char prompt[128] = "[DeltaZero@DeltaOs] # ";

void shellstart(){
    terminal_printf("Shell started\n");
    terminal_cprintf(prompt, 5);
}
void ver(){
    terminal_move_cursor(0,cursory + 1);
    terminal_printf("DeltaKernel V=0.2.0\n");
    terminal_cprintf(prompt, 5);
}
