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
#include <stdint.h>
#include <endian.h>
#include "lkv373a-opc.h"
#include "lkv373a-dis.h"



int
print_insn_lkv373a (bfd_vma memaddr, struct disassemble_info * info)
{
  fprintf_ftype       print = info->fprintf_func;
  void *              fd = info->stream;
  uint8_t             instr_buf[4];
  uint32_t            instr;
  instruction_t       op;
  cpu_status_t *      cpu = NULL;
  int                 err = 0;

  /* last 16 bits are often immediate, o might be usefule to split */
  info->bytes_per_chunk = 2;

  /* status of the CPU is preserved between calls, so data addresses can be
   * determined */
  if (info->private_data == NULL)
  {
    /* allocate status structure */
    info->private_data = (cpu_status_t*) malloc(sizeof(cpu_status_t));
    memset(info->private_data, 0, sizeof(cpu_status_t));
  }
  cpu = (cpu_status_t*) info->private_data;

  /* read, convert to int and parse instruction */
  err = info->read_memory_func(memaddr, instr_buf, 4, info);
  if (err != 0)
  {
    info->memory_error_func(err, memaddr, info);
  }
  instr = insn_arr_to_int(instr_buf);
  op = insn_to_op_struct(instr);

  /* update state of CPU */
  cpu->regs[0] = 0; /* dummy line to supress error */

  /* print decoded opcode */
  print(fd, ".dword 0x%04X", instr);
  op.op = op.op; /* dummy line to supress error */

  /* Say how many bytes we consumed.  */
  return 4;
}

int
insn_arr_to_int(uint8_t *array)
{
  uint32_t * arr32 = (uint32_t*) array;

  return htobe32(arr32[0]);
}

instruction_t
insn_to_op_struct(uint32_t instr)
{
  instruction_t insn = {
    .op = wrong_op,
    .imm = instr
  };
  return insn;
}
