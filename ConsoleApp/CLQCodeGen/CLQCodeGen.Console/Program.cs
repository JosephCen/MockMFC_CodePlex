using CLQCodeGen.CodeTemplates;
using CLQCodeGen.Generators;
using CLQCodeGen.Helpers;
using CLQCodeGen.ProxyAdapters;
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace CLQCodeGen.Console
{
    class Program
    {
        static void Main(string[] args)
        {
            Run(args);
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

        static void Run(string[] args)
        {
            var fileHelper = new FileHelper();

            if (args.Length == 2)
            {
                var prodAssmName = args[0];
                var targetTypeName = GetTargetTypeName(args[1]);

                var unitTestGenerator = new UnitTest(prodAssmName, targetTypeName);

                var unitTestClassDefinition = unitTestGenerator.TransformText();
                var unitTestSourceFileName = Path.Combine(Environment.CurrentDirectory, $"{unitTestGenerator.GetUnitTestClassName()}.cs");

                fileHelper.SaveAndArchiveOriginal(unitTestSourceFileName, unitTestClassDefinition);
            }
            if (args.Length == 1)
            {
                var modelTypeSourceFile = Path.IsPathRooted(args[0]) ? args[0] : Path.GetPathRoot(args[0]);

                var generator = new StaticModelType(modelTypeSourceFile);

                var classDefinition = generator.TransformText();

                fileHelper.SaveAndArchiveOriginal(modelTypeSourceFile, classDefinition);
            }
        }

        static string GetTargetTypeName(string arg)
        {
            var testSrcFileMatch = Regex.Match(arg, @"(\w+)Test(?:.cs)?");

            return testSrcFileMatch.Success ? testSrcFileMatch.Groups[1].Value : arg;
        }

        static void TestFunc()
        {
            var curPath = Environment.CurrentDirectory;

            var parentPath = System.IO.Path.GetDirectoryName(curPath);
        }
    }
}
