using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class DateTimeParameter : BaseQueryParameter
    {
        #region BaseQueryParameter Members

        public override QueryParmType ParameterType
        {
            get { return QueryParmType.DateTime; }
        }

        public override DbType DbParameterType
        {
            get { return DbType.DateTime; }
        }

        public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        {
            DateTime dtValue = DateTime.MinValue;
            BaseSearchParameter searchParm = null;

            dtValue = DateTime.Parse(parmValStr);
            searchParm = new EqualSearchParameter(qryParmDef);
            searchParm.ParameterValue = dtValue;

            return searchParm;
        }

        #endregion
    }
}
