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

## Table of contents

- [Why?](#why)
- [Quick start](#quick-start)
  - [Linux](#linux)
  - [Windows](#windows)
- [Project Structure](#project-structure)
- [Bugs and feature requests](#bugs-and-feature-requests)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Copyright and license](#copyright-and-license)


## Why?

The reason behind building Elegant86 is to make Assembly much easier to learn by offering a CPU execution-visualizer that shows you exactly what happened after executing that specific instruction. But you might ask, who needs Assembly in the '20s? Assembly is here to stay. Understanding Assembly will give you a good perspective of how your project really works and how to optimize it.

Elegant86 doesn't invoke another assembler to assemble your code. Elegant86 itself is an assembler. A simple in design, powerful in execution one. So you have the chance to dive into its code and see how your instructions get converted into machine code. The design simplicity made contributing to the project easier but there is a cost. Simply, Elegant86 is not optimal.


## Quick start

#### Linux

To build Elegant86 you need `git`, `qt5`, `g++ >= 10` or `clang++ >= 10`, `cmake`, `make`.
1. `git clone https://github.com/moesay/elegant86`
2. `cd elegant86`
3. `mkdir build && cd build`
4. `cmake ..`

If `g++-10` isn't detected by cmake, use `cmake -DCMAKE_CXX_COMPILER=/path/to/g++-10`. By default its `-DCMAKE_CXX_COMPILER=/usr/bin/g++-10` 

5. `make Elegant86`

#### Windows

Elegant86 hasn't been guranteed to compile with Windows compilers. This section will be updated after testing.


## Project Structure

```text
elegant86/
├── assembler/
└───── fpasm/
│       ├── fpasm.cpp
│       └── fpasm.h
└───── include/
│       ├── add.h
│       ├── base.h
│       ├── mov.h
│       ├── precom.h
│       └── push.h
└───── source/
│       ├── add.cpp
│       ├── base.cpp
│       ├── mov.cpp
│       └── push.cpp
├── codeEditor/
└───── include/
│       ├── codeEditor.h
│       ├── highlighter.h
│       └── linenumber.h
└───── source/
│       ├── codeEditor.cpp
│       ├── highlighter.cpp
│       └── linenumber.cpp
├── include/
│  ├── console.h
│  ├── Editor.h
│  ├── exc.h
│  ├── labels.h
│  ├── ports.h
│  ├── Simulate.h
│  ├── stack.h
│  ├── utils.h
│  └── variabels.h
├── source/
│  ├── console.cpp
│  ├── Editor.cpp
│  ├── exc.cpp
│  ├── labels.cpp
│  ├── ports.cpp
│  ├── Simulate.cpp
│  ├── stack.cpp
│  ├── utils.cpp
│  └── variabels.cpp
└──
```


## Bugs and feature requests

Have a bug or a feature request? Please first [check the issues](https://github.com/moesay/elegant86/issues) and search for existing and closed one. If your problem or idea is not addressed yet, [please open a new issue](https://github.com/moesay/elegant86/issues/new).


## Documentation

Elegant86 hasn't been documented yet. This section will be updated once the documentation is ready.


## Contributing

There are no contributing guidelines for Elegant86 (so far). Feel free to fork the repo and help releasing Elegant86 faster.

**Note:** Instructions implementations won't be pulled to the repository without a successful unit test. Elegant86 uses `gtest` for testing. Consider writing the proper unit test for your implementation.


## Copyright and license

Code copyright 2019–2021 the [Elegant86 Author](https://github.com/moesay). Code released under the [GNU General Public License](https://github.com/moesay/elegant86/blob/master/LICENSE).


Elegant86 icon copyright 2010–2021 [Freepik Company S.L.](https://www.freepik.com/)
