package dmitry.lab4

import io.kotlintest.IsolationMode
import io.kotlintest.shouldBe
import io.kotlintest.shouldThrow
import io.kotlintest.specs.WordSpec
import java.lang.IllegalArgumentException

class IntegrationTests : WordSpec() {
    override fun isolationMode() = IsolationMode.InstancePerLeaf

    private val set = ConcurrentSet<Int>(25)

    private fun runThreads(n: Int, initializer: (Int) -> (() -> Unit)) {
        val threads = Array(n) { i ->
            val function = initializer(i)
            Thread(function)
        }

        for (i in 0 until n) {
            threads[i].start()
        }

        for (i in 0 until n) {
            threads[i].join()
        }
    }

    init {
        "Addition" should {
            "work with 1000 parallel threads" {
                runThreads(1000) {
                    i -> { set.add(i) }
                }

                for (i in 0 until 1000) {
                    set.contains(i) shouldBe true
                }
            }
        }

        "Deletion" should {
            for (i in 0 until 1000) {
                set.add(i)
            }

            "work with 1000 parallel threads" {
                runThreads(1000) {
                    i -> { set.remove(i) }
                }

                set.isEmpty shouldBe true
            }

            "work in parallel with addition" {
                fun adder(n: Int): () -> Unit = { set.add(n) }
                fun remover(n: Int): () -> Unit = { set.remove(n) }

                runThreads(2000) { i ->
                    if (i%2 == 0) adder(i/2 + 1000)
                    else remover(i/2)
                }

                for (i in 0 until 1000) {
                    set.contains(i) shouldBe false
                }

                for (i in 1000 until 2000) {
                    set.contains(i) shouldBe true
                }
            }
        }
    }
}