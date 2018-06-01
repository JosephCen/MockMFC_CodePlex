using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CLQCodeGen.CodeTemplates.SubTemplates
{
    public partial class BaseSubTemplate
    {
        protected virtual void CalculateTemplateData()
        {
        }

        protected void WriteSubTemplate(Type templateType, params object[] parameters)
        {
            var subTemplate = (BaseSubTemplate)Activator.CreateInstance(templateType, parameters);

            subTemplate.CalculateTemplateData();

            Write(subTemplate.TransformText());
        }
    }
}
