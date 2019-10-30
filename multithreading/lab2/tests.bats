#!/usr/bin/env bats

function reset {
  chmod -R =777 "$@"
}

function original_chmod {
  run chmod "$@"
  original_status=$status
  original_output=$output
  reset "${@: -1}"
}

function my_chmod {
  run ./chmod "$@"
  output=$(sed 's/.\/chmod/chmod/g' <<< $output)
  reset "${@: -1}"
}

function runwith {
  original_chmod "$@"
  my_chmod "$@"
}

reset simple.file
reset test1
reset test2
reset test3

@test "444:        returns r--r--r--" {
  runwith -v 444 simple.file 
  
  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "-333:       returns r--r--r--" {
  runwith -v -333 simple.file 
  
  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "=w:         returns -w--w----" {
  runwith -v =w simple.file 
  
  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "+st:        returns rwsrwsrwt" {
  runwith -v +st simple.file 

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "-x,+st:     returns rwSrwSrwT" {
  runwith -v -x,+st simple.file 

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "ug+t,o+s:   does nothing" {
  runwith -v ug+t,o+s simple.file 

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "=700,g=u:   returns rx-rx----" {
  runwith -v =700,g=u simple.file 

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "0:          doesn't remove SUID/SGID on directories" {
  chmod +s test2
  original_chmod -v 0 test2

  chmod +s test2
  my_chmod -v 0 test2

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "=0:         removes SUID/SGID on directories" {
  chmod +s test2
  original_chmod -v =0 test2

  chmod +s test2
  my_chmod -v =0 test2

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "+X:         sets +x on directory" {
  chmod 0 test2
  original_chmod -v +X test2

  chmod 0 test2
  my_chmod -v +X test2

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "+X:         sets +x on normal file if anyone can execute it" {
  chmod 1 simple.file
  original_chmod -v +X simple.file

  chmod 1 simple.file
  my_chmod -v +X simple.file

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "+X:         does nothing on normal file if nobody can execute it" {
  chmod 0 simple.file
  original_chmod -v +X simple.file

  chmod 0 simple.file
  my_chmod -v +X simple.file

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "works on symlinks" {
  runwith -v 777 test1/link1

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}

@test "ignore symlinks when recursing" {
  runwith -Rv 555 test3

  [ "$status" -eq "$original_status" ]
  [ "$output" = "$original_output" ]
}