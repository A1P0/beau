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
#include "type.h"
#include "sym.h"
#include "compile.h"

/* LLVM module */
static LLVMModuleRef module;
/* source file name */
static char *filename;

static LLVMValueRef /* return LLVMValueRef of a literal */
literal(node *n)
{
        switch (n->token->type) {

        case T_DEC_LIT:
                return LLVMConstIntOfString(LLVMInt64Type(), n->token->string,
                        10);

        }

        return NULL;
}

static void /* Global variable definitions */
globalvardef(node *n)
{
        LLVMValueRef global;
        sym *s;
        type *t;

        s = sym_search(n->name, NULL);
        if (s != NULL)
                lfatal("%s: Line %d: Referenced symbol already defined.",
                        filename, n->token->line);

        t = type_search(n->datatype);

        if (t == NULL)
                lfatal("%s: Line %d: Referenced type doesn't exist.",
                        filename, n->token->line);

        global = LLVMAddGlobal(module, t->type, n->name);

        s = lalloc(sizeof(sym));
        
        s->name = n->name;
        s->datatype = t;
        s->value = global;

        sym_add(s, NULL);

        if (n->middle == NULL)
                return;

        LLVMSetInitializer(global, literal(n->middle));
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
compile(node *tree, char *name)
{
        LLVMTargetMachineRef machine;
        LLVMTargetRef target;
        char *errors;

        filename = name;

        LLVMInitializeAllTargetInfos();
        LLVMInitializeAllTargets();
        LLVMInitializeAllTargetMCs();
        LLVMInitializeAllAsmParsers();
        LLVMInitializeAllAsmPrinters();

        LLVMInitializeCore(LLVMGetGlobalPassRegistry());

        module = LLVMModuleCreateWithName(name);

        type_init();

        root(tree);

        LLVMDumpModule(module);

        LLVMGetTargetFromTriple(LLVMGetDefaultTargetTriple(), &target, &errors);
        machine = LLVMCreateTargetMachine(target, LLVMGetDefaultTargetTriple(),
                "generic", LLVMGetHostCPUFeatures(), LLVMCodeGenLevelDefault,
                LLVMRelocDefault, LLVMCodeModelDefault);

        LLVMSetTarget(module, LLVMGetDefaultTargetTriple());

        LLVMTargetMachineEmitToFile(machine, module, "result.o", LLVMObjectFile,
                &errors);

        LLVMDisposeModule(module);
}