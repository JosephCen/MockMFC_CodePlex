﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 14.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace CLQCodeGen.CodeTemplates.SubTemplates
{
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public partial class ConstructorTestMethods : BaseSubTemplate
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public override string TransformText()
        {
            this.Write("        #region Constructor\r\n\r\n        /// <summary>\r\n        /// Verify <see cre" +
                    "f=\"");
            
            #line 5 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("\"/> is registered as <see cref=\"");
            
            #line 5 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetInterfaceName()));
            
            #line default
            #line hidden
            this.Write("\"/>.\r\n        /// </summary>\r\n        [TestMethod, TestCategory(TestCategory.Unit" +
                    "Test)]\r\n        public void ");
            
            #line 8 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("_ServiceLocator_the_class_is_registered_as_");
            
            #line 8 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetInterfaceName()));
            
            #line default
            #line hidden
            this.Write("()\r\n        {\r\n            // Arrange\r\n            using (var serviceLocator = ne" +
                    "w ServiceLocator())\r\n            {\r\n                // Act\r\n                var " +
                    "result = serviceLocator\r\n                                  .Is(typeof(");
            
            #line 15 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("))\r\n                                  .RegisteredAs<");
            
            #line 16 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetInterfaceName()));
            
            #line default
            #line hidden
            this.Write(">();\r\n\r\n                // Assert\r\n                Assert.IsTrue(result);\r\n      " +
                    "      }\r\n        }\r\n\r\n        /// <summary>\r\n        /// Verify instance of <see" +
                    " cref=\"");
            
            #line 24 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("\"/> will be created successful by <see cref=\"ServiceLocator\"/>.\r\n        /// </su" +
                    "mmary>\r\n        [TestMethod, TestCategory(TestCategory.UnitTest)]\r\n        publi" +
                    "c void ");
            
            #line 27 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("_ServiceLocator_returns_instance()\r\n        {\r\n            // Arrange\r\n          " +
                    "  using (var serviceLocator = new ServiceLocator())\r\n            {\r\n            " +
                    "    // Act\r\n                var ");
            
            #line 33 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetInstanceVariableName()));
            
            #line default
            #line hidden
            this.Write(" = serviceLocator.GetInstance<");
            
            #line 33 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetInterfaceName()));
            
            #line default
            #line hidden
            this.Write(">();\r\n\r\n                // Assert\r\n                Assert.IsNotNull(");
            
            #line 36 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetInstanceVariableName()));
            
            #line default
            #line hidden
            this.Write(");\r\n                Assert.IsInstanceOfType(");
            
            #line 37 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetInstanceVariableName()));
            
            #line default
            #line hidden
            this.Write(", typeof(");
            
            #line 37 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\ConstructorTestMethods.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("));\r\n            }\r\n        }\r\n\r\n        #endregion\r\n");
            return this.GenerationEnvironment.ToString();
        }
    }
    
    #line default
    #line hidden
}
