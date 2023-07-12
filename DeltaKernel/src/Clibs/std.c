#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/CHeaders/std.h"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/atoi.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/Clibs/math.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/drivers/keyboards.c"
#include "/mnt/c/TheBoilerPlateFactory/DeltaKernel/src/CHeaders/print.h"

#define VGA_HEIGHT 40
#define VGA_WIDTH 80

char * video_mem = (char*) 0xb8000;
unsigned long int cursor = 0;
extern void gdt_flush (uint32_t);
extern void idt_flush (uint32_t);
isr_t interrupt_handlers[256];
uint64_t ticks = 0;
idt_entry_t idt_entries[256];
idt_ptr_t idt_ptr;
gdt_entry_t gdt_entries[5];
gdt_ptr_t gdt_ptr;

cursorx = 0;
cursory = 0;

static inline void outb(uint16_t port, uint8_t val) {
	asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port) :"memory");
}
static inline uint8_t inb(uint16_t port) {
	uint8_t ret;
	asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port) : "memory");
	return ret;
}
int strlen(char * string) {
	int i = 0;
	while (*string++) {
		i++;
	}
	return i;
}
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}
int strcmp(char * x, char * y) {
	int i = 0;
	int fail = 0;
	while (x[i] != '\0' && y[i] != 'y') {
		if (x[i] != y[i]) {
			fail = 1;
			break;
		}
		i++;
	}
	if ((x[i] == '\0' && y[i] != '\0') || (x[i] != '\0' && y[i] == '\0')) {
		fail = 1;
	}
	return fail;
}
void * memset(void *dest, int val, size_t len) {
	unsigned char *temp = dest;
	for (; len != 0; len--) *temp++ = val;
	return temp;
}
void terminal_move_cursor(int x, int y) {
	uint16_t pos = y * VGA_WIDTH + x;
	cursor = pos;
	cursorx = x;
	cursory = y;
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
uint16_t terminal_get_cursor(void) {
	uint16_t pos = 0;
	outb(0x3D4, 0x0F);
	pos |= inb(0x3D5);
	outb(0x3D4, 0x0E);
	pos |= ((uint16_t)inb(0x3D5)) << 8;
	return pos;
}
void terminal_clear_screen(void) {
	int limit = (VGA_WIDTH * 2) * (VGA_HEIGHT);
	for (int i = 0; i < limit; ++i) {
		video_mem[i] = 0;
	}
}
uint8_t getch() {
	return inb(0x60);
}

int terminal_printf(char * text) {
	unsigned long int pos = cursor * 2;
	unsigned long int i = 0;
	while (text[i] != '\0') {
		if (text[i] != '\n') {
			video_mem[pos] = text[i];
			video_mem[pos+1] = 15;
			pos += 2;
		} else {
			terminal_move_cursor(0,cursory + 1);
			pos = cursor * 2;
		}
		if (pos / 2 > (VGA_WIDTH + VGA_WIDTH - 1) * VGA_HEIGHT) {
			scroll();
		}
		i++;
	}
	cursor = pos / 2;
	terminal_move_cursor(cursor,0);
}
int terminal_cprintf(char * text, int color) {
	unsigned long int pos = cursor * 2;
	unsigned long int i = 0;
	while (text[i] != '\0') {
		if (text[i] != '\n') {
			video_mem[pos] = text[i];
			video_mem[pos+1] = color;
			pos += 2;
		} else {
			terminal_move_cursor(0,cursory + 1);
			pos = cursor * 2;
		}
		if (pos / 2 > (VGA_WIDTH + VGA_WIDTH - 1) * VGA_HEIGHT) {
			scroll();
		}
		i++;
	}
	cursor = pos / 2;
	terminal_move_cursor(cursor,0);
}
void sleep(unsigned long second) {
	unsigned int seconds = ticks + (second * 100);
	while (ticks != seconds) {}
}
void register_interrupt_handler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}
void isr_handler(registers_t regs) {
	char string[124];
	terminal_cprintf("Unhandled interrupt ",4);
	itoa(regs.int_no,string,10);
	terminal_cprintf(string,4);
	terminal_cprintf("! ",4);
}
void irq_handler(registers_t regs) {
	if (regs.int_no >= 40) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
	if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}
}
void timer_callback(registers_t regs) {
	ticks++;
}
static void idt_set_gate (uint16_t num, uint32_t base, uint16_t sel, uint8_t flags) {
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
	idt_entries[num].sel = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].flags = flags | 0x60;
}
static void init_idt () {
	idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
	idt_ptr.base  = (uint32_t)&idt_entries;
	memset(&idt_entries, 0, sizeof(idt_entry_t)*256);
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);
	idt_set_gate(0, (uint32_t)isr0, 0x08, 0x8E);
	idt_set_gate(1, (uint32_t)isr1, 0x08, 0x8E);
	idt_set_gate(2, (uint32_t)isr2, 0x08, 0x8E);
	idt_set_gate(3, (uint32_t)isr3, 0x08, 0x8E);
	idt_set_gate(4, (uint32_t)isr4, 0x08, 0x8E);
	idt_set_gate(5, (uint32_t)isr5, 0x08, 0x8E);
	idt_set_gate(6, (uint32_t)isr6, 0x08, 0x8E);
	idt_set_gate(7, (uint32_t)isr7, 0x08, 0x8E);
	idt_set_gate(8, (uint32_t)isr8, 0x08, 0x8E);
	idt_set_gate(9, (uint32_t)isr9, 0x08, 0x8E);
	idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
	idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
	idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
	idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
	idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
	idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
	idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
	idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
	idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
	idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
	idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
	idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
	idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
	idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
	idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
	idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
	idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
	idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
	idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
	idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
	idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
	idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);
	idt_set_gate(32, (uint32_t)irq0, 0x08, 0x8E);
	idt_set_gate(33, (uint32_t)irq1, 0x08, 0x8E);
	idt_set_gate(34, (uint32_t)irq2, 0x08, 0x8E);
	idt_set_gate(35, (uint32_t)irq3, 0x08, 0x8E);
	idt_set_gate(36, (uint32_t)irq4, 0x08, 0x8E);
	idt_set_gate(37, (uint32_t)irq5, 0x08, 0x8E);
	idt_set_gate(38, (uint32_t)irq6, 0x08, 0x8E);
	idt_set_gate(39, (uint32_t)irq7, 0x08, 0x8E);
	idt_set_gate(40, (uint32_t)irq8, 0x08, 0x8E);
	idt_set_gate(41, (uint32_t)irq9, 0x08, 0x8E);
	idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
	idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
	idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
	idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
	idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
	idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);
	idt_set_gate(128, (uint32_t)isr128, 0x08, 0x8E);
	idt_flush((uint32_t)&idt_ptr);
}
void init_timer(uint32_t freq) {
	register_interrupt_handler(IRQ0, &timer_callback);
	uint32_t divisor = 1193180 / freq;
	outb(0x43, 0x36);
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
	outb(0x40, low);
	outb(0x40, high);
}
static void gdt_set_gate (int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;
	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;
	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}
