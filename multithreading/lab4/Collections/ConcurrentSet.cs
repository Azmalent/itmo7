using System.Collections.Generic;

namespace lab4.Collections
{
    public sealed class ConcurrentSet<T>
    {
        private HashTable<T> container;
        private object lockObject;

        public ConcurrentSet(int hashTableSize)
        {
            container = new HashTable<T>(hashTableSize);
            lockObject = new object();
        }

        public bool Add(T item)
        {
            lock(lockObject)
            {
                return container.Add(item);
            }
        }
        public bool Remove(T item)
        {
            lock(lockObject)
            {
                return container.Remove(item);
            }
        }

        public bool Contains(T item)
        {
            lock(lockObject)
            {
                return container.Contains(item);
            }
        }

        public bool IsEmpty
        {
            get {
                lock(lockObject)
                {
                    return container.IsEmpty;
                }
            }
        }

        /* -------------------------- */
        /* --Реализация-IEnumerable-- */
        /* -------------------------- */
        IEnumerator<T> GetEnumerator()
        {
            return container.GetEnumerator();
        }
    }
}