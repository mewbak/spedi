//===------------------------------------------------------------*- C++ -*-===//
//
// This file is distributed under BSD License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// 
// Copyright (c) 2016 University of Kaiserslautern.

#pragma once
#include "DisassemblyCFG.h"
#include "DisassemblyCallGraph.h"
#include "MCInstAnalyzerARM.h"

namespace disasm {

class SectionDisassemblyARM;
class RawInstAnalyzer;

/**
 * SectionDisassemblyAnalyzerARM
 */
class SectionDisassemblyAnalyzerARM {
public:
    SectionDisassemblyAnalyzerARM() = delete;
    SectionDisassemblyAnalyzerARM
        (SectionDisassemblyARM *sec_disasm,
         const std::pair<addr_t, addr_t> &exec_region);

    virtual ~SectionDisassemblyAnalyzerARM() = default;
    SectionDisassemblyAnalyzerARM
        (const SectionDisassemblyAnalyzerARM &src) = default;
    SectionDisassemblyAnalyzerARM
        &operator=(const SectionDisassemblyAnalyzerARM &src) = default;
    SectionDisassemblyAnalyzerARM(SectionDisassemblyAnalyzerARM &&src) =
    default;

    void buildCFG();
    void refineCFG();
    void buildCallGraph();
    /*
     * Search in CFG to find direct successor
     */
    CFGNode *findNextDirectSuccessor
        (const CFGNode &cfg_node) noexcept;
    /*
     * Search in CFG to find remote successor matching target
     */
    CFGNode *findRemoteDirectSuccessor(addr_t target) noexcept;

    void RefineMaximalBlocks(const std::vector<addr_t> &known_code_addrs);
    bool isValidCodeAddr(addr_t addr) const noexcept;
    const DisassemblyCFG &getCFG() const noexcept;

    /*
     * precondition: given instruction is PC-relative load
     */
    CFGNode *findCFGNodeAffectedByLoadStartingFrom
        (const CFGNode &node, addr_t target) noexcept;

    size_t calculateNodeWeight(const CFGNode *node) const noexcept;

    bool isSwitchStatement(const CFGNode &node) const noexcept;

private:
    /*
     * Finds a valid basic block in and invalidates all direct predecessors that
     * do not target it.
     */
    void resolveValidBasicBlock(CFGNode &node);
    void resolveOverlapBetweenCFGNodes(CFGNode &node);
    void resolveCFGConflicts(CFGNode &node);
    void resolveLoadConflicts(CFGNode &node);
    void resolveSwitchStmtsAndLoadConflicts(CFGNode &node);
    void shortenToCandidateAddressOrSetToData
        (CFGNode &node, addr_t addr) noexcept;

private:
    // call graph related methods
    void buildProcedureStartingFrom(CFGNode & entry_node);

private:
    SectionDisassemblyARM *m_sec_disassembly;
    MCInstAnalyzerARM m_analyzer;
    addr_t m_exec_addr_start;
    addr_t m_exec_addr_end;
    DisassemblyCFG m_sec_cfg;
    DisassemblyCallGraph m_call_graph;
};
}