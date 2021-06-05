#!/bin/sh

echo "Tests in C:"
root_dir=$HOME/code/9cc/c
docker run --rm -v $root_dir:/9cc/c -w /9cc/c compilerbook /bin/bash -c "make; make test"

echo "Tests in Rust:"
root_dir=$HOME/code/9cc/rust
docker run --rm -v $root_dir:/9cc/rust -w /9cc/rust compilerbook /bin/bash -c "/home/user/.cargo/bin/cargo build --release; tests/test.sh" 
