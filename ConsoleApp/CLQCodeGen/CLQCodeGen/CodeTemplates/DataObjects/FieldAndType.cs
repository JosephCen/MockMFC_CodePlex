namespace CLQCodeGen.CodeTemplates.DataObjects
{
    public class FieldAndType
    {
        public string Name { get; set; }

        public TypeExtension TypeExtension { get; set; }

        public string TypeName =>
                        TypeExtension.Type.Name;

        public FieldAndType Clone()
        {
            return 
                new FieldAndType
                    {
                        Name = Name,
                        TypeExtension = TypeExtension.Clone()
                };
        }
    }
}
