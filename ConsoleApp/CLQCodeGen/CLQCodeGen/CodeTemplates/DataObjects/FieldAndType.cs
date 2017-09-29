using System;

namespace CLQCodeGen.CodeTemplates.DataObjects
{
    public class FieldAndType
    {
        public string Name { get; set; }

        public Type Type { get; set; }

        public string TypeName
        {
            get { return this.Type.Name; }
        }

        public FieldAndType Clone()
        {
            return 
                new FieldAndType
                    {
                        Name = Name,
                        Type = Type
                    };
        }
    }
}
