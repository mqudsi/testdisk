/*

    File: file_mat.c

    Copyright (C) 2009 Christophe GRENIER <grenier@cgsecurity.org>
  
    This software is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.
  
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
  
    You should have received a copy of the GNU General Public License along
    with this program; if not, write the Free Software Foundation, Inc., 51
    Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#include <stdio.h>
#include "types.h"
#include "filegen.h"

static void register_header_check_mat(file_stat_t *file_stat);
static int header_check_mat(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new);

const file_hint_t file_hint_mat= {
  .extension="mat",
  .description="Matlab",
  .min_header_distance=0,
  .max_filesize=10*1024*1024,
  .recover=1,
  .enable_by_default=1,
  .register_header_check=&register_header_check_mat
};

static const unsigned char mat_header[7]= {'M', 'A', 'T', 'L', 'A', 'B', ' '};

static void register_header_check_mat(file_stat_t *file_stat)
{
  register_header_check(0, mat_header,sizeof(mat_header), &header_check_mat, file_stat);
}

static int header_check_mat(const unsigned char *buffer, const unsigned int buffer_size, const unsigned int safe_header_only, const file_recovery_t *file_recovery, file_recovery_t *file_recovery_new)
{
  const unsigned char mat_le[4]={0x00, 0x01, 'I', 'M'};
  const unsigned char mat_be[4]={0x01, 0x00, 'M', 'I'};
  if(memcmp(buffer,mat_header,sizeof(mat_header))==0 &&
    (memcmp(&buffer[0x7c], mat_le, sizeof(mat_le))==0 ||
     memcmp(&buffer[0x7c], mat_be, sizeof(mat_be))==0))
  {
    reset_file_recovery(file_recovery_new);
    file_recovery_new->extension=file_hint_mat.extension;
    return 1;
  }
  return 0;
}
