#include "pl0_vm.h"

//#define VM_DEBUG

plzero_vm::plzero_vm(int stack_size)
:S(stack_size)
{ }

int plzero_vm::base(int l){
    int ret = b;
    for(int i=0;i<l;i++){
        ret = S[ret];
    }

    return ret;
}

void plzero_vm::run(const vector<pcode>& program){
    t = 2;
    b = 1;
    p = 0;
    S[0] = S[2] = 0;
    S[1] = 1;

    int iter = 0;

    while(1){
        iter++;

#ifdef VM_DEBUG
        fprintf(stdout,"------------------\n");
        fprintf(stdout,"%d - ", p);
#endif

        pcode c = program[p];
        p++;

#ifdef VM_DEBUG
        fprintf(stdout,"%s\n",c.to_s().c_str());
#endif

        if(c.cmd == "lit"){
            S[t+1] = c.a;
            t = t+1;
        } else if(c.cmd == "opr"){
            switch(c.a){
                case OPR_RET:
                {
                    t = b-2;
                    p = S[t+3];
                    b = S[t+1];

                    break;
                }
                case OPR_ADD:
                {
                    t = t- 1;
                    S[t] = (S[t] + S[t+1]);
                    break;
                }
                case OPR_SUB:
                {
                    t = t - 1;
                    S[t] = (S[t] - S[t+1]);
                    break;
                }
                case OPR_MUL:
                {
                    t = t - 1;
                    S[t] = (S[t] * S[t+1]);
                    break;
                }
                case OPR_DIV:
                {
                    t = t - 1;
                    S[t] = (S[t] / S[t+1]);
                    break;
                }
                case OPR_EQ:
                {
                    t = t - 1;
                    S[t] = (S[t] == S[t+1]);
                    break;
                }
                case OPR_NEQ:
                {
                    t = t - 1;
                    S[t] = (S[t] != S[t+1]);
                    break;
                }
                case OPR_GE:
                {
                    t = t - 1;
                    S[t] = (S[t] >= S[t+1]);
                    break;
                }
                case OPR_LE:
                {
                    t = t - 1;
                    S[t] = (S[t] <= S[t+1]);
                    break;
                }
                case OPR_GT:
                {
                    t = t - 1;
                    S[t] = (S[t] > S[t+1]);
                    break;
                }
                case OPR_LT:
                {
                    t = t - 1;
                    S[t] = (S[t] < S[t+1]);
                    break;
                }
                case OPR_NEG:
                {
                    S[t] = -S[t];
                    break;
                }
                case OPR_ODD:
                {
                    S[t] = (S[t] % 2 == 1);
                    break;
                }
            }
        } else if(c.cmd == "lod"){
            S[t+1] = S[base(c.l) + c.a + 2];
            t = t + 1;
        } else if(c.cmd == "sto"){
            S[base(c.l) + c.a + 2] = S[t];
            t = t - 1;
        } else if(c.cmd == "cal"){
            S[t+1] = b;
            S[t+2] = base(c.l);
            S[t+3] = p;

            b = t+2;
            p = c.a;
            t = t+3;
        } else if(c.cmd == "int"){
            t += c.a;
        } else if(c.cmd == "jmp"){
            p = c.a;
        } else if(c.cmd == "jpc"){
            if(S[t--] == 0){
                p = c.a;
            }
        } else if(c.cmd == "halt"){
            break;
        } else if(c.cmd == "read"){
            int n;
            scanf("%d",&n);

            S[base(c.l) + c.a + 2] = n;
        } else if(c.cmd == "write"){
            printf("%d\n",S[base(c.l) + c.a + 2]);
        } else {
            fprintf(stderr,"[plzero_vm] invalid opcode '%s' in location %d\n",c.cmd.c_str(),p);

            exit(1);
        }

#ifdef VM_DEBUG
        print_S(stdout);
        fprintf(stdout,"\n");
#endif
    }

#ifdef VM_DEBUG
    print_S(stdout);
    fprintf(stdout,"\n");
#endif

    fprintf(stderr,"terminating plzero_vm\n");
}

void plzero_vm::print_S(FILE* fp){
    for(int i=0;i<=t;i++){
        fprintf(fp,"%d ",S[i]);
    }
    fprintf(fp,"\n");
}
