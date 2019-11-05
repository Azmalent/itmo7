package tasks

/**
 Задание 9:
 Создайте 9 классов: студент, преподаватель, профессор, ассистент, магистрант, аспирант, декан,
 ректор, руководитель.
 1) Унаследуйте магистранта и аспиранта от класса Студент.
 2) Унаследуйте профессора и ассистента от Преподавателя.
 3) Унаследуйте декана и ректора от Руководителя.
 */
object Task9 : ITask {
    open class Student
    class Master : Student()
    class Candidate : Student()

    open class Teacher
    class Professor : Teacher()
    class Assistant : Teacher()

    open class Director
    class Dean : Director()
    class Rector : Director()

    override fun run() {
        println("Hello, world!")
    }
}