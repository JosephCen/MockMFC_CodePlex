using System;
using System.IO;
using CLQCodeGen.CodeTemplates;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace CLQCodeGen.Tests.SystemTests
{
    [TestClass]
    public class UnitTestTest
    {
        [TestMethod, TestCategory("UnitTest")]
        public void UnitTest_for_AccountContactAspectLoader()
        {
            // Arrange
            var unitTestSourceFile = @"W:\Q\Dev\1042653\Tests\Domain.Services.Tests\AccountContacts\Gateways\Loaders\AccountContactAspectLoaderTest.cs";
            var currentDirectory = Path.GetDirectoryName(unitTestSourceFile);
            var sourceFileName = "AccountContactAspectLoader";
            Environment.CurrentDirectory = currentDirectory;
            var expectedSourceCode = File.ReadAllText(unitTestSourceFile);

            // Act
            var unitTest = new UnitTest("_default", sourceFileName);
            var generatedSourceCode = unitTest.TransformText();

            // Assert
            Assert.IsFalse(string.IsNullOrEmpty(generatedSourceCode));
            Assert.AreEqual(expectedSourceCode, generatedSourceCode);
        }
    }
}
