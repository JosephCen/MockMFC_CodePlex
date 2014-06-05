using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class IntegerParameter : BaseQueryParameter
    {

        #region BaseQueryParameter Members

        public override QueryParmType ParameterType
        {
            get { return QueryParmType.Integer; }
        }

        public override DbType DbParameterType
        {
            get { return DbType.Int32; }
        }

        public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        {
            Int32 intValue = 0;
            BaseSearchParameter searchParm = null;

            intValue = Int32.Parse(parmValStr);
            searchParm = new EqualSearchParameter(qryParmDef);
            searchParm.ParameterValue = intValue;

            return searchParm;
        }

        #endregion
    }
}
