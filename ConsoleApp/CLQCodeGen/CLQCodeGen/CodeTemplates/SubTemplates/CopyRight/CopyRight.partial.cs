using System;

namespace CLQCodeGen.CodeTemplates.SubTemplates
{
    public partial class CopyRight
    {
        private readonly string _className;

        public CopyRight(string className)
        {
            _className = className;
        }

        private string GetClassName() => 
            _className;

        private string GetCurrentYear() => 
            DateTime.Now.Year.ToString();
    }
}
