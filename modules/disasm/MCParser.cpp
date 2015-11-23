//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015 Technical University of Kaiserslautern.
// Created by M. Ammar Ben Khadra.

#include "MCParser.h"
#include "MCInst.h"
#include <cassert>

namespace disasm {

void MCParser::initialize(cs_arch arch, cs_mode mode,
                addr_t end_addr) {

    m_arch = arch;
    m_mode = mode;
    m_end_addr = end_addr;
    cs_err err_no;
    err_no = cs_open(m_arch, m_mode, &m_handle);
    if (err_no) {
        throw std::runtime_error("Failed on cs_open() "
                                     "with error returned:" + err_no);
    }
    cs_option(m_handle, CS_OPT_DETAIL, CS_OPT_ON);
    m_valid = true;
}

MCParser::~MCParser() {
    cs_close(&m_handle);
}

void MCParser::reset(cs_arch arch, cs_mode mode) {
    if(valid())
        cs_close(&m_handle);
    m_arch = arch;
    m_mode = mode;
    cs_err err_no;
    err_no = cs_open(m_arch, m_mode, &m_handle);
    if (err_no) {
        throw std::runtime_error("Failed on cs_open() "
                                     "with error returned:" + err_no);
    }
    cs_option(m_handle, CS_OPT_DETAIL, CS_OPT_ON);
    m_valid = true;
}

void MCParser::changeModeTo(cs_mode mode) {
    m_mode = mode;
    cs_option(m_handle, CS_OPT_MODE, mode);
}

bool MCParser::disasm(const uint8_t *code,
                      size_t *size,
                      addr_t *address,
                      MCInst *inst) {
    assert(*address <= m_end_addr && "Address out of bound");
    return cs_disasm_iter(m_handle, &code, size, address, inst->getRawPtr());
}

////TODO: remove skip data and the corresponding i-word
//bool MCParser::skipData(const uint8_t *code, addr_t *address, MCInst *inst) {
//    addr_t original_addr = *address;
//    assert(*address <= m_end_addr && "Address out of bound");
//    while((*address <= m_end_addr) &&
//        !(cs_disasm_iter(m_handle, &code, 0, address, inst->getRawPtr()))) {
//        *address += static_cast<unsigned int> (m_iword);
//        code += static_cast<unsigned int> (m_iword);
//    }
//    return (original_addr != *address);
//}
}
