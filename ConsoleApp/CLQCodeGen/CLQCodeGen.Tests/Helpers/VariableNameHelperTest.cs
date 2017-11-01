using System.Linq;
using CLQCodeGen.Helpers;
using Microsoft.VisualStudio.TestTools.UnitTesting;

namespace CLQCodeGen.Tests.Helpers
{
    [TestClass]
    public class VariableNameHelperTest
    {
        private VariableNameHelper _targetHelper;

        #region Test Initialise

        [TestInitialize]
        public void TestInitialise()
        {
            _targetHelper = new VariableNameHelper();
        }

        #endregion

        #region Test Methods

        [TestMethod, TestCategory("UnitTest")]
        public void VariableNameHelper_SplitIntoWords_works_well()
        {
            // Arrange
            var name = "VariableNameHelper";
            var expectWords = new[] { "Variable", "Name", "Helper" };

            // Act
            var words = _targetHelper.SplitIntoWords(name);

            // Assert
            Assert.IsTrue(expectWords.SequenceEqual(words));
        }

        #endregion
    }
}
