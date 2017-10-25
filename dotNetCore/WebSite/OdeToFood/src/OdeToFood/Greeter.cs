using Microsoft.Extensions.Configuration;

namespace OdeToFood
{
    public interface IGreeter
    {
        string GetGreetMessage();
    }

    internal class Greeter : IGreeter
    {
        private const string GreetKey = "Greeting";

        private IConfiguration _configuration;

        public Greeter(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        public string GetGreetMessage()
        {
            return _configuration[GreetKey];
        }
    }
}
