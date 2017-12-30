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
  switch (op.type)
  {
    case instr_type_r:
      print(fd, "%s $%d, $%d, $%d, 0x%x", op.descr->name, op.rd, op.rs, op.rb, op.imm);
      break;
    case instr_type_i:
      print(fd, "%s $%d, $%d, 0x%x", op.descr->name, op.rd, op.rs, op.imm);
      break;
    case instr_type_j:
      print(fd, "%s $pc+(%x*4)", op.descr->name, op.imm);
      break;
    default:
      print(fd, "%s 0x%04X", op.descr->name, (instr));
  }
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
  int opcode_int = (instr & OPCODE_MASK) >> OPCODE_SHIFT;
  insn_descr_t * descr = &opcodes[first_invalid_opcode];
  instruction_t insn = {
    .op = wrong_op,
    .imm = instr,
    .descr = descr
  };

  /* if is valid instruction, convert int to enum */
  if (opcode_int < first_invalid_opcode)
  {
    insn.op = (opcode_t) opcode_int;
    descr = &opcodes[opcode_int];
    insn.descr = descr;
    /* imm is extracted only for valid opcodes, otherwise it stores whole
     * instruction */
    insn.imm = (instr & descr->imm->mask) >> descr->imm->shift;
  }
  insn.type = descr->type;
  insn.rd = (instr & descr->rd->mask) >> descr->rd->shift;
  insn.rs = (instr & descr->rs->mask) >> descr->rs->shift;
  insn.rb = (instr & descr->rb->mask) >> descr->rb->shift;
  return insn;
}

/* extraction params for R-type instructions */
const argument_t rd_r = {RD_R_MASK, RD_R_SHIFT};
const argument_t rs_r = {RS_R_MASK, RS_R_SHIFT};
const argument_t rb_r = {RB_R_MASK, RB_R_SHIFT};
const argument_t imm_r = {IMM_R_MASK, IMM_R_SHIFT};

/* extraction params for I-type instructions */
const argument_t rd_i = {RD_I_MASK, RD_I_SHIFT};
const argument_t rs_i = {RS_I_MASK, RS_I_SHIFT};
const argument_t rb_i = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t imm_i = {IMM_I_MASK, IMM_I_SHIFT};

/* extraction params for J-type instructions */
const argument_t rd_j = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t rs_j = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t rb_j = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t imm_j = {IMM_J_MASK, IMM_J_SHIFT};

/* extraction params for unknown instructions */
const argument_t rd_u = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t rs_u = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t rb_u = {EMPTY_MASK, EMPTY_SHIFT};
const argument_t imm_u = {EMPTY_MASK, EMPTY_SHIFT};

insn_descr_t opcodes[] = {
  /* {enum, name, type, rd_mask, rd_shift} */
  /* 00 */ {jmp, "jmp", instr_type_j, &rd_j, &rs_j, &rb_j, &imm_j},
  /* 01 */ {call, "call", instr_type_j, &rd_j, &rs_j, &rb_j, &imm_j},
  /* 02 */ {op_2, "unk2", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 03 */ {j_, "j?", instr_type_j, &rd_j, &rs_j, &rb_j, &imm_j},
  /* 04 */ {jg, "jg", instr_type_j, &rd_j, &rs_j, &rb_j, &imm_j},
  /* 05 */ {op_5, "unk5", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 06 */ {lh, "lh", instr_type_i, &rd_i, &rs_i, &rb_i, &imm_i},
  /* 07 */ {op_7, "unk7", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 08 */ {op_8, "unk8", instr_type_i, &rd_i, &rs_i, &rb_i, &imm_i},
  /* 09 */ {op_9, "unk9", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0a */ {op_a, "unkA", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0b */ {op_b, "unkB", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0c */ {op_c, "unkC", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0d */ {op_d, "unkD", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0e */ {op_e, "unkE", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 0f */ {op_f, "unkF", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 10 */ {op_10, "unk10", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 11 */ {ret, "ret", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 12 */ {op_12, "unk12", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 13 */ {op_13, "unk13", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 14 */ {op_14, "unk14", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 15 */ {op_15, "unk15", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 16 */ {op_16, "unk16", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 17 */ {op_17, "unk17", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 18 */ {op_18, "unk18", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 19 */ {op_19, "unk19", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1a */ {op_1a, "unk1A", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1b */ {op_1b, "unk1B", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1c */ {op_1c, "unk1C", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1d */ {op_1d, "unk1D", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1e */ {op_1e, "unk1E", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 1f */ {op_1f, "unk1F", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 20 */ {op_20, "unk20", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 21 */ {lw, "lw", instr_type_r, &rd_r, &rs_r, &rb_r, &imm_r},
  /* 22 */ {op_22, "unk22", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 23 */ {op_23, "unk23", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 24 */ {op_24, "unk24", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 25 */ {op_25, "unk25", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 26 */ {op_26, "unk26", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 27 */ {op_27, "unk27", instr_type_i, &rd_i, &rs_i, &rb_i, &imm_i},
  /* 28 */ {op_28, "unk28", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 29 */ {op_29, "unk29", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 2a */ {la, "la", instr_type_i, &rd_i, &rs_i, &rb_i, &imm_i},
  /* 2b */ {op_2b, "unk2B", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 2c */ {op_2c, "unk2C", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 2d */ {op_2d, "unk2D", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 2e */ {op_2e, "unk2E", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 2f */ {cmp, "cmp", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 30 */ {op_30, "unk30", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 31 */ {op_31, "unk31", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 32 */ {op_32, "unk32", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 33 */ {op_33, "unk33", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 34 */ {op_34, "unk34", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 35 */ {sw, "sw", instr_type_r, &rd_r, &rs_r, &rb_r, &imm_r},
  /* 36 */ {op_36, "unk36", instr_type_r, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 37 */ {op_37, "unk37", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 38 */ {op_38, "unk38", instr_type_r, &rd_r, &rs_r, &rb_r, &imm_r},
  /* 39 */ {op_39, "unk39", instr_type_r, &rd_r, &rs_r, &rb_r, &imm_r},
  /* 3a */ {op_3a, "unk3A", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 3b */ {op_3b, "unk3B", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 3c */ {op_3c, "unk3C", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 3d */ {op_3d, "unk3D", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 3e */ {op_3e, "unk3E", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  /* 3f */ {op_3f, "unk3F", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
// /* xx */ {op_x, "unkX", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u},
  {wrong_op, ".dword", first_invalid_type, &rd_u, &rs_u, &rb_u, &imm_u}
};
