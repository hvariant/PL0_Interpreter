#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "parser.h"
#include <map>

using std::map;
using std::vector;
using std::pair;
using std::string;

typedef enum{
    OPR_RET,
    OPR_NEG,
    OPR_ADD,
    OPR_SUB,
    OPR_MUL,
    OPR_DIV,
    OPR_ODD,
    OPR_EQ,
    OPR_NEQ,
    OPR_LT,
    OPR_GE,
    OPR_GT,
    OPR_LE,
} opr_type;

struct pcode{
    pcode(const string& s,int _l,int _a)
    :cmd(s)
    ,l(_l)
    ,a(_a)
    { }

    string cmd;
    int l;
    int a;

    string to_s() const;
};

typedef map<string, int> symtab_type;
typedef pair<string, int> def_pair;

struct proc_local{
    proc_local(const string& s,proc_local* _parent,int _addr)
    :name(s)
    ,parent(_parent)
    ,addr(_addr)
    { }

    vector<string> vars;
    symtab_type consts;

    string full_name() const;

    string name;
    int addr;

    proc_local* parent;
    vector<proc_local*> children;
};

struct call_ref{
    call_ref(int i,const string& proc,const string& ref)
    :index(i)
    ,full_proc_name(proc)
    ,ref_name(ref)
    { }
    string full_proc_name;
    string ref_name;

    int index;
};


class codegen{
public:
    codegen()
    :g_program(NULL)
    {}
    ~codegen();

    void generate_code(const ast* t);
    void print(FILE* fp);

    vector<pcode>& get_output(){ return output; }
private:
    void gen_block(const ast* t,proc_local*,const string& name);
    void gen_procedure(const ast* t,proc_local*);

    void gen_const(const ast* t,proc_local*);
    void gen_vars(const ast* t,proc_local*);

    void gen_statement(const ast* t,proc_local*);
    void gen_condition(const ast* t,proc_local*);
    void gen_expression(const ast* t,proc_local*);

    void gen_compare(const ast* t);

    int emit_ref(const pcode& c, const string& sym, proc_local*);
    int emit(const pcode& c);

    void resolve_call_refs();

    vector<call_ref> call_refs_list;

    proc_local* g_program;

    void add_to_map(proc_local* tab);
    map<string, proc_local*> proc_local_map;

    vector<pcode> output;
};

#endif
