#include <defs.h>
#include <stdio.h>
#include <sys/gdt.h>
int glob = 0;

void start(void* modulep, void* physbase, void* physfree)
{
	// kernel starts here
}

#define INITIAL_STACK_SIZE 4096
char stack[INITIAL_STACK_SIZE];
uint32_t* loader_stack;
extern char kernmem, physbase;

void boot(void)
{
	// note: function changes rsp, local stack variables can't be practically used
	volatile register char *rsp asm ("rsp");
	loader_stack = (uint32_t*)rsp;
	rsp = &stack[INITIAL_STACK_SIZE];
	__asm__(
		"movq %%rsp, %0;"
		"movq %1, %%rsp;"
		:"=g"(loader_stack)
		:"r"(&stack[INITIAL_STACK_SIZE])
	);
	reload_gdt();
	start(
		(char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase,
		&physbase,
		(void*)(uint64_t)loader_stack[4]
	);
//	while(1);
	printf("Print a string: %s","abc\n");
	printf("Print a char: %c",'x');
//	char c = NULL;
	putchar((unsigned char)'\n');
//	printf("Print a empty char: %c",c);
	printf("Print both: %s %c\n", "abc ",'y');
	printf("Print int:%x %d %x\n",13,-33,33);
	printf("Print a address: %p\n",&glob);
	while(1);
}
