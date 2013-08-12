#ifndef CCAN_BHEAP_POISON_H_
#define CCAN_BHEAP_POISON_H_

#include <stdint.h>

#define BH_POISON_BASE   0xdead0000
#define BH_POISON_NODE   (BH_POISON_BASE | 0x1000)
#define BH_POISON_PARENT (BH_POISON_BASE | 0xf000)

#define BHN_P(x) { .d = { (void *)(uintptr_t)(BH_POISON_NODE + x * 2), (void *)(uintptr_t)(BH_POISON_NODE + x * 2 + 1) } }

#endif
