using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text.RegularExpressions;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    abstract class BaseQueryParameter
    {
        private static Regex mS_ParmStrWithPat;

        static BaseQueryParameter()
        {
            String pattern = "^\\s*"
                           + "(?(\")"
                           // match text like ' "a,bc", "def"'
                           + "\"([^\"]+)\"\\s*(?:,\\s*\"([^\"]+)\"\\s*)*" // with quot
                           + "|"
                           // match text like ' abc, def'
                           + "([^,]+?)\\s*(?:,\\s*([^,]+?)\\s*)*" // without quot
                           + ")"
                           + "$";

            mS_ParmStrWithPat = new Regex(pattern, RegexOptions.Compiled | RegexOptions.Singleline);
        }

        public abstract QueryParmType ParameterType
        { get; }

        public abstract DbType DbParameterType
        { get; }

        public abstract BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr);

        public SearchParameterSet ParseParameter(QueryParmDef qryParmDef, String parmStr)
        {
            SearchParameterSet parmSet = new SearchParameterSet(qryParmDef);
            Match m = mS_ParmStrWithPat.Match(parmStr);

            if (!String.IsNullOrEmpty(parmStr))
            {
                if (m.Success)
                {
                    Group g = null;

                    g = (m.Groups[1].Success ? m.Groups[1] : m.Groups[3]);
                    if (g.Success)
                        parmSet.AddSearchParameter(ParseParameterValue(qryParmDef, g.Value));

                    g = (m.Groups[2].Success ? m.Groups[2] : m.Groups[4]);
                    if (g.Success)
                    {
                        foreach (Capture c in g.Captures)
                            parmSet.AddSearchParameter(ParseParameterValue(qryParmDef, c.Value));
                    }
                }
                else
                {
                    throw new Exception(String.Format("Cannot parse {0} for parameter {1}!", parmStr, qryParmDef.Name));
                }
            }

            return parmSet;
        }
    }

    static class QueryParameter
    {
        private static IDictionary<QueryParmType, BaseQueryParameter> mS_QryParmDic;

        static QueryParameter()
        {
            BaseQueryParameter[] qryParmArr = null;

            qryParmArr = new BaseQueryParameter[]{
                                new IntegerParameter(),
                                new Int64Parameter(),
                                new DateTimeParameter(),
                                new GuidParametercs(),
                                new StringParameter(),
                                new TrimStrParameter() };
            mS_QryParmDic = qryParmArr.ToDictionary(p => p.ParameterType);
        }

        public static BaseQueryParameter GetQueryParameter(QueryParmDef qryParmDef)
        {
            return mS_QryParmDic[qryParmDef.ParameterType];
        }
    }
}
