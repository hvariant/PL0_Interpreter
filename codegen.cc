#include "codegen.h"
#include <assert.h>
#include <sstream>

using std::stringstream;

const char* opr_list[] = {"ret","neg","add","sub","mul","div","odd","eq","neq","lt","ge","gt","le"};

int string_to_n(const char* s){
    int n;
    stringstream ss;
    ss << s;
    ss >> n;

    return n;
}

void codegen::generate_code(const ast* t){
    assert(t->type() == AST_PROGRAM);
    assert(t->children_size() == 1);

    gen_block(t->get_child(0),NULL,"#program#");

    resolve_call_refs();
}

void codegen::gen_block(const ast* t,proc_local* parent,const string& name){
    proc_local* block_local = new proc_local(name, parent, output.size() );
    add_to_map(block_local);

    if(parent == NULL){
        g_program = block_local;
    } else {
        parent->children.push_back(block_local);
    }

    int start_index = emit(pcode("jmp",0,-1));

    int i = 0, nvars;
    const ast* child = t->get_child(i);

    if(child->type() == AST_CONST){
        gen_const(child,block_local);

        child = t->get_child(++i);
    }

    nvars = 0;
    if(child->type() == AST_VAR){
        gen_vars(child,block_local);
        nvars = child->children_size();

        child = t->get_child(++i);
    }

    while(child->type() == AST_PROCEDURE){
        gen_procedure(child,block_local);
        child = t->get_child(++i);
    }

    int index = output.size();
    output[start_index].a = index;

    if(nvars > 0){
        emit(pcode("int",0,nvars));
    }
    gen_statement(child,block_local);

    if(parent == NULL){
        emit(pcode("halt",0,0));
    } else {
        emit(pcode("opr",0,OPR_RET)); //return
    }
}

void codegen::gen_procedure(const ast* t,proc_local* parent){
    const ast* child = t->get_child(0); //name of the procedure
    assert(child->type() == AST_TERMINAL);
    assert(child->token() == PL0_ID);
    string name = child->term_sym();

    child = t->get_child(1); //block
    gen_block(child,parent,name);
}

void codegen::gen_const(const ast* t,proc_local* tab){
    assert(t->children_size() > 0);
    assert(t->children_size() % 2 == 0);

    for(int i=0;i<t->children_size();i += 2){
        const ast* name = t->get_child(i);
        const ast* val = t->get_child(i+1);

        assert(name->type() == AST_TERMINAL);
        assert(name->token() == PL0_ID);

        assert(val->type() == AST_TERMINAL);
        assert(val->token() == PL0_NUMBER);

        int n = string_to_n(val->term_sym());
        tab->consts.insert(def_pair(name->term_sym(),n));
    }
}

void codegen::gen_vars(const ast* t,proc_local* tab){
    assert(t->type() == AST_VAR);
    assert(t->children_size() > 0);

    for(int i=0; i<t->children_size(); i++){
        const ast* var = t->get_child(i);
        assert(var->type() == AST_TERMINAL);
        assert(var->token() == PL0_ID);

        tab->vars.push_back(var->term_sym());
    }
}

void codegen::gen_statement(const ast* t,proc_local* tab){
    if(t->children_size() == 0) return;

    const ast* child = t->get_child(0);
    switch(child->type()){
        case AST_ASSIGN:
        {
            const ast* var = child->get_child(0);
            assert(var->type() == AST_TERMINAL);
            assert(var->token() == PL0_ID);

            const ast* expr = child->get_child(1);
            gen_expression(expr,tab);

            emit_ref(pcode("sto",-1,-1), var->term_sym(), tab);
            break;
        }
        case AST_CALL:
        {
            const ast* func = child->get_child(0);
            assert(func->type() == AST_TERMINAL);
            assert(func->token() == PL0_ID);

            emit_ref(pcode("cal",-1,-1),func->term_sym(), tab);
            break;
        }
        case AST_STATEMENT:
        {
            gen_statement(child,tab);
            for(int i=1;i<t->children_size();i++){
                child = t->get_child(i);
                gen_statement(child,tab);
            }

            break;
        }
        case AST_IF:
        {
            const ast* cond = child->get_child(0);
            gen_condition(cond,tab);

            int i = emit(pcode("jpc",0,-1));

            const ast* stmt = child->get_child(1);
            gen_statement(stmt,tab);

            int index = output.size();
            output[i].a = index;

            break;
        }
        case AST_WHILE:
        {
            int i0 = output.size();

            const ast* cond = child->get_child(0);
            gen_condition(cond,tab);

            int i = emit(pcode("jpc",0,-1));
            const ast* stmt = child->get_child(1);
            gen_statement(stmt,tab);

            emit(pcode("jmp",0,i0));

            int i1 = output.size();
            output[i].a = i1;

            break;
        }
        case AST_READ:
        {
            const ast* var = child->get_child(0);
            assert(var->type() == AST_TERMINAL);
            assert(var->token() == PL0_ID);

            emit_ref(pcode("read",-1,-1), var->term_sym(), tab);
            break;
        }
        case AST_WRITE:
        {
            const ast* var = child->get_child(0);
            assert(var->type() == AST_TERMINAL);
            assert(var->token() == PL0_ID);

            emit_ref(pcode("write",-1,-1), var->term_sym(), tab);
            break;
        }
        default:
        {
            fprintf(stderr,"[codegen] invalid statement type:%s\n",ast::get_ast_type_map(child->type()));

            exit(1);
        }
    }
}

