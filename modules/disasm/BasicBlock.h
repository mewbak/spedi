//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2015 Technical University of Kaiserslautern.

#pragma once
#include "common.h"
#include "Fragment.h"
#include <vector>

namespace disasm {

/**
 * BasicBlock
 * a lightweight container for data relevant to basic blocks contained in
 * a maximal block.
 */
class BasicBlock {
public:
    /**
     * Construct a BasicBlock that is initially not valid.  Calling
     * methods other than operator= and valid on this results in
     * undefined behavior.
     */
    explicit BasicBlock(unsigned int id);
    virtual ~BasicBlock() = default;
    BasicBlock(const BasicBlock &src) = default;
    BasicBlock &operator=(const BasicBlock &src) = default;
    BasicBlock(BasicBlock &&src) = default;

    friend class MaximalBlock;
    friend class MaximalBlockBuilder;

    unsigned int id() const;
    bool valid() const ;
    const size_t &size() const;

    bool isAppendableBy(const cs_insn *inst) const;
    bool isAppendableAt(const addr_t addr) const;
    size_t instCount() const;
    addr_t startAddr() const;
    const std::vector<addr_t> &getInstAddresses() const {
        return m_insts_addr;
    }


private:
    void append(const cs_insn *inst);

private:
    unsigned int m_id;
    size_t m_mem_size;
    std::vector<addr_t> m_insts_addr;
};
}