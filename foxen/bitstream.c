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
	while (reader->pos >= 8 && reader->src != reader->src_end) {
		reader->buf = (reader->buf << 8U) | *(reader->src++);
		reader->pos -= 8;
	}
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
	assert((n_bits >= 1) && (n_bits <= (BUFSIZE - 7)));

	/* Copy the current buffer content, skip already read bits */
	uint64_t bits = reader->buf << reader->pos;

	/* Advance the position, read new bytes from the source byte stream */
	reader->pos += n_bits;
	_fx_bitstream_fill_buf(reader);

	/* Mask out the "low" bits */
	return bits >> (BUFSIZE - n_bits);
}

uint64_t fx_bitstream_peek_msb(fx_bitstream_t *reader, uint8_t n_bits) {
	assert((n_bits >= 1) && (n_bits <= (BUFSIZE - 7)));
	return (reader->buf << reader->pos) >> (BUFSIZE - n_bits);
}

