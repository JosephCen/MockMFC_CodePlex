using CLQCodeGen.Generators;

namespace CLQCodeGen.ProxyAdapters
{
    public class CallUnitTestGeneratorResponse
    {
        public bool IsSuccess { get; set; }

        public string ErrorMessage { get; set; }

        public UnitTestGenerateResponse Response { get; set; }
    }
}
