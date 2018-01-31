using System.IO;
using System.Text.RegularExpressions;
using CommandLine;

namespace CLQCodeGen.Console.GenArguments
{
    [Verb("StaticModel", HelpText = "Generate the code of a static model (such as 'struct TurnaroundTimeUnit'). A *.json config is required for the static model.")]
    internal class StaticModelArgument : IArgument
    {
        [Option('s', "StaticModel", Required = true, HelpText = "The name of source file name for static model type.")]
        public string StaticModel { get; set; }

        public string StaticModelFile { get; set; }

        private void FormatValues()
        {
            if (Regex.IsMatch(StaticModel, @"\w+\.cs"))
            {
                StaticModelFile = Path.GetFullPath(StaticModel);
            }
        }

        public bool Validate()
        {
            FormatValues();

            var isValidate =
                       !string.IsNullOrEmpty(StaticModel)
                    && !string.IsNullOrEmpty(StaticModelFile);

            return isValidate;
        }
    }
}
