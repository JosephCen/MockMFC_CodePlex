using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CLQCodeGen.Helpers;
using Microsoft.VisualStudio.TestTools.UnitTesting;

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
        public void TestLazyType()
        {
            var lazyStrBuilder = new Lazy<StringBuilder>(() => new StringBuilder());
            var instanceType = lazyStrBuilder.GetType();

            Debug.WriteLine($"TypeName: {instanceType.Name}");
            Debug.WriteLine($"IsGenericParameter: {instanceType.IsGenericParameter}");
            Debug.WriteLine($"IsGenericType: {instanceType.IsGenericType}");
            Debug.WriteLine($"IsGenericTypeDefinition: {instanceType.IsGenericTypeDefinition}");

            var genericTypeDef = instanceType.GetGenericTypeDefinition();

            Debug.WriteLine($"GetGenericTypeDefinition: {genericTypeDef.Name}");

            var genericArguments = instanceType.GetGenericArguments();

            Debug.WriteLine($"GetGenericTypeDefinition: {string.Join(", ", genericArguments.Select(t => t.Name))}");
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
