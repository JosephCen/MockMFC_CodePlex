using System;

namespace CLQCodeGen.CodeTemplates.DataObjects
{
    public class TypeExtension
    {
        public Type Type { get; set; }

        public bool IsLazy { get; set; }

        public TypeExtension Clone()
        {
            return
                new TypeExtension
                {
                    Type = Type,
                    IsLazy = IsLazy
                };
        }
    }
}
