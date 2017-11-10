using System;
using System.Collections.Generic;
using System.Threading.Tasks;

namespace KatanaIntro.Components
{
    using Owin;
    using AppFunc = Func<IDictionary<string, object>, Task>;

    internal class PassbyWebComponent
    {
        private AppFunc _next;

        public PassbyWebComponent(AppFunc next)
        {
            _next = next;
        }

        public async Task Invoke(IDictionary<string, object> environment)
        {
            await _next(environment);
        }
    }

    internal static class _PassbyWebComponent
    {
        public static void UsePassbyWebComponent(this IAppBuilder app)
        {
            app.Use<PassbyWebComponent>();
        }
    }
}
