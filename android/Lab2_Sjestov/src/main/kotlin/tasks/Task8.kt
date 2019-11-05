package tasks

/**
 Задание 8:
 Создайте два класса самолетов AirBusA320 и AirBusA380. Добавьте в класс самолета AirBusA320
 методы, отвечающие за скорость и длину разбега самолета, которые выводят на экран
 соответствующие надписи о действии экземпляра. Унаследуйте AirBusA380 от самолета AirbusA320,
 добавив ему метод отвечающий за вместимость пассажиров.
 */
object Task8 : ITask {
    open class AirBusA320 {
        fun printSpeed() {
            println("Скорость: 840 км/ч")
        }

        fun printTakeoffRun() {
            println("Длина разбега: 2090 м")
        }
    }

    class AirBus380 : AirBusA320() {
        fun printPassengerCapacity() {
            println("Вместимость: 525 пассажиров в салоне трёх классов, 853 пассажира в одноклассовой конфигурации")
        }
    }

    override fun run() {
        val plane = AirBus380()
        plane.printSpeed()
        plane.printTakeoffRun()
        plane.printPassengerCapacity()
    }
}