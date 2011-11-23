/* ====================================================================
 * Copyright (c) 1989-2000 Carnegie Mellon University.  All rights 
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer. 
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * This work was supported in part by funding from the Defense Advanced 
 * Research Projects Agency and the National Science Foundation of the 
 * United States of America, and the CMU Sphinx Speech Consortium.
 *
 * THIS SOFTWARE IS PROVIDED BY CARNEGIE MELLON UNIVERSITY ``AS IS'' AND 
 * ANY EXPRESSED OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY
 * NOR ITS EMPLOYEES BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ====================================================================
 *
 */
/*
	30 May 1989 David R. Fulmer (drf) updated to do byte order
		conversions when necessary.
 */

#include <sphinxbase/byteorder.h>
#include <sphinxbase/prim_type.h>

#include <sys_compat/file.h>
#include <stdio.h>

int
awritefloat (char *file,
	     float *data,
	     int length)
{
  int             fd;
  int             size;
  int             offset;

  if ((fd = open (file, O_CREAT | O_WRONLY | O_TRUNC | O_BINARY, 0644)) < 0)
  {
    fprintf (stderr, "awritefloat: %s: can't create\n", file);
    perror("");
    return -1;
  }

  SWAP_INT32(&length);
  if (write (fd, (char *) &length, 4) != 4)
  {
    fprintf (stderr, "awritefloat: %s: can't write length\n", file);
    perror("");
    close (fd);
    return -1;
  }
  SWAP_INT32(&length);

  for(offset = 0; offset < length; offset++)
    SWAP_FLOAT32(data + offset);
  size = length * sizeof (float);
  if (write (fd, (char *) data, size) != size)
  {
    fprintf (stderr, "awritefloat: %s: can't write data\n", file);
    perror("");
    close (fd);
    return (-1);
  }
  for(offset = 0; offset < length; offset++)
    SWAP_FLOAT32(data + offset);

  printf ("Wrote %d floats in %s.\n", length, file);
  close (fd);
  return length;
}
