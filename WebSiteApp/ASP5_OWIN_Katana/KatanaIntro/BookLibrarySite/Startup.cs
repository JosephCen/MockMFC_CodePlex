using Microsoft.Owin;
using Owin;

[assembly: OwinStartupAttribute(typeof(BookLibrarySite.Startup))]
namespace BookLibrarySite
{
    public partial class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            ConfigureAuth(app);
        }
    }
}
