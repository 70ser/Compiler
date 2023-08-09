# CC

## 目标

- [x] 词法分析和语法分析(转成AST)
  
- [x] 添加库函数

- [x] 生成中间代码IR
  
- [ ] 优化中间代码
  
- [x] 生成目标代码
  
- [x] 环境部署测试
  

## 环境

wsl ubuntu 20.04

gcc version 9.4.0

bison (GNU Bison) 3.5.1

flex 2.6.4

## 测试
切换到 `devIR` 分支之后会在标准错误输出中输出各个阶段的 `IR` 和寄存器的分配

其中输出主要使用的函数有 `showIR` \ `printIR` \ `print()` 等

可以采用以下的命令来将这些信息重定向到文件中

```shell
./build/crash src/testcase/test.sy -S -o src/testcase/cmp.s 2> src/testcase/test.ir
```