/*
 * compile.c
 * Code generation procedures
 */
#include <stdio.h>
#include <llvm-c/Core.h>
#include <llvm-c/Target.h>
#include <llvm-c/Types.h>
#include <llvm-c/TargetMachine.h>
#include "lone.h"
#include "node.h"
#include "compile.h"

/* LLVM module */
static LLVMModuleRef module;

static void /* Global variable definitions */
globalvardef(node *n)
{
        LLVMValueRef global;
        LLVMValueRef val;
        
        global = LLVMAddGlobal(module, LLVMInt8Type(), "ayyo");
        val = LLVMConstInt(LLVMInt8Type(), 42, false);
        LLVMSetInitializer(global, val);
}

static void /* root level code gen */
root(node *tree)
{
        switch (tree->type) {

        case N_INCLUDE: /* skip for now */
                break;

        case N_VARDEF:
                globalvardef(tree);
                break;

        }

        if (tree->next != NULL)
                root(tree->next);
}

void
compile(node *tree, char *filename)
{
        LLVMTargetMachineRef machine;
        LLVMTargetRef target;
        char *errors;

        LLVMInitializeAllTargetInfos();
        LLVMInitializeAllTargets();
        LLVMInitializeAllTargetMCs();
        LLVMInitializeAllAsmParsers();
        LLVMInitializeAllAsmPrinters();

        LLVMInitializeCore(LLVMGetGlobalPassRegistry());

        module = LLVMModuleCreateWithName(filename);

        root(tree);

        LLVMDumpModule(module);

        LLVMGetTargetFromTriple(LLVMGetDefaultTargetTriple(), &target, &errors);
        machine = LLVMCreateTargetMachine(target, LLVMGetDefaultTargetTriple(),
                "generic", LLVMGetHostCPUFeatures(), LLVMCodeGenLevelDefault,
                LLVMRelocDefault, LLVMCodeModelDefault);

        LLVMSetTarget(module, LLVMGetDefaultTargetTriple());

        LLVMTargetMachineEmitToFile(machine, module, "result.o", LLVMObjectFile, &errors);

        LLVMDisposeModule(module);
}