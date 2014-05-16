CPP = g++
CFLAGS = -g

HEADERS = tokens.h lexer.h parser.h

LEXER_SRC = lexer.cc
PARSER_SRC = parser.cc
COMMON_SRC = tokens.cc

LEXER_DEMO = lexer_demo.cc
PARSER_DEMO = parser_demo.cc

lexer_test: ${LEXER_SRC} ${COMMON_SRC} ${HEADER} ${LEXER_DEMO}
	${CPP} ${LEXER_SRC} ${COMMON_SRC} ${LEXER_DEMO} -o $@ ${CFLAGS}

clean:
	rm -rf lexer_test
