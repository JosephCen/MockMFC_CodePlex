using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;
using CLQCodeGen.CodeTemplates.DataObjects;

namespace CLQCodeGen.Helpers
{
    internal class TypeHelper
    {
        public IList<TypeExtension> GetDependencyList(Type targetType)
        {
            var ctorMethod = FindConstructorForTest(targetType);
            var paramTypeList = ctorMethod.GetParameters().Select(p => p.ParameterType).ToList();
            var typeExtensionList = new List<TypeExtension>();

            foreach (var type in paramTypeList)
            {
                Type innerType;

                if (IsLazyType(type, out innerType))
                {
                    typeExtensionList.Add(
                        new TypeExtension
                        {
                            IsLazy = true,
                            Type = innerType
                        });
                }
                else
                {
                    typeExtensionList.Add(
                        new TypeExtension
                        {
                            IsLazy = false,
                            Type = type
                        });
                }
            }

            return typeExtensionList;
        }

        public bool IsLazyType(Type paramType)
        {
            Type innerType;

            return IsLazyType(paramType, out innerType);
        }

        public bool IsLazyType(Type paramType, out Type innerType)
        {
            innerType = null;

            if (paramType.IsGenericType)
            {
                var genericTypeDef = paramType.GetGenericTypeDefinition();

                if (genericTypeDef == typeof(Lazy<>))
                {
                    innerType = paramType.GetGenericArguments().First();

                    return true;
                }
            }

            return false;
        }

        public IList<MethodInfo> GetMethodList(Type targetType)
        {
            var methodBindingFlags = BindingFlags.Public | BindingFlags.Instance;

            return targetType.GetMethods(methodBindingFlags).Where(m => targetType.Equals(m.DeclaringType)).ToList();
        }

        private ConstructorInfo FindConstructorForTest(Type targetType)
        {
            var ctorBindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static;
            var allCtorMethodArray = targetType.GetConstructors(ctorBindingFlags);
            var ctorMethodArray =
                    allCtorMethodArray
                        .Where(ctor => !ctor.IsStatic)
                        .Where(ctor => !ctor.GetCustomAttributes().Any(a => a is System.Runtime.CompilerServices.CompilerGeneratedAttribute))
                        .ToArray();
            var ctorMethod = ctorMethodArray.First();

            if (ctorMethodArray.Length > 1)
            {
                var nonInjectionCtorArray =
                        ctorMethodArray
                            .Where(ctor => !ctor.GetCustomAttributes().Any(a => a.GetType().Name == "LocatingServiceConstructorAttribute"))
                            .ToArray();

                if (nonInjectionCtorArray.Length > 1)
                {
                    ctorMethod = nonInjectionCtorArray.First(
                                        ctor => ctor.GetParameters().Any(
                                                    p => p.ParameterType.Name == "IDbConnectionFactory"));
                }
                else
                {
                    ctorMethod = nonInjectionCtorArray.Single();
                }
            }

            return ctorMethod;
        }
    }
}
