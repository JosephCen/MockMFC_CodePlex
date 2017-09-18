using System;

namespace CLQCodeGen.Generators
{
    [Serializable]
    public class UnitTestGenerateRequest
    {
        public string ProdAssemblyName { get; set; }

        public string TargetTypeName { get; set; }

        public string UnitTestFileName { get; set; }
    }
}
