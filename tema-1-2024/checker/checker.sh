#!/bin/bash

test_all()
{
    cd ../src
    make clean
    ulimit -n 1024
    cd ..
    echo '<VMCK_NEXT_BEGIN>'
    hw_checker --legacy
    echo '<VMCK_NEXT_END>'
}

test_all
