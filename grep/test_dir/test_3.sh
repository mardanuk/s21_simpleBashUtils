
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
)
  
declare -a tests_extra=(
" -e Ms.  -h ./datasets/data1"
" -e butt -h ./datasets/data2"
" -e e -h ./datasets/data1"
" -e q -h ./datasets/data2"
" -e d -h ./datasets/data1"
" -e qwe -h ./datasets/data2"

" -e Ms.  -h ./datasets/data1 ./datasets/data2"
" -e butt -h ./datasets/data2 ./datasets/data2"
" -e e -h ./datasets/data1 ./datasets/data2"
" -e q -h ./datasets/data2 ./datasets/data2"
" -e d -h ./datasets/data1 ./datasets/data2"
" -e qwe -h ./datasets/data2 ./datasets/data2"

" -e Ms.  -h ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e butt -h ./datasets/data2 ./datasets/data2 qwe"
" -e e -h ./datasets/data1 ./datasets/data2 some_bullshit.txt"
" -e q -h ./datasets/data2 ./datasets/data2 ()"
" -e d -h ./datasets/data1 ./datasets/data2 -0-0-0"
" -e qwe -h ./datasets/data2 ./datasets/data2 >.<"

" -e string -h ./datasets/multipleMathesInARow"
" -e qwe -h ./datasets/qwerty.txt"
" -e stuff -h ./datasets/stuffFile"
" -e q -h ./datasets/data2"
" -e d -h ./datasets/data1"
" -e qwe -h ./datasets/data2"


" -e Ms.  -s ./datasets/data1"
" -e butt -s ./datasets/data2"
" -e e -s ./datasets/data1"
" -e q -s ./datasets/data2"
" -e d -s ./datasets/data1"
" -e qwe -s ./datasets/data2"

" -e Ms.  -s ./datasets/data1 ./datasets/data2"
" -e butt -s ./datasets/data2 ./datasets/data2"
" -e e -s ./datasets/data1 ./datasets/data2"
" -e q -s ./datasets/data2 ./datasets/data2"
" -e d -s ./datasets/data1 ./datasets/data2"
" -e qwe -s ./datasets/data2 ./datasets/data2"

" -e Ms.  -s ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e butt -s ./datasets/data2 ./datasets/data2 qwe"
" -e e -s ./datasets/data1 ./datasets/data2 some_bullshit.txt"
" -e q -s ./datasets/data2 ./datasets/data2 ()"
" -e d -s ./datasets/data1 ./datasets/data2 -0-0-0"
" -e qwe -s ./datasets/data2 ./datasets/data2 >.<"

" -e string -s ./datasets/multipleMathesInARow"
" -e qwe -s ./datasets/qwerty.txt"
" -e stuff -s ./datasets/stuffFile"
" -e q -s ./datasets/data2"
" -e d -s ./datasets/data1"
" -e qwe -s ./datasets/data2"

" -e Ms.  -o ./datasets/data1"
" -e butt -o ./datasets/data2"
" -e e -o ./datasets/data1"
" -e q -o ./datasets/data2"
" -e d -o ./datasets/data1"
" -e qwe -o ./datasets/data2"

" -e q -o ./datasets/data2 ./datasets/data2"
" -e qwe -o ./datasets/data2 ./datasets/data2"

" -e Ms.  -o ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e q -o ./datasets/data2 ./datasets/data2 ()"
" -e d -o ./datasets/data1 ./datasets/data2 -0-0-0"
" -e qwe -o ./datasets/data2 ./datasets/data2 >.<"

" -e string -o ./datasets/multipleMathesInARow"
" -e qwe -o ./datasets/qwerty.txt"
" -e stuff -o ./datasets/stuffFile"
" -e q -o ./datasets/data2"
" -e d -o ./datasets/data1"
" -e qwe -o ./datasets/data2"

" -e Ms.  -f ./test_dir/pattern ./datasets/data1"
" -e butt -f ./test_dir/pattern ./datasets/data2"
" -e e -f ./test_dir/pattern ./datasets/data1"
" -e q -f ./test_dir/pattern ./datasets/data2"
" -e d -f ./test_dir/pattern ./datasets/data1"
" -e qwe -f ./test_dir/pattern ./datasets/data2"

" -e Ms.  -f ./test_dir/pattern ./datasets/data1 ./datasets/data2"
" -e butt -f ./test_dir/pattern ./datasets/data2 ./datasets/data2"
" -e e -f ./test_dir/pattern ./datasets/data1 ./datasets/data2"
" -e q -f ./test_dir/pattern ./datasets/data2 ./datasets/data2"
" -e d -f ./test_dir/pattern ./datasets/data1 ./datasets/data2"
" -e qwe -f ./test_dir/pattern ./datasets/data2 ./datasets/data2"

" -e Ms.  -f ./test_dir/pattern ./datasets/data1 ./datasets/data2 -f nonexistent.file"
" -e butt -f ./test_dir/pattern ./datasets/data2 ./datasets/data2 qwe"
" -e e -f ./test_dir/pattern ./datasets/data1 ./datasets/data2 some_bullshit.txt"
" -e q -f ./test_dir/pattern ./datasets/data2 ./datasets/data2 ()"
" -e d -f ./test_dir/pattern ./datasets/data1 ./datasets/data2 -0-0-0"
" -e qwe -f ./test_dir/pattern ./datasets/data2 ./datasets/data2 >.<"

" -e string -f ./test_dir/pattern ./datasets/multipleMathesInARow"
" -e stuff -f ./test_dir/pattern ./datasets/stuffFile"
" -e q -f ./test_dir/pattern ./datasets/data2"
" -e d -f ./test_dir/pattern ./datasets/data1"
" -e qwe -f ./test_dir/pattern ./datasets/data2"
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

# shellcheck disable=SC2043
for var2 in e
do
    for i in "${tests_extra[@]}"
    do
        var="-$var2"
        testing $i
    done
done
echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"