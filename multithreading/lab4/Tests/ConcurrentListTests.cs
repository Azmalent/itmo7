using System;
using lab4.Collections;
using Xunit;

namespace lab4.Tests
{
    public class ConcurrentListTests
    {
        private ConcurrentDistinctList<string> list = new ConcurrentDistinctList<string>();

        [Fact]
        void NullReferences()
        {            
            Assert.Throws<NullReferenceException>(
                () => list.Add(null)
            );

            Assert.Throws<NullReferenceException>(
                () => list.Remove(null)
            );

            Assert.Throws<NullReferenceException>(
                () => list.Contains(null)
            );
        }

        [Fact]
        void AddExistingElement()
        {
            list.Add("foo");
            Assert.False(list.Add("foo"));
        }

        [Fact]
        void AddElement()
        {
            Assert.True(list.Add("foo"));
            Assert.True(list.Add("bar"));
            Assert.True(list.Add("baz"));

            Assert.True(list.Contains("foo"));
            Assert.True(list.Contains("bar"));
            Assert.True(list.Contains("baz"));

            Assert.False(list.IsEmpty);
        }

        [Fact]
        void RemoveFromEmptyList()
        {
            Assert.False(list.Remove("foo"));
        }

        [Fact]
        void RemoveExistingElement()
        {
            list.Add("foo");
            list.Add("bar");
            list.Add("baz");

            Assert.True(list.Remove("bar"));
            Assert.True(list.Remove("foo"));
            Assert.True(list.Remove("baz"));

            Assert.True(list.IsEmpty);
        }

        [Fact]
        void RemoveNonExistentElement()
        {
            list.Add("foo");
            list.Add("bar");
            list.Add("baz");

            Assert.False(list.Remove("hello"));
        }
    }
}