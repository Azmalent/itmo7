# Вариант #11
#     Напечатать имя текущего каталога
#     Сменить текущий каталог
#     Напечатать содержимое текущего каталога
#     Создать косвенную (символьную) ссылку на файл
#     Выполнить введенную команду
#     Выйти из программы

#!/bin/bash

function echoerr()
{
    echo "$@" 1>&2
}

function my_cd()
{
    echo "Введите адрес директории для перехода: "
    read new_dir
    if [ -n "$new_dir" ]; then
        cd "$new_dir" 2>>~/lab1_err
        [ $? -ne 0 ] && echoerr "Ошибка: директория не найдена."
    else 
        echoerr "Ошибка: введённая строка пуста."
    fi
}

function my_ln()
{
    echo "Введите адрес исходного файла: "
    read src_file
    if [ -z "$src_file" ]; then echoerr "Ошибка: введённая строка пуста." 
    elif [ ! -e "$src_file" ]; then echoerr "Ошибка: файл не найден." 
    else
        echo "Введите адрес ссылки: "
        read link_file
        if [ -z "$link_file" ]; then 
            echoerr "Ошибка: введённая строка пуста." 
        elif [ -e "$link_file" ]; then 
            echoerr "Ошибка: файл уже существует."
        else 
            ln -s "$src_file" "$link_file" 2>>~/lab1_err
        fi
    fi
}

function my_eval()
{
    echo "Введите команду: "
    read cmd
    if [ -z "$cmd" ]; then echoerr "Ошибка: введённая строка пуста."
    else
        eval "$cmd" 2>>~/lab1_err
        case $? in
            0) echo "Команда выполнена успешно.";;
            127) echoerr "Ошибка: некорректная команда.";;
            *) echoerr "Ошибка при выполнении команды. Проверьте правильность аргументов.";;
        esac
    fi
}

while true; do
    echo -e "\nНажмите нужную клавишу и затем Enter для выбора пункта.\n"

    echo "[1] Напечатать имя текущего каталога"
    echo "[2] Сменить текущий каталог"
    echo "[3] Напечатать содержимое текущего каталога"
    echo "[4] Создать косвенную (символьную) ссылку на файл"
    echo "[5] Выполнить введенную команду"
    echo -e "[6] Выйти из программы\n"

    read key
    echo

    case "$key" in
        1) echo -e "${PWD##*/}";;     # pwd
        2) my_cd;;              # cd
        3) echo | ls -l;;       # ls
        4) my_ln;;              # ln
        5) my_eval;;            # eval
        6) exit 0;;             # exit program
        *) echo "Такого пункта нет! Попробуйте ещё раз.";;
    esac
done