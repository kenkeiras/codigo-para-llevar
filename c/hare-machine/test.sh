#!/bin/sh

assert_failed() {
    if [ $? -eq 0 ];then
        echo 'Error: Expected failure'
        exit 1
    fi
}

set -eu

# Should detect halting properly
echo 'Should HALT'

for ex in examples/halt/*;do
    echo -n "  ./check-halt \"$ex\"="
    ./check-halt "$ex" 2>>/dev/null
done

set +e
echo
echo 'Should LOOP (not halt)'
for ex in examples/loop/*;do
    echo -n "  ./check-halt \"$ex\"="
    ./check-halt "$ex"  2>>/dev/null; assert_failed
done
