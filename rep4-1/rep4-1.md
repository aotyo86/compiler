# report 4.1
以下の式を手で木に変換して見よ。さらに、これをスタックを使って計算するintel64の命令に落として見よ。
examples のコンパイラを使った結果と比較してみよ。また、gdb で実際にどのような動作をするかを調べてみよ。

- 3-(4-2)
- 0+(1+(3-2))-(0+(1+(2-3)))


---
## 計算 3-(4-2)
[作成したアセンブラコード](https://github.com/aotyo86/compiler/blob/main/rep4-1/cal_64.asm)

### アセンブラ出力
```
$ ./cal_64 
Result: 1
```

### 先生のコード結果
```
3-(4-2)
 = 0x00000001 = 1
```

### gdbによるトレース
以下の命令後のstackを見ることにする

初期のstack
```
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe692
0x7fffffffe440:	0x0000000000000000	0x00007fffffffe6b5
0x7fffffffe450:	0x00007fffffffe6c5
```

`push rax     ; stack 3`
```
0x7fffffffe428:	0x0000000000000003	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe692	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b5
```

`push rax     ; stack 4`
```
0x7fffffffe420:	0x0000000000000004	0x0000000000000003
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe692
0x7fffffffe440:	0x0000000000000000
```

`pop rbx      ; pop 4`
```
0x7fffffffe428:	0x0000000000000003	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe692	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b5
```

`pop rax      ; pop 3`
```
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe692
0x7fffffffe440:	0x0000000000000000	0x00007fffffffe6b5
0x7fffffffe450:	0x00007fffffffe6c5
```

---
## 計算 0+(1+(3-2))-(0+(1+(2-3)))

[作成したアセンブラコード](https://github.com/aotyo86/compiler/blob/main/rep4-1/cal2_64.asm)

### アセンブラ出力
```
$ ./cal2_64 
Result: 2
```

### 先生のコード結果
```
0+(1+(3-2))-(0+(1+(2-3))
 = 0x00000002 = 2
```

### gdbトレース
以下の命令後のstackを見ることにする

初期
```
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000	0x00007fffffffe6b4
0x7fffffffe450:	0x00007fffffffe6c4
```

`push rax     ; stack 0`
```
0x7fffffffe428:	0x0000000000000000	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b4
```


`push rax     ; stack 1`
```
0x7fffffffe420:	0x0000000000000001	0x0000000000000000
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000
```

`push rax     ; stack 3`
```
0x7fffffffe418:	0x0000000000000003	0x0000000000000001
0x7fffffffe428:	0x0000000000000000	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690
```

`pop rbx      ; pop 3`
```
0x7fffffffe420:	0x0000000000000001	0x0000000000000000
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000
```

`pop rax      ; pop 1`
```
0x7fffffffe428:	0x0000000000000000	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b4
```



`pop rbx        ; pop 0`
```
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000	0x00007fffffffe6b4
0x7fffffffe450:	0x00007fffffffe6c4
```

`push rbx     ; stack 0 + (1+(3-2))`
```
0x7fffffffe428:	0x0000000000000002	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b4
```

`push rax     ; stack 0`
```
0x7fffffffe420:	0x0000000000000000	0x0000000000000002
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000
```

`push rax     ; stack 1`
```
0x7fffffffe418:	0x0000000000000001	0x0000000000000000
0x7fffffffe428:	0x0000000000000002	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690
```

`push rax     ; stack 2`
```
0x7fffffffe410:	0x0000000000000002	0x0000000000000001
0x7fffffffe420:	0x0000000000000000	0x0000000000000002
0x7fffffffe430:	0x0000000000000001
```

`pop rbx      ; pop 2`
```
0x7fffffffe418:	0x0000000000000001	0x0000000000000000
0x7fffffffe428:	0x0000000000000002	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690
```

`pop rax      ; pop 1`
```
0x7fffffffe420:	0x0000000000000000	0x0000000000000002
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000
```

`pop rbx        ; pop 0`
```
0x7fffffffe428:	0x0000000000000002	0x0000000000000001
0x7fffffffe438:	0x00007fffffffe690	0x0000000000000000
0x7fffffffe448:	0x00007fffffffe6b4
```

`pop rax      ; pop 0+(1+(3-2))`
```
0x7fffffffe430:	0x0000000000000001	0x00007fffffffe690
0x7fffffffe440:	0x0000000000000000	0x00007fffffffe6b4
0x7fffffffe450:	0x00007fffffffe6c4
```