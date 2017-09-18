using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection;

namespace CLQCodeGen.Helpers
{
    internal class TypeHelper
    {
        private ConstructorInfo FindConstructorForTest(Type targetType)
        {
            var ctorBindingFlags = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static;
            var ctorMethodArray = targetType.GetConstructors(ctorBindingFlags);
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

        public IList<Type> GetDependencyList(Type targetType)
        {
            var ctorMethod = FindConstructorForTest(targetType);

            return ctorMethod.GetParameters().Select(p => p.ParameterType).ToList();
        }

        public IList<MethodInfo> GetMethodList(Type targetType)
        {
            var methodBindingFlags = BindingFlags.Public | BindingFlags.Instance;

            return targetType.GetMethods(methodBindingFlags).Where(m => targetType.Equals(m.DeclaringType)).ToList();
        }
    }
}
