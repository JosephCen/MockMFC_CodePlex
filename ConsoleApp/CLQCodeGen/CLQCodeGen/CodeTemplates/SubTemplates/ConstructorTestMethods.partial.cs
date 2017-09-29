using System.Linq;

using CLQCodeGen.Helpers;

namespace CLQCodeGen.CodeTemplates.SubTemplates
{
    public partial class ConstructorTestMethods
    {
        private readonly string _targetTypeName;

        private readonly VariableNameHelper _variableNameHelper;

        public ConstructorTestMethods(string targetTypeName)
        {
            _variableNameHelper = new VariableNameHelper();

            _targetTypeName = targetTypeName;
        }

        private string GetTargetTypeName() =>
            _targetTypeName;

        private string GetTargetInterfaceName() => 
            $"I{_targetTypeName}";

        private string GetInstanceVariableName()
        {
            var targetProgramTypeName = _variableNameHelper.SplitIntoWords(_targetTypeName).Last();

            return _variableNameHelper.LowerFirstChar(targetProgramTypeName);
        }
    }
}
