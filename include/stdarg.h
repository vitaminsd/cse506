#define va_list __builtin_va_list
#define va_start(ap,list) __builtin_va_start(ap,list)
#define va_arg(ap,type) __builtin_va_arg((ap),type)
#define va_end(ap) __builtin_va_end(ap)
