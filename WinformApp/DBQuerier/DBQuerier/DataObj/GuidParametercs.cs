using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class GuidParametercs : BaseQueryParameter
    {

        #region BaseQueryParameter Members

        public override QueryParmType ParameterType
        {
            get { return QueryParmType.Guid; }
        }

        public override DbType DbParameterType
        {
            get { return DbType.Guid; ; }
        }

        public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        {
            Guid guidValue = Guid.Empty;
            BaseSearchParameter searchParm = null;

            guidValue = new Guid(parmValStr);
            searchParm = new EqualSearchParameter(qryParmDef);
            searchParm.ParameterValue = guidValue;

            return searchParm;
        }

        #endregion
    }
}
