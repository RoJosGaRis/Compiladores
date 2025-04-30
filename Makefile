# Ruta relativa
SYNTAX_DIR = syntax
FLEX_DIR = flex
LEMON = lemon/lemon
FLEX = flex/win_flex.exe

# Archivos
SCANNER_SRC = $(FLEX_DIR)/scanner.c
SCANNER_HDR = $(FLEX_DIR)/scanner.h
PARSER_SRC = $(SYNTAX_DIR)/example.c
PARSER_HDR = $(SYNTAX_DIR)/example.h
DRIVER = $(SYNTAX_DIR)/driver

# Ejecutable final
EXE = $(DRIVER).exe

# Compilador
CXX = g++

# Regla por defecto
all: $(EXE)

# Compila el driver si todo está generado
$(EXE): $(SYNTAX_DIR)/driver.c $(SCANNER_SRC) $(PARSER_SRC)
	$(CXX) -o $@ $(SYNTAX_DIR)/driver.c $(SCANNER_SRC) $(PARSER_SRC)

# Genera el parser con Lemon
$(PARSER_SRC) $(PARSER_HDR): $(SYNTAX_DIR)/example.y
	$(LEMON) $(SYNTAX_DIR)/example.y

# Genera el scanner con win_flex
$(SCANNER_SRC) $(SCANNER_HDR): $(FLEX_DIR)/scanner.l
	$(FLEX) --outfile=$(SCANNER_SRC) --header-file=$(SCANNER_HDR) $(FLEX_DIR)/scanner.l

# Limpieza
.PHONY: clean
clean:
	del /Q "flex/scanner.c" "flex/scanner.h" "syntax/example.c" "syntax/example.h" "syntax/driver.exe" 2>nul

