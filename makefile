GCC_FLAGS=-ly -g -Wall -Wno-unused-function

SRC_CONTEXT=src
TGT_CONTEXT=target
TEST_CONTEXT=test

DOTNET_PROJ=$(SRC_CONTEXT)/dotnet
DEPS= test.vi $(DOTNET_PROJ)/lib.cs $(SRC_CONTEXT)/grammar.y $(SRC_CONTEXT)/lex.l $(SRC_CONTEXT)/node.c $(SRC_CONTEXT)/types.c $(SRC_CONTEXT)/print.c $(SRC_CONTEXT)/variables.c $(SRC_CONTEXT)/translator.c $(SRC_CONTEXT)/errors.c
OUT=$(TGT_CONTEXT)/compiler

INPUT=test.vi
OUTPUT=a.out

CS_FILE=$(TGT_CONTEXT)/dotnet/Program.cs


INCLUDE=include\

	# dotnet format $(TGT_CONTEXT)/dotnet/


$(OUT): $(DEPS)
	@mkdir -p $(TGT_CONTEXT)
	@mkdir -p $(TGT_CONTEXT)/dotnet/
	lex -o $(TGT_CONTEXT)/lex.yy.c $(SRC_CONTEXT)/lex.l
	bison -b $(TGT_CONTEXT)/y -v -d $(SRC_CONTEXT)/grammar.y
	gcc -o $(OUT) $(TGT_CONTEXT)/lex.yy.c  -I $(INCLUDE) $(SRC_CONTEXT)/types.c $(SRC_CONTEXT)/node.c $(SRC_CONTEXT)/print.c $(SRC_CONTEXT)/variables.c $(SRC_CONTEXT)/translator.c  $(SRC_CONTEXT)/errors.c $(TGT_CONTEXT)/y.tab.c   $(GCC_FLAGS)


compile: $(OUT)
	cp -R $(DOTNET_PROJ)/ $(TGT_CONTEXT)/dotnet/
	./$(OUT) $(CS_FILE) < $(INPUT)
	dotnet publish $(TGT_CONTEXT)/dotnet/ -a x64 -c release -v q -nologo -o $(TGT_CONTEXT)/bin
	cp $(TGT_CONTEXT)/bin/dotnet $(OUTPUT)

translate: $(OUT)
	./$(OUT) $(CS_FILE) < $(INPUT)

debug: $(DEPS)
	@mkdir -p $(TGT_CONTEXT)
	lex -o $(TGT_CONTEXT)/lex.yy.c $(SRC_CONTEXT)/lex.l
	bison -b $(TGT_CONTEXT)/y -Wcounterexamples -v -d $(SRC_CONTEXT)/grammar.y 

	

all: $(OUT)

clean:
	rm -rf $(TGT_CONTEXT)/*
	rm -rf $(OUTPUT)


.PHONY: all clean