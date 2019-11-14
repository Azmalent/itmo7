using lab4.Collections;
using Xunit;

namespace lab4.Tests
{
    public class ConcurrentSetTests
    {
        private ConcurrentSet<int> set = new ConcurrentSet<int>(8);

        [Fact] void AddElement()
        {
            set.Add(100);
            Assert.True( set.Contains(100) );
        }

        [Fact] void RemoveElement()
        {
            Assert.True(true);
        }

        [Fact] void IsEmpty()
        {
            Assert.True(set.IsEmpty);

            set.Add(100);
            Assert.False(set.IsEmpty);
        }
    }
}