package dmitry.lab4

import java.util.concurrent.atomic.AtomicInteger

class ConcurrentSet<T>(private val hashtableSize: Int) where T : Comparable<T> {
    init {
        if (hashtableSize < 1) throw IllegalArgumentException("Hashtable size cannot be less than 1")
    }

    private val hashtable = Array(hashtableSize) { ConcurrentList<T>() }
    private var itemCount = AtomicInteger(0)

    private fun getKey(item: T) = item.hashCode() % hashtableSize

    val isEmpty: Boolean
        get() = itemCount.get() == 0

    fun contains(item: T): Boolean {
        val key = getKey(item)
        return hashtable[key].contains(item)
    }

    fun add(item: T): Boolean {
        val key = getKey(item)
        val added = hashtable[key].add(item)

        if (added) itemCount.incrementAndGet()
        return added
    }

    fun remove(item: T): Boolean {
        val key = getKey(item)
        val removed = hashtable[key].remove(item)

        if (removed) itemCount.decrementAndGet()
        return removed
    }
}