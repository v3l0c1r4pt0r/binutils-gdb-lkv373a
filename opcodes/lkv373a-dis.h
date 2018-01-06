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

#ifndef LKV373A_DIS_H
#define LKV373A_DIS_H 1

#ifdef __cplusplus
extern "C" {
#endif

#include "lkv373a-opc.h"

int print_text_lkv373a (bfd_vma memaddr, struct disassemble_info * info);
int print_data_lkv373a (bfd_vma memaddr, struct disassemble_info * info);
int insn_arr_to_int(uint8_t *array);
instruction_t insn_to_op_struct(uint32_t instr);
void update_cpu(cpu_status_t *cpu, instruction_t op);

#ifdef __cplusplus
}
#endif

#endif /* lkv373a-dis.h */
