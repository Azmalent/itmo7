#!/usr/bin/bats

function runalone {
    run ./processor "$@"
}

function runwith {
    run ../generator.py 100 uniform -a 10 -b 100 | ./processor "$@"
}

@test "Prints help" {
    runalone -h
    [[ $status == 0 ]]
    [[ $output == *Использование* ]]
}

@test "Errors with negative thread pool size" {
    runalone -t -10
    [[ $status == 1 ]]
    [[ $output == *Ошибка* ]]
}

@test "Errors with invalid flags" {
    runalone -z
    [[ $status == 1 ]]
    [[ $output == *Ошибка* ]]
}