static void init_gdt () {
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
	gdt_ptr.base  = (uint32_t)&gdt_entries;
	gdt_set_gate(0, 0, 0, 0, 0);
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	gdt_flush((uint32_t)&gdt_ptr);
}
void init_dt() {
	init_gdt();
	init_idt();
	memset(&interrupt_handlers, 0, sizeof(isr_t)*256);
}
void hang_forever() {
	terminal_cprintf("Hanging!", 4);
	while (1 == 1) {}
}
void disable_interrupts() {
	asm volatile ("cli\n");
}
void enable_interrupts() {
	asm volatile ("sti\n");
}
void scroll() {
	int i;
	for (i = 0; i < (VGA_WIDTH + VGA_WIDTH) * VGA_HEIGHT; i++) {
		video_mem[i] = video_mem[i + VGA_WIDTH + VGA_WIDTH];
	}
	for (i = (VGA_WIDTH + VGA_WIDTH - 1) * VGA_HEIGHT; i < (VGA_WIDTH + VGA_WIDTH) * VGA_HEIGHT; i++) {
		video_mem[i] = 0;
	}
	terminal_move_cursor(cursorx,cursory-1);
}
int abs(int i) {
	return i < 0 ? -i : i;
}
void * memcpy(void * dest, const void * src, size_t len) {
	char * d = dest;
	const char * s = src;
	while (len--) {
		*d++ = *s++;
	}
	return dest;
}
char * strcpy(char * dest, const char * src) {
	return memcpy(dest, src, strlen (src) + 1);
}
void pic_send_eoi(uint8_t irq) {
	if(irq >= IRQ8) {
		outb(0xA0, 0x20);
	}
	outb(0x20, 0x20);
}
