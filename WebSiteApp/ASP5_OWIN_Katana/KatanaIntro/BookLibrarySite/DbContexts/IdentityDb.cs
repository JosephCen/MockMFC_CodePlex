using BookLibrarySite.Models;
using Microsoft.AspNet.Identity.EntityFramework;

namespace BookLibrarySite.DbContexts
{
    public class IdentityDb : IdentityDbContext<ApplicationUser>
    {
        public IdentityDb()
            : base("DefaultConnection")
        {
        }

        public static IdentityDb Create()
        {
            return new IdentityDb();
        }
    }
}