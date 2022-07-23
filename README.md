<p align="center">
  <a href="https://github.com/moesay/elegant86/">
    <img src="https://github.com/moesay/elegant86/blob/master/resources/elegant86.png" alt="Elegant86 logo" width="200" height="200">
  </a>
</p>

<h3 align="center">Elegant86</h3>

<p align="center">
  A cross-platform 8086 assembler & emulator built to make assembly easier.
  <br>
  <a href="https://github.com/moesay/elegant86/issues/new?template=bug_report.md">Report bug</a>
  ·
  <a href="https://github.com/moesay/elegant86/issues/new?template=feature_request.md">Request feature</a>
</p>

<p align="center">
      <a href="https://github.com/moesay/Elegant86/blob/master/LICENSE" alt="License">
        <img src="https://img.shields.io/github/license/moesay/elegant86" /></a>
      <a href="https://github.com/moesay/Elegant86/" alt="Status">
        <img src="https://img.shields.io/badge/Status-WIP-f10" /></a>
      <a href="https://github.com/moesay/Elegant86/" alt="Dev Status">
        <img src="https://img.shields.io/badge/Developing-Active-green" /></a>
          <a href="https://github.com/moesay/Elegant86/actions/workflows/build.yml" alt="Status">
        <img src="https://github.com/moesay/Elegant86/actions/workflows/build.yml/badge.svg" /></a>
      <a href="https://github.com/moesay/Elegant86/" alt="Repo Size">
        <img src="https://img.shields.io/github/repo-size/moesay/elegant86?label=Repository%20size" /></a>
      <a href="https://github.com/moesay/Elegant86/issues/" alt="Issues">
        <img src="https://img.shields.io/github/issues/moesay/elegant86" /></a>
      <a href="https://github.com/moesay/Elegant86/pulls/" alt="PRs">
        <img src="https://img.shields.io/github/issues-pr/moesay/elegant86" /></a>
 </p>

## Table of contents

