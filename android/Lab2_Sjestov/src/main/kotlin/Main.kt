import tasks.*

fun main(args: Array<String>) {
    println("Введите номер задания: ")
    val n = readLine()?.toIntOrNull() ?: 1

    val tasks = arrayOf(
        Task1, Task2, Task3, Task4, Task5, Task6, Task7, Task8, Task9, Task10
    )

    if (n in 1..10) tasks[n - 1].run()
    else println("Такого задания нет!")
}