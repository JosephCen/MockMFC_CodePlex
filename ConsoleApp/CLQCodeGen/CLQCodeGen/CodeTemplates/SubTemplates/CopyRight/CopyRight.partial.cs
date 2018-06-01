using System;

namespace CLQCodeGen.CodeTemplates.SubTemplates.CopyRight
{
    public partial class CopyRight
    {
        private readonly string _className;

        private CopyRightData Data { get; set; }

        public CopyRight(string className)
        {
            _className = className;
        }

        protected override void CalculateTemplateData()
        {
            Data = new CopyRightData
            {
                ClassName = _className,
                CurrentYear = DateTime.Now.Year.ToString()
            };
        }
    }
}
