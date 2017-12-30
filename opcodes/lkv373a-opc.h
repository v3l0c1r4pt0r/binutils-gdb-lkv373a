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


/* opcode is always first 6 bits */
#define OPCODE_MASK 0xfc000000
#define OPCODE_SHIFT 26

/* J-type immediate is 26 bits after opcode */
#define IMM_J_MASK 0x3ffffff
#define IMM_J_SHIFT 0

/* if unknown, no extraction takes place */
#define EMPTY_MASK 0
#define EMPTY_SHIFT 0

typedef enum {
  wrong_op = -1,
  jmp = 0x0,
  call = 0x1,
  op_2 = 0x2,
  j_ = 0x3,
  jg = 0x4,
  op_5 = 0x5,
  first_invalid_opcode
} opcode_t;

typedef enum {
  instr_type_r,
  instr_type_i,
  instr_type_j,
  first_invalid_type
} instr_type_t;

typedef struct {
  uint32_t mask;
  uint32_t shift;
} argument_t;

typedef struct {
  opcode_t opcode;
  char name[7]; /* max name len plus one */
  instr_type_t type;
  const argument_t *rd;
  const argument_t *rs;
  const argument_t *rb;
  const argument_t *imm;
} insn_descr_t;

typedef struct {
  opcode_t op; /* opcode */
  short rd; /* destination */
  short rs; /* source */
  short rb; /* base */
  int imm; /* immediate: either value, offset or instruction count */
  instr_type_t type;
  insn_descr_t *descr; /* opcode description */
} instruction_t;

typedef struct {
  uint32_t regs[32]; /* values of all 32 CPU registers */
} cpu_status_t;

extern const argument_t rd_r, rs_r, rb_r, imm_r;
extern const argument_t rd_i, rs_i, rb_i, imm_i;
extern const argument_t rd_j, rs_j, rb_j, imm_j;
extern const argument_t rd_u, rs_u, rb_u, imm_u;

extern insn_descr_t opcodes[];

#endif /* LKV373A_OPC */

