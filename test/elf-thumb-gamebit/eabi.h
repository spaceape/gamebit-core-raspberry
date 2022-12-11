#ifndef __EABI_H__
#define __EABI_H__

/*
Inlined assembly example:
	__asm (
		"\tudiv  r0, %[input_a], %[input_b]\n"
		"\tmul   r1, r0, %[input_b]\n"
		"\tsub   %[result], %[input_a], r1\n"
		: [result] "=r" (res)
		: [input_a] "r" (a), [input_b] "r" (b)
	);
*/

unsigned __aeabi_uidiv(unsigned a, unsigned b) {
	unsigned res = 0;

	while (a >= b) {
		a -= b;
		res += 1;
	}

	return res;
}

unsigned __aeabi_uidivmod(unsigned a, unsigned b) {
	if (b == 0) {
		return 0;
	}
	// Check if power of 2
	unsigned k = b - 1;
	if ((b & k) == 0) {
		return a & k;
	}
	
	while (a >= b) {
		a -= b;
	}
	return a;
}

#endif // __EABI_H__
