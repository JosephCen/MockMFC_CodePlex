using CLQCodeGen.Helpers;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CLQCodeGen.Tests.Helpers
{
    [TestClass]
    public sealed class TypeHelperTest
    {
        private TypeHelper _targetHelper;

        #region Test Initialise

        [TestInitialize]
        public void TestInitialise()
        {
            _targetHelper = new TypeHelper();
        }

        #endregion

        #region Test Methods

        private class Disposable : IDisposable
        {
            public void Dispose()
            {
                Debug.WriteLine("Class is disposed.");
            }
        }

        [TestMethod, TestCategory("UnitTest")]
        public void TestFun()
        {
            using (var c = new Disposable())
            {
                Debug.WriteLine("Do something.");

                return;
            }
        }

        [TestMethod, TestCategory("UnitTest")]
        [DeploymentItem(@"Helpers\MyDataXml.xml")]
        public void TestFunWithDeployItem()
        {
            Debug.WriteLine(Environment.CurrentDirectory);
            Assert.IsTrue(File.Exists("MyDataXml.xml"));
        }

        #endregion
    }
}
