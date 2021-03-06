﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 14.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace CLQCodeGen.CodeTemplates
{
    using System;
    using System.Linq;
    using System.Text;
    using System.Collections.Generic;
    using System.Reflection;
    using CLQCodeGen.CodeTemplates.SubTemplates;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public partial class UnitTest : BaseMainTemplate
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public override string TransformText()
        {
            
            #line 9 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    WriteSubTemplate(typeof(SubTemplates.CopyRight.CopyRight), GetUnitTestClassName());

            
            #line default
            #line hidden
            
            #line 12 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    PrintNamespaces();

            
            #line default
            #line hidden
            this.Write("\r\nnamespace ");
            
            #line 16 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetUnitTestNamespace()));
            
            #line default
            #line hidden
            this.Write("\r\n{\r\n    /// <summary>\r\n    /// Unit-test for <see cref=\"");
            
            #line 19 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("\"/>.\r\n    /// </summary>\r\n    [TestClass]\r\n    public class ");
            
            #line 22 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetUnitTestClassName()));
            
            #line default
            #line hidden
            this.Write("\r\n    {\r\n        #region Fields\r\n\r\n        private ");
            
            #line 26 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write(" ");
            
            #line 26 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetFieldName()));
            
            #line default
            #line hidden
            this.Write(";\r\n\r\n");
            
            #line 28 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    var mockFieldAndTypeList = GetMockFieldAndTypeList();

    foreach (var fieldAndType in mockFieldAndTypeList)
    {
        WriteLine($"        private Mock<{fieldAndType.TypeName}> {fieldAndType.Name};");
    }

            
            #line default
            #line hidden
            this.Write("\r\n        #endregion\r\n\r\n");
            
            #line 39 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    WriteSubTemplate(typeof(TestInitialise), GetTargetFieldName(), GetTargetType(), GetMockFieldAndTypeList());

            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 43 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    WriteSubTemplate(typeof(ConstructorTestMethods), GetTargetTypeName());

            
            #line default
            #line hidden
            this.Write("\r\n        #region Test Methods\r\n\r\n");
            
            #line 49 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    foreach (var methodInfo in GetMethodList())
    {

            
            #line default
            #line hidden
            this.Write("        [TestMethod, TestCategory(TestCategory.UnitTest)]\r\n        public void ");
            
            #line 54 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("_");
            
            #line 54 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(methodInfo.Name));
            
            #line default
            #line hidden
            this.Write("_()\r\n        {\r\n");
            
            #line 56 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    if (methodInfo.GetParameters().Any())
    {
        var isFirstParam = true;

        Write($"            // {methodInfo.Name}(");
        foreach (var paramInfo in methodInfo.GetParameters())
        {
            Write(isFirstParam ? "" : ", ");
            Write($"{paramInfo.ParameterType.Name} {paramInfo.Name}");
            isFirstParam = false;
        }
        WriteLine(")");
    }

            
            #line default
            #line hidden
            this.Write("            // Arrange\r\n\r\n            // Act\r\n\r\n            // Assert\r\n        }\r" +
                    "\n\r\n");
            
            #line 78 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    }

            
            #line default
            #line hidden
            this.Write("        #endregion\r\n    }\r\n}\r\n");
            return this.GenerationEnvironment.ToString();
        }
        
        #line 84 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\UnitTest.tt"

    private void PrintNamespaces()
    {
        var namespaceList = GetNamespaces();

        foreach (var ns in namespaceList.Distinct().OrderBy(s => s))
	    {
	        WriteLine($"using {ns};");
	    }
    }

    private string GetTargetTypeName()
	{
	    return GetTargetType().Name;
	}

	private string GetTargetTypeNameSpace()
	{
	    return GetTargetType().Namespace;
	}

        
        #line default
        #line hidden
    }
    
    #line default
    #line hidden
}
