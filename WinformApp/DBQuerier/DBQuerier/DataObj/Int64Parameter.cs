using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class Int64Parameter : BaseQueryParameter
    {
        #region IQueryParameter Members

        public override QueryParmType ParameterType
        {
            get { return QueryParmType.Int64; }
        }

        public override DbType DbParameterType
        {
            get { return DbType.Int64; }
        }

        public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        {
            Int64 int64Value = 0l;
            BaseSearchParameter searchParm = null;

            int64Value = Int64.Parse(parmValStr);
            searchParm = new EqualSearchParameter(qryParmDef);
            searchParm.ParameterValue = int64Value;

            return searchParm;
        }

        #endregion
    }
}
