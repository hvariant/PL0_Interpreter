#ifndef __PL0_VM_H__
#define __PL0_VM_H__

#include "codegen.h"

class plzero_vm{
public:
    plzero_vm(int stack_size=65536);
    ~plzero_vm(){ }

    void run(const vector<pcode>& program);
private:
    int base(int l);

    void print_S(FILE* fp);

    vector<int> S;
    int p,b,t;
};

#endif
