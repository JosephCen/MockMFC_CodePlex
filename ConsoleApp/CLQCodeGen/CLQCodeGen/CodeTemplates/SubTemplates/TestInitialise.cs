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
    using CLQCodeGen.CodeTemplates.DataObjects;
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public partial class TestInitialise : BaseSubTemplate
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public override string TransformText()
        {
            
            #line 3 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

    const int inFuncMockThreshold = 3;
    var mockFieldAndTypeList = GetMockFieldAndTypeList();

            
            #line default
            #line hidden
            this.Write("        #region Test Initialise\r\n\r\n        /// <summary>\r\n        /// Initializat" +
                    "ion for each unit-test.\r\n        /// </summary>\r\n        [TestInitialize]\r\n     " +
                    "   public void TestInitialise()\r\n        {\r\n");
            
            #line 15 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

    foreach (var fieldAndType in mockFieldAndTypeList)                                        //
    {                                                                                         //
        WriteLine($"            {fieldAndType.Name} = new Mock<{fieldAndType.TypeName}>();"); //
    }                                                                                         //

            
            #line default
            #line hidden
            this.Write("\r\n            ");
            
            #line 22 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetFieldName()));
            
            #line default
            #line hidden
            this.Write(" = new ");
            
            #line 22 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetTargetTypeName()));
            
            #line default
            #line hidden
            this.Write("(");
            
            #line 22 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
                   //
    if (mockFieldAndTypeList.Count > 0)                                                       //
        WriteLine("");                                                                        //
                                                                                              //
    for (int i = 0; i < mockFieldAndTypeList.Count; ++i)                                      //
    {                                                                                         //
	    var fieldAndType = mockFieldAndTypeList[i];                                           //
        var isLast = (i+1 == mockFieldAndTypeList.Count);                                     //
        var lineEnd = (isLast ? "" : ",\r\n");                                                //
                                                                                              //
        if (fieldAndType.TypeExtension.IsLazy)                                                //
        {                                                                                     //

            
            #line default
            #line hidden
            this.Write("                                    new Lazy<");
            
            #line 35 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(fieldAndType.TypeName));
            
            #line default
            #line hidden
            this.Write(">(() => ");
            
            #line 35 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(fieldAndType.Name));
            
            #line default
            #line hidden
            this.Write(".Object)");
            
            #line 35 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(lineEnd));
            
            #line default
            #line hidden
            
            #line 35 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

        }                                                                                     //
        else                                                                                  //
        {                                                                                     //

            
            #line default
            #line hidden
            this.Write("                                    ");
            
            #line 40 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(fieldAndType.Name));
            
            #line default
            #line hidden
            this.Write(".Object");
            
            #line 40 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(lineEnd));
            
            #line default
            #line hidden
            
            #line 40 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

        }                                                                                     //
    }                                                                                         //
    WriteLine(");");                                                                          //
                                                                                              //
    foreach (var fieldAndType in mockFieldAndTypeList)                                        //
    {                                                                                         //
        if (mockFieldAndTypeList.Count > inFuncMockThreshold)                                 //
        {                                                                                     //

            
            #line default
            #line hidden
            this.Write("\r\n            Initialise(");
            
            #line 51 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(fieldAndType.Name));
            
            #line default
            #line hidden
            this.Write(");\r\n");
            
            #line 52 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            
        }
        else
        {
            SetupMock(fieldAndType);
        }
    }

            
            #line default
            #line hidden
            this.Write("        }\r\n\r\n");
            
            #line 62 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

    if (mockFieldAndTypeList.Count > inFuncMockThreshold)
    {
        foreach (var fieldAndType in mockFieldAndTypeList)
        {
            var curFieldAndType = fieldAndType.Clone();

            curFieldAndType.Name = ToInFuncName(curFieldAndType.Name);

            
            #line default
            #line hidden
            this.Write("        private void Initialise(Mock<");
            
            #line 71 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(curFieldAndType.TypeName));
            
            #line default
            #line hidden
            this.Write("> ");
            
            #line 71 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(curFieldAndType.Name));
            
            #line default
            #line hidden
            this.Write(")\r\n        {\r\n");
            
            #line 73 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"
 
        SetupMock(curFieldAndType);

            
            #line default
            #line hidden
            this.Write("        }\r\n\r\n");
            
            #line 78 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

        }
    }

            
            #line default
            #line hidden
            this.Write("        #endregion\r\n");
            return this.GenerationEnvironment.ToString();
        }
        
        #line 83 "C:\Users\s1ig\Documents\GitHub\MockMFC_CodePlex\ConsoleApp\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\TestInitialise.tt"

    private void SetupMock(FieldAndType fieldAndType)
    {
        if (fieldAndType.TypeName.Contains("AspectLoader"))
        {
            WriteSubTemplate(typeof(SetupAspectLoaderMock), fieldAndType);
        }
    }

        
        #line default
        #line hidden
    }
    
    #line default
    #line hidden
}
