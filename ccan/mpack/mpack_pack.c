/* Licenced under LGPLv3+ - see LICENSE file for details */
#include "mpack.h"

#include <stddef.h>

static int mpack_pack_raw(mpack_writer *p, void *data, size_t len)
{
	return p->write(p, data, len);
}

static inline void enc_u8(uint8_t *buf, uint8_t i)
{
	buf[0] = i;
}

static inline void enc_u16(uint8_t *buf, uint16_t i)
{
	buf[0] = i >> 8;
	buf[1] = i & 0xff;
}

static inline void enc_u32(uint8_t *buf, uint32_t i)
{
	enc_u16(buf, i >> 16);
	enc_u16(buf + 2, i & 0xffff);
}

static inline void enc_u64(uint8_t *buf, uint64_t i)
{
	enc_u32(buf, i >> 32);
	enc_u32(buf + 4, i & 0xffffffff);
}

/* common integer packing */
static int mpack_pack_cint(mpack_writer *p, uintmax_t i, uint8_t enc_base)
{
	uint8_t buf[1 + sizeof(uint64_t)];
	size_t len;
	if (i <= UINT8_MAX) {
		buf[0] = enc_base;
		enc_u8(buf + 1, i);
		len = 2;
	} else if (i <= UINT16_MAX) {
		buf[0] = enc_base + 1;
		enc_u16(buf + 1, i);
		len = 3;
	} else if (i <= UINT32_MAX) {
		buf[0] = enc_base + 2;
		enc_u32(buf + 1, i);
		len = 1 + sizeof(uint32_t);
	} else if (i <= UINT64_MAX) {
		buf[0] = enc_base + 3;
		enc_u64(buf + 1, i);
		len = 1 + sizeof(uint64_t);
	} else
		return -1;
	return mpack_pack_raw(p, buf, len);
}

int mpack_pack_uint(mpack_writer *p, uintmax_t i)
{
	if (i <= 0x7f) {
		uint8_t v = i;
		return mpack_pack_raw(p, &v, 1);
	} else
		return mpack_pack_cint(p, i, 0xcc);
}

int mpack_pack_int(mpack_writer *p, intmax_t i)
{
	if (0 > i && i > -0x1f) {
		uint8_t v = i | 0xe0; /* | 0b11100000 */
		return mpack_pack_raw(p, &v, 1);
	} else if (i <= 0x7f) {
		uint8_t v = i;
		return mpack_pack_raw(p, &v, 1);
	} else
		return mpack_pack_cint(p, i, 0xd0);
}

int mpack_pack_bytes(mpack_writer *p, void *data, size_t len)
{
	uint8_t buf[1 + sizeof(uint32_t)];
	size_t plen;
	if (len <= UINT8_MAX) {
		buf[0] = 0xc4;
		enc_u8(buf + 1, len);
		plen = 2;
	} else if (len <= UINT16_MAX) {
		buf[0] = 0xc5;
		enc_u16(buf + 1, len);
		plen = 3;
	} else if (len <= UINT32_MAX) {
		buf[0] = 0xc6;
		enc_u32(buf + 1, len);
		plen = 5;
	} else
		return -1;

	int r = mpack_pack_raw(p, buf, plen);
	if (r)
		return r;
	return mpack_pack_raw(p, data, len);
}

int mpack_pack_array_start(mpack_writer *p, uint_least32_t count)
{
	uint8_t buf[5];
	size_t len;
	if (count <= 15) {
		buf[0] = count | 0x90; /* 0b1001xxxx */
		len = 1;
	} else if (count <= UINT16_MAX) {
		buf[0] = 0xdc;
		enc_u16(buf + 1, count);
		len = 3;
	} else if (count <= UINT32_MAX) {
		buf[0] = 0xdd;
		enc_u32(buf + 1, count);
		len = 5;
	} else
		return -1;
	return mpack_pack_raw(p, buf, len);
}
