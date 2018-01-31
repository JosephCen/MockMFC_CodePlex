using System.IO;
using CLQCodeGen.CodeTemplates;
using CLQCodeGen.Console.GenArguments;
using CLQCodeGen.Generators;
using CLQCodeGen.Helpers;
using CLQCodeGen.ProxyAdapters;
using CommandLine;

namespace CLQCodeGen.Console
{
    class Program
    {
        static int Main(string[] args)
        {
            return Run(args) ? 0 : 1;
        }

        static void RunInAppDomain(string[] args)
        {
            if (args.Length == 3)
            {
                var generatorAdapter = new UnitTestGeneratorAdapter();
                var request = new CallUnitTestGeneratorRequest
                {
                    Request = new UnitTestGenerateRequest
                    {
                        ProdAssemblyName = args[0],
                        TargetTypeName = args[1],
                        UnitTestFileName = args[2]
                    }
                };

                var response = generatorAdapter.CallUnitTestGeneratorInIsolateAppDomain(request);

                System.Console.WriteLine($"Is Success: {response.IsSuccess}");
            }
        }

        static bool Run(string[] args)
        {
            var fileHelper = new FileHelper();
            var isSuccess = true;

            using (var argsParser = new Parser(config => { config.HelpWriter = System.Console.Out; config.CaseSensitive = false; }))
            {
                argsParser.ParseArguments<UnitTestArgument, StaticModelArgument>(args)
                    .WithParsed<UnitTestArgument>(
                        argument =>
                        {
                            isSuccess = isSuccess && argument.Validate();

                            if (isSuccess)
                            {
                                var generator = new UnitTest(argument.ProdAssemblyName, argument.TargetTypeName);
                                var sourceFileName = Path.GetFullPath($"{generator.GetUnitTestClassName()}.cs");
                                var classText = generator.TransformText();

                                fileHelper.SaveAndArchiveOriginal(sourceFileName, classText);
                            }
                        })
                    .WithParsed<StaticModelArgument>(
                        argument =>
                        {
                            isSuccess = isSuccess && argument.Validate();

                            if (isSuccess)
                            {
                                if (argument.GenSampleConfigJSON && string.IsNullOrEmpty(argument.StaticModelFile))
                                {
                                    var sampleJSON = StaticModelType.WriteSampleConfigJSON(System.Environment.CurrentDirectory);

                                    System.Console.WriteLine($"{sampleJSON} has been generated.");
                                }
                                else
                                {
                                    var generator = new StaticModelType(argument.StaticModelFile);
                                    var sourceFileName = argument.StaticModelFile;
                                    var classText = generator.TransformText();

                                    fileHelper.SaveAndArchiveOriginal(sourceFileName, classText);
                                }
                            }
                        })
                    .WithNotParsed(errs => isSuccess = false);
            }

            return isSuccess;
        }
    }
}