- [Why?](#why)
- [Quick start](#quick-start)
  - [Linux](#linux)
  - [Windows](#windows)
- [Implemented instructions list](#implemented-instructions-list)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Bugs and features requests](#bugs-and-features-requests)
- [Copyright and license](#copyright-and-license)


## Why?

The reason behind building Elegant86 is to make Assembly much easier to learn by offering a CPU execution-visualizer that shows you exactly what happened after executing that specific instruction. But you might ask, who needs Assembly in the '20s? Assembly is here to stay. Understanding Assembly will give you a good perspective of how your project really works and how to optimize it.

Elegant86 doesn't invoke another assembler to assemble your code. Elegant86 itself is an assembler. A simple in design, powerful in execution one. So you have the chance to dive into its code and see how your instructions get converted into machine code. The design simplicity made contributing to the project easier but there is a cost. Simply, Elegant86 is not optimal.


## Quick start

#### Linux

To build Elegant86 you need `git`, `qt5`, `g++ >= 10` or `clang++ >= 10`, `cmake`, `make`.


```
$ git clone git@github.com:moesay/Elegant86.git
$ cd Elegant86
$ mkdir build && cd build
$ cmake ..
$ make Elegant86
```

If `g++-10` isn't detected by cmake, use `cmake -DCMAKE_CXX_COMPILER=/path/to/g++-10`.

By default its `-DCMAKE_CXX_COMPILER=/usr/bin/g++-10`


#### Windows

Elegant86 hasn't been guranteed to compile with Windows compilers. This section will be updated after testing.


## Implemented instructions list

| Instruction 	|       Status       	| Instruction 	|       Status       	| Instruction 	|       Status       	| Instruction 	|       Status       	|
|:-----------:	|:------------------:	|:-----------:	|:------------------:	|:-----------:	|:------------------:	|:-----------:	|:------------------:	|
|     add     	| :heavy_check_mark: 	|     push    	| :heavy_check_mark: 	|     pop     	| :heavy_check_mark: 	|     mov     	| :heavy_check_mark: 	|
|      in     	|         :x:        	|     out     	|         :x:        	|     adc     	|         :x:        	|     inc     	|         :x:        	|
|     sub     	|         :x:        	|     sbb     	|         :x:        	|     dec     	|         :x:        	|     neg     	|         :x:        	|
|     cmp     	|         :x:        	|     aas     	| :heavy_check_mark: 	|     das     	| :heavy_check_mark: 	|     mul     	|         :x:        	|
|     imul    	|         :x:        	|     aam     	| :heavy_check_mark: 	|     div     	|         :x:        	|     idiv    	|         :x:        	|
|     aad     	| :heavy_check_mark: 	|     cbw     	| :heavy_check_mark: 	|     cwd     	| :heavy_check_mark: 	|     not     	|         :x:        	|
|     shl     	|         :x:        	|     sal     	|         :x:        	|     shr     	|         :x:        	|     sar     	|         :x:        	|
|     rol     	|         :x:        	|     ror     	|         :x:        	|     rcl     	|         :x:        	|     rcr     	|         :x:        	|
|     and     	| :heavy_check_mark: 	|     test    	|         :x:        	|      or     	|         :x:        	|     xchg    	|         :x:        	|
|     xor     	|         :x:        	|     rep     	|         :x:        	|     movs    	|         :x:        	|     cmps    	|         :x:        	|
|     scas    	|         :x:        	|     lods    	|         :x:        	|     sti     	| :heavy_check_mark: 	|     call    	|         :x:        	|
|     jmp     	|         :x:        	|     ret     	|         :x:        	|      je     	|         :x:        	|     cld     	| :heavy_check_mark: 	|
|      jl     	|         :x:        	|     into    	| :heavy_check_mark: 	|     jle     	|         :x:        	|     jng     	|         :x:        	|
|     aaa     	| :heavy_check_mark: 	|     jnae    	|         :x:        	|     jbe     	|         :x:        	|     jna     	|         :x:        	|
|      jp     	|         :x:        	|     jpe     	|         :x:        	|     wait    	| :heavy_check_mark: 	|      js     	|         :x:        	|
|     jne     	|         :x:        	|     jnz     	|         :x:        	|     jnl     	|         :x:        	|     jge     	|         :x:        	|
|     jnle    	|         :x:        	|      jg     	|         :x:        	|     jnb     	|         :x:        	|     jae     	|         :x:        	|
|     nop     	| :heavy_check_mark: 	|      ja     	|         :x:        	|     jnp     	|         :x:        	|     jpo     	|         :x:        	|
|     jno     	|         :x:        	|     ret     	|         :x:        	|     jns     	|         :x:        	|     loop    	|         :x:        	|
|    loopz    	|         :x:        	|    loope    	|         :x:        	|    loopnz   	|         :x:        	|    loopne   	|         :x:        	|
|     jcxz    	|         :x:        	|     int     	|         :x:        	|     jnge    	|         :x:        	|     iret    	| :heavy_check_mark: 	|
|     clc     	| :heavy_check_mark: 	|     cmc     	| :heavy_check_mark: 	|     stc     	| :heavy_check_mark: 	|      jz     	|         :x:        	|
|     std     	| :heavy_check_mark: 	|     cli     	| :heavy_check_mark: 	|     stds    	|         :x:        	|     hlt     	| :heavy_check_mark: 	|
|      jo     	|         :x:        	|     esc     	|         :x:        	|     lock    	| :heavy_check_mark: 	|      jb     	|         :x:        	|
|     daa     	| :heavy_check_mark: 	|     lahf    	| :heavy_check_mark: 	|     jnbe    	|         :x:        	|     popf    	| :heavy_check_mark: 	|
|     sahf    	| :heavy_check_mark: 	|             	|                    	|             	|                    	|             	|                    	|

## Documentation

Elegant86 hasn't been documented yet. This section will be updated once the documentation is ready.


## Contributing

There are no contributing guidelines for Elegant86 (so far). Feel free to fork the repo and help releasing Elegant86 faster.

**Note:** Instructions implementations won't be pulled to the repository without a successful unit test. Elegant86 uses `gtest` for testing. Consider writing the proper unit test for your implementation.


## Bugs and features requests

Have a bug or a feature request? Please first [check the issues](https://github.com/moesay/elegant86/issues) and search for existing and closed one. If your problem or idea is not addressed yet, [please open a new issue](https://github.com/moesay/elegant86/issues/new).


## Copyright and license

Code copyright 2019–2021 the [Elegant86 Author](https://github.com/moesay). Code released under the [GNU General Public License](https://github.com/moesay/elegant86/blob/master/LICENSE).


Elegant86 icon copyright :copyright: [Freepik Company S.L.](https://www.freepik.com/)
