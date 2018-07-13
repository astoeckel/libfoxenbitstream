#  libfoxenbitstream -- Tiny, inflexible bitstream reader
#  Copyright (C) 2018  Andreas Stöckel
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as
#  published by the Free Software Foundation, either version 3 of the
#  License, or (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.

project('libfoxenbitstream', 'c', default_options : ['c_std=c99'])

# Define the contents of the actual library
inc_foxen = include_directories('.')
lib_foxenbitstream = library(
    'foxenbitstream',
    'foxen/bitstream.c',
    include_directories: inc_foxen,
    install: true)

# Install the header file
install_headers('foxen/bitstream.h', subdir: 'foxen')

# Build and register the unit test
dep_foxenunit = dependency(
    'libfoxenunit',
    fallback:['libfoxenunit', 'dep_foxenunit'])
exe_test_bitstream = executable(
    'test_bitstream',
    'test/test_bitstream.c',
    include_directories: inc_foxen,
    link_with: lib_foxenbitstream,
    dependencies: dep_foxenunit)
test('test_bitstream', exe_test_bitstream)

# Generate a Pkg config file
pkg = import('pkgconfig')
pkg.generate(
    libraries: [lib_foxenbitstream],
    subdirs: [],
    name: 'libfoxenbitstream',
    version: '1.0',
    filebase: 'libfoxenbitstream',
    description: 'A simple bitstream reader')

# Export the dependency
dep_foxenbitstream = declare_dependency(
    include_directories: inc_foxen,
    link_with: lib_foxenbitstream)

