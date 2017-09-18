using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CLQCodeGen.Tests.CodeTemplates
{
    [TestClass]
    public sealed class SetupAspectLoaderMockTest
    {
        #region Inner Object

        private class AspectLoader
        {
            public IList<string> Load(IList<string> stringList)
            {
                return stringList;
            }
        }

        #endregion

        #region Try Methods

        [TestMethod, TestCategory("UnitTest")]
        public void Get_Generic_Type_Of_a_Generic_List()
        {
            // Arrange
            var aspectLoader = new AspectLoader();
            var type = aspectLoader.GetType();
            var methodInfo = type.GetMethods().First();

            // Act
            var returnType = methodInfo.ReturnType;
            var returnParameter = methodInfo.ReturnParameter;

            // Assert
            Assert.IsNotNull(returnType);
            Assert.IsNotNull(returnParameter);
        }

        #endregion
    }
}
