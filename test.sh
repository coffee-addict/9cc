#!/bin/sh

root_dir=$HOME/Documents/code/9cc

# C compiler by C
docker run --rm -v $root_dir/c:/9cc/c -w /9cc/c compilerbook /bin/bash -c "make; make test"

# # C compiler by Rust
# docker run --rm -v $root_dir/rust:/9cc/rust -w /9cc/rust compilerbook /bin/bash -c "~/.cargo/bin/cargo run rust"
