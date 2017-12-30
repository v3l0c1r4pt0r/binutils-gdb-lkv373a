/* lkv373a-opc.h -- LKV373A Opcodes

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


#ifndef LKV373A_OPC
#define LKV373A_OPC


typedef enum {
  wrong_op = -1,
  jmp = 0x0,
  call = 0x1,
  op_2 = 0x2,
  first_invalid_opcode
} opcode_t;

typedef struct {
  opcode_t op;
  int imm;
} instruction_t;

typedef struct {
  uint32_t regs[32]; /* values of all 32 CPU registers */
} cpu_status_t;

#endif /* LKV373A_OPC */

