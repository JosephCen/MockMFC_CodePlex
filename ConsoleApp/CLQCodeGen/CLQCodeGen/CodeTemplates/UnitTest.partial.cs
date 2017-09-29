using CLQCodeGen.CodeTemplates.DataObjects;
using CLQCodeGen.Helpers;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;

namespace CLQCodeGen.CodeTemplates
{
    public partial class UnitTest
    {
        private readonly string _targetAssemblyNameParam;
        private readonly string _targetTypeName;
        private readonly string _unitTestFileName;

        private readonly TypeHelper _typeHelper;
        private readonly ProjectFileHelper _projectFileHelper;
        private readonly VariableNameHelper _variableNameHelper;

        private Type _targetType;
        private Assembly _targetAssembly;
        private IList<Type> _dependencyList;
        private IList<MethodInfo> _methodList;
        private bool? _useFullName;

        public UnitTest(string targetAssemblyNameParam, string targetTypeName, string unitTestFileName = "")
        {
            _typeHelper = new TypeHelper();
            _projectFileHelper = new ProjectFileHelper();
            _variableNameHelper = new VariableNameHelper();

            _targetAssemblyNameParam = targetAssemblyNameParam;
            _targetTypeName = targetTypeName;
            _unitTestFileName = unitTestFileName;
        }

        private Type GetTargetType()
        {
            if (_targetType == null)
            {
                var typeName = $"{GetTargetNamespace()}.{_targetTypeName}";
                var assembly = GetTargetAssembly();

                _targetType = assembly.GetType(typeName);

                if (_targetType == null)
                {
                    throw new InvalidOperationException($"Fail to load type {typeName}. You may need to rebuild your product assembly.");
                }
            }

            return _targetType;
        }

        private string GetTargetAssemblyFileName()
        {
            var assemblyName = _targetAssemblyNameParam;

            if (assemblyName.Equals("_default", StringComparison.OrdinalIgnoreCase))
            {
                var unitTestAssemblyName = _projectFileHelper.GetAssemblyName(GetUnitTestProjectFile());

                assemblyName = unitTestAssemblyName.Replace(".Tests", "");
            }

            var assemblyFileName = $"{assemblyName}.dll";

            var assemblyProjectFile = string.Empty;

            if (_projectFileHelper.TryFindReferencedProjectFileByAssemblyName(
                                                                GetUnitTestProjectFile(),
                                                                assemblyName,
                                                                out assemblyProjectFile))
            {
                return Path.Combine(Path.GetDirectoryName(assemblyProjectFile), @"bin\Debug", assemblyFileName);
            }

            return Path.Combine(GetUnitTestProjectPath(), @"bin\Debug", assemblyFileName);
        }

        private Assembly GetTargetAssembly()
        {
            if (_targetAssembly == null)
            {
                var assemblyFileName = GetTargetAssemblyFileName();

                if (!File.Exists(assemblyFileName))
                {
                    throw new InvalidOperationException($"Fail to find assmbly file {assemblyFileName}. You may need to give an assembly file name.");
                }

                // Just load given assembly file.
                // var assembly = Assembly.LoadFile(assemblyFileName);

                // AddAssemblyFileResolver();

                var assembly = Assembly.LoadFrom(assemblyFileName);

                _targetAssembly = assembly;
            }

            return _targetAssembly;
        }

        private void AddAssemblyFileResolver()
        {
            var appDomain = AppDomain.CurrentDomain;

            appDomain.AssemblyResolve += LoadFromBinFolder;
        }

        private Assembly LoadFromBinFolder(object sender, ResolveEventArgs args)
        {
            var projBinPath = Path.Combine(GetUnitTestProjectPath(), @"bin\Debug");
            var assmName = new AssemblyName(args.Name);
            var assmFileFullName = Path.Combine(projBinPath, $"{assmName.Name}.dll");

            if (File.Exists(assmFileFullName))
            {
                return Assembly.LoadFrom(assmFileFullName);
            }
            else
            {
                return null;
            }
        }

