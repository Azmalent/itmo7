#!/usr/bin/bats

function runalone {
    run ./processor "$@"
}

function runwith {
    ../generator.py 100 uniform -a -100 -b 100 | ./processor "$@"
    status=$?
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

@test "Errors with negative metrics interval" {
    runalone -n -10
    [[ $status == 1 ]]
    [[ $output == *Ошибка* ]]
}

@test "Errors with invalid flags" {
    runalone -z
    [[ $status == 1 ]]
    [[ $output == *Ошибка* ]]
}

@test "PER_TASK works" {
    runwith -s per_task 
    [[ $status == 0 ]]
}

@test "PER_TYPE works" {
    runwith -s per_type
    [[ $status == 0 ]]
}

@test "THREAD_POOL works" {
    runwith -s thread_pool -t 10 
    [[ $status == 0 ]]
}

@test "Metrics" {
    runwith -n 100
    [[ $status == 0 ]]
}