using CLQCodeGen.CodeTemplates;
using System;

namespace CLQCodeGen.Generators
{
    public class UnitTestGenerator : MarshalByRefObject
    {
        public UnitTestGenerateResponse Generate(UnitTestGenerateRequest request)
        {
            var response = new UnitTestGenerateResponse
                               {
                                   IsSuccess = true,
                                   GeneratedText = string.Empty,
                                   ExceptionMessage = string.Empty
                               };

            try
            {
                var unitTest = new UnitTest(
                                        request.ProdAssemblyName,
                                        request.TargetTypeName,
                                        request.UnitTestFileName);

                response.GeneratedText = unitTest.TransformText();
            }
            catch (Exception ex)
            {
                response.IsSuccess = false;
                response.ExceptionMessage = ex.Message;
            }

            return response;
        }
    }
}
