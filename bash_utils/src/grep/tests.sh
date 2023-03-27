#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR 'int int' grep_tests/t1"
"VAR 'int' grep_tests/t1 grep_tests/t2 grep_tests/t3"
"VAR opt grep_tests/t2"
"VAR _ grep_tests/t3"
"VAR INCLUDE grep_tests/t2"
"VAR asasa grep_tests/t4"
"VAR _ grep_tests/t4 grep_tests/t2"
"VAR _ grep_tests/no_file"
"VAR 'int' grep_tests/t1 grep_tests/t2 grep_tests/t3 -kn"
"VAR 'int' grep_tests/t1 grep_tests/t2 grep_tests/t3 -h -e B -e"
"VAR 'int' grep_tests/t1 grep_tests/t2 grep_tests/t3 -v -e B -n"
)

declare -a extra=(
"-e opt -e low grep_tests/t1"
"-f grep_tests/patt -vi grep_tests/t3 grep_tests/t1 -h"
"-f grep_tests/patt -vi grep_tests/t3 grep_tests/t1 -h -e N"
"-u opt -e low grep_tests/t1"
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
echo "§§§ EXTRA §§§"
# специфические тесты
for i in "${extra[@]}"
do
    # var="-"
    testing $i
done
echo "§§§ 1 PARAMETR §§§"
# 1 параметр
for var1 in e i v c l n s
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done
echo "§§§ 2 PARAMETRS §§§"
# 2 параметра
for var1 in e i v c l n s
do
    for var2 in e i v c l n s
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1 -$var2"
                testing $i
            done
        fi
    done
done

# # 3 параметра
# for var1 in e i v c l n s
# do
#     for var2 in e i v c l n s
#     do
#         for var3 in e i v c l n s
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1 -$var2 -$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

# 2 сдвоенных параметра
for var1 in e i v c l n s
do
    for var2 in e i v c l n s
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

# # 3 строенных параметра
# for var1 in e i v c l n s
# do
#     for var2 in e i v c l n s
#     do
#         for var3 in e i v c l n s
#         do
#             if [ $var1 != $var2 ] && [ $var2 != $var3 ] && [ $var1 != $var3 ]
#             then
#                 for i in "${tests[@]}"
#                 do
#                     var="-$var1$var2$var3"
#                     testing $i
#                 done
#             fi
#         done
#     done
# done

echo "\033[31mFAIL: $FAIL\033[0m"
echo "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
