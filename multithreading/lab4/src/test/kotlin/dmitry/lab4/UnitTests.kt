package dmitry.lab4

import io.kotlintest.IsolationMode
import io.kotlintest.shouldBe
import io.kotlintest.specs.WordSpec

class UnitTests : WordSpec() {
    override fun isolationMode() = IsolationMode.InstancePerLeaf

    private val list = ConcurrentList<Int>()

    init {
        "Addition" should {
            "return true on success" {
                list.add(1) shouldBe true
                list.contains(1) shouldBe true
            }

            "return false if element exists" {
                list.add(1)
                list.add(1) shouldBe false
            }

            "work with multiple elements" {
                list.add(1) shouldBe true
                list.add(2) shouldBe true
                list.add(3) shouldBe true

                list.contains(1) shouldBe true
                list.contains(2) shouldBe true
                list.contains(3) shouldBe true
            }
        }

        "Deletion" should {
            "return true on success" {
                list.add(1)
                list.remove(1) shouldBe true

                list.isEmpty shouldBe true
            }

            "return false if element doesn't exist" {
                list.remove(1) shouldBe false
            }
        }
    }
}