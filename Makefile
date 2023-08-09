VPATH=src/frontend:src/structure:src/util:

# 产生前端分析代码
frontend: src/structure/ast.h src/frontend/lexer.l src/frontend/parser.y
	cd src/frontend && bison -d -o parser.cpp parser.y -t -r all && flex -o lexer.cpp lexer.l

# # 产生前端目标文件
# test-frontend: frontend
# 	cd build && g++ ../src/frontend/*.cpp -c

# 产生调试程序
test-main: frontend
	mkdir -p build 
	clang++ -lm -O0 -g -Wall -Xclang -disable-O0-optnone -fdiagnostics-color=always -DDEBUG \
	src/**/*.cpp \
	src/main.cpp \
	-I./src -o build/crash

# submit: frontend
	

.PHONY: clean
clean:
	rm -f build/*