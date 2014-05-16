CPP = g++
CFLAGS = -g

HEADERS = tokens.h lexer.h parser.h ast.h

LEXER_SRC = lexer.cc
PARSER_SRC = parser.cc ast.cc
COMMON_SRC = tokens.cc

LEXER_DEMO = lexer_demo.cc
PARSER_DEMO = parser_demo.cc

all: lexer_test parser_test

lexer_test: ${LEXER_SRC} ${COMMON_SRC} ${HEADER} ${LEXER_DEMO}
	${CPP} ${LEXER_SRC} ${COMMON_SRC} ${LEXER_DEMO} -o $@ ${CFLAGS}

parser_test: ${LEXER_SRC} ${COMMON_SRC} ${PARSER_SRC} ${HEADER} ${PARSER_DEMO}
	${CPP} ${LEXER_SRC} ${COMMON_SRC} ${PARSER_SRC} ${PARSER_DEMO} -o $@ ${CFLAGS}

clean:
	rm -rf lexer_test
	rm -rf parser_test