using System.Text.RegularExpressions;
using CommandLine;

namespace CLQCodeGen.Console.GenArguments
{
    [Verb("UnitTest", HelpText = "Generate the schema code of unit test for given target type.")]
    internal class UnitTestArgument : IArgument
    {
        [Option('p', "ProdAssemblyName", Default = "_default", HelpText = "The name of relative product assembly.")]
        public string ProdAssemblyName { get; set; }

        [Option('t', "TargetTypeName", Required = true, HelpText = "The name of unit test type or unit test source file name.")]
        public string TargetTypeName { get; set; }

        public bool Validate()
        {
            FormatValues();

            var isValidate =
                       !string.IsNullOrEmpty(ProdAssemblyName)
                    && !string.IsNullOrEmpty(TargetTypeName);

            return isValidate;
        }

        private void FormatValues()
        {
            var testSrcFileMatch = Regex.Match(TargetTypeName, @"(\w+)Test(?:.cs)?");

            TargetTypeName = testSrcFileMatch.Success ? testSrcFileMatch.Groups[1].Value : string.Empty;
        }
    }
}
