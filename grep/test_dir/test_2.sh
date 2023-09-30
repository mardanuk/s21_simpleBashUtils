SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
" -e 'Ms. ' ./datasets/data1"
" -e 'butt' ./datasets/data2"
" -e 'e' ./datasets/data1"
" -e 'q' ./datasets/data2"
" -e 'd' ./datasets/data1"
" -e 'qwe' ./datasets/data2"

" -e 'Ms. ' ./datasets/data1 ./datasets/data2"
" -e 'butt' ./datasets/data2 ./datasets/data2"
" -e 'e' ./datasets/data1 ./datasets/data2"
" -e 'q' ./datasets/data2 ./datasets/data2"
" -e 'd' ./datasets/data1 ./datasets/data2"
" -e 'qwe' ./datasets/data2 ./datasets/data2"

" -e 'Ms. ' ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e 'butt' ./datasets/data2 ./datasets/data2 qwe"
" -e 'e' ./datasets/data1 ./datasets/data2 some_bullshit.txt"
" -e 'q' ./datasets/data2 ./datasets/data2 ()"
" -e 'd' ./datasets/data1 ./datasets/data2 -0-0-0"
" -e 'qwe' ./datasets/data2 ./datasets/data2 >.<"

" -e 'string' ./datasets/multipleMathesInARow"
" -e 'qwe' ./datasets/qwerty.txt"
" -e 'stuff' ./datasets/stuffFile"
" -e 'q' ./datasets/data2"
" -e 'd' ./datasets/data1"
" -e 'qwe' ./datasets/data2"

" -e 'Ms. ' ./datasets/data1"
" -e 'butt' ./datasets/data2"
" -e 'e' ./datasets/data1"
" -e 'q' ./datasets/data2"
" -e 'd' ./datasets/data1"
" -e 'qwe' ./datasets/data2"

" -e 'Ms. ' ./datasets/data1 ./datasets/data2"
" -e 'butt' ./datasets/data2 ./datasets/data2"
" -e 'e' ./datasets/data1 ./datasets/data2"
" -e 'q' ./datasets/data2 ./datasets/data2"
" -e 'd' ./datasets/data1 ./datasets/data2"
" -e 'qwe' ./datasets/data2 ./datasets/data2"

" -e 'Ms. ' ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e 'butt' ./datasets/data2 ./datasets/data2 qwe"
" -e 'e' ./datasets/data1 ./datasets/data2 some_bullshit.txt"
" -e 'q' ./datasets/data2 ./datasets/data2 ()"
" -e 'd' ./datasets/data1 ./datasets/data2 -0-0-0"
" -e 'qwe' ./datasets/data2 ./datasets/data2 >.<"

" -e 'string' ./datasets/multipleMathesInARow"
" -e 'qwe' ./datasets/qwerty.txt"
" -e 'stuff' ./datasets/stuffFile"
" -e 'q' ./datasets/data2"
" -e 'd' ./datasets/data1"
" -e 'qwe' ./datasets/data2"

)
testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}


for var1 in e i v c l n
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done
echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"