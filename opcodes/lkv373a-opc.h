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

/* R-type destination is 5 bits after opcode */
#define RD_R_MASK 0x3e00000
#define RD_R_SHIFT 21

/* R-type source is 5 bits after destination */
#define RS_R_MASK 0x1f0000
#define RS_R_SHIFT 16

/* R-type base is 5 bits after source */
#define RB_R_MASK 0xf800
#define RB_R_SHIFT 11

/* R-type immediate is 11 bits after base */
#define IMM_R_MASK 0x7ff
#define IMM_R_SHIFT 0
#define IMM_R_SIGNMASK 0x400
#define IMM_R_SIGNEXT 0xfffff800

/* I-type destination is 5 bits after opcode */
#define RD_I_MASK 0x3e00000
#define RD_I_SHIFT 21

/* I-type source is 5 bits after destination */
#define RS_I_MASK 0x1f0000
#define RS_I_SHIFT 16

/* I-type immediate is 16 bits after source */
#define IMM_I_MASK 0xffff
#define IMM_I_SHIFT 0
#define IMM_I_SIGNMASK 0x8000
#define IMM_I_SIGNEXT 0xffff0000

/* J-type immediate is 26 bits after opcode */
#define IMM_J_MASK 0x3ffffff
#define IMM_J_SHIFT 0
#define IMM_J_SIGNMASK 0x2000000
#define IMM_J_SIGNEXT 0xfc000000

/* if unknown, no extraction takes place */
#define EMPTY_MASK 0
#define EMPTY_SHIFT 0
#define EMPTY_SIGNMASK 0
#define EMPTY_SIGNEXT 0

typedef enum {
  wrong_op = -1,
  jmp = 0x0,
  call = 0x1,
  op_2 = 0x2,
  j_ = 0x3,
  jg = 0x4,
  nop = 0x5,
  lh = 0x6,
  op_7 = 0x7,
  op_8 = 0x8,
  op_9 = 0x9,
  op_a = 0xa,
  op_b = 0xb,
  op_c = 0xc,
  op_d = 0xd,
  op_e = 0xe,
  op_f = 0xf,
  op_10 = 0x10,
  ret = 0x11,
  op_12 = 0x12,
  op_13 = 0x13,
  op_14 = 0x14,
  op_15 = 0x15,
  op_16 = 0x16,
  op_17 = 0x17,
  op_18 = 0x18,
  op_19 = 0x19,
  op_1a = 0x1a,
  op_1b = 0x1b,
  op_1c = 0x1c,
  op_1d = 0x1d,
  op_1e = 0x1e,
  op_1f = 0x1f,
  op_20 = 0x20,
  lw = 0x21,
  op_22 = 0x22,
  op_23 = 0x23,
  op_24 = 0x24,
  op_25 = 0x25,
  op_26 = 0x26,
  mov = 0x27,
  op_28 = 0x28,
  op_29 = 0x29,
  la = 0x2a,
  op_2b = 0x2b,
  op_2c = 0x2c,
  op_2d = 0x2d,
  op_2e = 0x2e,
  cmp = 0x2f,
  op_30 = 0x30,
  op_31 = 0x31,
  op_32 = 0x32,
  op_33 = 0x33,
  op_34 = 0x34,
  sw = 0x35,
  op_36 = 0x36,
  op_37 = 0x37,
  op_38 = 0x38,
  op_39 = 0x39,
  op_3a = 0x3a,
  op_3b = 0x3b,
  op_3c = 0x3c,
  op_3d = 0x3d,
  op_3e = 0x3e,
  op_3f = 0x3f,
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
  uint32_t signmask; /**< sign mask */
  uint32_t signext; /**< sign extension */
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

