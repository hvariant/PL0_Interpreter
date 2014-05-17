CPP = g++
CFLAGS = -g -fno-inline -Wall -Werror -Wno-sign-compare -Wno-reorder

HEADERS = tokens.h lexer.h parser.h ast.h codegen.h pl0_vm.h

LEXER_SRC = lexer.cc
PARSER_SRC = parser.cc ast.cc
COMMON_SRC = tokens.cc

SRCS = ${LEXER_SRC} ${PARSER_SRC} ${COMMON_SRC} compiler.cc codegen.cc pl0_vm.cc

LEXER_DEMO = lexer_demo.cc
PARSER_DEMO = parser_demo.cc

all: lexer_test parser_test compiler

lexer_test: ${LEXER_SRC} ${COMMON_SRC} ${HEADER} ${LEXER_DEMO}
	${CPP} ${LEXER_SRC} ${COMMON_SRC} ${LEXER_DEMO} -o $@ ${CFLAGS}

parser_test: ${LEXER_SRC} ${COMMON_SRC} ${PARSER_SRC} ${HEADER} ${PARSER_DEMO}
	${CPP} ${LEXER_SRC} ${COMMON_SRC} ${PARSER_SRC} ${PARSER_DEMO} -o $@ ${CFLAGS}

compiler: ${SRCS} ${HEADERS}
	${CPP} ${CFLAGS} ${SRCS} -o $@

clean:
	rm -rf lexer_test{,.dSYM}
	rm -rf parser_test{,.dSYM}
	rm -rf compiler{,.dSYM}
