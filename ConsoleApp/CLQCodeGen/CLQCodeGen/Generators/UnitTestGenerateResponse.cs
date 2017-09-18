using System;

namespace CLQCodeGen.Generators
{
    [Serializable]
    public class UnitTestGenerateResponse
    {
        public bool IsSuccess { get; set; }

        public string GeneratedText { get; set; }

        public string ExceptionMessage { get; set; }
    }
}
