using System;
using System.IO;
using System.Linq;
using System.Xml;

namespace CLQCodeGen.Helpers
{
    internal class ProjectFileHelper
    {
        #region Fields

        private const string NsMs = "ms"; // Default namespace from Microsoft for project file.

        #endregion

        public string GetRootNamespace(string fullFileName)
        {
            var xmlDocumentPair = LoadProjectFile(fullFileName);
            var fileXml = xmlDocumentPair.Document;
            var nsmgr = xmlDocumentPair.NamespaceManager;

            return fileXml.SelectSingleNode($"//{NsMs}:RootNamespace", nsmgr).InnerText;
        }

        public string GetAssemblyName(string fullFileName)
        {
            var xmlDocumentPair = LoadProjectFile(fullFileName);
            var fileXml = xmlDocumentPair.Document;
            var nsmgr = xmlDocumentPair.NamespaceManager;

            return fileXml.SelectSingleNode($"//{NsMs}:AssemblyName", nsmgr).InnerText;
        }

        public bool TryFindReferencedProjectFileByAssemblyName(string fullFileName, string assemblyName, out string foundProjectFile)
        {
            var xmlDocumentPair = LoadProjectFile(fullFileName);
            var fileXml = xmlDocumentPair.Document;
            var nsmgr = xmlDocumentPair.NamespaceManager;
            var referenceNodes = fileXml.SelectNodes($"//{NsMs}:ItemGroup/{NsMs}:ProjectReference", nsmgr);

            foundProjectFile =
                referenceNodes.OfType<XmlNode>()
                    .Select(n => n.Attributes["Include"].Value)
                    .Select(pp => Path.Combine(Path.GetDirectoryName(fullFileName), pp)) // partial path to absolute path
                    .FirstOrDefault(fn => GetAssemblyName(fn) == assemblyName); // Select project file full name

            return !string.IsNullOrEmpty(foundProjectFile);
        }

        public string FindReferencedProjectFileByAssemblyName(string fullFileName, string assemblyName)
        {
            string foundProjectFile;

            if (!TryFindReferencedProjectFileByAssemblyName(fullFileName, assemblyName, out foundProjectFile))
            {
                throw new Exception($"Fail to find project name for assembly {assemblyName}");
            }

            return foundProjectFile;
        }

        public bool TryFindCurrentProjectFileBySourceFile(string sourceFileName, out string foundProjectFile)
        {
            var projectPath = Path.GetDirectoryName(sourceFileName);

            foundProjectFile = string.Empty;

            for (; !string.IsNullOrEmpty(projectPath); projectPath = Path.GetDirectoryName(projectPath))
            {
                var csprojFiles = Directory.GetFiles(projectPath, "*.csproj");
                if (csprojFiles.Length == 1)
                {
                    foundProjectFile = csprojFiles.Single();

                    break;
                }
            }

            return !string.IsNullOrEmpty(foundProjectFile);
        }

        public string FindCurrentProjectFileBySourceFile(string sourceFileName)
        {
            string foundProjectFile;

            if (!TryFindCurrentProjectFileBySourceFile(sourceFileName, out foundProjectFile))
            {
                throw new Exception($"Fail to find project name for source file: {sourceFileName}");
            }

            return foundProjectFile;
        }

        private XmlDocumentPair LoadProjectFile(string fullFileName)
        {
            if (!File.Exists(fullFileName))
            {
                throw new InvalidOperationException($"Project file '{fullFileName}' is not existing.");
            }

            var fileXml = new XmlDocument();
            var nsmgr = new XmlNamespaceManager(fileXml.NameTable);

            nsmgr.AddNamespace(NsMs, "http://schemas.microsoft.com/developer/msbuild/2003");
            fileXml.Load(fullFileName);

            return
                new XmlDocumentPair
                {
                    Document = fileXml,
                    NamespaceManager = nsmgr
                };
        }

        #region Inner Object

        private class XmlDocumentPair
        {
            public XmlDocument Document { get; set; }

            public XmlNamespaceManager NamespaceManager { get; set; }
        }

        #endregion
    }
}
