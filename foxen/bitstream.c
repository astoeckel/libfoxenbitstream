/*
 *  libfoxenbitstream -- Tiny, inflexible bitstream reader
 *  Copyright (C) 2018  Andreas Stöckel
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <assert.h>

#include <foxen/bitstream.h>

/******************************************************************************
 * PRIVATE FUNCTIONS                                                          *
 ******************************************************************************/

#define BUFSIZE (sizeof(((fx_bitstream_t *)NULL)->buf) * 8U)

static inline void _fx_bitstream_fill_buf(fx_bitstream_t *reader) {
	while (reader->pos >= 8U && reader->src != reader->src_end) {
		reader->buf = (reader->buf << 8U) | *(reader->src++);
		reader->pos -= 8U;
	}
}

static inline uint64_t _fx_bitstream_read_msb(
    fx_bitstream_t *reader, uint8_t n_bits,
    fx_bitstream_byte_callback_t callback, void *callback_data) {
	assert((n_bits >= 1U) && (n_bits <= (BUFSIZE - 7U)));

	/* Copy the current buffer content, skip already read bits */
	uint64_t bits = reader->buf << reader->pos;

	/* Advance the position */
	const uint8_t pos_new = reader->pos + n_bits;

	/* If the callback is specified, issue entire bytes that were read */
	if (callback) {
		const uint8_t i0 = reader->pos / 8U, i1 = pos_new / 8U;
		uint64_t buf = reader->buf << (i0 * 8U);
		for (uint8_t i = i0; i < i1; i++) {
			uint8_t byte = buf >> (BUFSIZE - 8U);
			callback(byte, callback_data);
			buf = buf << 8U;
		}
	}
	reader->pos = pos_new;

	/* Read new bytes from the byte stream */
	_fx_bitstream_fill_buf(reader);

	/* Mask out the "low" bits */
	return bits >> (BUFSIZE - n_bits);
}

/******************************************************************************
 * PUBLIC C API                                                               *
 ******************************************************************************/

void fx_bitstream_set_source(fx_bitstream_t *reader, const uint8_t *src,
                             uint32_t src_len) {
	reader->src = src;
	reader->src_end = src + src_len;
	_fx_bitstream_fill_buf(reader);
}

uint64_t fx_bitstream_read_msb(fx_bitstream_t *reader, uint8_t n_bits) {
	return _fx_bitstream_read_msb(reader, n_bits, NULL, NULL);
}

uint64_t fx_bitstream_read_msb_ex(fx_bitstream_t *reader, uint8_t n_bits,
                                  fx_bitstream_byte_callback_t callback,
                                  void *callback_data) {
	return _fx_bitstream_read_msb(reader, n_bits, callback, callback_data);
}

uint64_t fx_bitstream_peek_msb(fx_bitstream_t *reader, uint8_t n_bits) {
	assert((n_bits >= 1U) && (n_bits <= (BUFSIZE - 7U)));
	return (reader->buf << reader->pos) >> (BUFSIZE - n_bits);
}

