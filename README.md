# ZeBee (zb)
[!logo](assets/zebee-logo.webp)
An interpreter, written in c, for the Zhaw Basic Loop/While "programming" language

## Installation
### Linux
1. Clone the repository
2. Run `make` and `sudo make install` to build the interpreter and install it in `/usr/local/bin`

### Windows
1. Get Linux
2. See ^

(or download the prebuilt [`.exe`](https://github.com/pixlmint/zb-interpreter/actions/runs/9213072043/artifacts/1532236530))

## Using It
See the `examples` directory for a selection of Example programs

Once the interpreter was installed and you're in the root of the repository,
execute `zb fibonacci.zb 4` to execute the `fibonacci.zb` program and passing 
4 as the value for `x1`

## Important
since it's actually a considerably bigger amount to implement ZHAW's strange behavior 
around semicolons, my interpreter just expects them on every **assign** line.
