#!/bin/sh

echo "Tests in C:"
root_dir=$HOME/code/9cc/c
docker run --rm -v $root_dir:/9cc/c -w /9cc/c compilerbook /bin/bash -c "make; make test"
