# libfoxenbitstream ― Tiny, non-versatile bitstream reader

[![Build Status](https://travis-ci.org/astoeckel/libfoxenbitstream.svg?branch=master)](https://travis-ci.org/astoeckel/libfoxenbitstream)

This library provides a tiny, totally non-versatile bitstream reader that is on the verge of being reasonably fast. Allows to read a series of 1-57 bits of data as unsigned integers in MSB order (most significant bit first) from a byte stream.

## Usage

```C
fx_bitstream_t bitstream;
fx_bitstream_init(&bitstream);

/* 1111'1111 0000'0000 1111'1111 */
/* AAAB BBBB BCC ...             */
/*   3      62 0                 */
uint8_t bytes[] = {0xFF, 0x00, 0xFF};
fx_bitstream_set_source(&bitstream, bytes, sizeof(bytes));

EXPECT_EQ(7, fx_bitstream_read_msb(&bitstream, 3));  /* A */
EXPECT_EQ(62, fx_bitstream_read_msb(&bitstream, 6)); /* B */
EXPECT_EQ(0, fx_bitstream_read_msb(&bitstream, 2));  /* C */
/* etc. */
```

See `foxen/bitstream.h` for the complete documentation.

## FAQ about the *Foxen* series of C libraries

**Q: What's with the name?**

**A:** [*Foxen*](http://kingkiller.wikia.com/wiki/Foxen) is a mysterious glowing object used by Auri to guide her through the catacumbal “Underthing”. These software libraries are similar. Mysterious and catacumbal. Probably less useful than an eternal sympathy lamp though.

**Q: What is the purpose and goal of these libraries?**

**A:** The *Foxen* series of libraries is comprised of a set of extremely small C libraries that rely on the [Meson](https://mesonbuild.com/) build system for dependency management. The libraries share are an API that does not use heap memory allocations. They can thus be easily compiled to tiny, standalone [WASM](https://webassembly.org/) code.

**Q: Why?**

**A:** Excellent question! The author mainly created these libraries because he grew tired of copying his own source code files between projects all the time.

**Q: Would you recommend to use these libraries in my project?**

**A:** That depends. Some of the code is fairly specialized according to my own needs and might not be intended to be general. If what you are going to use these libraries for aligns with their original purpose, then sure, go ahead. Otherwise, I'd probably advise against using these libraries, and as explained below, I'm not super open to expanding their scope.

**Q: Can you licence these libraries under a something more permissive than AGPLv3?**

**A:** Maybe, if you ask nicely. I'm not a fan of giving my work away “for free” (i.e., inclusion of my code in commercial or otherwise proprietary software) without getting something back (e.g., public access to the source code of the things other people built with it). That being said, some of the `foxen` libraries may be too trivial to warrant the use of a strong copyleft licence. Correspondingly, I might reconsider this decision for individual libraries. See “[Why you shouldn't use the Lesser GPL for your next library](https://www.gnu.org/licenses/why-not-lgpl.en.html)” for more info.

**Q: Can I contribute?**

**A:** Sure! Feel free to open an issue or a PR. However, be warned that since I've mainly developed these libraries for use in my own stuff, I might be a little picky about what I'm going to include and what not.

## Licence

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.
