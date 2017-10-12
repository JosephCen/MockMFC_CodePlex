using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using CLQCodeGen.Helpers;
using JetBrains.Annotations;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;

namespace CLQCodeGen.CodeTemplates
{
    public partial class StaticModelType
    {
        private readonly ProjectFileHelper _projectFileHelper;
        private readonly VariableNameHelper _variableNameHelper;

        private string _sourceFileName;

        public StaticModelType(string sourceFileName)
        {
            _projectFileHelper = new ProjectFileHelper();
            _variableNameHelper = new VariableNameHelper();

            _sourceFileName = sourceFileName;
        }

        private string GetModelTypeName() =>
            Path.GetFileNameWithoutExtension(_sourceFileName);

        private string GetSourceFilePath() =>
            Path.GetDirectoryName(_sourceFileName);

        private string GetProjectFile()
        {
            return _projectFileHelper.FindCurrentProjectFileBySourceFile(GetSourceFilePath());
        }

        private string GetProjectRootNamespace()
        {
            return _projectFileHelper.GetRootNamespace(GetProjectFile());
        }

        private string GetModelTypeNamespace()
        {
            var rootNamespace = GetProjectRootNamespace();
            var projectPath = Path.GetDirectoryName(GetProjectFile());
            var sourceFilePath = GetSourceFilePath();

            return sourceFilePath.Replace(projectPath, rootNamespace).Replace('\\', '.');
        }

        private class CodeNamePair
        {
            public string Code { get; set; }

            public string CodeVar => $"{ Code }Code";

            public string CodeValue => Code.ToUpper();

            public string NameVar => $"{ Code }Name";

            public string NameValue { get; set; }

            public string ClassVar => $"{ Code }Value";
        }

        private IList<CodeNamePair> GetCodeNamePairs()
        {
            IEnumerable<KeyValuePair<string, JToken>> modelTypeData = ReadModelTypeData();

            return
                modelTypeData.Where(prop => prop.Value is JValue)
                             .Select(prop =>
                                new CodeNamePair
                                {
                                    Code = prop.Key,
                                    NameValue = ((JValue)prop.Value).Value.ToString()
                                })
                             .ToList();
        }

        private JObject ReadModelTypeData()
        {
            var dataFileName = GetModelTypeDataFileName();

            try
            {
                return JObject.Parse(File.ReadAllText(dataFileName));
            }
            catch (JsonReaderException ex)
            {
                var sampleFileName = Path.Combine(Path.GetDirectoryName(dataFileName), "StaticModelType.sample.json");

                if (!File.Exists(sampleFileName))
                {
                    var sampleFileContent =
@"{
  'Standard' : 'Standard Quotation',
  'Project' : 'Project Quotation'
}";

                    File.WriteAllText(sampleFileName, sampleFileContent);
                }

                throw new InvalidOperationException($"Fail to parse the model data file for class { GetModelTypeName() }.", ex);
            }
        }

        [NotNull]
        private string GetModelTypeDataFileName()
        {
            var directory = GetSourceFilePath();
            var fileNameWithoutExt = GetModelTypeName();
            var modelTypeDataFileName = Path.Combine(directory, $"{ fileNameWithoutExt }.json");

            if (!File.Exists(modelTypeDataFileName))
            {
                throw new InvalidOperationException($"Fail to load model type data for class { GetModelTypeName() }.");
            }

            return modelTypeDataFileName;
        }
    }
}
