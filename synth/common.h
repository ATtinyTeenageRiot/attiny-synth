#pragma once
#include <stdint.h>

#define likely(x)      __builtin_expect(!!(x), 1)
#define unlikely(x)    __builtin_expect(!!(x), 0)

static const uint8_t mixbuff_len = 8;
extern volatile int16_t mixbuff[2][mixbuff_len];

// range of scale: 0-8 (maps to 0.-1.)
static inline int8_t scale(int8_t val, int8_t scale)
{
	switch (scale) {
		case 0: return 0;
		case 1: return val>>3;
		case 2: return val>>2;
		case 3: return (val>>1) - (val>>3);
		case 4: return val>>1;
		case 5: return (val>>1) + (val>>3);
		case 6: return val - (val>>2);
		case 7: return val - (val>>3);
		case 8: return val;
	}
	return 0;
}

// call repeatedly with same value
// compiler can then put the if statements on the outside
static inline int8_t bulk_scale(int8_t val, uint8_t scaler)
{
#if 0
	return (int16_t(val)*scaler)>>8;
#else
	int16_t res = 0;
	if ((scaler&0x0F) && (val&0xF0)) {
		if (scaler & 0x01) res += val;
		if (scaler & 0x02) res += val << 1;
		if (scaler & 0x04) res += val << 2;
		if (scaler & 0x08) res += val << 3;
	}
	if (scaler&0xF0) {
		if (scaler & 0x10) res += val << 4;
		if (scaler & 0x20) res += val << 5;
		if (scaler & 0x40) res += val << 6;
		if (scaler & 0x80) res += val << 7;
	}
	return res>>8;
#endif
}