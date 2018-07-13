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

#include <foxen/bitstream.h>
#include <foxen/unittest.h>

/******************************************************************************
 * Unit tests                                                                 *
 ******************************************************************************/

#define INIT_BITSTREAM(...)                                \
	const uint8_t buf[] = {__VA_ARGS__};                   \
	fx_bitstream_t reader;                                 \
	fx_bitstream_init(&reader);                            \
	EXPECT_TRUE(fx_bitstream_at_source_end(&reader));      \
	fx_bitstream_set_source(&reader, buf, sizeof(buf));    \
	if (sizeof(buf) <= 4) {                                \
		EXPECT_TRUE(fx_bitstream_at_source_end(&reader));  \
	} else {                                               \
		EXPECT_FALSE(fx_bitstream_at_source_end(&reader)); \
	}

#define TEST_READ(expect, n_bits)                              \
	EXPECT_TRUE(fx_bitstream_can_read(&reader, n_bits));       \
	EXPECT_EQ(expect, fx_bitstream_peek_msb(&reader, n_bits)); \
	EXPECT_EQ(expect, fx_bitstream_read_msb(&reader, n_bits));

void test_read_bits_msb_bytes() {
	INIT_BITSTREAM(0x01U, 0x23U, 0x45U);

	TEST_READ(buf[0], 8);
	TEST_READ(buf[1], 8);
	TEST_READ(buf[2], 8);
}

void test_read_bits_msb_16_bytes() {
	INIT_BITSTREAM(0x01U, 0x23U, 0x45U, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00,
	               0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77);
	for (int i = 0; i < 16; i++) {
		TEST_READ(buf[i], 8);
	}
	EXPECT_TRUE(fx_bitstream_at_source_end(&reader));
}

void test_read_bits_msb_nibbles() {
	INIT_BITSTREAM(0x01U, 0x23U, 0x45U);
	TEST_READ((buf[0] & 0xF0) >> 4, 4);
	TEST_READ((buf[0] & 0x0F) >> 0, 4);
	TEST_READ((buf[1] & 0xF0) >> 4, 4);
	TEST_READ((buf[1] & 0x0F) >> 0, 4);
	TEST_READ((buf[2] & 0xF0) >> 4, 4);
	TEST_READ((buf[2] & 0x0F) >> 0, 4);
}

void test_read_bits_msb_32_nibbles() {
	INIT_BITSTREAM(0x01U, 0x23U, 0x45U, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00,
	               0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77);
	for (int i = 0; i < 16; i++) {
		TEST_READ((buf[i] & 0xF0) >> 4, 4);
		TEST_READ((buf[i] & 0x0F) >> 0, 4);
	}
	EXPECT_TRUE(fx_bitstream_at_source_end(&reader));
}

void test_read_bits_flac_sync1() {
	INIT_BITSTREAM(0xFFU, 0xF8U);
	TEST_READ(0x3FFEU, 14);
	TEST_READ(0U, 1);
	TEST_READ(0U, 1);
}

void test_read_bits_flac_sync2() {
	INIT_BITSTREAM(0xFFU, 0xF9U);
	TEST_READ(0x3FFEU, 14);
	TEST_READ(0U, 1);
	TEST_READ(1U, 1);
}

void test_read_bits_three() {
	INIT_BITSTREAM(0x01U, 0x23U, 0x45U, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x00,
	               0x11, 0x22, 0x33);
	TEST_READ(0x0U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x2U, 3);
	TEST_READ(0x2U, 3);
	TEST_READ(0x1U, 3);
	TEST_READ(0x5U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x5U, 3);
	TEST_READ(0x3U, 3);
	TEST_READ(0x1U, 3);
	TEST_READ(0x7U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x4U, 3);
	TEST_READ(0x6U, 3);
	TEST_READ(0x5U, 3);
	TEST_READ(0x3U, 3);
	TEST_READ(0x6U, 3);
	TEST_READ(0x3U, 3);
	TEST_READ(0x3U, 3);
	TEST_READ(0x6U, 3);
	TEST_READ(0x7U, 3);
	TEST_READ(0x4U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x4U, 3);
	TEST_READ(0x2U, 3);
	TEST_READ(0x2U, 3);
	TEST_READ(0x1U, 3);
	TEST_READ(0x0U, 3);
	TEST_READ(0x6U, 3);
	EXPECT_FALSE(fx_bitstream_can_read(&reader, 4));
	EXPECT_TRUE(fx_bitstream_can_read(&reader, 3));
	TEST_READ(0x3U, 3);
	EXPECT_FALSE(fx_bitstream_can_read(&reader, 1));
	EXPECT_TRUE(fx_bitstream_at_source_end(&reader));
}

void test_example_code() {
	fx_bitstream_t bitstream;
	fx_bitstream_init(&bitstream);

	/* 1111'1111 0000'0000 1111'1111 */
	/* AAAB BBBB BCC ...             */
	uint8_t bytes[] = {0xFF, 0x00, 0xFF};
	fx_bitstream_set_source(&bitstream, bytes, sizeof(bytes));

	EXPECT_EQ(7, fx_bitstream_read_msb(&bitstream, 3));  /* A */
	EXPECT_EQ(62, fx_bitstream_read_msb(&bitstream, 6)); /* B */
	EXPECT_EQ(0, fx_bitstream_read_msb(&bitstream, 2));  /* C */
}

/******************************************************************************
 * Main program                                                               *
 ******************************************************************************/

int main() {
	RUN(test_read_bits_msb_bytes);
	RUN(test_read_bits_msb_16_bytes);
	RUN(test_read_bits_msb_nibbles);
	RUN(test_read_bits_msb_32_nibbles);
	RUN(test_read_bits_three);
	RUN(test_read_bits_flac_sync1);
	RUN(test_read_bits_flac_sync2);
	RUN(test_example_code);
	DONE;
}
