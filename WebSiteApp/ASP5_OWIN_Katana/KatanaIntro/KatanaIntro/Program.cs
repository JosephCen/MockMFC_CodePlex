using Microsoft.Owin.Hosting;
using Owin;
using System;

namespace KatanaIntro
{
    class Program
    {
        static void Main(string[] args)
        {
            var uri = "http://localhost:8081";

            using (WebApp.Start<Startup>(uri))
            {
                Console.WriteLine("Start!");
                Console.ReadLine();
                Console.WriteLine("Stopping...");
            }
        }
    }

    public class Startup
    {
        public void Configuration(IAppBuilder app)
        {
            app.Run(ctx =>
            {
                return ctx.Response.WriteAsync("Hello World!");
            });
        }
    }
}
