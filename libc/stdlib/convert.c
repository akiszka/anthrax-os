#include <stdlib.h>

char *convert(uint64_t num, int base) {
	static char representation[]= "0123456789ABCDEF";
	static char buffer[65]; 
	char *ptr; 
	
	ptr = &buffer[64]; 
	*ptr = '\0'; 
	
	do { 
		*--ptr = representation[num%base]; 
		num /= base; 
	} while(num != 0); 
	
	return(ptr); 
}
