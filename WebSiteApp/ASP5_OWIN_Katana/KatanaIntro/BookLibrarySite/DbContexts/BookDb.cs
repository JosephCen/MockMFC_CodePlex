using Books.Entities;
using System.Data.Entity;

namespace BookLibrarySite.DbContexts
{
    public class BookDb : DbContext
    {
        public BookDb()
            : base("DefaultConnection")
        {
        }

        public DbSet<Book> Books { get; set; }
    }
}