void codegen::gen_condition(const ast* t,proc_local* tab){
    const ast* child = t->get_child(0);

    if(child->type() == AST_ODD){
        const ast* cchild = child->get_child(0);
        gen_expression(cchild,tab);

        emit(pcode("opr",0,OPR_ODD));
    } else {
        const ast* child2 = t->get_child(2);

        gen_expression(child,tab);
        gen_expression(child2,tab);

        gen_compare(t->get_child(1));
    }
}

void codegen::gen_expression(const ast* t,proc_local* tab){
    if(t->type() == AST_TERMINAL){
        if(t->token() == PL0_NUMBER){
            int n = string_to_n(t->term_sym());

            emit(pcode("lit",0,n));
        } else {
            emit_ref(pcode("lod",-1,-1),t->term_sym(), tab);
        }

        return;
    }

    const ast* child = t->get_child(0);

    if(child->type() == AST_POSITIVE){
        gen_expression(child->get_child(0),tab);
        return;
    }

    if(child->type() == AST_NEGATIVE){
        gen_expression(child->get_child(0),tab);
        emit(pcode("opr",0,OPR_NEG));

        return;
    }

    switch(t->type()){
        case AST_PLUS:
        {
            const ast* op1 = t->get_child(0);
            const ast* op2 = t->get_child(1);

            gen_expression(op1,tab);
            gen_expression(op2,tab);

            emit(pcode("opr",0,OPR_ADD));
            break;
        }
        case AST_MINUS:
        {
            const ast* op1 = t->get_child(0);
            const ast* op2 = t->get_child(1);

            gen_expression(op1,tab);
            gen_expression(op2,tab);

            emit(pcode("opr",0,OPR_SUB));
            break;
        }
        case AST_TIMES:
        {
            const ast* op1 = t->get_child(0);
            const ast* op2 = t->get_child(1);

            gen_expression(op1,tab);
            gen_expression(op2,tab);

            emit(pcode("opr",0,OPR_MUL));
            break;
        }
        case AST_DIV:
        {
            const ast* op1 = t->get_child(0);
            const ast* op2 = t->get_child(1);

            gen_expression(op1,tab);
            gen_expression(op2,tab);

            emit(pcode("opr",0,OPR_DIV));
            break;
        }
        default:
        {
            fprintf(stderr,"[codegen] unknown operation type '%s' in expression\n",ast::get_ast_type_map(t->type()));
            exit(1);

            break;
        }
    }
}

void codegen::gen_compare(const ast* t){
    assert(t->type() == AST_TERMINAL);
    assert(t->token() == PL0_COMPARE || t->token() == PL0_EQ);

    string op = t->term_sym();
    if(op == "="){
        emit(pcode("opr",0,OPR_EQ));
    } else if(op == "#"){
        emit(pcode("opr",0,OPR_NEQ));
    } else if(op == ">"){
        emit(pcode("opr",0,OPR_GT));
    } else if(op == "<"){
        emit(pcode("opr",0,OPR_LT));
    } else if(op == ">="){
        emit(pcode("opr",0,OPR_GE));
    } else if(op == "<="){
        emit(pcode("opr",0,OPR_LT));
    } else {
        fprintf(stderr,"[codegen] invalid operator '%s' in for PL0_COMPARE\n",op.c_str());

        exit(1);
    }
}

void codegen::print(FILE* fp){
    for(int i=0;i<output.size();i++){
        string s = output[i].to_s();
        fprintf(fp,"%d %s\n",i,s.c_str());
    }

    for(int i=0;i<call_refs_list.size();i++){
        fprintf(fp,"%d: %s  -> %s\n",call_refs_list[i].index, call_refs_list[i].full_proc_name.c_str() , call_refs_list[i].ref_name.c_str());
    }
}

