using System;
using System.Collections.Generic;
using System.Reflection;
using CLQCodeGen.CodeTemplates.DataObjects;
using CLQCodeGen.Helpers;

namespace CLQCodeGen.CodeTemplates.SubTemplates
{
    public partial class SetupAspectLoaderMock
    {
        private readonly Type _aspectLoaderType;
        private readonly string _mockVariableName;

        private readonly TypeHelper _typeHelper;
        private readonly VariableNameHelper _variableNameHelper;

        public SetupAspectLoaderMock(FieldAndType aspectLoaderField)
        {
            _typeHelper = new TypeHelper();
            _variableNameHelper = new VariableNameHelper();

            _aspectLoaderType = aspectLoaderField.TypeExtension.Type;
            _mockVariableName = aspectLoaderField.Name;
        }

        private string GetMockVariableName()
        {
            return _mockVariableName;
        }

        private IList<MethodInfo> _allMethods;

        private IList<MethodInfo> GetAllMethods()
        {
            if (_allMethods == null)
            {
                _allMethods = _typeHelper.GetMethodList(_aspectLoaderType);
            }

            return _allMethods;
        }

        private string GetMethodName(MethodInfo methodInfo)
        {
            return methodInfo.Name;
        }

        private Type GetGenericType(Type type, int argumentIndex)
        {
            if (type.IsGenericType)
            {
                var genericTypes = type.GenericTypeArguments;

                if (argumentIndex < genericTypes.Length)
                {
                    return genericTypes[argumentIndex];
                }
            }

            return null;
        }

        private Type GetReturnListGenericType(MethodInfo methodInfo)
        {
            return GetGenericType(methodInfo.ReturnType, 0);
        }

        private bool GetIsGenericList(Type type)
        {
            return type.IsGenericType && (type.Name == "IList`1" || type.Name == "List`1");
        }

        private bool GetIsReturnGenericList(MethodInfo methodInfo)
        {
            return GetIsGenericList(methodInfo.ReturnType);
        }

        private bool GetIsSingleReferenceParameter(MethodInfo methodInfo)
        {
            var parameterInfos = methodInfo.GetParameters();

            if (parameterInfos.Length == 1 && GetIsGenericList(parameterInfos[0].ParameterType))
            {
                var parameterGenericType = GetGenericType(parameterInfos[0].ParameterType, 0);

                if (parameterGenericType.Name.Contains("Reference"))
                {
                    return true;
                }
            }

            return false;
        }

        private Type GetParameterGenericType(MethodInfo methodInfo, int parameterIndex)
        {
            var parameterInfo = methodInfo.GetParameters()[parameterIndex];

            return GetGenericType(parameterInfo.ParameterType, 0);
        }

        private string GetVariableNameFromType(Type type)
        {
            return _variableNameHelper.LowerFirstChar(type.Name.Substring(type.IsInterface ? 1 : 0));
        }
    }
}
