using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using CLQCodeGen.CodeTemplates.DataObjects;
using CLQCodeGen.Helpers;

namespace CLQCodeGen.CodeTemplates.SubTemplates
{

    public partial class TestInitialise
    {
        #region Fields

        private readonly VariableNameHelper _variableNameHelper;

        private string _targetFieldName;
        private Type _targetType;
        private IList<FieldAndType> _mockFieldAndTypeList;

        #endregion

        public TestInitialise(
                    string targetFieldName,
                    Type targetType,
                    IList<FieldAndType> mockFieldAndTypeList)
        {
            _variableNameHelper = new VariableNameHelper();

            _targetFieldName = targetFieldName;
            _targetType = targetType;
            _mockFieldAndTypeList = mockFieldAndTypeList;
        }

        private IList<FieldAndType> GetMockFieldAndTypeList() => 
            _mockFieldAndTypeList;

        private string GetTargetFieldName() =>
            _targetFieldName;

        private string GetTargetTypeName() =>
            _targetType.Name;

        private string ToInFuncName(string name) =>
            _variableNameHelper.ToInFuncName(name);
    }
}
