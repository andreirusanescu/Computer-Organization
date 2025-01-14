#!/bin/bash

test_all()
{

    echo '<VMCK_NEXT_BEGIN>'
    python3 checker.py --all
    echo '<VMCK_NEXT_END>'
}

test_all
