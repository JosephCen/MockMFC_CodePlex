using System;
using System.IO;
using System.Text.RegularExpressions;
using CommandLine;

namespace CLQCodeGen.Console.GenArguments
{
    [Verb("StaticModel", HelpText = "Generate the code of a static model (such as 'struct TurnaroundTimeUnit'). A *.json config is required for the static model. "
                                  + "The json config must be same name with the source file name but different file extension.")]
    internal class StaticModelArgument : IArgument
    {
        private const string SampleConfigJSON = "SampleConfigJSON";

        public StaticModelArgument()
        {
            GenSampleConfigJSON = false;
            StaticModelFile = string.Empty;
        }

        [Option('s', "StaticModel", Required = true, HelpText = "The name of source file name for static model type. Use '" + SampleConfigJSON + "' to get a sample config JSON.")]
        public string StaticModel { get; set; }

        public bool GenSampleConfigJSON { get; set; }

        public string StaticModelFile { get; set; }

        private void FormatValues()
        {
            if (!string.IsNullOrEmpty(StaticModel))
            {
                if (Regex.IsMatch(StaticModel, @"\w+\.cs"))
                {
                    StaticModelFile = Path.GetFullPath(StaticModel);
                }
                else if (SampleConfigJSON.Equals(StaticModel, StringComparison.Ordinal))
                {
                    GenSampleConfigJSON = true;
                }
            }
        }

        public bool Validate()
        {
            FormatValues();

            var isValidate =
                       !string.IsNullOrEmpty(StaticModel)
                    && (!string.IsNullOrEmpty(StaticModelFile) || GenSampleConfigJSON);

            return isValidate;
        }
    }
}
