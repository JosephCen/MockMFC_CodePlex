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
    
    #line 1 "C:\Users\s1ig\documents\visual studio 2015\Projects\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\CopyRight.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "14.0.0.0")]
    public partial class CopyRight : BaseSubTemplate
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public override string TransformText()
        {
            this.Write("//-----------------------------------------------------------------------\r\n// <co" +
                    "pyright file=\"");
            
            #line 3 "C:\Users\s1ig\documents\visual studio 2015\Projects\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\CopyRight.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetClassName()));
            
            #line default
            #line hidden
            this.Write(".cs\" company=\"Eurofins\">\r\n//  Copyright ");
            
            #line 4 "C:\Users\s1ig\documents\visual studio 2015\Projects\CLQCodeGen\CLQCodeGen\CodeTemplates\SubTemplates\CopyRight.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(GetCurrentYear()));
            
            #line default
            #line hidden
            this.Write(" Eurofins Scientific Ltd, Ireland\r\n//  Usage reserved to Eurofins Global Franchis" +
                    "e Model subscribers.\r\n// </copyright>\r\n//---------------------------------------" +
                    "--------------------------------\r\n\r\n");
            return this.GenerationEnvironment.ToString();
        }
    }
    
    #line default
    #line hidden
}