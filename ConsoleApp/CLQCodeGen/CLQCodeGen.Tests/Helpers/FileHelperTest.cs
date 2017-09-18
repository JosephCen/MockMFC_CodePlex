using System;

namespace CLQCodeGen.Tests.Helpers
{
    using CLQCodeGen.Helpers;
    using Microsoft.VisualStudio.TestTools.UnitTesting;
    using System.IO;

    [TestClass]
    public sealed class FileHelperTest
    {
        private FileHelper _targetHelper;

        #region Test Initialise & Cleanup

        [TestInitialize]
        public void TestInitialise()
        {
            _targetHelper = new FileHelper();
        }

        [ClassCleanup]
        public static void TestCleanup()
        {
            foreach (var f in Directory.GetFiles(Environment.CurrentDirectory, "*.txt"))
            {
                File.Delete(f);
            }
        }

        #endregion

        #region Test Methods

        [TestMethod, TestCategory("UnitTest")]
        public void FileHelper_SaveAndArchiveOriginal_will_save_file_content()
        {
            // Arrange
            var fileContent = "FILE CONTENT";
            var fileName = Path.Combine(Environment.CurrentDirectory, "TestTxtFile.txt");

            // Act
            _targetHelper.SaveAndArchiveOriginal(fileName, fileContent);

            // Assert
            Assert.IsTrue(File.Exists(fileName));
            Assert.AreEqual(fileContent, File.ReadAllText(fileName));
        }

        [TestMethod, TestCategory("UnitTest")]
        [DeploymentItem(@"Helpers\TypeHelperTest_SampleFile.txt")]
        public void FileHelper_SaveAndArchiveOriginal_will_archive_original_file()
        {
            // Arrange
            var originalFileContent = File.ReadAllText("TypeHelperTest_SampleFile.txt");
            var archivedFileName = "TypeHelperTest_SampleFile.archive.txt";
            var fileContent = "FILE CONTENT";
            var fileName = Path.Combine(Environment.CurrentDirectory, "TypeHelperTest_SampleFile.txt");

            // Act
            _targetHelper.SaveAndArchiveOriginal(fileName, fileContent);

            // Assert
            Assert.IsTrue(File.Exists(fileName));
            Assert.AreEqual(fileContent, File.ReadAllText(fileName));
            Assert.IsTrue(File.Exists(archivedFileName));
            Assert.AreEqual(originalFileContent, File.ReadAllText(archivedFileName));
        }

        #endregion
    }
}
