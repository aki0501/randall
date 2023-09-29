# Make x86-64 random byte generators.

# Copyright 2015, 2020, 2021 Paul Eggert

# This program is free software: you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation, either version 3 of the
# License, or (at your option) any later version.

# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

# Optimization level.  Change this -O2 to -Og or -O0 or whatever.
OPTIMIZE =

# The C compiler and its options.
CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -fanalyzer \
  -march=native -mtune=native -mrdrnd

# The archiver command, its options and filename extension.
TAR = tar
TARFLAGS = --gzip --transform 's,^,randall/,'
TAREXT = tgz

# Define object containing source files
OBJ = randall.c options.c output.c rand64-hw.c rand64-sw.c mrand48_r.c

default: randall

randall: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $@

check: randall
	./randall 10 | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 0 | wc -c | [[ "$$(cat)" == "0" ]]
	./randall 1 | wc -c | [[ "$$(cat)" == "1" ]]
	./randall 10 -i rdrand | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 10 -i mrand48_r | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 10 -i /dev/random | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 10 -o 10 | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 10 -o 2 | wc -c | [[ "$$(cat)" == "10" ]]
	./randall 0 -o 2 | wc -c | [[ "$$(cat)" == "0" ]]
	./randall 20 -o 3 -i rdrand | wc -c | [[ "$$(cat)" == "20" ]]
	if ! ./randall 10 -i rdrand >/dev/null; then exit 1; fi
	if ! ./randall 10 -i mrand48_r >/dev/null; then exit 1; fi
	if ./randall 10 -i drand >/dev/null; then exit 1; fi
	if ./randall 10 -i >/dev/null; then exit 1; fi
	if ./randall 10 -o >/dev/null; then exit 1; fi
	if ! ./randall 10 -o stdio >/dev/null; then exit 1; fi
	if ./randall 10 -o -10 >/dev/null; then exit 1; fi
	if ./randall 10 -o 0 >/dev/null; then exit 1; fi
	if ! ./randall 10 -o 10 >/dev/null; then exit 1; fi
	if ! ./randall 10 -o 10 -i rdrand >/dev/null; then exit 1; fi
	if ./randall 10 -o 10 -i >/dev/null; then exit 1; fi
	if ./randall -o 10 -i 10 >/dev/null; then exit 1; fi

assignment: randall-assignment.$(TAREXT)
assignment-files = COPYING Makefile randall.c
randall-assignment.$(TAREXT): $(assignment-files)
	$(TAR) $(TARFLAGS) -cf $@ $(assignment-files)

submission-tarball: randall-submission.$(TAREXT)
submission-files = $(assignment-files) \
  notes.txt mrand48_r.c  mrand48_r.h  \
  options.c  options.h  output.c  output.h  \
  rand64-hw.c  rand64-hw.h  rand64-sw.c  rand64-sw.h

randall-submission.$(TAREXT): $(submission-files)
	$(TAR) $(TARFLAGS) -cf $@ $(submission-files)

repository-tarball:
	$(TAR) -czf randall-git.tgz .git

.PHONY: default clean assignment submission-tarball repository-tarball check

clean:
	rm -f *.o *.$(TAREXT) randall
