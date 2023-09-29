/* Generate N bytes of random output.  */

/* When generating output this program uses the x86-64 RDRAND
   instruction if available to generate random numbers, falling back
   on /dev/random and stdio otherwise.

   This program is not portable.  Compile it with gcc -mrdrnd for a
   x86-64 machine.

   Copyright 2015, 2017, 2020 Paul Eggert

   This program is free software: you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation, either version 3 of the
   License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "output.h"
#include "rand64-hw.h"

/* Main program, which outputs N bytes of random data. */
int
main (int argc, char **argv)
{
  /* Collect data about passed in arguments.  */
  struct options opts = {
    .nbytes = 0,
    .N = 0,
    .iflag = "rdrand",
  };
  parse_options( argc, argv, &opts );

  /* If there's no work to do, don't worry about which library to use.  */
  if (opts.nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
  appropriate library.  */
  unsigned long long (*rand64) (void);
  void (*finalize) (void);

  /* Check input for -i flag (default=rdrand for no -i flag).  */
  if (strcmp(opts.iflag, "mrand48_r") == 0) {
    mrand48_rng_init();
    rand64 = mrand48_rng;
    finalize = mrand48_rng_fini;
  }
  else if (strcmp(opts.iflag, "rdrand") == 0) {
    if (rdrand_supported ()) {
      /* Use rdrand */
      hardware_rand64_init();
      rand64 = hardware_rand64;
      finalize = hardware_rand64_fini;
    }
    else {
      /* rdrand is not supported */
      exit(1);
    }
  }
  else {
    /* User passed in a file as an argument */
    software_rand64_init(opts.iflag);
    rand64 = software_rand64;
    finalize = software_rand64_fini;
  }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  /* Check input for -o flag (default=stdio for no -o flag). */
  if (opts.N == 0) {
    /* -o N wasn't specified */
    do
    {
      unsigned long long x = rand64 ();
      int outbytes = opts.nbytes < wordsize ? opts.nbytes : wordsize;
      if (!writebytes (x, outbytes))
      {
        output_errno = errno;
        break;
      }
      opts.nbytes -= outbytes;
    }
    while (0 < opts.nbytes);
  }
  else {
    /* -o N was specified */
    if (!write_N_bytes (opts.nbytes, opts.N, rand64 ))
    {
      output_errno = errno;
    }
  }

  if (fclose (stdout) != 0)
      output_errno = errno;

  if (output_errno)
  {
    errno = output_errno;
    perror ("output");
  }

  finalize ();
  return !!output_errno;
}
