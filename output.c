#include "output.h"

bool
writebytes (unsigned long long x, int nbytes)
{
  do
  {
    if (putchar (x) < 0)
      return false;
    x >>= CHAR_BIT;
    nbytes--;
  }
  while (0 < nbytes);

  return true;
}



bool
write_N_bytes (int nbytes, int N, unsigned long long (*rand64) (void))
{
  int *buf = malloc(nbytes);

  if (buf != NULL) {
    /* Put nbytes random bytes into buf */
    unsigned long long x;
    for (int i = 0; i < nbytes; i++) {
      if (i % 8 == 0) {
        x = rand64(); // 8 bytes
      }
      buf[i] = (x >> (8 * i)) & 0xff;
    }
    int bytes_written = 0;

    do
    {
      /* Write N bytes from buf to stdout */
      int num_bytes_to_write = nbytes < N + bytes_written ? nbytes - bytes_written : N;
      int curr_bytes = write(1, buf + bytes_written, num_bytes_to_write);

      if (curr_bytes < 0) {
        perror("write");
        free(buf);
        return false;
      }
      bytes_written += curr_bytes;
    } while (bytes_written < nbytes);
  }

  free(buf);
  return true;
}