string pcode::to_s() const{
    stringstream ss;

    if(cmd == "lit"){
        ss << cmd << " " << a;
    } else if(cmd == "opr"){
        ss << cmd << " " << opr_list[a];
    } else if(cmd == "lod"){
        ss << cmd << " " << l << "," << a;
    } else if(cmd == "sto"){
        ss << cmd << " " << l << "," << a;
    } else if(cmd == "cal"){
        ss << cmd << " " << l << "," << a;
    } else if(cmd == "int"){
        ss << cmd << " " << a;
    } else if(cmd == "jmp"){
        ss << cmd << " " << a;
    } else if(cmd == "jpc"){
        ss << cmd << " " << a;
    } else if(cmd == "halt"){
        ss << cmd ;
    } else if(cmd == "read"){
        ss << cmd << " " << l << "," << a;
    } else if(cmd == "write"){
        ss << cmd << " " << l << "," << a;
    } else {
        fprintf(stderr,"[pcode] invalid command:%s\n",cmd.c_str());
    }

    return ss.str();
}

string proc_local::full_name() const{
    if(parent == NULL){
        return name;
    }

    string ret = parent->full_name();

    return ret + "::" + name;
}

void codegen::add_to_map(proc_local* tab){
    proc_local_map.insert(pair<string, proc_local*>(tab->full_name(),tab));
}

static int lookup_symbol(proc_local* tab, const string& sym,int cur_level, int* level, int* index){
    if(tab == NULL){
        return 0;
    }

    for(int i=0;i<tab->vars.size();i++){
        if(tab->vars[i] == sym){
            *level = cur_level;
            *index = i;

            return 1;
        }
    }

    return lookup_symbol(tab->parent,sym,cur_level+1,level,index);
}

static int lookup_const(proc_local* tab, const string& sym, int* val){
    if(tab == NULL){
        return 0;
    }

    if(tab->consts.find(sym) != tab->consts.end()){
        *val = tab->consts[sym];

        return 1;
    }

    return lookup_const(tab->parent,sym,val);
}

int codegen::emit_ref(const pcode& c, const string& sym, proc_local* tab){
    int o_index;

    if(c.cmd == "lod" || c.cmd == "sto" || c.cmd == "read" || c.cmd == "write"){ //@TODO: finish
        int level,index,val;

        if(c.cmd == "lod" && lookup_const(tab,sym,&val)){
            pcode nc("lit",0,val);
            o_index = output.size();
            output.push_back(nc);
        } else {
            if(!lookup_symbol(tab,sym,0,&level,&index)){
                fprintf(stderr,"[codegen]: undefined reference to symbol '%s' in '%s'\n",sym.c_str(), tab->full_name().c_str());
                exit(1);
            }

            pcode nc(c.cmd,level,index);
            o_index = output.size();
            output.push_back(nc);
        }
    } else {
        assert(c.cmd == "cal");

        o_index = output.size();
        output.push_back(c);

        call_refs_list.push_back(call_ref(o_index,tab->full_name(),sym));
    }

    return o_index;
}

void codegen::resolve_call_refs(){
    for(int i=0;i<call_refs_list.size();i++){
        int index = call_refs_list[i].index;
        proc_local* pl = proc_local_map[call_refs_list[i].full_proc_name];

        if(pl->name == call_refs_list[i].ref_name){ //self recursion
            output[index].l = 0;
            output[index].a = pl->addr;

            continue;
        }

        //check for local procedures first
        proc_local* target = NULL;

        for(int j=0;j<pl->children.size();j++){
            if(pl->children[j]->name == call_refs_list[i].ref_name){
                target = pl->children[j];
                break;
            }
        }

        if(target != NULL){
            int target_ind = target->addr;
            output[index].l = 1;
            output[index].a = target_ind;
        } else { //climb up the hierachy
            proc_local* cur = pl->parent;
            int level = 0;

            while(cur && !target){
                for(int j=0;j<cur->children.size();j++){
                    if(cur->children[j]->name == call_refs_list[i].ref_name){
                        target = cur->children[j];

                        break;
                    }
                }

                cur = cur->parent;
                level++;
            }

            if(cur == NULL){
                fprintf(stderr,"[codegen] undefined reference to procedure '%s' in %s\n",call_refs_list[i].ref_name.c_str(),call_refs_list[i].full_proc_name.c_str());

                exit(1);
            }

            int target_ind = target->addr;
            output[index].l = level;
            output[index].a = target_ind;
        }
    }

    call_refs_list.clear();
}

int codegen::emit(const pcode& c){
    int index = output.size();
    output.push_back(c);

    return index;
}

codegen::~codegen(){ //@TODO: implement
    
}
