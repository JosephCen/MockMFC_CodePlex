using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class TrimStrParameter : StringParameter
    {
        public override QueryParmType ParameterType
        {
            get { return QueryParmType.TrimStr; }
        }

        //public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        //{
        //    System.Diagnostics.Debug.Assert(this.ParameterType == qryParmDef.ParameterType);

        //    String strValue = String.Empty;

        //    strValue = parmValStr.Trim();
        //    if (String.IsNullOrEmpty(strValue))
        //        strValue = null;

        //    return strValue;
        //}
    }
}
