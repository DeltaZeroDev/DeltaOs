char prompt[128] = "[DeltaZero@DeltaOs] # ";

void shellstart(){
    terminal_printf("Shell started\n\n");
    terminal_cprintf(prompt, 5);
}
void ver(){
    terminal_printf("DeltaKernel V=0.1\n");
}
