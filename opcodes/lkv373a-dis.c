/* Disassemble LKV373A microblaze instructions.

   Copyright (C) 2009-2017 Free Software Foundation, Inc.

   This file is part of the GNU opcodes library.

   This library is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3, or (at your option)
   any later version.

   It is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
   License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; see the file COPYING.  If not, write to the
   Free Software Foundation, 51 Franklin Street - Fifth Floor, Boston,
   MA 02110-1301, USA.  */


#include "sysdep.h"
#define STATIC_TABLE
#define DEFINE_TABLE

#include "disassemble.h"
#include <strings.h>
#include "lkv373a-opc.h"
#include "lkv373a-dis.h"



int
print_insn_lkv373a (bfd_vma memaddr, struct disassemble_info * info)
{
  fprintf_ftype       print = info->fprintf_func;
  void *              fd = info->stream;

  /* Say how many bytes we consumed.  */
  return 4;
}
