using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;

using DBQuerier.DatabaseObj;


namespace DBQuerier.DataObj
{
    class StringParameter : BaseQueryParameter
    {
        #region BaseQueryParameter Members

        public override QueryParmType ParameterType
        {
            get { return QueryParmType.String; }
        }

        public override DbType DbParameterType
        {
            get { return DbType.String; }
        }

        public override BaseSearchParameter ParseParameterValue(QueryParmDef qryParmDef, String parmValStr)
        {
            BaseSearchParameter searchParm = null;

            System.Diagnostics.Debug.Assert(this.ParameterType == qryParmDef.ParameterType);

            if (parmValStr.Contains('%'))
                searchParm = new LikeSearchParameter(qryParmDef);
            else
                searchParm = new EqualSearchParameter(qryParmDef);
            searchParm.ParameterValue = parmValStr;

            return searchParm;
        }

        #endregion
    }
}
