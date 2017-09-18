using CLQCodeGen.Generators;
using System;
using System.Reflection;

namespace CLQCodeGen.ProxyAdapters
{
    public class UnitTestGeneratorAdapter
    {
        public CallUnitTestGeneratorResponse CallUnitTestGeneratorInIsolateAppDomain(CallUnitTestGeneratorRequest request)
        {
            AppDomain appDomain = null;
            var response = new CallUnitTestGeneratorResponse
                               {
                                   IsSuccess = true,
                                   ErrorMessage = string.Empty
                               };

            try
            {
                var generatorType = typeof(UnitTestGenerator);
                var assembly = Assembly.GetAssembly(generatorType);

                appDomain = AppDomain.CreateDomain("AppDomain for UnitTestGenerator");

                var generatorProxy = appDomain.CreateInstanceFromAndUnwrap(assembly.Location, generatorType.FullName) as UnitTestGenerator;

                if (generatorProxy != null)
                {
                    var generatorResponse = generatorProxy.Generate(request.Request);

                    response.Response = generatorResponse;
                    if (!generatorResponse.IsSuccess)
                    {
                        response.IsSuccess = false;
                        response.ErrorMessage = "Some error happened in execution of generator.";
                    }
                }
                else
                {
                    response.IsSuccess = false;
                    response.ErrorMessage = $"Fail to create generator {generatorType.FullName} in assembly {assembly.FullName}.";
                }
            }
            finally
            {
                if (appDomain != null)
                {
                    AppDomain.Unload(appDomain);
                }
            }

            return response;
        }
    }
}
