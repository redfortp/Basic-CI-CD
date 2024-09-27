s21_command=(
    "./s21_grep"
    )
sys_command=(
    "grep"
    )

flags=(
    "i"
    "v"
    "c"
    "l"
    "n"
    "h"
    "s"
)

test_files=(
    "test_files/test_file_1.txt"
    "test_files/test_file_2.txt"
    "test_files/test_file_3.txt"
    "doesntexit.txt"
)
test_f_files=(
    "test_files/test_f_flag_file_1.txt"
    "test_files/test_f_flag_file_2.txt"
    "doesntexit.txt"
)

SUCCESS_COUNTER=0
FAIL_COUNTER=0
for opt in "${flags[@]}"
do
    for opt2 in "${flags[@]}"
    do
        for f_flag_file in "${test_f_files[@]}"
        do
#                for pattern in "${patterns[@]}"
#                do
                for test_file in "${test_files[@]}"
                do
                for test_file in "${test_files[@]}"
                    do
                    "${s21_command[0]}" -"${opt}" -"${opt2}" -f "${f_flag_file}" ${test_file} > s21_grep_test.log 2>/dev/null
                    "${sys_command[0]}" -"${opt}" -"${opt2}" -f "${f_flag_file}" ${test_file} > grep_test.log 2>/dev/null
                    DIFF=$(diff -s grep_test.log s21_grep_test.log)
                    if [ "$DIFF" == "Files grep_test.log and s21_grep_test.log are identical" ]
                    then
                        let SUCCESS_COUNTER++
                        echo "./s21_grep -$opt -$opt2 -f $f_flag_file $test_file"
                        echo "SUCCESS | $SUCCESS_COUNTER:$FAIL_COUNTER"
                    else
                        let FAIL_COUNTER++
                        echo "./s21_grep -$opt -$opt2 -f $f_flag_file $test_file"
                        echo "FAIL | $SUCCESS_COUNTER:$FAIL_COUNTER"
                    fi
                    rm *.log
                    done
                done
#            done
        done
    done
done
echo " "
echo "SUCCESS : $SUCCESS_COUNTER"
echo "FAIL : $FAIL_COUNTER"