        private string GetUnitTestFilePath() =>
            (!string.IsNullOrEmpty(_unitTestFileName) && Path.IsPathRooted(_unitTestFileName))
                 ? Path.GetDirectoryName(_unitTestFileName)
                 : Environment.CurrentDirectory;

        private string GetUnitTestProjectPath()
        {
            var projectPath = GetUnitTestFilePath();

            for (; !string.IsNullOrEmpty(projectPath); projectPath = Path.GetDirectoryName(projectPath))
            {
                if (Directory.GetFiles(projectPath, "*.csproj").Length == 1)
                {
                    break;
                }
            }

            return projectPath;
        }

        private string GetUnitTestProjectFile() =>
            Directory.GetFiles(GetUnitTestProjectPath(), "*.csproj").Single();

        private string GetUnitTestRootNamespace()
        {
            return _projectFileHelper.GetRootNamespace(GetUnitTestProjectFile());
        }

        private string GetUnitTestNamespace()
        {
            var rootNamespace = GetUnitTestRootNamespace();
            var projectPath = GetUnitTestProjectPath();
            var unitTestFilePath = GetUnitTestFilePath();

            return unitTestFilePath.Replace(projectPath, rootNamespace).Replace('\\', '.');
        }

        private string GetTargetNamespace()
        {
            var unitTestRootNamespace = GetUnitTestRootNamespace();
            var unitTestNamespace = GetUnitTestNamespace();

            return unitTestNamespace.Replace(unitTestRootNamespace, unitTestRootNamespace.Replace(".Tests", ""));
        }

        private IList<Type> GetDependencyList()
        {
            if (_dependencyList == null)
            {
                _dependencyList = _typeHelper.GetDependencyList(GetTargetType());
            }

            return _dependencyList;
        }

        private bool UseFullName()
        {
            if (!_useFullName.HasValue)
            {
                var fieldNames = GetDependencyList().Select(t => GetFieldNameFromType(t, false)).ToList();
                var isDup = fieldNames.Count > 1 && fieldNames.Distinct().Count() != fieldNames.Count;

                _useFullName = isDup;
            }

            return _useFullName.Value;
        }

        private IList<FieldAndType> GetMockFieldAndTypeList() =>
            GetDependencyList()
                .Select(t => new FieldAndType { Name = $"_{GetFieldNameFromType(t, UseFullName())}Mock", Type = t })
                .ToList();

        private IList<MethodInfo> GetMethodList()
        {
            if (_methodList == null)
            {
                _methodList = _typeHelper.GetMethodList(GetTargetType());
            }

            return _methodList;
        }

        public string GetUnitTestClassName() =>
            $"{_targetTypeName}Test";

        private string GetFieldNameFromType(Type type, bool isFull)
        {
            var typeName = type.Name;
            var fieldName = typeName.Substring(type.IsInterface ? 1 : 0);
            var wordList = _variableNameHelper.SplitIntoWords(fieldName);

            if (isFull)
            {
                var excludePrefixes = new[] { "db" };

                if (excludePrefixes.Any(p => p.Equals(wordList[0], StringComparison.OrdinalIgnoreCase)))
                {
                    wordList.RemoveAt(0);
                }

                fieldName = _variableNameHelper.LowerFirstChar(string.Concat(wordList));
            }
            else
            {
                fieldName = _variableNameHelper.LowerFirstChar(wordList.Last());
            }

            return fieldName;
        }

        private IList<string> GetNamespaces()
        {
            var namespaceList = new List<string>
            {
                "Eurofins.ComLIMS.ApplicationBlock.ServiceLocation",
                "Eurofins.ComLIMS.Quotation.UnitTesting",
                "Microsoft.VisualStudio.TestTools.UnitTesting",
                "Moq"
            };

            namespaceList.Add(GetTargetTypeNameSpace());
            namespaceList.AddRange(GetDependencyList().Select(t => t.Namespace));

            return namespaceList;
        }

        private string GetTargetFieldName() =>
            $"_target{_variableNameHelper.UpperFirstChar(GetFieldNameFromType(GetTargetType(), false))}";
    }
}
