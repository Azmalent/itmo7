using System;
using System.Linq;
using System.Collections.Generic;
using System.Collections;

using static System.Threading.Interlocked;

namespace lab4.Collections
{
    public sealed class ConcurrentSet<T> : IEnumerable<T>
    {
        private readonly ConcurrentDistinctList<T>[] table;
        private volatile int itemCount = 0;

        public ConcurrentSet(int hashtableCapacity = 10)
        {
            if (hashtableCapacity < 1) throw new ArgumentOutOfRangeException("Capacity must be positive");
            
            table = new ConcurrentDistinctList<T>[hashtableCapacity];
            for (int i = 0; i < table.Length; i++) table[i] = new ConcurrentDistinctList<T>();
        }
        
        private int GetKey(T item)
        {
            return item.GetHashCode() % table.Length;
        }

        public bool Add(T item)
        {
            int key = GetKey(item);
            var list = table[key];

            bool added = list.Add(item);
            if (added) Increment(ref itemCount);
            return added;
        }

        public bool Remove(T item)
        {
            int key = GetKey(item);
            var list = table[key];

            bool removed = list.Remove(item);
            if (removed) Decrement(ref itemCount);
            return removed;
        }

        public bool Contains(T item)
        {
            int key = GetKey(item);
            return table[key].Contains(item);
        }

        public bool IsEmpty => itemCount == 0;

        /* -------------------------- */
        /* --Реализация-IEnumerable-- */
        /* -------------------------- */
        public IEnumerator<T> GetEnumerator()
        {
            for (int i = 0; i < table.Length; i++)
            {
                foreach(T item in table[i])
                {
                    yield return item;
                }
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return this.GetEnumerator();
        }
    }
}