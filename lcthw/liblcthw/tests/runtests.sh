echo "Running unit tests:"

for i in tests/*_tests
do
    $VALGRIND ./$i 2>&1 >> tests/tests.log
    if [ ! $? ]
    then
        echo "ERROR in test $i: here's tests/tests.log"
        echo "------"
        tail tests/tests.log
        exit 1
    else
        echo $i PASS
    fi
done

echo ""
cat tests/tests.log
